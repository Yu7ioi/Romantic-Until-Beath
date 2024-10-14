// 世界从11003开始
#define MAX_ROOM					50
#define MAX_MAPNAME					50
#define MAX_ROOM_PLAYER_ID			20
#define Dialog_DeathMatch_Title		"Death Match"
// #define Dialog_DeathMatch			300
// #define Dialog_DeathMatch_R_C		Dialog_DeathMatch + 1//rc = room create = 房间创建
// #define Dialog_DeathMatch_S_R		Dialog_DeathMatch + 2//sr= search room = 搜索房间
// #define Dialog_DeathMatch_R_D		Dialog_DeathMatch + 3//rd = room details = 房间详情
enum deathmatchtype {
    rmax, //人头记录
    rplayer, //人数记录
    rmode, //中途进入模式记录
    rmapname[MAX_PLAYER_NAME], //地图名记录
    rcreator[MAX_PLAYER_NAME], //创建者记录
    rcountdown, //倒计时
    rtimer, //计时器
    rmapid, //地图id
    rdeath, //已拿人头总数
    rplayerid[MAX_ROOM_PLAYER_ID], //记录房间内玩家的id
    rstate //状态记录 -1 = 未创建 0 = 未开始, 1 = 已开始, 2 = 结束中
};
enum deathmatchgametype {
    g_rid,
    g_page,
    g_mapid,
    g_ikill,
    g_ideath,
    PlayerText:g_draw,
    g_anticheat //2020.4.1写 防止数据异常的误封造成
};
new AimbotWarnings[MAX_PLAYERS]; //2020.4.1写
new DeathMatch_Map[MAX_MAPNAME][MAX_PLAYER_NAME];
new DeathMatch_Room[MAX_ROOM][deathmatchtype], DeathMatch_Game[MAX_PLAYERS][deathmatchgametype];
new MapGuns[MAX_ROOM][5], Float:MapSpawns[MAX_ROOM][10][5], MapHP[MAX_ROOM], DeathMatchCount;
new MapInterior[MAX_ROOM];
stock DeathMatch_LoadMapData(mapid) {
    new position[128];
    // PS:linux环境下 地图名请不要使用中文 不然会读不到
    format(position, sizeof(position), "DeathMatch/Maps/%s.txt", DeathMatch_Map[mapid]);
    if(fexist(position) == 1) {
        for (new i = 0; i < 10; i++) {
            if(i < 5) {
                MapGuns[mapid][i] = 0;
            }
            MapSpawns[mapid][i][4] = 0;
        }
        new File:m = fopen(position, io_read), info[256], msg[128], idx = 0, id;
        while (fread(m, info)) {
            msg = strtok(info, idx);
            if(mk_strcmp(msg, "weapon") == 0) {
                for (new i = 0; i < 5; i++) {
                    if(MapGuns[mapid][i] == 0) {
                        id = i;
                        i = 6;
                    }
                }
                msg = strtok(info, idx);
                MapGuns[mapid][id] = strval(msg);
            }
            if(mk_strcmp(msg, "spawn") == 0) {
                for (new i = 0; i < 10; i++) {
                    if(MapSpawns[mapid][i][4] == 0) {
                        id = i;
                        i = 11;
                    }
                }
                msg = strtok(info, idx);
                MapSpawns[mapid][id][0] = floatstr(msg);
                msg = strtok(info, idx);
                MapSpawns[mapid][id][1] = floatstr(msg);
                msg = strtok(info, idx);
                MapSpawns[mapid][id][2] = floatstr(msg);
                msg = strtok(info, idx);
                MapSpawns[mapid][id][3] = floatstr(msg);
                MapSpawns[mapid][id][4] = 1;
            }
            if(mk_strcmp(msg, "hp") == 0) {
                msg = strtok(info, idx);
                MapHP[mapid] = strval(msg);
            }
            if(mk_strcmp(msg, "interior") == 0) {
                msg = strtok(info, idx);
                MapInterior[mapid] = strval(msg);
            }
            idx = 0;
        }
        fclose(m);
    }
}
stock Initialize_DeathMatch() {
    DeathMatchCount = 0;
    for (new i = 0; i < MAX_ROOM; i++) {
        DeathMatch_Room_Delete(i);
    }
    for (new i = 0; i < MAX_MAPNAME; i++) {
        format(DeathMatch_Map[i], 2, "0");
    }
    new position[128];
    format(position, sizeof(position), "DeathMatch/DeathMatch.txt");
    if(fexist(position) == 1) {
        new File:dm = fopen(position, io_read), info[256], msg[128], idx = 0, mapid = -1;
        while (fread(dm, info)) {
            msg = strtok(info, idx);
            if(mk_strcmp(msg, "addmap") == 0) {
                msg = strtok(info, idx);
                for (new i = 0; i < MAX_MAPNAME; i++) {
                    if(mk_strcmp(DeathMatch_Map[i], "0") == 0) {
                        mapid = i;
                        i = MAX_MAPNAME + 1;
                    }
                }
                if(mapid == -1) {
                    printf("DM地图已达到上限请修改最大值(MAX_MAPNAME from DeathMatch.inc)");
                }
                format(DeathMatch_Map[mapid], MAX_PLAYER_NAME, msg);
                DeathMatch_LoadMapData(mapid);
                mapid = -1;
                DeathMatchCount++;
            }
            idx = 0;
        }
        printf("DeathMatch * 共加载[%d/%d]个DM地图.", DeathMatchCount, MAX_MAPNAME);
        fclose(dm);
    } else {
        printf("找不到数据库文件(DeathMatch.txt from DeathMatch.inc)");
    }
}
stock DeathMatch_OnPlayerWeaponShot(const playerid, const hittype, const hitid) {
    if(hittype == BULLET_HIT_TYPE_PLAYER && IsPlayerConnected(hitid) == 1) {
        if(IsPlayerDeathMatch(playerid) == 0 && IsPlayerDeathMatch(hitid) == 1) {
            FuckAnitCheat(playerid, "乱入DM", 12);
            // new msg[128];
            // format(iXp[playerid][State], MAX_PLAYER_NAME, "[监禁中]");
            // iXp[playerid][jail_time] = 600;
            // Jail_Lockup(playerid);
            // format(msg, sizeof msg, "系统 * %s(%d) 被关进了监狱里, 原因:乱入DM.", GetName(playerid), playerid);
            // SCMToAll(Color_Red, msg);
        }
    }
    return 1;
}
CMD:dm(playerid, cmdtext[]) {
    if(GetPlayerCreditpoints(playerid) <= 95) return SendClientMessage(playerid, Color_Yellow, "[DM]您的游戏信誉分过低,请健康游戏,将会自动补回");
    new tmp[128], idx;
    tmp = strtok(cmdtext, idx);
    if(mk_strcmp(tmp, "start", true) == 0) {
        if(DeathMatch_Game[playerid][g_rid] == -1) {
            SCM(playerid, Color_White, "[DM]你未加入任何房间.");
            return 1;
        }
        if(mk_strcmp(GetName(playerid), DeathMatch_Room[DeathMatch_Game[playerid][g_rid]][rcreator]) != 0) {
            SCM(playerid, Color_White, "[DM]你不是创建者.");
            return 1;
        }
        DeathMatch_Room_Start(DeathMatch_Game[playerid][g_rid]);
        return 1;
    }
    if(mk_strcmp(tmp, "leave", true) == 0) {
        DeathMatch_Leave(playerid);
        return 1;
    }
    if(mk_strcmp(tmp, "list", true) == 0) {
        ShowPlayerDeathMatchDialog(playerid);
        return 1;
    }
    SCM(playerid, Color_White, "[DM]/dm list (列表/创建房间/搜索房间)");
    SCM(playerid, Color_White, "[DM]/dm start (开始)");
    SCM(playerid, Color_White, "[DM]/dm leave (离开房间)");
    return 1;
}

stock DeathMatch_Leave(const playerid) {
    if(DeathMatch_Game[playerid][g_rid] == -1) {
        SCM(playerid, Color_White, "[DM]你未加入任何房间.");
        return 1;
    }
    if(mk_strcmp(DeathMatch_Room[DeathMatch_Game[playerid][g_rid]][rcreator], GetName(playerid)) == 0 && DeathMatch_Room[DeathMatch_Game[playerid][g_rid]][rstate] == 0) {
        new tmp[128];
        format(tmp, sizeof tmp, "[DM]房间已关闭,原因:创建者退出.");
        SCMToRoom(DeathMatch_Game[playerid][g_rid], tmp);
        DeathMatch_Room_Delete(DeathMatch_Game[playerid][g_rid]);
        return 1;
    }
    DeathMatch_Room_Quit(playerid);
    SCM(playerid, Color_White, "[DM]你离开了竞技房间.");
    return 1;
}

stock ShowPlayerDeathMatchDialog(const playerid) {
    new msg[128];
    format(msg, sizeof msg, "创建房间\n搜索房间");
    Dialog_Show(playerid, Dialog_DeathMatch, DIALOG_STYLE_LIST, Dialog_DeathMatch_Title, msg, "选择", "关闭");
    return 1;
}
stock SearchForIdleRoom() {
    for (new i = 0; i < MAX_ROOM; i++) {
        if(DeathMatch_Room[i][rstate] == -1) {
            return i;
        }
    }
    return -1;
}
stock IsMapNameExist(const mapname[]) {
    for (new i = 0; i < MAX_MAPNAME; i++) {
        if(strlen(DeathMatch_Map[i]) > 2 && mk_strcmp(DeathMatch_Map[i], mapname) == 0) {
            return i;
        }
    }
    return -1;
}
stock Initialize_DeathMatch_Game(const playerid) {
    DeathMatch_Game[playerid][g_rid] = -1;
    DeathMatch_Game[playerid][g_mapid] = -1;
    DeathMatch_Game[playerid][g_ikill] = 0;
    DeathMatch_Game[playerid][g_ideath] = 0;
}
stock DeathMatch_PlayerLogin(const playerid) {
    AimbotWarnings[playerid] = 0;
    DeathMatch_Game[playerid][g_anticheat] = 0;
    DeathMatch_Game[playerid][g_draw] = CreatePlayerTextDraw(playerid, 280.0, 5.0, "~~");
    PlayerTextDrawColor(playerid, DeathMatch_Game[playerid][g_draw], 0x0088FFFF);
    PlayerTextDrawBackgroundColor(playerid, DeathMatch_Game[playerid][g_draw], 0x000000FF);
    PlayerTextDrawLetterSize(playerid, DeathMatch_Game[playerid][g_draw], 0.4, 2.00);
    PlayerTextDrawFont(playerid, DeathMatch_Game[playerid][g_draw], 1);
    PlayerTextDrawSetOutline(playerid, DeathMatch_Game[playerid][g_draw], 1);
    PlayerTextDrawSetProportional(playerid, DeathMatch_Game[playerid][g_draw], 1);
    PlayerTextDrawSetShadow(playerid, DeathMatch_Game[playerid][g_draw], 1);
    PlayerTextDrawAlignment(playerid, DeathMatch_Game[playerid][g_draw], 0);
}
stock DeathMatch_OnPlayerDisconnect(const playerid) {
    if(DeathMatch_Game[playerid][g_rid] != -1) {
        DeathMatch_Room_Quit(playerid);
    }
    PlayerTextDrawDestroy(playerid, DeathMatch_Game[playerid][g_draw]);
}
stock DeathMatch_OnPlayerSpawn(const playerid) {
    if(DeathMatch_Game[playerid][g_rid] != -1) {
        DeathMatch_SetGunSpawn(playerid);
    }
}
stock DeathMatch_OnPlayerDeath(const playerid, const killerid) {
    if(DeathMatch_Game[playerid][g_rid] != -1 && DeathMatch_Game[playerid][g_mapid] != -1 && DeathMatch_Room[DeathMatch_Game[playerid][g_rid]][rstate] == 1) {
        DeathMatch_Game[playerid][g_anticheat] = 0;
        DeathMatch_Game[playerid][g_ideath]++;
        if(killerid != INVALID_PLAYER_ID) {
            DeathMatch_Game[killerid][g_ikill]++;
        }
        DeathMatch_Room[DeathMatch_Game[playerid][g_rid]][rdeath]++;
        if(DeathMatch_Room[DeathMatch_Game[playerid][g_rid]][rdeath] == DeathMatch_Room[DeathMatch_Game[playerid][g_rid]][rmax]) {
            DeathMatch_Room[DeathMatch_Game[playerid][g_rid]][rstate] = 2;
            DeathMatch_End(DeathMatch_Game[playerid][g_rid]);
        }
        DeathMatch_Update_Draw(DeathMatch_Game[playerid][g_rid]);
    }
}
stock DeathMatch_SetGunSpawn(const playerid) {
    DeathMatch_Game[playerid][g_anticheat] = 0;
    ResetPlayerWeapons(playerid); //清空玩家的武器 2020.4.1写
    new mid = DeathMatch_Game[playerid][g_mapid], r = random(10);
    SetPlayerVirtualWorld(playerid, 11003 + mid);
    SetPlayerInterior(playerid, MapInterior[mid]);
    SetPlayerPos(playerid, MapSpawns[mid][r][0], MapSpawns[mid][r][1], MapSpawns[mid][r][2]);
    SetPlayerFacingAngle(playerid, MapSpawns[mid][r][3]);
    SetPlayerHealth(playerid, 1000000);
    SetPlayerArmour(playerid, 100.0);
    SetTimerEx("DeathMatch_TeleportDelay", 1000, false, "iii", playerid, r, mid);
}
forward DeathMatch_TeleportDelay(playerid, randomid, mapid);
public DeathMatch_TeleportDelay(playerid, randomid, mapid) {
    PlayerInfo[playerid][lastXMoved] = MapSpawns[mapid][randomid][0];
    PlayerInfo[playerid][lastYMoved] = MapSpawns[mapid][randomid][1];
    PlayerInfo[playerid][lastZMoved] = MapSpawns[mapid][randomid][2];
    SetPlayerPos(playerid, MapSpawns[mapid][randomid][0], MapSpawns[mapid][randomid][1], MapSpawns[mapid][randomid][2]);
    SetPlayerFacingAngle(playerid, MapSpawns[mapid][randomid][3]);
    PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
    for (new i = 0; i < 5; i++) {
        GivePlayerWeapon(playerid, MapGuns[mapid][i], 9527);
    }
    SetPlayerHealth(playerid, MapHP[mapid]);
    SetPlayerArmour(playerid, 100.0);
    // SetTimerEx("DeathMatch_AntiCheat", 1500, false, "i", playerid); //加一点延迟不然反应太快了可能
    DeathMatch_Game[playerid][g_anticheat] = 1;
}
// forward DeathMatch_AntiCheat(playerid);
// public DeathMatch_AntiCheat(playerid) {
//     // DeathMatch_Game[playerid][g_anticheat] = 1;
//     return 1;
// }
forward DeathMatch_Room_Start_Countdown(roomid);
public DeathMatch_Room_Start_Countdown(roomid) {
    if(DeathMatch_Room[roomid][rcountdown] == 0) {
        KillTimer(DeathMatch_Room[roomid][rtimer]);
        DeathMatch_Room[roomid][rstate] = 1;
        for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++) {
            if(IsPlayerConnected(i) == 1 && DeathMatch_Game[i][g_rid] == roomid) {
                DeathMatch_Game[i][g_mapid] = DeathMatch_Room[roomid][rmapid];
                GameTextForPlayer(i, "~b~start!", 850, 3);
                PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
                DeathMatch_SetGunSpawn(i);
            }
        }
    } else {
        new msg[12];
        format(msg, sizeof msg, "~r~%d", DeathMatch_Room[roomid][rcountdown]);
        for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++) {
            if(IsPlayerConnected(i) == 1 && DeathMatch_Game[i][g_rid] == roomid) {
                GameTextForPlayer(i, msg, 850, 3);
                PlayerPlaySound(i, 1056, 0.0, 0.0, 0.0);
            }
        }
        DeathMatch_Room[roomid][rcountdown]--;
    }
}
stock DeathMatch_Room_Start(const roomid) {
    DeathMatch_Room[roomid][rcountdown] = 10;
    DeathMatch_Room[roomid][rmapid] = IsMapNameExist(DeathMatch_Room[roomid][rmapname]);
    SCMToRoom(roomid, "[DM]进入倒计时.");
    DeathMatch_Room[roomid][rtimer] = SetTimerEx("DeathMatch_Room_Start_Countdown", 950, 1, "i", roomid);
}
stock DeathMatch_End(const id) {
    new msg[128];
    for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++) {
        if(IsPlayerConnected(i) == 1 && DeathMatch_Game[i][g_rid] == id) {
            SCM(i, Color_White, "|_____________________________________________________________|");
            format(msg, sizeof msg, "[DM]已结束, 杀敌:[%d], 死亡:[%d]", DeathMatch_Game[i][g_ikill], DeathMatch_Game[i][g_ideath]);
            SCM(i, Color_White, msg);
            SCM(i, Color_White, "|_____________________________________________________________|");
            Initialize_Player_DeathMatch(i);
        }
    }
    DeathMatch_Room_Delete(id);
}
stock Initialize_Player_DeathMatch(const playerid) {
    Initialize_DeathMatch_Game(playerid);
    PlayerTextDrawHide(playerid, DeathMatch_Game[playerid][g_draw]);
    // new Float:health;
    // GetPlayerHealth(playerid, health);
    // if(health) SpawnPlayer(playerid);
    DeathMatch_Game[playerid][g_anticheat] = 0;
    SpawnPlayer(playerid);
    //最后一个人死的话会被fakekill误封 //还是让玩家自己离开这边或者是判断生命值生成
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerInterior(playerid, 0);
}
stock DeathMatch_Room_Delete(const id) {
    if(DeathMatch_Room[id][rstate] == -1) return 1;
    for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++) {
        if(IsPlayerConnected(i) == 1 && DeathMatch_Game[i][g_rid] == id) {
            DeathMatch_Room_Quit(i);
        }
    }
    DeathMatch_Room[id][rmax] = 0;
    DeathMatch_Room[id][rplayer] = 0;
    DeathMatch_Room[id][rmode] = 0;
    format(DeathMatch_Room[id][rmapname], MAX_PLAYER_NAME, "unknown");
    format(DeathMatch_Room[id][rcreator], MAX_PLAYER_NAME, "unknown");
    DeathMatch_Room[id][rdeath] = 0;
    DeathMatch_Room[id][rstate] = -1;
    return 1;
}
stock DeathMatch_Room_Create(const playerid, const id, const mode, const mapname[], const max) {
    DeathMatch_Room[id][rmax] = max;
    DeathMatch_Room[id][rplayer] = 0;
    DeathMatch_Room[id][rmode] = mode;
    format(DeathMatch_Room[id][rmapname], MAX_PLAYER_NAME, mapname);
    format(DeathMatch_Room[id][rcreator], MAX_PLAYER_NAME, GetName(playerid));
    DeathMatch_Room[id][rdeath] = 0;
    DeathMatch_Room[id][rstate] = 0;
    for (new i = 0; i < MAX_ROOM_PLAYER_ID; i++) {
        DeathMatch_Room[id][rplayerid][i] = -1;
    }
}
stock DeathMatch_Update_Draw(const roomid) {
    new msg[44];
    for (new i = 0; i < MAX_ROOM_PLAYER_ID; i++) {
        if(DeathMatch_Room[roomid][rplayerid][i] != -1) {
            format(msg, sizeof msg, "~p~MaxKill: ~w~%d/%d", DeathMatch_Room[roomid][rdeath], DeathMatch_Room[roomid][rmax]);
            PlayerTextDrawSetString(DeathMatch_Room[roomid][rplayerid][i], DeathMatch_Game[DeathMatch_Room[roomid][rplayerid][i]][g_draw], msg);
        }
    }
}
stock DeathMatch_Room_Join(const playerid, const roomid) {
    new id = -1;
    for (new i = 0; i < MAX_ROOM_PLAYER_ID; i++) {
        if(DeathMatch_Room[roomid][rplayerid][i] == -1) {
            id = i;
            i = MAX_ROOM_PLAYER_ID + 1;
        }
    }
    if(id == -1) return 0;
    DeathMatch_Room[roomid][rplayerid][id] = playerid;
    new msg[128];
    DeathMatch_Room[roomid][rplayer]++;
    DeathMatch_Game[playerid][g_rid] = roomid;
    DeathMatch_Game[playerid][g_ideath] = 0;
    DeathMatch_Game[playerid][g_ikill] = 0;
    format(msg, sizeof(msg), "~p~MaxKill: ~w~0/%d", DeathMatch_Room[roomid][rmax]);
    PlayerTextDrawSetString(playerid, DeathMatch_Game[playerid][g_draw], msg);
    PlayerTextDrawShow(playerid, DeathMatch_Game[playerid][g_draw]);
    format(msg, sizeof msg, "[DM]%s 加入了本房间.", GetName(playerid));
    SCMToRoom(roomid, msg);
    // DeathMatch_SetGunSpawn(playerid); //2020.4.1新增 加入房间后直接拉到出生点
    return 1;
}
stock DeathMatch_Room_Quit(const playerid) {
    DeathMatch_Game[playerid][g_anticheat] = 0;
    new dmrid = DeathMatch_Game[playerid][g_rid];
    for (new i = 0; i < MAX_ROOM_PLAYER_ID; i++) {
        if(DeathMatch_Room[dmrid][rplayerid][i] == playerid) {
            DeathMatch_Room[dmrid][rplayerid][i] = -1;
            i = MAX_ROOM_PLAYER_ID + 1;
        }
    }
    Initialize_Player_DeathMatch(playerid);
    DeathMatch_Room[dmrid][rplayer]--;
    if(DeathMatch_Room[dmrid][rplayer] == 0) {
        DeathMatch_Room_Delete(dmrid);
    } else {
        new msg[128];
        format(msg, sizeof msg, "[DM]%s 离开了本房间.", GetName(playerid));
        SCMToRoom(dmrid, msg);
    }
}
stock IsPlayerDeathMatch(const playerid) {
    if(DeathMatch_Game[playerid][g_rid] != -1 && DeathMatch_Game[playerid][g_mapid] != -1) return 1;
    return 0;
}
stock getPlayerACstate(const playerid) { //给反作弊调用的 Rogue-AC
    return DeathMatch_Game[playerid][g_anticheat];
}
stock SCMToRoom(const roomid, const msg[]) {
    for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++) {
        if(IsPlayerConnected(i) == 1 && DeathMatch_Game[i][g_rid] == roomid) {
            SCM(i, Color_White, msg);
        }
    }
}
stock GetRoomCount() {
    new count = 0;
    for (new i = 0; i < MAX_ROOM; i++) {
        if(DeathMatch_Room[i][rstate] != -1) {
            count++;
        }
    }
    return count;
}
stock ShowSearchRoomDialog(const playerid, const page) {
    new msg[1024], title[128];
    format(title, sizeof title, "%s (%d/%d)", Dialog_DeathMatch_Title, page, GetMaxPage(GetRoomCount()));
    format(msg, sizeof msg, "上一页");
    for (new i = (page - 1) * RACE_PAGES; i < page * RACE_PAGES; i++) {
        if(DeathMatch_Room[i][rstate] != -1) {
            format(msg, sizeof msg, "%s\nID %d %s %s", msg, i, DeathMatch_Room[i][rmapname], DeathMatch_Room[i][rcreator]);
        }
    }
    format(msg, sizeof msg, "%s\n下一页", msg);
    Dialog_Show(playerid, Dialog_DeathMatch_S_R, DIALOG_STYLE_LIST, title, msg, "选择", "返回");
}
// forward DeathMatch_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
// public DeathMatch_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
// {
// 	if(dialogid == Dialog_DeathMatch)
// 	{
// 		if(!response)
// 		{
// 			return 1;
// 		}
// 		if(listitem == 0)
// 		{
// 			new msg[1024];
// 			for(new i = 0; i < DeathMatchCount; i++)
// 			{
// 				format(msg, sizeof msg, "%s%s\n", msg, DeathMatch_Map[i]);
// 			}
// 			format(msg, sizeof msg, "%s\n________________________________________________\n请输入 [地图名] [是否允许中途进入(0 = 不可以, 1 = 可以)] [人头]", msg);
// 			ShowPlayerDialog(playerid, Dialog_DeathMatch_R_C, DIALOG_STYLE_INPUT, Dialog_DeathMatch_Title, msg, "创建", "返回");
// 			return 1;
// 		}
// 		if(listitem == 1)
// 		{
// 			DeathMatch_Game[playerid][g_page] = 1;
// 			ShowSearchRoomDialog(playerid, 1);
// 			return 1;
// 		}
// 		return 1;
// 	}
// 	new msg[128], idx, id;
// 	if(dialogid == Dialog_DeathMatch_S_R)
// 	{
// 		if(!response)
// 		{
// 			ShowPlayerDeathMatchDialog(playerid);
// 			return 1;
// 		}
// 		msg = strtok(inputtext, idx);
// 		if(mk_strcmp(msg, "上一页") == 0)
// 		{
// 			if(DeathMatch_Game[playerid][g_page] == 1)
// 			{
// 				SCM(playerid, Color_White, "[DM]这是第一页.");
// 				return 1;
// 			}
// 			DeathMatch_Game[playerid][g_page] --;
// 			ShowSearchRoomDialog(playerid, DeathMatch_Game[playerid][g_page]);
// 			return 1;
// 		}
// 		if(mk_strcmp(msg, "下一页") == 0)
// 		{
// 			if(DeathMatch_Game[playerid][g_page] >= GetMaxPage(GetRoomCount()))
// 			{
// 				SCM(playerid, Color_White, "[DM]这是最后一页.");
// 				return 1;
// 			}
// 			DeathMatch_Game[playerid][g_page] ++;
// 			ShowSearchRoomDialog(playerid, DeathMatch_Game[playerid][g_page]);
// 			return 1;
// 		}
// 		if(mk_strcmp(msg, "ID") == 0)
// 		{
// 			msg = strtok(inputtext ,idx);
// 			id = strval(msg);
// 			new string[256], dstate[2][8] = {"准备中","已开始"};
// 			format(string, sizeof string, "房间名:[%s]\n创建者:[%s]\n当前人数:[%d]\n杀戮:[%d]\n状态:[%s]\n_____________________________________\n",
// 			DeathMatch_Room[id][rmapname], DeathMatch_Room[id][rcreator], DeathMatch_Room[id][rplayer], DeathMatch_Room[id][rmax], dstate[DeathMatch_Room[id][rstate]]);
// 			if(DeathMatch_Room[id][rstate] == 0)
// 			{
// 				format(string, sizeof string, "%s{00FF00}加入 %d", string, id);
// 			}
// 			if(DeathMatch_Room[id][rstate] == 1 && DeathMatch_Room[id][rmode] == 1)
// 			{
// 				format(string, sizeof string, "%s{00FF00}加入 %d", string, id);
// 			}
// 			ShowPlayerDialog(playerid, Dialog_DeathMatch_R_D, DIALOG_STYLE_LIST, Dialog_DeathMatch_Title, string, "选择", "返回");
// 			return 1;
// 		}
// 		return 1;
// 	}
// 	if(dialogid == Dialog_DeathMatch_R_D)
// 	{
// 		if(!response)
// 		{
// 			ShowSearchRoomDialog(playerid, DeathMatch_Game[playerid][g_page]);
// 			return 1;
// 		}
// 		msg = strtok(inputtext ,idx);
// 		if(mk_strcmp(msg, "加入") == 0)
// 		{
// 			msg = strtok(inputtext ,idx);
// 			id = strval(msg);
// 			if(DeathMatch_Room[id][rstate] == 1 && DeathMatch_Room[id][rmode] == 0)
// 			{
// 				SCM(playerid, Color_White, "[DM]该房间不允许中途加入.");
// 				return 1;
// 			}
// 			if(DeathMatch_Game[playerid][g_rid] != -1)
// 			{
// 				SCM(playerid, Color_White, "[DM]你已经加入了一个房间.");
// 				return 1;
// 			}
// 			if(DeathMatch_Room_Join(playerid, id) == 0)
// 			{
// 				SCM(playerid, Color_White, "[DM]该房间人数已满.");
// 			}
// 			if(DeathMatch_Room[id][rstate] == 1)
// 			{
// 				DeathMatch_Game[playerid][g_mapid] = DeathMatch_Room[id][rmapid];
// 				DeathMatch_SetGunSpawn(playerid);
// 			}
// 			return 1;
// 		}
// 		return 1;
// 	}
// 	if(dialogid == Dialog_DeathMatch_R_C)
// 	{
// 		if(!response)
// 		{
// 			ShowPlayerDeathMatchDialog(playerid);
// 			return 1;
// 		}
// 		if(DeathMatch_Game[playerid][g_rid] != -1)
// 		{
// 			SCM(playerid, Color_White, "[DM]请先退出房间.");
// 			return 1;
// 		}
// 		msg = strtok(inputtext, idx);
// 		if(!strlen(msg))
// 		{
// 			SCM(playerid, Color_White, "[DM]地图名不能为空.");
// 			return 1;
// 		}
// 		new mname[MAX_PLAYER_NAME];
// 		format(mname, sizeof mname, msg);
// 		if(IsMapNameExist(mname) == -1)
// 		{
// 			SCM(playerid, Color_White, "[DM]错误的地图名.");
// 			return 1;
// 		}
// 		msg = strtok(inputtext, idx);
// 		if(!strlen(msg))
// 		{
// 			SCM(playerid, Color_White, "[DM]房间模式不能为空.");
// 			return 1;
// 		}
// 		new mode = strval(msg);
// 		if(mode < 0 || mode > 1)
// 		{
// 			SCM(playerid, Color_White, "[DM]错误的模式.");
// 			return 1;
// 		}
// 		id = SearchForIdleRoom();
// 		if(id == -1)
// 		{
// 			SCM(playerid, Color_White, "[DM]当前房间已满.");
// 			return 1;
// 		}
// 		msg = strtok(inputtext ,idx);
// 		if(!strlen(msg))
// 		{
// 			SCM(playerid, Color_White, "[DM]人头数量不能为空.");
// 			return 1;
// 		}
// 		new rmaxs = strval(msg);
// 		if(rmaxs < 10 || rmaxs > 300)
// 		{
// 			SCM(playerid, Color_White, "[DM]错误的人头数量,范围: 10 - 300");
// 			return 1;
// 		}
// 		new modename[2][8] = {"不允许","允许"};
// 		DeathMatch_Room_Create(playerid, id, mode, mname, rmaxs);
// 		format(msg, sizeof msg, "[DM]房间创建成功. 中途进入:[%s], 开始:[/dm start], 关闭:[/dm leave]", modename[mode]);
// 		SCM(playerid, Color_White, msg);
// 		if(DeathMatch_Room_Join(playerid, id) == 0)
// 		{
// 			SCM(playerid, Color_White, "[DM]该房间人数已满.");
// 		}
// 		return 1;
// 	}
// 	return 1;
// }


// 新增一个提示


public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart) {
    if(issuerid != INVALID_PLAYER_ID && IsPlayerDeathMatch(issuerid)) //如果不是自杀且玩家在DM中
    {
        new infoString[128], Float:health, Float:armour;
        GetPlayerHealth(playerid, health);
        GetPlayerArmour(playerid, armour);
        format(infoString, sizeof(infoString), "[DM]玩家[%s]受到你[%.0f]点伤害 血量[%.0f] 护甲[%.0f] FPS值[%d] ping[%d]", GetName(playerid), amount, health, armour, GetPlayerFPS(playerid), GetPlayerPing(playerid));
        SendClientMessage(issuerid, Color_White, infoString);
        return 1;
    }
    if(issuerid != INVALID_PLAYER_ID && GetPlayerVirtualWorld(issuerid) == 0)
    {
        // 2020.12.27修复信誉分扣错成受害者BUG
        SendClientMessage(issuerid, Color_Yellow, "[系统]您在大世界不可以攻击其他玩家哦,每次攻击都可能造成信誉分损失");
        new rand = random(2); //取随机数
        // if(rand) PlayerInfo[issuerid][bantotal]++; // 根据信誉分算法自动执行
        if(rand)
        {
            SendClientMessage(playerid, Color_Yellow, "[系统]很抱歉让您在大世界受到了攻击，我们已对对方采取扣除信誉分措施");
            GiveCreditpoints(issuerid, 1); // 根据信誉分算法自动执行
        }
        return 1;
    }
    return 0;
}


stock GetPlayerFPS(playerid) {
    SetPVarInt(playerid, "DrunkL", GetPlayerDrunkLevel(playerid)); //用于后续判定FPS
    if(GetPVarInt(playerid, "DrunkL") < 100) SetPlayerDrunkLevel(playerid, 2000);
    else {
        if(GetPVarInt(playerid, "LDrunkL") != GetPVarInt(playerid, "DrunkL")) {
            SetPVarInt(playerid, "FPS", (GetPVarInt(playerid, "LDrunkL") - GetPVarInt(playerid, "DrunkL")));
            SetPVarInt(playerid, "LDrunkL", GetPVarInt(playerid, "DrunkL"));
            if((GetPVarInt(playerid, "FPS") > 0) && (GetPVarInt(playerid, "FPS") < 256)) {
                return GetPVarInt(playerid, "FPS") - 1;
            }
        }
    }
    return 0;
}


// 来源于 https://forum.sa-mp.com/showthread.php?t=322484
// public OnPlayerKeyStateChange （ playerid，newkeys，oldkeys ）
// { 
//     if （ newkeys ＆ KEY_FIRE && ForbiddenWeap （ playerid ） 按下鼠标左键调用

stock ForbiddenWeap(const playerid, const checkmode) //禁止使用的武器 2020.4.1写 By YuCarl77
{
    new weap = GetPlayerWeapon(playerid);
    if(checkmode) {
        new flag = true;
        for (new i = 0; i < 5; i++) { //DM禁止使用除DM外的武器
            if(weap == MapGuns[DeathMatch_Game[playerid][g_mapid]][i]) flag = false;
        }
        if(flag) return flag;
    } else {
        if(weap == 35 || weap == 36 || weap == 37 || weap == 38 || weap == 39 || weap == 40) return true;
        if(weap == 41 || weap == 42 || weap == 43 || weap == 44 || weap == 45) return true;
        if(weap == 18) return true;
        //大世界禁止使用这些武器 然后直接没收玩家的所有武器
    }

    // // ==========================禁止武器================== =============== 
    // id 35 =火箭发射器
    // id 36 = HS火箭发射器
    // id 37 =喷火器
    // id 38 =迷你枪
    // id 39 =挎包冲锋
    // id 40 =雷管
    // id 41 = 喷雾 
    // id 42 =灭火器
    // id 43 =照相机
    // id 44 =夜视镜
    // id 45 =隔热镜
    return false;
}




// anti aimbot 反自瞄
// https://forum.sa-mp.com/showthread.php?t=571728


Float:DistanceCameraTargetToLocation(Float:CamX, Float:CamY, Float:CamZ, Float:ObjX, Float:ObjY, Float:ObjZ, Float:FrX, Float:FrY, Float:FrZ) 
{
    new Float:TGTDistance;

    TGTDistance = floatsqroot((CamX - ObjX) * (CamX - ObjX) + (CamY - ObjY) * (CamY - ObjY) + (CamZ - ObjZ) * (CamZ - ObjZ));

    new Float:tmpX, Float:tmpY, Float:tmpZ;

    tmpX = FrX * TGTDistance + CamX;
    tmpY = FrY * TGTDistance + CamY;
    tmpZ = FrZ * TGTDistance + CamZ;

    return floatsqroot((tmpX - ObjX) * (tmpX - ObjX) + (tmpY - ObjY) * (tmpY - ObjY) + (tmpZ - ObjZ) * (tmpZ - ObjZ));
}

Float:GetPointAngleToPoint(Float:x2, Float:y2, Float:X, Float:Y) {

    new Float:DX, Float:DY;
    new Float:angle;

    DX = floatabs(floatsub(x2, X));
    DY = floatabs(floatsub(y2, Y));

    if(DY == 0.0 || DX == 0.0) {
        if(DY == 0 && DX > 0) angle = 0.0;
        else if(DY == 0 && DX < 0) angle = 180.0;
        else if(DY > 0 && DX == 0) angle = 90.0;
        else if(DY < 0 && DX == 0) angle = 270.0;
        else if(DY == 0 && DX == 0) angle = 0.0;
    } else {
        angle = atan(DX / DY);

        if(X > x2 && Y <= y2) angle += 90.0;
        else if(X <= x2 && Y < y2) angle = floatsub(90.0, angle);
        else if(X < x2 && Y >= y2) angle -= 90.0;
        else if(X >= x2 && Y > y2) angle = floatsub(270.0, angle);
    }
    return floatadd(angle, 90.0);
}

GetXYInFrontOfPoint( & Float:x, & Float:y, Float:angle, Float:distance) {
    x += (distance * floatsin(-angle, degrees));
    y += (distance * floatcos(-angle, degrees));
}

IsPlayerAimingAt(playerid, Float:x, Float:y, Float:z, Float:radius) {
    new Float:camera_x, Float:camera_y, Float:camera_z, Float:vector_x, Float:vector_y, Float:vector_z;
    GetPlayerCameraPos(playerid, camera_x, camera_y, camera_z);
    GetPlayerCameraFrontVector(playerid, vector_x, vector_y, vector_z);

    new Float:vertical, Float:horizontal;

    switch (GetPlayerWeapon(playerid)) {
        case 34, 35, 36:{
            if(DistanceCameraTargetToLocation(camera_x, camera_y, camera_z, x, y, z, vector_x, vector_y, vector_z) < radius) return true;
            return false;
        }
        case 30, 31:{
            vertical = 4.0;
            horizontal = -1.6;
        }
        case 33:{
            vertical = 2.7;
            horizontal = -1.0;
        }
        default:{
            vertical = 6.0;
            horizontal = -2.2;
        }
    }

    new Float:angle = GetPointAngleToPoint(0, 0, floatsqroot(vector_x * vector_x + vector_y * vector_y), vector_z) - 270.0;
    new Float:resize_x, Float:resize_y, Float:resize_z = floatsin(angle + vertical, degrees);
    GetXYInFrontOfPoint(resize_x, resize_y, GetPointAngleToPoint(0, 0, vector_x, vector_y) + horizontal, floatcos(angle + vertical, degrees));

    if(DistanceCameraTargetToLocation(camera_x, camera_y, camera_z, x, y, z, resize_x, resize_y, resize_z) < radius) return true;
    return false;
}

bool:IsPlayerAimingAtPlayer(playerid, target) {
    new Float:x, Float:y, Float:z;
    GetPlayerPos(target, x, y, z);
    if(IsPlayerAimingAt(playerid, x, y, z - 0.75, 0.25)) return true;
    if(IsPlayerAimingAt(playerid, x, y, z - 0.25, 0.25)) return true;
    if(IsPlayerAimingAt(playerid, x, y, z + 0.25, 0.25)) return true;
    if(IsPlayerAimingAt(playerid, x, y, z + 0.75, 0.25)) return true;
    return false;
}

// public OnFilterScriptInit() {
//     return print("Filterscript Anti-Aimbot by n0minal sucefully loaded!");
// }


public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ) 
{
    DeathMatch_OnPlayerWeaponShot(playerid, hittype, hitid);
    switch (weaponid) {
        case 0..18, 39..54:return 1;
    } //invalid weapons

    if(hittype == BULLET_HIT_TYPE_PLAYER && IsPlayerConnected(hitid) && !IsPlayerNPC(hitid)) 
    {
        new Float:Shot[3], Float:Hit[3];
        GetPlayerLastShotVectors(playerid, Shot[0], Shot[1], Shot[2], Hit[0], Hit[1], Hit[2]);

        new playersurf = GetPlayerSurfingVehicleID(playerid);
        new hitsurf = GetPlayerSurfingVehicleID(hitid);
        new Float:targetpackets = NetStats_PacketLossPercent(hitid);
        new Float:playerpackets = NetStats_PacketLossPercent(playerid);

        if(~(playersurf) && ~(hitsurf) && !IsPlayerInAnyVehicle(playerid) && !IsPlayerInAnyVehicle(hitid)) 
        {
            if(!IsPlayerAimingAtPlayer(playerid, hitid) && !IsPlayerInRangeOfPoint(hitid, 5.0, Hit[0], Hit[1], Hit[2])) 
            {
                new string[128];
                AimbotWarnings[playerid]++;

                format(string, sizeof(string), "{FFFFFF}Player %s warning of aimbot or lag [Target PL: %f | Shooter PL:%f]!", GetName(playerid), targetpackets, playerpackets);
                SCMToAdmins(0x0066FFFF, string);

                if(AimbotWarnings[playerid] > 10) 
                {
                    if(targetpackets < 1.2 && playerpackets < 1.2) 
                    {
                        return Kick(playerid);
                    }
                    else 
                    {
                        format(string, sizeof(string), "{FFFFFF}Player %s is probably using aimbot [Target PL: %f | Shooter PL:%f]!", GetName(playerid), targetpackets, playerpackets);
                        SCMToAdmins(0x0066FFFF, string);
                    }
                }
                return 0;
            } 
            else 
            {
                return 1;
            }
        } 
        else 
        {
            return 1;
        }
    }
    return 1;
}
Dialog:Dialog_DeathMatch(playerid, response, listitem, inputtext[]) {
    if(response) {
        if(listitem == 0) {
            new msg[1024];
            for (new i = 0; i < DeathMatchCount; i++) {
                format(msg, sizeof msg, "%s%s\n", msg, DeathMatch_Map[i]);
            }
            format(msg, sizeof msg, "%s\n________________________________________________\n请输入 [地图名] [是否允许中途进入(0 = 不可以, 1 = 可以)] [人头]", msg);
            Dialog_Show(playerid, Dialog_DeathMatch_R_C, DIALOG_STYLE_INPUT, Dialog_DeathMatch_Title, msg, "创建", "返回");
            return 1;
        }
        if(listitem == 1) {
            DeathMatch_Game[playerid][g_page] = 1;
            ShowSearchRoomDialog(playerid, 1);
            return 1;
        }
    }
    return 1;
}
Dialog:Dialog_DeathMatch_R_C(playerid, response, listitem, inputtext[]) {
    if(!response) {
        ShowPlayerDeathMatchDialog(playerid);
        return 1;
    }
    new msg[128], idx, id;
    if(DeathMatch_Game[playerid][g_rid] != -1) {
        SCM(playerid, Color_White, "[DM]请先退出房间.");
        return 1;
    }
    msg = strtok(inputtext, idx);
    if(!strlen(msg)) {
        SCM(playerid, Color_White, "[DM]地图名不能为空.");
        return 1;
    }
    new string[MAX_PLAYER_NAME];
    format(string, sizeof(string), msg);
    if(IsMapNameExist(string) == -1) {
        SCM(playerid, Color_White, "[DM]错误的地图名.");
        return 1;
    }
    msg = strtok(inputtext, idx);
    if(!strlen(msg)) {
        SCM(playerid, Color_White, "[DM]房间模式不能为空.");
        return 1;
    }
    new mode = strval(msg);
    if(mode < 0 || mode > 1) {
        SCM(playerid, Color_White, "[DM]错误的模式.");
        return 1;
    }
    id = SearchForIdleRoom();
    if(id == -1) {
        SCM(playerid, Color_White, "[DM]当前房间已满.");
        return 1;
    }
    msg = strtok(inputtext, idx);
    if(!strlen(msg)) {
        SCM(playerid, Color_White, "[DM]人头数量不能为空.");
        return 1;
    }
    new rmaxs = strval(msg);
    if(rmaxs < 10 || rmaxs > 300) {
        SCM(playerid, Color_White, "[DM]错误的人头数量,范围:10 - 300");
        return 1;
    }
    // new modename[2][8] = {"不允许","允许"};
    DeathMatch_Room_Create(playerid, id, mode, string, rmaxs);
    if(mode) format(msg, sizeof msg, "[DM]房间创建成功. 中途进入:[允许], 开始:[/dm start], 关闭:[/dm leave]");
    else format(msg, sizeof msg, "[DM]房间创建成功. 中途进入:[不允许], 开始:[/dm start], 关闭:[/dm leave]");
    // format(msg, sizeof msg, "[DM]房间创建成功. 中途进入:[%s], 开始:[/dm start], 关闭:[/dm leave]", modename[mode]);
    SCM(playerid, Color_White, msg);
    if(DeathMatch_Room_Join(playerid, id) == 0) {
        SCM(playerid, Color_White, "[DM]该房间人数已满.");
        return 1;
    }
    return 1;
}
Dialog:Dialog_DeathMatch_S_R(playerid, response, listitem, inputtext[]) {
    if(response) {
        new msg[128], idx, id;
        msg = strtok(inputtext, idx);
        if(mk_strcmp(msg, "上一页") == 0) {
            if(DeathMatch_Game[playerid][g_page] == 1) {
                SCM(playerid, Color_White, "[DM]这是第一页.");
                return 1;
            }
            DeathMatch_Game[playerid][g_page]--;
            ShowSearchRoomDialog(playerid, DeathMatch_Game[playerid][g_page]);
            return 1;
        }
        if(mk_strcmp(msg, "下一页") == 0) {
            if(DeathMatch_Game[playerid][g_page] >= GetMaxPage(GetRoomCount())) {
                SCM(playerid, Color_White, "[DM]这是最后一页.");
                return 1;
            }
            DeathMatch_Game[playerid][g_page]++;
            ShowSearchRoomDialog(playerid, DeathMatch_Game[playerid][g_page]);
            return 1;
        }
        if(mk_strcmp(msg, "ID") == 0) {
            msg = strtok(inputtext, idx);
            id = strval(msg);
            new string[256];
            // new dstate[][8] = {
            //     "准备中",
            //     "已开始"
            // };
            if(DeathMatch_Room[id][rstate] == 0) {
                format(string, sizeof string, "房间名:[%s]\n创建者:[%s]\n当前人数:[%d]\n杀戮:[%d]\n状态:[准备中]", DeathMatch_Room[id][rmapname], DeathMatch_Room[id][rcreator], DeathMatch_Room[id][rplayer], DeathMatch_Room[id][rmax]);
                format(string, sizeof string, "%s\n{00FF00}加入 %d", string, id);
            }
            if(DeathMatch_Room[id][rstate] == 1 && DeathMatch_Room[id][rmode] == 1) {
                format(string, sizeof string, "房间名:[%s]\n创建者:[%s]\n当前人数:[%d]\n杀戮:[%d]\n状态:[已开始]", DeathMatch_Room[id][rmapname], DeathMatch_Room[id][rcreator], DeathMatch_Room[id][rplayer], DeathMatch_Room[id][rmax]);
                format(string, sizeof string, "%s\n{00FF00}加入 %d", string, id);
            }
            Dialog_Show(playerid, Dialog_DeathMatch_R_D, DIALOG_STYLE_LIST, Dialog_DeathMatch_Title, string, "选择", "返回");
            return 1;
        }
    } else {
        ShowPlayerDeathMatchDialog(playerid);
    }
    return 1;
}
Dialog:Dialog_DeathMatch_R_D(playerid, response, listitem, inputtext[]) {
    if(!response) {
        ShowSearchRoomDialog(playerid, DeathMatch_Game[playerid][g_page]);
        return 1;
    }
    new msg[128], idx, id;
    msg = strtok(inputtext, idx);
    if(mk_strcmp(msg, "加入") == 0) {
        msg = strtok(inputtext, idx);
        id = strval(msg);
        if(DeathMatch_Room[id][rstate] == 1 && DeathMatch_Room[id][rmode] == 0) {
            SCM(playerid, Color_White, "[DM]该房间不允许中途加入.");
            return 1;
        }
        if(DeathMatch_Game[playerid][g_rid] != -1) {
            SCM(playerid, Color_White, "[DM]你已经加入了一个房间.");
            return 1;
        }
        if(DeathMatch_Room_Join(playerid, id) == 0) {
            SCM(playerid, Color_White, "[DM]该房间人数已满.");
        }
        if(DeathMatch_Room[id][rstate] == 1) {
            DeathMatch_Game[playerid][g_mapid] = DeathMatch_Room[id][rmapid];
            DeathMatch_SetGunSpawn(playerid);
        }
        return 1;
    }
    return 1;
}
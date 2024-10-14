// IORP Script
// #include <izcmd>
#define	BasicWorld		10002 //基础数字号  
// 5666~6666是赛车世界 10001是DM

enum pworldtype {
    w_invite
};

new pWorld[MAX_PLAYERS][pworldtype];

stock Initialize_pWorld(const playerid) {
    pWorld[playerid][w_invite] = -1;
}

stock MyWorld(const playerid) {
    return playerid + BasicWorld;
}

CMD:w(playerid, cmdtext[]) { //切换世界
    new worlds, str[128];
    if(sscanf(cmdtext, "i", worlds)) {
        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), 0);
        SetPlayerVirtualWorld(playerid, 0);
        SCM(playerid, Color_WorldRed, "[世界] 你回到了大世界.");
        return 1;
    }
    if(worlds < 0 || worlds > 10000 || worlds >= 5666 && worlds <= 6666) return SCM(playerid, Color_WorldRed, "[世界] /w 0为大世界，世界只能切换为0-10000，且不可进入5666-6666的赛车世界");
    if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), worlds);
    SetPlayerVirtualWorld(playerid, worlds);
    format(str, sizeof(str), "[世界] 你切换到 %d 号平行世界.", worlds);
    SCM(playerid, Color_WorldRed, str);
    for (new i = GetPlayerPoolSize(); i >= 0; i--) {
        if(IsPlayerConnected(i)) {
            if(PlayerInfo[i][tvid] == playerid && i != playerid) {
                SetPlayerVirtualWorld(i, GetPlayerVirtualWorld(playerid));
                SetPlayerInterior(i, GetPlayerInterior(playerid));
                if(IsPlayerInAnyVehicle(i)) PlayerSpectateVehicle(i, GetPlayerVehicleID(playerid));
                else PlayerSpectatePlayer(i, playerid);
                SCM(i, Color_Orange, "[TV]:对方切换了世界，自动追踪观看.");
            }
        }
    }
    return 1;
}
CMD:ww(playerid, cmdtext[]) {
    new tmp[128], idx;
    tmp = strtok(cmdtext, idx);
    if(!strlen(tmp)) {
        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), MyWorld(playerid));
        SetPlayerVirtualWorld(playerid, MyWorld(playerid));
        SCM(playerid, Color_WorldRed, "[世界] 你进入了你的专属世界.");
        SCM(playerid, Color_WorldRed, "[世界] /ww kick - 踢出世界, /ww invite - 邀请进入, /ww back - 回到大世界.");
        for (new i = GetPlayerPoolSize(); i >= 0; i--) {
            if(IsPlayerConnected(i)) {
                if(PlayerInfo[i][tvid] == playerid && i != playerid) {
                    SetPlayerVirtualWorld(i, GetPlayerVirtualWorld(playerid));
                    SetPlayerInterior(i, GetPlayerInterior(playerid));
                    if(IsPlayerInAnyVehicle(i)) PlayerSpectateVehicle(i, GetPlayerVehicleID(playerid));
                    else PlayerSpectatePlayer(i, playerid);
                    SCM(i, Color_Orange, "[TV]:对方切换了世界，自动追踪观看.");
                }
            }
        }
        return 1;
    }
    if(mk_strcmp(tmp, "back", true) == 0) {
        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), 0);
        SetPlayerVirtualWorld(playerid, 0);
        SCM(playerid, Color_WorldRed, "[世界] 你回到了大世界.");
        return 1;
    }
    if(mk_strcmp(tmp, "yes", true) == 0) {
        if(pWorld[playerid][w_invite] == -1) return SCM(playerid, Color_WorldRed, "[世界] 目前无人邀请你哦.");
        if(IsPlayerConnected(pWorld[playerid][w_invite]) == 0) return SCM(playerid, Color_WorldRed, "[世界] 对方已离线.");
        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), pWorld[playerid][w_invite] + BasicWorld);
        SetPlayerVirtualWorld(playerid, pWorld[playerid][w_invite] + BasicWorld);
        format(tmp, sizeof tmp, "[世界] 你进入了 %s 的专属世界.", GetName(pWorld[playerid][w_invite]));
        SCM(playerid, Color_WorldRed, tmp);
        format(tmp, sizeof tmp, "[世界] %s 进入了你的专属世界.", GetName(playerid));
        SCM(pWorld[playerid][w_invite], Color_WorldRed, tmp);
        pWorld[playerid][w_invite] = -1;
        for (new i = GetPlayerPoolSize(); i >= 0; i--) {
            if(IsPlayerConnected(i)) {
                if(PlayerInfo[i][tvid] == playerid && i != playerid) {
                    SetPlayerVirtualWorld(i, GetPlayerVirtualWorld(playerid));
                    SetPlayerInterior(i, GetPlayerInterior(playerid));
                    if(IsPlayerInAnyVehicle(i)) PlayerSpectateVehicle(i, GetPlayerVehicleID(playerid));
                    else PlayerSpectatePlayer(i, playerid);
                    SCM(i, Color_Orange, "[TV]:对方切换了世界，自动追踪观看.");
                }
            }
        }
        return 1;
    }
    if(mk_strcmp(tmp, "invite", true) == 0) {
        tmp = strtok(cmdtext, idx);
        if(!strlen(tmp)) return SCM(playerid, Color_WorldRed, "[世界] /ww invite [对方ID] (邀请某人进入我的专属世界)");
        new id = strval(tmp);
        if(IsPlayerConnected(id) == 0 || id==playerid || IsPlayerNPC(id)) return SCM(playerid, Color_WorldRed, "[世界] 你输入的对方ID错误.");
        if(GetPlayerVirtualWorld(id) == MyWorld(playerid)) return SCM(playerid, Color_WorldRed, "[世界] 它已经在你的专属世界里了.");
        format(tmp, sizeof tmp, "[世界] 已将邀请发给 %s 静待回复.", GetName(id));
        SCM(playerid, Color_WorldRed, tmp);
        format(tmp, sizeof tmp, "[世界] %s 邀请你进入他的专属世界, 同意输入'/ww yes', 不同意就无视.", GetName(playerid));
        SCM(id, Color_WorldRed, tmp);
        pWorld[id][w_invite] = playerid;
        return 1;
    }
    if(mk_strcmp(tmp, "kick", true) == 0) {
        tmp = strtok(cmdtext, idx);
        if(!strlen(tmp)) return SCM(playerid, Color_WorldRed, "[世界] /ww kick [对方ID] (把某人踢出我的专属世界)");
        new id = strval(tmp);
        if(IsPlayerConnected(id) == 0 || id==playerid|| IsPlayerNPC(id)) return SCM(playerid, Color_WorldRed, "[世界] 你输入的对方ID错误.");
        if(GetPlayerVirtualWorld(id) != MyWorld(playerid)) {
            format(tmp, sizeof tmp, "[世界] %s 并没有在你的专属世界里.", GetName(id));
            SCM(playerid, Color_WorldRed, tmp);
            return 1;
        }
        SetPlayerVirtualWorld(id, 0);
        format(tmp, sizeof tmp, "[世界] 你把 %s 踢出了你的专属世界.", GetName(id));
        SCM(playerid, Color_WorldRed, tmp);
        format(tmp, sizeof tmp, "[世界] 你被 %s 踢出了他的专属世界", GetName(playerid));
        SCM(id, Color_WorldRed, tmp);
        return 1;
    }
    return 1;
}
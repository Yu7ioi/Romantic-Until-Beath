// IORP Script
/*
    This server is modified from 5F free residential open source, Prace open source
    To the makers of Prace:[Ghost]Rui ushio_p [Ghost]Dylan
    Tribute to the makers of the free settlement :[ITC]dyq [ITC]fangye [ITC]Super_wlc [ITC
    Special thanks to ryddawn and technical advisor [Fire]KiVen OBJ:JoshenKM;

								The MIT License  
								
	Copyright (c) <2019-2020> <YuCarl77>  
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.  
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.  
	==================================
	版权(c) <2019-2020> <YuCarl77>   

	使用该许可证的软件被授予以下权限，免费，任何人可以得到这个软件及其相关文档的一个拷贝，
	并且经营该软件不受任何限制，包括无限制的使用、复制、修改、合并、出版、发行、发放从属证书、或者出售该软件的拷贝的权利。
	同时允许获得这些软件的用户享受这些权利，使其服从下面的条件：  
	
	以上的版权通知和权限通知应该包含在所有该软件的拷贝中或者是其他该软件的真实部分中。
	
	该软件按本来的样子提供，没有任何形式的担保，不管是明确地或者暗含的，包含这些但是不受商业性质的担保的限制。
	适合一个特定的用途并且不受侵犯。作者和版权持有人在任何场合对使用该软件涉及的任何要求、损害或者其他责任都不应负责。
	不管它是正在起作用还是只是合同形式、民事侵权或是其他方式，如由它引起，在其作用范围内、与该软件有联系、该软件的使用或者有这个软件引起的其他行为。  
	=====================================   
*/

// #include <izcmd>

// #define Team_Dialog 800 //团队dialog
// #define Team_ListDialog Team_Dialog+1 //团队列表
// #define Team_ManageDialog Team_Dialog+2 //团队管理
// #define Team_DisbandDialog Team_Dialog+3 //团队管理解散二次确认
// #define Team_ChangeShortName Team_Dialog+4 //团队管理修改简称
// #define Team_ChangeFullName Team_Dialog+5 //团队管理修改全称
// #define Team_DescriptionDialog Team_Dialog+6 //团队管理修改简介 第一行
// #define Team_NextDescription Team_Dialog+7 //团队管理修改简介 无穷尽下一行
// #define Team_MemberDialog Team_Dialog+8 //团队成员列表
// #define Team_MemberManageDialog Team_Dialog+9 //团队成员管理dialog
// #define Team_InviteDialog Team_Dialog+10 //团队管理邀请玩家加入Dialog
// #define MemberManagerInviteList Team_Dialog+11 //团队管理邀请玩家加入和团队成员列表
// #define Team_TransferDialog Team_Dialog+12 //团队管理解散二次确认
// #define Team_Path "Team/Team.db" //团队SQLITE DB路径
#define MAX_TEAM 20 //最大团队数上限
#define MAX_TeamMember 50 //最大团队人数上限
#define TeamShortName_Length 12
#define TeamFullName_Length 24
#define TeamDescription_Length 512
#define Color_Team 0xFFFF88FF

// new DB:Teamdb, TeamCount = 0; //用户保存DB 初始化团队时调用 后边全走这个
// 2020.7.24适配MYSQLR41
new SelectedTeam[MAX_PLAYERS], SelectTeamPage[MAX_PLAYERS], SelectMemberPage[MAX_PLAYERS];
new SelectedUID[MAX_PLAYERS];
new tInvited[MAX_PLAYERS]; //邀请加入的队伍

enum tInfo {
    TID, //团队ID
    ShortName[TeamShortName_Length+1], //团队简称
    FullName[TeamFullName_Length+1], //团队全称
    Description[TeamDescription_Length+1] //团队简介
};
new TeamInfo[MAX_TEAM][tInfo];


stock Initialize_Team() { //OnGameModeInit时调用
    // if(!fexist(Team_Path)) {
    //     print("Team >> 找不到数据库文件!");
    // } else {
    //     Teamdb = db_open(Team_Path);
    SetupTeamTable();
    UpDate_TeamList();
    // }
    return 1;
}


// stock Team_OnGameModeExit() { //OnGameModeExit时调用
//     // for (new i = 0; i < TeamCount; i++) {
//     //     TeamInfo[i][TID] = -1;
//     //     format(TeamInfo[i][ShortName], 12, "");
//     //     format(TeamInfo[i][FullName], 24, "");
//     //     format(TeamInfo[i][Description], 512, "");
//     // }
//     // db_close(Teamdb);
//     return 1;
// }

stock SetupTeamTable() {
    new string[512];
    // 初始化团队用户表
    format(string, sizeof(string), "CREATE TABLE IF NOT EXISTS `teamuser`  (");
    strins(string, "`UID` int(11) UNSIGNED NOT NULL,", strlen(string));
    strins(string, "`TID` int(11) NULL DEFAULT NULL,", strlen(string));
    strins(string, "`Level` tinyint(1) NULL DEFAULT NULL,", strlen(string));
    strins(string, "PRIMARY KEY (`UID`) USING BTREE", strlen(string));
    strins(string, ") ENGINE = InnoDB CHARACTER SET = gbk;", strlen(string));
    mysql_pquery(g_Sql, string);
    // 初始化团队表
    format(string, sizeof(string), "CREATE TABLE IF NOT EXISTS `teamsystem`  (");
    strins(string, "`TID` int(11) NOT NULL AUTO_INCREMENT,", strlen(string));
    strins(string, "`ShortName` varchar(12) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,", strlen(string));
    strins(string, "`FullName` varchar(24) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,", strlen(string));
    strins(string, "`Description` varchar(512) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,", strlen(string));
    strins(string, "PRIMARY KEY (`TID`) USING BTREE", strlen(string));
    strins(string, ") ENGINE = InnoDB AUTO_INCREMENT = 6 CHARACTER SET = gbk;", strlen(string));
    mysql_pquery(g_Sql, string);
}

stock UpDate_TeamList() { //更新团队列表 在创建、删除后调用
    // 先清空一次
    // for (new i = 0; i < TeamCount; i++) {
    //     TeamInfo[i][TID] = 0;
    //     format(TeamInfo[i][ShortName], 12, "");
    //     format(TeamInfo[i][FullName], 24, "");
    //     format(TeamInfo[i][Description], 512, "");
    // }
    // 2020-07-24 16:03:43 注释了 因为一开始的时候本来就是空的好像没必要清？
    // 而且下面会复写一遍的

    // new msg[700], DBResult:cf;
    new Query[128];
    mysql_format(g_Sql, Query, sizeof(Query), "SELECT * FROM `teamsystem`"); //团队表
    mysql_pquery(g_Sql, Query, "UpDateTeamListQuery");
    // cf = db_query(Teamdb, msg);
    // TeamCount = db_num_rows(cf); //团队总数 = DB里的行数
    // for (new i = 0; i < TeamCount; i++) {
    //     new tid;
    //     // shortname[12], fullname[24], description[1024];

    //     db_get_field(cf, 0, msg, sizeof(msg));
    //     tid = strval(msg);
    //     TeamInfo[i][TID] = tid;

    //     db_get_field(cf, 1, msg, sizeof(msg));
    //     format(TeamInfo[i][ShortName], 12, msg);

    //     db_get_field(cf, 2, msg, sizeof(msg));
    //     format(TeamInfo[i][FullName], 24, msg);

    //     db_get_field(cf, 3, msg, sizeof(msg));
    //     format(TeamInfo[i][Description], 512, msg);

    //     db_next_row(cf);
    // }
    // db_free_result(cf);
}
function UpDateTeamListQuery() {
    new cut = cache_num_rows();
    for (new i = 0; i < cut; i++) 
    {
        cache_get_value_name_int(i, "TID", TeamInfo[i][TID]);
        cache_get_value_name(i, "ShortName", TeamInfo[i][ShortName]);
        cache_get_value_name(i, "FullName", TeamInfo[i][FullName]);
        cache_get_value_name(i, "Description", TeamInfo[i][Description]);
    }
}

stock team_Manage(const playerid, const selectid) {
    if(!team_GetLevel(PlayerInfo[playerid][ID], selectid)) return SendClientMessage(playerid, Color_Team, "[团队]你不是团队管理员,无法操作");
    Dialog_Show(playerid, Team_ManageDialog, DIALOG_STYLE_LIST, "团队管理", "修改简称\n修改全称\n修改介绍\n管理成员\n{FF0000}解散团队", "确定", "取消"); //展开管理项
    return 1;
}

stock team_GetLevel(const uid, const selectid) { //获取玩家的团队内部等级以判断玩家是否是管理员
    new Query[128], level;
    new Cache:result;
    mysql_format(g_Sql, Query, sizeof(Query), "SELECT `Level` FROM `teamuser` WHERE `UID` = %d AND `TID` = %d", uid, TeamInfo[selectid][TID]);
    result = mysql_query(g_Sql, Query);
    cache_get_value_index_int(0, 0, level);
    cache_delete(result);
    return level;
}

stock team_GiveAdmin(const uid, const level) {
    new Query[128];
    mysql_format(g_Sql, Query, sizeof(Query), "UPDATE `teamuser` SET `Level` = '%d' WHERE `UID` = %d", level, uid);
    mysql_pquery(g_Sql, Query);
}
stock team_Create(const playerid, const shortname[], const fullname[]) {
    new TeamCount = GetTeamCount();
    if(TeamCount >= MAX_TEAM) return -1; //不可创建团队 因为已经上限
    new flag = true;
    for (new i = 0; i < TeamCount; i++) {
        if((!strcmp(TeamInfo[i][FullName], fullname, true)) || (!strcmp(TeamInfo[i][ShortName], shortname, true))) {
            flag = false;
            break;
        }
    }
    if(!flag) return SendClientMessage(playerid, Color_Team, "[团队]已经有相同名称的团队存在了哦!");

    // 判断玩家是否在团队里 如果已经创建则不能接着创建;
    // 判断是否已经有同名的团队了，简称、全称 如果有则直接返回SCM不能
    // 创建 insert into 
    new msg[700];
    mysql_format(g_Sql, msg, sizeof(msg), "INSERT INTO `teamsystem` (ShortName,FullName,Description) VALUES ('%e','%e','null')", shortname, fullname);
    mysql_pquery(g_Sql, msg, "CreateOwnerQuery", "d", playerid);
    

    return 1;
}
function CreateOwnerQuery(playerid) {
    // mysql_format(g_Sql, msg, sizeof(msg), "SELECT * from `teamsystem` WHERE `ShortName` = '%e'", shortname);
    // mysql_pquery(g_Sql, const query[], const callback[] = "", const format[] = "", {Float, _:...});
    // new tid;
    // db_get_field_assoc(cf, "TID", msg, sizeof(msg));
    // tid = strval(msg);
    // db_free_result(cf);
    // 这里直接用的cache_insert_id 没有再去获取插入后的tid,如果有问题改回去
    new Query[128];
    mysql_format(g_Sql, Query, sizeof(Query), "INSERT INTO `teamuser` (UID,TID,LEVEL) VALUES (%d,%d,2)", PlayerInfo[playerid][ID], cache_insert_id());
    mysql_pquery(g_Sql, Query);
    UpDate_TeamList();
}

stock team_Delete(const tid) { //删除团队
    // 删除之前先把这个团队的名字取出来
    new Query[128], Cache:result, fullname[24];
    mysql_format(g_Sql, Query, sizeof(Query), "SELECT `FullName` from `teamsystem` WHERE TID = %d", tid);
    result = mysql_query(g_Sql, Query);
    cache_get_value_index(0, 0, fullname);
    cache_delete(result);
    // 删除这个团队
    mysql_format(g_Sql, Query, sizeof(Query), "DELETE FROM `teamsystem` WHERE TID = %d", tid);
    mysql_pquery(g_Sql, Query);
    // 删除所处于这个团队的用户
    mysql_format(g_Sql, Query, sizeof(Query), "DELETE FROM `teamuser` WHERE TID = %d", tid);
    mysql_pquery(g_Sql, Query);
    
    // 如果玩家的团队名字是已经删除的这个团队的名字 就清空
    for (new i = GetPlayerPoolSize(); i >= 0; i--) 
    {
        if(IsPlayerConnected(i) && PlayerInfo[i][Login])
        {
            if(strcmp(PlayerInfo[i][Team], fullname, true) == 0) format(PlayerInfo[i][Team], TeamFullName_Length, "null"); // 得判断用户的团队名是否是哪个团队的名字 是的话清空
        }
    }
    // 更新团队名称列表
    UpDate_TeamList();
}

stock team_Changename(const tid, const shortname, const fullname) { //修改团队名字
    new Query[256];
    mysql_format(g_Sql, Query, sizeof(Query), "UPDATE `teamsystem` SET `ShortName` = '%e',`FullName` = '%e' WHERE `TID` = %d", shortname, fullname, tid);
    mysql_pquery(g_Sql, Query);
    // 更新团队名称列表
    UpDate_TeamList();
}
stock team_Transfer(const tid, const currentuid, const olduid) {
    //转让团队 //先给新任的UID level为2 然后旧的降为1
    // 转让的用户必须是该团队内的用户，否则无法转让
    new Query[256];
    mysql_format(g_Sql, Query, sizeof(Query), "UPDATE `teamuser` SET `Level` = 2 WHERE `UID` = '%d' AND `TID` = '%d'", currentuid, tid);
    mysql_pquery(g_Sql, Query);

    mysql_format(g_Sql, Query, sizeof(Query), "UPDATE `teamuser` SET `Level` = 1 WHERE `UID` = '%d' AND `TID` = '%d'", olduid, tid);
    mysql_pquery(g_Sql, Query);
    // 更新团队名称列表
    UpDate_TeamList();
}

stock team_Kick(const uid) {
    new Query[90];
    mysql_format(g_Sql, Query, sizeof(Query), "DELETE FROM `teamuser` WHERE `UID` = '%d'", uid);
    mysql_pquery(g_Sql, Query);
    for (new i = GetPlayerPoolSize(); i >= 0; i--) {
        if(IsPlayerConnected(i) && PlayerInfo[i][Login])
        {
            if(PlayerInfo[i][ID] == uid) //清空玩家的团队信息
            {
                format(PlayerInfo[i][Team], TeamFullName_Length, "null");
                break;
            }
        }
    }
    // UpDate_TeamList();
}

stock team_Invite(const inviteid, const tid, const pname[], const tname[]) {
    tInvited[inviteid] = tid;
    new str[128];
    format(str, sizeof(str), "[团队]%s 邀请您加入 %s,输入 /t yes加入", pname, tname);
    SendClientMessage(inviteid, Color_Team, str);
}

stock team_Join(const uid, const tid) { //判断玩家是否已经拥有团队 有的话提示禁止加入
    new Query[128];
    mysql_format(g_Sql, Query, sizeof(Query), "INSERT INTO `teamuser` (UID,TID,LEVEL) VALUES ('%d','%d',0)", uid, tid);
    mysql_pquery(g_Sql, Query);
    // UpDate_TeamList();
}

stock team_List(const playerid, const page) { //为指定玩家显示赛道选择菜单,page表示赛道页数
    new maxpage;
    // maxpage = TeamCount / 15 + 1; // 一页15个
    new Cache:result, TeamCount;
    result = mysql_query(g_Sql, "SELECT COUNT(*) FROM `teamsystem`");
    cache_get_value_index_int(0, 0, TeamCount);
    cache_delete(result);
    if(TeamCount % 10 == 0) maxpage = TeamCount / 10; //一页10个
    else maxpage = (TeamCount / 10) + 1;
    new title[64], msg[1024];
    format(title, 64, "团队列表 - %i/%i", page, maxpage);
    format(msg, 1024, "ID\t简称\t全称%s\n", msg);
    if(page > 1 && page == maxpage) format(msg, 1024, "%s\n上一页", msg);
    if(page == 1 && page != maxpage) format(msg, 1024, "%s\n下一页\n", msg);
    if(page > 1 && page < maxpage) format(msg, 1024, "%s\n上一页\n下一页", msg);
    for (new i = (page - 1) * 15; i < page * 15; i++) {
        //如果拥有者是自己就高亮
        if(TeamInfo[i][TID]) format(msg, sizeof(msg), "%s\n%d\t%s\t%s\n", msg, i, TeamInfo[i][ShortName], TeamInfo[i][FullName]);
    }
    Dialog_Show(playerid, Team_ListDialog, DIALOG_STYLE_TABLIST_HEADERS, title, msg, "信息", "关闭");
    return 1;
}

stock team_MemberList(const playerid, const page, const tid) { //成员列表
    new Query[512];
    mysql_format(g_Sql, Query, sizeof(Query), "SELECT `teamuser`.`UID`,`users`.`Name`,`teamuser`.`TID`,`teamuser`.`Level` FROM `teamuser`\
    join `users` on `users`.ID = `teamuser`.`UID`  WHERE `teamuser`.`TID` = '%d'  ORDER BY `teamuser`.`Level` DESC", tid); //团队表 按等级降序 这样子就很适合
    // 就创始人肯定是在最前面的
    mysql_pquery(g_Sql, Query, "MemberList_Query", "dd", playerid, page);
    // 临时变量存放

}

function MemberList_Query(playerid, page) {
    enum tempInfo {
        UID, //UID
        Name[24],
        Level //等级
    };

    // 这一部分超出知识范围了
    // 显示UID 名字 等级 等级0是成员 1是管理员 2是创立者
    // 然后支持翻页 ,双击玩家再弹出一个窗 写 踢出成员 给管理员 取消管理员 转让团队 难点在于 1. 怎么写成员列表 2.双击后跳转到那边怎么传递之前选的玩家过去
    // 找个地方写邀请加入
    // printf("tid:%d", tid);
    new TeamMemberInfo[MAX_TeamMember][tempInfo];
    new TeamMemberCount = cache_num_rows(); //团队总数 = DB里的行数
    // printf("%d", TeamMemberCount);
    for (new i = 0; i < TeamMemberCount; i++) {
        cache_get_value_name_int(i, "UID", TeamMemberInfo[i][UID]);//取出UID
        cache_get_value_name_int(i, "Level", TeamMemberInfo[i][Level]);//取出他的等级
        cache_get_value_name(i, "Name", TeamMemberInfo[i][Name]);//那个人的名字
    }
    new title[32], msg[1024]; 
    new maxpage = GetMaxPage(TeamMemberCount, 10); 
    //总页数计算

    //2020.3.24 22.42新增 修复跨团队查看页数超页问题
    if(page > maxpage) page = 1;

    format(title, sizeof(title), "成员列表 - %i/%i", page, maxpage);
    format(msg, sizeof(msg), " "); //清空msg
    format(msg, sizeof(msg), "UID\t用户名\t等级%s\n", msg);
    if(page > 1 && page == maxpage) format(msg, sizeof(msg), "%s\n上一页", msg);
    if(page == 1 && page != maxpage) format(msg, sizeof(msg), "%s\n下一页\n", msg);
    if(page > 1 && page < maxpage) format(msg, sizeof(msg), "%s\n上一页\n下一页", msg);

    for (new i = (page - 1) * 10; i < page * 10; i++) { //i < page * 10 - 1
        if(i > TeamMemberCount) break;
        //如果拥有者是自己就高亮
        new levelname[6];
        if(TeamMemberInfo[i][Level] == 2) format(levelname, sizeof(levelname), "队长");
        if(TeamMemberInfo[i][Level] == 1) format(levelname, sizeof(levelname), "管理");
        if(TeamMemberInfo[i][Level] == 0) format(levelname, sizeof(levelname), "成员");
        if(TeamMemberInfo[i][UID]) 
        {
            new online_state = false;
            for (new j = GetPlayerPoolSize(); j >= 0; j--) {
                if(IsPlayerConnected(j) && !IsPlayerNPC(j)) {
                    if(!strcmp(GetName(j), TeamMemberInfo[i][Name], true)) 
                    {
                        online_state = true;
                        break;
                    }
                }
            }
            if(online_state) format(msg, sizeof(msg), "%s\n%d\t%s | {00FF00}在线\t%s\n", msg, TeamMemberInfo[i][UID], TeamMemberInfo[i][Name], levelname);
            else format(msg, sizeof(msg), "%s\n%d\t%s | {FF0000}离线\t%s\n", msg, TeamMemberInfo[i][UID], TeamMemberInfo[i][Name], levelname);
        }

    }
    Dialog_Show(playerid, Team_MemberDialog, DIALOG_STYLE_TABLIST_HEADERS, title, msg, "操作", "返回");
}

stock team_DescriptionChange(const tid, const string[]) {
    new Query[622];
    mysql_format(g_Sql, Query, sizeof(Query), "UPDATE `teamsystem` SET `Description` = '%e' WHERE `TID` = %d", string, tid);
    mysql_pquery(g_Sql, Query);
    UpDate_TeamList();
}

stock team_ShortNameChange(const tid, const string[]) {
    new Query[128];
    mysql_format(g_Sql, Query, sizeof(Query), "UPDATE `teamsystem` SET `ShortName` = '%e' WHERE `TID` = %d", string, tid);
    mysql_pquery(g_Sql, Query);
    UpDate_TeamList();
}

stock team_SCM(const string[], const fullname[]) {
    for (new i = GetPlayerPoolSize(); i >= 0; i--) {
        if(PlayerInfo[i][Login]) {
            if(strcmp(fullname, PlayerInfo[i][Team]) == 0) {
                SendClientMessage(i, Color_Team, string);
            }
        }
    }
}
stock team_FullNameChange(const tid, const string[]) {
    new Query[128];
    mysql_format(g_Sql, Query, sizeof(Query), "UPDATE `teamsystem` SET `FullName` = '%e' WHERE `TID` = %d", string, tid);
    mysql_pquery(g_Sql, Query);
    UpDate_TeamList();
}


stock team_OnPlayerLogin(const playerid) {
    // 玩家登录游戏时判断teamuser表 where uid = 他  的TID 再去调用 teamsystem里的TID寻找团队名字 然后赋值给PlayerInfo[playerid][Team]
    tInvited[playerid] = -1; //初始化没人邀请他
    SelectTeamPage[playerid] = 1;
    SelectMemberPage[playerid] = 1;
    SelectedUID[playerid] = -1;
    format(PlayerInfo[playerid][Team], TeamFullName_Length, "null");
    // PlayerInfo[playerid][Team] = "";
    new Query[128];
    mysql_format(g_Sql, Query, sizeof(Query), "SELECT `TID` FROM `teamuser` WHERE UID = '%d' LIMIT 1", PlayerInfo[playerid][ID]); //团队用户表
    mysql_pquery(g_Sql, Query, "team_LoginQuery", "d", playerid);
}

function team_LoginQuery(playerid) {
    if(cache_num_rows())
    {
        new tid, Query[128];
        cache_get_value_int(0, 0, tid);

        mysql_format(g_Sql, Query, sizeof(Query), "SELECT `FullName` FROM `teamsystem` WHERE TID = '%d'", tid); //团队用户表
        mysql_pquery(g_Sql, Query, "pIfExistTeamTip", "d", playerid);
    }
}

function pIfExistTeamTip(playerid) {
    if(cache_num_rows()) //如果有行数的话 就是说他有在团队里 那么就给他值
    {
        cache_get_value(0, 0, PlayerInfo[playerid][Team]);
        //让玩家的团队名称显示为队伍的全称
        if(strcmp(PlayerInfo[playerid][Team], "null", true)) { //如果对方在团队中
            new msg[144];
            format(msg, sizeof(msg), "[团队]{FFFF88}%s 成员{00FFAA} %s(%d) {FFFF88}已上线", PlayerInfo[playerid][Team], GetName(playerid), playerid); //团队用户表
            SendClientMessageToAll(Color_Team, msg);
        }
    }
}
// teamlevel同理 0级普通成员 1级管理员 2级团队创始人   在创建和转让时必须有2级    2级团队离开团队将意味着团队解散

// 创建团队 删除团队 转让队长 踢出成员 邀请加入 团队列表 团队简介修改 团队简称修改 团队全称修改



CMD:t(const playerid, const cmdtext[]) {
    new tmp[128], idx;
    tmp = strtok(cmdtext, idx);
    if(!strlen(tmp) || !strcmp(tmp, "help", true)) {
        // ShowPlayerDialog(playerid, 5, DIALOG_STYLE_LIST, "广告牌系统", "在广告牌附近按Y可操作广告牌,可回收金币\n/board buy购买广告牌.\n/board list 查看广告牌列表\n/board goto id传送到广告牌", "确定", "返回");
        // SendClientMessage(playerid, Color_Team, "[团队]/t create 简称 全称 - 创建团队");
        // SendClientMessage(playerid, Color_Team, "[团队]/t list - 查看团队列表");
        Dialog_Show(playerid, MessageBox, DIALOG_STYLE_LIST, "团队系统", "创建团队/t create 团队列表/t list\n我的团队/t wode或/wdtd\n\
        拉取队友/t get 传送到队员/t goto\n/t invite邀请玩家加入团队\n列表式操作\
        n团队用户等级分为0,1,2\n0为成员,1为管理员,2为创始者\n管理员可拉取其他玩家为团队成员\n\
        创始者比管理员多一个转让和解散权限", "确定", "返回");
        return 1;
    }
    if(!strcmp(tmp, "create", true)) { //创建团队
        if(strcmp(PlayerInfo[playerid][Team], "null", true)) return SendClientMessage(playerid, Color_Team, "[团队]您已经在一个团队中了,不可创建团队");
        if(PlayerInfo[playerid][Cash] < 80000) return SendClientMessage(playerid, Color_Team, "[团队]您的金币不足8W");
        new shortname[12], fullname[24];
        tmp = strtok(cmdtext, idx);
        if(!strlen(tmp)) return SCM(playerid, Color_Team, "[团队]/t create 简称 全称");
        format(shortname, sizeof(shortname), tmp);
        if(strlen(shortname) < 2 || strlen(shortname) > 12) return SCM(playerid, Color_Team, "[团队]简称长度太短或太长了");
        tmp = strtok(cmdtext, idx);
        format(fullname, sizeof(fullname), tmp);
        if(strlen(fullname) < 2 || strlen(fullname) > 24) return SCM(playerid, Color_Team, "[团队]全称长度太短或太长了");
        if(!strcmp(fullname, "null", false)) return SCM(playerid, Color_Team, "[团队]不可使用非法名");
        new placeholder;
        for (new i = 0; i < sizeof InvalidWords; i++) //屏蔽词自动变*
        {
            placeholder = strfind(shortname, InvalidWords[i], true);
            if(placeholder != -1) {
                SendClientMessage(playerid, Color_Team, "[团队]不可使用非法名称");
                return 1;
            }
        }
        for (new i = 0; i < sizeof InvalidWords; i++) //屏蔽词自动变*
        {
            placeholder = strfind(fullname, InvalidWords[i], true);
            if(placeholder != -1) {
                SendClientMessage(playerid, Color_Team, "[团队]不可使用非法名称");
                return 1;
            }
        }
        team_Create(playerid, shortname, fullname);
        GivePlayerCash(playerid, -80000);
        format(PlayerInfo[playerid][Team], TeamFullName_Length, fullname);
        SendClientMessage(playerid, Color_Team, "[团队]创建团队成功!");
        return 1;
    }
    if(!strcmp(tmp, "list", true)) return team_List(playerid, SelectTeamPage[playerid]); //查看团队列表
    if(!strcmp(tmp, "wode", true)) return cmd_wdtd(playerid, ""); //我的团队
    if(!strcmp(tmp, "get", true)) {
        tmp = strtok(cmdtext, idx);
        new getid = strval(tmp);
        team_Get(getid, playerid);
        return 1;
    }
    if(!strcmp(tmp, "goto", true)) {
        tmp = strtok(cmdtext, idx);
        new gotoid = strval(tmp);
        team_Goto(gotoid, playerid);
        return 1;
    }
    if(!strcmp(tmp, "yes", true)) {
        if(tInvited[playerid] == -1) return SendClientMessage(playerid, Color_Team, "[团队]暂时没有团队邀请您哦");
        if(strcmp(PlayerInfo[playerid][Team], "null", true)) return SendClientMessage(playerid, Color_Team, "[团队]您已经在一个团队中了");
        new TeamCount = GetTeamCount();
        for (new i = 0; i < TeamCount; i++) {
            if(TeamInfo[i][TID] == tInvited[playerid]) {
                format(PlayerInfo[playerid][Team], TeamFullName_Length, TeamInfo[i][FullName]); //设置团队为那个团队的名字
                break;
            }
        }
        team_Join(PlayerInfo[playerid][ID], tInvited[playerid]);
        tInvited[playerid] = -1; //重置状态
        new str[128];
        format(str, sizeof(str), "[团队]%s(%d)加入了队伍", GetName(playerid), playerid);
        team_SCM(str, PlayerInfo[playerid][Team]);
        return 1;
    }
    if(!strcmp(tmp, "invite", true)) {
        if(strcmp(PlayerInfo[playerid][Team], "null", true) == 0) return SendClientMessage(playerid, Color_Team, "[团队]您未任何一个团队中");
        tmp = strtok(cmdtext, idx);
        if(!strlen(tmp)) return SendClientMessage(playerid, Color_Team, "[团队]请输入您要邀请的playerid");
        new inviteid = strval(tmp);
        if(PlayerInfo[inviteid][Login] == false || tInvited[inviteid]!=-1) return SendClientMessage(playerid, Color_Team, "[团队]对方未登录或已有邀请");
        if(strcmp(PlayerInfo[inviteid][Team], "null", true)) return SendClientMessage(playerid, Color_Team, "[团队]对方已经在某个团队中了");
        new TeamCount = GetTeamCount();
        for (new i = 0; i < TeamCount; i++) {
            if(!strcmp(PlayerInfo[playerid][Team], TeamInfo[i][FullName])) {
                SelectedTeam[playerid] = i;
                break;
            }
        }
        if(!team_GetLevel(PlayerInfo[playerid][ID], SelectedTeam[playerid])) return SendClientMessage(playerid, Color_Team, "[团队]您不是团队的管理员");
        // const inviteid, const tid, const pname[], const tname[]
        SendClientMessage(playerid, Color_Team, "[团队]您已发出邀请,请等待对方回复,15秒后失效");
        team_Invite(inviteid, TeamInfo[SelectedTeam[playerid]][TID], GetName(playerid), TeamInfo[SelectedTeam[playerid]][FullName]);
        SetTimerEx("tinviteTimer", 15000, false, "ii", inviteid, playerid);
        return 1;
    }
    SendClientMessage(playerid, Color_Team, "[团队]命令不存在!");
    return 1;
}

CMD:wdtd(const playerid, const cmdtext[]) {
    if(!strcmp(PlayerInfo[playerid][Team], "null", true)) return SendClientMessage(playerid, Color_Team, "[团队]您还没有在团队中哦,请让团队管理员邀请您加入吧");
    new TeamCount = GetTeamCount();
    for (new i = 0; i < TeamCount; i++) {
        if(!strcmp(PlayerInfo[playerid][Team], TeamInfo[i][FullName])) {
            SelectedTeam[playerid] = i;
            break;
        }
    }
    new msg[512];
    format(msg, sizeof msg, "团队简称 %s TID:%d\n全称 %s\n团队介绍\n团队成员\n管理团队\n{FF0000}退出团队", TeamInfo[SelectedTeam[playerid]][ShortName], TeamInfo[SelectedTeam[playerid]][TID], TeamInfo[SelectedTeam[playerid]][FullName]);
    Dialog_Show(playerid, Team_Dialog, DIALOG_STYLE_LIST, "我的团队", msg, "确定", "取消");
    return 1;
}

// forward team_memberlist(...);



stock team_Get(const getid, const playerid) {
    if(getid == -1) {
        new flag = false;
        for (new i = GetPlayerPoolSize(); i >= 0; i--) {
            if(IsPlayerConnected(i)) {
                if(PlayerInfo[i][ID] == SelectedUID[playerid]) {
                    if(pRaceing[i]) {
                        break;
                    }
                    new Float:x, Float:y, Float:z;
                    // format(str, sizeof(str), "[管理员] %s 使用了 Get. ", GetName(playerid));
                    // SCMToAdmins(0x0066FFFF, str);
                    GetPlayerPos(playerid, x, y, z);
                    if(IsPlayerInAnyVehicle(i)) SetVehiclePos(GetPlayerVehicleID(i), x + 2, y + 2, z + 0.35);
                    else SetPlayerPos(i, x + 2, y + 2, z + 0.35);
                    new msg[96];
                    format(msg, sizeof(msg), "[团队]管理员%s(%d)使用了Get", GetName(playerid), playerid);
                    team_SCM(msg, PlayerInfo[playerid][Team]);
                    flag = true;
                    break;
                }
            }
        }
        if(!flag) return SendClientMessage(playerid, Color_Team, "[团队]对方不在线或正在赛道中");
    } else {
        if(!PlayerInfo[getid][Login] || pRaceing[getid]) return SendClientMessage(playerid, Color_Team, "[团队]对方不在线或正在赛道中");
        if(strcmp(PlayerInfo[getid][Team], PlayerInfo[playerid][Team]) != 0) return SendClientMessage(playerid, Color_Team, "[团队]对方不是您的团队成员");
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        if(IsPlayerInAnyVehicle(getid)) SetVehiclePos(GetPlayerVehicleID(getid), x + 2, y + 2, z + 0.35);
        else SetPlayerPos(getid, x + 2, y + 2, z + 0.35);
        new msg[96];
        format(msg, sizeof(msg), "[团队]成员%s(%d)使用了/t get %d(%s)", GetName(playerid), playerid, getid, GetName(getid));
        team_SCM(msg, PlayerInfo[playerid][Team]);
    }
    return 1;
}


stock team_Goto(const gotoid, const playerid) {
    if(gotoid == -1) {
        new flag = false;
        for (new i = GetPlayerPoolSize(); i >= 0; i--) {
            if(IsPlayerConnected(i)) {
                if(PlayerInfo[i][ID] == SelectedUID[playerid]) {
                    if(pRaceing[i]) {
                        break;
                    }
                    new Float:x, Float:y, Float:z;
                    // format(str, sizeof(str), "[管理员] %s 使用了 Get. ", GetName(playerid));
                    // SCMToAdmins(0x0066FFFF, str);
                    GetPlayerPos(i, x, y, z);
                    if(IsPlayerInAnyVehicle(playerid)) SetVehiclePos(GetPlayerVehicleID(playerid), x + 2, y + 2, z + 0.35);
                    else SetPlayerPos(playerid, x + 2, y + 2, z + 0.35);
                    new msg[96];
                    format(msg, sizeof(msg), "[团队]管理员%s(%d)使用了Goto", GetName(playerid), playerid);
                    team_SCM(msg, PlayerInfo[playerid][Team]);
                    flag = true;
                    break;
                }
            }
        }
        if(!flag) return SendClientMessage(playerid, Color_Team, "[团队]对方不在线或正在赛道中");
    } else {
        if(!PlayerInfo[gotoid][Login] || pRaceing[gotoid]) return SendClientMessage(playerid, Color_Team, "[团队]对方不在线或正在赛道中");
        if(strcmp(PlayerInfo[gotoid][Team], PlayerInfo[playerid][Team]) != 0) return SendClientMessage(playerid, Color_Team, "[团队]对方不是您的团队成员");
        new Float:x, Float:y, Float:z;
        GetPlayerPos(gotoid, x, y, z);
        if(IsPlayerInAnyVehicle(playerid)) SetVehiclePos(GetPlayerVehicleID(playerid), x + 2, y + 2, z + 0.35);
        else SetPlayerPos(playerid, x + 2, y + 2, z + 0.35);
        new msg[96];
        format(msg, sizeof(msg), "[团队]成员%s(%d)使用了/t goto %d(%s)", GetName(playerid), playerid, gotoid, GetName(gotoid));
        team_SCM(msg, PlayerInfo[playerid][Team]);
    }
    return 1;
}


function tinviteTimer(playerid, inviter) {
    if(tInvited[playerid] != -1) 
    {
        tInvited[playerid] = -1;
        SendClientMessage(playerid, Color_Team, "[团队]您在15秒内没有做出回应,自动拒绝");
        SendClientMessage(inviter, Color_Team, "[团队]对方在15秒内没有做出回应,自动拒绝");
    }
    return 1;
}
 //掉用这个的时候先把简介清空
Dialog:Team_DescriptionDialog(playerid, response, listitem, inputtext[]){
    if(!response) {
        team_DescriptionChange(TeamInfo[SelectedTeam[playerid]][TID], TeamInfo[SelectedTeam[playerid]][Description]);
        SendClientMessage(playerid, Color_Team, "[团队]修改简介成功");
        return 1;
    }
    if(strcmp(inputtext, " ") == 0 || strlen(inputtext) < 1 || strlen(inputtext) > TeamDescription_Length) {
        Dialog_Show(playerid, Team_DescriptionDialog, DIALOG_STYLE_INPUT, "团队简介", "{FF0000}请输入简介\n不能为空并且输入字符在(1-512)之内", "确定", "返回");
        return 1;
    }
    format(TeamInfo[SelectedTeam[playerid]][Description], TeamDescription_Length, "%s\n", inputtext);
    new placeholder, msg[600];
    for (new i = 0; i < sizeof InvalidWords; i++) //屏蔽词自动变*
    {
        placeholder = strfind(TeamInfo[SelectedTeam[playerid]][Description], InvalidWords[i], true);
        if(placeholder != -1) {
            for (new x = placeholder; x < placeholder + strlen(InvalidWords[i]); x++) {
                TeamInfo[SelectedTeam[playerid]][Description][x] = '*';
            }
        }
    }
    format(msg, sizeof(msg), "你目前输入的内容:%s", TeamInfo[SelectedTeam[playerid]][Description]);
    Dialog_Show(playerid, Team_NextDescription, DIALOG_STYLE_INPUT, "团队简介", msg, "下一行", "确定");
    return 1;
}
Dialog:Team_NextDescription(playerid, response, listitem, inputtext[]) {
    if(!response) {
        team_DescriptionChange(TeamInfo[SelectedTeam[playerid]][TID], TeamInfo[SelectedTeam[playerid]][Description]);
        SendClientMessage(playerid, Color_Team, "[团队]修改简介成功");
        return 1;
    }
    if(strcmp(inputtext, " ") == 0 || strlen(inputtext) < 1 || strlen(inputtext) > 128 || strlen(TeamInfo[SelectedTeam[playerid]][Description]) >= TeamDescription_Length) {
        Dialog_Show(playerid, Team_NextDescription, DIALOG_STYLE_INPUT, "团队简介", "{FF0000}请输入简介\n不能为空并且输入字符在(1-512)之内", "确定", "返回");
        return 1;
    }
    format(TeamInfo[SelectedTeam[playerid]][Description], TeamDescription_Length, "%s%s\n", TeamInfo[SelectedTeam[playerid]][Description], inputtext);
    new placeholder, msg[600];
    for (new i = 0; i < sizeof InvalidWords; i++) //屏蔽词自动变*
    {
        placeholder = strfind(TeamInfo[SelectedTeam[playerid]][Description], InvalidWords[i], true);
        if(placeholder != -1) {
            for (new x = placeholder; x < placeholder + strlen(InvalidWords[i]); x++) {
                TeamInfo[SelectedTeam[playerid]][Description][x] = '*';
            }
        }
    }
    format(msg, sizeof(msg), "你目前输入的内容:%s", TeamInfo[SelectedTeam[playerid]][Description]);
    Dialog_Show(playerid, Team_NextDescription, DIALOG_STYLE_INPUT, "团队简介", msg, "下一行", "确定");
    return 1;
}
Dialog:Team_DisbandDialog(playerid, response, listitem, inputtext[]) {
    if(!response) {
        if(!strcmp(TeamInfo[SelectedTeam[playerid]][FullName], PlayerInfo[playerid][Team], true)) {
            team_Delete(TeamInfo[SelectedTeam[playerid]][TID]);
            SendClientMessage(playerid, Color_Team, "[团队]您的解散了...");
        } else {
            SendClientMessage(playerid, Color_Team, "[团队]系统内部错误...");
        }
    }
    return 1;
}
Dialog:Team_ChangeShortName(playerid, response, listitem, inputtext[]) { //要加个判断是不是有人用了
    if(response) {
        if(strlen(inputtext) < 2 || strlen(inputtext) > 12) return Dialog_Show(playerid, Team_ChangeShortName, DIALOG_STYLE_INPUT, "修改简称", "请在下方输入团队的简称\n长度需2~12位以内", "确认", "取消");
        new placeholder;
        for (new i = 0; i < sizeof InvalidWords; i++) //屏蔽词自动变*
        {
            placeholder = strfind(inputtext, InvalidWords[i], true);
            if(placeholder != -1) {
                SendClientMessage(playerid, Color_Team, "[团队]不可使用非法名称");
                return 1;
            }
        }
        new flag = true;
        new TeamCount = GetTeamCount();
        for (new i = 0; i < TeamCount; i++) {
            if((!strcmp(TeamInfo[i][FullName], inputtext, true)) || (!strcmp(TeamInfo[i][ShortName], inputtext, true))) {
                flag = false;
                break;
            }
        }
        if(!flag) return SendClientMessage(playerid, Color_Team, "[团队]已经有相同名称的团队存在了哦!");
        team_ShortNameChange(TeamInfo[SelectedTeam[playerid]][TID], inputtext);
        SendClientMessage(playerid, Color_Team, "[团队]修改团队简称成功");
    }
    return 1;
}
Dialog:Team_ChangeFullName(playerid, response, listitem, inputtext[]) {  //要加个判断是不是有人用了
    if(response) {
        if(strlen(inputtext) < 2 || strlen(inputtext) > TeamFullName_Length) return Dialog_Show(playerid, Team_ChangeShortName, DIALOG_STYLE_INPUT, "修改简称", "请在下方输入团队的简称\n长度需2~12位以内", "确认", "取消");
        if(!strcmp(inputtext, "null", false)) return SCM(playerid, Color_Team, "[团队]不可使用非法名");
        new placeholder;
        for (new i = 0; i < sizeof InvalidWords; i++) //屏蔽词自动变*
        {
            placeholder = strfind(inputtext, InvalidWords[i], true);
            if(placeholder != -1) {
                SendClientMessage(playerid, Color_Team, "[团队]不可使用非法名称");
                return 1;
            }
        }
        new flag = true;
        new TeamCount = GetTeamCount();
        for (new i = 0; i < TeamCount; i++) {
            if((!strcmp(TeamInfo[i][FullName], inputtext, true)) || (!strcmp(TeamInfo[i][ShortName], inputtext, true))) {
                flag = false;
                break;
            }
        }
        if(!flag) return SendClientMessage(playerid, Color_Team, "[团队]已经有相同名称的团队存在了哦!");
        team_FullNameChange(TeamInfo[SelectedTeam[playerid]][TID], inputtext);
        SendClientMessage(playerid, Color_Team, "[团队]修改团队全称成功");
    }
    return 1;
}
Dialog:Team_ManageDialog(playerid, response, listitem, inputtext[]) { 
    // 修改简称\n修改全称\n修改介绍\n管理成员\n转让团队\n{FF0000}解散团队
    // 管理成员有成员列表 踢出成员 给管理员 取消管理员 邀请加入 转让团队
    if(response) {
        if(listitem == 0) {
            Dialog_Show(playerid, Team_ChangeShortName, DIALOG_STYLE_INPUT, "修改简称", "请在下方输入团队的简称\n长度需2~12位以内", "确认", "取消");
            return 1;
        }
        if(listitem == 1) {
            Dialog_Show(playerid, Team_ChangeFullName, DIALOG_STYLE_INPUT, "修改全称", "请在下方输入团队的全称\n长度需2~24位以内", "确认", "取消");
            return 1;
        }
        if(listitem == 2) {
            format(TeamInfo[SelectedTeam[playerid]][Description], TeamDescription_Length, " ");
            Dialog_Show(playerid, Team_DescriptionDialog, DIALOG_STYLE_INPUT, "团队简介", "{FF0000}请输入简介\n不能为空并且输入字符在(1-512)之内", "确定", "返回");
            return 1;
        }
        if(listitem == 3) { //管理成员
            Dialog_Show(playerid, Team_MemberManageDialog, DIALOG_STYLE_LIST, "团队管理", "成员列表\n邀请加入", "确定", "返回");
            return 1;
        }
        // if(listitem == 4) { //转让团队   转让团队放在管理成员里好了，这样子选中玩家就可以转让了
        //     if(team_GetLevel(PlayerInfo[playerid][ID],SelectedTeam[playerid])!=2) return SendClientMessage(playerid, Color_Team, "[团队]您不是团队的创始人");
        // }
        if(listitem == 4) {
            if(team_GetLevel(PlayerInfo[playerid][ID], SelectedTeam[playerid]) != 2) return SendClientMessage(playerid, Color_Team, "[团队]您不是团队的创始人");
            Dialog_Show(playerid, Team_DisbandDialog, DIALOG_STYLE_MSGBOX, "最后一次确认", "您真的确认要解散您的团队吗?这是最后一次提示!", "否", "是");
            return 1;
        }
    }
    return 1;
}
Dialog:Team_MemberManageDialog(playerid, response, listitem, inputtext[]) { 
    if(response) {
        if(listitem == 0) {
            team_MemberList(playerid, SelectMemberPage[playerid], TeamInfo[SelectedTeam[playerid]][TID]);
        }
        if(listitem == 1) {
            Dialog_Show(playerid, Team_InviteDialog, DIALOG_STYLE_INPUT, "邀请加入", "请在下方输入您想要邀请的玩家ID", "确定", "取消");
        }
    } else {
        team_Manage(playerid, SelectedTeam[playerid]); //返回管理界面
    }
    return 1;
}
Dialog:Team_InviteDialog(playerid, response, listitem, inputtext[]) {
    if(response) {
        if(strlen(inputtext) < 2 || strlen(inputtext) > MAX_PLAYER_NAME) return Dialog_Show(playerid, Team_InviteDialog, DIALOG_STYLE_INPUT, "邀请加入", "请在下方输入您想要邀请的玩家ID", "确定", "取消");
        if(AccountCheck(inputtext)) {

            new Query[128];
            mysql_format(g_Sql, Query, sizeof(Query), "SELECT `ID` FROM `users` WHERE `Name` = '%e'", inputtext); //先去拿他的UID 
            new uid, Cache:result = mysql_query(g_Sql, Query);
            cache_get_value_index_int(0, 0, uid);
            cache_delete(result);

            //然后判断他是不是已经有团队了,如果没有就insert 有就出错.
            mysql_format(g_Sql, Query, sizeof(Query), "SELECT * FROM `teamuser` WHERE `UID` = %d", uid);
            result = mysql_query(g_Sql, Query);          
            new cut = cache_num_rows();
            cache_delete(result);
            if(cut) return Dialog_Show(playerid, Team_InviteDialog, DIALOG_STYLE_INPUT, "邀请加入", "对方已经在某个团队中了\n请在下方输入您想要邀请的玩家ID", "确定", "取消");


            new flag = true;
            for (new i = GetPlayerPoolSize(); i >= 0; i--) {
                if(PlayerInfo[i][ID] == uid && tInvited[i] == -1) //清空玩家的团队信息
                {
                    flag = false;
                    team_Invite(i, TeamInfo[SelectedTeam[playerid]][TID], GetName(playerid), TeamInfo[SelectedTeam[playerid]][FullName]);
                    SetTimerEx("tinviteTimer", 15000, false, "ii", i, playerid);
                    break;
                }
            }
            if(flag) return SendClientMessage(playerid, Color_Team, "[团队]对方不在线或已有邀请...");
            SendClientMessage(playerid, Color_Team, "[团队]您已发出邀请,请等待对方回复,15秒后失效");


            // format(msg, sizeof(msg), "SELECT * FROM `teamuser` ORDER BY Level DESC WHERE TID = %d", TeamInfo[SelectedTeam[playerid]][TID]); //判断团队是否人数上限 默认100个人
            // // 就创始人肯定是在最前面的
            // cf = db_query(Teamdb, msg);
            // cut = db_num_rows(cf);
            // db_free_result(cf);
            // if(cut >= MAX_TeamMember) return SendClientMessage(playerid, Color_Team, "[团队]人数上限,不可邀请该玩家!");
            // team_Join(uid, TeamInfo[SelectedTeam[playerid]][TID]); //这样就直接把人拉进队了 没有同意与否这一说
            // format(msg, sizeof(msg), "[团队]管理员权限 - 您将%s拉进了队伍", inputtext);
            // SendClientMessage(playerid, Color_Team, msg);
            // team_Manage(playerid, SelectedTeam[playerid]);
            // for (new i = GetPlayerPoolSize(); i >= 0; i--) {
            //     if(PlayerInfo[i][ID] == uid) //清空玩家的团队信息
            //     {
            //         format(PlayerInfo[i][Team], 24, TeamInfo[SelectedTeam[playerid]][FullName]);
            //         break;
            //     }
            // }
            // 要发送请求跟tp一样 太繁琐了
        } else {
            Dialog_Show(playerid, Team_InviteDialog, DIALOG_STYLE_INPUT, "邀请加入", "玩家不存在\n请在下方输入您想要邀请的玩家ID", "确定", "取消");
        }
    } else {
        team_Manage(playerid, SelectedTeam[playerid]);
    }
    return 1;
}
Dialog:Team_MemberDialog(playerid, response, listitem, inputtext[]) { 
    if(response) {
        new msg[1024], idx;
        msg = strtok(inputtext, idx);
        if(strcmp(msg, "上一页") == 0) {
            SelectMemberPage[playerid]--;
            team_MemberList(playerid, SelectMemberPage[playerid], TeamInfo[SelectedTeam[playerid]][TID]);
            return 1;
        }
        if(strcmp(msg, "下一页") == 0) {
            SelectMemberPage[playerid]++;
            team_MemberList(playerid, SelectMemberPage[playerid], TeamInfo[SelectedTeam[playerid]][TID]);
            return 1;
        }
        if(!strlen(inputtext)) return SendClientMessage(playerid, Color_Team, "[团队]发生了内部错误");
        SelectedUID[playerid] = strval(inputtext); // 处理选中后
        if(!TeamInfo[SelectedTeam[playerid]][TID]) return SendClientMessage(playerid, Color_Team, "[团队]错误的团队ID");
        if(strcmp(TeamInfo[SelectedTeam[playerid]][FullName], PlayerInfo[playerid][Team], false)) return SendClientMessage(playerid, Color_Team, "[团队]您不是该团队的成员");
        // 漏了个检测该用户是不是这个团队的成员 不然的话会出现跨队操作…… 2020.3.23修复
        new title[48];
        format(title, sizeof(title), "成员 - UID:%d", SelectedUID[playerid]);
        format(msg, sizeof msg, "请选择\n拉TA过来\n传送到TA\n给予管理员\n取消管理员\n{FF0000}转让团队\n移出团队", TeamInfo[SelectedTeam[playerid]][ShortName], TeamInfo[SelectedTeam[playerid]][TID], TeamInfo[SelectedTeam[playerid]][FullName]);
        Dialog_Show(playerid, MemberManagerInviteList, DIALOG_STYLE_LIST, "详细信息", msg, "确定", "返回");
        return 1;
    } 
    //如果是管理员的话就返回到管理界面 否则返回我的团队界面
    // if(team_GetLevel(PlayerInfo[playerid][ID], SelectedTeam[playerid])) team_Manage(playerid, SelectedTeam[playerid]);
    // if(team_GetLevel(PlayerInfo[playerid][ID], SelectedTeam[playerid])) 
    // {
    //     // team_MemberList(playerid, SelectMemberPage[playerid], TeamInfo[SelectedTeam[playerid]][TID]);
    // }
    new msg[256];
    format(msg, sizeof msg, "团队简称 %s TID:%d\n全称 %s\n团队介绍\n团队成员\n管理团队", TeamInfo[SelectedTeam[playerid]][ShortName], TeamInfo[SelectedTeam[playerid]][TID], TeamInfo[SelectedTeam[playerid]][FullName]);
    Dialog_Show(playerid, Team_Dialog, DIALOG_STYLE_LIST, "详细信息", msg, "确定", "返回");
    return 1;
}
Dialog:MemberManagerInviteList(playerid, response, listitem, inputtext[]) { 
    if(response) {
        if(listitem == 1) {
            team_Get(-1, playerid);
            return 1;
        }
        if(listitem == 2) {
            team_Goto(-1, playerid);
            return 1;
        }
        if(listitem == 3) {
            if(team_GetLevel(SelectedUID[playerid], SelectedTeam[playerid])) {
                SendClientMessage(playerid, Color_Team, "[团队]对方已经是管理员了");
                team_Manage(playerid, SelectedTeam[playerid]);
                return 1;
            } 
            if(team_GetLevel(SelectedUID[playerid], SelectedTeam[playerid]) < 2) {
                SendClientMessage(playerid, Color_Team, "[团队]您的权限不足");
                team_Manage(playerid, SelectedTeam[playerid]);
                return 1;
            }
            new msg[96]; //这里得判断对方的level等级 如果是2的话 就不应该再去设置了 1的话也是
            team_GiveAdmin(SelectedUID[playerid], 1);
            // SendClientMessage(playerid, Color_Team, "[团队]给予对方管理员权限");
            format(msg, sizeof(msg), "[团队]管理员%s(%d)给予团队成员UID:%d管理员", GetName(playerid), playerid, SelectedUID[playerid]);
            team_SCM(msg, PlayerInfo[playerid][Team]);
            return 1;
        }
        if(listitem == 4) {
            if(team_GetLevel(SelectedUID[playerid], SelectedTeam[playerid]) == 2) 
            {
                SendClientMessage(playerid, Color_Team, "[团队]不可对创始人进行操作");
                team_Manage(playerid, SelectedTeam[playerid]);
                return 1;
            }
            if(team_GetLevel(SelectedUID[playerid], SelectedTeam[playerid]) < 2) {
                SendClientMessage(playerid, Color_Team, "[团队]您的权限不足");
                team_Manage(playerid, SelectedTeam[playerid]);
                return 1;
            }
            new msg[96]; //这里得判断对方的level等级 如果是2的话 就不应该再去设置了
            team_GiveAdmin(SelectedUID[playerid], 0);
            // SendClientMessage(playerid, Color_Team, "[团队]取消对方管理员权限");
            format(msg, sizeof(msg), "[团队]管理员%s(%d)取消了团队成员UID:%d的管理员权限", GetName(playerid), playerid, SelectedUID[playerid]);
            team_SCM(msg, PlayerInfo[playerid][Team]);
            return 1;
        }
        if(listitem == 5) {
            if(team_GetLevel(PlayerInfo[playerid][ID], SelectedTeam[playerid]) != 2) 
            {
                SendClientMessage(playerid, Color_Team, "[团队]您不是团队的创始人不可转让团队");
                team_Manage(playerid, SelectedTeam[playerid]);
                return 1;
            }
            Dialog_Show(playerid, Team_TransferDialog, DIALOG_STYLE_MSGBOX, "转让团队最后一次确认", "{FF0000}您确定要转让团队吗?这是最后一次确认,请仔细考虑!", "否", "是");
            return 1;
        }
        if(listitem == 6) {
            if(SelectedUID[playerid] == PlayerInfo[playerid][ID]) 
            {
                SendClientMessage(playerid, Color_Team, "[团队]您不可移除您自己,如需退出团队请在'我的团队'中设置");
                team_Manage(playerid, SelectedTeam[playerid]);
                return 1;
            }
            new msg[96];
            format(msg, sizeof(msg), "[团队]管理员%s(%d)移除了团队成员UID:%d", GetName(playerid), playerid, SelectedUID[playerid]);
            team_SCM(msg, PlayerInfo[playerid][Team]);
            team_Kick(SelectedUID[playerid]);
            return 1;
        }
        team_Manage(playerid, SelectedTeam[playerid]);
        return 1;
    }
    team_Manage(playerid, SelectedTeam[playerid]);
    return 1;
}
Dialog:Team_TransferDialog(playerid, response, listitem, inputtext[]) { 
    if(!response) {
        if(SelectedUID[playerid] == PlayerInfo[playerid][ID]) return SendClientMessage(playerid, Color_Team, "[团队]您不可转让给您自己");
        team_Transfer(TeamInfo[SelectedTeam[playerid]][TID], SelectedUID[playerid], PlayerInfo[playerid][ID]);
        new msg[128];
        format(msg, sizeof(msg), "[团队]创始人由UID:%d 变更为 UID:%d", PlayerInfo[playerid][ID], SelectedUID[playerid]);
        team_SCM(msg, PlayerInfo[playerid][Team]);
    } else {
        team_Manage(playerid, SelectedTeam[playerid]);
    }
    return 1;
}
Dialog:Team_ListDialog(playerid, response, listitem, inputtext[]) { 
    if(response) {
        new msg[1200], idx;
        msg = strtok(inputtext, idx);
        if(strcmp(msg, "上一页") == 0) {
            SelectTeamPage[playerid]--;
            team_List(playerid, SelectTeamPage[playerid]);
            return 1;
        }
        if(strcmp(msg, "下一页") == 0) {
            SelectTeamPage[playerid]++;
            team_List(playerid, SelectTeamPage[playerid]);
            return 1;
        }
        SelectedTeam[playerid] = strval(inputtext);
        if(!TeamInfo[SelectedTeam[playerid]][TID]) return SendClientMessage(playerid, Color_Team, "[团队]错误的团队ID");
        format(msg, sizeof msg, "团队简称 %s TID:%d\n全称 %s\n团队介绍\n团队成员\n管理团队", TeamInfo[SelectedTeam[playerid]][ShortName], TeamInfo[SelectedTeam[playerid]][TID], TeamInfo[SelectedTeam[playerid]][FullName]);
        Dialog_Show(playerid, Team_Dialog, DIALOG_STYLE_LIST, "详细信息", msg, "确定", "返回");
        return 1;
    }
    // team_List(playerid, SelectTeamPage[playerid]);
    return 1;
}
Dialog:Team_Dialog(playerid, response, listitem, inputtext[]) { 
    if(response) {
        if(listitem <= 1) return team_List(playerid, SelectTeamPage[playerid]);
        if(listitem == 2) return Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "介绍", TeamInfo[SelectedTeam[playerid]][Description], "了解", "");
        if(listitem == 3) {
            team_MemberList(playerid, SelectMemberPage[playerid], TeamInfo[SelectedTeam[playerid]][TID]);
            return 1;
        }
        if(listitem == 4) return team_Manage(playerid, SelectedTeam[playerid]); //选中的团队ID
        if(listitem == 5) {
            if(team_GetLevel(PlayerInfo[playerid][ID], SelectedTeam[playerid]) == 2) return SendClientMessage(playerid, Color_Team, "[团队]您是团队的创始人,如需解散团队请在成员列表内找到自己");
            new msg[96];
            format(msg, sizeof(msg), "[团队]%s(%d)离开了团队...", GetName(playerid), playerid);
            team_SCM(msg, PlayerInfo[playerid][Team]);
            team_Kick(PlayerInfo[playerid][ID]);
            return 1;
        }
        return 1;
    }
    team_List(playerid, SelectTeamPage[playerid]);
    return 1;
}


stock GetTeamCount() {
    new Cache:result, TeamCount;
    result = mysql_query(g_Sql, "SELECT COUNT(*) FROM `teamsystem`");
    cache_get_value_index_int(0, 0, TeamCount);
    cache_delete(result);
    return TeamCount;
}
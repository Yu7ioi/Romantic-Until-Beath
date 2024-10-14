/*
    This server is modified from 5F free residential open source, Prace open source
    To the makers of Prace:[Ghost]Rui ushio_p [Ghost]Dylan
    Tribute to the makers of the free settlement :[ITC]dyq [ITC]fangye [ITC]Super_wlc [ITC
    Special thanks to ryddawn and technical advisor [Fire]KiVen OBJ:JoshenKM;

								The MIT License  
								
	Copyright (c) <2019-2020> YuCarl77 & ryddawn 
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
	版权(c) <2019-2020> YuCarl77 & ryddawn 

	使用该许可证的软件被授予以下权限，免费，任何人可以得到这个软件及其相关文档的一个拷贝，
	并且经营该软件不受任何限制，包括无限制的使用、复制、修改、合并、出版、发行、发放从属证书、或者出售该软件的拷贝的权利。
	同时允许获得这些软件的用户享受这些权利，使其服从下面的条件：  
	
	以上的版权通知和权限通知应该包含在所有该软件的拷贝中或者是其他该软件的真实部分中。
	
	该软件按本来的样子提供，没有任何形式的担保，不管是明确地或者暗含的，包含这些但是不受商业性质的担保的限制。
	适合一个特定的用途并且不受侵犯。作者和版权持有人在任何场合对使用该软件涉及的任何要求、损害或者其他责任都不应负责。
	不管它是正在起作用还是只是合同形式、民事侵权或是其他方式，如由它引起，在其作用范围内、与该软件有联系、该软件的使用或者有这个软件引起的其他行为。  
	=====================================   
*/



#define MAX_CAMERAS 30 //镜头最大数量
// #define CAMERA_DIALOG_MAIN 520 //主窗口ID
#define CAMERA_DIALOG_ROWS 10 //每页显示的行数


#define MOVE_FORWARD    		(1)
#define MOVE_BACK       		(2)
#define MOVE_LEFT       		(3)
#define MOVE_RIGHT      		(4)
#define MOVE_FORWARD_LEFT       (5)
#define MOVE_FORWARD_RIGHT      (6)
#define MOVE_BACK_LEFT          (7)
#define MOVE_BACK_RIGHT         (8)


forward PlayCamera(playerid, SID, EID);

enum e_PLAYER_FLY_MODE {
    flyMode,
    flyType,
    flyObject,
    flyDirection,
    flyKeys[2],
    flyTick,
    Float:flyMovementSpeed
};
new g_FlyMode[MAX_PLAYERS][e_PLAYER_FLY_MODE];


enum Cinfo {
    CameraStatus[MAX_CAMERAS],
    Float:CameraX[MAX_CAMERAS],
    Float:CameraY[MAX_CAMERAS],
    Float:CameraZ[MAX_CAMERAS],
    Float:CameraAtX[MAX_CAMERAS],
    Float:CameraAtY[MAX_CAMERAS],
    Float:CameraAtZ[MAX_CAMERAS],
    MoveTime[MAX_CAMERAS]
}
new CameraInfo[MAX_PLAYERS][Cinfo]; //记录镜头信息

enum CreateInfo {
    CreateStatus, //创建状态.0(无),1(第一个坐标),2(第二个坐标)
    Float:CreateX,
    Float:CreateY,
    Float:CreateZ,
    Float:CreateAtX,
    Float:CreateAtY,
    Float:CreateAtZ
}
new CreateCamera[MAX_PLAYERS][CreateInfo]; //临时记录创建镜头信息

enum PCI //PlayerCameraInfo
{
    Using, //使用中的ID
    Page, //记录页数
    StartID, //第一个播放的镜头ID
    EndID, //结束播放的镜头ID
    cTimerID, //计时器ID
    TF //记录正反
}
new PlayerCamera[MAX_PLAYERS][PCI];

stock Initialize_Camera() //初始化镜头系统
{
    printf("视角系统 - By YuCarl77 & ryddawn");
    for (new i = 0; i < MAX_PLAYERS; i++) {
        ResetCreateInfo(i);
        ResetCameraInfo(i);
        PlayerCamera[i][Using] = -1;
        PlayerCamera[i][Page] = 1;
        PlayerCamera[i][StartID] = -1;
        PlayerCamera[i][EndID] = -1;
    }
}

stock Camera_OnPlayerConnect(const playerid) { //玩家连接时清空上一个玩家镜头信息
    ResetCreateInfo(playerid);
    g_FlyMode[playerid][flyMode] = 1;
    g_FlyMode[playerid][flyType] = 0;
    g_FlyMode[playerid][flyObject] = INVALID_OBJECT_ID;
    g_FlyMode[playerid][flyDirection] = 0;
    g_FlyMode[playerid][flyKeys][0] = 0;
    g_FlyMode[playerid][flyKeys][1] = 0;
    g_FlyMode[playerid][flyTick] = 0;
    g_FlyMode[playerid][flyMovementSpeed] = 50.0;

    PlayerCamera[playerid][Using] = -1;
    PlayerCamera[playerid][Page] = 1;
    PlayerCamera[playerid][StartID] = -1;
    PlayerCamera[playerid][EndID] = -1;
}
stock Camera_OnPlayerDisConnect(const playerid) { //玩家下线时清空OBJ
    if(g_FlyMode[playerid][flyType] != 0)
    {
        DestroyDynamicObject(g_FlyMode[playerid][flyObject]);
        g_FlyMode[playerid][flyObject] = INVALID_OBJECT_ID;
    }
}


CMD:camera(playerid, params[]) { //玩家输入camera指令
    if(PlayerInfo[playerid][tvzt] || IsPlayerInAnyVehicle(playerid))
    {
        SendClientMessage(playerid, Color_White, "[摄像机]无法在TV状态或特殊状态下调用");
        return 1;
    }
    new msg[128];
    format(msg, sizeof msg, "====== 播放镜头 ======\n====== 停止播放 ======\n====== 编辑镜头 ======\n====== 视角类型 ======\n====== 视角移速 ======");
    Dialog_Show(playerid, CAMERA_DIALOG_MAIN, DIALOG_STYLE_LIST, "我的镜头", msg, "确定", "取消");
    return 1;
}
CMD:ssj(playerid, params[]) return cmd_camera(playerid, params); //多指令指向
CMD:jt(playerid, params[]) return cmd_camera(playerid, params);
CMD:cam(playerid, params[]) return cmd_camera(playerid, params);

// public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) {

//     if(dialogid == CAMERA_DIALOG_MAIN) //主窗口
//     {
//         if(response) {
//             switch (listitem) {
//                 case 0:{
//                     Camera_SelectDialog(playerid, PlayerCamera[playerid][Page]);
//                     return 1;
//                 }
//                 case 1:{
//                     KillTimer(PlayerCamera[playerid][cTimerID]);
//                     SetCameraBehindPlayer(playerid);
//                     return 1;
//                 }
//                 case 2:{
//                     Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
//                     return 1;
//                 }
//             }
//         }
//     }
//     if(dialogid == CAMERA_DIALOG_MAIN + 1) //调整窗口
//     {
//         if(response) {
//             for (new i; i < CAMERA_DIALOG_ROWS; i++) {
//                 if(listitem == i) {
//                     new row;
//                     new title[32];
// 					new msg[1024];
//                     row = (PlayerCamera[playerid][Page] - 1) * CAMERA_DIALOG_ROWS + i;
//                     if(CameraInfo[playerid][CameraStatus][row] == 0) format(msg, sizeof(msg), "创建镜头", msg);
//                     else if(CameraInfo[playerid][CameraStatus][row] == 1) format(msg, sizeof(msg), "删除镜头\n查看\nMoveTime[%d]", CameraInfo[playerid][MoveTime][row]);
//                     format(title, sizeof(title), "镜头[%d]", row);
//                     PlayerCamera[playerid][Using] = row;
//                     ShowPlayerDialog(playerid, CAMERA_DIALOG_MAIN + 2, DIALOG_STYLE_LIST, title, msg, "确定", "取消");
//                     return 1;
//                 }
//             }
//             switch (listitem) {
//                 case 10:{
//                     PlayerCamera[playerid][Page]--;
//                     Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
//                     return 1;
//                 }
//                 case 11:{
//                     PlayerCamera[playerid][Page]++;
//                     Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
//                     return 1;
//                 }
//             }
//         }
//     }
//     if(dialogid == CAMERA_DIALOG_MAIN + 2) //显示镜头信息
//     {
//         if(response) {
//             new nUsing = PlayerCamera[playerid][Using];
//             switch (listitem) {
//                 case 0:{
//                     if(CameraInfo[playerid][CameraStatus][nUsing] == 0) {
//                         CreateCamera[playerid][CreateStatus] = 1;
//                         SCM(playerid, Color_White, "[摄像机]开始创建镜头");
//                         return 1;
//                     }
//                     if(CameraInfo[playerid][CameraStatus][nUsing] == 1) {
//                         DeletCamera(playerid, nUsing);
//                         Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
//                         return 1;
//                     }
//                 }
//                 case 1:{
//                     SetPlayerCameraPos(playerid, CameraInfo[playerid][CameraX][nUsing], CameraInfo[playerid][CameraY][nUsing], CameraInfo[playerid][CameraZ][nUsing]);
//                     SetPlayerCameraLookAt(playerid, CameraInfo[playerid][CameraAtX][nUsing], CameraInfo[playerid][CameraAtY][nUsing], CameraInfo[playerid][CameraAtZ][nUsing]);
//                     CreateCamera[playerid][CreateStatus] = 3;
//                     SCM(playerid, Color_White, "[摄像机]按Y返回");
//                     return 1;
//                 }
//                 case 2:{
//                     ShowPlayerDialog(playerid, CAMERA_DIALOG_MAIN + 3, DIALOG_STYLE_INPUT, "修改movetime", "输入修改数值", "确定", "取消");
//                     return 1;
//                 }

//             }
//         }
//     }
//     if(dialogid == CAMERA_DIALOG_MAIN + 3) //修改movetime窗口
//     {
//         if(response) {
//             new id = PlayerCamera[playerid][Using];
//             CameraInfo[playerid][MoveTime][id] = strval(inputtext);
//             Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
//             return 1;
//         }
//     }
//     if(dialogid == CAMERA_DIALOG_MAIN + 4) //选择播放镜头窗口
//     {
//         if(response) {
//             for (new i; i < CAMERA_DIALOG_ROWS; i++) {
//                 if(listitem == i) {
//                     new row;
//                     row = (PlayerCamera[playerid][Page] - 1) * CAMERA_DIALOG_ROWS + i;
//                     if(CreateCamera[playerid][CreateStatus] == 4) {
//                         PlayerCamera[playerid][EndID] = row;
//                         PlayerCamera[playerid][cTimerID] = SetTimerEx("PlayCamera", 3000, false, "iii", playerid, PlayerCamera[playerid][StartID], PlayerCamera[playerid][EndID]);

//                         if(PlayerCamera[playerid][StartID] < PlayerCamera[playerid][EndID]) {
//                             PlayerCamera[playerid][TF] = 1;
//                         } else {
//                             PlayerCamera[playerid][TF] = 2;
//                         }
//                         ResetCreateInfo(playerid);
//                         return 1;
//                     } else {
//                         PlayerCamera[playerid][StartID] = row;
//                         CreateCamera[playerid][CreateStatus] = 4;
//                         Camera_SelectDialog(playerid, PlayerCamera[playerid][Page]);
//                     }
//                     return 1;
//                 }
//             }
//             switch (listitem) {
//                 case 10:{
//                     PlayerCamera[playerid][Page]--;
//                     Camera_SelectDialog(playerid, PlayerCamera[playerid][Page]);
//                     return 1;
//                 }
//                 case 11:{
//                     PlayerCamera[playerid][Page]++;
//                     Camera_SelectDialog(playerid, PlayerCamera[playerid][Page]);
//                     return 1;
//                 }
//             }
//         }
//     }

//     return 1;
// }

// public OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
//     if(newkeys & KEY_YES) { //按下Y 
//         CreatingCamera(playerid);
//         return 1;
//     }
//     return 1;
// }

public PlayCamera(playerid, SID, EID) {
    if(PlayerCamera[playerid][TF] == 1) {
        InterpolateCameraPos(playerid, CameraInfo[playerid][CameraX][SID], CameraInfo[playerid][CameraY][SID], CameraInfo[playerid][CameraZ][SID], \
            CameraInfo[playerid][CameraX][SID + 1], CameraInfo[playerid][CameraY][SID + 1], CameraInfo[playerid][CameraZ][SID + 1], CameraInfo[playerid][MoveTime][SID], CAMERA_MOVE);
        InterpolateCameraLookAt(playerid, CameraInfo[playerid][CameraAtX][SID], CameraInfo[playerid][CameraAtY][SID], CameraInfo[playerid][CameraAtZ][SID], \
            CameraInfo[playerid][CameraAtX][SID + 1], CameraInfo[playerid][CameraAtY][SID + 1], CameraInfo[playerid][CameraAtZ][SID + 1], CameraInfo[playerid][MoveTime][SID], CAMERA_MOVE);
        if(PlayerCamera[playerid][StartID] == PlayerCamera[playerid][EndID]) {
            SetCameraBehindPlayer(playerid);
            return 1;
        } else {
            PlayerCamera[playerid][StartID]++;
            new stid = PlayerCamera[playerid][StartID];
            if(CameraInfo[playerid][CameraStatus][stid] == 0) {
                SetCameraBehindPlayer(playerid);
                return 1;
            } else {
                PlayerCamera[playerid][cTimerID] = SetTimerEx("PlayCamera", CameraInfo[playerid][MoveTime][SID] - 500, false, "iii", playerid, PlayerCamera[playerid][StartID], PlayerCamera[playerid][EndID]);
                return 1;
            }
        }
    } else {
        InterpolateCameraPos(playerid, CameraInfo[playerid][CameraX][SID], CameraInfo[playerid][CameraY][SID], CameraInfo[playerid][CameraZ][SID], \
            CameraInfo[playerid][CameraX][SID - 1], CameraInfo[playerid][CameraY][SID - 1], CameraInfo[playerid][CameraZ][SID - 1], CameraInfo[playerid][MoveTime][SID - 1], CAMERA_MOVE);
        InterpolateCameraLookAt(playerid, CameraInfo[playerid][CameraAtX][SID], CameraInfo[playerid][CameraAtY][SID], CameraInfo[playerid][CameraAtZ][SID], \
            CameraInfo[playerid][CameraAtX][SID - 1], CameraInfo[playerid][CameraAtY][SID - 1], CameraInfo[playerid][CameraAtZ][SID - 1], CameraInfo[playerid][MoveTime][SID - 1], CAMERA_MOVE);
        PlayerCamera[playerid][StartID]--;
        // printf("%d %d", PlayerCamera[playerid][StartID], PlayerCamera[playerid][EndID]);
        if(PlayerCamera[playerid][StartID] + 1 == PlayerCamera[playerid][EndID]) {
            SetCameraBehindPlayer(playerid);
            return 1;
        } else {

            PlayerCamera[playerid][cTimerID] = SetTimerEx("PlayCamera", CameraInfo[playerid][MoveTime][SID] - 500, false, "iii", playerid, PlayerCamera[playerid][StartID], PlayerCamera[playerid][EndID]);
            return 1;
        }
    }
}

stock ResetCreateInfo(const playerid) //创建信息初始化
{
    CreateCamera[playerid][CreateStatus] = 0;
    CreateCamera[playerid][CreateX] = 0;
    CreateCamera[playerid][CreateY] = 0;
    CreateCamera[playerid][CreateZ] = 0;
    CreateCamera[playerid][CreateAtX] = 0;
    CreateCamera[playerid][CreateAtY] = 0;
    CreateCamera[playerid][CreateAtZ] = 0;
    return 1;
}
stock ResetCameraInfo(const playerid) //camera信息初始化
{
    for (new i = 0; i < MAX_CAMERAS; i++) {
        CameraInfo[playerid][CameraStatus][i] = 0;
        CameraInfo[playerid][CameraX][i] = 0;
        CameraInfo[playerid][CameraY][i] = 0;
        CameraInfo[playerid][CameraZ][i] = 0;
        CameraInfo[playerid][CameraAtX][i] = 0;
        CameraInfo[playerid][CameraAtY][i] = 0;
        CameraInfo[playerid][CameraAtZ][i] = 0;
        CameraInfo[playerid][MoveTime][i] = 5000;
    }
    return 1;
}
stock NewCamera(const playerid, const CameraID, Float:X, Float:Y, Float:Z, Float:AtX, Float:AtY, Float:AtZ) //新建镜头
{
    CameraInfo[playerid][CameraStatus][CameraID] = 1;
    CameraInfo[playerid][CameraX][CameraID] = X;
    CameraInfo[playerid][CameraY][CameraID] = Y;
    CameraInfo[playerid][CameraZ][CameraID] = Z;
    CameraInfo[playerid][CameraAtX][CameraID] = AtX;
    CameraInfo[playerid][CameraAtY][CameraID] = AtY;
    CameraInfo[playerid][CameraAtZ][CameraID] = AtZ;
    return 1;
}
stock DeletCamera(const playerid, const CameraID) //删除镜头
{
    CameraInfo[playerid][CameraStatus][CameraID] = 0;
    CameraInfo[playerid][CameraX][CameraID] = 0;
    CameraInfo[playerid][CameraY][CameraID] = 0;
    CameraInfo[playerid][CameraZ][CameraID] = 0;
    CameraInfo[playerid][CameraAtX][CameraID] = 0;
    CameraInfo[playerid][CameraAtZ][CameraID] = 0;
    CameraInfo[playerid][CameraAtY][CameraID] = 0;
    return 1;
}
stock CreatingCamera(const playerid) //创建镜头
{
    if(CreateCamera[playerid][CreateStatus] != 0) {
        if(CreateCamera[playerid][CreateStatus] == 1) {
            GetPlayerCameraPos(playerid, CreateCamera[playerid][CreateX], CreateCamera[playerid][CreateY], CreateCamera[playerid][CreateZ]);
            CreateCamera[playerid][CreateStatus] = 2;
            SCM(playerid, Color_White, "[摄像机]创建镜头位置成功");
            SCM(playerid, Color_White, "[摄像机]第二步 - 创建镜头看的位置");
            return 1;
        }
        if(CreateCamera[playerid][CreateStatus] == 2) {
            GetPlayerCameraPos(playerid, CreateCamera[playerid][CreateAtX], CreateCamera[playerid][CreateAtY], CreateCamera[playerid][CreateAtZ]);
            NewCamera(playerid, PlayerCamera[playerid][Using], CreateCamera[playerid][CreateX], CreateCamera[playerid][CreateY], CreateCamera[playerid][CreateZ], CreateCamera[playerid][CreateAtX], CreateCamera[playerid][CreateAtY], CreateCamera[playerid][CreateAtZ]);
            Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
            SCM(playerid, Color_White, "[摄像机]该镜头创建成功");
            SetPlayerCustomCamera(playerid,0);
            ResetCreateInfo(playerid);
            return 1;
        }
        // if(CreateCamera[playerid][CreateStatus] == 1) {
        //     GetPlayerPos(playerid, CreateCamera[playerid][CreateX], CreateCamera[playerid][CreateY], CreateCamera[playerid][CreateZ]);
        //     CreateCamera[playerid][CreateStatus] = 2;
        //     SCM(playerid, Color_White, "[摄像机]创建镜头位置成功，请按Y当前镜头查看位置，比如高楼.地面");
        //     return 1;
        // }
        // if(CreateCamera[playerid][CreateStatus] == 2) {
        //     GetPlayerPos(playerid, CreateCamera[playerid][CreateAtX], CreateCamera[playerid][CreateAtY], CreateCamera[playerid][CreateAtZ]);
        //     SCM(playerid, Color_White, "[摄像机]创建镜头位置成功");
        //     NewCamera(playerid, PlayerCamera[playerid][Using], CreateCamera[playerid][CreateX], CreateCamera[playerid][CreateY], CreateCamera[playerid][CreateZ], CreateCamera[playerid][CreateAtX], CreateCamera[playerid][CreateAtY], CreateCamera[playerid][CreateAtZ]);
        //     ResetCreateInfo(playerid);
        //     Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
        //     return 1;
        // }
        if(CreateCamera[playerid][CreateStatus] == 3) {
            SetCameraBehindPlayer(playerid);
            ResetCreateInfo(playerid);
            return 1;
        }
    }
    return 1;
}
stock Camera_ShowPlayerDialog(const playerid, page) //给玩家显示镜头窗口
{
    if(PlayerInfo[playerid][tvzt] || IsPlayerInAnyVehicle(playerid))
    {
        SendClientMessage(playerid, Color_White, "[摄像机]无法在TV状态或特殊状态下调用");
        return 1;
    }
    new msg[256], maxpage;
    maxpage = GetMaxPage(MAX_CAMERAS, CAMERA_DIALOG_ROWS);
    if(page < 1) {
        SCM(playerid, Color_White, "[摄像机]页数小于1,跳转到首页");
        page = 1;
        PlayerCamera[playerid][Page] = page;
    }
    if(page > maxpage) {
        SCM(playerid, Color_White, "[摄像机]页数大于最大页数,跳转到末页");
        page = maxpage;
        PlayerCamera[playerid][Page] = page;
    }
    new amax, amin;
    amax = (page * CAMERA_DIALOG_ROWS) - 1;
    amin = (page - 1) * CAMERA_DIALOG_ROWS;
    for (new i = amin; i <= amax; i++) {
        if(CameraInfo[playerid][CameraStatus][i] == 1) format(msg, sizeof(msg), "%s镜头%d [已创建]\n", msg, i);
        else format(msg, sizeof(msg), "%s镜头%d\n", msg, i);
    }
    format(msg, sizeof(msg), "%s上一页\n下一页", msg);
    Dialog_Show(playerid, CAMERA_ADJUST, DIALOG_STYLE_LIST, "镜头", msg, "确定", "取消");
    return 1;
}
stock Camera_SelectDialog(const playerid, page) //玩家选择播放的镜头窗口
{
    new msg[256],
        title[64],
        maxpage;
    format(title, sizeof(title), "选择第一个播放的镜头");
    if(CreateCamera[playerid][CreateStatus] == 4) format(title, sizeof(title), "选择最后一个播放的镜头");
    maxpage = GetMaxPage(MAX_CAMERAS, CAMERA_DIALOG_ROWS);
    if(page < 1) {
        SCM(playerid, Color_White, "[摄像机]页数小于1,跳转到首页");
        page = 1;
        PlayerCamera[playerid][Page] = page;
    }
    if(page > maxpage) {
        SCM(playerid, Color_White, "[摄像机]页数大于最大页数,跳转到末页");
        page = maxpage;
        PlayerCamera[playerid][Page] = page;
    }
    new amax, amin;
    amax = (page * CAMERA_DIALOG_ROWS) - 1;
    amin = (page - 1) * CAMERA_DIALOG_ROWS;
    for (new i = amin; i <= amax; i++) {
        if(CameraInfo[playerid][CameraStatus][i] == 1) format(msg, sizeof(msg), "%s镜头%d [已创建]\n", msg, i);
        else format(msg, sizeof(msg), "%s镜头%d\n", msg, i);
    }
    format(msg, sizeof(msg), "%s上一页\n下一页", msg);
    Dialog_Show(playerid, CAMERA_Play, DIALOG_STYLE_LIST, title, msg, "确定", "取消");
    return 1;
}
// stock GetMaxPage(const count, const rows) //给出总数和每页行数,返回页数
// {
//     new maxpage;
//     maxpage = count % rows;
//     if(maxpage == 0) {
//         maxpage = count / rows;
//     } else {
//         maxpage = (count / rows) + 1;
//     }
//     return maxpage;
// }
/*stock GetPlayerCameraCount(playerid)//获取玩家镜头数量
{
	new count = 0;
	for(new i = 0;i < MAX_CAMERAS;i ++)
	{
		if(CameraInfo[playerid][CameraStatus][i] != 0)
		{
			count ++;
		}
	}
	return count;
}*/



stock MovePlayerCamera(const playerid, const Float:CP[3], const Float:FV[3]) {
    new
    Float:NP[3];

    #define OFFSET_X (FV[0]*6000.0)
    #define OFFSET_Y (FV[1]*6000.0)
    #define OFFSET_Z (FV[2]*6000.0)
    switch (g_FlyMode[playerid][flyDirection]) {
        case MOVE_FORWARD:{
            NP[0] = CP[0] + OFFSET_X;
            NP[1] = CP[1] + OFFSET_Y;
            NP[2] = CP[2] + OFFSET_Z;
        }
        case MOVE_BACK:{
            NP[0] = CP[0] - OFFSET_X;
            NP[1] = CP[1] - OFFSET_Y;
            NP[2] = CP[2] - OFFSET_Z;
        }
        case MOVE_LEFT:{
            NP[0] = CP[0] - OFFSET_Y;
            NP[1] = CP[1] + OFFSET_X;
            NP[2] = CP[2];
        }
        case MOVE_RIGHT:{
            NP[0] = CP[0] + OFFSET_Y;
            NP[1] = CP[1] - OFFSET_X;
            NP[2] = CP[2];
        }
        case MOVE_BACK_LEFT:{
            NP[0] = CP[0] + (-OFFSET_X - OFFSET_Y);
            NP[1] = CP[1] + (-OFFSET_Y + OFFSET_X);
            NP[2] = CP[2] - OFFSET_Z;
        }
        case MOVE_BACK_RIGHT:{
            NP[0] = CP[0] + (-OFFSET_X + OFFSET_Y);
            NP[1] = CP[1] + (-OFFSET_Y - OFFSET_X);
            NP[2] = CP[2] - OFFSET_Z;
        }
        case MOVE_FORWARD_LEFT:{
            NP[0] = CP[0] + (OFFSET_X - OFFSET_Y);
            NP[1] = CP[1] + (OFFSET_Y + OFFSET_X);
            NP[2] = CP[2] + OFFSET_Z;
        }
        case MOVE_FORWARD_RIGHT:{
            NP[0] = CP[0] + (OFFSET_X + OFFSET_Y);
            NP[1] = CP[1] + (OFFSET_Y - OFFSET_X);
            NP[2] = CP[2] + OFFSET_Z;
        }
    }

    MoveDynamicObject(g_FlyMode[playerid][flyObject], NP[0], NP[1], NP[2], g_FlyMode[playerid][flyMovementSpeed]);

    g_FlyMode[playerid][flyTick] = GetTickCount();
}

Dialog:CameraType(playerid, response, listitem, inputtext[]){
    if(response)
    {
        new msg[128], idx;
        msg = strtok(inputtext, idx);
        if(strval(msg) == 2) g_FlyMode[playerid][flyMode] = 2; 
        else g_FlyMode[playerid][flyMode] = 1; 
    }
    AntiCommand[playerid] = 0;
    CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/camera");
    return 1;
}

Dialog:diaCameraSpeed(playerid, response, listitem, inputtext[]){
    if(response)
    {
        
        new msg[128], idx;
        msg = strtok(inputtext, idx);
        new Float:speed = strval(msg);
        if(speed<=0.0 || speed>=2000.0) speed = 50.0;
        g_FlyMode[playerid][flyMovementSpeed] = speed;
    }
    AntiCommand[playerid] = 0;
    CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/camera");
    return 1;
}


stock SetPlayerCustomCamera(const playerid, type = 1) {
	switch (type) {
		case 1,2: {
			TogglePlayerSpectating(playerid, true);

			new
				Float:X,
				Float:Y,
				Float:Z
			;
			GetPlayerPos(playerid, X, Y, Z);
			g_FlyMode[playerid][flyObject] = CreateDynamicObject(19300, X, Y, Z, 0.0, 0.0, 0.0, .playerid = playerid);
			AttachCameraToDynamicObject(playerid, g_FlyMode[playerid][flyObject]);

			g_FlyMode[playerid][flyType] = type;

		    g_FlyMode[playerid][flyDirection] = 0;
		    g_FlyMode[playerid][flyKeys][0] = 0;
		    g_FlyMode[playerid][flyKeys][1] = 0;
		    g_FlyMode[playerid][flyTick] = 0;
		}

	    default: {
			new Float:CP[3];
			GetPlayerCameraPos(playerid, CP[0], CP[1], CP[2]);

			TogglePlayerSpectating(playerid, false);
			SpawnPlayer(playerid);
			DestroyDynamicObject(g_FlyMode[playerid][flyObject]);
		    g_FlyMode[playerid][flyObject] = INVALID_OBJECT_ID;
			g_FlyMode[playerid][flyType] = 0;

            SetTimerEx("@delaySetPlayerPos", 100, false, "ifff", playerid,CP[0], CP[1], CP[2]);
			// SetPlayerPos(playerid, CP[0], CP[1], CP[2]);
		}
	}
}

stock SetPlayerCustomCameraSpeed(const playerid, const Float:speed) {
    g_FlyMode[playerid][flyMovementSpeed] = speed;
	return true;
}

forward @delaySetPlayerPos(const playerid,Float:x,Float:y,Float:z);
public @delaySetPlayerPos(const playerid,Float:x,Float:y,Float:z)
{
    // 这将设置玩家的位置，然后将玩家的z坐标调整到该位置下方最近的实心地面。
    SetPlayerPos(playerid, x, y, z);
    return 1;
}

Dialog:CAMERA_DIALOG_MAIN(playerid, response, listitem, inputtext[]) { //镜头主窗口
    if(response) {
        switch (listitem) {
            case 0:{
                Camera_SelectDialog(playerid, PlayerCamera[playerid][Page]);
                return 1;
            }
            case 1:{
                KillTimer(PlayerCamera[playerid][cTimerID]);
                SetCameraBehindPlayer(playerid);
                return 1;
            }
            case 2:{
                Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
                return 1;
            }
            case 3:{
                new msg[128];
                format(msg, sizeof msg, "您当前的飞行模式为%d\n1:可以用移动键移动和鼠标旋转的相机(默认)\n2:相机只用鼠标旋转(无法移动)",g_FlyMode[playerid][flyMode]);
                Dialog_Show(playerid,CameraType,DIALOG_STYLE_INPUT,"请编辑相机移动视角的类型",msg,"确认","返回");
                // 视角类型
                return 1;
            }
            case 4:{
                new msg[128];
                format(msg, sizeof msg, "您当前的视角移速为%.2f\n请在下方输入想要的数值,数值越大则越快,反之越慢",g_FlyMode[playerid][flyMovementSpeed]);
                Dialog_Show(playerid,diaCameraSpeed,DIALOG_STYLE_INPUT,"请编辑相机移动视角的速度",msg,"确认","返回");
                // 视角移速
                return 1;
            }
        }
        return 1;
    }
    return 1;
}
Dialog:CAMERA_ADJUST(playerid, response, listitem, inputtext[]) { //调整窗口
    if(response) {
        for (new i; i < CAMERA_DIALOG_ROWS; i++) {
            if(listitem == i) {
                new row;
                new title[32];
                new msg[1024];
                row = (PlayerCamera[playerid][Page] - 1) * CAMERA_DIALOG_ROWS + i;
                if(CameraInfo[playerid][CameraStatus][row] == 0) format(msg, sizeof(msg), "创建镜头", msg);
                else if(CameraInfo[playerid][CameraStatus][row] == 1) format(msg, sizeof(msg), "删除镜头\n查看\nMoveTime[%d]", CameraInfo[playerid][MoveTime][row]);
                format(title, sizeof(title), "镜头[%d]", row);
                PlayerCamera[playerid][Using] = row;
                Dialog_Show(playerid, CAMERA_INFO, DIALOG_STYLE_LIST, title, msg, "确定", "取消");
                return 1;
            }
        }
        switch (listitem) {
            case 10:{
                PlayerCamera[playerid][Page]--;
                Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
                return 1;
            }
            case 11:{
                PlayerCamera[playerid][Page]++;
                Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
                return 1;
            }
        }
        return 1;
    }
    AntiCommand[playerid] = 0;
    CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/camera");
    return 1;
}
Dialog:CAMERA_INFO(playerid, response, listitem, inputtext[]) { //显示镜头信息
    if(response) {
        new nUsing = PlayerCamera[playerid][Using];
        switch (listitem) {
            case 0:{
                if(CameraInfo[playerid][CameraStatus][nUsing] == 0) {
                    CreateCamera[playerid][CreateStatus] = 1;
                    SetPlayerCustomCamera(playerid, g_FlyMode[playerid][flyMode]);
                    SCM(playerid, Color_White, "[摄像机]第一步 - 定好镜头位置后,按鼠标左键创建镜头");
                    return 1;
                }
                if(CameraInfo[playerid][CameraStatus][nUsing] == 1) {
                    DeletCamera(playerid, nUsing);
                    Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
                    return 1;
                }
            }
            case 1:{
                SetPlayerCameraPos(playerid, CameraInfo[playerid][CameraX][nUsing], CameraInfo[playerid][CameraY][nUsing], CameraInfo[playerid][CameraZ][nUsing]);
                SetPlayerCameraLookAt(playerid, CameraInfo[playerid][CameraAtX][nUsing], CameraInfo[playerid][CameraAtY][nUsing], CameraInfo[playerid][CameraAtZ][nUsing]);
                CreateCamera[playerid][CreateStatus] = 3;
                SCM(playerid, Color_White, "[摄像机]按鼠标左键返回");
                return 1;
            }
            case 2:{
                Dialog_Show(playerid, CAMERA_MoveTime, DIALOG_STYLE_INPUT, "修改movetime", "输入修改数值", "确定", "取消");
                return 1;
            }
        }
        return 1;
    }
    AntiCommand[playerid] = 0;
    CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/camera");
    return 1;
}
Dialog:CAMERA_MoveTime(playerid, response, listitem, inputtext[]) { //修改movetime窗口  
    if(response) {
        new id = PlayerCamera[playerid][Using];
        CameraInfo[playerid][MoveTime][id] = strval(inputtext);
        Camera_ShowPlayerDialog(playerid, PlayerCamera[playerid][Page]);
        return 1;
    }
    AntiCommand[playerid] = 0;
    CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/camera");
    return 1;
}
Dialog:CAMERA_Play(playerid, response, listitem, inputtext[]) { //选择播放镜头窗口
    if(response) {
        for (new i; i < CAMERA_DIALOG_ROWS; i++) {
            if(listitem == i) {
                new row;
                row = (PlayerCamera[playerid][Page] - 1) * CAMERA_DIALOG_ROWS + i;
                if(CreateCamera[playerid][CreateStatus] == 4) {
                    PlayerCamera[playerid][EndID] = row;
                    PlayerCamera[playerid][cTimerID] = SetTimerEx("PlayCamera", 200, false, "iii", playerid, PlayerCamera[playerid][StartID], PlayerCamera[playerid][EndID]);

                    if(PlayerCamera[playerid][StartID] < PlayerCamera[playerid][EndID]) {
                        PlayerCamera[playerid][TF] = 1;
                    } else {
                        PlayerCamera[playerid][TF] = 2;
                    }
                    ResetCreateInfo(playerid);
                    return 1;
                } else {
                    PlayerCamera[playerid][StartID] = row;
                    CreateCamera[playerid][CreateStatus] = 4;
                    Camera_SelectDialog(playerid, PlayerCamera[playerid][Page]);
                }
                return 1;
            }
        }
        switch (listitem) {
            case 10:{
                PlayerCamera[playerid][Page]--;
                Camera_SelectDialog(playerid, PlayerCamera[playerid][Page]);
                return 1;
            }
            case 11:{
                PlayerCamera[playerid][Page]++;
                Camera_SelectDialog(playerid, PlayerCamera[playerid][Page]);
                return 1;
            }
        }
        return 1;
    }
    AntiCommand[playerid] = 0;
    CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/camera");
    return 1;
}
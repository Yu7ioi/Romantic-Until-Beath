// IORP Script
// #include <a_samp>
// #define BillDBFile "BillBoards/BillBoards.db"
// OBJID
#define BOARDOBJ5  7606
#define BOARDOBJ3  9314
#define BOARDOBJ7 19353
#define BOARDOBJ  19603
#define BOARDOBJ2  19604 //滚动的OBJ
#define BOARDOBJ4  6963 
#define BOARDOBJ6 8132
#define BOARDOBJ8 7313
// #define BOARDOBJ8 8132
// OBJ价格
#define BOARDOBJPRICE  8000
#define BOARDOBJ2PRICE  8000
#define BOARDOBJ3PRICE  8000
#define BOARDOBJ4PRICE  8000
#define BOARDOBJ5PRICE  8000
#define BOARDOBJ6PRICE 8000
#define BOARDOBJ7PRICE 8000
#define BOARDOBJ8PRICE 8000
#define MAX_BOARDS 8000
// 对话框ID
// #define Dialog_BOARDS Dialog_BUYBOARDS+1
// #define Dialog_BOARDSTEXT Dialog_BUYBOARDS+2
// #define Dialog_BOARDSMINDEX Dialog_BUYBOARDS+3
// #define Dialog_BOARDSMSIZE Dialog_BUYBOARDS+4
// #define Dialog_BOARDSFONTFACE Dialog_BUYBOARDS+5
// #define BOARDSFONTFACESIZE Dialog_BUYBOARDS+6
// #define Dialog_BOARDSBOLD Dialog_BUYBOARDS+7
// #define Dialog_BOARDSFONTCOLOR Dialog_BUYBOARDS+8
// #define Dialog_BOARDSBACKCOLOR Dialog_BUYBOARDS+9
// #define Dialog_BOARDSTextA Dialog_BUYBOARDS+10
// #define Dialog_BOARDSPOS Dialog_BUYBOARDS+11
// #define Dialog_BOARDSTEXT2 Dialog_BUYBOARDS+12
// #define BoardsListDialog Dialog_BUYBOARDS+13 //+14给传送用了 也就是714
// #define Dialog_BUYBOARDS 700
new pInEditTagBoards[MAX_PLAYERS];
new SelectBoardsPage[MAX_PLAYERS];//广告牌选择页 2020.3.2 By YuCarl77
enum bInfo {
    ID,
    ObjID,
    UsersID,
    BText[1024],
    MaterialIndex,
    MaterialSize,
    FontFace[64], //字体
    FontSize,
    Bold,
    FontColor,
    BackColor,
    TextAlignment,
    Float:BX,
    Float:BY,
    Float:BZ,
    Float:BRX,
    Float:BRY,
    Float:BRZ,
    BWorld,
    BInt,
    TagBoard,
    BState,
    Text3D:B3DText
    // 加个3d文字显示
};
new BoardsInfo[MAX_BOARDS][bInfo], BoardsCount;
// DB:BoardsDB;
stock Initialize_Boards() {
    // if(!fexist(BillDBFile)) {
    //     print("公告牌 >> 未找到数据库文件.");
    // } else {
    SetupBillboardTable();
    new Query[128];
    mysql_format(g_Sql, Query, sizeof(Query), "SELECT `boards`.*,`users`.`Name` FROM `boards` JOIN `users` WHERE `boards`.`UsersID` = `users`.`ID`");
    mysql_pquery(g_Sql, Query, "InitializeBoardsQuery");
    return 1;
}
function InitializeBoardsQuery() {
    BoardsCount = cache_num_rows();
    for (new i = 0; i < BoardsCount; i++) {
        new name[MAX_PLAYER_NAME+1];

        cache_get_value_name_int(i, "ID", BoardsInfo[i][ID]);
        cache_get_value_name(i, "Name", name);
        cache_get_value_name_int(i, "ObjID", BoardsInfo[i][ObjID]);
        cache_get_value_name_int(i, "UsersID", BoardsInfo[i][UsersID]);
        cache_get_value_name(i, "Text", BoardsInfo[i][BText], 1024);
        cache_get_value_name_int(i, "MaterialIndex", BoardsInfo[i][MaterialIndex]);
        cache_get_value_name_int(i, "MaterialSize", BoardsInfo[i][MaterialSize]);
        cache_get_value_name(i, "FontFace", BoardsInfo[i][FontFace], 64);
        cache_get_value_name_int(i, "FontSize", BoardsInfo[i][FontSize]);
        cache_get_value_name_int(i, "Bold", BoardsInfo[i][Bold]);
        cache_get_value_name_int(i, "FontColor", BoardsInfo[i][FontColor]);
        cache_get_value_name_int(i, "BackColor", BoardsInfo[i][BackColor]);
        cache_get_value_name_int(i, "TextAlignment", BoardsInfo[i][TextAlignment]);
        cache_get_value_name_float(i, "X", BoardsInfo[i][BX]);
        cache_get_value_name_float(i, "Y", BoardsInfo[i][BY]);
        cache_get_value_name_float(i, "Z", BoardsInfo[i][BZ]);
        cache_get_value_name_float(i, "RX", BoardsInfo[i][BRX]);
        cache_get_value_name_float(i, "RY", BoardsInfo[i][BRY]);
        cache_get_value_name_float(i, "RZ", BoardsInfo[i][BRZ]);
        cache_get_value_name_int(i, "World", BoardsInfo[i][BWorld]);
        cache_get_value_name_int(i, "Int", BoardsInfo[i][BInt]);

        BoardsInfo[i][TagBoard] = CreateDynamicObject(BoardsInfo[i][ObjID], BoardsInfo[i][BX], BoardsInfo[i][BY], BoardsInfo[i][BZ], \
        BoardsInfo[i][BRX], BoardsInfo[i][BRY], BoardsInfo[i][BRZ], BoardsInfo[i][BWorld], BoardsInfo[i][BInt]);
        SetDynamicObjectMaterialText(BoardsInfo[i][TagBoard], BoardsInfo[i][MaterialIndex], BoardsInfo[i][BText], \
        BoardsInfo[i][MaterialSize], BoardsInfo[i][FontFace], BoardsInfo[i][FontSize], BoardsInfo[i][Bold], BoardsInfo[i][FontColor],\
        BoardsInfo[i][BackColor], BoardsInfo[i][TextAlignment]);

        BoardsInfo[i][BState] = 0;

        new temp[96];
        format(temp, sizeof(temp), "用户名:%s\nUID:%d",name,BoardsInfo[i][UsersID]);
        BoardsInfo[i][B3DText] = CreateDynamic3DTextLabel(temp, Color_White, BoardsInfo[i][BX], BoardsInfo[i][BY], BoardsInfo[i][BZ]-1.5, \
        4.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, -1, -1);
    }
    printf("公告牌 >> 读取广告牌[%d]个,剩余可用[%d]个", BoardsCount, MAX_BOARDS - BoardsCount);
}
stock Boards_OnGameModeExit() {
    // db_close(BoardsDB);
    // print("[公告牌]卸载");
    return 1;
}
stock UpdateBoardsData(const i) {
    new msg[2048];
    mysql_format(g_Sql, msg, sizeof(msg), "UPDATE `boards` SET `ObjID` = '%d',`UsersID` = '%d',`Text` = '%e',`MaterialIndex` = '%d',`MaterialSize` = '%d',\
    `FontFace` = '%s',`FontSize` = '%d',`Bold` = '%d',`FontColor` = '%d',`BackColor` = '%d',`TextAlignment` = '%d',`X` = '%f',`Y` = '%f',`Z` = '%f',\
    `RX` = '%f',`RY` = '%f',`RZ` = '%f',`World` = '%d',`Int` = '%d' WHERE `ID` = '%d'",BoardsInfo[i][ObjID], BoardsInfo[i][UsersID], \
    BoardsInfo[i][BText], BoardsInfo[i][MaterialIndex], BoardsInfo[i][MaterialSize], BoardsInfo[i][FontFace], BoardsInfo[i][FontSize], \
    BoardsInfo[i][Bold], BoardsInfo[i][FontColor], BoardsInfo[i][BackColor], BoardsInfo[i][TextAlignment], BoardsInfo[i][BX], \
    BoardsInfo[i][BY], BoardsInfo[i][BZ], BoardsInfo[i][BRX], BoardsInfo[i][BRY], BoardsInfo[i][BRZ], BoardsInfo[i][BWorld], \
    BoardsInfo[i][BInt], BoardsInfo[i][ID]);
    mysql_pquery(g_Sql, msg);
    //print(msg);
    new query[128];
    mysql_format(g_Sql, query, sizeof(query), "SELECT `Name` FROM `users` WHERE `ID` = '%d'", BoardsInfo[i][UsersID]); //然后访问users表去查那个人的名字 不能再用msg了
    new Cache:result = mysql_query(g_Sql, query);
    new name[MAX_PLAYER_NAME+1];
    cache_get_value_index(0, 0, name);
    cache_delete(result);
    format(query, sizeof(query), "用户名:%s\nUID:%d",name,BoardsInfo[i][UsersID]);
    DestroyDynamic3DTextLabel(BoardsInfo[i][B3DText]);
    BoardsInfo[i][B3DText] = CreateDynamic3DTextLabel(query, Color_White, BoardsInfo[i][BX], BoardsInfo[i][BY], BoardsInfo[i][BZ]-1.5, 4.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, -1, -1);
    return 1;
}
stock Boards_OnPlayerConnect(const playerid) { //新增 2020.3.1
    SelectBoardsPage[playerid] = 1;
    pInEditTagBoards[playerid] = -1;
    return 1;
}
stock Boards_OnPlayerDisconnect(const playerid) {
    if(pInEditTagBoards[playerid] != -1) {
        UpdateBoards(pInEditTagBoards[playerid]);
        pInEditTagBoards[playerid] = -1;
    }
    return 1;
}
stock EditBoards(const playerid, const i) {
    // if(i >= MAX_BOARDS || i < 0) return SCM(playerid, -1, "[广告牌] 错误的广告牌ID.");
    if(i >= BoardsCount || i < 0) return SCM(playerid, -1, "[广告牌] 错误的广告牌ID.");
    if(BoardsInfo[i][BState] == 1) return SCM(playerid, -1, "[广告牌] 该广告牌有人正在编辑中!");
    //	if(pInEditTagAttire[playerid] == 1) return SCM(playerid,-1,"[广告牌] 你正在编辑装扮,所以无法编辑该广告牌..");
    BoardsInfo[i][BState] = 1;
    EditDynamicObject(playerid, BoardsInfo[i][TagBoard]);
    return 1;
}
stock DeleteBoards(const playerid,const i) {
    if(BoardsInfo[i][ID] == 0) return -1; //如果一个物品被删了返回-1错误
    DestroyDynamicObject(BoardsInfo[i][TagBoard]);
    DestroyDynamic3DTextLabel(BoardsInfo[i][B3DText]);
    //新增回收给对应价格的金币
    if(playerid!=-1) {
        new money;
        if(BoardsInfo[i][ObjID] == BOARDOBJ) money = BOARDOBJPRICE;
        if(BoardsInfo[i][ObjID] == BOARDOBJ2) money = BOARDOBJ2PRICE;
        if(BoardsInfo[i][ObjID] == BOARDOBJ3) money = BOARDOBJ3PRICE;
        if(BoardsInfo[i][ObjID] == BOARDOBJ4) money = BOARDOBJ4PRICE;
        if(BoardsInfo[i][ObjID] == BOARDOBJ5) money = BOARDOBJ5PRICE;
        if(BoardsInfo[i][ObjID] == BOARDOBJ6) money = BOARDOBJ6PRICE;
        if(BoardsInfo[i][ObjID] == BOARDOBJ7) money = BOARDOBJ7PRICE;
        if(BoardsInfo[i][ObjID] == BOARDOBJ8) money = BOARDOBJ8PRICE;
        GivePlayerCash(playerid, money);
    }
    new Query[128];
    mysql_format(g_Sql, Query, sizeof(Query), "DELETE FROM `boards` WHERE `ID` = '%d'", BoardsInfo[i][ID]);
    mysql_pquery(g_Sql, Query);
    BoardsInfo[i][ID] = 0;
    BoardsInfo[i][UsersID] = -1;
    BoardsCount--;
    return 1;
}
stock CreateBoard(const playerid, const objid, Float:x, Float:y, Float:z, const uid) {
    new i = BoardsCount;
    BoardsCount++;
    BoardsInfo[i][ObjID] = objid;
    BoardsInfo[i][BX] = x;
    BoardsInfo[i][BY] = y;
    BoardsInfo[i][BZ] = z;
    BoardsInfo[i][BRX] = 0;
    BoardsInfo[i][BRY] = 0;
    BoardsInfo[i][BRZ] = 0;
    BoardsInfo[i][BWorld] = GetPlayerVirtualWorld(playerid);
    BoardsInfo[i][BInt] = GetPlayerInterior(playerid);
    BoardsInfo[i][UsersID] = uid;
    format(BoardsInfo[i][BText], 1024, "请修改文字");
    BoardsInfo[i][MaterialIndex] = 0;
    BoardsInfo[i][MaterialSize] = OBJECT_MATERIAL_SIZE_256x128;
    format(BoardsInfo[i][FontFace], 64, "黑体");
    BoardsInfo[i][FontSize] = 28;
    BoardsInfo[i][Bold] = 0;
    BoardsInfo[i][FontColor] = 0xFFFF8200;
    BoardsInfo[i][BackColor] = 0xFF000000;
    BoardsInfo[i][TextAlignment] = OBJECT_MATERIAL_TEXT_ALIGN_CENTER;
    new msg[1024];
    mysql_format(g_Sql, msg, sizeof(msg), "INSERT INTO `boards` (`ObjID`,`UsersID`,`Text`,`MaterialIndex`,`MaterialSize`,`FontFace`,\
    `FontSize`,`Bold`,`FontColor`,`BackColor`,`TextAlignment`,`X`,`Y`,`Z`,`RX`,`RY`,`RZ`,`World`,`Int`) VALUES \
    ('%d','%d','%e','%d','%d','%e','%d','%d','%d','%d','%d','%f','%f','%f','%f','%f','%f','%d','%d')", \
    objid, uid, "请修改文字", 0, OBJECT_MATERIAL_SIZE_256x128, "Arial", 28, 0, 0xFFFF8200, 0xFF000000, OBJECT_MATERIAL_TEXT_ALIGN_CENTER,\
    x, y, z, 0, 0, 0, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid));
    mysql_pquery(g_Sql, msg, "CreateBoardQuery", "d", i);
    return 1;
}
function CreateBoardQuery(i) {
    BoardsInfo[i][ID] = cache_insert_id();
    CreateUpdate_A_Boards(i);
}
stock CreateUpdate_A_Boards(const i) {
    BoardsInfo[i][TagBoard] = CreateDynamicObject(BoardsInfo[i][ObjID], BoardsInfo[i][BX], BoardsInfo[i][BY], BoardsInfo[i][BZ], BoardsInfo[i][BRX], BoardsInfo[i][BRY], BoardsInfo[i][BRZ], BoardsInfo[i][BWorld], BoardsInfo[i][BInt]);
    SetDynamicObjectMaterialText(BoardsInfo[i][TagBoard], BoardsInfo[i][MaterialIndex], BoardsInfo[i][BText], BoardsInfo[i][MaterialSize], BoardsInfo[i][FontFace], BoardsInfo[i][FontSize], BoardsInfo[i][Bold], BoardsInfo[i][FontColor], BoardsInfo[i][BackColor], BoardsInfo[i][TextAlignment]);
    BoardsInfo[i][BState] = 0;
    new query[128];
    mysql_format(g_Sql, query, sizeof(query), "SELECT `Name` FROM `users` WHERE `ID` = '%d'", BoardsInfo[i][UsersID]); //然后访问users表去查那个人的名字 不能再用msg了
    new Cache:result = mysql_query(g_Sql, query);
    new name[MAX_PLAYER_NAME+1];
    cache_get_value_index(0, 0, name);
    cache_delete(result);
    format(query, sizeof(query), "用户名:%s\nUID:%d",name,BoardsInfo[i][UsersID]);
    BoardsInfo[i][B3DText] = CreateDynamic3DTextLabel(query, Color_White, BoardsInfo[i][BX], BoardsInfo[i][BY], BoardsInfo[i][BZ]-1.5, 4.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, -1, -1);
    return 1;
}
stock UpdateBoards(const i) {
    DestroyDynamicObject(BoardsInfo[i][TagBoard]);
    BoardsInfo[i][TagBoard] = CreateDynamicObject(BoardsInfo[i][ObjID], BoardsInfo[i][BX], BoardsInfo[i][BY], BoardsInfo[i][BZ], BoardsInfo[i][BRX], BoardsInfo[i][BRY], BoardsInfo[i][BRZ], BoardsInfo[i][BWorld], BoardsInfo[i][BInt]);
    SetDynamicObjectMaterialText(BoardsInfo[i][TagBoard], BoardsInfo[i][MaterialIndex], BoardsInfo[i][BText], BoardsInfo[i][MaterialSize], BoardsInfo[i][FontFace], BoardsInfo[i][FontSize], BoardsInfo[i][Bold], BoardsInfo[i][FontColor], BoardsInfo[i][BackColor], BoardsInfo[i][TextAlignment]);
    BoardsInfo[i][BState] = 0;
    UpdateBoardsData(i);
    return 1;
}
// stock GetClosestBoards(playerid) {
//     new flag=-1;
//     for(new i=0;i<=MAX_BOARDS;i++){
//         if(IsPlayerInRangeOfPoint(playerid, 1.5, BoardsInfo[i][BX], BoardsInfo[i][BY], BoardsInfo[i][BZ]) && BoardsInfo[i][BState] == 0) {
//             flag = i;
//             break;
//         }
//     }
//     return flag;
// }
// stock EditBorads(const playerid)
stock KeyBoards(const playerid) {
    if(pInEditTagBoards[playerid] != -1) return SCM(playerid, Color_White, "[广告牌] 你正在编辑物件,不可以进行其他操作!");
    //获取最近的广告牌);
    for (new i = 0; i <= BoardsCount; i++) {
    // for (new i = 0; i <= MAX_BOARDS; i++) {
        if(IsPlayerInRangeOfPoint(playerid, 2.0, BoardsInfo[i][BX], BoardsInfo[i][BY], BoardsInfo[i][BZ]) && BoardsInfo[i][BState] == 0 && BoardsInfo[i][ID]) {
            pInEditTagBoards[playerid] = i; //2020.3.25改 默认5太远了
            // pInEditTagBoards[playerid] = BoardsInfo[i][ID];
            break;
        }
    }
    // if(BoardsInfo[pInEditTagBoards[playerid]][ID] == 0) return pInEditTagBoards[playerid] = -1;
    if(pInEditTagBoards[playerid] != -1) // != -1
    {
        if(GetPlayerVirtualWorld(playerid) != BoardsInfo[pInEditTagBoards[playerid]][BWorld] && GetPlayerInterior(playerid) != BoardsInfo[pInEditTagBoards[playerid]][BInt]) return pInEditTagBoards[playerid] = -1;
        if(PlayerInfo[playerid][ID] != BoardsInfo[pInEditTagBoards[playerid]][UsersID]) {
            Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "广告牌系统", "这个广告牌不是你的!", "关闭", "");
            pInEditTagBoards[playerid] = -1;
            return 1;
        }
        // print("5");
        MyBoard(playerid, pInEditTagBoards[playerid]); //2020.3.2 10:13修改
        return 1;
        // MyBoard(playerid,BoardsInfo[pInEditTagBoards[playerid]][ID]); //好像调不出来
    }
    return 0;
}
stock MyBoard(const playerid, const i) {
    new matelsizetext[48];
    if(BoardsInfo[i][MaterialSize] == 10) {
        matelsizetext = "32x32";
    }
    if(BoardsInfo[i][MaterialSize] == 20) {
        matelsizetext = "64x32";
    }
    if(BoardsInfo[i][MaterialSize] == 30) {
        matelsizetext = "64x64";
    }
    if(BoardsInfo[i][MaterialSize] == 40) {
        matelsizetext = "128x32";
    }
    if(BoardsInfo[i][MaterialSize] == 50) {
        matelsizetext = "128x64";
    }
    if(BoardsInfo[i][MaterialSize] == 60) {
        matelsizetext = "128x128";
    }
    if(BoardsInfo[i][MaterialSize] == 70) {
        matelsizetext = "256x32";
    }
    if(BoardsInfo[i][MaterialSize] == 80) {
        matelsizetext = "256x64";
    }
    if(BoardsInfo[i][MaterialSize] == 90) {
        matelsizetext = "256x128";
    }
    if(BoardsInfo[i][MaterialSize] == 100) {
        matelsizetext = "256x256";
    }
    if(BoardsInfo[i][MaterialSize] == 110) {
        matelsizetext = "512x64";
    }
    if(BoardsInfo[i][MaterialSize] == 120) {
        matelsizetext = "512x128";
    }
    if(BoardsInfo[i][MaterialSize] == 130) {
        matelsizetext = "512x256";
    }
    if(BoardsInfo[i][MaterialSize] == 140) {
        matelsizetext = "512x512";
    }
    new boldtext[48];
    if(BoardsInfo[i][Bold] == 1) {
        boldtext = "是";
    }
    if(BoardsInfo[i][Bold] == 0) {
        boldtext = "否";
    }
    new fontcolortext[48];
    format(fontcolortext, sizeof(fontcolortext), "{%06x}▇▇▇{FFFFFF}", BoardsInfo[i][FontColor] >>> 8);
    new backcolortext[48];
    format(backcolortext, sizeof(backcolortext), "{%06x}▇▇▇{FFFFFF}", BoardsInfo[i][BackColor] >>> 8);
    new alignmenttext[48];
    if(BoardsInfo[i][TextAlignment] == 0) {
        alignmenttext = "向左对起";
    }
    if(BoardsInfo[i][TextAlignment] == 1) {
        alignmenttext = "居中对齐";
    }
    if(BoardsInfo[i][TextAlignment] == 2) {
        alignmenttext = "向右对齐";
    }
    new string[512];
    format(string, sizeof(string), "修改文字\n材质指标:[%d]\n材质大小:[%s]\n字体:[%s]\n字体大小:[%d]\n字体加粗:[%s]\n字体颜色:[%s]\n背景颜色:[%s]\n对齐方式:[%s]\n编辑坐标\n{FF0000}回收牌子",
        BoardsInfo[i][MaterialIndex], matelsizetext, BoardsInfo[i][FontFace], BoardsInfo[i][FontSize],
        boldtext, fontcolortext, backcolortext, alignmenttext);
    new str[64];
    format(str, 64, "我的广告牌[%d]", i+1);
    Dialog_Show(playerid, BOARDS, DIALOG_STYLE_LIST, str, string, "选择", "关闭");
    return 1;
}
stock Board_EditMode(const playerid, const objectid, const response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz) {
    if(pInEditTagBoards[playerid] != -1) {
        if(response == EDIT_RESPONSE_FINAL) {
            BoardsInfo[pInEditTagBoards[playerid]][BX] = x;
            BoardsInfo[pInEditTagBoards[playerid]][BY] = y;
            BoardsInfo[pInEditTagBoards[playerid]][BZ] = z;
            BoardsInfo[pInEditTagBoards[playerid]][BRX] = rx;
            BoardsInfo[pInEditTagBoards[playerid]][BRY] = ry;
            BoardsInfo[pInEditTagBoards[playerid]][BRZ] = rz;
            SetDynamicObjectPos(objectid, x, y, z);
            SetDynamicObjectRot(objectid, rx, ry, rz);
            UpdateBoards(pInEditTagBoards[playerid]);
            pInEditTagBoards[playerid] = -1;
            BoardsInfo[pInEditTagBoards[playerid]][BState] = 0; //可能是因为这个导致出现按Y无法编辑的情况
            SCM(playerid, -1, "[小提示]:按空格可以调整你的视角");
        }
        if(response == EDIT_RESPONSE_CANCEL) {
            if(pInEditTagBoards[playerid] != -1) {
                UpdateBoards(pInEditTagBoards[playerid]);
                BoardsInfo[pInEditTagBoards[playerid]][BState] = 0; //尝试修复
                pInEditTagBoards[playerid] = -1;
            }
        }
    }
    return 1;
}
public OnPlayerSelectDynamicObject(playerid, objectid, modelid, Float:x, Float:y, Float:z) {
    for (new i = 0; i < BoardsCount; i++) { // I < max_boards
        if(BoardsInfo[i][TagBoard] == objectid) {
            if(PlayerInfo[playerid][ID] == BoardsInfo[i][UsersID]) {
                MyBoard(playerid, i);
                CancelEdit(playerid);
                pInEditTagBoards[playerid] = i;
            }
        }
    }
    return 1;
}
CMD:b(playerid,cmdtext[]) return cmd_board(playerid,cmdtext);
CMD:board(playerid, cmdtext[]) {
    new tmp[128],idx;
    tmp = strtok(cmdtext, idx);
    if(!strlen(tmp) || !strcmp(tmp, "help", true)) return Dialog_Show(playerid, MessageBox, DIALOG_STYLE_LIST, "广告牌系统", "在广告牌附近按Y可操作广告牌,可回收金币\n/board buy购买广告牌.\n/board list 查看广告牌列表\n/board goto id传送到广告牌", "确定", "返回");
    if(!strcmp(tmp, "buy", true)) { //购买
        if(Board_howMany(playerid) >= 5) return SendClientMessage(playerid, Color_White, "[广告牌]单个玩家上限公告牌数量为5个哦");
    //再写一个判断上限的
        new msgs[512];
        format(msgs, sizeof(msgs), "名字\t模型ID\t价格(金币)\n样式1\t%d\t%d\
        \n样式2\t%d\t%d\
        \n样式3\t%d\t%d\
        \n样式4\t%d\t%d\
        \n样式5\t%d\t%d\
        \n样式6\t%d\t%d\
        \n样式7\t%d\t%d\
        \n流动广告牌\t%d\t%d", BOARDOBJ, BOARDOBJPRICE, BOARDOBJ2, BOARDOBJ2PRICE, BOARDOBJ3, BOARDOBJ3PRICE, BOARDOBJ4, BOARDOBJ4PRICE, BOARDOBJ5, BOARDOBJ5PRICE, BOARDOBJ6, BOARDOBJ6PRICE,BOARDOBJ7, BOARDOBJ7PRICE,BOARDOBJ8, BOARDOBJ8PRICE);
        Dialog_Show(playerid, Dialog_BUYBOARDS, DIALOG_STYLE_TABLIST_HEADERS, "广告牌购买菜单", msgs, "购买", "取消");
        return 1;
    }
    if(!strcmp(tmp, "list", true)) return Board_ShowListDialog(playerid,SelectBoardsPage[playerid]); //查看所有广告牌 列表式
    if(!strcmp(tmp, "goto", true)) {
        tmp = strtok(cmdtext, idx);
        if(mk_strcmp(tmp, "") == 0) return SendClientMessage(playerid, Color_White, "[广告牌] /board goto ID");
        new id = strval(tmp);
        if(id <= 0 || id > BoardsCount) return SendClientMessage(playerid, Color_White, "[广告牌]..错误的牌子ID"); //|| BoardsInfo[id][]
        if(!BoardsInfo[id-1][ID]) return SendClientMessage(playerid, Color_White, "[广告牌]..广告牌不存在哦");
        SetPlayerPos(playerid, BoardsInfo[id-1][BX]+0.5, BoardsInfo[id-1][BY]+0.5, BoardsInfo[id-1][BZ]);
        new msg[256];
        format(msg, sizeof(msg), "[广告牌] %s(%d) {FFFF00}传送到{FF00FF}/board goto %d  牌子%d", GetName(playerid), playerid, id, id);
        SendClientMessageToAll(Color_White, msg);
        return 1;
    }
    if(!strcmp(tmp, "delete", true)) {
        if(PlayerInfo[playerid][AdminLevel] < 4) return 0;
        tmp = strtok(cmdtext, idx);
        if(mk_strcmp(tmp, "") == 0) return SendClientMessage(playerid, Color_White, "[广告牌] /board delete ID 删除广告牌");
        new id = strval(tmp);
        if(id <= 0 || id > BoardsCount) return SendClientMessage(playerid, Color_White, "[广告牌]..错误的牌子ID"); //|| BoardsInfo[id][]
        if(!BoardsInfo[id-1][ID]) return SendClientMessage(playerid, Color_White, "[广告牌]..广告牌不存在哦");
        if(DeleteBoards(-1,id-1) != -1) {
            SendClientMessage(playerid, Color_White, "[广告牌]删除成功..");
            new msg[256];
            format(msg, sizeof(msg), "[管理员] %s 使用了命令 board delete. ", GetName(playerid));
            SCMToAdmins(0x0066FFFF, msg);
            AdminCommandRecord(playerid, "board delete", msg);
        } 
        else 
        {
            SendClientMessage(playerid, Color_White, "[广告牌]删除失败了..");
        }
        return 1;
    }
    SendClientMessage(playerid, Color_White, "[广告牌]指令不存在...");
    return 1;
}
// CMD:selectobj(playerid, cmdtext[]) {
//     SelectObject(playerid);
//     SCM(playerid, -1, "[小提示]:按空格可以调整你的视角");
//     return 1;
// }
stock Board_ShowListDialog(const playerid,const page)//为指定玩家显示赛道选择菜单,page表示赛道页数
{
	new title[64],msg[1024],maxpage;
    // maxpage = BoardsCount / 15 + 1; // 一页15个
    maxpage = GetMaxPage(BoardsCount, 15);
	format(title,64,"广告牌列表 - %i/%i",page,maxpage); 
    format(msg,1024,"ID\t拥有者UID\tOBJID%s\n",msg);
    if(page > 1 && page == maxpage) format(msg,1024,"%s\n上一页",msg);
	if(page ==1 && page!= maxpage) format(msg,1024,"%s\n下一页\n",msg);
	if(page > 1 && page < maxpage) format(msg,1024,"%s\n上一页\n下一页",msg);
    for(new i=(page-1)*15;i<page*15;i++)
	{
        //如果拥有者是自己就高亮
        if(BoardsInfo[i][ID]) { //如果存在的话
            if(BoardsInfo[i][UsersID] == PlayerInfo[playerid][ID]) format(msg,sizeof(msg),"%s\n%d\t{FFFF00}%d\t%d\n",msg,i+1,BoardsInfo[i][UsersID],BoardsInfo[i][ObjID]);
            else format(msg,sizeof(msg),"%s\n%d\t%d\t%d\n",msg,i+1,BoardsInfo[i][UsersID],BoardsInfo[i][ObjID]);
        }
	}
	Dialog_Show(playerid,BoardsListDialog,DIALOG_STYLE_TABLIST_HEADERS,title,msg,"传送","关闭");
    return 1;
}
stock Board_howMany(const playerid) { //2020.3.2新增 返回玩家拥有了多少个公告牌
    new uid = PlayerInfo[playerid][ID];
    new cut = 0;
    for (new i = 0; i < BoardsCount; i++) 
    {
        if(BoardsInfo[i][UsersID] == uid) cut++;
    }
    return cut;
}
Dialog:BoardsListDialog(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        new msg[128], idx;
        msg = strtok(inputtext, idx);
        if(strcmp(msg, "上一页") == 0) {
            SelectBoardsPage[playerid]--;
            Board_ShowListDialog(playerid, SelectBoardsPage[playerid]);
            return 1;
        }
        if(strcmp(msg, "下一页") == 0) {
            SelectBoardsPage[playerid]++;
            Board_ShowListDialog(playerid, SelectBoardsPage[playerid]);
            return 1;
        }
        format(msg, sizeof msg, "/board goto %s", inputtext);
        AntiCommand[playerid] = 0;
        OnPlayerCommandText(playerid, msg);
        return 1;
    }
    return 1;
}
Dialog:BOARDS(playerid, response, listitem, inputtext[]) {
    if(!response) {
        pInEditTagBoards[playerid] = -1;
        return 1;
    }
    if(listitem == 0) return Dialog_Show(playerid, BOARDSTEXT, DIALOG_STYLE_INPUT, "广告牌", "{FF0000}请输入你要在广告牌写的字", "确定", "返回");
    if(listitem == 1) return Dialog_Show(playerid, BOARDSMINDEX, DIALOG_STYLE_INPUT, "广告牌", "{FF0000}请输入你要设置的材质指标(0-15)", "确定", "返回");
    if(listitem == 2) return Dialog_Show(playerid, BOARDSMSIZE, DIALOG_STYLE_LIST, "广告牌(选择你要的材质大小)", "32x32\n64x32\n64x64\n128x32\n128x64\n128x128\n256x32\n256x64\n256x128\n256x256\n512x64\n512x128\n512x256\n512x512", "确定", "返回");
    if(listitem == 3) return Dialog_Show(playerid, BOARDSFONTFACE, DIALOG_STYLE_INPUT, "广告牌", "{FF0000}请输入你要设置的字体\n请输入电脑中确实存在的字体,否则无法显示", "确定", "返回");
    if(listitem == 4) return Dialog_Show(playerid, BOARDSFONTFACESIZE, DIALOG_STYLE_INPUT, "广告牌", "{FF0000}请输入你要设置的字体大小(1-256)", "确定", "返回");
    if(listitem == 5) return Dialog_Show(playerid, BOARDSBOLD, DIALOG_STYLE_LIST, "广告牌(字体是否加粗)", "是\n否", "确定", "返回");
    if(listitem == 6) return Dialog_Show(playerid, BOARDSFONTCOLOR, DIALOG_STYLE_INPUT, "广告牌", "{FF0000}请输入你要设置的颜色的值", "确定", "返回");
    if(listitem == 7) return Dialog_Show(playerid, BOARDSBACKCOLOR, DIALOG_STYLE_INPUT, "广告牌", "{FF0000}请输入你要设置的颜色的值", "确定", "返回");
    if(listitem == 8) return Dialog_Show(playerid, BOARDSTextA, DIALOG_STYLE_LIST, "广告牌(字体对齐方式)", "向左对齐\n居中对齐\n向右对齐", "确定", "返回");
    if(listitem == 9) return EditBoards(playerid, pInEditTagBoards[playerid]);
    if(listitem == 10) {
        if(DeleteBoards(playerid, pInEditTagBoards[playerid]) != -1) {
            SendClientMessage(playerid, Color_White, "[广告牌]删除成功并退还给你了金币..");
            pInEditTagBoards[playerid] = -1;
        } else {
            SendClientMessage(playerid, Color_White, "[广告牌]删除失败了..");
            MyBoard(playerid, pInEditTagBoards[playerid]);
        }
        return 1;
    }
    return 1;
}
Dialog:BOARDSTEXT(playerid, response, listitem, inputtext[]) {
    if(!response) {
        MyBoard(playerid, pInEditTagBoards[playerid]);
        return 1;
    }
    if(strcmp(inputtext, " ") == 0 || strlen(inputtext) < 1 || strlen(inputtext) > 1024) {
        Dialog_Show(playerid, BOARDSTEXT, DIALOG_STYLE_INPUT, "广告牌", "{FF0000}请输入你要在广告牌写的字\n不能为空并且输入字符在(1-1024)之内", "确定", "返回");
        return 1;
    }
    format(BoardsInfo[pInEditTagBoards[playerid]][BText], 1024, "%s\n", inputtext);
    new placeholder, msg[1024];
    for (new i = 0; i < sizeof InvalidWords; i++) //屏蔽词自动变*
    {
        placeholder = strfind(BoardsInfo[pInEditTagBoards[playerid]][BText], InvalidWords[i], true);
        if(placeholder != -1) {
            for (new x = placeholder; x < placeholder + strlen(InvalidWords[i]); x++) {
                BoardsInfo[pInEditTagBoards[playerid]][BText][x] = '*';
            }
        }
    }
    format(msg, 1024, "你目前输入的内容:%s", BoardsInfo[pInEditTagBoards[playerid]][BText]);
    Dialog_Show(playerid, BOARDSTEXT2, DIALOG_STYLE_INPUT, "广告牌", msg, "下一行", "确定");
    return 1;
}
Dialog:BOARDSTEXT2(playerid, response, listitem, inputtext[]) {
    if(!response) {
        UpdateBoards(pInEditTagBoards[playerid]);
        pInEditTagBoards[playerid] = -1;
        SCM(playerid, -1, "[广告牌]:修改成功");
        return 1;
    }
    if(strcmp(inputtext, " ") == 0 || strlen(inputtext) < 1 || strlen(inputtext) > 1024 || strlen(BoardsInfo[pInEditTagBoards[playerid]][BText]) >= 1024) {
        Dialog_Show(playerid, BOARDSTEXT2, DIALOG_STYLE_INPUT, "广告牌", "{FF0000}请输入你要在广告牌写的字\n不能为空并且输入字符在(1-1024)之内", "下一行", "确定");
        return 1;
    }
    format(BoardsInfo[pInEditTagBoards[playerid]][BText], 1024, "%s%s\n", BoardsInfo[pInEditTagBoards[playerid]][BText], inputtext);
    new placeholder, msg[1024];
    for (new i = 0; i < sizeof InvalidWords; i++) //屏蔽词自动变*
    {
        placeholder = strfind(BoardsInfo[pInEditTagBoards[playerid]][BText], InvalidWords[i], true);
        if(placeholder != -1) {
            for (new x = placeholder; x < placeholder + strlen(InvalidWords[i]); x++) {
                BoardsInfo[pInEditTagBoards[playerid]][BText][x] = '*';
            }
        }
    }
    format(msg, 1024, "你目前输入的内容:%s", BoardsInfo[pInEditTagBoards[playerid]][BText]);
    Dialog_Show(playerid, BOARDSTEXT2, DIALOG_STYLE_INPUT, "广告牌", msg, "下一行", "确定");
    return 1;
}
Dialog:BOARDSMINDEX(playerid, response, listitem, inputtext[]) {
    if(!response) {
        MyBoard(playerid, pInEditTagBoards[playerid]);
        return 1;
    }
    if(strval(inputtext) < 0 || strval(inputtext) > 15) {
        Dialog_Show(playerid, BOARDSMINDEX, DIALOG_STYLE_INPUT, "广告牌", "{FF0000}请输入你要设置的材质指标\n请确定在(0-15)之内", "确定", "返回");
        return 1;
    }
    BoardsInfo[pInEditTagBoards[playerid]][MaterialIndex] = strval(inputtext);
    UpdateBoards(pInEditTagBoards[playerid]);
    pInEditTagBoards[playerid] = -1;
    SCM(playerid, -1, "[广告牌]:修改成功");
    return 1;
}
Dialog:BOARDSMSIZE(playerid, response, listitem, inputtext[]) {
    if(!response) {
        MyBoard(playerid, pInEditTagBoards[playerid]);
        return 1;
    }
    if(listitem == 0) {
        BoardsInfo[pInEditTagBoards[playerid]][MaterialSize] = 10;
        UpdateBoards(pInEditTagBoards[playerid]);
        pInEditTagBoards[playerid] = -1;
        SCM(playerid, -1, "[广告牌]:修改成功");
        return 1;
    }
    if(listitem == 1) {
        BoardsInfo[pInEditTagBoards[playerid]][MaterialSize] = 20;
        UpdateBoards(pInEditTagBoards[playerid]);
        pInEditTagBoards[playerid] = -1;
        SCM(playerid, -1, "[广告牌]:修改成功");
        return 1;
    }
    if(listitem == 2) {
        BoardsInfo[pInEditTagBoards[playerid]][MaterialSize] = 30;
        UpdateBoards(pInEditTagBoards[playerid]);
        pInEditTagBoards[playerid] = -1;
        SCM(playerid, -1, "[广告牌]:修改成功");
        return 1;
    }
    if(listitem == 3) {
        BoardsInfo[pInEditTagBoards[playerid]][MaterialSize] = 40;
        UpdateBoards(pInEditTagBoards[playerid]);
        pInEditTagBoards[playerid] = -1;
        SCM(playerid, -1, "[广告牌]:修改成功");
        return 1;
    }
    if(listitem == 4) {
        BoardsInfo[pInEditTagBoards[playerid]][MaterialSize] = 50;
        UpdateBoards(pInEditTagBoards[playerid]);
        pInEditTagBoards[playerid] = -1;
        SCM(playerid, -1, "[广告牌]:修改成功");
        return 1;
    }
    if(listitem == 5) {
        BoardsInfo[pInEditTagBoards[playerid]][MaterialSize] = 60;
        UpdateBoards(pInEditTagBoards[playerid]);
        pInEditTagBoards[playerid] = -1;
        SCM(playerid, -1, "[广告牌]:修改成功");
        return 1;
    }
    if(listitem == 6) {
        BoardsInfo[pInEditTagBoards[playerid]][MaterialSize] = 70;
        UpdateBoards(pInEditTagBoards[playerid]);
        pInEditTagBoards[playerid] = -1;
        SCM(playerid, -1, "[广告牌]:修改成功");
        return 1;
    }
    if(listitem == 7) {
        BoardsInfo[pInEditTagBoards[playerid]][MaterialSize] = 80;
        UpdateBoards(pInEditTagBoards[playerid]);
        pInEditTagBoards[playerid] = -1;
        SCM(playerid, -1, "[广告牌]:修改成功");
        return 1;
    }
    if(listitem == 8) {
        BoardsInfo[pInEditTagBoards[playerid]][MaterialSize] = 90;
        UpdateBoards(pInEditTagBoards[playerid]);
        pInEditTagBoards[playerid] = -1;
        SCM(playerid, -1, "[广告牌]:修改成功");
        return 1;
    }
    if(listitem == 9) {
        BoardsInfo[pInEditTagBoards[playerid]][MaterialSize] = 100;
        UpdateBoards(pInEditTagBoards[playerid]);
        pInEditTagBoards[playerid] = -1;
        SCM(playerid, -1, "[广告牌]:修改成功");
        return 1;
    }
    if(listitem == 10) {
        BoardsInfo[pInEditTagBoards[playerid]][MaterialSize] = 110;
        UpdateBoards(pInEditTagBoards[playerid]);
        pInEditTagBoards[playerid] = -1;
        SCM(playerid, -1, "[广告牌]:修改成功");
        return 1;
    }
    if(listitem == 11) {
        BoardsInfo[pInEditTagBoards[playerid]][MaterialSize] = 120;
        UpdateBoards(pInEditTagBoards[playerid]);
        pInEditTagBoards[playerid] = -1;
        SCM(playerid, -1, "[广告牌]:修改成功");
        return 1;
    }
    if(listitem == 12) {
        BoardsInfo[pInEditTagBoards[playerid]][MaterialSize] = 130;
        UpdateBoards(pInEditTagBoards[playerid]);
        pInEditTagBoards[playerid] = -1;
        SCM(playerid, -1, "[广告牌]:修改成功");
        return 1;
    }
    if(listitem == 13) {
        BoardsInfo[pInEditTagBoards[playerid]][MaterialSize] = 140;
        UpdateBoards(pInEditTagBoards[playerid]);
        pInEditTagBoards[playerid] = -1;
        SCM(playerid, -1, "[广告牌]:修改成功");
        return 1;
    }
    return 1;
}
Dialog:BOARDSFONTFACE(playerid, response, listitem, inputtext[]) {
    if(!response) {
        MyBoard(playerid, pInEditTagBoards[playerid]);
        return 1;
    }
    if(strcmp(inputtext, " ") == 0 || strlen(inputtext) < 1 || strlen(inputtext) > 16) {
        Dialog_Show(playerid, BOARDSFONTFACE, DIALOG_STYLE_INPUT, "广告牌", "{FF0000}请输入你要设置的字体\n不能为空并且输入字符在(1-16)之内", "确定", "返回");
        return 1;
    }
    format(BoardsInfo[pInEditTagBoards[playerid]][FontFace], 64, inputtext);
    UpdateBoards(pInEditTagBoards[playerid]);
    pInEditTagBoards[playerid] = -1;
    SCM(playerid, -1, "[广告牌]:修改成功");
    return 1;
}
Dialog:BOARDSFONTFACESIZE(playerid, response, listitem, inputtext[]) {
    if(!response) {
        MyBoard(playerid, pInEditTagBoards[playerid]);
        return 1;
    }
    if(strval(inputtext) < 1 || strval(inputtext) > 256) {
        Dialog_Show(playerid, BOARDSFONTFACESIZE, DIALOG_STYLE_INPUT, "广告牌", "{FF0000}请输入你要设置的字体大小(1-256)\n不能超出范围", "确定", "返回");
        return 1;
    }
    BoardsInfo[pInEditTagBoards[playerid]][FontSize] = strval(inputtext);
    UpdateBoards(pInEditTagBoards[playerid]);
    pInEditTagBoards[playerid] = -1;
    SCM(playerid, -1, "[广告牌]:修改成功");
    return 1;
}
Dialog:BOARDSBOLD(playerid, response, listitem, inputtext[]) {
    if(!response) {
        MyBoard(playerid, pInEditTagBoards[playerid]);
        return 1;
    }
    if(listitem == 0) {
        BoardsInfo[pInEditTagBoards[playerid]][Bold] = 1;
        UpdateBoards(pInEditTagBoards[playerid]);
        pInEditTagBoards[playerid] = -1;
        SCM(playerid, -1, "[广告牌]:修改成功");
        return 1;
    }
    if(listitem == 1) {
        BoardsInfo[pInEditTagBoards[playerid]][Bold] = 0;
        UpdateBoards(pInEditTagBoards[playerid]);
        pInEditTagBoards[playerid] = -1;
        SCM(playerid, -1, "[广告牌]:修改成功");
        return 1;
    }
    return 1;
}
//这个会蹦
Dialog:BOARDSFONTCOLOR(playerid, response, listitem, inputtext[]) {
    if(!response) {
        MyBoard(playerid, pInEditTagBoards[playerid]);
        return 1;
    }
    new color = strval(inputtext);
    if(!strlen(inputtext) || color < -9999999999 || color > 9999999999) return Dialog_Show(playerid, BOARDSFONTCOLOR, DIALOG_STYLE_INPUT, "广告牌", "{FF0000}请输入你要设置ARGB格式的颜色值\n不能为空", "确定", "返回");
    BoardsInfo[pInEditTagBoards[playerid]][FontColor] = color;
    UpdateBoards(pInEditTagBoards[playerid]);
    pInEditTagBoards[playerid] = -1;
    SCM(playerid, -1, "[广告牌]:修改成功");
    return 1;
}
//这个也会蹦
Dialog:BOARDSBACKCOLOR(playerid, response, listitem, inputtext[]) {
    if(!response) {
        MyBoard(playerid, pInEditTagBoards[playerid]);
        return 1;
    }
    new color = strval(inputtext);
    if(!strlen(inputtext) || color < -9999999999 || color > 9999999999) return Dialog_Show(playerid, BOARDSFONTCOLOR, DIALOG_STYLE_INPUT, "广告牌", "{FF0000}请输入你要设置ARGB格式的颜色值\n不能为空", "确定", "返回");
    BoardsInfo[pInEditTagBoards[playerid]][BackColor] = color;
    UpdateBoards(pInEditTagBoards[playerid]);
    pInEditTagBoards[playerid] = -1;
    SCM(playerid, -1, "[广告牌]:修改成功");
    return 1;
}
Dialog:BOARDSTextA(playerid, response, listitem, inputtext[]) {
    if(!response) {
        MyBoard(playerid, pInEditTagBoards[playerid]);
        return 1;
    }
    if(listitem == 0) {
        BoardsInfo[pInEditTagBoards[playerid]][TextAlignment] = 0;
        UpdateBoards(pInEditTagBoards[playerid]);
        pInEditTagBoards[playerid] = -1;
        SCM(playerid, -1, "[广告牌]:修改成功");
        return 1;
    }
    if(listitem == 1) {
        BoardsInfo[pInEditTagBoards[playerid]][TextAlignment] = 1;
        UpdateBoards(pInEditTagBoards[playerid]);
        pInEditTagBoards[playerid] = -1;
        SCM(playerid, -1, "[广告牌]:修改成功");
        return 1;
    }
    if(listitem == 2) {
        BoardsInfo[pInEditTagBoards[playerid]][TextAlignment] = 2;
        UpdateBoards(pInEditTagBoards[playerid]);
        pInEditTagBoards[playerid] = -1;
        SCM(playerid, -1, "[广告牌]:修改成功");
        return 1;
    }
    return 1;
}
Dialog:Dialog_BUYBOARDS(playerid, response, listitem, inputtext[]) {
    if(!response) {
        return 1;
    }
    if(listitem == 0) {
        if(PlayerInfo[playerid][Cash] < BOARDOBJPRICE) {
            SCM(playerid, -1, "[广告牌]你的金币不足");
            return 1;
        }
        SCM(playerid, -1, "[广告牌]你购买了一个广告牌!");
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        CreateBoard(playerid, BOARDOBJ, x, y, z + 2, PlayerInfo[playerid][ID]);
        GivePlayerCash(playerid, -BOARDOBJPRICE);
        return 1;
    }
    if(listitem == 1) {
        if(PlayerInfo[playerid][Cash] < BOARDOBJPRICE) {
            SCM(playerid, -1, "[广告牌]你的金币不足");
            return 1;
        }
        SCM(playerid, -1, "[广告牌]你购买了一个广告牌!");
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        CreateBoard(playerid, BOARDOBJ2, x, y, z + 2, PlayerInfo[playerid][ID]);
        GivePlayerCash(playerid, -BOARDOBJ2PRICE);
        return 1;
    }
    if(listitem == 2) {
        if(PlayerInfo[playerid][Cash] < BOARDOBJ3PRICE) {
            SCM(playerid, -1, "[广告牌]你的金币不足");
            return 1;
        }
        SCM(playerid, -1, "[广告牌]你购买了一个广告牌!");
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        CreateBoard(playerid, BOARDOBJ3, x, y, z + 2, PlayerInfo[playerid][ID]);
        GivePlayerCash(playerid, -BOARDOBJ3PRICE);
        return 1;
    }
    if(listitem == 3) {
        if(PlayerInfo[playerid][Cash] < BOARDOBJ4PRICE) {
            SCM(playerid, -1, "[广告牌]你的金币不足");
            return 1;
        }
        SCM(playerid, -1, "[广告牌]你购买了一个广告牌!");
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        CreateBoard(playerid, BOARDOBJ4, x, y, z + 2, PlayerInfo[playerid][ID]);
        GivePlayerCash(playerid, -BOARDOBJ4PRICE);
        return 1;
    }
    if(listitem == 4) {
        if(PlayerInfo[playerid][Cash] < BOARDOBJ5PRICE) {
            SCM(playerid, -1, "[广告牌]你的金币不足");
            return 1;
        }
        SCM(playerid, -1, "[广告牌]你购买了一个广告牌!");
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        CreateBoard(playerid, BOARDOBJ5, x, y, z + 2, PlayerInfo[playerid][ID]);
        GivePlayerCash(playerid, -BOARDOBJ5PRICE);
        return 1;
    }
    if(listitem == 5) {
        if(PlayerInfo[playerid][Cash] < BOARDOBJ6PRICE) {
            SCM(playerid, -1, "[广告牌]你的金币不足");
            return 1;
        }
        SCM(playerid, -1, "[广告牌]你购买了一个广告牌!");
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        CreateBoard(playerid, BOARDOBJ6, x, y, z + 2, PlayerInfo[playerid][ID]);
        GivePlayerCash(playerid, -BOARDOBJ6PRICE);
        return 1;
    }
    if(listitem == 6) {
        if(PlayerInfo[playerid][Cash] < BOARDOBJ7PRICE) {
            SCM(playerid, -1, "[广告牌]你的金币不足");
            return 1;
        }
        SCM(playerid, -1, "[广告牌]你购买了一个广告牌!");
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        CreateBoard(playerid, BOARDOBJ7, x, y, z + 2, PlayerInfo[playerid][ID]);
        GivePlayerCash(playerid, -BOARDOBJ7PRICE);
        return 1;
    }
    if(listitem == 7) {
        if(PlayerInfo[playerid][Cash] < BOARDOBJ8PRICE) {
            SCM(playerid, -1, "[广告牌]你的金币不足");
            return 1;
        }
        SCM(playerid, -1, "[广告牌]你购买了一个广告牌!");
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        CreateBoard(playerid, BOARDOBJ8, x, y, z + 2, PlayerInfo[playerid][ID]);
        GivePlayerCash(playerid, -BOARDOBJ8PRICE);
        return 1;
    }
    return 1;
}


stock SetupBillboardTable() {
    mysql_pquery(g_Sql, "CREATE TABLE IF NOT EXISTS `boards`  (\
    `ID` int(10) UNSIGNED NOT NULL AUTO_INCREMENT,\
    `ObjID` int(11) NOT NULL,\
    `UsersID` int(10) UNSIGNED NOT NULL,\
    `Text` varchar(1023) CHARACTER SET gbk COLLATE gbk_chinese_ci NOT NULL,\
    `MaterialIndex` int(11) NOT NULL,\
    `MaterialSize` int(11) NOT NULL,\
    `FontFace` varchar(63) CHARACTER SET gbk COLLATE gbk_chinese_ci NOT NULL,\
    `FontSize` int(11) NOT NULL,\
    `Bold` int(11) NOT NULL,\
    `FontColor` int(11) NOT NULL,\
    `BackColor` int(11) NOT NULL,\
    `TextAlignment` int(11) NOT NULL,\
    `X` varchar(12) CHARACTER SET gbk COLLATE gbk_chinese_ci NOT NULL,\
    `Y` varchar(12) CHARACTER SET gbk COLLATE gbk_chinese_ci NOT NULL,\
    `Z` varchar(12) CHARACTER SET gbk COLLATE gbk_chinese_ci NOT NULL,\
    `RX` varchar(12) CHARACTER SET gbk COLLATE gbk_chinese_ci NOT NULL,\
    `RY` varchar(12) CHARACTER SET gbk COLLATE gbk_chinese_ci NOT NULL,\
    `RZ` varchar(12) CHARACTER SET gbk COLLATE gbk_chinese_ci NOT NULL,\
    `World` int(11) NOT NULL,\
    `Int` int(11) NOT NULL,\
    PRIMARY KEY (`ID`) USING BTREE\
    ) ENGINE = InnoDB CHARACTER SET = gbk COLLATE = gbk_chinese_ci ROW_FORMAT = Compact;");
}
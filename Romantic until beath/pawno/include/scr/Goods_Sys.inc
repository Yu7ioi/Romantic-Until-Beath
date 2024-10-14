/*
  -----------------使用说明------------------------
	You should create a mysql table in order to save
	datas.
  -------------------------------------------------
  ---------------版權聲明--------------------------
  重寫：Episodes KiVen juse 
  --------------------------------------------------


*/
// #include <a_samp>
// // #include <zcmd>
// // #include <sscanf2>
// #include <streamer>
// #include <a_mysql>
//#include <antscorehack>
//for verson
#define VER_INFO "v0.2"
//for setting
#define MAX_GOODS 1000 //the maxium goods u can carry in ur server.
#define GOODS_RANGE (2.0)//for range of goods can be operated.

// //----------MYSQL STUFFS----------------------
// #define MYSQL_HOST	"127.0.0.1" //改成服务器IP地址
// #define MYSQL_USER	"samp" // MYSQL用户名
// #define MYSQL_PASS 	"rst2020!!!"// 改成你自己的密码
// #define MYSQL_DB	"rstGoods"// 改成你自己的数据表
// //For unicode setting
// #define charset "gbk"//"gbk" is for Chinese(simplify)if u want use other language pls change this!


//Below is for dialog defines,if u get confussed with dialogs id,just edit the first one.
// #define GODIOG_BUY 3224
// #define GODIOG_PL GODIOG_BUY+1
// #define GODIOG_CGNAME GODIOG_BUY+2
// #define GODIOG_TAKEDOWN GODIOG_BUY+3
// #define GODIOG_KEY GODIOG_BUY+4
// #define GODIOG_PASS GODIOG_BUY+5
// #define GODIOG_SELL GODIOG_BUY+6
// #define GODIOG_LIST GODIOG_BUY+7 //2020.3.2新增 操控我的家具列表用
//color
// #define {80FF80}     {80FF80}
/*
ＳＡＭＰＬＥ　ＯＦ　ＭＹＳＱＬ　ＴＡＢＬＥ
GID TAKEN


*/

#define PL_CONCENTS_YES "{FF80C0}举起物品\n{FF80FF}修改名字\n{8080FF}微调坐标\n{FF0080}添加密码\n{C0C0C0}公开物品 = {00FF40}是\n卖掉物品"
#define PL_CONCENTS_NO "{FF80C0}举起物品\n{FF80FF}修改名字\n{8080FF}微调坐标\n{FF0080}添加密码\n{C0C0C0}公开物品 = {FF0000}否\n卖掉物品"

enum GoodsInfo {
    Goodid,
    GoodTaken, //1 for yes,0 for no
    OrderId,
    GoodOwner,
    GoodOwnerName[MAX_PLAYER_NAME],
    GoodName[125],
    GoodObjid,
    GoodPrize,
    Float:GoodX,
    Float:GoodY,
    Float:GoodZ,
    Float:GoodRX,
    Float:GoodRY,
    Float:GoodRZ,
    Key,
    WID,
    GoodState,
    Text3D:Good3DTextLabel,
    bool:issale,
    bool:topublic,
};

new loadcount = 0;
new GOODS[MAX_GOODS][GoodsInfo];

new GOODS_OPRATEID[MAX_PLAYERS];
new bool:GOODS_STATUS[MAX_PLAYERS];
new bool:TAKEDOWN_STATUS[MAX_PLAYERS];
stock InitGoods() {
    // print("\n_______________________________________");
    // print("________ＧＯＯＤＳ　ＳＹＳＴＥＭ________");
    print("[家具系统加载中]由Episodes制作 [Creadits By]:Jinzhi");
    print("[当前版本]:"VER_INFO"");

    //OK First we gonna check mysql right now
    // ConnectMySQL();
    // printf("%d",mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_DB, MYSQL_PASS));
    // select TABLE_NAME from INFORMATION_SCHEMA.TABLES where  TABLE_NAME='user_goods'。
    // mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_DB, MYSQL_PASS)
    // if(mysql_ping!=-1) //连接MYSQL服务器
    
    // if(mysql_ping() != -1)
    // {
    //     if(mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_DB, MYSQL_PASS)){
    //         print("[MySQL] 家具系统连接成功!");
    //         mysql_set_charset(charset);
    //         // printf("%d", mysql_query("select TABLE_NAME from INFORMATION_SCHEMA.TABLES where  TABLE_NAME='user_goods'"));
    //         for (new i = 0; i < MAX_GOODS; i++)
    //         {
    //             LoadGoods(i); //加载家具OBJ
    //         }
    //         printf("[家具]共加载了%d个家具", loadcount);
    //     }
    // } 
    // else
    // {
    //     print("[MySQL]家具系统连接到MYSQL时出错，请检查设置!");
    // }
    for (new i = 0; i < MAX_GOODS; i++)
    {
        LoadGoods(i); //加载家具OBJ
    }
    printf("[家具]共加载了%d个家具", loadcount);
}
stock LoadGoods(id) {
    // CheckMySQL();
    //Create String to store values
    // new field[16][125], GID;
    // format(string, sizeof(string), "SELECT * FROM `user_goods` WHERE GID = '%d'", id);
    // mysql_query(string);
    // mysql_store_result();
    new string[128];
    mysql_format(g_Sql, string, sizeof(string), "SELECT\
	`user_goods`.*,\
	`users`.`Name` \
    FROM `user_goods`\
	LEFT JOIN `users` ON `user_goods`.`UID` = `users`.`ID` \
    WHERE GID = %d;", id);
    mysql_pquery(g_Sql, string, "OnGoodsLoaded");
    //show tables like 'table_name'; 

    //make loop to load multi data
    // printf("%d",mysql_retrieve_row());
    // while (mysql_retrieve_row()) //this will be running until all rows are processed
    // {
    //     mysql_fetch_field_row(field[1], "GID");
    //     mysql_fetch_field_row(field[2], "TAKEN");
    //     mysql_fetch_field_row(field[3], "OWNER");
    //     mysql_fetch_field_row(field[4], "GNAME");
    //     mysql_fetch_field_row(field[5], "GOBJID");
    //     mysql_fetch_field_row(field[6], "SALE");
    //     mysql_fetch_field_row(field[7], "PRICE");
    //     mysql_fetch_field_row(field[8], "X");
    //     mysql_fetch_field_row(field[9], "Y");
    //     mysql_fetch_field_row(field[10], "Z");
    //     mysql_fetch_field_row(field[11], "RX");
    //     mysql_fetch_field_row(field[12], "RY");
    //     mysql_fetch_field_row(field[13], "RZ");
    //     mysql_fetch_field_row(field[14], "KEY");
    //     mysql_fetch_field_row(field[15], "WID");
    //     //Given values

    //     GID = strval(field[1]);
    //     GOODS[GID][GoodTaken] = strval(field[2]);
    //     format(GOODS[GID][GoodOwner], 65, field[3]);
    //     format(GOODS[GID][GoodName], 125, field[4]);
    //     GOODS[GID][GoodObjid] = strval(field[5]);
    //     if(strcmp(field[6], "YES", false) == 0) {
    //         GOODS[GID][issale] = true;
    //     } else {
    //         GOODS[GID][issale] = false;
    //     }
    //     GOODS[GID][GoodPrize] = strval(field[7]);

    //     GOODS[GID][GoodX] = floatstr(field[8]);
    //     GOODS[GID][GoodY] = floatstr(field[9]);
    //     GOODS[GID][GoodZ] = floatstr(field[10]);
    //     GOODS[GID][GoodRX] = floatstr(field[11]);
    //     GOODS[GID][GoodRY] = floatstr(field[12]);
    //     GOODS[GID][GoodRZ] = floatstr(field[13]);
    //     GOODS[GID][Key] = strval(field[14]);
    //     GOODS[GID][WID] = strval(field[15]);

    //     CreateGoods(GID);
    //     loadcount++;
    // }
    // mysql_free_result();
}
function OnGoodsLoaded(){
    //show tables like 'table_name'; 
    //make loop to load multi data
    // printf("%d",mysql_retrieve_row());
    // while (mysql_retrieve_row()) //this will be running until all rows are processed
    // {
    //     mysql_fetch_field_row(field[1], "GID");
    //     mysql_fetch_field_row(field[2], "TAKEN");
    //     mysql_fetch_field_row(field[3], "OWNER");
    //     mysql_fetch_field_row(field[4], "GNAME");
    //     mysql_fetch_field_row(field[5], "GOBJID");
    //     mysql_fetch_field_row(field[6], "SALE");
    //     mysql_fetch_field_row(field[7], "PRICE");
    //     mysql_fetch_field_row(field[8], "X");
    //     mysql_fetch_field_row(field[9], "Y");
    //     mysql_fetch_field_row(field[10], "Z");
    //     mysql_fetch_field_row(field[11], "RX");
    //     mysql_fetch_field_row(field[12], "RY");
    //     mysql_fetch_field_row(field[13], "RZ");
    //     mysql_fetch_field_row(field[14], "KEY");
    //     mysql_fetch_field_row(field[15], "WID");
    //     //Given values

    //     GID = strval(field[1]);
    //     GOODS[GID][GoodTaken] = strval(field[2]);
    //     format(GOODS[GID][GoodOwner], 65, field[3]);
    //     format(GOODS[GID][GoodName], 125, field[4]);
    //     GOODS[GID][GoodObjid] = strval(field[5]);
    //     if(strcmp(field[6], "YES", false) == 0) {
    //         GOODS[GID][issale] = true;
    //     } else {
    //         GOODS[GID][issale] = false;
    //     }
    //     GOODS[GID][GoodPrize] = strval(field[7]);

    //     GOODS[GID][GoodX] = floatstr(field[8]);
    //     GOODS[GID][GoodY] = floatstr(field[9]);
    //     GOODS[GID][GoodZ] = floatstr(field[10]);
    //     GOODS[GID][GoodRX] = floatstr(field[11]);
    //     GOODS[GID][GoodRY] = floatstr(field[12]);
    //     GOODS[GID][GoodRZ] = floatstr(field[13]);
    //     GOODS[GID][Key] = strval(field[14]);
    //     GOODS[GID][WID] = strval(field[15]);

    //     CreateGoods(GID);
    //     loadcount++;
    // }   
    if(cache_num_rows()) //this will be running until all rows are processed
    {
        new GID;
        cache_get_value_name_int(0,"GID",GID);
        cache_get_value_name_int(0, "TAKEN", GOODS[GID][GoodTaken]);
        cache_get_value_name_int(0, "UID", GOODS[GID][GoodOwner]);
        cache_get_value_name(0, "Name", GOODS[GID][GoodOwnerName]);
        cache_get_value_name(0, "GNAME", GOODS[GID][GoodName], 125);
        cache_get_value_name_int(0, "GOBJID", GOODS[GID][GoodObjid]);
        // new sale_[16];
        cache_get_value_name_bool(0, "SALE", GOODS[GID][issale]);
        // 这里改了 本来是yes no的
        cache_get_value_name_int(0, "PRICE", GOODS[GID][GoodPrize]);
        cache_get_value_name_float(0, "X", GOODS[GID][GoodX]);
        cache_get_value_name_float(0, "Y", GOODS[GID][GoodY]);
        cache_get_value_name_float(0, "Z", GOODS[GID][GoodZ]);
        cache_get_value_name_float(0, "RX", GOODS[GID][GoodRX]);
        cache_get_value_name_float(0, "RY", GOODS[GID][GoodRY]);
        cache_get_value_name_float(0, "RZ", GOODS[GID][GoodRZ]);
        cache_get_value_name_int(0, "KEY", GOODS[GID][Key]);
        cache_get_value_name_int(0, "WID", GOODS[GID][WID]);
        // if(strcmp(sale_, "YES", false) == 0) {
        //     GOODS[GID][issale] = true;
        // } else {
        //     GOODS[GID][issale] = false;
        // }

        CreateGoods(GID);
        loadcount++;
    }    
}
stock SaveGoods(id) {
    // CheckMySQL();
    // if(mysql_ping() == -1) mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_DB, MYSQL_PASS);
    new string[128];
    mysql_format(g_Sql, string, sizeof(string), "SELECT * FROM `user_goods` WHERE GID = '%d'", id);
    mysql_pquery(g_Sql, string, "Save_PlayerGoods", "d", id);
    return 1;
}

// CheckMySQL() {
//     if(mysql_ping() == -1) mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_DB, MYSQL_PASS);
// }


// 2021.10.7 修复
//  error #1054 while executing query "UPDATE `user_goods`...... Unknown column 'OWNER' in 'field list'

function Save_PlayerGoods(id)
{
    // new value;
    // cache_get_row_count(value);
    // return value;
    new string[512], sale[20];
    if(GOODS[id][issale] == true) format(sale, sizeof(sale), "YES");
    else format(sale, sizeof(sale), "NO");
    if(cache_num_rows()) {
        mysql_format(g_Sql, string, sizeof(string), "UPDATE `user_goods` SET `TAKEN`='%d',`UID`='%d',`GNAME`='%e',`GOBJID`='%d',`SALE`='%e',`PRICE`='%d',`X`='%f',`Y`='%f',`Z`='%f',`RX`='%f',`RY`='%f',`RZ`='%f',`KEY`='%d',`WID`='%d' WHERE `GID`='%d'",
            GOODS[id][GoodTaken],
            GOODS[id][GoodOwner],
            GOODS[id][GoodName],
            GOODS[id][GoodObjid],
            sale,
            GOODS[id][GoodPrize],
            GOODS[id][GoodX],
            GOODS[id][GoodY],
            GOODS[id][GoodZ],
            GOODS[id][GoodRX],
            GOODS[id][GoodRY],
            GOODS[id][GoodRZ],
            GOODS[id][Key],
            GOODS[id][WID],
            id);
        mysql_pquery(g_Sql, string);
        return true;
    } 
    mysql_format(g_Sql, string, sizeof(string), "INSERT INTO `user_goods` (`GID`,`TAKEN`,`UID`,`GNAME`,`GOBJID`,`SALE`,`PRICE`,`X`,`Y`,`Z`,`RX`,`RY`,`RZ`,`KEY`,`WID`)  VALUES ('%d','%d','%d','%e','%d','%e','%d','%f','%f','%f','%f','%f','%f','%d','%d')",
            id,
            GOODS[id][GoodTaken],
            GOODS[id][GoodOwner],
            GOODS[id][GoodName],
            GOODS[id][GoodObjid],
            sale,
            GOODS[id][GoodPrize],
            GOODS[id][GoodX],
            GOODS[id][GoodY],
            GOODS[id][GoodZ],
            GOODS[id][GoodRX],
            GOODS[id][GoodRY],
            GOODS[id][GoodRZ],
            GOODS[id][Key],
            GOODS[id][WID]);
    mysql_pquery(g_Sql, string);
    return true;
}

function CreateGoods(GID) {
    GOODS[GID][OrderId] = CreateDynamicObject(GOODS[GID][GoodObjid], GOODS[GID][GoodX], GOODS[GID][GoodY], GOODS[GID][GoodZ], GOODS[GID][GoodRX], GOODS[GID][GoodRY], GOODS[GID][GoodRZ], GOODS[GID][WID]);
    new string[512];
    if(GOODS[GID][issale] == true) {
        format(string, sizeof(string), "{80FF80}物品正在售卖...\n{FFFFFF}[物品ID]:{80FF80} %d\n{FFFFFF}[随机顺序ID]:{80FF80} %d\n{FFFFFF}[模型ID]:{80FF80} %d\n{FFFFFF}[价格]:{80FF80} %d\n{FFFFFF}按{80FF80}Y{FFFFFF}操作", GID, GOODS[GID][OrderId], GOODS[GID][GoodObjid], GOODS[GID][GoodPrize]);
        GOODS[GID][Good3DTextLabel] = CreateDynamic3DTextLabel(string, Color_White, GOODS[GID][GoodX], GOODS[GID][GoodY], GOODS[GID][GoodZ], 4.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, GOODS[GID][WID]);
    } else {
        format(string, sizeof(string), "{80FF80}%s\n{FFFFFF}[主人]:{80FF80} %d %s\n{FFFFFF}[物品ID]: {80FF80}%d\n{FFFFFF}[随机顺序ID]:{80FF80} %d\n{FFFFFF}[模型ID]:{80FF80} %d\n{FFFFFF}按{80FF80}Y{FFFFFF}操作", GOODS[GID][GoodName], GOODS[GID][GoodOwner], GOODS[GID][GoodOwnerName], GID, GOODS[GID][OrderId], GOODS[GID][GoodObjid]);
        GOODS[GID][Good3DTextLabel] = CreateDynamic3DTextLabel(string, Color_White, GOODS[GID][GoodX], GOODS[GID][GoodY], GOODS[GID][GoodZ], 4.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, GOODS[GID][WID]);
    }

    return 1;
}

function DelGoods(GID) {

    //Delete Obj and 3dtext first
    DestroyDynamic3DTextLabel(GOODS[GID][Good3DTextLabel]);
    DestroyDynamicObject(GOODS[GID][OrderId]);

    //then reset values
    GOODS[GID][GoodTaken] = 0;
    // format(GOODS[GID][GoodOwner], 65, "N/A");
    GOODS[GID][GoodOwner] = 0;
    format(GOODS[GID][GoodOwnerName], MAX_PLAYER_NAME, "NULL");
    GOODS[GID][GoodObjid] = 0;
    GOODS[GID][GoodPrize] = 0;
    GOODS[GID][GoodX] = 0;
    GOODS[GID][GoodY] = 0;
    GOODS[GID][GoodZ] = 0;
    GOODS[GID][GoodRX] = 0;
    GOODS[GID][GoodRY] = 0;
    GOODS[GID][GoodRZ] = 0;
    GOODS[GID][Key] = 0;
    GOODS[GID][WID] = 0;
    GOODS[GID][issale] = true;
    GOODS[GID][topublic] = true;

    new string[125];
    //Delete From database
    // format(string, sizeof(string), "DELETE FROM `user_goods` WHERE `GID` = '%d'", GID);
    // mysql_query(string);
    mysql_format(g_Sql, string, sizeof(string), "DELETE FROM `user_goods` WHERE `GID` = '%d'", GID);
    mysql_pquery(g_Sql, string);
    loadcount--; //2020.3.8修
    //SaveGoods();
    return 1;
}











stock getAvailableGoodsID() {
    new i = 0;
    while (i != MAX_GOODS) {
        if(GOODS[i][GoodTaken] == 0) {
            return i;
        }
        i++;
    }
    return -1;
}

function SellGoodsToSYS(playerid, GID) {
    new gotprize;
    //GOODS[GID][GoodTaken]=1;
    // format(GOODS[GID][GoodOwner], 65, "N/A");
    GOODS[GID][GoodOwner] = 0;
    format(GOODS[GID][GoodOwnerName], MAX_PLAYER_NAME, "NULL");
    format(GOODS[GID][GoodName], 65, "NULL");
    // format(GOODS[GID][GoodName], 65, "N/A");
    //GOODS[GID][GoodObjid]=modelid;
    //GOODS[GID][GoodPrize]=seprice;
    GOODS[GID][Key] = 0;
    //GOODS[GID][WID]=GetPlayerVirtualWorld(playerid);
    GOODS[GID][issale] = true;
    GOODS[GID][topublic] = true;
    UpdateGoods3dtextlabel(GID);
    gotprize = GOODS[GID][GoodPrize] / 2;
	GivePlayerMoney(playerid, gotprize);
    // SetPlayerScore(playerid, GetPlayerScore(playerid) + gotprize);
    new str[125];
    format(str, sizeof(str), "[家具] 你获得了 %d 卖掉了这个东西", gotprize);
    SendClientMessage(playerid, 0xFFFFFFC8, str);
    SaveGoods(GID);
}

function UpdateGoods3dtextlabel(GID) {
    new label[256];
    //Check if it sale
    if(GOODS[GID][issale] == true) {
        format(label, sizeof(label), "{80FF80}物品正在售卖...\n{FFFFFF}[物品ID]:{80FF80} %d\n{FFFFFF}[随机顺序ID]:{80FF80} %d\n{FFFFFF}[模型ID]:{80FF80} %d\n{FFFFFF}[价格]:{80FF80} %d\n{FFFFFF}按{80FF80}Y{FFFFFF}操作", GID, GOODS[GID][OrderId], GOODS[GID][GoodObjid], GOODS[GID][GoodPrize]);
        UpdateDynamic3DTextLabelText(GOODS[GID][Good3DTextLabel], Color_White, label);

    } else {
        format(label, sizeof(label), "{80FF80}%s\n{FFFFFF}[主人]:{80FF80} %d %s\n{FFFFFF}[物品ID]:{80FF80} %d\n{FFFFFF}[随机顺序ID]:{80FF80} %d\n{FFFFFF}[模型ID]:{80FF80} %d\n{FFFFFF}按{80FF80}Y{FFFFFF}操作", GOODS[GID][GoodName], GOODS[GID][GoodOwner], GOODS[GID][GoodOwnerName], GID, GOODS[GID][OrderId], GOODS[GID][GoodObjid]);
        UpdateDynamic3DTextLabelText(GOODS[GID][Good3DTextLabel], Color_White, label);
    }
    return 1;
}

function PickUP(playerid) {
    GOODS_STATUS[playerid] = true;
    ClearAnimations(playerid);
    ApplyAnimation(playerid, "CARRY", "crry_prtial", 4.0, 0, 0, 1, 1, 1);
    DestroyDynamic3DTextLabel(GOODS[GOODS_OPRATEID[playerid]][Good3DTextLabel]);
    DestroyDynamicObject(GOODS[GOODS_OPRATEID[playerid]][OrderId]);

    SetDynamicObjectPos(GOODS[GOODS_OPRATEID[playerid]][OrderId], 0, 0, 9999999);
    SetPlayerAttachedObject(playerid, 9, GOODS[GOODS_OPRATEID[playerid]][GoodObjid], 1, 0, 0.6, 0, 0, 90, 0, 1.000000, 1.000000, 1.000000);
    SendClientMessage(playerid, 0xFF8000C8, "[家具]你举起了物品!");
    return 1;
}
function TakeDown(playerid) {
    GOODS_STATUS[playerid] = false;
    TAKEDOWN_STATUS[playerid] = false;
    GetPlayerPos(playerid, GOODS[GOODS_OPRATEID[playerid]][GoodX], GOODS[GOODS_OPRATEID[playerid]][GoodY], GOODS[GOODS_OPRATEID[playerid]][GoodZ]);
    GOODS[GOODS_OPRATEID[playerid]][WID] = GetPlayerVirtualWorld(playerid);
    //	SetDynamicObjectPos(GOODS[GOODS_OPRATEID[playerid]][OrderId],GOODS[GOODS_OPRATEID[playerid]][GoodX]+1,GOODS[GOODS_OPRATEID[playerid]][GoodY]+1,GOODS[GOODS_OPRATEID[playerid]][GoodZ]);
    CreateGoods(GOODS_OPRATEID[playerid]);
    /*
     	new string[256];
    	if(GOODS[GOODS_OPRATEID[playerid]][issale]==true)
    	{
    	format(string,sizeof(string),"{80FF80}物品正在售卖...\n{FFFFFF}[物品ID]:{80FF80} %d\n{FFFFFF}[随机顺序ID]:{80FF80} %d\n{FFFFFF}[模型ID]:{80FF80} %d\n{FFFFFF}[价格]:{80FF80} %d\n{FFFFFF}按{80FF80}Y{FFFFFF}操作",GOODS_OPRATEID[playerid],GOODS[GOODS_OPRATEID[playerid]][OrderId],GOODS[GOODS_OPRATEID[playerid]][GoodObjid],GOODS[GOODS_OPRATEID[playerid]][GoodPrize]);
    	GOODS[GOODS_OPRATEID[playerid]][Good3DTextLabel]=CreateDynamic3DTextLabel(string, Color_White,GOODS[GOODS_OPRATEID[playerid]][GoodX]+1,GOODS[GOODS_OPRATEID[playerid]][GoodY]+1,GOODS[GOODS_OPRATEID[playerid]][GoodZ]-0.3, 4.0,INVALID_PLAYER_ID,INVALID_VEHICLE_ID,-1,GOODS[GOODS_OPRATEID[playerid]][WID]);
    	}else{
    	format(string,sizeof(string),"{80FF80}%s\n{FFFFFF}[主人]:{80FF80} %s\n{FFFFFF}[物品ID]: {80FF80}%d\n{FFFFFF}[模型ID]:{80FF80} %d\n{FFFFFF}按{80FF80}Y{FFFFFF}操作",GOODS[GOODS_OPRATEID[playerid]][GoodName],GOODS[GOODS_OPRATEID[playerid]][GoodOwner],GOODS_OPRATEID[playerid],GOODS[GOODS_OPRATEID[playerid]][GoodObjid]);
    	GOODS[GOODS_OPRATEID[playerid]][Good3DTextLabel]=CreateDynamic3DTextLabel(string, Color_White,GOODS[GOODS_OPRATEID[playerid]][GoodX]+1,GOODS[GOODS_OPRATEID[playerid]][GoodY]+1,GOODS[GOODS_OPRATEID[playerid]][GoodZ]-0.3, 4.0,INVALID_PLAYER_ID,INVALID_VEHICLE_ID,-1,GOODS[GOODS_OPRATEID[playerid]][WID]);
    	}
    	printf("TAKE DOWN WORLD = %d",GOODS[GOODS_OPRATEID[playerid]][WID]);
    	*/
    ClearAnimations(playerid);
    SendClientMessage(playerid, 0x00FFFFC8, "[家具]你放下了物品!");
    SaveGoods(GOODS_OPRATEID[playerid]);
    return 1;
}

function ResetGoods(playerid, pid) {
    GOODS_STATUS[playerid] = false;
    TAKEDOWN_STATUS[playerid] = false;
    RemovePlayerAttachedObject(pid, 9);
    GetPlayerPos(playerid, GOODS[pid][GoodX], GOODS[pid][GoodY], GOODS[pid][GoodZ]);

    //	SetDynamicObjectPos(GOODS[pid][OrderId],GOODS[pid][GoodX]+1,GOODS[pid][GoodY]+1,GOODS[pid][GoodZ]);

    new string[256];
    if(GOODS[pid][issale] == true) {
        format(string, sizeof(string), "{80FF80}{80FF80}物品正在售卖...\n{FFFFFF}[物品ID]:{80FF80} %d\n{FFFFFF}[随机顺序ID]:{80FF80} %d\n{FFFFFF}[模型ID]:{80FF80} %d\n{FFFFFF}[价格]:{80FF80} %d\n{FFFFFF}按{80FF80}Y{FFFFFF}操作", pid, GOODS[pid][OrderId], GOODS[pid][GoodObjid], GOODS[pid][GoodPrize]);
        GOODS[pid][Good3DTextLabel] = CreateDynamic3DTextLabel(string, Color_White, GOODS[pid][GoodX] + 1, GOODS[pid][GoodY] + 1, GOODS[pid][GoodZ] - 0.3, 4.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, GOODS[pid][WID]);
    } else {
        format(string, sizeof(string), "{80FF80}{80FF80}%s\n{FFFFFF}[主人]:{80FF80} %d %s\n{FFFFFF}[物品ID]: {80FF80}%d\n{FFFFFF}[模型ID]: {80FF80}%d\n{FFFFFF}按{80FF80}Y{FFFFFF}操作", GOODS[pid][GoodName], GOODS[pid][GoodOwner], GOODS[pid][GoodOwnerName], pid, GOODS[pid][GoodObjid]);
        GOODS[pid][Good3DTextLabel] = CreateDynamic3DTextLabel(string, Color_White, GOODS[pid][GoodX] + 1, GOODS[pid][GoodY] + 1, GOODS[pid][GoodZ] - 0.3, 4.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, GOODS[pid][WID]);
    }
    SendClientMessage(pid, 0x00FFFFC8, "[家具]由于刷新物品,强制放下!");
}



stock getClosestGOODS(const playerid) {
    new i = 0;
    while (i != MAX_GOODS) {
        if(IsPlayerInRangeOfPoint(playerid, GOODS_RANGE, GOODS[i][GoodX], GOODS[i][GoodY], GOODS[i][GoodZ]) && GOODS[i][GoodTaken] == 1) {
            return i;
        }
        i++;
    }
    return -1;
}


Dialog:GODIOG_BUY(playerid, response, listitem, inputtext[]) {
    if(response == 1) {

        if(GOODS[GOODS_OPRATEID[playerid]][issale] == false) return Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "{FF0000}ERROR>.<[反重复购买BUG]", "{FF0000}此物品已经有主人了", "恩~", "");
        if(GetPlayerMoney(playerid) < GOODS[GOODS_OPRATEID[playerid]][GoodPrize]) return Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "ERROR>.<", "哎~积分不够", "算了", "");

        // if(GetPlayerScore(playerid) < GOODS[GOODS_OPRATEID[playerid]][GoodPrize]) return Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "ERROR>.<", "哎~积分不够", "算了", "");
        // SetPlayerScore(playerid, GetPlayerScore(playerid) - GOODS[GOODS_OPRATEID[playerid]][GoodPrize]);
        GivePlayerMoney(playerid, GOODS[GOODS_OPRATEID[playerid]][GoodPrize] * -1);
        SendClientMessage(playerid, 0xFFFFFFC8, "[家具]购买成功");
        // new owner[65];
        // GetPlayerName(playerid, owner, 65);
        GOODS[GOODS_OPRATEID[playerid]][GoodOwner] = PlayerInfo[playerid][ID];
        // format(GOODS[GOODS_OPRATEID[playerid]][GoodOwner], 65, owner);
        format(GOODS[GOODS_OPRATEID[playerid]][GoodName], 125, "%s 的物品", GetName(playerid));
        GOODS[GOODS_OPRATEID[playerid]][Key] = 0;
        GOODS[GOODS_OPRATEID[playerid]][issale] = false;
        GOODS[GOODS_OPRATEID[playerid]][topublic] = false;
        // 		GOODS[GOODS_OPRATEID[playerid]][GoodPrize]=0; Disabled
        GOODS[GOODS_OPRATEID[playerid]][WID] = GetPlayerVirtualWorld(playerid);
        UpdateGoods3dtextlabel(GOODS_OPRATEID[playerid]);
        SaveGoods(GOODS_OPRATEID[playerid]);
        //SaveGoods();
        return 1;
    }
    return 1;
}
Dialog:GODIOG_LIST(playerid, response, listitem, inputtext[]) {
    if(response) 
    {
        new msg[128], idx;
        msg = strtok(inputtext, idx);
        if(strcmp(msg, "ID:") == 0) {
            msg = strtok(inputtext, idx);
            GOODS_OPRATEID[playerid] = strval(msg);
            SetPlayerPos(playerid, GOODS[GOODS_OPRATEID[playerid]][GoodX], GOODS[GOODS_OPRATEID[playerid]][GoodY], GOODS[GOODS_OPRATEID[playerid]][GoodZ]);
            if(GOODS[GOODS_OPRATEID[playerid]][topublic] == true) {
                new title[285];
                format(title, 285, "{FFFFFF}物品ID:{80FF80} %d {FFFFFF}主人:{80FF80} %d - {80FFFF}操作菜单", GOODS_OPRATEID[playerid], GOODS[GOODS_OPRATEID[playerid]][GoodOwner]);
                Dialog_Show(playerid, GODIOG_PL, DIALOG_STYLE_LIST, title, PL_CONCENTS_YES, "选择", "关闭");
            } else {
                new title[285];
                format(title, 285, "{FFFFFF}物品ID:\
                    {80FF80} %d {FFFFFF}主人:\
                    {80FF80} %d - {80FFFF}操作菜单", GOODS_OPRATEID[playerid], GOODS[GOODS_OPRATEID[playerid]][GoodOwner]);
                Dialog_Show(playerid, GODIOG_PL, DIALOG_STYLE_LIST, title, PL_CONCENTS_NO, "选择", "关闭");
            }
            return 1;
        }
        return SendClientMessage(playerid, Color_White, "[不可选]");
    }
    return 1;
}
Dialog:GODIOG_CGNAME(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        if(!strlen(inputtext)) return Dialog_Show(playerid, GODIOG_CGNAME, DIALOG_STYLE_INPUT, "{FF0000}密碼修改", "{FF0000}名字不能为空!", "修改", "关闭");
        format(GOODS[GOODS_OPRATEID[playerid]][GoodName], 125, inputtext);
        UpdateGoods3dtextlabel(GOODS_OPRATEID[playerid]);
        SendClientMessage(playerid, 0xFFFFFFC8, "[家具]名字修改成功");
        SaveGoods(GOODS_OPRATEID[playerid]);
        return 1;
    }
    return 1;
}
Dialog:GODIOG_PL(playerid, response, listitem, inputtext[]) {
    if(response) {
        if(listitem == 0) {
            //Pick up goods here
            ApplyAnimation(playerid, "CARRY", "liftup", 4, 0, 0, 1, 1, 1);
            SetTimerEx("PickUP", 1500, false, "i", playerid);
            SendClientMessage(playerid, 0x00FFFFC8, "[家具]:按住开火键放下物品");
            return 1;
        }
        if(listitem == 1) {
            //here is for name changing
            Dialog_Show(playerid, GODIOG_CGNAME, DIALOG_STYLE_INPUT, "{FF80C0}名字修改", "{FF80C0}请输入新名称", "修改", "关闭");
            return 1;
        }
        if(listitem == 2) {
            //here is for pos editing
            EditDynamicObject(playerid, GOODS[GOODS_OPRATEID[playerid]][OrderId]);
            return 1;
        }
        if(listitem == 3) {
            //here is for key
            Dialog_Show(playerid, GODIOG_KEY, DIALOG_STYLE_INPUT, "{8000FF}密码", "{8000FF}请输入你的物品密码(限数字)\n{FF0000}注意'0'为没有密码", "添加", "关闭");
            return 1;
        }
        if(listitem == 4) {
            //here is for public setting
            if(GOODS[GOODS_OPRATEID[playerid]][topublic] == true) {
                GOODS[GOODS_OPRATEID[playerid]][topublic] = false;
                //	print("YES");
            } else {
                GOODS[GOODS_OPRATEID[playerid]][topublic] = true;
                //print("NO");
            }
            //save it
            SaveGoods(GOODS_OPRATEID[playerid]);
            //then show player dialog
            //I wonder if i should put a if to see that player is owner?
            if(GOODS[GOODS_OPRATEID[playerid]][topublic] == true) {
                new title[285];
                format(title, 285, "{FFFFFF}物品ID:{80FF80} %d {FFFFFF}主人:{80FF80} %d - {80FFFF}操作菜单", GOODS_OPRATEID[playerid], GOODS[GOODS_OPRATEID[playerid]][GoodOwner]);
                Dialog_Show(playerid, GODIOG_PL, DIALOG_STYLE_LIST, title, PL_CONCENTS_YES, "选择", "关闭");
            } else {
                new title[285];
                format(title, 285, "{FFFFFF}物品ID:\
                    {80FF80} %d {FFFFFF}主人:\
                    {80FF80} %d - {80FFFF}操作菜单", GOODS_OPRATEID[playerid], GOODS[GOODS_OPRATEID[playerid]][GoodOwner]);
                Dialog_Show(playerid, GODIOG_PL, DIALOG_STYLE_LIST, title, PL_CONCENTS_NO, "选择", "关闭");
            }
            return 1;
        }
        if(listitem == 5) {
            new title[285];
            format(title, 285, "卖掉 {FFFFFF}物品ID:\
                {80FF80} %d {FFFFFF}主人:\
                {80FF80} %d - {80FFFF} 確認嗎?", GOODS_OPRATEID[playerid], GOODS[GOODS_OPRATEID[playerid]][GoodOwner]);
            Dialog_Show(playerid, GODIOG_SELL, DIALOG_STYLE_LIST, title, "卖给系统(半价)\n卖给玩家", "选择", "关闭");
            return 1;
        }
    }
    return 1;
}
Dialog:GODIOG_SELL(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        if(listitem == 0) {
            SellGoodsToSYS(playerid, GOODS_OPRATEID[playerid]);
            return 1;
        }
    }
    return 1;
}
Dialog:GODIOG_TAKEDOWN(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        ApplyAnimation(playerid, "Freeweights", "gym_free_putdown", 2, 0, 0, 1, 1, 1);
        RemovePlayerAttachedObject(playerid, 9);
        TAKEDOWN_STATUS[playerid] = true;
        SetTimerEx("TakeDown", 1500, false, "i", playerid);
        return 1;
    }
    return 1;
}
Dialog:GODIOG_KEY(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        if(!strval(inputtext)) {
            GOODS[GOODS_OPRATEID[playerid]][Key] = 0;
            Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "{80FF80}密码修改", "{80FF80}密码被取消！", "OK", "");
            return 1;
        }
        GOODS[GOODS_OPRATEID[playerid]][Key] = strval(inputtext);
        SendClientMessage(playerid, 0xFFFFFFC8, "[家具]密码修改成功");
        SaveGoods(GOODS_OPRATEID[playerid]);
        return 1;
    }
    return 1;
}
Dialog:GODIOG_PASS(playerid, response, listitem, inputtext[]) { //password check
    if(response == 1) {
        if(GOODS[GOODS_OPRATEID[playerid]][Key] == strval(inputtext)) {

            SendClientMessage(playerid, 0xFFFFFFC8, "[家具]:密码正确,你可以操作这个物品了!");
            if(GOODS[GOODS_OPRATEID[playerid]][topublic] == true) {
                new title[285];
                format(title, 285, "{FFFFFF}物品ID:{80FF80}%d {FFFFFF}主人:{80FF80}%d - {80FFFF}操作菜单", GOODS_OPRATEID[playerid], GOODS[GOODS_OPRATEID[playerid]][GoodOwner]);
                Dialog_Show(playerid, GODIOG_PL, DIALOG_STYLE_LIST, title, PL_CONCENTS_YES, "选择", "关闭");
            } else {
                new title[285];
                format(title, 285, "{FFFFFF}物品ID:{80FF80}%d {FFFFFF}主人:{80FF80}%d - {80FFFF}操作菜单", GOODS_OPRATEID[playerid], GOODS[GOODS_OPRATEID[playerid]][GoodOwner]);
                Dialog_Show(playerid, GODIOG_PL, DIALOG_STYLE_LIST, title, PL_CONCENTS_NO, "选择", "关闭");
            }
        } else {
            Dialog_Show(playerid, GODIOG_PASS, DIALOG_STYLE_INPUT, "{80FF80}安全验证程式", "{FF0000}错误的密码!\n要操作请输入密码\n{80FF80}如果你忘记密码请去论坛求助", "OK", "取消");
            return 1;
        }
    }
    return 1;
}


//家具系统
CMD:creategoods(const playerid, const cmdtext[]) {
    if(PlayerInfo[playerid][AdminLevel] < 4) return SendClientMessage(playerid, Color_White, "[家具系统]:你的管理员权限等级不足.");
    //Create String to store in local values
    new modelid, seprice, Float:x, Float:y, Float:z;
    if(sscanf(cmdtext, "ii", modelid, seprice)) return SendClientMessage(playerid, Color_White, "[家具系统]:{FF0000}/creategoods{37FF00} [objid] [价格]");
    GetPlayerPos(playerid, x, y, z);
    new GID;
    GID = getAvailableGoodsID();
    GOODS[GID][GoodTaken] = 1;
    format(GOODS[GID][GoodOwner], 65, "N/A");
    format(GOODS[GID][GoodName], 65, "N/A");
    GOODS[GID][GoodObjid] = modelid;
    GOODS[GID][GoodPrize] = seprice;
    GOODS[GID][GoodX] = x + 1;
    GOODS[GID][GoodY] = y + 1;
    GOODS[GID][GoodZ] = z;
    GOODS[GID][GoodRX] = 0;
    GOODS[GID][GoodRY] = 0;
    GOODS[GID][GoodRZ] = 0;
    GOODS[GID][Key] = 0;
    GOODS[GID][WID] = GetPlayerVirtualWorld(playerid);
    GOODS[GID][issale] = true;
    GOODS[GID][topublic] = true;
    CreateGoods(GID);
    new tester[125];
    format(tester, sizeof(tester), "[家具系统]你创建了服务器[ID %d ]的家具 [Objid %d][价格 %d]", GID, GOODS[GID][GoodObjid], GOODS[GID][GoodPrize]);
    SendClientMessage(playerid, Color_Furniture, tester);
    //Save it
    SaveGoods(GID);
    return 1;
}
CMD:delgoods(const playerid, const cmdtext[]) {
    if(PlayerInfo[playerid][AdminLevel] < 4) return SendClientMessage(playerid, Color_White, "[家具系统]:你的管理员权限等级不足.");
    new deid;
    if(sscanf(cmdtext, "i", deid)) return SendClientMessage(playerid, Color_White, "[家具]{FF0000}/delgoods{37FF00} [ID]");
    if(deid < 0) return SendClientMessage(playerid, Color_White, "[家具]{FF0000} ID不可为负数!");
    if(GOODS[deid][GoodTaken] != 1) return SendClientMessage(playerid, Color_White, "[家具]错误的*家具*ID");
    new tester[125];
    format(tester, sizeof(tester), "[家具]你已经删除了家具ID[%d] OBJID[%d]", deid, GOODS[deid][GoodObjid]);
    DelGoods(deid); //2020.2.11修复 原作者语句顺序的问题 导致OBJID一直是0 因为删除后OBJID那个数组就被清0了
    SendClientMessage(playerid, Color_Furniture, tester);
    return 1;
}
CMD:gogoods(const playerid, const cmdtext[]) {
    // new gid, tmp[128]; //create string to store values
    new gid;
    if(sscanf(cmdtext, "i", gid)) return SendClientMessage(playerid, Color_White, "[家具]{FF0000}/gogoods{37FF00} [ID]");
    if(GOODS[gid][GoodTaken] != 1) return SendClientMessage(playerid, Color_White, "[家具]错误！家具ID不存在！");
    // gid = strval(tmp);
    // GOODS[gid][OrderId] = strval(tmp); //2020.2.9?0?4?0?7?0?0?0?7gogoods?0?8?ě?0?4?ě
    //create float
    new Float:x, Float:y, Float:z;
    GetDynamicObjectPos(GOODS[gid][OrderId], x, y, z);
    SetPlayerPos(playerid, x, y, z);
    SetPlayerVirtualWorld(playerid, GOODS[gid][WID]);
    new tester[125];
    format(tester, sizeof(tester), "[家具]你已经传送到了家具ID %d  %d", gid, GOODS[gid][OrderId]);
    SendClientMessage(playerid, Color_Furniture, tester);
    return 1;
}
CMD:resetgoods(const playerid, const cmdtext[]) {
    if(PlayerInfo[playerid][AdminLevel] < 4) return SendClientMessage(playerid, Color_White, "[家具系统]:你的管理员权限等级不足.");
    new count = 0;
    for (new i = 0; i < MAX_PLAYERS; i++) {
        if(GOODS_STATUS[i] == true) {
            ResetGoods(playerid, i);
            count++;
        }
    }
    new tester[125];
    format(tester, sizeof(tester), "[家具]所有家具 %d 都被重置了", count);
    SendClientMessage(playerid, Color_Furniture, tester);
    return 1;
}
CMD:mygoods(const playerid, const cmdtext[]) {
    new GOODS_LIST[1000], title[128], tocount, youcount;
    strcat(GOODS_LIST, "{80FF80}[物品ID]\t[物品名称]\t[物品价格]\n");
    for (new i = 0; i < MAX_GOODS; i++) {
        if(GOODS[i][GoodTaken] == 1) {
            tocount++;
            // if(!strcmp(GOODS[i][GoodOwner], GetName(playerid), true)) {
            if(GOODS[i][GoodOwner] == PlayerInfo[playerid][ID]) {
                new str[125];
                youcount++;
                //format(GOODS_LIST,sizeof(GOODS_LIST),"\n{80FF80}%s			%d\n",GOODS[i][GoodName],i);
                format(str, sizeof(str), "ID: %d\t{80FF80}%s\t%d\n", i, GOODS[i][GoodName], GOODS[i][GoodPrize]);
                strcat(GOODS_LIST,str);
            }
        }
    }
    format(title, sizeof(title), "{80FFFF}我的物品 - {80FF80}%d{80FFFF}个 服务器共{80FF80}%d{80FFFF}个 占{80FF80}%d%", youcount, tocount, floatround(youcount * 100.0 / tocount));
    Dialog_Show(playerid, GODIOG_LIST, DIALOG_STYLE_TABLIST_HEADERS, title, GOODS_LIST, "编辑", "关闭");
    return 1;
}
CMD:wdjj(const playerid, const cmdtext[]) return cmd_mygoods(playerid, cmdtext);
CMD:resetowner(const playerid, const cmdtext[]) {
    new gid; //create string to store values
    if(sscanf(cmdtext, "i", gid)) return SendClientMessage(playerid, Color_White, "[家具]{FF0000}/resetowner{37FF00} [ID]");
    SellGoodsToSYS(playerid, gid);
    return 1;
}
CMD:goodshelp(const playerid, const cmdtext[]) {
    Dialog_Show(playerid, MessageBox, DIALOG_STYLE_LIST, "{FF80C0}家具{80FF80}帮助", "{C0C0C0}查看我的物品{FF0000}/mygoods或/wdjj\n{C0C0C0}传送物品{FF0000}/gogoods{C0C0C0}\n操作物品请按{FF0000}Y{C0C0C0}键", "关闭", "");
    return 1;
}
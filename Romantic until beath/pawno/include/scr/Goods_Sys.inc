/*
  -----------------ʹ��˵��------------------------
	You should create a mysql table in order to save
	datas.
  -------------------------------------------------
  ---------------�����--------------------------
  �،���Episodes KiVen juse 
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
// #define MYSQL_HOST	"127.0.0.1" //�ĳɷ�����IP��ַ
// #define MYSQL_USER	"samp" // MYSQL�û���
// #define MYSQL_PASS 	"rst2020!!!"// �ĳ����Լ�������
// #define MYSQL_DB	"rstGoods"// �ĳ����Լ������ݱ�
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
// #define GODIOG_LIST GODIOG_BUY+7 //2020.3.2���� �ٿ��ҵļҾ��б���
//color
// #define {80FF80}     {80FF80}
/*
�ӣ��ͣУ̣š��ϣơ��ͣ٣ӣѣ̡��ԣ��£̣�
GID TAKEN


*/

#define PL_CONCENTS_YES "{FF80C0}������Ʒ\n{FF80FF}�޸�����\n{8080FF}΢������\n{FF0080}��������\n{C0C0C0}������Ʒ = {00FF40}��\n������Ʒ"
#define PL_CONCENTS_NO "{FF80C0}������Ʒ\n{FF80FF}�޸�����\n{8080FF}΢������\n{FF0080}��������\n{C0C0C0}������Ʒ = {FF0000}��\n������Ʒ"

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
    // print("________�ǣϣϣģӡ��ӣ٣ӣԣţ�________");
    print("[�Ҿ�ϵͳ������]��Episodes���� [Creadits By]:Jinzhi");
    print("[��ǰ�汾]:"VER_INFO"");

    //OK First we gonna check mysql right now
    // ConnectMySQL();
    // printf("%d",mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_DB, MYSQL_PASS));
    // select TABLE_NAME from INFORMATION_SCHEMA.TABLES where  TABLE_NAME='user_goods'��
    // mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_DB, MYSQL_PASS)
    // if(mysql_ping!=-1) //����MYSQL������
    
    // if(mysql_ping() != -1)
    // {
    //     if(mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_DB, MYSQL_PASS)){
    //         print("[MySQL] �Ҿ�ϵͳ���ӳɹ�!");
    //         mysql_set_charset(charset);
    //         // printf("%d", mysql_query("select TABLE_NAME from INFORMATION_SCHEMA.TABLES where  TABLE_NAME='user_goods'"));
    //         for (new i = 0; i < MAX_GOODS; i++)
    //         {
    //             LoadGoods(i); //���ؼҾ�OBJ
    //         }
    //         printf("[�Ҿ�]��������%d���Ҿ�", loadcount);
    //     }
    // } 
    // else
    // {
    //     print("[MySQL]�Ҿ�ϵͳ���ӵ�MYSQLʱ��������������!");
    // }
    for (new i = 0; i < MAX_GOODS; i++)
    {
        LoadGoods(i); //���ؼҾ�OBJ
    }
    printf("[�Ҿ�]��������%d���Ҿ�", loadcount);
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
        // ������� ������yes no��
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


// 2021.10.7 �޸�
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
        format(string, sizeof(string), "{80FF80}��Ʒ��������...\n{FFFFFF}[��ƷID]:{80FF80} %d\n{FFFFFF}[���˳��ID]:{80FF80} %d\n{FFFFFF}[ģ��ID]:{80FF80} %d\n{FFFFFF}[�۸�]:{80FF80} %d\n{FFFFFF}��{80FF80}Y{FFFFFF}����", GID, GOODS[GID][OrderId], GOODS[GID][GoodObjid], GOODS[GID][GoodPrize]);
        GOODS[GID][Good3DTextLabel] = CreateDynamic3DTextLabel(string, Color_White, GOODS[GID][GoodX], GOODS[GID][GoodY], GOODS[GID][GoodZ], 4.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, GOODS[GID][WID]);
    } else {
        format(string, sizeof(string), "{80FF80}%s\n{FFFFFF}[����]:{80FF80} %d %s\n{FFFFFF}[��ƷID]: {80FF80}%d\n{FFFFFF}[���˳��ID]:{80FF80} %d\n{FFFFFF}[ģ��ID]:{80FF80} %d\n{FFFFFF}��{80FF80}Y{FFFFFF}����", GOODS[GID][GoodName], GOODS[GID][GoodOwner], GOODS[GID][GoodOwnerName], GID, GOODS[GID][OrderId], GOODS[GID][GoodObjid]);
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
    loadcount--; //2020.3.8��
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
    format(str, sizeof(str), "[�Ҿ�] ������ %d �������������", gotprize);
    SendClientMessage(playerid, 0xFFFFFFC8, str);
    SaveGoods(GID);
}

function UpdateGoods3dtextlabel(GID) {
    new label[256];
    //Check if it sale
    if(GOODS[GID][issale] == true) {
        format(label, sizeof(label), "{80FF80}��Ʒ��������...\n{FFFFFF}[��ƷID]:{80FF80} %d\n{FFFFFF}[���˳��ID]:{80FF80} %d\n{FFFFFF}[ģ��ID]:{80FF80} %d\n{FFFFFF}[�۸�]:{80FF80} %d\n{FFFFFF}��{80FF80}Y{FFFFFF}����", GID, GOODS[GID][OrderId], GOODS[GID][GoodObjid], GOODS[GID][GoodPrize]);
        UpdateDynamic3DTextLabelText(GOODS[GID][Good3DTextLabel], Color_White, label);

    } else {
        format(label, sizeof(label), "{80FF80}%s\n{FFFFFF}[����]:{80FF80} %d %s\n{FFFFFF}[��ƷID]:{80FF80} %d\n{FFFFFF}[���˳��ID]:{80FF80} %d\n{FFFFFF}[ģ��ID]:{80FF80} %d\n{FFFFFF}��{80FF80}Y{FFFFFF}����", GOODS[GID][GoodName], GOODS[GID][GoodOwner], GOODS[GID][GoodOwnerName], GID, GOODS[GID][OrderId], GOODS[GID][GoodObjid]);
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
    SendClientMessage(playerid, 0xFF8000C8, "[�Ҿ�]���������Ʒ!");
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
    	format(string,sizeof(string),"{80FF80}��Ʒ��������...\n{FFFFFF}[��ƷID]:{80FF80} %d\n{FFFFFF}[���˳��ID]:{80FF80} %d\n{FFFFFF}[ģ��ID]:{80FF80} %d\n{FFFFFF}[�۸�]:{80FF80} %d\n{FFFFFF}��{80FF80}Y{FFFFFF}����",GOODS_OPRATEID[playerid],GOODS[GOODS_OPRATEID[playerid]][OrderId],GOODS[GOODS_OPRATEID[playerid]][GoodObjid],GOODS[GOODS_OPRATEID[playerid]][GoodPrize]);
    	GOODS[GOODS_OPRATEID[playerid]][Good3DTextLabel]=CreateDynamic3DTextLabel(string, Color_White,GOODS[GOODS_OPRATEID[playerid]][GoodX]+1,GOODS[GOODS_OPRATEID[playerid]][GoodY]+1,GOODS[GOODS_OPRATEID[playerid]][GoodZ]-0.3, 4.0,INVALID_PLAYER_ID,INVALID_VEHICLE_ID,-1,GOODS[GOODS_OPRATEID[playerid]][WID]);
    	}else{
    	format(string,sizeof(string),"{80FF80}%s\n{FFFFFF}[����]:{80FF80} %s\n{FFFFFF}[��ƷID]: {80FF80}%d\n{FFFFFF}[ģ��ID]:{80FF80} %d\n{FFFFFF}��{80FF80}Y{FFFFFF}����",GOODS[GOODS_OPRATEID[playerid]][GoodName],GOODS[GOODS_OPRATEID[playerid]][GoodOwner],GOODS_OPRATEID[playerid],GOODS[GOODS_OPRATEID[playerid]][GoodObjid]);
    	GOODS[GOODS_OPRATEID[playerid]][Good3DTextLabel]=CreateDynamic3DTextLabel(string, Color_White,GOODS[GOODS_OPRATEID[playerid]][GoodX]+1,GOODS[GOODS_OPRATEID[playerid]][GoodY]+1,GOODS[GOODS_OPRATEID[playerid]][GoodZ]-0.3, 4.0,INVALID_PLAYER_ID,INVALID_VEHICLE_ID,-1,GOODS[GOODS_OPRATEID[playerid]][WID]);
    	}
    	printf("TAKE DOWN WORLD = %d",GOODS[GOODS_OPRATEID[playerid]][WID]);
    	*/
    ClearAnimations(playerid);
    SendClientMessage(playerid, 0x00FFFFC8, "[�Ҿ�]���������Ʒ!");
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
        format(string, sizeof(string), "{80FF80}{80FF80}��Ʒ��������...\n{FFFFFF}[��ƷID]:{80FF80} %d\n{FFFFFF}[���˳��ID]:{80FF80} %d\n{FFFFFF}[ģ��ID]:{80FF80} %d\n{FFFFFF}[�۸�]:{80FF80} %d\n{FFFFFF}��{80FF80}Y{FFFFFF}����", pid, GOODS[pid][OrderId], GOODS[pid][GoodObjid], GOODS[pid][GoodPrize]);
        GOODS[pid][Good3DTextLabel] = CreateDynamic3DTextLabel(string, Color_White, GOODS[pid][GoodX] + 1, GOODS[pid][GoodY] + 1, GOODS[pid][GoodZ] - 0.3, 4.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, GOODS[pid][WID]);
    } else {
        format(string, sizeof(string), "{80FF80}{80FF80}%s\n{FFFFFF}[����]:{80FF80} %d %s\n{FFFFFF}[��ƷID]: {80FF80}%d\n{FFFFFF}[ģ��ID]: {80FF80}%d\n{FFFFFF}��{80FF80}Y{FFFFFF}����", GOODS[pid][GoodName], GOODS[pid][GoodOwner], GOODS[pid][GoodOwnerName], pid, GOODS[pid][GoodObjid]);
        GOODS[pid][Good3DTextLabel] = CreateDynamic3DTextLabel(string, Color_White, GOODS[pid][GoodX] + 1, GOODS[pid][GoodY] + 1, GOODS[pid][GoodZ] - 0.3, 4.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, GOODS[pid][WID]);
    }
    SendClientMessage(pid, 0x00FFFFC8, "[�Ҿ�]����ˢ����Ʒ,ǿ�Ʒ���!");
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

        if(GOODS[GOODS_OPRATEID[playerid]][issale] == false) return Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "{FF0000}ERROR>.<[���ظ�����BUG]", "{FF0000}����Ʒ�Ѿ���������", "��~", "");
        if(GetPlayerMoney(playerid) < GOODS[GOODS_OPRATEID[playerid]][GoodPrize]) return Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "ERROR>.<", "��~���ֲ���", "����", "");

        // if(GetPlayerScore(playerid) < GOODS[GOODS_OPRATEID[playerid]][GoodPrize]) return Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "ERROR>.<", "��~���ֲ���", "����", "");
        // SetPlayerScore(playerid, GetPlayerScore(playerid) - GOODS[GOODS_OPRATEID[playerid]][GoodPrize]);
        GivePlayerMoney(playerid, GOODS[GOODS_OPRATEID[playerid]][GoodPrize] * -1);
        SendClientMessage(playerid, 0xFFFFFFC8, "[�Ҿ�]����ɹ�");
        // new owner[65];
        // GetPlayerName(playerid, owner, 65);
        GOODS[GOODS_OPRATEID[playerid]][GoodOwner] = PlayerInfo[playerid][ID];
        // format(GOODS[GOODS_OPRATEID[playerid]][GoodOwner], 65, owner);
        format(GOODS[GOODS_OPRATEID[playerid]][GoodName], 125, "%s ����Ʒ", GetName(playerid));
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
                format(title, 285, "{FFFFFF}��ƷID:{80FF80} %d {FFFFFF}����:{80FF80} %d - {80FFFF}�����˵�", GOODS_OPRATEID[playerid], GOODS[GOODS_OPRATEID[playerid]][GoodOwner]);
                Dialog_Show(playerid, GODIOG_PL, DIALOG_STYLE_LIST, title, PL_CONCENTS_YES, "ѡ��", "�ر�");
            } else {
                new title[285];
                format(title, 285, "{FFFFFF}��ƷID:\
                    {80FF80} %d {FFFFFF}����:\
                    {80FF80} %d - {80FFFF}�����˵�", GOODS_OPRATEID[playerid], GOODS[GOODS_OPRATEID[playerid]][GoodOwner]);
                Dialog_Show(playerid, GODIOG_PL, DIALOG_STYLE_LIST, title, PL_CONCENTS_NO, "ѡ��", "�ر�");
            }
            return 1;
        }
        return SendClientMessage(playerid, Color_White, "[����ѡ]");
    }
    return 1;
}
Dialog:GODIOG_CGNAME(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        if(!strlen(inputtext)) return Dialog_Show(playerid, GODIOG_CGNAME, DIALOG_STYLE_INPUT, "{FF0000}�ܴa�޸�", "{FF0000}���ֲ���Ϊ��!", "�޸�", "�ر�");
        format(GOODS[GOODS_OPRATEID[playerid]][GoodName], 125, inputtext);
        UpdateGoods3dtextlabel(GOODS_OPRATEID[playerid]);
        SendClientMessage(playerid, 0xFFFFFFC8, "[�Ҿ�]�����޸ĳɹ�");
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
            SendClientMessage(playerid, 0x00FFFFC8, "[�Ҿ�]:��ס�����������Ʒ");
            return 1;
        }
        if(listitem == 1) {
            //here is for name changing
            Dialog_Show(playerid, GODIOG_CGNAME, DIALOG_STYLE_INPUT, "{FF80C0}�����޸�", "{FF80C0}������������", "�޸�", "�ر�");
            return 1;
        }
        if(listitem == 2) {
            //here is for pos editing
            EditDynamicObject(playerid, GOODS[GOODS_OPRATEID[playerid]][OrderId]);
            return 1;
        }
        if(listitem == 3) {
            //here is for key
            Dialog_Show(playerid, GODIOG_KEY, DIALOG_STYLE_INPUT, "{8000FF}����", "{8000FF}�����������Ʒ����(������)\n{FF0000}ע��'0'Ϊû������", "����", "�ر�");
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
                format(title, 285, "{FFFFFF}��ƷID:{80FF80} %d {FFFFFF}����:{80FF80} %d - {80FFFF}�����˵�", GOODS_OPRATEID[playerid], GOODS[GOODS_OPRATEID[playerid]][GoodOwner]);
                Dialog_Show(playerid, GODIOG_PL, DIALOG_STYLE_LIST, title, PL_CONCENTS_YES, "ѡ��", "�ر�");
            } else {
                new title[285];
                format(title, 285, "{FFFFFF}��ƷID:\
                    {80FF80} %d {FFFFFF}����:\
                    {80FF80} %d - {80FFFF}�����˵�", GOODS_OPRATEID[playerid], GOODS[GOODS_OPRATEID[playerid]][GoodOwner]);
                Dialog_Show(playerid, GODIOG_PL, DIALOG_STYLE_LIST, title, PL_CONCENTS_NO, "ѡ��", "�ر�");
            }
            return 1;
        }
        if(listitem == 5) {
            new title[285];
            format(title, 285, "���� {FFFFFF}��ƷID:\
                {80FF80} %d {FFFFFF}����:\
                {80FF80} %d - {80FFFF} �_�J��?", GOODS_OPRATEID[playerid], GOODS[GOODS_OPRATEID[playerid]][GoodOwner]);
            Dialog_Show(playerid, GODIOG_SELL, DIALOG_STYLE_LIST, title, "����ϵͳ(���)\n�������", "ѡ��", "�ر�");
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
            Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "{80FF80}�����޸�", "{80FF80}���뱻ȡ����", "OK", "");
            return 1;
        }
        GOODS[GOODS_OPRATEID[playerid]][Key] = strval(inputtext);
        SendClientMessage(playerid, 0xFFFFFFC8, "[�Ҿ�]�����޸ĳɹ�");
        SaveGoods(GOODS_OPRATEID[playerid]);
        return 1;
    }
    return 1;
}
Dialog:GODIOG_PASS(playerid, response, listitem, inputtext[]) { //password check
    if(response == 1) {
        if(GOODS[GOODS_OPRATEID[playerid]][Key] == strval(inputtext)) {

            SendClientMessage(playerid, 0xFFFFFFC8, "[�Ҿ�]:������ȷ,����Բ��������Ʒ��!");
            if(GOODS[GOODS_OPRATEID[playerid]][topublic] == true) {
                new title[285];
                format(title, 285, "{FFFFFF}��ƷID:{80FF80}%d {FFFFFF}����:{80FF80}%d - {80FFFF}�����˵�", GOODS_OPRATEID[playerid], GOODS[GOODS_OPRATEID[playerid]][GoodOwner]);
                Dialog_Show(playerid, GODIOG_PL, DIALOG_STYLE_LIST, title, PL_CONCENTS_YES, "ѡ��", "�ر�");
            } else {
                new title[285];
                format(title, 285, "{FFFFFF}��ƷID:{80FF80}%d {FFFFFF}����:{80FF80}%d - {80FFFF}�����˵�", GOODS_OPRATEID[playerid], GOODS[GOODS_OPRATEID[playerid]][GoodOwner]);
                Dialog_Show(playerid, GODIOG_PL, DIALOG_STYLE_LIST, title, PL_CONCENTS_NO, "ѡ��", "�ر�");
            }
        } else {
            Dialog_Show(playerid, GODIOG_PASS, DIALOG_STYLE_INPUT, "{80FF80}��ȫ��֤��ʽ", "{FF0000}���������!\nҪ��������������\n{80FF80}���������������ȥ��̳����", "OK", "ȡ��");
            return 1;
        }
    }
    return 1;
}


//�Ҿ�ϵͳ
CMD:creategoods(const playerid, const cmdtext[]) {
    if(PlayerInfo[playerid][AdminLevel] < 4) return SendClientMessage(playerid, Color_White, "[�Ҿ�ϵͳ]:��Ĺ���ԱȨ�޵ȼ�����.");
    //Create String to store in local values
    new modelid, seprice, Float:x, Float:y, Float:z;
    if(sscanf(cmdtext, "ii", modelid, seprice)) return SendClientMessage(playerid, Color_White, "[�Ҿ�ϵͳ]:{FF0000}/creategoods{37FF00} [objid] [�۸�]");
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
    format(tester, sizeof(tester), "[�Ҿ�ϵͳ]�㴴���˷�����[ID %d ]�ļҾ� [Objid %d][�۸� %d]", GID, GOODS[GID][GoodObjid], GOODS[GID][GoodPrize]);
    SendClientMessage(playerid, Color_Furniture, tester);
    //Save it
    SaveGoods(GID);
    return 1;
}
CMD:delgoods(const playerid, const cmdtext[]) {
    if(PlayerInfo[playerid][AdminLevel] < 4) return SendClientMessage(playerid, Color_White, "[�Ҿ�ϵͳ]:��Ĺ���ԱȨ�޵ȼ�����.");
    new deid;
    if(sscanf(cmdtext, "i", deid)) return SendClientMessage(playerid, Color_White, "[�Ҿ�]{FF0000}/delgoods{37FF00} [ID]");
    if(deid < 0) return SendClientMessage(playerid, Color_White, "[�Ҿ�]{FF0000} ID����Ϊ����!");
    if(GOODS[deid][GoodTaken] != 1) return SendClientMessage(playerid, Color_White, "[�Ҿ�]�����*�Ҿ�*ID");
    new tester[125];
    format(tester, sizeof(tester), "[�Ҿ�]���Ѿ�ɾ���˼Ҿ�ID[%d] OBJID[%d]", deid, GOODS[deid][GoodObjid]);
    DelGoods(deid); //2020.2.11�޸� ԭ�������˳������� ����OBJIDһֱ��0 ��Ϊɾ����OBJID�Ǹ�����ͱ���0��
    SendClientMessage(playerid, Color_Furniture, tester);
    return 1;
}
CMD:gogoods(const playerid, const cmdtext[]) {
    // new gid, tmp[128]; //create string to store values
    new gid;
    if(sscanf(cmdtext, "i", gid)) return SendClientMessage(playerid, Color_White, "[�Ҿ�]{FF0000}/gogoods{37FF00} [ID]");
    if(GOODS[gid][GoodTaken] != 1) return SendClientMessage(playerid, Color_White, "[�Ҿ�]���󣡼Ҿ�ID�����ڣ�");
    // gid = strval(tmp);
    // GOODS[gid][OrderId] = strval(tmp); //2020.2.9?0?4?0?7?0?0?0?7gogoods?0?8?��?0?4?��
    //create float
    new Float:x, Float:y, Float:z;
    GetDynamicObjectPos(GOODS[gid][OrderId], x, y, z);
    SetPlayerPos(playerid, x, y, z);
    SetPlayerVirtualWorld(playerid, GOODS[gid][WID]);
    new tester[125];
    format(tester, sizeof(tester), "[�Ҿ�]���Ѿ����͵��˼Ҿ�ID %d  %d", gid, GOODS[gid][OrderId]);
    SendClientMessage(playerid, Color_Furniture, tester);
    return 1;
}
CMD:resetgoods(const playerid, const cmdtext[]) {
    if(PlayerInfo[playerid][AdminLevel] < 4) return SendClientMessage(playerid, Color_White, "[�Ҿ�ϵͳ]:��Ĺ���ԱȨ�޵ȼ�����.");
    new count = 0;
    for (new i = 0; i < MAX_PLAYERS; i++) {
        if(GOODS_STATUS[i] == true) {
            ResetGoods(playerid, i);
            count++;
        }
    }
    new tester[125];
    format(tester, sizeof(tester), "[�Ҿ�]���мҾ� %d ����������", count);
    SendClientMessage(playerid, Color_Furniture, tester);
    return 1;
}
CMD:mygoods(const playerid, const cmdtext[]) {
    new GOODS_LIST[1000], title[128], tocount, youcount;
    strcat(GOODS_LIST, "{80FF80}[��ƷID]\t[��Ʒ����]\t[��Ʒ�۸�]\n");
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
    format(title, sizeof(title), "{80FFFF}�ҵ���Ʒ - {80FF80}%d{80FFFF}�� ��������{80FF80}%d{80FFFF}�� ռ{80FF80}%d%", youcount, tocount, floatround(youcount * 100.0 / tocount));
    Dialog_Show(playerid, GODIOG_LIST, DIALOG_STYLE_TABLIST_HEADERS, title, GOODS_LIST, "�༭", "�ر�");
    return 1;
}
CMD:wdjj(const playerid, const cmdtext[]) return cmd_mygoods(playerid, cmdtext);
CMD:resetowner(const playerid, const cmdtext[]) {
    new gid; //create string to store values
    if(sscanf(cmdtext, "i", gid)) return SendClientMessage(playerid, Color_White, "[�Ҿ�]{FF0000}/resetowner{37FF00} [ID]");
    SellGoodsToSYS(playerid, gid);
    return 1;
}
CMD:goodshelp(const playerid, const cmdtext[]) {
    Dialog_Show(playerid, MessageBox, DIALOG_STYLE_LIST, "{FF80C0}�Ҿ�{80FF80}����", "{C0C0C0}�鿴�ҵ���Ʒ{FF0000}/mygoods��/wdjj\n{C0C0C0}������Ʒ{FF0000}/gogoods{C0C0C0}\n������Ʒ�밴{FF0000}Y{C0C0C0}��", "�ر�", "");
    return 1;
}
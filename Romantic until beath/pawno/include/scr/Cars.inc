// IORP Script
// #include <a_samp>
#define Text_Strlen             256 //默认24
#define ChangeCarsScore     	3000
#define MAX_VEHICLE             500
// #define AC_Dialog   			600
#define ChangeCarTextScore      1500
// #define AC_List                 AC_Dialog + 1
// #define AC_MyCar                AC_Dialog + 2
// #define AC_Text                 AC_Dialog + 3
// #define AC_Color                AC_Dialog + 4
// #define AC_KickPassengers       AC_Dialog + 5
// #define AC_ChangeCars           AC_Dialog + 6
// #define AC_GiveCars             AC_Dialog + 7
// #define CDIALOG_CarZBEditSave   AC_Dialog + 8
// #define CDIALOG_CarZB           AC_Dialog + 9
// #define CDIALOG_CarZBSpeed      AC_Dialog + 10
// #define CDIALOG_MIAN            AC_Dialog + 11
// #define CDIALOG_CarZBParcel     AC_Dialog + 12
// #define AC_DressupList			AC_Dialog + 13
// #define AC_Sell					AC_Dialog + 14 //2020.3.4新增回收给系统 选择卖给系统还是玩家
// #define AC_Sell1				AC_Dialog + 15 //卖给玩家
// #define AC_BUY                  AC_Dialog + 16 //上车提示是否买车的对话框
//修改玩家颜色 已经定义为了AC_Dialog + 17 不要再使用17
// #define Color_ACColor 0xFF8040FF

new VehicleColoursTableRGBA[256] = {
    // The existing colours from San Andreas
    0x000000FF,
    0xF5F5F5FF,
    0x2A77A1FF,
    0x840410FF,
    0x263739FF,
    0x86446EFF,
    0xD78E10FF,
    0x4C75B7FF,
    0xBDBEC6FF,
    0x5E7072FF,
    0x46597AFF,
    0x656A79FF,
    0x5D7E8DFF,
    0x58595AFF,
    0xD6DAD6FF,
    0x9CA1A3FF,
    0x335F3FFF,
    0x730E1AFF,
    0x7B0A2AFF,
    0x9F9D94FF,
    0x3B4E78FF,
    0x732E3EFF,
    0x691E3BFF,
    0x96918CFF,
    0x515459FF,
    0x3F3E45FF,
    0xA5A9A7FF,
    0x635C5AFF,
    0x3D4A68FF,
    0x979592FF,
    0x421F21FF,
    0x5F272BFF,
    0x8494ABFF,
    0x767B7CFF,
    0x646464FF,
    0x5A5752FF,
    0x252527FF,
    0x2D3A35FF,
    0x93A396FF,
    0x6D7A88FF,
    0x221918FF,
    0x6F675FFF,
    0x7C1C2AFF,
    0x5F0A15FF,
    0x193826FF,
    0x5D1B20FF,
    0x9D9872FF,
    0x7A7560FF,
    0x989586FF,
    0xADB0B0FF,
    0x848988FF,
    0x304F45FF,
    0x4D6268FF,
    0x162248FF,
    0x272F4BFF,
    0x7D6256FF,
    0x9EA4ABFF,
    0x9C8D71FF,
    0x6D1822FF,
    0x4E6881FF,
    0x9C9C98FF,
    0x917347FF,
    0x661C26FF,
    0x949D9FFF,
    0xA4A7A5FF,
    0x8E8C46FF,
    0x341A1EFF,
    0x6A7A8CFF,
    0xAAAD8EFF,
    0xAB988FFF,
    0x851F2EFF,
    0x6F8297FF,
    0x585853FF,
    0x9AA790FF,
    0x601A23FF,
    0x20202CFF,
    0xA4A096FF,
    0xAA9D84FF,
    0x78222BFF,
    0x0E316DFF,
    0x722A3FFF,
    0x7B715EFF,
    0x741D28FF,
    0x1E2E32FF,
    0x4D322FFF,
    0x7C1B44FF,
    0x2E5B20FF,
    0x395A83FF,
    0x6D2837FF,
    0xA7A28FFF,
    0xAFB1B1FF,
    0x364155FF,
    0x6D6C6EFF,
    0x0F6A89FF,
    0x204B6BFF,
    0x2B3E57FF,
    0x9B9F9DFF,
    0x6C8495FF,
    0x4D8495FF,
    0xAE9B7FFF,
    0x406C8FFF,
    0x1F253BFF,
    0xAB9276FF,
    0x134573FF,
    0x96816CFF,
    0x64686AFF,
    0x105082FF,
    0xA19983FF,
    0x385694FF,
    0x525661FF,
    0x7F6956FF,
    0x8C929AFF,
    0x596E87FF,
    0x473532FF,
    0x44624FFF,
    0x730A27FF,
    0x223457FF,
    0x640D1BFF,
    0xA3ADC6FF,
    0x695853FF,
    0x9B8B80FF,
    0x620B1CFF,
    0x5B5D5EFF,
    0x624428FF,
    0x731827FF,
    0x1B376DFF,
    0xEC6AAEFF,
    0x000000FF,
    // SA-MP extended colours (0.3x)
    0x177517FF,
    0x210606FF,
    0x125478FF,
    0x452A0DFF,
    0x571E1EFF,
    0x010701FF,
    0x25225AFF,
    0x2C89AAFF,
    0x8A4DBDFF,
    0x35963AFF,
    0xB7B7B7FF,
    0x464C8DFF,
    0x84888CFF,
    0x817867FF,
    0x817A26FF,
    0x6A506FFF,
    0x583E6FFF,
    0x8CB972FF,
    0x824F78FF,
    0x6D276AFF,
    0x1E1D13FF,
    0x1E1306FF,
    0x1F2518FF,
    0x2C4531FF,
    0x1E4C99FF,
    0x2E5F43FF,
    0x1E9948FF,
    0x1E9999FF,
    0x999976FF,
    0x7C8499FF,
    0x992E1EFF,
    0x2C1E08FF,
    0x142407FF,
    0x993E4DFF,
    0x1E4C99FF,
    0x198181FF,
    0x1A292AFF,
    0x16616FFF,
    0x1B6687FF,
    0x6C3F99FF,
    0x481A0EFF,
    0x7A7399FF,
    0x746D99FF,
    0x53387EFF,
    0x222407FF,
    0x3E190CFF,
    0x46210EFF,
    0x991E1EFF,
    0x8D4C8DFF,
    0x805B80FF,
    0x7B3E7EFF,
    0x3C1737FF,
    0x733517FF,
    0x781818FF,
    0x83341AFF,
    0x8E2F1CFF,
    0x7E3E53FF,
    0x7C6D7CFF,
    0x020C02FF,
    0x072407FF,
    0x163012FF,
    0x16301BFF,
    0x642B4FFF,
    0x368452FF,
    0x999590FF,
    0x818D96FF,
    0x99991EFF,
    0x7F994CFF,
    0x839292FF,
    0x788222FF,
    0x2B3C99FF,
    0x3A3A0BFF,
    0x8A794EFF,
    0x0E1F49FF,
    0x15371CFF,
    0x15273AFF,
    0x375775FF,
    0x060820FF,
    0x071326FF,
    0x20394BFF,
    0x2C5089FF,
    0x15426CFF,
    0x103250FF,
    0x241663FF,
    0x692015FF,
    0x8C8D94FF,
    0x516013FF,
    0x090F02FF,
    0x8C573AFF,
    0x52888EFF,
    0x995C52FF,
    0x99581EFF,
    0x993A63FF,
    0x998F4EFF,
    0x99311EFF,
    0x0D1842FF,
    0x521E1EFF,
    0x42420DFF,
    0x4C991EFF,
    0x082A1DFF,
    0x96821DFF,
    0x197F19FF,
    0x3B141FFF,
    0x745217FF,
    0x893F8DFF,
    0x7E1A6CFF,
    0x0B370BFF,
    0x27450DFF,
    0x071F24FF,
    0x784573FF,
    0x8A653AFF,
    0x732617FF,
    0x319490FF,
    0x56941DFF,
    0x59163DFF,
    0x1B8A2FFF,
    0x38160BFF,
    0x041804FF,
    0x355D8EFF,
    0x2E3F5BFF,
    0x561A28FF,
    0x4E0E27FF,
    0x706C67FF,
    0x3B3E42FF,
    0x2E2D33FF,
    0x7B7E7DFF,
    0x4A4442FF,
    0x28344EFF
};
new VehicleLockDoor[][32] = {
    "{00FF00}开",
    "{FF0000}关"
};
enum cInfo {
    ID,
    UsersID,
    ModelID,
    Color1,
    Color2,
    Lock,
    Float:CarX,
    Float:CarY,
    Float:CarZ,
    Float:CarA,
    Value,
    Text[Text_Strlen],
    BanGoto,
    Text3D:TagText,
    GotoID,
    SellValue,
    Mod1,
    Mod2,
    Mod3,
    Mod4,
    Mod5,
    Mod6,
    Mod7,
    Mod8,
    Mod9,
    Mod10,
    Printjob,
    Moded
};
enum VaInfo {
    VehicleID,
    ModelID,
    VaObjectID,
    Float:VaX,
    Float:VaY,
    Float:VaZ,
    Float:VaRX,
    Float:VaRY,
    Float:VaRZ,
    TagObjects
}
new VehAttachedObject[MAX_VEHICLE][15][VaInfo], VehAttachedObjectList[MAX_VEHICLE][15], PlayerEdit[MAX_PLAYERS][3];
new Float:EditSpeed[MAX_PLAYERS];
new CarInfo[MAX_VEHICLE][cInfo], pACPage[MAX_PLAYERS], pACEdit[MAX_PLAYERS], CarCount, pChangeCarColor[MAX_PLAYERS];
// new DB:Cardb;
enum buycarzbtype {
    zbname[32],
        zbmoney,
        zbid
};
new buycarzbs[][buycarzbtype] = {
    {
        "蓝色霓虹灯",
        2000,
        18648
    },
    {
        "绿色霓虹灯",
        2000,
        18649
    },
    {
        "黄色霓虹灯",
        2000,
        18650
    },
    {
        "粉红霓虹灯",
        2000,
        18651
    },
    {
        "新警灯",
        1500,
        19620
    },
    {
        "老警灯",
        1500,
        19419
    },
    {
        "小警灯",
        1500,
        18646
    },
    {
        "尾翼1",
        1000,
        1146
    },
    {
        "尾翼2",
        1000,
        1147
    },
    {
        "尾翼3",
        1000,
        1000
    },
    {
        "尾翼4",
        1000,
        1001
    },
    {
        "尾翼5",
        1000,
        1002
    },
    {
        "尾翼6",
        1000,
        1003
    },
    {
        "鲨鱼",
        2500,
        1608
    },
    {
        "乌龟",
        2500,
        1609
    },
    {
        "鹦鹉",
        3500,
        19079
    },
    {
        "海豚",
        2500,
        1607
    },
    {
        "TAXI1",
        1500,
        19308
    },
    {
        "TAXI2",
        1500,
        19311
    },
    {
        "鹿",
        1500,
        19315
    },
    {
        "防空炮",
        2500,
        3267
    },
    {
        "导弹",
        2500,
        3790
    },
    {
        "爆闪灯",
        1500,
        19150
    },
    {
        "日光灯(小)",
        1500,
        19279
    },
    {
        "日光灯(大)",
        1500,
        19280
    },
    {
        "复古大炮",
        3000,
        2064
    },
    {
        "喷火",
        2500,
        18694
    },
    {
        "加特林",
        2000,
        2985
    },
    {
        "火焰（中）",
        1500,
        18698
    },
    {
        "火龙头",
        2500,
        3528
    },
    {
        "水花",
        1000,
        18740
    }
};
new TagObjectsState[][16] = {
    "未装扮",
    "装扮中"
};
new ListGotoNickName[][16] = {
    "{00FF00}开",
    "{FF0000}关"
};
stock UpdataACarData(const i) {
    new Query[1024], temp[512];
    strins(temp, "UPDATE `cars` SET `UsersID` = '%d',`ModelID` = '%d',`Color1` = '%d',`Color2` = '%d',`Lock` = '%d',`X` = '%f'", strlen(temp));
    strins(temp, ",`Y` = '%f',`Z`= '%f',`A` = '%f',`Text` = '%e',`BanGoto` = '%d',`Mod1`='%d',`Mod2`='%d',`Mod3`='%d',`Mod4`='%d'", strlen(temp));
    strins(temp, ",`Mod5`='%d',`Mod6`='%d',`Mod7`='%d',`Mod8`='%d',`Mod9`='%d',`Mod10`='%d',`Moded`='%d',`Printjob`='%d' WHERE `ID` = '%d'", strlen(temp));
    
    mysql_format(g_Sql, Query, sizeof(Query), temp, CarInfo[i][UsersID], CarInfo[i][ModelID], CarInfo[i][Color1], CarInfo[i][Color2],\
    CarInfo[i][Lock], CarInfo[i][CarX], CarInfo[i][CarY], CarInfo[i][CarZ], CarInfo[i][CarA], CarInfo[i][Text], CarInfo[i][BanGoto],\
    CarInfo[i][Mod1], CarInfo[i][Mod2], CarInfo[i][Mod3], CarInfo[i][Mod4], CarInfo[i][Mod5], CarInfo[i][Mod6], CarInfo[i][Mod7], \
    CarInfo[i][Mod8], CarInfo[i][Mod9], CarInfo[i][Mod10], CarInfo[i][Moded], CarInfo[i][Printjob], CarInfo[i][ID]);
    mysql_pquery(g_Sql, Query);
    // format(Query, sizeof(Query), "UPDATE `cars` SET UsersID = '%d',ModelID = '%d',Color1 = '%d',Color2 = '%d',Lock = '%d',X = '%f',Y = '%f',Z = '%f',A = '%f',Text = '%s',BanGoto = '%d',Mod1='%d',Mod2='%d',Mod3='%d',Mod4='%d',Mod5='%d',Mod6='%d',Mod7='%d',Mod8='%d',Mod9='%d',Mod10='%d',Moded='%d',Printjob='%d' WHERE ID = '%d'",
    //     CarInfo[i][UsersID], CarInfo[i][ModelID], CarInfo[i][Color1], CarInfo[i][Color2], CarInfo[i][Lock], CarInfo[i][CarX], CarInfo[i][CarY], CarInfo[i][CarZ], CarInfo[i][CarA], CarInfo[i][Text], CarInfo[i][BanGoto],
    //     CarInfo[i][Mod1], CarInfo[i][Mod2], CarInfo[i][Mod3], CarInfo[i][Mod4], CarInfo[i][Mod5], CarInfo[i][Mod6], CarInfo[i][Mod7], CarInfo[i][Mod8], CarInfo[i][Mod9], CarInfo[i][Mod10], CarInfo[i][Moded], CarInfo[i][Printjob], CarInfo[i][ID]);
    // db_free_result(db_query(Cardb, msg));
    return 1;
}
stock pBuyAC(const playerid) {
    if(GetPlayerVehicleSeat(playerid) != 0) return SendClientMessage(playerid, Color_ACColor, "[爱车] 购买失败,原因:请先移步至想要购买的爱车上.");
    new vid = GetPlayerVehicleID(playerid);
    if(CarInfo[vid][ID] == 0) {
        SCM(playerid, Color_ACColor, "[爱车] 购买失败,原因:该车不能购买.");
        new Float:x,Float:y,Float:z;
        GetPlayerPos(playerid,x,y,z);
		SetPlayerPos(playerid,x,y,z+2);
        SetVehiclePos(vid, CarInfo[vid][CarX], CarInfo[vid][CarY], CarInfo[vid][CarZ]);
        return 1;
    }
    Search_MyCar_of_CarCont(playerid);
    if(pACEdit[playerid] != -1) {
        SCM(playerid, Color_ACColor, "[爱车] 购买失败,原因:你已经有一部了.");
        new Float:x,Float:y,Float:z;
        GetPlayerPos(playerid,x,y,z);
		SetPlayerPos(playerid,x,y,z+2);
        SetVehiclePos(vid, CarInfo[vid][CarX], CarInfo[vid][CarY], CarInfo[vid][CarZ]);
        return 1;
    }
    if(PlayerInfo[playerid][Cash] < CarInfo[vid][Value]) {
        SCM(playerid, Color_ACColor, "[爱车] 购买失败,原因:你的金钱不够.");
        new Float:x,Float:y,Float:z;
        GetPlayerPos(playerid,x,y,z);
		SetPlayerPos(playerid,x,y,z+2);
        SetVehiclePos(vid, CarInfo[vid][CarX], CarInfo[vid][CarY], CarInfo[vid][CarZ]);
        return 1;
    }
    if(CarInfo[vid][UsersID] != 0) //玩家已经买了再出售的车
    {
        if(CarInfo[vid][SellValue] == 0) return SCM(playerid, Color_ACColor, "[爱车] 购买失败,原因:该爱车已经名花有主了.");
        new flag = true; //判断玩家是否在线;
        for (new i = GetPlayerPoolSize(); i >= 0; i--) {
            if(IsPlayerConnected(i)) {
                if(PlayerInfo[i][Login]) {
                    if(PlayerInfo[i][ID] == CarInfo[vid][UsersID]) {
                        GivePlayerCash(i, CarInfo[vid][SellValue]); //卖家拿钱
                        GivePlayerCash(playerid, -CarInfo[vid][SellValue]); //买家给钱
                        new msg[256];
                        format(msg, sizeof(msg), "[爱车] 你的爱车已卖出,成交价%d,购买者:%s", CarInfo[vid][SellValue], GetName(playerid));
                        SCM(i, Color_ACColor, msg);
                        DestroyDynamic3DTextLabel(CarInfo[vid][TagText]);
                        format(CarInfo[vid][Text], Text_Strlen, "%s", GetName(playerid));
                        CarInfo[vid][TagText] = CreateDynamic3DTextLabel(CarInfo[vid][Text], Color_ACColor, 0.0, 0.0, 0.0, 20.0, INVALID_PLAYER_ID,vid);
                        // Attach3DTextLabelToVehicle(CarInfo[vid][TagText], vid, 0.0, 0.0, 0.0);
                        mysql_format(g_Sql, msg, sizeof(msg), "UPDATE `cars` SET `SellValue` = '%d' WHERE `ID` = '%d'", 0, CarInfo[vid][UsersID]);
                        mysql_pquery(g_Sql, msg);
                        // format(msg, sizeof(msg), "UPDATE `cars` SET SellValue = '%d' WHERE ID = '%d'", 0, CarInfo[vid][UsersID]);
                        // db_free_result(db_query(Cardb, msg));
						// CarInfo[vid][ID] = PlayerInfo[playerid][ID]; //尝试性修复买完爱车不出现在爱车列表里的情况  这个是有问题的 到时候看看是什么贵东西
                        //  2020.3.6去掉这句话
                        CarInfo[vid][UsersID] = PlayerInfo[playerid][ID];
                        CarInfo[vid][SellValue] = 0; //2020.3.4增加 不然可能导致刚卖出去的还能再买入
                        UpdataACarData(vid);
                        flag = false;
                        break;
                        // u_yes = 1;
                    }
                }
            }
        }
        if(flag) {
            new msg[256];
            mysql_format(g_Sql, msg, sizeof(msg), "SELECT `Cash` FROM `users` WHERE `ID` = '%d'", CarInfo[vid][UsersID]);
            mysql_pquery(g_Sql, msg, "UpdateOfflineCarsSale", "dd", vid, playerid);
        }
        // if(u_yes == 0)
        // {
        //     format(msg,sizeof(msg),"游戏币 %d",CarInfo[vid][SellValue]);
        // 	OfflineMessage_InsertInto("系统",GetUserName(u_id),"你的爱车已卖出",msg);
        // }
        return 1;
    } else { //系统车
        GivePlayerCash(playerid, -CarInfo[vid][Value]);
		// CarInfo[vid][ID] = PlayerInfo[playerid][ID]; //尝试性修复买完爱车不出现在爱车列表里的情况
        // 这句话有问题
        CarInfo[vid][UsersID] = PlayerInfo[playerid][ID];
        DestroyDynamic3DTextLabel(CarInfo[vid][TagText]);
        format(CarInfo[vid][Text], Text_Strlen, "%s", GetName(playerid));
        CarInfo[vid][TagText] = CreateDynamic3DTextLabel(CarInfo[vid][Text], Color_ACColor, 0.0, 0.0, 0.0, 20.0, INVALID_PLAYER_ID,vid);
        // Attach3DTextLabelToVehicle(CarInfo[vid][TagText], vid, 0.0, 0.0, 0.0);
        UpdataACarData(vid);
    }
    return 1;
}
stock AdminCreateCar(const modelid, const value, Float:x, Float:y, Float:z, Float:a, const usersid = 0) {
    new vid, engine, lights, alarm, doors, bonnet, boot, objective, msg[2048];
    CarCount++;
    vid = AddStaticVehicleEx(modelid, x + 2, y + 2, z, a, 0, 0, -1);
    SetVehicleVirtualWorld(vid, -1);
    CarInfo[vid][UsersID] = usersid;
    CarInfo[vid][ModelID] = modelid;
    CarInfo[vid][CarX] = x;
    CarInfo[vid][CarY] = y;
    CarInfo[vid][CarZ] = z;
    CarInfo[vid][CarA] = a;
    CarInfo[vid][Color1] = 0;
    CarInfo[vid][Color2] = 0;
    CarInfo[vid][Lock] = 0;
    CarInfo[vid][Value] = value;
    format(CarInfo[vid][Text], Text_Strlen, "[爱车]该载具由管理员创建\n可购买,价格%d", value);
    CarInfo[vid][GotoID] = vid;
    CarInfo[vid][BanGoto] = 0;
    CarInfo[vid][SellValue] = 0;
    CarInfo[vid][Mod1] = 0;
    CarInfo[vid][Mod2] = 0;
    CarInfo[vid][Mod3] = 0;
    CarInfo[vid][Mod4] = 0;
    CarInfo[vid][Mod5] = 0;
    CarInfo[vid][Mod6] = 0;
    CarInfo[vid][Mod7] = 0;
    CarInfo[vid][Mod8] = 0;
    CarInfo[vid][Mod9] = 0;
    CarInfo[vid][Mod10] = 0;
    CarInfo[vid][Moded] = 0;
    CarInfo[vid][Printjob] = 0;
    CarInfo[vid][TagText] = CreateDynamic3DTextLabel(CarInfo[vid][Text], Color_ACColor, 0.0, 0.0, 0.0, 20.0, INVALID_PLAYER_ID,vid);
    // Attach3DTextLabelToVehicle(CarInfo[vid][TagText], vid, 0.0, 0.0, 0.0);
    GetVehicleParamsEx(vid, engine, lights, alarm, doors, bonnet, boot, objective);
    SetVehicleParamsEx(vid, engine, lights, alarm, CarInfo[vid][Lock], bonnet, boot, objective);
    SetVehicleVirtualWorld(vid, 0);
    // mysql_tquery(g_Sql, "START TRANSACTION;"); // Start transaction.
    // 2021.10.7 尝试修复插入数据库因关键词冲突报错问题
    mysql_format(g_Sql, msg, sizeof(msg), "INSERT INTO `cars` (`UsersID`,`ModelID`,`Color1`,`Color2`,`Lock`,`X`,`Y`,`Z`,`A`,`Value`,`Text`, \
    `BanGoto`,`Mod1`,`Mod2`,`Mod3`,`Mod4`,`Mod5`,`Mod6`,`Mod7`,`Mod8`,`Mod9`,`Mod10`,`Moded`,`Printjob`) VALUES ('%d','%d','%d','%d','%d','%f', \
    '%f','%f','%f','%d','%e','%d','%d',\
    '%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d');",usersid, modelid, 0, 0, 0, x, y, z, a, value, CarInfo[vid][Text], \
    CarInfo[vid][BanGoto], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    mysql_tquery(g_Sql, msg, "CreateCarIDQuery", "d", vid); // Commit transaction. 
    // mysql_tquery(g_Sql, "COMMIT;"); // Commit transaction. 
    // db_free_result(db_query(Cardb, "begin transaction"));
    // format(msg, sizeof(msg), "INSERT INTO Cars (UsersID,ModelID,Color1,Color2,Lock,X,Y,Z,A,Value,Text,BanGoto,Mod1,Mod2,Mod3,Mod4,Mod5,Mod6,Mod7,Mod8,Mod9,Mod10,Moded,Printjob) VALUES ('%d','%d','%d','%d','%d','%f','%f','%f','%f','%d','%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')",
    //     usersid, modelid, 0, 0, 0, x, y, z, a, value, CarInfo[vid][Text], CarInfo[vid][BanGoto], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    // db_free_result(db_query(Cardb, msg));
    // format(msg, sizeof(msg), "SELECT seq FROM sqlite_sequence WHERE name = 'Cars'");
    // cf = db_query(Cardb, msg);
    // db_get_field(cf, 0, msg, sizeof(msg));
    // CarInfo[vid][ID] = strval(msg);
    // db_free_result(cf);
    // db_free_result(db_query(Cardb, "commit transaction"));
    return 0;
}

function CreateCarIDQuery(vid) {
    CarInfo[vid][ID] = cache_insert_id();
}
// stock Player_ACSpawn(const i) {
//     SetVehiclePos(i, CarInfo[i][CarX], CarInfo[i][CarY], CarInfo[i][CarZ]);
//     ChangeVehicleColor(i, CarInfo[i][Color1], CarInfo[i][Color2]);
//     SetVehicleVirtualWorld(i, -1);
//     new engine, lights, alarm, doors, bonnet, boot, objective;
//     GetVehicleParamsEx(i, engine, lights, alarm, doors, bonnet, boot, objective);
//     SetVehicleParamsEx(i, engine, lights, alarm, CarInfo[i][Lock], bonnet, boot, objective);
//     SetVehicleZAngle(i, CarInfo[i][CarA]);
//     return 1;
// }
stock Cars_OnGameModeExit() {
    for (new i = 0; i < CarCount; i++) {
        if(CarInfo[i][ID] != 0) {
            // DestroyVehicle(i);
            DestroyVehicle(CarInfo[i][GotoID]); //2020.3.6改
            DestroyDynamic3DTextLabel(CarInfo[i][TagText]);
            CarInfo[i][ID] = 0;
            CarInfo[i][UsersID] = 0;
            CarInfo[i][ModelID] = 0;
            CarInfo[i][Color1] = 0;
            CarInfo[i][Color2] = 0;
            CarInfo[i][Lock] = 0;
            CarInfo[i][CarX] = 0;
            CarInfo[i][CarY] = 0;
            CarInfo[i][CarZ] = 0;
            CarInfo[i][CarA] = 0;
            CarInfo[i][Value] = 0;
            format(CarInfo[i][Text], Text_Strlen, " ");
            CarInfo[i][GotoID] = 0;
            CarInfo[i][BanGoto] = 0;
            CarInfo[i][SellValue] = 0;
            for (new v = 0; v < 15; v++) {
                if(VehAttachedObject[i][v][ModelID] != 0) {
                    DestroyDynamicObject(VehAttachedObject[i][v][VaObjectID]);
                }
            }
        }
    }
    // db_close(Cardb);
    return 1;
}
stock Initialize_Cars() {
    // if(!fexist("Cars/Cars.db")) {
    //     print("Cars >> 找不到数据库文件!");
    // } else {
    
    // Cardb = db_open("Cars/Cars.db");
    // format(msg, sizeof(msg), "SELECT * FROM Cars");
    // cf = db_query(Cardb, msg);
    // }
    SetupCarsTable();
    new Query[128];
    mysql_format(g_Sql, Query, sizeof(Query), "SELECT * FROM `cars`");
    mysql_pquery(g_Sql, Query, "InitializeCarsQuery");
    return 1;
}
function InitializeCarsQuery() {
    CarCount = cache_num_rows();
    for (new i = 0; i < CarCount; i++) {
        new modelid, Float:x, Float:y, Float:z, Float:a, color1, color2;
        cache_get_value_index_int(i, 2, modelid);
        cache_get_value_index_int(i, 3, color1);
        cache_get_value_index_int(i, 4, color2);
        cache_get_value_index_float(i, 6, x);
        cache_get_value_index_float(i, 7, y);
        cache_get_value_index_float(i, 8, z);
        cache_get_value_index_float(i, 9, a);

        new vid = AddStaticVehicleEx(modelid, x, y, z, a, color1, color2, -1);
        SetVehicleVirtualWorld(vid, -1);

        CarInfo[i][GotoID] = vid;
        CarInfo[i][ModelID] = modelid;
        CarInfo[i][CarX] = x;
        CarInfo[i][CarY] = y;
        CarInfo[i][CarZ] = z;
        CarInfo[i][CarA] = a;
        CarInfo[i][Color1] = color1;
        CarInfo[i][Color2] = color2;


        cache_get_value_index_int(i, 0, CarInfo[i][ID]);
        cache_get_value_index_int(i, 1, CarInfo[i][UsersID]);
        cache_get_value_index_int(i, 5, CarInfo[i][Lock]);
        cache_get_value_index_int(i, 10, CarInfo[i][Value]);
        cache_get_value_index(i, 11, CarInfo[i][Text], Text_Strlen);
        cache_get_value_index_int(i, 12, CarInfo[i][BanGoto]);
        cache_get_value_index_int(i, 13, CarInfo[i][SellValue]);
        cache_get_value_index_int(i, 14, CarInfo[i][Mod1]);
        cache_get_value_index_int(i, 15, CarInfo[i][Mod2]);
        cache_get_value_index_int(i, 16, CarInfo[i][Mod3]);
        cache_get_value_index_int(i, 17, CarInfo[i][Mod4]);
        cache_get_value_index_int(i, 18, CarInfo[i][Mod5]);
        cache_get_value_index_int(i, 19, CarInfo[i][Mod6]);
        cache_get_value_index_int(i, 20, CarInfo[i][Mod7]);
        cache_get_value_index_int(i, 21, CarInfo[i][Mod8]);
        cache_get_value_index_int(i, 22, CarInfo[i][Mod9]);
        cache_get_value_index_int(i, 23, CarInfo[i][Mod10]);
        cache_get_value_index_int(i, 24, CarInfo[i][Moded]);
        cache_get_value_index_int(i, 25, CarInfo[i][Printjob]);


        if(CarInfo[vid][Moded] == 1) 
        {
            AddVehicleComponent(vid, CarInfo[i][Mod1]);
            AddVehicleComponent(vid, CarInfo[i][Mod2]);
            AddVehicleComponent(vid, CarInfo[i][Mod3]);
            AddVehicleComponent(vid, CarInfo[i][Mod4]);
            AddVehicleComponent(vid, CarInfo[i][Mod5]);
            AddVehicleComponent(vid, CarInfo[i][Mod6]);
            AddVehicleComponent(vid, CarInfo[i][Mod7]);
            AddVehicleComponent(vid, CarInfo[i][Mod8]);
            AddVehicleComponent(vid, CarInfo[i][Mod9]);
            AddVehicleComponent(vid, CarInfo[i][Mod10]);
            ChangeVehiclePaintjob(vid, CarInfo[i][Printjob]);
        }
        CarInfo[i][TagText] = CreateDynamic3DTextLabel(CarInfo[i][Text], Color_ACColor, 0.0, 0.0, 0.0, 20.0, INVALID_PLAYER_ID,vid);
        // Attach3DTextLabelToVehicle(CarInfo[vid][TagText], vid, 0.0, 0.0, 0.0);
        new engine, lights, alarm, doors, bonnet, boot, objective;
        GetVehicleParamsEx(vid, engine, lights, alarm, doors, bonnet, boot, objective);
        SetVehicleParamsEx(vid, engine, lights, alarm, CarInfo[i][Lock], bonnet, boot, objective);
        SetVehicleVirtualWorld(vid, 0);
        new Query[128];
        mysql_format(g_Sql, Query, sizeof(Query), "SELECT * FROM `va` WHERE `VehicleID` = %d", CarInfo[i][ID]);
        mysql_pquery(g_Sql, Query, "CarsObjectsLoadQuery", "d", vid);
    }
    printf("Car >> 读取Car[%d]辆,剩余可用Car[%d]辆", CarCount, MAX_VEHICLE - CarCount);
}

function CarsObjectsLoadQuery(vid) {
    new cut = cache_num_rows();
    if(cut > 0) {
        for (new i = 0; i < cut; i++) {
            new slot;
            cache_get_value_name_int(i, "Slot", slot);
            cache_get_value_name_int(i, "Model", VehAttachedObject[vid][slot][ModelID]);
            cache_get_value_name_float(i, "X", VehAttachedObject[vid][slot][VaX]);
            cache_get_value_name_float(i, "Y", VehAttachedObject[vid][slot][VaY]);
            cache_get_value_name_float(i, "Z", VehAttachedObject[vid][slot][VaZ]);
            cache_get_value_name_float(i, "RX", VehAttachedObject[vid][slot][VaRX]);
            cache_get_value_name_float(i, "RY", VehAttachedObject[vid][slot][VaRY]);
            cache_get_value_name_float(i, "RZ", VehAttachedObject[vid][slot][VaRZ]);
            cache_get_value_name_int(i, "TagObjects", VehAttachedObject[vid][slot][TagObjects]);

            if(VehAttachedObject[vid][slot][TagObjects] == 1) 
            {
                VehAttachedObject[vid][slot][VaObjectID] = CreateDynamicObject(VehAttachedObject[vid][slot][ModelID], 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
                AttachDynamicObjectToVehicle(VehAttachedObject[vid][slot][VaObjectID], vid, VehAttachedObject[vid][slot][VaX], VehAttachedObject[vid][slot][VaY],
                    VehAttachedObject[vid][slot][VaZ], VehAttachedObject[vid][slot][VaRX], VehAttachedObject[vid][slot][VaRY], VehAttachedObject[vid][slot][VaRZ]);
            }
        }
    }
}

/*
stock VehicleAttachedObjectUpdate()
{
	for(new playerid;playerid < MAX_PLAYERS;playerid ++)
	{
	    if(IsPlayerConnected(playerid))
	    {
    		if(PlayerEdit[playerid][2] != 0)
    		{
    			new Keys,ud,lr;
    			GetPlayerKeys(playerid,Keys,ud,lr);
    			if(Keys == KEY_LOOK_BEHIND)
    			{
                    new str[128];
					format(str,sizeof(str),"左右\n前后\n上下\n前翻\n侧翻\n旋转\n调速\n{FF0000}删除\n状态:%s",TagObjectsState[VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][TagObjects]]);
 					ShowPlayerDialog(playerid, CDIALOG_CarZB, DIALOG_STYLE_LIST, "物件编辑", str, "选择", "退出");
				}
    			if(Keys == KEY_FIRE)
    			{
    			     ShowPlayerDialog(playerid,CDIALOG_CarZBEditSave,DIALOG_STYLE_LIST,"物件编辑","?鸤n?鸤n?鸤n\
					 跳出三界外\n不在五行中\n------------------------------\n{FF0000}点我重置该装扮\n------------------------------\n{00FF00}点我保存该装扮\n------------------------------","保存","重置");
    			}
    			if(Keys == KEY_ANALOG_LEFT)
    			{
    			    DestroyDynamicObject(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaObjectID]);
    			    VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaObjectID] = CreateDynamicObject(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][ModelID],0.0,0.0,0.0,0.0,0.0,0.0);
                    switch (PlayerEdit[playerid][2])
    			    {
    			        case 1:VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaX] -= EditSpeed[playerid];
    			        case 2:VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaY] -= EditSpeed[playerid];
    			        case 3:VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaZ] -= EditSpeed[playerid];
    			        case 4:VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRX] -= EditSpeed[playerid];
    			        case 5:VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRY] -= EditSpeed[playerid];
    			        case 6:VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRZ] -= EditSpeed[playerid];
    			    }
					AttachDynamicObjectToVehicle(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaObjectID],PlayerEdit[playerid][0],
					VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaX],
					VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaY],
					VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaZ],
					VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRX],
					VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRY],
					VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRZ]);
					Streamer_UpdateAll();
    			}
    			if(Keys == KEY_ANALOG_RIGHT)
   				{

    			    DestroyDynamicObject(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaObjectID]);
                    VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaObjectID] = CreateDynamicObject(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][ModelID],0.0,0.0,0.0,0.0,0.0,0.0);
                    switch (PlayerEdit[playerid][2])
    			    {
    			        case 1:VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaX] += EditSpeed[playerid];
    			        case 2:VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaY] += EditSpeed[playerid];
    			        case 3:VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaZ] += EditSpeed[playerid];
    			        case 4:VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRX] += EditSpeed[playerid];
    			        case 5:VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRY] += EditSpeed[playerid];
    			        case 6:VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRZ] += EditSpeed[playerid];
    			    }
					AttachDynamicObjectToVehicle(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaObjectID],PlayerEdit[playerid][0],
					VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaX],
					VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaY],
					VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaZ],
					VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRX],
					VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRY],
					VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRZ]);
					Streamer_UpdateAll();
    			}
    		}
    	}
    }
}*/
stock pViewACList(const playerid, const page) {
    new msg[1024];
    format(msg, sizeof(msg), "%s\n{0088FF}上一页", msg);
    for (new i = (page - 1) * 15; i < page * 15; i++) {
    	if(CarInfo[i][ID] != 0) { //2020.3.4取消限制 这样子还没出售的车应该也能买了
    		format(msg, sizeof(msg), "%s\n传送 %d\t载具ID %s(%d)\t价格 %d", msg, CarInfo[i][GotoID], VehicleNames[CarInfo[i][ModelID] - 400], CarInfo[i][ModelID], CarInfo[i][Value]);
        }
    }
    format(msg, sizeof(msg), "%s\n{0088FF}下一页", msg);
    Dialog_Show(playerid, AC_List, DIALOG_STYLE_LIST, "爱车列表", msg, "确定", "关闭");
    return 1;
}
stock Search_MyCar_of_CarCont(const playerid) {
    new s = 0;
    for (new i = 0; i < MAX_VEHICLE; i++) {
        if(CarInfo[i][UsersID] == PlayerInfo[playerid][ID]) {
            pACEdit[playerid] = i;
            s = i;
            break;
            // i = CarCount + 1;
        }
    }
    if(s == 0) pACEdit[playerid] = -1;
    return 1;
}
stock pViewMyCar(const playerid) {
    new cartext[48];
    Search_MyCar_of_CarCont(playerid);
    if(pACEdit[playerid] == -1) return SCM(playerid, Color_ACColor, "[爱车] 查看失败,原因:你没有爱车.");
    if(CarInfo[pACEdit[playerid]][Moded] == 0) {
        cartext = "{F81414}关{FFFFFF}]";
    }
    if(CarInfo[pACEdit[playerid]][Moded] == 1) {
        cartext = "{37DB45}开{FFFFFF}]";
    }
    /*if(CarInfo[pACEdit[playerid]][Moded])==0)
    {
    cartext="{F81414}关{FFFFFF}]";
    }
    if(CarInfo[pACEdit[playerid]][Moded])==1)
    {
    cartext="{37DB45}开{FFFFFF}]";
    }*/
    new msg[256];
    format(msg, sizeof(msg), "车门状态\n更改坐标\n禁止传送\n查看状态\n修改文字\n修改颜色\n踢出乘客\n召唤爱车\n更换爱车\n给予爱车\n爱车物件\n{FF0000}爱车出售\n保存改装:[%s", cartext);
    Dialog_Show(playerid, AC_MyCar, DIALOG_STYLE_LIST, "我的爱车", msg, "确定", "关闭");
    return 1;
}
stock CallMyAc(const playerid){
    new msg[256];
    format(msg,sizeof(msg),"[爱车] 你召唤了你的爱车[%s(%d)].",VehicleNames[GetVehicleModel(CarInfo[pACEdit[playerid]][GotoID])-400],GetVehicleModel(CarInfo[pACEdit[playerid]][GotoID]));
    SCM(playerid,Color_ACColor,msg);
    new Float:x,Float:y,Float:z;
    GetPlayerPos(playerid,x,y,z);
    SetVehiclePos(CarInfo[pACEdit[playerid]][GotoID],x+1,y+1,z+1);
    SetVehicleVirtualWorld(CarInfo[pACEdit[playerid]][GotoID],GetPlayerVirtualWorld(playerid));
    PutPlayerInVehicle(playerid,CarInfo[pACEdit[playerid]][GotoID],0);
    return 1;
}
stock ChangeCarColor(const playerid, const page) {
    new msg[1024];
    // if(pChangeCarColor[playerid] != 1 && pChangeCarColor[playerid] != 2)
    // {
    // 	format(msg,sizeof(msg),"{0088FF}请选择颜色 当前第 %d 页 - 共 %d 页",page,GetMaxPage(sizeof(VehicleColoursTableRGBA)));
    // }
    // else
    // {
    format(msg, sizeof(msg), "{0088FF}请选择颜色 %d 当前第 %d 页 - 共 %d 页", pChangeCarColor[playerid], page, GetMaxPage(sizeof(VehicleColoursTableRGBA)));
    // }
    format(msg, sizeof(msg), "%s\n上一页", msg);
    for (new i = (page - 1) * 10; i < page * 10; i++) {
        format(msg, sizeof(msg), "%s\nColorID\t%d\t{%06x}▇\t▇\t▇", msg, i, VehicleColoursTableRGBA[i] >>> 8);
    }
    format(msg, sizeof(msg), "%s\n下一页", msg);
    Dialog_Show(playerid, AC_Color, DIALOG_STYLE_LIST, "我的爱车", msg, "更改", "关闭");
    return 1;
}
/*stock Streamer_UpdateAll()
{
	for(new i=0;i<MAX_PLAYERS;i++)
	    {
	        if(IsPlayerConnected(i)==1)
	            {
	                Streamer_Update(i);
	            }
	    }
}*/
// forward Cars_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
// public Cars_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
// {
// 	new msg[256];

// 	return 1;
// }
public OnVehicleSpawn(vehicleid) //汽车出生的事件
{
    new vid = vehicleid;
    SetVehiclePos(vehicleid, CarInfo[vid][CarX], CarInfo[vid][CarY], CarInfo[vid][CarZ]);
    SetVehicleZAngle(vehicleid, CarInfo[vid][CarA]);
    ChangeVehicleColor(vehicleid,CarInfo[vid][Color1], CarInfo[vid][Color2]);
    if(CarInfo[vid][Moded] == 0) {
        CarInfo[vid][Mod1] = 0;
        CarInfo[vid][Mod2] = 0;
        CarInfo[vid][Mod3] = 0;
        CarInfo[vid][Mod4] = 0;
        CarInfo[vid][Mod5] = 0;
        CarInfo[vid][Mod6] = 0;
        CarInfo[vid][Mod7] = 0;
        CarInfo[vid][Mod8] = 0;
        CarInfo[vid][Mod9] = 0;
        CarInfo[vid][Mod10] = 0;
    }
    if(CarInfo[vid][Moded] == 1) {
        AddVehicleComponent(vid, CarInfo[vid][Mod1]);
        AddVehicleComponent(vid, CarInfo[vid][Mod2]);
        AddVehicleComponent(vid, CarInfo[vid][Mod3]);
        AddVehicleComponent(vid, CarInfo[vid][Mod4]);
        AddVehicleComponent(vid, CarInfo[vid][Mod5]);
        AddVehicleComponent(vid, CarInfo[vid][Mod6]);
        AddVehicleComponent(vid, CarInfo[vid][Mod7]);
        AddVehicleComponent(vid, CarInfo[vid][Mod8]);
        AddVehicleComponent(vid, CarInfo[vid][Mod9]);
        AddVehicleComponent(vid, CarInfo[vid][Mod10]);
        ChangeVehiclePaintjob(vid, CarInfo[vid][Printjob]);
    }
    return 1;
}
public OnVehicleMod(playerid, vehicleid, componentid) //汽车改装
{
    new vid = vehicleid;
    if(CarInfo[vid][Moded] == 0) {
        if(CarInfo[vid][Mod1] == 0) {
            CarInfo[vid][Mod1] = componentid;
            return 1;
        }
        if(CarInfo[vid][Mod2] == 0) {
            CarInfo[vid][Mod2] = componentid;
            return 1;
        }
        if(CarInfo[vid][Mod3] == 0) {
            CarInfo[vid][Mod3] = componentid;
            return 1;
        }
        if(CarInfo[vid][Mod4] == 0) {
            CarInfo[vid][Mod4] = componentid;
            return 1;
        }
        if(CarInfo[vid][Mod5] == 0) {
            CarInfo[vid][Mod5] = componentid;
            return 1;
        }
        if(CarInfo[vid][Mod6] == 0) {
            CarInfo[vid][Mod6] = componentid;
            return 1;
        }
        if(CarInfo[vid][Mod7] == 0) {
            CarInfo[vid][Mod7] = componentid;
            return 1;
        }
        if(CarInfo[vid][Mod8] == 0) {
            CarInfo[vid][Mod8] = componentid;
            return 1;
        }
        if(CarInfo[vid][Mod9] == 0) {
            CarInfo[vid][Mod9] = componentid;
            return 1;
        }
        if(CarInfo[vid][Mod10] == 0) {
            CarInfo[vid][Mod10] = componentid;
            return 1;
        }
    }
    SendClientMessage(playerid, 0xFF0000FF, "[爱车]:你得先关闭改装保存,否则本次汽车改装不保存!");
    return 1;
}
public OnVehiclePaintjob(playerid, vehicleid, paintjobid) {
    new vid = vehicleid;
    if(CarInfo[vid][Moded] == 0) {
        CarInfo[vid][Printjob] = paintjobid;
    }
    return 1;
}
stock GetUserName(const id) {
    new msg[128], DBResult:uf;
    format(msg, sizeof(msg), "SELECT * FROM `users` WHERE ID = '%d'", id);
    uf = db_query(UsersDB, msg);
    new count = db_num_rows(uf);
    for (new c = 0; c < count; c++) {
        db_get_field_assoc(uf, "Name", msg, sizeof(msg));
        db_next_row(uf);
    }
    db_free_result(uf);
    if(id == 0) {
        msg = "系统";
    }
    return msg;
}
// stock Cars_PlayerStateChange(const playerid, const newstate)
// {
// 	new vehicleid = GetPlayerVehicleID(playerid),str[128];
// 	if(newstate == PLAYER_STATE_DRIVER)
// 	{
// 		if(CarInfo[vehicleid][ID] != 0)
// 		{
// 			if(CarInfo[vehicleid][UsersID] == 0)
// 			{
// 				format(str,sizeof(str),"[爱车] 该爱车正在出售中,价值: %d 购买输入 /cars buy.",CarInfo[vehicleid][Value]);
// 				SCM(playerid,Color_ACColor,str);
// 			}
// 			else
// 			{
// 			    if(CarInfo[vehicleid][UsersID] == GetPVarInt(playerid,"UsersDatabaseID"))
// 			    {
// 					SCM(playerid,Color_ACColor,"[爱车] 欢迎回到你的爱车.");
// 				}
// 				else
// 				{
// 					format(str,sizeof(str),"[爱车] Model: %s(%d) Value: %d Cash by: UsersID: %d",VehicleNames[GetVehicleModel(vehicleid)-400],GetVehicleModel(vehicleid),CarInfo[vehicleid][Value],CarInfo[vehicleid][UsersID]);
// 					SCM(playerid,Color_ACColor,str);
// 					if(CarInfo[vehicleid][SellValue] != 0)
// 					{
// 						format(str,sizeof(str),"[爱车] 该载具正在出售中,价格: %d 购买输入 /cars buy",CarInfo[vehicleid][SellValue]);
// 						SCM(playerid,Color_ACColor,str);
// 					}
// 				}
// 			}
// 		}
// 		return 1;
// 	}
// 	return 0;
// }
//指令
CMD:wdac(const playerid, const cmdtext[]) {
    pViewMyCar(playerid);
    return 1;
}
CMD:ac(playerid,cmdtext[]) return cmd_cars(playerid,cmdtext);
CMD:cars(const playerid, const cmdtext[]) {
    new tmp[128], idx;
    tmp = strtok(cmdtext, idx);
    if(!strlen(tmp)) return cmd_help(playerid, "");
    if(strcmp(tmp, "buy", true) == 0) return pBuyAC(playerid);
    if(strcmp(tmp, "buyobj", true) == 0) return cmd_aczb(playerid, "");
    if(strcmp(tmp, "wode", true) == 0) return CallMyAc(playerid);
    if(strcmp(tmp, "create", true) == 0) {
        if(PlayerInfo[playerid][AdminLevel] < 4) return 0; //管理员等级4级
        new Float:x, Float:y, Float:z, Float:a;
        tmp = strtok(cmdtext, idx);
        if(!strlen(tmp)) return SCM(playerid, Color_ACColor, "[爱车] 用法: /cars create [载具ID] [价格]");
        new modelids = strval(tmp);
        if(modelids < 400 || modelids > 611) return SCM(playerid, Color_ACColor, "[爱车] 创建失败,原因:ModelID错误.");
        tmp = strtok(cmdtext, idx);
        if(!strlen(tmp)) return SCM(playerid, Color_ACColor, "[爱车] 创建失败,原因:售出价格不能为空.");
        new values = strval(tmp);
        if(values < 4000) return SCM(playerid, Color_ACColor, "[爱车] 创建失败,原因:售出价格不能少于4000.");
        if(CarCount >= MAX_VEHICLE) return SCM(playerid, Color_ACColor, "[爱车] 创建失败,原因:爱车数量已经达到上线.");
        GetPlayerFacingAngle(playerid, a);
        GetPlayerPos(playerid, x, y, z);
        AdminCreateCar(modelids, values, x, y, z, a);
        SCM(playerid, Color_ACColor, "[爱车] 创建成功.");
        return 1;
    }
    if(strcmp(tmp, "list", true) == 0) return cmd_llac(playerid, "");
    if(strcmp(tmp, "lock", true) == 0) return LockCars(playerid);
    return 1;
}
CMD:aczb(const playerid, const cmdtext[]) {
    new nm[1024], str[1024];
    for (new i = 0; i < sizeof(buycarzbs); i++) {
        format(str, 128, "名称:[%s]价格:[%d]\n", buycarzbs[i][zbname], buycarzbs[i][zbmoney]);
        strcat(nm, str);
    }
    Dialog_Show(playerid, AC_DressupList, DIALOG_STYLE_LIST, "爱车装扮列表", nm, "购买", "关闭");
    return 1;
}
CMD:llac(const playerid, const cmdtext[]) {
    pACPage[playerid] = 1;
    pViewACList(playerid, 1);
    return 1;
}
stock Cars_OnPlayerConnect(const playerid) {
    EditSpeed[playerid] = 0.1; //爱车默认调速为0.1
    pChangeCarColor[playerid] = 1; //默认玩家爱车调的颜色值为1
}
stock AdminDeleteCar(const i){ //请确保这个ID是正确的再调用本方法 不然裂开 2020.3.5写 By YuCarl77
	new msg[256];
	mysql_format(g_Sql, msg,sizeof(msg), "DELETE FROM `cars` WHERE `ID` = '%d'",CarInfo[i][ID]);
	mysql_pquery(g_Sql, msg);
	DestroyDynamic3DTextLabel(CarInfo[i][TagText]); // 删除3D文字
	DestroyVehicle(CarInfo[i][GotoID]); //删除车

	// 重置所有属性
	CarInfo[i][ID] = 0;
	CarInfo[i][UsersID] = 0;
	CarInfo[i][ModelID] = 0;
	CarInfo[i][Color1] = 0;
	CarInfo[i][Color2] = 0;
	CarInfo[i][Lock] = 0;
	CarInfo[i][CarX] = 0;
	CarInfo[i][CarY] = 0;
	CarInfo[i][CarZ] = 0;
	CarInfo[i][CarA] = 0;
	CarInfo[i][Value] = 0;
	CarInfo[i][GotoID] = 0;
	CarInfo[i][BanGoto] = 0;
	CarInfo[i][SellValue] = 0;
	format(CarInfo[i][Text], Text_Strlen, " ");
	for (new v = 0; v < 15; v++) {
		if(VehAttachedObject[i][v][ModelID] != 0) {
			DestroyDynamicObject(VehAttachedObject[i][v][VaObjectID]);
		}
	} //删除爱车里的装扮
	CarCount--; //车总数减1
	return 1;
}

stock LockCars(const playerid) {
    new engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(CarInfo[pACEdit[playerid]][GotoID], engine, lights, alarm, doors, bonnet, boot, objective);
    if(CarInfo[pACEdit[playerid]][Lock] == 0)
    {
        CarInfo[pACEdit[playerid]][Lock] = 1;
        SCM(playerid,Color_ACColor,"[爱车] 车门状态已{FF0000}关闭");
    }
    else
    {
        CarInfo[pACEdit[playerid]][Lock] = 0;
        SCM(playerid,Color_ACColor,"[爱车] 车门状态已{00FF00}开启");
    }
    SetVehicleParamsEx(CarInfo[pACEdit[playerid]][GotoID], engine, lights, alarm, CarInfo[pACEdit[playerid]][Lock], bonnet, boot, objective);
    UpdataACarData(pACEdit[playerid]);
    return 1;
}

Dialog:AC_List(playerid, response, listitem, inputtext[]) {
    if(!response) return 1;
    new idx, tmp[128];
    tmp = strtok(inputtext, idx);
    if(strcmp(tmp, "上一页") == 0) {
        if(pACPage[playerid] == 1) return SCM(playerid, Color_ACColor, "[爱车] 翻页失败,原因:这是第一页.");
        pACPage[playerid]--;
        pViewACList(playerid, pACPage[playerid]);
        return 1;
    }
    if(strcmp(tmp, "下一页") == 0) {
        if(pACPage[playerid] == GetMaxPage(CarCount, 15)) return SCM(playerid, Color_ACColor, "[爱车] 翻页失败,原因:这是最后一页.");
        pACPage[playerid]++;
        pViewACList(playerid, pACPage[playerid]);
        return 1;
    }
    if(strcmp(tmp, "传送", true) == 0) {
        tmp = strtok(inputtext, idx);
        new id = strval(tmp);
        if(CarInfo[id][SellValue] == 0) {
            if(CarInfo[id][BanGoto] == 1) return SCM(playerid, Color_ACColor, "[爱车] 传送失败,原因:该爱车主人禁止传送.");
        }
        new Float:x, Float:y, Float:z;
        GetVehiclePos(id, x, y, z);
        SetPlayerPos(playerid, x, y, z + 2);
        SCM(playerid, Color_ACColor, "[爱车] 你传送到了这辆爱车.");
        return 1;
    }
    return 1;
}
Dialog:AC_MyCar(playerid, response, listitem, inputtext[]) {
    if(!response) return 1;
    if(listitem == 0) {
        LockCars(playerid);
        return 1;
    }
    if(listitem == 1) {
        if(GetPlayerVehicleSeat(playerid) != 0) return SCM(playerid, Color_ACColor, "[爱车] 更改失败,原因:你没在主驾驶座位.");
        if(CarInfo[GetPlayerVehicleID(playerid)][UsersID] != PlayerInfo[playerid][ID]) return SCM(playerid, Color_ACColor, "[爱车] 更改失败,原因:这不是你的爱车.");
        new Float:x, Float:y, Float:z, Float:a;
        GetVehicleZAngle(GetPlayerVehicleID(playerid), a);
        GetVehiclePos(GetPlayerVehicleID(playerid), x, y, z);
        CarInfo[pACEdit[playerid]][CarX] = x, CarInfo[pACEdit[playerid]][CarY] = y, CarInfo[pACEdit[playerid]][CarZ] = z, CarInfo[pACEdit[playerid]][CarA] = a;
        SCM(playerid, Color_ACColor, "[爱车] 已更新爱车坐标.");
        UpdataACarData(pACEdit[playerid]);
        return 1;
    }
    if(listitem == 2) {
        if(CarInfo[pACEdit[playerid]][BanGoto] == 0) {
            CarInfo[pACEdit[playerid]][BanGoto] = 1;
            SCM(playerid, Color_ACColor, "[爱车] 游览传送已{FF0000}关闭");
        } else {
            CarInfo[pACEdit[playerid]][BanGoto] = 0;
            SCM(playerid, Color_ACColor, "[爱车] 游览传送已{00FF00}开启");
        }
        UpdataACarData(pACEdit[playerid]);
        return 1;
    }
    if(listitem == 3) {
        new str[1024];
        format(str, sizeof(str), "{FFFFFF}载具名称\t%s(%d)\n载具颜色\t{%06x}▇ {%06x}▇\n{FFFFFF}载具门锁\t%s\n游览传送\t%s\n载具文字\t%s\n",
            VehicleNames[GetVehicleModel(CarInfo[pACEdit[playerid]][GotoID]) - 400], GetVehicleModel(CarInfo[pACEdit[playerid]][GotoID]), VehicleColoursTableRGBA[CarInfo[pACEdit[playerid]][Color1]] >>> 8,
            VehicleColoursTableRGBA[CarInfo[pACEdit[playerid]][Color2]] >>> 8, VehicleLockDoor[CarInfo[pACEdit[playerid]][Lock]], ListGotoNickName[CarInfo[pACEdit[playerid]][BanGoto]],
            CarInfo[pACEdit[playerid]][Text]);
        Dialog_Show(playerid, AC_Dialog, DIALOG_STYLE_LIST, "我的爱车", str, "确定", "关闭");
        return 1;
    }
    if(listitem == 4) {
        new msg[256];
        format(msg, sizeof(msg), "{FFFFFF}原文字 >>> %s\n{FFFFFF}请输入新文字,该操作将花费 %d 金钱", CarInfo[pACEdit[playerid]][Text], CarInfo[pACEdit[playerid]][Text], ChangeCarTextScore);
        Dialog_Show(playerid, AC_Text, DIALOG_STYLE_INPUT, "我的爱车", msg, "确定", "关闭");
        return 1;
    }
    if(listitem == 5) {
        pACPage[playerid] = 1;
        ChangeCarColor(playerid, 1);
        return 1;
    }
    if(listitem == 6) {
        new msg[256];
        format(msg, sizeof(msg), "请输入你想踢出的乘客ID.");
        Dialog_Show(playerid, AC_KickPassengers, DIALOG_STYLE_INPUT, "我的爱车", msg, "确定", "关闭");
        return 1;
    }
    if(listitem == 7) {
        CallMyAc(playerid);
        return 1;
    }
    if(listitem == 8) {
        new msg[256];
        format(msg, sizeof(msg), "请输入载具ID\t该操作将消费 %d 金钱", ChangeCarsScore);
        Dialog_Show(playerid, AC_ChangeCars, DIALOG_STYLE_INPUT, "我的爱车", msg, "确定", "关闭");
        return 1;
    }
    if(listitem == 9) {
        SCM(playerid, 0xFF0000FF, "该功能暂时关闭了");
        //ShowPlayerDialog(playerid,AC_GiveCars,DIALOG_STYLE_INPUT,"我的爱车","请输入对方ID.","确定","关闭");
        return 1;
    }
    if(listitem == 10) {
        new string[64], d;
        new msg[256];
        PlayerEdit[playerid][0] = pACEdit[playerid];
        for (new i; i < 15; i++) {
            if(VehAttachedObject[PlayerEdit[playerid][0]][i][ModelID] != 0) {
                format(string, 64, "槽位:%d\t模型ID:%d\n", i, VehAttachedObject[PlayerEdit[playerid][0]][i][ModelID]);
                strcat(msg, string);
                VehAttachedObjectList[PlayerEdit[playerid][0]][d] = i;
                d++;
            }
        }
        if(strlen(msg) == 0) {
            SCM(playerid, Color_ACColor, "[爱车物件] 该载具没有物件.");
        } else {
            Dialog_Show(playerid, CDIALOG_CarZBParcel, DIALOG_STYLE_LIST, "包裹(槽位数:15)", msg, "选择", "退出");
        }
        return 1;
    }
    if(listitem == 11) {
        Dialog_Show(playerid, AC_Sell, DIALOG_STYLE_LIST, "我的爱车", "卖给玩家\n卖给系统({FF0000}只补还爱车的价格)", "确定", "取消");
        return 1;
    }
    if(listitem == 12) {
        if(CarInfo[pACEdit[playerid]][Moded] == 0) {
            if(PlayerInfo[playerid][Cash] < 500) {
                SCM(playerid, Color_ACColor, "[爱车]保存改装需要500金币");
                return 1;
            }
            GivePlayerCash(playerid, -500);
            CarInfo[pACEdit[playerid]][Moded] = 1;
            SCM(playerid, Color_ACColor, "[爱车]保存改装已{FF0000}开启");
            UpdataACarData(pACEdit[playerid]);
            return 1;
        }
        if(CarInfo[pACEdit[playerid]][Moded] == 1) {
            CarInfo[pACEdit[playerid]][Moded] = 0;
            SCM(playerid, Color_ACColor, "[爱车]保存改装已{00FF00}关闭");
            UpdataACarData(pACEdit[playerid]);
            return 1;
        }
        return 1;
    }
    return 1;
}


Dialog:AC_BUY(playerid, response, listitem, inputtext[]) {
    if(!response) {
        new Float:x, Float:y, Float:z;
        new vid = GetPlayerVehicleID(playerid);
        if(CarInfo[vid][ID]) {
            GetPlayerPos(playerid, x, y, z);
            SetPlayerPos(playerid, x, y, z + 2);
            SetVehiclePos(vid, CarInfo[vid][CarX], CarInfo[vid][CarY], CarInfo[vid][CarZ]);
        }
        return 1;
        //丢出车外
    }
    cmd_cars(playerid, "buy");
    return 1;
}
Dialog:AC_Sell(playerid, response, listitem, inputtext[]) {
    if(response) {
        if(listitem == 0) {
            // 卖给玩家
            Dialog_Show(playerid, AC_Sell_Twice, DIALOG_STYLE_INPUT, "我的爱车", "价格为 0 = 不出售\n请输入价格", "确定", "退出");
            return 1;
        } else {
            // 卖给系统
            // 从数据库中删除
            GivePlayerCash(playerid, CarInfo[pACEdit[playerid]][Value]); //删除数据库前先给玩家MONEY
            AdminDeleteCar(pACEdit[playerid]); //2020-3-5 00:31:49测试 如果-1还是错 再看吧
            pACEdit[playerid] = -1; // 取消玩家编辑状态
            SendClientMessage(playerid, Color_ACColor, "[爱车]回收给系统成功!");
            return 1;
        }
    }
    return 1;
}
Dialog:AC_Sell_Twice(playerid, response, listitem, inputtext[]) {
    if(!response) return 1;
    new value = strval(inputtext);
    if(value < CarInfo[pACEdit[playerid]][Value] - 500 || value < 2000) {
        SCM(playerid, Color_ACColor, "[爱车] 操作失败,原因:价格不能低于2000且不能低于买入价-500.");
        pACEdit[playerid] = -1;
        return 1;
    }
    if(value == 0) {
        pACEdit[playerid] = -1;
        return 1;
    }
    // 2020.3.4写 注销这些 感觉冗余了 没必要写这段判断了
    // if(value == 0)
    // {
    //     new msg[256];
    // 	format(msg,sizeof(msg),"UPDATE `cars` SET SellValue = %d WHERE ID = %d",0,CarInfo[pACEdit[playerid]][ID]);
    // 	db_free_result(db_query(Cardb,msg));
    // 	format(CarInfo[pACEdit[playerid]][Text],Text_Strlen,"%s的爱车",GetName(playerid));
    // 	Delete3DTextLabel(CarInfo[pACEdit[playerid]][TagText]);
    // 	CarInfo[pACEdit[playerid]][TagText] = Create3DTextLabel(CarInfo[pACEdit[playerid]][Text],Color_ACColor,0.0,0.0,0.0,20.0,0);
    // 	Attach3DTextLabelToVehicle(CarInfo[pACEdit[playerid]][TagText],GetPlayerVehicleID(playerid),0.0,0.0,0.0);
    // 	UpdataACarData(pACEdit[playerid]);
    // 	CarInfo[pACEdit[playerid]][SellValue] = 0;
    // 	pACEdit[playerid] = -1;
    // 	return 1;
    // }
    // else
    // {
    // }
    new msg[256];
    mysql_format(g_Sql, msg, sizeof(msg), "UPDATE `cars` SET `SellValue` = %d WHERE `ID` = %d", value, CarInfo[pACEdit[playerid]][ID]);
    mysql_pquery(g_Sql, msg);
    format(CarInfo[pACEdit[playerid]][Text], Text_Strlen, "出售中\n价格:%d\n拥有者:%s", value, GetName(playerid));
    DestroyDynamic3DTextLabel(CarInfo[pACEdit[playerid]][TagText]);
    CarInfo[pACEdit[playerid]][TagText] = CreateDynamic3DTextLabel(CarInfo[pACEdit[playerid]][Text], Color_ACColor, 0.0, 0.0, 0.0, 20.0, INVALID_PLAYER_ID, GetPlayerVehicleID(playerid));
    // Attach3DTextLabelToVehicle(CarInfo[pACEdit[playerid]][TagText],GetPlayerVehicleID(playerid),0.0,0.0,0.0);
    UpdataACarData(pACEdit[playerid]);
    CarInfo[pACEdit[playerid]][SellValue] = value;
    pACEdit[playerid] = -1;
    return 1;
}
Dialog:AC_GiveCars(playerid, response, listitem, inputtext[]) {
    if(!response) return 1;
    if(CarInfo[pACEdit[playerid]][SellValue] != 0) return SCM(playerid, Color_ACColor, "[爱车] 操作失败,原因:爱车正在出售中.");
    new id = strval(inputtext);
    // if(IsPlayerConnected(id) == 0) return SCM(playerid,Color_ACColor,"[爱车] 操作失败,原因:错误对方ID.");
    if(!PlayerInfo[playerid][Login]) return SCM(playerid, Color_ACColor, "[爱车] 操作失败,原因:错误对方ID或对方未登录.");
    if(GetPlayerVehicleID(id) != CarInfo[pACEdit[playerid]][GotoID]) return SCM(playerid, Color_ACColor, "[爱车] 操作失败,原因:对方没在爱车上.");
    if(id == playerid) return SCM(playerid, Color_ACColor, "[爱车] 操作失败,原因:你不能给予你自己.");
    Search_MyCar_of_CarCont(id);
    if(pACEdit[id] != -1) return SCM(playerid, Color_ACColor, "[爱车] 操作失败,原因:对方已经有一部爱车了.");
    new msg[256];
    format(msg, sizeof(msg), "[爱车] %s 把一辆爱车 [%s(%d)] 给了你", GetName(playerid), VehicleNames[GetVehicleModel(CarInfo[pACEdit[playerid]][GotoID]) - 400], GetVehicleModel(CarInfo[pACEdit[playerid]][GotoID]));
    SCM(id, Color_ACColor, msg);
    format(msg, sizeof(msg), "[爱车] 你把这部爱车给了 %s(%d)", GetName(id), id);
    SCM(playerid, Color_ACColor, msg);
    CarInfo[pACEdit[playerid]][UsersID] = PlayerInfo[id][ID];
    UpdataACarData(pACEdit[playerid]);
    pACEdit[id] = pACEdit[playerid];
    pACEdit[playerid] = -1;
    return 1;
}
Dialog:AC_ChangeCars(playerid, response, listitem, inputtext[]) {
    if(!response) return 1;
    if(CarInfo[pACEdit[playerid]][SellValue] != 0) return SCM(playerid, Color_ACColor, "[爱车] 操作失败,原因:爱车正在出售中.");
    if(PlayerInfo[playerid][Cash] < ChangeCarsScore) {
        new msg[256];
        format(msg, sizeof(msg), "[爱车] 更换失败,原因:你的金钱不足.");
        SCM(playerid, Color_ACColor, msg);
        return 1;
    }
    new id = strval(inputtext);
    if(id < 400 || id > 611) return SCM(playerid, Color_ACColor, "[System] 错误.");
    new msg[256];
    mysql_format(g_Sql, msg, sizeof(msg), "UPDATE `cars` SET `ModelID` = %d WHERE `ID` = %d", id, CarInfo[pACEdit[playerid]][ID]);
    mysql_query(g_Sql, msg);
    format(msg, sizeof(msg), "[爱车] 更换成功.");
    SCM(playerid, Color_ACColor, msg);
    DestroyVehicle(CarInfo[pACEdit[playerid]][GotoID]);
    AddStaticVehicleEx(id, CarInfo[pACEdit[playerid]][CarX], CarInfo[pACEdit[playerid]][CarY], CarInfo[pACEdit[playerid]][CarZ],
        CarInfo[pACEdit[playerid]][CarA], CarInfo[pACEdit[playerid]][Color1], CarInfo[pACEdit[playerid]][Color2], -1);
    CarInfo[pACEdit[playerid]][ModelID] = id;
    GivePlayerCash(playerid, -ChangeCarsScore);
    return 1;
}
Dialog:AC_KickPassengers(playerid, response, listitem, inputtext[]) {
    if(!response) return 1;
    new id = strval(inputtext);
    if(IsPlayerConnected(id) == 0) return SCM(playerid, Color_ACColor, "[爱车] 踢出失败,原因:错误的ID.");
    if(GetPlayerVehicleID(id) != CarInfo[pACEdit[playerid]][GotoID]) return SCM(playerid, Color_ACColor, "[爱车] 踢出失败,原因:该玩家没有在你的爱车中.");
    new Float:x, Float:y, Float:z;
    GetPlayerPos(id, x, y, z);
    SetPlayerPos(id, x, y, z + 3);
    new msg[128];
    format(msg, sizeof(msg), "[爱车] 你把 %s 踢出了你的爱车.", GetName(id));
    SCM(playerid, Color_ACColor, msg);
    format(msg, sizeof(msg), "[爱车] %s 把你踢出了载具.", GetName(playerid));
    SCM(playerid, Color_ACColor, msg);
    return 1;
}
Dialog:AC_Color(playerid, response, listitem, inputtext[]) {
    if(!response) return 1;
    if(CarInfo[pACEdit[playerid]][SellValue] != 0) return SCM(playerid, Color_ACColor, "[爱车] 操作失败,原因:爱车正在出售中.");
    new idx, tmp[128];
    tmp = strtok(inputtext, idx);
    if(strcmp(tmp, "请选择颜色") == 0) {
        if(pChangeCarColor[playerid] == 1) pChangeCarColor[playerid] = 2;
        else pChangeCarColor[playerid] = 1;
        ChangeCarColor(playerid, pACPage[playerid]);
        return 1;
    }
    if(strcmp(tmp, "上一页") == 0) {
        if(pACPage[playerid] == 1) return SCM(playerid, Color_ACColor, "[爱车] 翻页失败,原因:这是第一页.");
        pACPage[playerid]--;
        ChangeCarColor(playerid, pACPage[playerid]);
        return 1;
    }
    if(strcmp(tmp, "下一页") == 0) {
        if(pACPage[playerid] == GetMaxPage(sizeof(VehicleColoursTableRGBA)) - 1) return SCM(playerid, Color_ACColor, "[爱车] 翻页失败,原因:这是最后一页.");
        pACPage[playerid]++;
        ChangeCarColor(playerid, pACPage[playerid]);
        return 1;
    }
    if(strcmp(tmp, "ColorID", true) == 0) {
        tmp = strtok(inputtext, idx);
        new id = strval(tmp);
        if(pChangeCarColor[playerid] == 1) {
            CarInfo[pACEdit[playerid]][Color1] = id;
            ChangeVehicleColor(pACEdit[playerid], id, CarInfo[pACEdit[playerid]][Color2]);
        } else {
            CarInfo[pACEdit[playerid]][Color2] = id;
            ChangeVehicleColor(pACEdit[playerid], CarInfo[pACEdit[playerid]][Color1], id);
        }
        ChangeCarColor(playerid, pACPage[playerid]);
        UpdataACarData(pACEdit[playerid]);
    }
    return 1;
}
Dialog:AC_Text(playerid, response, listitem, inputtext[]) {
    if(!response) return 1;
    if(CarInfo[pACEdit[playerid]][SellValue] != 0) return SCM(playerid, Color_ACColor, "[爱车] 操作失败,原因:爱车正在出售中.");
    if(GetPlayerVehicleSeat(playerid) != 0) return SCM(playerid, Color_ACColor, "[爱车] 更改失败,原因:你没在主驾驶座位.");
    if(CarInfo[GetPlayerVehicleID(playerid)][UsersID] != PlayerInfo[playerid][ID]) return SCM(playerid, Color_ACColor, "[爱车] 更改失败,原因:这不是你的爱车.");
    if(PlayerInfo[playerid][Cash] < ChangeCarTextScore) {
        new str[256];
        format(str, sizeof(str), "[爱车] 更换失败,原因:你的金钱不足.");
        SCM(playerid, Color_ACColor, str);
        return 1;
    }
    if(strlen(inputtext) > Text_Strlen - 10) return SCM(playerid, Color_ACColor, "[爱车] 修改失败,原因:字符串过长.");
    new placeholder;
    for (new i = 0; i < sizeof InvalidWords; i++) //屏蔽词自动变*
    {
        placeholder = strfind(inputtext, InvalidWords[i], true);
        if(placeholder != -1) {
            for (new x = placeholder; x < placeholder + strlen(InvalidWords[i]); x++) {
                inputtext[x] = '*';
            }
        }
    }
    format(CarInfo[pACEdit[playerid]][Text], Text_Strlen, "%s", inputtext);
    GivePlayerCash(playerid, -ChangeCarTextScore);
    SCM(playerid, Color_ACColor, "[爱车] 载具文字修改成功!");
    DestroyDynamic3DTextLabel(CarInfo[pACEdit[playerid]][TagText]);
    CarInfo[pACEdit[playerid]][TagText] = CreateDynamic3DTextLabel(CarInfo[pACEdit[playerid]][Text], Color_ACColor, 0.0, 0.0, 0.0, 20.0, INVALID_PLAYER_ID, GetPlayerVehicleID(playerid));
    // Attach3DTextLabelToVehicle(CarInfo[pACEdit[playerid]][TagText],GetPlayerVehicleID(playerid),0.0,0.0,0.0);
    UpdataACarData(pACEdit[playerid]);
    return 1;
}



Dialog:CDIALOG_CarZBEditSave(playerid, response, listitem, inputtext[]) {
    if(!response) return 1;
    new vid = GetPlayerVehicleID(playerid);
    if(listitem == 6) {
        if(!IsPlayerInAnyVehicle(playerid)) return SCM(playerid, Color_ACColor, "[爱车物件] 你不在载具上.");
        if(CarInfo[vid][UsersID] != PlayerInfo[playerid][ID]) return SCM(playerid, Color_ACColor, "[爱车物件] 这不是你的载具.");
        new tmp[3];
        tmp[0] = PlayerEdit[playerid][0];
        tmp[1] = PlayerEdit[playerid][1];
        tmp[2] = PlayerEdit[playerid][2];
        VehAttachedObject[tmp[0]][tmp[1]][VaX] = 0.0;
        VehAttachedObject[tmp[0]][tmp[1]][VaY] = 0.0;
        VehAttachedObject[tmp[0]][tmp[1]][VaZ] = 0.0;
        VehAttachedObject[tmp[0]][tmp[1]][VaRX] = 0.0;
        VehAttachedObject[tmp[0]][tmp[1]][VaRY] = 0.0;
        VehAttachedObject[tmp[0]][tmp[1]][VaRZ] = 0.0;
        new Query[512], String[512];
        strins(String, "UPDATE `va` SET `Model` = %d, `X` = '%f', `Y` = '%f', `Z` = '%f', `RX` = '%f'", strlen(String));
        strins(String, ", RY = '%f', RZ = '%f' WHERE VehicleID = %d AND Slot = %d", strlen(String));
        mysql_format(g_Sql, Query, sizeof(Query), String, VehAttachedObject[tmp[0]][tmp[1]][ModelID], VehAttachedObject[tmp[0]][tmp[1]][VaX],\
        VehAttachedObject[tmp[0]][tmp[1]][VaY], VehAttachedObject[tmp[0]][tmp[1]][VaZ], VehAttachedObject[tmp[0]][tmp[1]][VaRX],\
        VehAttachedObject[tmp[0]][tmp[1]][VaRY], VehAttachedObject[tmp[0]][tmp[1]][VaRZ], CarInfo[vid][ID], tmp[1]);
        mysql_pquery(g_Sql, Query);
        new str[128];
        format(str, sizeof(str), "左右\n前后\n上下\n前翻\n侧翻\n旋转\n调速\n{FF0000}删除\n状态:%s", TagObjectsState[VehAttachedObject[tmp[0]][tmp[1]][TagObjects]]);
        Dialog_Show(playerid, CDIALOG_CarZB, DIALOG_STYLE_LIST, "物件编辑", str, "选择", "退出");
        return 1;
    }
    if(listitem == 8) {
        if(!IsPlayerInAnyVehicle(playerid)) return SCM(playerid, Color_ACColor, "[爱车物件] 你不在载具上.");
        if(CarInfo[vid][UsersID] != PlayerInfo[playerid][ID]) return SCM(playerid, Color_ACColor, "[爱车物件] 这不是你的载具.");
        new tmp[3];
        tmp[0] = PlayerEdit[playerid][0];
        tmp[1] = PlayerEdit[playerid][1];
        tmp[2] = PlayerEdit[playerid][2];
        VehAttachedObject[tmp[0]][tmp[1]][VaX] = VehAttachedObject[tmp[0]][tmp[1]][VaX];
        VehAttachedObject[tmp[0]][tmp[1]][VaY] = VehAttachedObject[tmp[0]][tmp[1]][VaY];
        VehAttachedObject[tmp[0]][tmp[1]][VaZ] = VehAttachedObject[tmp[0]][tmp[1]][VaZ];
        VehAttachedObject[tmp[0]][tmp[1]][VaRX] = VehAttachedObject[tmp[0]][tmp[1]][VaRX];
        VehAttachedObject[tmp[0]][tmp[1]][VaRY] = VehAttachedObject[tmp[0]][tmp[1]][VaRY];
        VehAttachedObject[tmp[0]][tmp[1]][VaRZ] = VehAttachedObject[tmp[0]][tmp[1]][VaRZ];
        new Query[512], String[512];
        strins(String, "UPDATE `va` SET `Model` = %d, `X` = '%f', `Y` = '%f', `Z` = '%f', `RX` = '%f', `RY` = '%f', `RZ` = '%f' ", strlen(String));
        strins(String, "WHERE VehicleID = %d AND Slot = %d", strlen(String));
        mysql_format(g_Sql, Query, sizeof(Query), String, VehAttachedObject[tmp[0]][tmp[1]][ModelID], VehAttachedObject[tmp[0]][tmp[1]][VaX],\
        VehAttachedObject[tmp[0]][tmp[1]][VaY], VehAttachedObject[tmp[0]][tmp[1]][VaZ], VehAttachedObject[tmp[0]][tmp[1]][VaRX],\
        VehAttachedObject[tmp[0]][tmp[1]][VaRY], VehAttachedObject[tmp[0]][tmp[1]][VaRZ], CarInfo[vid][ID], tmp[1]);
        mysql_pquery(g_Sql, Query);
        PlayerEdit[playerid][0] = 0, PlayerEdit[playerid][1] = 0, PlayerEdit[playerid][2] = 0;
        return 1;
    }
    return 1;
}
Dialog:CDIALOG_CarZB(playerid, response, listitem, inputtext[]) {
    if(!response) {
        PlayerEdit[playerid][0] = 0, PlayerEdit[playerid][1] = 0, PlayerEdit[playerid][2] = 0;
        return 1;
    }
    new vid = GetPlayerVehicleID(playerid);
    if(listitem == 6) {
        if(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][TagObjects] == 0) {
            SCM(playerid, Color_ACColor, "[爱车物件] 操作失败,原因:该物件未装扮.");
            PlayerEdit[playerid][0] = 0, PlayerEdit[playerid][1] = 0, PlayerEdit[playerid][2] = 0;
            return 1;
        }
        new str[128];
        format(str, sizeof(str), "请输入你要的调整速度(当前速度:%0.1f)", EditSpeed[playerid]);
        Dialog_Show(playerid, CDIALOG_CarZBSpeed, DIALOG_STYLE_INPUT, "调速", str, "确定", "关闭");
        return 1;
    }
    if(listitem == 7) {
        if(IsValidDynamicObject(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaObjectID])) {
            DestroyDynamicObject(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaObjectID]);
        }
        VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][ModelID] = 0;
        new Query[256];
        mysql_format(g_Sql, Query, sizeof(Query), "DELETE FROM `va` WHERE `VehicleID` = %d AND `Slot` = %d", CarInfo[vid][ID], PlayerEdit[playerid][1]);
        mysql_pquery(g_Sql, Query);
        SCM(playerid, Color_ACColor, "[爱车物件] 你删除了该装扮.");
        return 1;
    }
    if(listitem == 8) {
        new Query[512];
        if(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][TagObjects] == 1) {
            VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][TagObjects] = 0;
            DestroyDynamicObject(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaObjectID]);
        } else {
            VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][TagObjects] = 1;
            VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaObjectID] = CreateDynamicObject(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][ModelID], 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
            AttachDynamicObjectToVehicle(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaObjectID], PlayerEdit[playerid][0],
                VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaX],
                VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaY],
                VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaZ],
                VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRX],
                VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRY],
                VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRZ]);
        }
        mysql_format(g_Sql, Query, sizeof(Query), "UPDATE `va` SET `TagObjects` = %d WHERE `VehicleID` = %d AND `Slot` = %d", VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][TagObjects], CarInfo[vid][ID], PlayerEdit[playerid][1]);
        mysql_pquery(g_Sql, Query);
        new str[128];
        format(str, sizeof(str), "左右\n前后\n上下\n前翻\n侧翻\n旋转\n调速\n{FF0000}删除\n状态:%s", TagObjectsState[VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][TagObjects]]);
        Dialog_Show(playerid, CDIALOG_CarZB, DIALOG_STYLE_LIST, "物件编辑", str, "选择", "退出");
        return 1;
    } 
    if(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][TagObjects] == 0) {
        SCM(playerid, Color_ACColor, "[爱车物件] 操作失败,原因:该物件未装扮.");
        PlayerEdit[playerid][0] = 0, PlayerEdit[playerid][1] = 0, PlayerEdit[playerid][2] = 0;
        return 1;
    }
    PlayerEdit[playerid][2] = listitem + 1;
    if(IsValidDynamicObject(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaObjectID])) {
        DestroyDynamicObject(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaObjectID]);
    }
    VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaObjectID] = CreateDynamicObject(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][ModelID], 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    AttachDynamicObjectToVehicle(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaObjectID], PlayerEdit[playerid][0],
        VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaX],
        VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaY],
        VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaZ],
        VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRX],
        VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRY],
        VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRZ]);
    SCM(playerid, Color_ACColor, "[爱车物件] 使用小键盘'4/6'可进行操作,输入'2'可弹出操作框,点击鼠标左键可进行保存.");
    Streamer_UpdateAll();
    return 1;
}
Dialog:CDIALOG_CarZBSpeed(playerid, response, listitem, inputtext[]) {
    if(!response) return 1;
    EditSpeed[playerid] = floatstr(inputtext);
    new str[128];
    format(str, sizeof(str), "左右\n前后\n上下\n前翻\n侧翻\n旋转\n调速\n{FF0000}删除\n状态:%s", TagObjectsState[VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][TagObjects]]);
    Dialog_Show(playerid, CDIALOG_CarZB, DIALOG_STYLE_LIST, "物件编辑", str, "选择", "退出");
    return 1;
}
Dialog:CDIALOG_CarZBParcel(playerid, response, listitem, inputtext[]) {
    if(!response) return 1;
    PlayerEdit[playerid][1] = VehAttachedObjectList[PlayerEdit[playerid][0]][listitem];
    new str[128];
    format(str, sizeof(str), "左右\n前后\n上下\n前翻\n侧翻\n旋转\n调速\n{FF0000}删除\n状态:%s", TagObjectsState[VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][TagObjects]]);
    Dialog_Show(playerid, CDIALOG_CarZB, DIALOG_STYLE_LIST, "物件编辑", str, "选择", "退出");
    return 1;
}
Dialog:AC_DressupList(playerid, response, listitem, inputtext[]) {
    if(response) {
        new vid = GetPlayerVehicleID(playerid);
        new msgs[128], Query[512];
        if(vid == 0) return SCM(playerid, Color_ACColor, "[载具] 你没有在载具上!");
        if(CarInfo[vid][UsersID] != PlayerInfo[playerid][ID]) return SCM(playerid, Color_ACColor, "[爱车物件] 这不是你的载具.");
        if(PlayerInfo[playerid][Cash] < buycarzbs[listitem][zbmoney]) {
            format(msgs, 128, "[载具] 你的金币不足 %d 不可以购买 %s", buycarzbs[listitem][zbmoney], buycarzbs[listitem][zbname]);
            SCM(playerid, -1, msgs);
            return 1;
        }
        if(buycarzbs[listitem][zbid] != 0) {
            for (new i; i < 15; i++) {
                if(VehAttachedObject[vid][i][ModelID] == 0) {
                    VehAttachedObject[vid][i][ModelID] = buycarzbs[listitem][zbid];
                    mysql_format(g_Sql, Query, sizeof(Query), "INSERT INTO `va` (VehicleID, Slot, Model, X, Y, Z, RX, RY, RZ) VALUES (%d, %d, %d, '%f', '%f', '%f', '%f', '%f', '%f')",\
                    CarInfo[vid][ID], i, VehAttachedObject[vid][i][ModelID], 0, 0, 0, 0, 0, 0);
                    mysql_pquery(g_Sql, Query);
                    GivePlayerCash(playerid, -buycarzbs[listitem][zbmoney]);
                    format(msgs, 128, "[载具] 你购买了 %s 花费了 %d 金币", buycarzbs[listitem][zbname], buycarzbs[listitem][zbmoney]);
                    SCM(playerid, Color_ACColor, msgs);
                    return 1;
                }
            }
            SCM(playerid, Color_ACColor, "[载具] 该载具包裹已满!");
        }
        return 1;
    }
    return 1;
}

function UpdateOfflineCarsSale(vid, playerid) {
    new cash;
    cache_get_value_name_int(0, "Cash", cash);
    new query[128];
    mysql_format(g_Sql, query, sizeof(query), "UPDATE `users` SET `Cash` = '%d' WHERE `ID` = '%d'", cash + CarInfo[vid][SellValue], CarInfo[vid][UsersID]);
    mysql_pquery(g_Sql, query);

    printf("[爱车]由于UID为%d的玩家不在线,但爱车出售了,由系统自带接管数据库金币", CarInfo[vid][UsersID]);
    GivePlayerCash(playerid, -CarInfo[vid][SellValue]); //买家给钱
    DestroyDynamic3DTextLabel(CarInfo[vid][TagText]);
    format(CarInfo[vid][Text], Text_Strlen, "%s", GetName(playerid));
    CarInfo[vid][TagText] = CreateDynamic3DTextLabel(CarInfo[vid][Text], Color_ACColor, 0.0, 0.0, 0.0, 20.0, INVALID_PLAYER_ID,vid);
    // Attach3DTextLabelToVehicle(CarInfo[vid][TagText], vid, 0.0, 0.0, 0.0);
    mysql_format(g_Sql, query, sizeof(query), "UPDATE `cars` SET `SellValue` = '%d' WHERE `ID` = '%d'", 0, CarInfo[vid][UsersID]);
    mysql_pquery(g_Sql, query);
    CarInfo[vid][UsersID] = PlayerInfo[playerid][ID];
    UpdataACarData(vid);
}
// 初始化爱车表
stock SetupCarsTable() {
    mysql_pquery(g_Sql,"CREATE TABLE IF NOT EXISTS `cars`  (\
    `ID` int(10) UNSIGNED NOT NULL AUTO_INCREMENT,\
    `UsersID` int(10) UNSIGNED NULL DEFAULT NULL,\
    `ModelID` int(11) NULL DEFAULT NULL,\
    `Color1` int(11) NULL DEFAULT NULL,\
    `Color2` int(11) NULL DEFAULT NULL,\
    `Lock` int(11) NULL DEFAULT NULL,\
    `X` varchar(12) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,\
    `Y` varchar(12) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,\
    `Z` varchar(12) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,\
    `A` varchar(12) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,\
    `Value` int(11) NULL DEFAULT NULL,\
    `Text` varchar(255) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,\
    `BanGoto` int(11) NULL DEFAULT NULL,\
    `SellValue` int(11) NULL DEFAULT 0,\
    `Mod1` int(11) NULL DEFAULT NULL,\
    `Mod2` int(11) NULL DEFAULT NULL,\
    `Mod3` int(11) NULL DEFAULT NULL,\
    `Mod4` int(11) NULL DEFAULT NULL,\
    `Mod5` int(11) NULL DEFAULT NULL,\
    `Mod6` int(11) NULL DEFAULT NULL,\
    `Mod7` int(11) NULL DEFAULT NULL,\
    `Mod8` int(11) NULL DEFAULT NULL,\
    `Mod9` int(11) NULL DEFAULT NULL,\
    `Mod10` int(11) NULL DEFAULT NULL,\
    `Moded` int(11) NULL DEFAULT NULL,\
    `Printjob` int(11) NULL DEFAULT NULL,\
    PRIMARY KEY (`ID`) USING BTREE\
    ) ENGINE = InnoDB CHARACTER SET = gbk COLLATE = gbk_chinese_ci ROW_FORMAT = Compact;");


// 2021.10.7 修复TagObject没有默认值的问题
    mysql_pquery(g_Sql,"CREATE TABLE IF NOT EXISTS `va`  (\
    `ID` int(10) UNSIGNED NOT NULL DEFAULT 0,\
    `VehicleID` int(11) NULL DEFAULT NULL,\
    `Slot` int(11) NULL DEFAULT NULL,\
    `Model` int(11) NULL DEFAULT NULL,\
    `X` varchar(12) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,\
    `Y` varchar(12) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,\
    `Z` varchar(12) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,\
    `RX` varchar(12) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,\
    `RY` varchar(12) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,\
    `RZ` varchar(12) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,\
    `TagObjects` int(1) NOT NULL DEFAULT 0,\
    PRIMARY KEY (`ID`) USING BTREE\
    ) ENGINE = InnoDB CHARACTER SET = gbk COLLATE = gbk_chinese_ci ROW_FORMAT = Compact;");
}
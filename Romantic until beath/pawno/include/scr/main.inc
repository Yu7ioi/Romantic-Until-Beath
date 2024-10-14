
#define GMText "Romantic v24.10.7"
#define ServerVersion "Powered by MYSQLR41.4 v2021.10.7"

// 本次测试啥的记得随时调
//----------MYSQL STUFFS----------------------
#define MYSQL_HOST	"localhost" //改成MYSQL IP地址
#define MYSQL_USER	"rud233" // MYSQL服务用户名
#define MYSQL_PASS 	"rud233"// 改成MYSQL服务密码
// #define MYSQL_USER	"root" // MYSQL服务用户名
// #define MYSQL_PASS 	"123456"// 改成MYSQL服务密码

#define MYSQL_DB	"rud"// 改成你自己的数据表
#define MYSQL_charset "gbk"//“gbk”是中文编码，如果你想使用其他语言，请修改!

//----------NODEJS MAIL STUFFS----------------------
#define SAMPMAILJS_URL "127.0.0.1:9008" 
// IP + PORT of the server running the NodeJS script
// 运行NodeJS脚本的服务器的IP +端口
#define SAMPMAILJS_PASSWORD "rst2020!!!" 
// This password should be the same as the NodeJS script
// 这个密码应该与NodeJS脚本相同
#define SAMPMAILJS_CONSOLELOG 0 
// Log some information on console
// 在控制台记录一些信息
#define SAMPMAILJS_MAXPOSTDATA 1024 
// Cellsize of the variable that holds the data sent to the script. Lower for small emails, higher for more extense emails
// 保存发送到脚本的数据的变量长度[字符串长度]。小邮件数值低，大邮件使用更多值.

new MySQL:g_Sql;
new Float:exp_multiple = 1.0; //经验倍数 默认正常经验 如果有需要双倍经验等功能可自行操控

// defines
#define function%0(%1)          forward %0(%1);public %0(%1)
#define Color_White				0xEFEFF7FF
#define Color_Admin				0x0088FFFF
#define Color_Red				0xFF0000FF
#define Color_Vehicle			0x66B3FFFF
#define Color_Orange			0xFFA500FF
#define Color_Green				0x32CD32FF
#define Color_House				0xADD8E6FF
#define Color_Mission			0x6CA6CDFF
#define Color_Yellow			0xFFD700FF
#define Color_LightBlue			0x33CCFFAA
#define Color_Blue				0x375FFFFF
#define Color_Gray				0xCFCFCFFF
#define Color_WorldRed          0xFF6A6AFF
#define Color_Race              0x98CDFEFF
#define Color_ACColor           0xFF8040FF
#define Color_AdminMessage      0x0066FFFF
#define Color_Furniture         0xFF0000C8
#define Color_Announcement      0x0088FFFF
#define SCM                     SendClientMessage
#define SCMALL                  SendClientMessageToAll

// #define PAY_SPRAY
// #define BARBER
// #define PIZZA
// #define BURGER
// #define CLUCKIN
// #define TATTOO
// #define CLOTHES
// #define AMMUNATION
#include <timerfix> //计时器精准度修复 配合timerfix插件使用
#include <izcmd> //2020.2.28开始转向izcmd
#include <a_http>
#include <a_mysql>
#include <sscanf2>
#include <streamer>
#include <mSelection>
#include <easyDialog>
#include <compat> //2021.2.23引入0.3dl和0.3.7兼容链接并添加插件

#include <common/mkstr>
#include <common/TimestampToDate>

//2020.7.8引入easydialog
// github https://github.com/Awsomedude/easyDialog



// new DB:user;
new skinlist = mS_INVALID_LISTID, planelist = mS_INVALID_LISTID, chaopaolist = mS_INVALID_LISTID, Motorolalist = mS_INVALID_LISTID;
new Shiplist = mS_INVALID_LISTID, Otherlist = mS_INVALID_LISTID, jingchelist = mS_INVALID_LISTID, huoche = mS_INVALID_LISTID;
new tuoche = mS_INVALID_LISTID, trainlist = mS_INVALID_LISTID, yueyelist = mS_INVALID_LISTID, minzhen = mS_INVALID_LISTID;

// new tpaB[MAX_PLAYERS], tpaid[MAX_PLAYERS], tpatime[MAX_PLAYERS];
// Email here
#define MAX_PCODE_LENGTH 5 //验证码长度
#define MAX_EMAIL_LENGTH 32 //邮箱长度
// TextDraw Here
new Text:Screen[8];
new PlayerText:velo[MAX_PLAYERS][22];
new PlayerText:network_txtdraw[MAX_PLAYERS][11];

new PlayerColorPage[MAX_PLAYERS];


enum AdminInfo {
    WannaBanName[MAX_PLAYER_NAME+1],
    WannaBanReason,
    WannaBanTime
}
new aInfo[MAX_PLAYERS][AdminInfo];
new AntiCommand[MAX_PLAYERS];
enum pInfo {
    Password[65],//SHA-256后的密码
    tvid, //正在观看的ID
    bool:tvzt, //是否正在观看
    Float:SavePos[4],
    nitro,
    SaveInterior, //玩家的空间ID吧 大概是都处在同一个的才看得见彼此？？（不是世界
    BuyID, //创建的车辆ID
    CreateCar, //控制判断是否创建车辆 其实可以用BuyID来判断的 
    CarLock,
    AutoFix, //车辆无敌状态
    JailSeconds, //被监狱的秒数
    Score, //时间分
    Cash,
    BanTime, //封禁时间
    BanReason, //封禁原因
    bool:Login, //是否登录
    // bool:Register, //是否注册
    Salt[12],
    regdate,
    ID, //数据库ID
    AdminLevel, //管理员等级
    Skin, //皮肤ID
    LoginAttempts, //尝试密码的错误次数
    Yztime, //这4个大概是邮箱的？？
    yzwrong,
    yzbantime,
    Confirmed,
    Email[MAX_EMAIL_LENGTH + 1],//邮箱
    ConfCode[MAX_PCODE_LENGTH + 1],//验证码
    bool:hys, //车辆换颜色判断保存
    tWeather, //2020.2.7新增设置界面的天气保存
    AutoFlip, //车辆自动翻车
    Float:lastZpos, //上一秒的Z轴坐标 用于反载具数据异常作弊
    Float:lastVehSpeed, //上一秒的车速 用于反载具数据异常作弊
    Float:lastXMoved, //与上一秒的X
    Float:lastYMoved, //Y
    Float:lastZMoved, //Z轴相比，如果数值太大且不是高级点可能是瞬移
    AFKTimes, //赛道中挂机时长过长直接T出赛道
    tHour,//玩家的时间-小时
    tMinute,//玩家的时间-分钟
    // Team[TeamFullName_Length+1], //团队名称
    Team[24+1], //团队名称
    NoCrash, //屏蔽碰撞 在PPC失效
    Designation[19], //称号 撑死4个汉字 +1因为\0
    Tail[33], //小尾巴 最多16个汉字 同理
    speedoMeter, //是否显示速度表 默认1
    yssym,//延时摄影的分
    yssyh,//延时摄影的时
    yssyjsq, //延时摄影计时器
    enableInvincible,//无敌状态
    displayObject,//开关OBJ
    netStats,//网络参数
    lastBytesSent, //最后一次发送(上传)的量
    lastBytesReceived, //最后一次收到（下载）的量
    lastMessagesReceived, //最后一次收到（下载）的包
    lastMessagesSent, //最后一次发送(上传)的包
    showName,//显示名称
    showStunt,//显示特效奖励
    tdSpeed,//3d速度表
    tdEnabled,//是否启用3d速度表
    exp,//当前经验值
    plevel, //等级
    LoginTimer, //登录超时计时器
    bantotal, //被封禁次数，用于计算信誉分;
    tpa_gotoid,//传送玩家id
	tpa_ban,//传送开关
	tpa_requesttimer,//传送请求计时
    bool:Android,//是否为安卓等移动端环境,
    DelayLogin,//延迟加载玩家个人信息，便于检测是否为安卓移动端等环境
	LastIP[32],//自动登录
    Cache: Cache_ID
}
new PlayerInfo[MAX_PLAYERS][pInfo]; //玩家资料

enum Chars {
    ID,
    Float:CharsX,
    Float:CharsY,
    Float:CharsZ
}

new Float:BirthPointInfo[][4] = {
    {
        1958.835693,
        1343.151123,
        15.374607,
        269.142425
    }, //原来的默认出生点 赌场那边
    {
        1751.1097,
        -2106.4529,
        13.5469,
        183.1979
    }, // El-Corona - Outside random house
    {
        2652.6418,
        -1989.9175,
        13.9988,
        182.7107
    }, // Random house in willowfield - near playa de seville and stadium
    {
        2489.5225,
        -1957.9258,
        13.5881,
        2.3440
    }, // Hotel in willowfield - near cluckin bell
    {
        2689.5203,
        -1695.9354,
        10.0517,
        39.5312
    }, // Outside stadium - lots of cars
    {
        2770.5393,
        -1628.3069,
        12.1775,
        4.9637
    }, // South in east beach - north of stadium - carparks nearby
    {
        2807.9282,
        -1176.8883,
        25.3805,
        173.6018
    }, // North in east beach - near apartments
    {
        2552.5417,
        -958.0850,
        82.6345,
        280.2542
    }, // Random house north of Las Colinas
    {
        2232.1309,
        -1159.5679,
        25.8906,
        103.2939
    }, // Jefferson motel
    {
        2388.1003,
        -1279.8933,
        25.1291,
        94.3321
    }, // House south of pig pen
    {
        2481.1885,
        -1536.7186,
        24.1467,
        273.4944
    }, // East LS - near clucking bell and car wash
    {
        2495.0720,
        -1687.5278,
        13.5150,
        359.6696
    }, // Outside CJ's house - lots of cars nearby
    {
        2306.8252,
        -1675.4340,
        13.9221,
        2.6271
    }, // House in ganton - lots of cars nearby
    {
        2191.8403,
        -1455.8251,
        25.5391,
        267.9925
    }, // House in south jefferson - lots of cars nearby
    {
        1830.1359,
        -1092.1849,
        23.8656,
        94.0113
    }, // Mulholland intersection carpark
    {
        2015.3630,
        -1717.2535,
        13.5547,
        93.3655
    }, // Idlewood house
    {
        1654.7091,
        -1656.8516,
        22.5156,
        177.9729
    }, // Right next to PD
    {
        1219.0851,
        -1812.8058,
        16.5938,
        190.0045
    }, // Conference Center
    {
        1508.6849,
        -1059.0846,
        25.0625,
        1.8058
    }, // Across the street of BANK - lots of cars in intersection carpark
    {
        1421.0819,
        -885.3383,
        50.6531,
        3.6516
    }, // Outside house in vinewood
    {
        1133.8237,
        -1272.1558,
        13.5469,
        192.4113
    }, // Near hospital
    {
        1235.2196,
        -1608.6111,
        13.5469,
        181.2655
    }, // Backalley west of mainstreet
    {
        590.4648,
        -1252.2269,
        18.2116,
        25.0473
    }, // Outside "BAnk of San Andreas"
    {
        842.5260,
        -1007.7679,
        28.4185,
        213.9953
    }, // North of Graveyard
    {
        911.9332,
        -1232.6490,
        16.9766,
        5.2999
    }, // LS Film Studio
    {
        477.6021,
        -1496.6207,
        20.4345,
        266.9252
    }, // Rodeo Place
    {
        255.4621,
        -1366.3256,
        53.1094,
        312.0852
    }, // Outside propery in richman
    {
        281.5446,
        -1261.4562,
        73.9319,
        305.0017
    }, // Another richman property
    {
        790.1918,
        -839.8533,
        60.6328,
        191.9514
    }, // Mulholland house
    {
        1299.1859,
        -801.4249,
        84.1406,
        269.5274
    }, // Maddoggs
    {
        1240.3170,
        -2036.6886,
        59.9575,
        276.4659
    }, // Verdant Bluffs
    {
        2215.5181,
        -2627.8174,
        13.5469,
        273.7786
    }, // Ocean docks 1
    {
        2509.4346,
        -2637.6543,
        13.6453,
        358.3565
    }, // Ocean Docks spawn 2
    {
        1435.8024,
        2662.3647,
        11.3926,
        1.1650
    }, //  Northern train station
    {
        1457.4762,
        2773.4868,
        10.8203,
        272.2754
    }, //  Northern golf club
    {
        1739.6390,
        2803.0569,
        14.2735,
        285.3929
    }, //  Northern housing estate 1
    {
        1870.3096,
        2785.2471,
        14.2734,
        42.3102
    }, //  Northern housing estate 2
    {
        1959.7142,
        2754.6863,
        10.8203,
        181.4731
    }, //  Northern house 1
    {
        2314.2556,
        2759.4504,
        10.8203,
        93.2711
    }, //  Northern industrial estate 1
    {
        2216.5674,
        2715.0334,
        10.8130,
        267.6540
    }, //  Northern industrial estate 2
    {
        2101.4192,
        2678.7874,
        10.8130,
        92.0607
    }, //  Northern near railway line
    {
        1951.1090,
        2660.3877,
        10.8203,
        180.8461
    }, //  Northern house 2
    {
        1666.6949,
        2604.9861,
        10.8203,
        179.8495
    }, //  Northern house 3
    {
        2808.3367,
        2421.5107,
        11.0625,
        136.2060
    }, //  Northern shopping centre
    {
        2633.3203,
        2349.7061,
        10.6719,
        178.7175
    }, //  V-Rock
    {
        2606.6348,
        2161.7490,
        10.8203,
        88.7508
    }, //  South V-Rock
    {
        2616.5286,
        2100.6226,
        10.8158,
        177.7834
    }, //  North Ammunation 1
    {
        2491.8816,
        2397.9370,
        10.8203,
        266.6003
    }, //  North carpark 1
    {
        2531.7891,
        2530.3223,
        21.8750,
        91.6686
    }, //  North carpark 2
    {
        2340.6677,
        2530.4324,
        10.8203,
        177.8630
    }, //  North Pizza Stack
    {
        2097.6855,
        2491.3313,
        14.8390,
        181.8117
    }, //  Emerald Isle
    {
        1893.1000,
        2423.2412,
        11.1782,
        269.4385
    }, //  Souvenir shop
    {
        1698.9330,
        2241.8320,
        10.8203,
        357.8584
    }, //  Northern casino
    {
        1479.4559,
        2249.0769,
        11.0234,
        306.3790
    }, //  Baseball stadium 1
    {
        1298.1548,
        2083.4016,
        10.8127,
        256.7034
    }, //  Baseball stadium 2
    {
        1117.8785,
        2304.1514,
        10.8203,
        81.5490
    }, //  North carparks
    {
        1108.9878,
        1705.8639,
        10.8203,
        0.6785
    }, //  Dirtring racing 1
    {
        1423.9780,
        1034.4188,
        10.8203,
        90.9590
    }, //  Sumo
    {
        1537.4377,
        752.0641,
        11.0234,
        271.6893
    }, //  Church
    {
        1917.9590,
        702.6984,
        11.1328,
        359.2682
    }, //  Southern housing estate
    {
        2089.4785,
        658.0414,
        11.2707,
        357.3572
    }, //  Southern house 1
    {
        2489.8286,
        928.3251,
        10.8280,
        67.2245
    }, //  Wedding chapel
    {
        2697.4717,
        856.4916,
        9.8360,
        267.0983
    }, //  Southern construction site
    {
        2845.6104,
        1288.1444,
        11.3906,
        3.6506
    }, //  Southern train station
    {
        2437.9370,
        1293.1442,
        10.8203,
        86.3830
    }, //  Wedding chapel (near Pyramid)
    {
        2299.5430,
        1451.4177,
        10.8203,
        269.1287
    }, //  Carpark (near Pyramid)
    {
        2214.3008,
        2041.9165,
        10.8203,
        268.7626
    }, //  Central parking lot
    {
        2005.9174,
        2152.0835,
        10.8203,
        270.1372
    }, //  Central motel
    {
        2222.1042,
        1837.4220,
        10.8203,
        88.6461
    }, //  Clowns Pocket
    {
        2025.6753,
        1916.4363,
        12.3382,
        272.5852
    }, //  The Visage
    {
        2087.9902,
        1516.5336,
        10.8203,
        48.9300
    }, //  Royal Casino
    {
        2172.1624,
        1398.7496,
        11.0625,
        91.3783
    }, //  Auto Bahn
    {
        2139.1841,
        987.7975,
        10.8203,
        0.2315
    }, //  Come-a-lot
    {
        1860.9672,
        1030.2910,
        10.8203,
        271.6988
    }, //  Behind 4 Dragons
    {
        1673.2345,
        1316.1067,
        10.8203,
        177.7294
    }, //  Airport carpark
    {
        1412.6187,
        2000.0596,
        14.7396,
        271.3568
    }, //  South baseball stadium houses
    {
        -2723.4639,
        -314.8138,
        7.1839,
        43.5562
    }, // golf course spawn
    {
        -2694.5344,
        64.5550,
        4.3359,
        95.0190
    }, // in front of a house
    {
        -2458.2000,
        134.5419,
        35.1719,
        303.9446
    }, // hotel
    {
        -2796.6589,
        219.5733,
        7.1875,
        88.8288
    }, // house
    {
        -2706.5261,
        397.7129,
        4.3672,
        179.8611
    }, // park
    {
        -2866.7683,
        691.9363,
        23.4989,
        286.3060
    }, // house
    {
        -2764.9543,
        785.6434,
        52.7813,
        357.6817
    }, // donut shop
    {
        -2660.9402,
        883.2115,
        79.7738,
        357.4440
    }, // house
    {
        -2861.0796,
        1047.7109,
        33.6068,
        188.2750
    }, //  parking lot
    {
        -2629.2009,
        1383.1367,
        7.1833,
        179.7006
    }, // parking lot at the bridge
    {
        -2079.6802,
        1430.0189,
        7.1016,
        177.6486
    }, // pier
    {
        -1660.2294,
        1382.6698,
        9.8047,
        136.2952
    }, //  pier 69
    {
        -1674.1964,
        430.3246,
        7.1797,
        226.1357
    }, // gas station]
    {
        -1954.9982,
        141.8080,
        27.1747,
        277.7342
    }, // train station
    {
        -1956.1447,
        287.1091,
        35.4688,
        90.4465
    }, // car shop
    {
        -1888.1117,
        615.7245,
        35.1719,
        128.4498
    }, // random
    {
        -1922.5566,
        886.8939,
        35.3359,
        272.1293
    }, // random
    {
        -1983.3458,
        1117.0645,
        53.1243,
        271.2390
    }, // church
    {
        -2417.6458,
        970.1491,
        45.2969,
        269.3676
    }, // gas station
    {
        -2108.0171,
        902.8030,
        76.5792,
        5.7139
    }, // house
    {
        -2097.5664,
        658.0771,
        52.3672,
        270.4487
    }, // random
    {
        -2263.6650,
        393.7423,
        34.7708,
        136.4152
    }, // random
    {
        -2287.5027,
        149.1875,
        35.3125,
        266.3989
    }, // baseball parking lot
    {
        -2039.3571,
        -97.7205,
        35.1641,
        7.4744
    }, // driving school
    {
        -1867.5022,
        -141.9203,
        11.8984,
        22.4499
    }, // factory
    {
        -1537.8992,
        116.0441,
        17.3226,
        120.8537
    }, // docks ship
    {
        -1708.4763,
        7.0187,
        3.5489,
        319.3260
    }, // docks hangar
    {
        -1427.0858,
        -288.9430,
        14.1484,
        137.0812
    }, // airport
    {
        -2173.0654,
        -392.7444,
        35.3359,
        237.0159
    }, // stadium
    {
        -2320.5286,
        -180.3870,
        35.3135,
        179.6980
    }, // burger shot
    {
        -2930.0049,
        487.2518,
        4.9141,
        3.8258
    } // harbor
};


// 2020.2.15 修改 原本是float:sqrt
stock Float:sqrt(Float:number) {
    new Float:new_guess;
    new Float:last_guess;
    if(number < 0.0) return -1.0;
    if(number == 0.0) return 0.0;
    new_guess = 1.0;
    do {
        last_guess = new_guess;
        new_guess = (last_guess + number / last_guess) / 2.0;
    } while (new_guess != last_guess);
    return new_guess;
}


stock SetPlayerPos_Birth(const playerid) {
    new rand = random(sizeof BirthPointInfo);
    SetPlayerPos(playerid, BirthPointInfo[rand][0], BirthPointInfo[rand][1], BirthPointInfo[rand][2]);
    SetPlayerFacingAngle(playerid, BirthPointInfo[rand][3]);
}

stock GetPlayerCreditpoints(const playerid) {
    // return 100 - PlayerInfo[playerid][bantotal] * 10;
    return 100 - PlayerInfo[playerid][bantotal];
}

stock GiveCreditpoints(const playerid,const points = 1) {
    PlayerInfo[playerid][bantotal]+=points;
    if(PlayerInfo[playerid][bantotal] < 0) PlayerInfo[playerid][bantotal] = 0;
    new str[32];
    if(points < 0) format(str, sizeof(str), "~g~+%d points", points * -1);
    else format(str, sizeof(str), "~r~-%d points", points);
    GameTextForPlayer(playerid, str, 2222, 1);
    new query[128];
    mysql_format(g_Sql, query, sizeof(query), "UPDATE  `users` SET `bantotal` = %d WHERE `Name` = '%e'", PlayerInfo[playerid][bantotal], GetName(playerid));
    mysql_pquery(g_Sql, query);
}

stock GivePlayerCash(const playerid, const Cashs) {
    PlayerInfo[playerid][Cash] = PlayerInfo[playerid][Cash] + Cashs;
    new str[64];
    if(Cashs > 0) format(str, sizeof(str), "~g~+%d Cash", Cashs);
    else format(str, sizeof(str), "~r~%d Cash", Cashs);
    GameTextForPlayer(playerid, str, 2222, 1);
    // 关于金钱的操作实时更新数据库以防止意外丢失
    new query[128];
    mysql_format(g_Sql, query, sizeof(query), "UPDATE `users` SET `Cash` = %d WHERE `Name` = '%e'", PlayerInfo[playerid][Cash], GetName(playerid));
    mysql_pquery(g_Sql, query);
    return 1;
}
stock GivePlayerExp(const playerid, const Float:exp_cut, const show = 1) {
    new str[128];
    PlayerInfo[playerid][exp]+=floatround(exp_cut);
    for(;;)
    {
        if(PlayerInfo[playerid][plevel] == 201) break;
        new nextLevelExp = PlayerInfo[playerid][plevel]*144;
        if(PlayerInfo[playerid][exp] >= nextLevelExp)
        {
            PlayerInfo[playerid][exp]-=nextLevelExp;
            // PlayerInfo[playerid][exp]-=nextLevelExp;
            PlayerInfo[playerid][plevel]++;
            format(str, sizeof str, "[等级]恭喜你升到了%d级", PlayerInfo[playerid][plevel]);
            SendClientMessage(playerid, Color_White, str);
        }
        else
        {
            break;
        }
    }
    // 关于经验值的操作实时更新数据库以防止意外丢失
    mysql_format(g_Sql, str, sizeof str, "UPDATE `users` SET `exp`= %d, `pLevel` = %d WHERE `Name` = '%e'", PlayerInfo[playerid][exp], PlayerInfo[playerid][plevel], GetName(playerid));
    mysql_pquery(g_Sql, str);
    if(show)
    {
        format(str, sizeof(str), "~b~+%.0f exp", exp_cut);
        GameTextForPlayer(playerid, str, 2222, 1);
    }
    return 1;
}
stock GetPlayerLevelName(const playerid) {
    new msg[8 + 1];
    format(msg, sizeof msg, "骇速车神"); 
    if(PlayerInfo[playerid][plevel]<=190) format(msg, sizeof msg, "闪电车神");
    if(PlayerInfo[playerid][plevel]<=180) format(msg, sizeof msg, "火速车神");
    if(PlayerInfo[playerid][plevel]<=160) format(msg, sizeof msg, "车神");
    if(PlayerInfo[playerid][plevel]<=170) format(msg, sizeof msg, "风速车神");
    if(PlayerInfo[playerid][plevel]<=150) format(msg, sizeof msg, "万王之王");
    if(PlayerInfo[playerid][plevel]<=145) format(msg, sizeof msg, "极速之王");
    if(PlayerInfo[playerid][plevel]<=140) format(msg, sizeof msg, "飞速之王");
    if(PlayerInfo[playerid][plevel]<=135) format(msg, sizeof msg, "飙车之王");
    if(PlayerInfo[playerid][plevel]<=130) format(msg, sizeof msg, "赛车之王");
    if(PlayerInfo[playerid][plevel]<=125) format(msg, sizeof msg, "雷霆骑士");
    if(PlayerInfo[playerid][plevel]<=125) format(msg, sizeof msg, "闪电骑士");
    if(PlayerInfo[playerid][plevel]<=120) format(msg, sizeof msg, "火速骑士");
    if(PlayerInfo[playerid][plevel]<=115) format(msg, sizeof msg, "风速骑士");
    if(PlayerInfo[playerid][plevel]<=110) format(msg, sizeof msg, "飞龙先锋");
    if(PlayerInfo[playerid][plevel]<=105) format(msg, sizeof msg, "雄鹰先锋");
    if(PlayerInfo[playerid][plevel]<=100) format(msg, sizeof msg, "猎豹先锋");
    if(PlayerInfo[playerid][plevel]<=90) format(msg, sizeof msg, "羚之先锋");
    if(PlayerInfo[playerid][plevel]<=80) format(msg, sizeof msg, "领先者");
    if(PlayerInfo[playerid][plevel]<=70) format(msg, sizeof msg, "超越者");
    if(PlayerInfo[playerid][plevel]<=60) format(msg, sizeof msg, "竞速者");
    if(PlayerInfo[playerid][plevel]<=50) format(msg, sizeof msg, "驾御者");
    if(PlayerInfo[playerid][plevel]<=40) format(msg, sizeof msg, "飞车手");
    if(PlayerInfo[playerid][plevel]<=30) format(msg, sizeof msg, "快车手");
    if(PlayerInfo[playerid][plevel]<=20) format(msg, sizeof msg, "车手");
    if(PlayerInfo[playerid][plevel]<=10) format(msg, sizeof msg, "新手");
    return msg;
}
stock GetMaxPage(const s, const rows = 20) //给定总数,和行数，求出页数 默认20行
{
    new maxpage;
    maxpage = s % rows; //RACE_PAGES >> rows
    if(maxpage == 0) {
        maxpage = s / rows; //RACE_PAGES >> rows
    } else {
        maxpage = (s / rows) + 1; //RACE_PAGES >> rows
    }
    return maxpage;
}

static const Coords[][Chars] = {
    {
        63,
        2067.4,
        -1831.2,
        13.5
    },
    {
        63,
        488.0,
        -1734.0,
        34.4
    },
    {
        63,
        720.016,
        -454.625,
        15.328
    },
    {
        63,
        -1420.547,
        2583.945,
        58.031
    },
    {
        63,
        1966.532,
        2162.65,
        10.995
    },
    {
        63,
        -2425.46,
        1020.83,
        49.39
    },
    {
        63,
        1021.8,
        -1018.7,
        30.9
    },
    {
        63,
        -1908.9,
        292.3,
        40.0
    },
    {
        63,
        -103.6,
        1112.4,
        18.7
    },
    {
        7,
        822.6,
        -1590.3,
        13.5
    },
    {
        7,
        -2570.1,
        245.4,
        10.3
    },
    {
        7,
        2726.6,
        -2026.4,
        17.5
    },
    {
        7,
        2080.3,
        2119.0,
        10.8
    },
    {
        7,
        675.7,
        -496.6,
        16.8
    },
    {
        29,
        -1805.7,
        943.2,
        24.8
    },
    {
        29,
        2750.9,
        2470.9,
        11.0
    },
    {
        29,
        2351.8,
        2529.0,
        10.8
    },
    {
        29,
        2635.5,
        1847.4,
        11.0
    },
    {
        29,
        2083.4,
        2221.0,
        11.0
    },
    {
        29,
        -1719.1,
        1359.4,
        8.6
    },
    {
        29,
        2330.2,
        75.2,
        31.0
    },
    {
        29,
        203.2,
        -200.4,
        6.5
    },
    {
        10,
        812.9,
        -1616.1,
        13.6
    },
    {
        10,
        1199.1,
        -924.0,
        43.3
    },
    {
        10,
        2362.2,
        2069.9,
        10.8
    },
    {
        10,
        2469.5,
        2033.8,
        10.8
    },
    {
        10,
        2172.9,
        2795.7,
        10.8
    },
    {
        10,
        1875.3,
        2072.0,
        10.8
    },
    {
        10,
        1161.5,
        2072.0,
        10.8
    },
    {
        10,
        -2356.0,
        1009.0,
        49.0
    },
    {
        10,
        -1913.3,
        826.2,
        36.9
    },
    {
        10,
        -2335.6,
        -165.6,
        39.5
    },
    {
        14,
        2397.8,
        -1895.6,
        13.7
    },
    {
        14,
        2421.6,
        -1509.6,
        24.1
    },
    {
        14,
        -2671.6,
        257.4,
        4.6
    },
    {
        14,
        2392.4,
        2046.5,
        10.8
    },
    {
        14,
        2844.5,
        2401.1,
        11.0
    },
    {
        14,
        2635.5,
        1674.3,
        11.0
    },
    {
        14,
        2105.7,
        2228.7,
        11.0
    },
    {
        14,
        -2154.0,
        -2461.2,
        30.8
    },
    {
        14,
        -1816.2,
        620.8,
        37.5
    },
    {
        14,
        -1216.0,
        1831.4,
        45.3
    },
    {
        14,
        172.73,
        1176.76,
        13.7
    },
    {
        14,
        932.0,
        -1353.0,
        14.0
    },
    {
        39,
        1971.7,
        -2036.6,
        13.5
    },
    {
        39,
        2071.6,
        -1779.9,
        13.5
    },
    {
        39,
        2094.6,
        2119.0,
        10.8
    },
    {
        39,
        -2490.5,
        -40.1,
        39.3
    },
    {
        45,
        -2376.4,
        909.2,
        45.4
    },
    {
        45,
        1654.0,
        1733.4,
        11.0
    },
    {
        45,
        2105.7,
        2257.4,
        11.0
    },
    {
        45,
        -2371.1,
        910.2,
        47.2
    },
    {
        45,
        501.7,
        -1358.5,
        16.4
    },
    {
        45,
        2818.6,
        2401.5,
        11.0
    },
    {
        45,
        2112.8,
        -1214.7,
        23.9
    },
    {
        45,
        2772.0,
        2447.6,
        11.0
    },
    {
        45,
        -2489.0,
        -26.9,
        32.6
    },
    {
        27,
        2644.252,
        -2028.246,
        12.5547
    },
    {
        27,
        1043.4,
        -1025.3,
        34.4
    },
    {
        6,
        1372.9,
        -1278.8,
        12.5
    },
    {
        6,
        -2626.6,
        209.4,
        4.9
    },
    {
        6,
        2535.9,
        2083.5,
        10.8
    },
    {
        6,
        2156.5,
        943.2,
        10.8
    },
    {
        6,
        779.7,
        1874.3,
        4.9
    },
    {
        6,
        -2092.7,
        -2463.8,
        30.6
    },
    {
        6,
        240.0,
        -178.2,
        2.0
    },
    {
        6,
        -1509.4,
        2611.8,
        58.5
    },
    {
        6,
        -315.67,
        829.87,
        13.43
    },
    {
        6,
        2332.9,
        63.6,
        31.0
    },
    {
        18,
        2447.364,
        -1974.496,
        12.5469
    }
};

stock Initialize_Main() {
    //加载默认地图
    // if(!fexist("Users/users.db")) {
    //     print("[提示]玩家数据库加载失败");
    // } else {
    //     user = db_open("Users/users.db");
    //     print("[提示]玩家数据库加载成功");
    // }
    for (new i; i < sizeof(Coords); i++) {
        CreateDynamicMapIcon(Coords[i][CharsX], Coords[i][CharsY], Coords[i][CharsZ], Coords[i][ID], 0xFFFFFFAA, -1, -1, -1);
    }
    print(" * Default map icons by wups *");
    skinlist = LoadModelSelectionMenu("Vehicles/skins.txt");
    planelist = LoadModelSelectionMenu("Vehicles/planelist.txt");
    chaopaolist = LoadModelSelectionMenu("Vehicles/chaopaolist.txt");
    Motorolalist = LoadModelSelectionMenu("Vehicles/Motorolalist.txt");
    Shiplist = LoadModelSelectionMenu("Vehicles/Shiplist.txt");
    Otherlist = LoadModelSelectionMenu("Vehicles/Otherlist.txt");
    huoche = LoadModelSelectionMenu("Vehicles/huochelist.txt");
    tuoche = LoadModelSelectionMenu("Vehicles/tuochelist.txt");
    jingchelist = LoadModelSelectionMenu("Vehicles/jingchelist.txt");
    trainlist = LoadModelSelectionMenu("Vehicles/trainlist.txt");
    yueyelist = LoadModelSelectionMenu("Vehicles/yueyelist.txt");
    minzhen = LoadModelSelectionMenu("Vehicles/normallist.txt");
    Screen[0] = TextDrawCreate(641.375000, 1.000000, "usebox");
    TextDrawLetterSize(Screen[0], 0.000000, 12.711111);
    TextDrawTextSize(Screen[0], -2.000000, 0.000000);
    TextDrawAlignment(Screen[0], 3);
    TextDrawColor(Screen[0], 0);
    TextDrawUseBox(Screen[0], true);
    TextDrawBoxColor(Screen[0], 102);
    TextDrawSetShadow(Screen[0], 0);
    TextDrawSetOutline(Screen[0], -315);
    TextDrawFont(Screen[0], 0);

    Screen[1] = TextDrawCreate(320.000000, 90.666793, "Welcome to RaceSpeedTime team server");
    TextDrawLetterSize(Screen[1], 0.25, 1.0);
    TextDrawAlignment(Screen[1], 2);
    TextDrawColor(Screen[1], Color_White);
    TextDrawSetShadow(Screen[1], 0);
    TextDrawSetOutline(Screen[1], 1);
    TextDrawBackgroundColor(Screen[1], 50);
    TextDrawFont(Screen[1], 1);
    TextDrawSetProportional(Screen[1], 1);

    Screen[2] = TextDrawCreate(641.375000, 314.166687, "usebox");
    TextDrawLetterSize(Screen[2], 0.000000, 14.655550);
    TextDrawTextSize(Screen[2], -2.000000, 0.000000);
    TextDrawAlignment(Screen[2], 3);
    TextDrawColor(Screen[2], 0);
    TextDrawUseBox(Screen[2], true);
    TextDrawBoxColor(Screen[2], 102);
    TextDrawSetShadow(Screen[2], 0);
    TextDrawSetOutline(Screen[2], 0);
    TextDrawFont(Screen[2], 1);

    Screen[3] = TextDrawCreate(550.000000, 388, ServerVersion);
    TextDrawLetterSize(Screen[3], 0.18, 0.72);
    TextDrawAlignment(Screen[3], 2);
    TextDrawColor(Screen[3], Color_White);
    TextDrawSetShadow(Screen[3], 0);
    TextDrawSetOutline(Screen[3], 1);
    TextDrawBackgroundColor(Screen[3], 50);
    TextDrawFont(Screen[3], 3);
    TextDrawSetProportional(Screen[3], 1);

    Screen[4] = TextDrawCreate(-1.125000, 117.000068, "usebox");
    TextDrawLetterSize(Screen[4], 0.131870, -0.003611);
    TextDrawTextSize(Screen[4], 637.375000, 0.000000);
    TextDrawAlignment(Screen[4], 1);
    TextDrawColor(Screen[4], 0);
    TextDrawUseBox(Screen[4], true);
    TextDrawBoxColor(Screen[4], 102);
    TextDrawSetShadow(Screen[4], 0);
    TextDrawSetOutline(Screen[4], 0);
    TextDrawFont(Screen[4], 0);

    Screen[5] = TextDrawCreate(637.000000, 314.750061, "usebox");
    TextDrawLetterSize(Screen[5], 0.000000, 0.007402);
    TextDrawTextSize(Screen[5], -6.375000, 0.000000);
    TextDrawAlignment(Screen[5], 3);
    TextDrawColor(Screen[5], 0);
    TextDrawUseBox(Screen[5], true);
    TextDrawBoxColor(Screen[5], 102);
    TextDrawSetShadow(Screen[5], 0);
    TextDrawSetOutline(Screen[5], 0);
    TextDrawFont(Screen[5], 0);

    Screen[6] = TextDrawCreate(641.375000, 402, "usebox");
    TextDrawLetterSize(Screen[6], 0.000000, 4.998147);
    TextDrawTextSize(Screen[6], -1.375000, 0.000000);
    TextDrawAlignment(Screen[6], 3);
    TextDrawColor(Screen[6], 0);
    TextDrawUseBox(Screen[6], true);
    TextDrawBoxColor(Screen[6], 255);
    TextDrawSetShadow(Screen[6], 0);
    TextDrawSetOutline(Screen[6], 0);
    TextDrawFont(Screen[6], 0);

    Screen[7] = TextDrawCreate(640.125000, 1.000000, "usebox");
    TextDrawLetterSize(Screen[7], 0.000000, 4.868515);
    TextDrawTextSize(Screen[7], -1.375000, 0.000000);
    TextDrawAlignment(Screen[7], 3);
    TextDrawColor(Screen[7], 255);
    TextDrawUseBox(Screen[7], true);
    TextDrawBoxColor(Screen[7], 255);
    TextDrawSetShadow(Screen[7], 0);
    TextDrawSetOutline(Screen[7], 0);
    TextDrawFont(Screen[7], 0);
}


new const VehicleNames[212][] = { //定义载具名称
    "Landstalker",
    "Bravura",
    "Buffalo",
    "Linerunner",
    "Pereniel",
    "Sentinel",
    "Dumper",
    "Firetruck",
    "Trashmaster",
    "Stretch",
    "Manana",
    "Infernus",
    "Voodoo",
    "Pony",
    "Mule",
    "Cheetah",
    "Ambulance",
    "Leviathan",
    "Moonbeam",
    "Esperanto",
    "Taxi",
    "Washington",
    "Bobcat",
    "Mr Whoopee",
    "BF Injection",
    "Hunter",
    "Premier",
    "Enforcer",
    "Securicar",
    "Banshee",
    "PColor_Redator",
    "Bus",
    "Rhino",
    "Barracks",
    "Hotknife",
    "Trailer",
    "Previon",
    "Coach",
    "Cabbie",
    "Stallion",
    "Rumpo",
    "RC Bandit",
    "Romero",
    "Packer",
    "Monster",
    "Admiral",
    "Squalo",
    "Seasparrow",
    "Pizzaboy",
    "Tram",
    "Trailer",
    "Turismo",
    "Speeder",
    "Reefer",
    "Tropic",
    "Flatbed",
    "Yankee",
    "Caddy",
    "Solair",
    "Berkley's RC Van",
    "Skimmer",
    "PCJ-600",
    "Faggio",
    "Freeway",
    "RC Baron",
    "RC Raider",
    "Glendale",
    "Oceanic",
    "Sanchez",
    "Sparrow",
    "Patriot",
    "Quad",
    "Coastguard",
    "Dinghy",
    "Hermes",
    "Sabre",
    "Rustler",
    "ZR-350",
    "Walton",
    "Regina",
    "Comet",
    "BMX",
    "Burrito",
    "Camper",
    "Marquis",
    "Baggage",
    "Dozer",
    "Maverick",
    "News Chopper",
    "Rancher",
    "FBI Rancher",
    "Virgo",
    "Color_Greenwood",
    "Jetmax",
    "Hotring",
    "Sandking",
    "Blista Compact",
    "Police Maverick",
    "Boxville",
    "Benson",
    "Mesa",
    "RC Goblin",
    "Hotring Racer A",
    "Hotring Racer B",
    "Bloodring Banger",
    "Rancher",
    "Super GT",
    "Elegant",
    "Journey",
    "Bike",
    "Mountain Bike",
    "Beagle",
    "Cropdust",
    "Stunt",
    "Tanker",
    "RoadTrain",
    "Nebula",
    "Majestic",
    "Buccaneer",
    "Shamal",
    "Hydra",
    "FCR-900",
    "NRG-500",
    "HPV1000",
    "Cement Truck",
    "Tow Truck",
    "Fortune",
    "Cadrona",
    "FBI Truck",
    "Willard",
    "Forklift",
    "Tractor",
    "Combine",
    "Feltzer",
    "Remington",
    "Slamvan",
    "Blade",
    "Freight",
    "Streak",
    "Vortex",
    "Vincent",
    "Bullet",
    "Clover",
    "Sadler",
    "Firetruck",
    "Hustler",
    "Intruder",
    "Primo",
    "Cargobob",
    "Tampa",
    "Sunrise",
    "Merit",
    "Utility",
    "Nevada",
    "Yosemite",
    "Windsor",
    "Monster A",
    "Monster B",
    "Uranus",
    "Jester",
    "Sultan",
    "Stratum",
    "Elegy",
    "Raindance",
    "RC Tiger",
    "Flash",
    "Tahoma",
    "Savanna",
    "Bandito",
    "Freight",
    "Trailer",
    "Kart",
    "Mower",
    "Duneride",
    "Sweeper",
    "Broadway",
    "Tornado",
    "AT-400",
    "DFT-30",
    "Huntley",
    "Stafford",
    "BF-400",
    "Newsvan",
    "Tug",
    "Trailer A",
    "Emperor",
    "Wayfarer",
    "Euros",
    "Hotdog",
    "Club",
    "Trailer B",
    "Trailer C",
    "Andromada",
    "Dodo",
    "RC Cam",
    "Launch",
    "Police Car (LSPD)",
    "Police Car (SFPD)",
    "Police Car (LVPD)",
    "Police Ranger",
    "Picador",
    "S.W.A.T. Van",
    "Alpha",
    "Phoenix",
    "Glendale",
    "Sadler",
    "Luggage Trailer A",
    "Luggage Trailer B",
    "Stair Trailer",
    "Boxville",
    "Farm Plow",
    "Utility Trailer"
};


//定义玩家颜色 进入时取随机色
new PlayerColors[200] = {
    0xFF8C13FF,
    0xC715FFFF,
    0x20B2AAFF,
    0xDC143CFF,
    0x6495EDFF,
    0xf0e68cFF,
    0x778899FF,
    0xFF1493FF,
    0xF4A460FF,
    0xEE82EEFF,
    0xFFD720FF,
    0x8b4513FF,
    0x4949A0FF,
    0x148b8bFF,
    0x14ff7fFF,
    0x556b2fFF,
    0x0FD9FAFF,
    0x10DC29FF,
    0x534081FF,
    0x0495CDFF,
    0xEF6CE8FF,
    0xBD34DAFF,
    0x247C1BFF,
    0x0C8E5DFF,
    0x635B03FF,
    0xCB7ED3FF,
    0x65ADEBFF,
    0x5C1ACCFF,
    0xF2F853FF,
    0x11F891FF,
    0x7B39AAFF,
    0x53EB10FF,
    0x54137DFF,
    0x275222FF,
    0xF09F5BFF,
    0x3D0A4FFF,
    0x22F767FF,
    0xD63034FF,
    0x9A6980FF,
    0xDFB935FF,
    0x3793FAFF,
    0x90239DFF,
    0xE9AB2FFF,
    0xAF2FF3FF,
    0x057F94FF,
    0xB98519FF,
    0x388EEAFF,
    0x028151FF,
    0xA55043FF,
    0x0DE018FF,
    0x93AB1CFF,
    0x95BAF0FF,
    0x369976FF,
    0x18F71FFF,
    0x4B8987FF,
    0x491B9EFF,
    0x829DC7FF,
    0xBCE635FF,
    0xCEA6DFFF,
    0x20D4ADFF,
    0x2D74FDFF,
    0x3C1C0DFF,
    0x12D6D4FF,
    0x48C000FF,
    0x2A51E2FF,
    0xE3AC12FF,
    0xFC42A8FF,
    0x2FC827FF,
    0x1A30BFFF,
    0xB740C2FF,
    0x42ACF5FF,
    0x2FD9DEFF,
    0xFAFB71FF,
    0x05D1CDFF,
    0xC471BDFF,
    0x94436EFF,
    0xC1F7ECFF,
    0xCE79EEFF,
    0xBD1EF2FF,
    0x93B7E4FF,
    0x3214AAFF,
    0x184D3BFF,
    0xAE4B99FF,
    0x7E49D7FF,
    0x4C436EFF,
    0xFA24CCFF,
    0xCE76BEFF,
    0xA04E0AFF,
    0x9F945CFF,
    0xDCDE3DFF,
    0x10C9C5FF,
    0x70524DFF,
    0x0BE472FF,
    0x8A2CD7FF,
    0x6152C2FF,
    0xCF72A9FF,
    0xE59338FF,
    0xEEDC2DFF,
    0xD8C762FF,
    0xD8C762FF,
    0xFF8C13FF,
    0xC715FFFF,
    0x20B2AAFF,
    0xDC143CFF,
    0x6495EDFF,
    0xf0e68cFF,
    0x778899FF,
    0xFF1493FF,
    0xF4A460FF,
    0xEE82EEFF,
    0xFFD720FF,
    0x8b4513FF,
    0x4949A0FF,
    0x148b8bFF,
    0x14ff7fFF,
    0x556b2fFF,
    0x0FD9FAFF,
    0x10DC29FF,
    0x534081FF,
    0x0495CDFF,
    0xEF6CE8FF,
    0xBD34DAFF,
    0x247C1BFF,
    0x0C8E5DFF,
    0x635B03FF,
    0xCB7ED3FF,
    0x65ADEBFF,
    0x5C1ACCFF,
    0xF2F853FF,
    0x11F891FF,
    0x7B39AAFF,
    0x53EB10FF,
    0x54137DFF,
    0x275222FF,
    0xF09F5BFF,
    0x3D0A4FFF,
    0x22F767FF,
    0xD63034FF,
    0x9A6980FF,
    0xDFB935FF,
    0x3793FAFF,
    0x90239DFF,
    0xE9AB2FFF,
    0xAF2FF3FF,
    0x057F94FF,
    0xB98519FF,
    0x388EEAFF,
    0x028151FF,
    0xA55043FF,
    0x0DE018FF,
    0x93AB1CFF,
    0x95BAF0FF,
    0x369976FF,
    0x18F71FFF,
    0x4B8987FF,
    0x491B9EFF,
    0x829DC7FF,
    0xBCE635FF,
    0xCEA6DFFF,
    0x20D4ADFF,
    0x2D74FDFF,
    0x3C1C0DFF,
    0x12D6D4FF,
    0x48C000FF,
    0x2A51E2FF,
    0xE3AC12FF,
    0xFC42A8FF,
    0x2FC827FF,
    0x1A30BFFF,
    0xB740C2FF,
    0x42ACF5FF,
    0x2FD9DEFF,
    0xFAFB71FF,
    0x05D1CDFF,
    0xC471BDFF,
    0x94436EFF,
    0xC1F7ECFF,
    0xCE79EEFF,
    0xBD1EF2FF,
    0x93B7E4FF,
    0x3214AAFF,
    0x184D3BFF,
    0xAE4B99FF,
    0x7E49D7FF,
    0x4C436EFF,
    0xFA24CCFF,
    0xCE76BEFF,
    0xA04E0AFF,
    0x9F945CFF,
    0xDCDE3DFF,
    0x10C9C5FF,
    0x70524DFF,
    0x0BE472FF,
    0x8A2CD7FF,
    0x6152C2FF,
    0xCF72A9FF,
    0xE59338FF,
    0xEEDC2DFF,
    0xD8C762FF,
    0xD8C762FF
};

new const InvalidWords[][] =
    // 屏蔽骂人脏话和敏感词
    // 仅限用于合法的、积极向上的敏感词过滤使用，严禁用于从事违反法律法规、危害国家、危害人民、不道德的活动
    // 本程序此部分为游戏内关键词屏蔽.使用者不得将此词库用于非法目的
    {
        "联姻",
        "lianyin",
        "黑鬼",
        "外国人永久居住管理条例",
        "杨勇宜",
        "卖国",
        "v信",
        "xin",
        "薇",
        "徴",
        "溦",
        "嶶",
        "霺",
        "v:",
        "v：",
        "WX",
        "Ⅴ",
        "∨",
        "加妹",
        "加微",
        "加v",
        "+妹",
        "+微",
        "+v",
        "+q",
        "+我",
        "我v",
        "我.v",
        "我威",
        "妹v",
        "上期",
        "赢了",
        "明晚",
        "QQ",
        "中了",
        "借款",
        "红包",
        "利息",
        "您有一条新消息",
        "办卡",
        "idiot",
        "stupid",
        "fuck",
        "ass",
        "lick",
        "suck",
        "dick",
        "pussy",
        "cum",
        "sperm",
        "gay",
        "sb",
        "傻逼",
        "shit",
        "操",
        "娇喘",
        "sex",
        "色情",
        "反动",
        "暴力",
        "抵制",
        "porn",
        "anus",
        "arse",
        "arsehole",
        "ass-hat",
        "ass-jabber",
        "ass-pirate",
        "assbag",
        "assbandit",
        "assbanger",
        "assbite",
        "assclown",
        "asscock",
        "asscracker",
        "asses",
        "assface",
        "assfuck",
        "assfucker",
        "assgoblin",
        "asshat",
        "asshead",
        "asshole",
        "asshopper",
        "assjacker",
        "asslick",
        "asslicker",
        "assmonkey",
        "assmunch",
        "assmuncher",
        "assnigger",
        "asspirate",
        "assshit",
        "assshole",
        "asssucker",
        "asswad",
        "asswipe",
        "axwound",
        "bampot",
        "bastard",
        "beaner",
        "bitch",
        "bitchass",
        "bitches",
        "bitchtits",
        "bitchy",
        "blowjob",
        "bollocks",
        "bollox",
        "boner",
        "brotherfucker",
        "bullshit",
        "bumblefuck",
        "butt?plug",
        "butt-pirate",
        "buttfucka",
        "buttfucker",
        "carpetmuncher",
        "chesticle",
        "chinc",
        "chink",
        "choad",
        "chode",
        "clit",
        "clitface",
        "clitfuck",
        "clusterfuck",
        "cock",
        "cockass",
        "cockbite",
        "cockburger",
        "cockface",
        "cockfucker",
        "cockhead",
        "cockjockey",
        "cockknoker",
        "cockmaster",
        "cockmongler",
        "cockmongruel",
        "cockmonkey",
        "cockmuncher",
        "cocknose",
        "cocknugget",
        "cockshit",
        "cocksmith",
        "cocksmoke",
        "cocksmoker",
        "cocksniffer",
        "cocksucker",
        "cockwaffle",
        "coochie",
        "coochy",
        "coon",
        "cooter",
        "cracker",
        "cumbubble",
        "cumdumpster",
        "cumguzzler",
        "cumjockey",
        "cumslut",
        "cumtart",
        "cunnie",
        "cunnilingus",
        "cunt",
        "cuntass",
        "cuntface",
        "cunthole",
        "cuntlicker",
        "cuntrag",
        "cuntslut",
        "dago",
        "damn",
        "deggo",
        "dick-sneeze",
        "dickbag",
        "dickbeaters",
        "dickface",
        "dickfuck",
        "dickfucker",
        "dickhead",
        "dickhole",
        "dickjuice",
        "dickmilk",
        "dickmonger",
        "dicks",
        "dickslap",
        "dicksucker",
        "dicksucking",
        "dicktickler",
        "dickwad",
        "dickweasel",
        "dickweed",
        "dickwod",
        "dike",
        "dipshit",
        "doochbag",
        "dookie",
        "douche",
        "douche-fag",
        "douchebag",
        "douchewaffle",
        "dumass",
        "dumbass",
        "dumbfuck",
        "dumbshit",
        "dumshit",
        "dyke",
        "fag",
        "fagbag",
        "fagfucker",
        "faggit",
        "faggot",
        "faggotcock",
        "fagtard",
        "fatass",
        "fellatio",
        "feltch",
        "fuckass",
        "fuckbag",
        "fuckboy",
        "fuckbrain",
        "fuckbutt",
        "fuckbutter",
        "fucked",
        "fucker",
        "fuckersucker",
        "fuckface",
        "fuckhead",
        "fuckhole",
        "fuckin",
        "fucking",
        "fucknut",
        "fucknutt",
        "fuckoff",
        "fucks",
        "fuckstick",
        "fucktard",
        "fucktart",
        "fuckup",
        "fuckwad",
        "fuckwit",
        "fuckwitt",
        "fudgepacker",
        "gayass",
        "gaybob",
        "gaydo",
        "gayfuck",
        "gayfuckist",
        "gaylord",
        "gaytard",
        "gaywad",
        "gooch",
        "gook",
        "gringo",
        "guido",
        "handjob",
        "heeb",
        "hoe",
        "homo",
        "homodumbshit",
        "honkey",
        "humping",
        "jackass",
        "jagoff",
        "jap",
        "jerk?off",
        "jerkass",
        "jigaboo",
        "jizz",
        "jungle?bunny",
        "junglebunny",
        "kike",
        "kooch",
        "kootch",
        "kraut",
        "kunt",
        "kyke",
        "lameass",
        "lardass",
        "lesbian",
        "lesbo",
        "lezzie",
        "mcfagget",
        "mick",
        "minge",
        "mothafucka",
        "mothafuckin",
        "motherfucker",
        "motherfucking",
        "muff",
        "muffdiver",
        "munging",
        "negro",
        "nigaboo",
        "nigga",
        "nigger",
        "niggers",
        "niglet",
        "nutsack",
        "paki",
        "panooch",
        "pecker",
        "peckerhead",
        "penis",
        "penisbanger",
        "penisfucker",
        "penispuffer",
        "piss",
        "pissed",
        "pissflaps",
        "polesmoker",
        "pollock",
        "poon",
        "poonani",
        "poonany",
        "poontang",
        "porch?monkey",
        "porchmonkey",
        "prick",
        "punanny",
        "punta",
        "pussies",
        "pussylicking",
        "puto",
        "queef",
        "queer",
        "queerbait",
        "queerhole",
        "renob",
        "rimjob",
        "ruski",
        "sand?nigger",
        "sandnigger",
        "schlong",
        "scrote",
        "shitass",
        "shitbag",
        "shitbagger",
        "shitbrains",
        "shitbreath",
        "shitcanned",
        "shitcunt",
        "shitdick",
        "shitface",
        "shitfaced",
        "shithead",
        "shithole",
        "shithouse",
        "shitspitter",
        "shitstain",
        "shitter",
        "shittiest",
        "shitting",
        "shitty",
        "shiz",
        "shiznit",
        "skank",
        "skeet",
        "skullfuck",
        "slut",
        "slutbag",
        "smeg",
        "spic",
        "spick",
        "splooge",
        "spook",
        "suckass",
        "unclefucker",
        "va-j-j",
        "vag",
        "vagina",
        "vajayjay",
        "vjayjay",
        "wank",
        "wankjob",
        "wetback",
        "whore",
        "whorebag",
        "whoreface",
        "wop",
        "爱液",
        "按摩棒",
        "拔出来",
        "爆草",
        "包二奶",
        "暴干",
        "暴奸",
        "暴乳",
        "爆乳",
        "暴淫",
        "屄",
        "被操",
        "被插",
        "被干",
        "逼奸",
        "仓井空",
        "插暴",
        "操逼",
        "操黑",
        "操烂",
        "肏你",
        "肏死",
        "操死",
        "操我",
        "厕奴",
        "插比",
        "插b",
        "插逼",
        "插进",
        "插你",
        "插我",
        "插阴",
        "潮吹",
        "潮喷",
        "成人dv",
        "成人电影",
        "成人论坛",
        "成人小说",
        "成人电",
        "成人卡通",
        "成人聊",
        "成人片",
        "成人视",
        "成人图",
        "成人文",
        "成人小",
        "成人色情",
        "成人网站",
        "成人文学",
        "艳情小说",
        "成人游戏",
        "吃精",
        "赤裸",
        "抽插",
        "扌由插",
        "抽一插",
        "春药",
        "大波",
        "大力抽送",
        "大乳",
        "荡妇",
        "荡女",
        "盗撮",
        "多人轮",
        "发浪",
        "放尿",
        "肥逼",
        "粉穴",
        "封面女郎",
        "风月大陆",
        "干死你",
        "干穴",
        "肛交",
        "肛门",
        "龟头",
        "裹本",
        "国产av",
        "好嫩",
        "豪乳",
        "黑逼",
        "后庭",
        "后穴",
        "虎骑",
        "花花公子",
        "换妻俱乐部",
        "黄片",
        "几吧",
        "鸡吧",
        "鸡巴",
        "鸡奸",
        "寂寞男",
        "寂寞女",
        "妓女",
        "激情",
        "集体淫",
        "奸情",
        "叫床",
        "脚交",
        "金鳞岂是池中物",
        "金麟岂是池中物",
        "精液",
        "就去日",
        "巨屌",
        "菊花洞",
        "菊门",
        "巨奶",
        "巨乳",
        "菊穴",
        "开苞",
        "口爆",
        "口活",
        "口交",
        "口射",
        "口淫",
        "裤袜",
        "狂操",
        "狂插",
        "浪逼",
        "浪妇",
        "浪叫",
        "浪女",
        "狼友",
        "聊性",
        "流淫",
        "铃木麻",
        "凌辱",
        "漏乳",
        "露b",
        "乱交",
        "乱伦",
        "轮暴",
        "轮操",
        "轮奸",
        "裸陪",
        "买春",
        "美逼",
        "美少妇",
        "美乳",
        "美腿",
        "美穴",
        "美幼",
        "秘唇",
        "迷奸",
        "密穴",
        "蜜穴",
        "蜜液",
        "摸奶",
        "摸胸",
        "母奸",
        "奈美",
        "奶子",
        "男奴",
        "内射",
        "嫩逼",
        "嫩女",
        "嫩穴",
        "捏弄",
        "女优",
        "炮友",
        "砲友",
        "喷精",
        "屁眼",
        "品香堂",
        "前凸后翘",
        "强jian",
        "强暴",
        "强奸处女",
        "情趣用品",
        "情色",
        "拳交",
        "全裸",
        "群交",
        "惹火身材",
        "人妻",
        "人兽",
        "日逼",
        "日烂",
        "肉棒",
        "肉逼",
        "肉唇",
        "肉洞",
        "肉缝",
        "肉棍",
        "肉茎",
        "肉具",
        "揉乳",
        "肉穴",
        "肉欲",
        "乳爆",
        "乳房",
        "乳沟",
        "乳交",
        "乳头",
        "三级片",
        "骚逼",
        "骚比",
        "骚女",
        "骚水",
        "骚穴",
        "色逼",
        "色界",
        "色猫",
        "色盟",
        "色情网站",
        "色区",
        "色色",
        "色诱",
        "色欲",
        "色b",
        "少年阿宾",
        "少修正",
        "射爽",
        "射颜",
        "食精",
        "释欲",
        "兽奸",
        "兽交",
        "手淫",
        "兽欲",
        "熟妇",
        "熟母",
        "熟女",
        "爽片",
        "爽死我了",
        "双臀",
        "死逼",
        "丝袜",
        "丝诱",
        "松岛枫",
        "酥痒",
        "汤加丽",
        "套弄",
        "体奸",
        "体位",
        "舔脚",
        "舔阴",
        "调教",
        "偷欢",
        "偷拍",
        "推油",
        "脱内裤",
        "文做",
        "我就色",
        "无码",
        "舞女",
        "无修正",
        "吸精",
        "夏川纯",
        "相奸",
        "小逼",
        "校鸡",
        "小穴",
        "小xue",
        "写真",
        "性感妖娆",
        "性感诱惑",
        "性虎",
        "性饥渴",
        "性技巧",
        "性交",
        "性奴",
        "性虐",
        "性息",
        "性欲",
        "胸推",
        "穴口",
        "学生妹",
        "穴图",
        "亚情",
        "颜射",
        "阳具",
        "杨思敏",
        "要射了",
        "夜勤病栋",
        "一本道",
        "一夜欢",
        "一夜情",
        "一ye情",
        "阴部",
        "淫虫",
        "阴唇",
        "淫荡",
        "阴道",
        "淫电影",
        "阴阜",
        "淫妇",
        "淫河",
        "阴核",
        "阴户",
        "淫贱",
        "淫叫",
        "淫教师",
        "阴茎",
        "阴精",
        "淫浪",
        "淫媚",
        "淫糜",
        "淫魔",
        "淫母",
        "淫女",
        "淫虐",
        "淫妻",
        "淫情",
        "淫色",
        "淫声浪语",
        "淫兽学园",
        "淫书",
        "淫术炼金士",
        "淫水",
        "淫娃",
        "淫威",
        "淫亵",
        "淫样",
        "淫液",
        "淫照",
        "阴b",
        "应召",
        "幼交",
        "幼男",
        "幼女",
        "欲火",
        "欲女",
        "玉女心经",
        "玉蒲团",
        "玉乳",
        "欲仙欲死",
        "玉穴",
        "援交",
        "原味内衣",
        "援助交际",
        "张筱雨",
        "招鸡",
        "招妓",
        "中年美妇",
        "抓胸",
        "自拍",
        "自慰",
        "作爱",
        "18禁",
        "99bb",
        "a4u",
        "a4y",
        "adult",
        "amateur",
        "anal",
        "a片",
        "gay片",
        "g点",
        "g片",
        "hardcore",
        "h动画",
        "h动漫",
        "incest",
        "secom",
        "sexinsex",
        "sm女王",
        "xiao77",
        "xing伴侣",
        "tokyohot",
        "yin荡",
        "贱人",
        "装b",
        "大sb",
        "傻b",
        "煞逼",
        "煞笔",
        "刹笔",
        "傻比",
        "沙比",
        "欠干",
        "婊子养的",
        "我日你",
        "我操",
        "我草",
        "卧艹",
        "爆你菊",
        "艹你",
        "他妈",
        "cao你",
        "你他妈",
        "真他妈",
        "别他吗",
        "草你吗",
        "草你丫",
        "操你妈",
        "擦你妈",
        "操你娘",
        "操他妈",
        "日你妈",
        "干你妈",
        "干你娘",
        "娘西皮",
        "狗操",
        "狗草",
        "狗杂种",
        "狗日的",
        "操你祖宗",
        "操你全家",
        "操你大爷",
        "妈逼",
        "你麻痹",
        "麻痹的",
        "妈了个逼",
        "马勒",
        "狗娘养",
        "贱比",
        "贱b",
        "下贱",
        "死全家",
        "全家死光",
        "全家不得好死",
        "全家死绝",
        "白痴",
        "无耻",
        "杀b",
        "你吗b",
        "你妈的",
        "婊子",
        "贱货",
        "人渣",
        "混蛋",
        "媚外",
        "和弦",
        "兼职",
        "限量",
        "铃声",
        "性伴侣",
        "男公关",
        "火辣",
        "精子",
        "射精",
        "诱奸",
        "强奸",
        "做爱",
        "性爱",
        "发生关系",
        "按摩",
        "快感",
        "处男",
        "猛男",
        "少妇",
        "屌",
        "屁股",
        "下体",
        "内裤",
        "浑圆",
        "咪咪",
        "发情",
        "刺激",
        "白嫩",
        "粉嫩",
        "兽性",
        "风骚",
        "呻吟",
        "sm",
        "阉割",
        "高潮",
        "裸露",
        "不穿",
        "一丝不挂",
        "脱光",
        "干你",
        "干死",
        "我干",
        "裙中性运动",
        "乱奸",
        "乱伦类",
        "乱伦小",
        "伦理大",
        "伦理电影",
        "伦理毛",
        "伦理片",
        "裸聊",
        "裸聊网",
        "裸体写真",
        "裸舞视",
        "裸照",
        "美女裸体",
        "美女写真",
        "美女上门",
        "美艳少妇",
        "妹按摩",
        "妹上门",
        "迷幻药",
        "迷幻藥",
        "迷昏口",
        "迷昏药",
        "迷昏藥",
        "迷魂香",
        "迷魂药",
        "迷魂藥",
        "迷奸粉",
        "迷奸药",
        "迷情粉",
        "迷情水",
        "迷情药",
        "迷药",
        "迷藥",
        "谜奸药",
        "骚妇",
        "骚货",
        "骚浪",
        "骚嘴",
        "色电影",
        "色妹妹",
        "色情表演",
        "色情电影",
        "色情服务",
        "色情图片",
        "色情小说",
        "色情影片",
        "色情片",
        "色视频",
        "色小说",
        "性服务",
        "性福情",
        "性感少",
        "性伙伴",
        "性交视频",
        "性交图片",
        "性奴集中营",
        "阴蒂",
        "阴间来电",
        "阴茎增大",
        "阴茎助勃",
        "阴毛",
        "陰唇",
        "陰道",
        "陰戶",
        "淫荡美女",
        "淫荡视频",
        "淫荡照片",
        "淫乱",
        "淫靡",
        "淫魔舞",
        "淫情女",
        "淫肉",
        "淫騷妹",
        "淫兽",
        "淫兽学",
        "淫穴",
        "morphine",
        "摇头丸",
        "乖乖粉",
        "narcotic",
        "麻醉药",
        "精神药品",
        "爱女人",
        "腐败",
        "贪污",
        "gcd",
        "共贪党",
        "gongchandang",
        "阿共",
        "共一产一党",
        "产党共",
        "公产党",
        "工产党",
        "共c党",
        "共x党",
        "共铲",
        "供产",
        "共惨",
        "供铲党",
        "供铲谠",
        "供铲裆",
        "共残党",
        "共残主义",
        "共产主义的幽灵",
        "拱铲",
        "老共",
        "中珙",
        "中gong",
        "gc党",
        "贡挡",
        "gong党",
        "g产",
        "狗产蛋",
        "共残裆",
        "恶党",
        "邪党",
        "共产专制",
        "共产王朝",
        "裆中央",
        "土共",
        "土g",
        "共狗",
        "g匪",
        "共匪",
        "仇共",
        "共产党腐败",
        "共产党专制",
        "共产党的报应",
        "共产党的末日",
        "communistparty",
        "症腐",
        "政腐",
        "政付",
        "正府",
        "政俯",
        "政f",
        "zhengfu",
        "政zhi",
        "挡中央",
        "档中央",
        "中国zf",
        "中央zf",
        "国wu院",
        "中华帝国",
        "gong和",
        "大陆官方",
        "北京政权",
        "刘志军",
        "张曙",
        "买别墅",
        "玩女人",
        "贪20亿",
        "许宗衡",
        "贪财物",
        "李启红",
        "贪腐财富",
        "落马",
        "高官名单",
        "陈希同",
        "玩忽职守",
        "有期徒刑",
        "陈良宇",
        "受贿罪",
        "滥用职权",
        "没收个人财产",
        "成克杰",
        "死刑",
        "程维高",
        "严重违纪",
        "开除党籍",
        "撤销职务",
        "刘方仁",
        "无期徒刑",
        "倪献策",
        "徇私舞弊",
        "梁湘",
        "以权谋私",
        "撤职。",
        "李嘉廷",
        "死刑缓期",
        "张国光",
        "韩桂芝",
        "宋平顺",
        "自杀",
        "黄瑶",
        "双规",
        "陈绍基",
        "判处死刑",
        "剥夺政治权利终身",
        "没收个人全部财产",
        "石兆彬",
        "侯伍杰",
        "王昭耀",
        "剥夺政治权利",
        "杜世成",
        "沈图",
        "叛逃美国",
        "罗云光",
        "起诉",
        "张辛泰",
        "李效时",
        "边少斌",
        "徐鹏航",
        "违纪",
        "收受股票",
        "王乐毅",
        "李纪周",
        "郑光迪",
        "田凤山。",
        "邱晓华",
        "郑筱萸",
        "孙鹤龄",
        "蓝田造假案",
        "于幼军",
        "留党察看",
        "何洪达",
        "朱志刚",
        "杨汇泉",
        "官僚主义",
        "徐炳松",
        "托乎提沙比尔",
        "王宝森",
        "经济犯罪",
        "畏罪自杀。",
        "陈水文",
        "孟庆平",
        "胡长清",
        "朱川",
        "许运鸿",
        "丘广钟",
        "刘知炳",
        "丛福奎",
        "王怀忠",
        "巨额财产",
        "来源不明罪",
        "李达昌",
        "刘长贵",
        "王钟麓",
        "阿曼哈吉",
        "付晓光",
        "自动辞",
        "刘克田",
        "吕德彬",
        "刘维明",
        "双开",
        "刘志华",
        "孙瑜",
        "李堂堂",
        "韩福才?青海",
        "欧阳德?广东",
        "韦泽芳?海南",
        "铁英?北京",
        "辛业江?海南",
        "于飞?广东",
        "姜殿武?河北",
        "秦昌典?重庆",
        "范广举?黑龙江",
        "张凯广东",
        "王厚宏海南",
        "陈维席安徽",
        "王有杰河南",
        "王武龙江苏",
        "米凤君吉林",
        "宋勇辽宁",
        "张家盟浙江",
        "马烈孙宁夏",
        "黄纪诚北京",
        "常征贵州",
        "王式惠重庆",
        "周文吉",
        "王庆录广西",
        "潘广田山东",
        "朱作勇甘肃",
        "孙善武河南",
        "宋晨光江西",
        "梁春禄广西政协",
        "鲁家善?中国交通",
        "金德琴?中信",
        "李大强?神华",
        "吴文英?纺织",
        "查克明?华能",
        "朱小华光大",
        "高严?国家电力",
        "王雪冰",
        "林孔兴",
        "刘金宝",
        "张恩照",
        "陈同海",
        "康日新",
        "王益",
        "张春江",
        "洪清源",
        "平义杰",
        "李恩潮",
        "孙小虹",
        "陈忠",
        "慕绥新",
        "田凤岐",
        "麦崇楷",
        "柴王群",
        "吴振汉",
        "张秋阳",
        "徐衍东",
        "徐发?黑龙江",
        "张宗海",
        "丁鑫发",
        "徐国健",
        "李宝金",
        "单平",
        "段义和",
        "荆福生",
        "陈少勇",
        "黄松有",
        "皮黔生",
        "王华元",
        "王守业",
        "刘连昆",
        "孙晋美",
        "邵松高",
        "肖怀枢",
        "刘广智?空军",
        "姬胜德?总参",
        "廖伯年?北京",
        "打人",
        "拆迁",
        "纠纷",
        "盗窃",
        "安眠酮",
        "代药物毒品类：血浆",
        "普萘洛尔",
        "呋塞米",
        "西布曲明",
        "testosterone",
        "胰岛素样生长因子",
        "促红细胞生成素",
        "地西泮",
        "尼可刹米",
        "甲睾酮",
        "adrenaline",
        "erythropoietin",
        "地奈德",
        "莫达非尼",
        "氯噻嗪",
        "苯巴比妥",
        "促性腺激素",
        "泼尼松",
        "麻黄草",
        "雄烯二醇",
        "地塞米松",
        "tamoxifen",
        "strychnine",
        "androst",
        "新型毒品",
        "杜冷丁",
        "兴奋剂",
        "mdma",
        "海洛因",
        "海luo因",
        "heroin",
        "diamorphine",
        "diacetylmorphine",
        "鸦片",
        "阿芙蓉",
        "咖啡因",
        "cocain",
        "三唑仑",
        "美沙酮",
        "麻古",
        "k粉",
        "凯他敏",
        "ketamine",
        "冰毒",
        "苯丙胺",
        "cannabis",
        "大麻",
        "爱他死",
        "氯胺酮",
        "benzodiazepines",
        "甲基安非他明",
        "安非他命",
        "吗啡",
        "专业代理",
        "帮忙点一下",
        "帮忙点下",
        "请点击进入",
        "详情请进入",
        "私人侦探",
        "私家侦探",
        "针孔摄象",
        "调查婚外情",
        "信用卡提现",
        "无抵押贷款",
        "广告代理",
        "原音铃声",
        "借腹生子",
        "找个妈妈",
        "找个爸爸",
        "代孕妈妈",
        "代生孩子",
        "代开发票",
        "腾讯客服电话",
        "销售热线",
        "免费订购热线",
        "低价出售",
        "款到发货",
        "回复可见",
        "连锁加盟",
        "加盟连锁",
        "免费二级域名",
        "免费使用",
        "免费索取",
        "蚁力神",
        "婴儿汤",
        "售肾",
        "刻章办",
        "买小车",
        "套牌车",
        "玛雅网",
        "电脑传讯",
        "视频来源",
        "下载速度",
        "高清在线",
        "全集在线",
        "在线播放",
        "txt下载",
        "六位qq",
        "6位qq",
        "位的qq",
        "个qb",
        "送qb",
        "用刀横向切腹",
        "完全自杀手册",
        "四海帮",
        "足球投注",
        "地下钱庄",
        "中国复兴党",
        "阿波罗网",
        "曾道人",
        "六合彩",
        "改卷内幕",
        "替考试",
        "隐形耳机",
        "出售答案",
        "考中答案",
        "答an",
        "da案",
        "资金周转",
        "救市",
        "股市圈钱",
        "崩盘",
        "资金短缺",
        "证监会",
        "质押贷款",
        "小额贷款",
        "周小川",
        "刘明康",
        "尚福林",
        "孔丹",
        "汉芯造假",
        "杨树宽",
        "中印边界谈判结果",
        "喂奶门",
        "摸nai门",
        "酒瓶门",
        "脱裤门",
        "75事件",
        "乌鲁木齐",
        "新疆骚乱",
        "针刺",
        "打针",
        "食堂涨价",
        "饭菜涨价",
        "h1n1",
        "瘟疫爆发",
        "yangjia",
        "y佳",
        "yang佳",
        "杨佳",
        "杨j",
        "袭警",
        "杀警",
        "武侯祠",
        "川b26931",
        "贺立旗",
        "周正毅",
        "px项目",
        "骂四川",
        "家l福",
        "家le福",
        "加了服",
        "麦当劳被砸",
        "豆腐渣",
        "这不是天灾",
        "龙小霞",
        "震其国土",
        "yuce",
        "提前预测",
        "地震预测",
        "隐瞒地震",
        "李四光预测",
        "蟾蜍迁徙",
        "地震来得更猛烈",
        "八级地震毫无预报",
        "踩踏事故",
        "聂树斌",
        "万里大造林",
        "陈相贵",
        "张丹红",
        "尹方明",
        "李树菲",
        "王奉友",
        "零八奥运艰",
        "惨奥",
        "奥晕",
        "凹晕",
        "懊运",
        "懊孕",
        "奥孕",
        "奥你妈的运",
        "反奥",
        "628事件",
        "weng安",
        "wengan",
        "翁安",
        "瓮安事件",
        "化工厂爆炸",
        "讨回工资",
        "代办发票",
        "代办各",
        "代办文",
        "代办学",
        "代办制",
        "代辦",
        "代表烦",
        "代開",
        "代考",
        "代理发票",
        "代理票据",
        "代您考",
        "代讨债",
        "代写毕",
        "代写论文",
        "代孕",
        "代追债",
        "考后付款",
        "考机构",
        "考考邓",
        "考联盟",
        "考前答案",
        "考前付",
        "考前密卷",
        "考前预测",
        "考试,答案",
        "考试,作弊器",
        "考试包过",
        "考试保",
        "考试答案",
        "考试机构",
        "考试联盟",
        "考试枪",
        "考试作弊",
        "考试作弊器",
        "考研考中",
        "透视功能",
        "透视镜",
        "透视扑",
        "透视器",
        "透视眼睛",
        "透视眼镜",
        "透视药",
        "透视仪",
        "打死经过",
        "打死人",
        "打砸办公",
        "打砸抢",
        "代血浆",
        "KC短信",
        "KC嘉年华",
        "短信广告",
        "短信群发",
        "短信群发器",
        "小6灵通",
        "短信商务广告",
        "段录定",
        "无界浏览",
        "无界浏览器",
        "无界",
        "无网界",
        "无网界浏览",
        "无帮国",
        "KC提示",
        "KC网站",
        "UP8新势力",
        "白皮书",
        "UP新势力",
        "移民",
        "易达网络卡",
        "安魂网",
        "罢工",
        "罢课",
        "纽崔莱七折",
        "手机复制",
        "手机铃声",
        "网关",
        "神通加持法",
        "全1球通",
        "如6意通",
        "清仓",
        "灵动卡",
        "答案卫星接收机",
        "高薪养廉",
        "佳静安定片",
        "航空母舰",
        "航空售票",
        "号码百事通",
        "考前发放",
        "成本价",
        "诚信通手机商城",
        "高利贷",
        "联4通",
        "黑庄",
        "黑手党",
        "黑车",
        "联通贵宾卡",
        "联总",
        "联总这声传单",
        "联总之声传单",
        "高息贷款",
        "高干子弟",
        "恭喜你的号码",
        "恭喜您的号码",
        "高干子女",
        "各个银行全称",
        "各种发票",
        "高官",
        "高官互调",
        "高官子女",
        "喝一送一",
        "卡号",
        "复制",
        "监听王",
        "传单",
        "旦科",
        "钓鱼岛",
        "钓鱼台",
        "当官靠后台",
        "党校安插亲信",
        "传九促三",
        "客户端非法字符",
        "刻章",
        "大麻树脂",
        "大麻油",
        "大法",
        "大法弟子",
        "dpp大法",
        "fa?lun",
        "falu",
        "发抡",
        "发抡功",
        "洗脑",
        "下法轮",
        "发轮",
        "发伦",
        "发伦功",
        "发仑",
        "发沦",
        "发纶",
        "发论",
        "发论功",
        "发论公",
        "发正念",
        "发囵",
        "发愣",
        "发瞟",
        "罚抡",
        "罚伦",
        "罚仑",
        "罚沦",
        "罚纶",
        "罚囵",
        "筏抡",
        "筏轮",
        "筏伦",
        "筏仑",
        "筏沦",
        "筏纶",
        "筏论",
        "筏囵",
        "伐抡",
        "伐轮",
        "伐伦",
        "伐仑",
        "伐沦",
        "伐论",
        "伐囵",
        "乏抡",
        "乏轮",
        "乏伦",
        "乏仑",
        "乏沦",
        "乏纶",
        "乏论",
        "乏囵",
        "阀抡",
        "阀伦",
        "阀仑",
        "阀沦",
        "阀纶",
        "阀论",
        "阀囵",
        "法?轮?功",
        "法*功",
        "法.轮.功",
        "法L功",
        "法lun功",
        "法功",
        "法会",
        "法抡",
        "法抡功",
        "法轮",
        "法轮大法",
        "法轮佛法",
        "法轮功",
        "法伦",
        "法仑",
        "法沦",
        "法纶",
        "法论",
        "法十轮十功",
        "法西斯",
        "法院",
        "法正",
        "法谪",
        "法谪功",
        "法輪",
        "法囵",
        "法愣",
        "珐.輪功",
        "珐抡",
        "珐轮",
        "珐伦",
        "珐仑",
        "珐沦",
        "五不",
        "五不争鸣论坛",
        "五出三进",
        "五套功法",
        "邝锦文",
        "垡抡",
        "垡轮",
        "垡伦",
        "垡仑",
        "垡沦",
        "垡纶",
        "垡论",
        "垡囵",
        "茳澤民",
        "荭志",
        "闳志",
        "闵维方",
        "氵去",
        "氵去车仑工力",
        "转法轮",
        "砝抡",
        "砝轮",
        "砝伦",
        "砝仑",
        "砝沦",
        "砝纶",
        "真、善、忍",
        "真理教",
        "真善美",
        "真善忍",
        "砝论",
        "砝囵",
        "泓志",
        "屙民",
        "珐纶",
        "珐论",
        "珐囵",
        "falun",
        "Falundafa",
        "fa轮",
        "Flg",
        "弟子",
        "地下教会",
        "炼功",
        "梦网洪志",
        "轮大",
        "抡功",
        "轮功",
        "伦功",
        "摩门教",
        "三水法轮",
        "天皇",
        "天怒",
        "天葬",
        "车臣",
        "达赖",
        "功法",
        "讲法",
        "基督",
        "基督教",
        "护法",
        "回教",
        "教徒",
        "功友",
        "大师",
        "达赖喇嘛",
        "虹志",
        "鸿志",
        "洪传",
        "贯通两极法",
        "光祖",
        "洪吟",
        "洪哲胜",
        "洪志",
        "宏法",
        "观世音",
        "宏志",
        "弘志",
        "古兰经",
        "红志",
        "车库",
        "车仑",
        "经文",
        "福音会",
        "中国教徒",
        "统一教",
        "观音法门",
        "清海无上师",
        "盘古",
        "李洪志",
        "志洪李",
        "李宏志",
        "轮法功",
        "三去车仑",
        "氵去车仑",
        "发论工",
        "法x功",
        "法o功",
        "法0功",
        "法一轮一功",
        "轮子功",
        "车仑工力",
        "法lun",
        "法lg",
        "fl功",
        "falungong",
        "大纪元",
        "dajiyuan",
        "明慧网",
        "明慧周报",
        "正见网",
        "新唐人",
        "伪火",
        "退党",
        "tuidang",
        "退dang",
        "超越红墙",
        "自fen",
        "九评",
        "9评",
        "9ping",
        "九ping",
        "jiuping",
        "藏字石",
        "集体自杀",
        "自sha",
        "zi杀",
        "suicide",
        "titor",
        "逢8必灾",
        "逢八必灾",
        "逢9必乱",
        "逢九必乱",
        "朱瑟里诺",
        "根达亚文明",
        "诺查丹玛斯",
        "人类灭亡进程表",
        "按照马雅历法",
        "推背图",
        "推bei图",
        "济世灵文",
        "诸世纪",
        "电狗",
        "电话定位器",
        "电话拦截器",
        "电话窃听",
        "电话监",
        "电话交友",
        "电话追杀系统",
        "电击枪",
        "电鸡",
        "电警棒",
        "枪出售",
        "枪的制",
        "枪货到",
        "枪决女犯",
        "枪模",
        "枪手",
        "枪销售",
        "枪械制",
        "枪子弹",
        "售步枪",
        "售纯度",
        "售单管",
        "售弹簧刀",
        "售防身",
        "售狗子",
        "售虎头",
        "售火药",
        "售假币",
        "售健卫",
        "售军用",
        "售猎枪",
        "售氯胺",
        "售麻醉",
        "售枪支",
        "售热武",
        "售三棱",
        "售手枪",
        "售五四",
        "售一元硬",
        "售子弹",
        "售左轮",
        "亚砷（酸）酐",
        "亚砷酸钾",
        "亚砷酸钠",
        "亚硒酸",
        "亚硒酸二钠",
        "亚硒酸镁",
        "亚硒酸钠",
        "亚硒酸氢钠",
        "亚硝酸乙酯",
        "亚硝酰乙氧",
        "氧化二丁基锡",
        "氧化汞",
        "氧化铊",
        "氧化亚铊",
        "氧氯化磷",
        "原装弹",
        "原子弹方法",
        "原子弹清单",
        "腐败中国",
        "三个呆婊",
        "你办事我放心",
        "社会主义灭亡",
        "打倒",
        "共产党",
        "共产主义",
        "胡锦涛",
        "江泽民",
        "江主席",
        "李鹏",
        "罗干",
        "温家宝",
        "中共",
        "朱镕",
        "朱镕基",
        "灭亡",
        "亡党",
        "粉碎四人帮",
        "激流",
        "特供",
        "特贡",
        "特共",
        "zf",
        "尼玛",
        "你妈",
        "给我口",
        "口我",
        "给爷爬",
        "肉便器",
        "RBQ",
        "biss",
        "央视",
        "杂总",
        "必死",
        "草拟吗",
        "草泥马",
        "了个b",
        "zf大楼",
        "殃视",
        "贪污腐败",
        "强制拆除",
        "形式主义",
        "政治风波",
        "政治",
        "太子党",
        "上海帮",
        "北京帮",
        "清华帮",
        "红色贵族",
        "权贵集团",
        "河蟹社会",
        "喝血社会",
        "九风",
        "9风",
        "十七大",
        "十7大",
        "17da",
        "九学",
        "9学",
        "四风",
        "4风",
        "南街村",
        "最淫官员",
        "警匪",
        "官匪",
        "独夫民贼",
        "官商勾结",
        "城管暴力执法",
        "强制捐款",
        "毒豺",
        "一党执政",
        "一党专制",
        "一党专政",
        "专制政权",
        "宪法法院",
        "胡平",
        "苏晓康",
        "贺卫方",
        "谭作人",
        "焦国标",
        "万润南",
        "张志新",
        "辛灝年",
        "高勤荣",
        "王炳章",
        "高智晟",
        "司马璐",
        "刘晓竹",
        "刘宾雁",
        "魏京生",
        "寻找林昭的灵魂",
        "别梦成灰",
        "谁是新中国",
        "讨伐中宣部",
        "异议人士",
        "民运人士",
        "启蒙派",
        "选国家主席",
        "民一主",
        "min主",
        "民竹",
        "民珠",
        "民猪",
        "chinesedemocracy",
        "大赦国际",
        "国际特赦",
        "da选",
        "投公",
        "公头",
        "宪政",
        "平反",
        "党章",
        "维权",
        "昝爱宗",
        "宪章",
        "08宪",
        "08xz",
        "抿主",
        "敏主",
        "人拳",
        "人木又",
        "人quan",
        "renquan",
        "人权",
        "新民党",
        "群体事件",
        "群体性事件",
        "上中央",
        "去中央",
        "讨说法",
        "请愿",
        "请命",
        "公开信",
        "联名上书",
        "万人大签名",
        "万人骚动",
        "截访",
        "上访",
        "shangfang",
        "信访",
        "访民",
        "集合",
        "集会",
        "组织集体",
        "静坐",
        "静zuo",
        "jing坐",
        "示威",
        "示wei",
        "游行",
        "you行",
        "油行",
        "游xing",
        "youxing",
        "官逼民反",
        "反party",
        "反共",
        "抗议",
        "亢议",
        "低制",
        "底制",
        "di制",
        "抵zhi",
        "dizhi",
        "boycott",
        "血书",
        "焚烧",
        "baoluan",
        "流血",
        "暴动",
        "baodong",
        "灭共",
        "杀毙",
        "霸工",
        "罢考",
        "罢餐",
        "霸餐",
        "罢参",
        "罢饭",
        "罢吃",
        "罢食",
        "罢ke",
        "霸课",
        "ba课",
        "罢教",
        "罢学",
        "罢运",
        "网特",
        "网评员",
        "网络评论员",
        "五毛党",
        "五毛们",
        "5毛党",
        "戒严",
        "jieyan",
        "jie严",
        "戒yan",
        "8的平方事件",
        "知道64",
        "八九年",
        "贰拾年",
        "2o年",
        "20和谐年",
        "贰拾周年",
        "六四",
        "六河蟹四",
        "六百度四",
        "六和谐四",
        "陆四",
        "陆肆",
        "198964",
        "5月35",
        "89年春夏之交",
        "64惨案",
        "64时期",
        "64运动",
        "4事件",
        "四事件",
        "北京风波",
        "学潮",
        "学chao",
        "xuechao",
        "学百度潮",
        "门安天",
        "天按门",
        "坦克压大学生",
        "民主女神",
        "历史的伤口",
        "高自联",
        "北高联",
        "血洗京城",
        "四二六社论",
        "王丹",
        "柴玲",
        "沈彤",
        "封从德",
        "王超华",
        "王维林",
        "吾尔开希",
        "吾尔开西",
        "侯德健",
        "阎明复",
        "方励之",
        "蒋捷连",
        "丁子霖",
        "辛灏年",
        "蒋彦永",
        "严家其",
        "陈一咨",
        "中华局域网",
        "党的喉舌",
        "互联网审查",
        "当局严密封锁",
        "新闻封锁",
        "封锁消息",
        "爱国者同盟",
        "关闭所有论坛",
        "网络封锁",
        "金盾工程",
        "gfw",
        "无界网络",
        "自由门",
        "何清涟",
        "中国的陷阱",
        "汪兆钧",
        "记者无疆界",
        "境外媒体",
        "维基百科",
        "纽约时报",
        "bbc中文网",
        "华盛顿邮报",
        "世界日报",
        "东森新闻网",
        "东森电视",
        "星岛日报",
        "wikipedia",
        "youtube",
        "googleblogger",
        "美国广播公司",
        "英国金融时报",
        "自由亚洲",
        "自由时报",
        "中国时报",
        "反分裂",
        "威胁论",
        "左翼联盟",
        "保钓组织",
        "主权",
        "弓单",
        "火乍",
        "木仓",
        "石肖",
        "核蛋",
        "步qiang",
        "bao炸",
        "爆zha",
        "baozha",
        "zha药",
        "zha弹",
        "炸dan",
        "炸yao",
        "zhadan",
        "zhayao",
        "hmtd",
        "三硝基甲苯",
        "六氟化铀",
        "炸药配方",
        "弹药配方",
        "炸弹配方",
        "皮箱炸弹",
        "火药配方",
        "人体炸弹",
        "人肉炸弹",
        "解放军",
        "兵力部署",
        "军转",
        "军事社",
        "8341部队",
        "第21集团军",
        "七大军区",
        "7大军区",
        "北京军区",
        "沈阳军区",
        "济南军区",
        "成都军区",
        "广州军区",
        "南京军区",
        "兰州军区",
        "颜色革命",
        "规模冲突",
        "塔利班",
        "基地组织",
        "恐怖分子",
        "恐怖份子",
        "三股势力",
        "印尼屠华",
        "印尼事件",
        "蒋公纪念歌",
        "马英九",
        "mayingjiu",
        "李天羽",
        "苏贞昌",
        "林文漪",
        "陈水扁",
        "陈s扁",
        "陈随便",
        "阿扁",
        "a扁",
        "告全国同胞书",
        "台百度湾",
        "台完",
        "台wan",
        "taiwan",
        "台弯",
        "湾台",
        "台湾国",
        "台湾共和国",
        "台军",
        "台独",
        "台毒",
        "台du",
        "taidu",
        "twdl",
        "一中一台",
        "打台湾",
        "两岸战争",
        "攻占台湾",
        "支持台湾",
        "进攻台湾",
        "占领台湾",
        "统一台湾",
        "收复台湾",
        "登陆台湾",
        "解放台湾",
        "解放tw",
        "解决台湾",
        "光复民国",
        "台湾独立",
        "台湾问题",
        "台海问题",
        "台海危机",
        "台海统一",
        "台海大战",
        "台海战争",
        "台海局势",
        "入联",
        "入耳关",
        "中华联邦",
        "国民党",
        "x民党",
        "民进党",
        "青天白日",
        "闹独立",
        "duli",
        "fenlie",
        "日本万岁",
        "小泽一郎",
        "劣等民族",
        "汉人",
        "汉维",
        "维汉",
        "维吾",
        "吾尔",
        "热比娅",
        "伊力哈木",
        "疆独",
        "东突厥斯坦解放组织",
        "东突解放组织",
        "蒙古分裂分子",
        "列确",
        "阿旺晋美",
        "藏人",
        "臧人",
        "zang人",
        "藏民",
        "藏m",
        "赖达",
        "dalai",
        "哒赖",
        "dl喇嘛",
        "丹增嘉措",
        "西独",
        "藏独",
        "葬独",
        "臧独",
        "藏毒",
        "藏du",
        "zangdu",
        "支持zd",
        "藏暴乱",
        "藏青会",
        "雪山狮子旗",
        "拉萨",
        "啦萨",
        "啦沙",
        "啦撒",
        "拉sa",
        "lasa",
        "la萨",
        "西藏",
        "藏西",
        "藏春阁",
        "藏獨",
        "藏独立",
        "藏妇会",
        "xizang",
        "xi藏",
        "x藏",
        "西z",
        "tibet",
        "希葬",
        "希藏",
        "硒藏",
        "稀藏",
        "西脏",
        "西奘",
        "西葬",
        "西臧",
        "援藏",
        "bjork",
        "王千源",
        "安拉",
        "回族",
        "回回",
        "回民",
        "穆斯林",
        "穆罕穆德",
        "穆罕默德",
        "默罕默德",
        "伊斯兰",
        "圣战组织",
        "清真",
        "清zhen",
        "qingzhen",
        "真主",
        "阿拉伯",
        "高丽棒子",
        "韩国狗",
        "满洲第三帝国",
        "满狗",
        "鞑子",
        "江丑闻",
        "江嫡系",
        "江毒",
        "江独裁",
        "江蛤蟆",
        "江核心",
        "江黑心",
        "江胡内斗",
        "江祸心",
        "江家帮",
        "江绵恒",
        "江派和胡派",
        "江派人马",
        "江泉集团",
        "江人马",
        "江三条腿",
        "江氏集团",
        "江氏家族",
        "江氏政治局",
        "江氏政治委员",
        "江梳头",
        "江太上",
        "江戏子",
        "江系人",
        "江系人马",
        "江宰民",
        "江贼",
        "江贼民",
        "麻果丸",
        "麻将透",
        "麻醉弹",
        "麻醉狗",
        "麻醉枪",
        "麻醉槍",
        "麻醉藥",
        "台湾版假币",
        "台湾应该独立",
        "台湾有权独立",
        "天灭",
        "帮凶",
        "保命",
        "共裁",
        "党文化",
        "血旗",
        "罪恶",
        "帝国",
        "独裁",
        "习主席",
        "封锁",
        "封网",
        "VPN",
        "SSR",
        "V2ray",
        "黑帮",
        "解体",
        "近期权力斗争",
        "恐惧",
        "权力斗争",
        "任用",
        "邪教",
        "邪毒素",
        "政治游戏",
        "习近平",
        "党中央",
        "共青团",
        "党员",
        "中央",
        "李克强",
        "cnm",
        "nm",
        "马勒戈壁",
        "杂种",
        "狗",
        "港独",
        "香港独立",
        "招聘",
        "网络",
        "有意者",
        "到货",
        "本店",
        "代购",
        "扣扣",
        "客服",
        "微店",
        "兼值",
        "淘宝",
        "小姐",
        "包夜",
        "3P",
        "LY",
        "JS",
        "技师",
        "BT",
        "毒龙",
        "楼凤",
        "足交",
        "口暴",
        "全套",
        "桑拿",
        "吞精",
        "乳方",
        "全职",
        "网购",
        "网络工作",
        "代理",
        "平近习",
        "xjp",
        "习太子",
        "习明泽",
        "老习",
        "温加宝",
        "温x",
        "温jia宝",
        "温宝宝",
        "温加饱",
        "温加保",
        "张培莉",
        "温云松",
        "温如春",
        "温jb",
        "胡温",
        "胡x",
        "胡jt",
        "胡boss",
        "胡总",
        "胡王八",
        "hujintao",
        "胡jintao",
        "胡j涛",
        "胡惊涛",
        "胡景涛",
        "胡紧掏",
        "湖紧掏",
        "胡紧套",
        "锦涛",
        "hjt",
        "胡派",
        "胡主席",
        "刘永清",
        "胡海峰",
        "胡海清",
        "民泽江",
        "江胡",
        "江哥",
        "江书记",
        "江浙闽",
        "江沢民",
        "江浙民",
        "择民",
        "则民",
        "茳泽民",
        "zemin",
        "ze民",
        "老江",
        "老j",
        "江core",
        "江x",
        "江派",
        "江zm",
        "jzm",
        "江某某",
        "江猪",
        "江绵康",
        "王冶坪",
        "江泽慧",
        "邓小平",
        "平小邓",
        "xiao平",
        "邓xp",
        "邓晓平",
        "邓朴方",
        "邓榕",
        "邓质方",
        "毛泽东",
        "猫泽东",
        "猫则东",
        "猫贼洞",
        "毛zd",
        "毛zx",
        "z东",
        "ze东",
        "泽d",
        "zedong",
        "毛太祖",
        "毛相",
        "主席画像",
        "改革历程",
        "朱容基",
        "朱镕鸡",
        "朱容鸡",
        "朱云来",
        "李peng",
        "里鹏",
        "李月月鸟",
        "李小鹏",
        "李小琳",
        "华主席",
        "华国",
        "国锋",
        "国峰",
        "锋同志",
        "白春礼",
        "薄熙来",
        "薄一波",
        "蔡赴朝",
        "蔡武",
        "曹刚川",
        "常万全",
        "陈炳德",
        "陈德铭",
        "陈建国",
        "陈至立",
        "戴秉国",
        "丁一平",
        "董建华",
        "杜德印",
        "傅锐",
        "郭伯雄",
        "郭金龙",
        "贺国强",
        "胡春华",
        "耀邦",
        "华建敏",
        "黄华华",
        "黄丽满",
        "黄兴国",
        "回良玉",
        "贾庆林",
        "贾廷安",
        "靖志远",
        "李长春",
        "李春城",
        "李建国",
        "李岚清",
        "李沛瑶",
        "李荣融",
        "李瑞环",
        "李铁映",
        "李先念",
        "李学举",
        "李源潮",
        "栗智",
        "梁光烈",
        "廖锡龙",
        "林树森",
        "林炎志",
        "林左鸣",
        "令计划",
        "柳斌杰",
        "刘奇葆",
        "刘少奇",
        "刘延东",
        "刘云山",
        "龙新民",
        "路甬祥",
        "罗箭",
        "吕祖善",
        "马飚",
        "马恺",
        "孟建柱",
        "欧广源",
        "强卫",
        "沈跃跃",
        "粟戎生",
        "苏树林",
        "孙家正",
        "铁凝",
        "屠光绍",
        "王东明",
        "汪东兴",
        "王鸿举",
        "王沪宁",
        "王乐泉",
        "王洛林",
        "王岐山",
        "王胜俊",
        "王太华",
        "王学军",
        "王兆国",
        "王振华",
        "吴邦国",
        "吴定富",
        "吴官正",
        "无官正",
        "吴胜利",
        "吴仪",
        "奚国华",
        "习仲勋",
        "徐才厚",
        "许其亮",
        "徐绍史",
        "杨洁篪",
        "叶剑英",
        "由喜贵",
        "俞正声",
        "袁纯清",
        "曾培炎",
        "曾庆红",
        "曾宪梓",
        "曾荫权",
        "张德江",
        "张定发",
        "张高丽",
        "张立昌",
        "张荣坤",
        "张志国",
        "赵洪祝",
        "紫阳",
        "周生贤",
        "周永康",
        "朱海仑",
        "中南海",
        "大陆当局",
        "中国当局",
        "北京当局",
        "党产共",
        "政府",
        "中央领导",
        "贺子珍",
        "李德生",
        "姚依林",
        "李干成",
        "黄镇",
        "刘瑞龙",
        "黄敬",
        "薄熙",
        "周建南",
        "徐明",
        "新疆骚乱"
    };
// stock YukiGetSquareDistance(Float:x1, Float:y1, Float:z1, Float:x2, Float:y2, Float:z2) {
//     x1 -= x2;
//     y1 -= y2;
//     z1 -= z2;
//     x1 *= x1;
//     y1 *= y1;
//     z1 *= z1;
//     return floatround(x1 + y1 + z1);
// }


stock AccountCheck(const name[]) //检查帐号是否存在(1存在/0不存在)
{
    new query[128];
    mysql_format(g_Sql, query, sizeof(query),"SELECT COUNT(*) FROM `users` WHERE `Name` = '%e' LIMIT 1", name);
    new registered_players, Cache:result = mysql_query(g_Sql, query);
    cache_get_value_int(0, 0, registered_players);
    cache_delete(result);
    return registered_players;
}
stock BanName(const playerid, const name[]) {
    // AdminInfo[playerid][BanName] = name[];
    format(AdminInfo[playerid][WannaBanName], MAX_PLAYER_NAME, "%s", name);
    return 1;
}
stock IsValidPassword(const password[]) //检查注册密码是否有效
{
    for (new i = 0; password[i] != EOS; i++) {
        switch (password[i]) {
            case '0'..
            '9', 'A'..
            'Z', 'a'..
            'z', '[', ']', '(', ')', '$', '@', '.', '_', '=':
                continue;
            default:
                return 0;
        }
    }
    return 1;
}
stock GetSpeed(const playerid) {
    /*new Float:x,Float:y,Float:z,Float:temp;
	GetVehicleVelocity(GetPlayerVehicleID(msg),x,y,z);
	//return floatpower((x * x) + (y * y) + (z * z),0.5) * 180;*/
    // if(pRaceing[msg]) {
    //     new Float:health;
    //     GetPlayerHealth(msg, health);
    //     if(!health) ST[3] = 0;
    // }
    // else {
    new Float:ST[4];
    if(IsPlayerInAnyVehicle(playerid)) GetVehicleVelocity(GetPlayerVehicleID(playerid), ST[0], ST[1], ST[2]);
    else GetPlayerVelocity(playerid,ST[0], ST[1], ST[2]);
    ST[3] = floatsqroot(floatpower(floatabs(ST[0]), 2.0) + floatpower(floatabs(ST[1]), 2.0) + floatpower(floatabs(ST[2]), 2.0)) * 180;
    return floatround(ST[3]);
    // return ST[3];
}
stock Initialize_tpa(const playerid) {
	PlayerInfo[playerid][tpa_gotoid] = -1;
	PlayerInfo[playerid][tpa_ban] = 0;
	PlayerInfo[playerid][tpa_requesttimer] = 0;
}
stock InitializationVelo(const playerid) {
    velo[playerid][0] = CreatePlayerTextDraw(playerid,517.000000, 422.500000, "KM/H");
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][0], 255);
    PlayerTextDrawFont(playerid, velo[playerid][0], 1);
    PlayerTextDrawLetterSize(playerid, velo[playerid][0], 0.200000, 0.80000);
    PlayerTextDrawColor(playerid, velo[playerid][0], 0xFEFBACFF);
    PlayerTextDrawSetOutline(playerid, velo[playerid][0], 1);
    PlayerTextDrawSetProportional(playerid, velo[playerid][0], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][0], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][0], 0);

    velo[playerid][1] = CreatePlayerTextDraw(playerid,516.000000, 411.500000, "-1");
    PlayerTextDrawAlignment(playerid, velo[playerid][1], 3);
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][1], 255);
    PlayerTextDrawFont(playerid, velo[playerid][1], 2);
    PlayerTextDrawLetterSize(playerid, velo[playerid][1], 0.600000, 2.400000);
    PlayerTextDrawColor(playerid, velo[playerid][1], 0xFFFEE6FF);
    PlayerTextDrawSetOutline(playerid, velo[playerid][1], 1);
    PlayerTextDrawSetProportional(playerid, velo[playerid][1], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][1], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][1], 0);

    velo[playerid][2] = CreatePlayerTextDraw(playerid,455.000000, 430.000000, "/");
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][2], 255);
    PlayerTextDrawFont(playerid, velo[playerid][2], 1);
    PlayerTextDrawLetterSize(playerid, velo[playerid][2], 0.410000, 1.299998);
    PlayerTextDrawColor(playerid, velo[playerid][2], 0xC0C0C0C8);
    PlayerTextDrawSetOutline(playerid, velo[playerid][2], 0);
    PlayerTextDrawSetProportional(playerid, velo[playerid][2], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][2], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][2], 0);

    velo[playerid][3] = CreatePlayerTextDraw(playerid,459.000000, 430.000000, "/");
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][3], 255);
    PlayerTextDrawFont(playerid, velo[playerid][3], 1);
    PlayerTextDrawLetterSize(playerid, velo[playerid][3], 0.410000, 1.299998);
    PlayerTextDrawColor(playerid, velo[playerid][3], 0xC0C0C0C8);
    PlayerTextDrawSetOutline(playerid, velo[playerid][3], 0);
    PlayerTextDrawSetProportional(playerid, velo[playerid][3], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][3], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][3], 0);

    velo[playerid][4] = CreatePlayerTextDraw(playerid,463.000000, 430.000000, "/");
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][4], 255);
    PlayerTextDrawFont(playerid, velo[playerid][4], 1);
    PlayerTextDrawLetterSize(playerid, velo[playerid][4], 0.410000, 1.299998);
    PlayerTextDrawColor(playerid, velo[playerid][4], 0xC0C0C0C8);
    PlayerTextDrawSetOutline(playerid, velo[playerid][4], 0);
    PlayerTextDrawSetProportional(playerid, velo[playerid][4], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][4], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][4], 0);

    velo[playerid][5] = CreatePlayerTextDraw(playerid,467.000000, 430.000000, "/");
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][5], 255);
    PlayerTextDrawFont(playerid, velo[playerid][5], 1);
    PlayerTextDrawLetterSize(playerid, velo[playerid][5], 0.410000, 1.299998);
    PlayerTextDrawColor(playerid, velo[playerid][5], 0xC0C0C0C8);
    PlayerTextDrawSetOutline(playerid, velo[playerid][5], 0);
    PlayerTextDrawSetProportional(playerid, velo[playerid][5], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][5], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][5], 0);

    velo[playerid][6] = CreatePlayerTextDraw(playerid,471.000000, 430.000000, "/");
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][6], 255);
    PlayerTextDrawFont(playerid, velo[playerid][6], 1);
    PlayerTextDrawLetterSize(playerid, velo[playerid][6], 0.389999, 1.299998);
    PlayerTextDrawColor(playerid, velo[playerid][6], 0xC0C0C0C8);
    PlayerTextDrawSetOutline(playerid, velo[playerid][6], 0);
    PlayerTextDrawSetProportional(playerid, velo[playerid][6], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][6], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][6], 0);

    velo[playerid][7] = CreatePlayerTextDraw(playerid,475.000000, 430.000000, "/");
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][7], 255);
    PlayerTextDrawFont(playerid, velo[playerid][7], 1);
    PlayerTextDrawLetterSize(playerid, velo[playerid][7], 0.410000, 1.299998);
    PlayerTextDrawColor(playerid, velo[playerid][7], 0xC0C0C0C8);
    PlayerTextDrawSetOutline(playerid, velo[playerid][7], 0);
    PlayerTextDrawSetProportional(playerid, velo[playerid][7], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][7], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][7], 0);

    velo[playerid][8] = CreatePlayerTextDraw(playerid,479.000000, 430.000000, "/");
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][8], 255);
    PlayerTextDrawFont(playerid, velo[playerid][8], 1);
    PlayerTextDrawLetterSize(playerid, velo[playerid][8], 0.410000, 1.299998);
    PlayerTextDrawColor(playerid, velo[playerid][8], 0xC0C0C0C8);
    PlayerTextDrawSetOutline(playerid, velo[playerid][8], 0);
    PlayerTextDrawSetProportional(playerid, velo[playerid][8], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][8], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][8], 0);

    velo[playerid][9] = CreatePlayerTextDraw(playerid,483.000000, 430.000000, "/");
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][9], 255);
    PlayerTextDrawFont(playerid, velo[playerid][9], 1);
    PlayerTextDrawLetterSize(playerid, velo[playerid][9], 0.410000, 1.299998);
    PlayerTextDrawColor(playerid, velo[playerid][9], 0xC0C0C0C8);
    PlayerTextDrawSetOutline(playerid, velo[playerid][9], 0);
    PlayerTextDrawSetProportional(playerid, velo[playerid][9], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][9], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][9], 0);

    velo[playerid][10] = CreatePlayerTextDraw(playerid,487.000000, 430.000000, "/");
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][10], 255);
    PlayerTextDrawFont(playerid, velo[playerid][10], 1);
    PlayerTextDrawLetterSize(playerid, velo[playerid][10], 0.410000, 1.299998);
    PlayerTextDrawColor(playerid, velo[playerid][10], 0xC0C0C0C8);
    PlayerTextDrawSetOutline(playerid, velo[playerid][10], 0);
    PlayerTextDrawSetProportional(playerid, velo[playerid][11], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][10], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][10], 0);

    velo[playerid][11] = CreatePlayerTextDraw(playerid,491.000000, 430.000000, "/");
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][11], 255);
    PlayerTextDrawFont(playerid, velo[playerid][11], 1);
    PlayerTextDrawLetterSize(playerid, velo[playerid][11], 0.410000, 1.299998);
    PlayerTextDrawColor(playerid, velo[playerid][11], 0xC0C0C0C8);
    PlayerTextDrawSetOutline(playerid, velo[playerid][11], 0);
    PlayerTextDrawSetProportional(playerid, velo[playerid][11], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][11], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][11], 0);

    velo[playerid][12] = CreatePlayerTextDraw(playerid,495.000000, 430.000000, "/");
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][12], 255);
    PlayerTextDrawFont(playerid, velo[playerid][12], 1);
    PlayerTextDrawLetterSize(playerid, velo[playerid][12], 0.410000, 1.299998);
    PlayerTextDrawColor(playerid, velo[playerid][12], 0xC0C0C0C8);
    PlayerTextDrawSetOutline(playerid, velo[playerid][12], 0);
    PlayerTextDrawSetProportional(playerid, velo[playerid][12], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][12], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][12], 0);

    velo[playerid][13] = CreatePlayerTextDraw(playerid,499.000000, 430.000000, "/");
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][13], 255);
    PlayerTextDrawFont(playerid, velo[playerid][13], 1);
    PlayerTextDrawLetterSize(playerid, velo[playerid][13], 0.410000, 1.299998);
    PlayerTextDrawColor(playerid, velo[playerid][13], 0xC0C0C0C8);
    PlayerTextDrawSetOutline(playerid, velo[playerid][13], 0);
    PlayerTextDrawSetProportional(playerid, velo[playerid][13], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][13], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][13], 0);

    velo[playerid][14] = CreatePlayerTextDraw(playerid,503.000000, 430.000000, "/");
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][14], 255);
    PlayerTextDrawFont(playerid, velo[playerid][14], 1);
    PlayerTextDrawLetterSize(playerid, velo[playerid][14], 0.410000, 1.299998);
    PlayerTextDrawColor(playerid, velo[playerid][14], 0xC0C0C0C8);
    PlayerTextDrawSetOutline(playerid, velo[playerid][14], 0);
    PlayerTextDrawSetProportional(playerid, velo[playerid][14], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][14], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][14], 0);

    velo[playerid][15] = CreatePlayerTextDraw(playerid,507.000000, 430.000000, "/");
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][15], 255);
    PlayerTextDrawFont(playerid, velo[playerid][15], 1);
    PlayerTextDrawLetterSize(playerid, velo[playerid][15], 0.410000, 1.299998);
    PlayerTextDrawColor(playerid, velo[playerid][15], 0xC0C0C0C8);
    PlayerTextDrawSetOutline(playerid, velo[playerid][15], 0);
    PlayerTextDrawSetProportional(playerid, velo[playerid][15], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][15], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][15], 0);

    velo[playerid][16] = CreatePlayerTextDraw(playerid,511.000000, 430.000000, "/");
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][16], 255);
    PlayerTextDrawFont(playerid, velo[playerid][16], 1);
    PlayerTextDrawLetterSize(playerid, velo[playerid][16], 0.410000, 1.299998);
    PlayerTextDrawColor(playerid, velo[playerid][16], 0xC0C0C0C8);
    PlayerTextDrawSetOutline(playerid, velo[playerid][16], 0);
    PlayerTextDrawSetProportional(playerid, velo[playerid][16], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][16], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][16], 0);

    velo[playerid][17] = CreatePlayerTextDraw(playerid,515.000000, 430.000000, "/");
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][17], 255);
    PlayerTextDrawFont(playerid, velo[playerid][17], 1);
    PlayerTextDrawLetterSize(playerid, velo[playerid][17], 0.410000, 1.299998);
    PlayerTextDrawColor(playerid, velo[playerid][17], 0xC0C0C0C8);
    PlayerTextDrawSetOutline(playerid, velo[playerid][17], 0);
    PlayerTextDrawSetProportional(playerid, velo[playerid][17], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][17], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][17], 0);

    velo[playerid][18] = CreatePlayerTextDraw(playerid,519.000000, 430.000000, "/");
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][18], 255);
    PlayerTextDrawFont(playerid, velo[playerid][18], 1);
    PlayerTextDrawLetterSize(playerid, velo[playerid][18], 0.410000, 1.299998);
    PlayerTextDrawColor(playerid, velo[playerid][18], 0xAA5555C8);
    PlayerTextDrawSetOutline(playerid, velo[playerid][18], 0);
    PlayerTextDrawSetProportional(playerid, velo[playerid][18], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][18], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][18], 0);

    velo[playerid][19] = CreatePlayerTextDraw(playerid,523.000000, 430.000000, "/");
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][19], 255);
    PlayerTextDrawFont(playerid, velo[playerid][19], 1);
    PlayerTextDrawLetterSize(playerid, velo[playerid][19], 0.410000, 1.299998);
    PlayerTextDrawColor(playerid, velo[playerid][19], 0xAA5555C8);
    PlayerTextDrawSetOutline(playerid, velo[playerid][19], 0);
    PlayerTextDrawSetProportional(playerid, velo[playerid][19], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][19], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][19], 0);

    velo[playerid][20] = CreatePlayerTextDraw(playerid,527.000000, 430.000000, "/");
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][20], 255);
    PlayerTextDrawFont(playerid, velo[playerid][20], 1);
    PlayerTextDrawLetterSize(playerid, velo[playerid][20], 0.410000, 1.299998);
    PlayerTextDrawColor(playerid, velo[playerid][20], 0xAA5555C81);
    PlayerTextDrawSetOutline(playerid, velo[playerid][20], 0);
    PlayerTextDrawSetProportional(playerid, velo[playerid][20], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][20], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][20], 0);

    velo[playerid][21] = CreatePlayerTextDraw(playerid,531.000000, 430.000000, "/");
    PlayerTextDrawBackgroundColor(playerid, velo[playerid][21], 255);
    PlayerTextDrawFont(playerid, velo[playerid][21], 1);
    PlayerTextDrawLetterSize(playerid, velo[playerid][21], 0.410000, 1.299998);
    PlayerTextDrawColor(playerid, velo[playerid][21], 0xAA5555C81);
    PlayerTextDrawSetOutline(playerid, velo[playerid][21], 0);
    PlayerTextDrawSetProportional(playerid, velo[playerid][21], 1);
    PlayerTextDrawSetShadow(playerid, velo[playerid][21], 1);
    PlayerTextDrawSetSelectable(playerid, velo[playerid][21], 0);


    for (new a = 0; a <= 21; a++) {
        PlayerTextDrawShow(playerid, velo[playerid][a]);
    }
}


stock UnLoadVelo(const playerid) {
    for (new a = 0; a <= 21; a++) {
        PlayerTextDrawDestroy(playerid, velo[playerid][a]);
    }
}
stock Initialize_LocalObjects() {
    print("[提示]加载服务器OBJ");
    AddStaticVehicle(411, 1231.1265, 727.1212, 11.5800, -28.0000, -1, -1);
    AddStaticVehicle(409, 1271.2623, 757.3828, 14.8200, 47.0000, -1, -1);
    AddStaticVehicle(400, 1279.2955, 729.7867, 11.9846, -125.0000, -1, -1);
    AddStaticVehicle(432, 1268.9523, 748.4949, 10.8997, -109.0000, -1, -1);
    AddStaticVehicle(495, 1282.9364, 740.3662, 12.5600, 120.0000, -1, -1);
    AddStaticVehicle(451, 1237.9313, 726.4261, 10.5420, 47.0000, -1, -1);
    AddStaticVehicle(541, 1229.0662, 734.3873, 10.5420, -120.0000, -1, -1);
    AddStaticVehicle(429, 1229.2205, 739.8235, 11.4800, -120.0000, -1, -1);
    AddStaticVehicle(480, 1228.9167, 745.2179, 10.6420, -62.0000, -1, -1);
    AddStaticVehicle(545, 1257.5408, 749.8536, 14.9239, 55.0000, -1, -1);
    AddStaticVehicle(575, 1250.3097, 750.1832, 14.7201, -76.0000, -1, -1);
    AddStaticVehicle(475, 1263.0428, 749.8746, 14.8201, -11.0000, -1, -1);
    AddStaticVehicle(603, 1242.0275, 757.1142, 14.9234, -47.0000, -1, -1);
    AddStaticVehicle(522, 1234.4760, 757.3914, 17.9948, -4.0000, -1, -1);
    AddStaticVehicle(463, 1229.5415, 756.2962, 17.9948, -105.0000, -1, -1);
    AddStaticVehicle(415, 1227.9149, 759.1169, 18.4120, -40.0000, -1, -1);
    AddStaticVehicle(410, 1282.7378, 768.3775, 10.5637, 55.0000, -1, -1);
    AddStaticVehicle(555, 1276.0140, 768.1845, 10.6210, 0.0000, -1, -1);
    AddStaticVehicle(556, 1279.8049, 745.9430, 11.1200, 135.0000, -1, -1);
    AddStaticVehicle(413, 1227.8733, 767.3305, 10.9637, -25.0000, -1, -1);
    AddStaticVehicle(419, 1235.2123, 768.0527, 10.6843, 40.0000, -1, -1);
    AddStaticVehicle(546, 1249.3566, 768.6234, 10.6643, 135.0000, -1, -1);
    AddStaticVehicle(402, 1248.6533, 759.2233, 10.6372, 0.0000, -1, -1);
    AddStaticVehicle(562, 1267.2515, 727.6193, 10.9628, 0.0000, -1, -1);
    AddStaticVehicle(560, 1248.2955, 725.7834, 10.9017, 0.0000, -1, -1);
    AddStaticVehicle(505, 1268.9503, 735.5294, 11.0833, 55.0000, -1, -1);
    AddStaticVehicle(502, 1257.4460, 727.0239, 11.1837, 0.0000, -1, -1);
    AddStaticVehicle(504, 1227.3489, 757.0361, 12.5800, 4.0000, -1, -1);
    AddStaticVehicle(477, 1229.8782, 757.5219, 11.4400, 0.0000, -1, -1);
    AddStaticVehicle(567, 1240.4728, 759.0765, 10.7372, -40.0000, -1, -1);
    // CreateDynamicObject(19850, 418.05, -1832.13, 4.49, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19850, 418.05, -1832.13, 4.49, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19451, 409.53, -1858.54, 4.66, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19451, 414.36, -1823.00, 4.66, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19451, 428.38, -1823.00, 4.66, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19451, 409.53, -1848.90, 4.66, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19451, 409.53, -1839.27, 4.66, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19451, 409.53, -1829.64, 4.66, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19850, 418.05, -1832.13, 3.08, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19359, 409.51, -1824.61, 4.66, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16112, 431.30, -1842.56, -8.06, 0.00, 0.00, 164.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16112, 429.16, -1856.77, -8.06, 0.00, 0.00, 141.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(14414, 412.27, -1848.72, 4.76, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19390, 422.82, -1841.68, 6.22, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19361, 425.77, -1842.47, 6.22, 0.00, 0.00, 60.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19361, 427.11, -1844.83, 6.22, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19453, 410.26, -1846.58, 6.22, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19453, 410.26, -1856.21, 6.22, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19361, 428.60, -1846.51, 6.22, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19361, 428.77, -1856.96, 6.22, 0.00, 0.00, -60.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19361, 430.12, -1854.60, 6.22, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19325, 430.13, -1851.44, 5.90, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19434, 430.12, -1847.37, 6.22, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19361, 411.78, -1841.68, 6.22, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19325, 417.96, -1841.67, 5.28, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19434, 414.19, -1841.68, 6.22, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19458, 414.20, -1843.51, 3.15, 90.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19458, 414.20, -1847.01, 3.15, 90.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19458, 414.20, -1850.50, 1.86, 90.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19378, 424.94, -1856.06, 4.60, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19378, 414.44, -1846.43, 4.58, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19378, 424.94, -1846.43, 4.58, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19943, 428.96, -1859.50, 0.92, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19361, 426.01, -1858.55, 6.22, 0.00, 0.00, -60.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19434, 424.68, -1860.07, 6.22, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19378, 414.44, -1856.06, 4.60, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19385, 422.82, -1841.84, 6.22, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19355, 425.75, -1842.63, 6.22, 0.00, 0.00, 60.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19355, 426.95, -1844.97, 6.22, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19355, 428.46, -1846.53, 6.22, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19428, 430.04, -1847.39, 6.22, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19355, 430.04, -1854.58, 6.22, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19355, 428.77, -1856.90, 6.22, 0.00, 0.00, -60.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19355, 426.01, -1858.51, 6.22, 0.00, 0.00, -60.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19428, 424.64, -1860.07, 6.22, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19355, 413.38, -1841.84, 6.20, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19447, 410.30, -1856.21, 6.22, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19447, 410.30, -1846.58, 6.20, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19447, 419.79, -1860.76, 3.38, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19359, 408.57, -1841.69, 4.66, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19359, 408.57, -1860.90, 4.66, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19361, 411.79, -1860.95, 6.22, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19434, 414.19, -1860.95, 6.22, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19361, 423.17, -1860.95, 6.22, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19355, 423.15, -1860.75, 6.22, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19355, 411.79, -1860.75, 6.22, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19428, 414.19, -1860.75, 6.22, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19325, 418.30, -1860.75, 5.28, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19375, 424.94, -1856.06, 7.96, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19375, 414.44, -1856.06, 7.96, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19448, 425.42, -1848.06, 7.98, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19448, 421.93, -1846.43, 7.96, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19429, 427.97, -1849.49, 7.96, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19429, 429.39, -1849.49, 7.95, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19429, 428.42, -1847.24, 7.97, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19429, 425.56, -1844.37, 7.97, 0.00, 90.00, 60.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19429, 424.18, -1843.57, 7.97, 0.00, 90.00, 60.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19429, 423.60, -1843.36, 7.95, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19448, 415.36, -1843.36, 7.96, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19325, 414.24, -1850.09, 4.64, 90.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19439, 414.20, -1844.31, 9.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19325, 414.24, -1846.68, 8.14, 90.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19439, 411.00, -1851.88, 9.72, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19366, 414.20, -1850.34, 9.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19366, 412.67, -1851.87, 9.72, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(970, 411.36, -1851.93, 8.51, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(970, 409.28, -1854.21, 8.51, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(970, 409.30, -1858.45, 8.51, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(970, 411.36, -1860.52, 8.51, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1506, 413.47, -1857.53, 8.00, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19325, 410.28, -1846.68, 8.14, 90.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19325, 418.25, -1860.75, 9.40, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19432, 424.70, -1860.01, 9.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19359, 423.17, -1860.72, 9.72, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19359, 413.42, -1860.72, 9.72, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19359, 410.29, -1850.34, 9.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19432, 410.29, -1844.31, 9.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19432, 410.29, -1842.70, 9.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19432, 410.99, -1841.75, 9.72, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(640, 427.44, -1860.00, 8.75, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19432, 425.59, -1859.30, 9.72, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19405, 413.47, -1859.09, 9.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19405, 413.47, -1853.42, 9.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19432, 413.49, -1855.25, 9.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19842, 418.00, -1861.07, -0.96, 90.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19359, 413.39, -1841.75, 9.72, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19359, 429.69, -1854.60, 9.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1419, 429.68, -1857.24, 8.51, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1419, 427.58, -1859.24, 8.51, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3920, 416.73, -1841.51, 7.07, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(640, 430.93, -1850.54, 5.36, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(640, 418.41, -1840.85, 5.36, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(639, 430.44, -1850.60, 6.11, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(638, 429.76, -1857.83, 5.36, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(638, 428.84, -1860.42, 5.36, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(638, 426.15, -1860.42, 5.36, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(638, 430.93, -1854.71, 5.36, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3920, 418.40, -1860.99, 6.87, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1419, 416.27, -1851.96, 8.51, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1419, 420.33, -1851.96, 8.51, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19359, 420.17, -1851.92, 9.72, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1419, 420.56, -1849.80, 8.51, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1419, 420.56, -1845.74, 8.51, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1419, 418.56, -1843.69, 8.51, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19439, 415.09, -1843.60, 9.72, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1419, 416.51, -1843.70, 8.51, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1506, 426.35, -1859.25, 8.00, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19466, 413.46, -1853.42, 10.02, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19466, 413.42, -1859.02, 10.02, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19405, 428.01, -1856.10, 9.72, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19432, 426.36, -1856.96, 9.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19359, 422.82, -1841.73, 9.72, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19325, 417.96, -1841.67, 9.40, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19432, 410.28, -1842.46, 9.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(638, 429.60, -1844.57, 5.36, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(638, 428.43, -1842.32, 5.36, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19359, 429.69, -1851.40, 9.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19359, 429.69, -1848.19, 9.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19405, 428.01, -1846.69, 9.72, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19359, 425.39, -1842.98, 9.72, 0.00, 0.00, 40.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19432, 426.53, -1845.82, 9.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19432, 426.47, -1844.92, 9.72, 0.00, 0.00, 4.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19359, 423.38, -1851.92, 9.72, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3920, 416.73, -1841.65, 10.40, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19466, 428.03, -1846.63, 10.02, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19466, 427.81, -1856.10, 10.02, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1491, 424.91, -1848.28, 8.00, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19432, 425.60, -1846.69, 9.72, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19432, 424.92, -1849.06, 9.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19432, 424.92, -1850.65, 9.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19432, 424.90, -1851.07, 9.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(638, 415.86, -1844.46, 8.75, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(638, 419.00, -1844.47, 8.75, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(638, 416.02, -1851.64, 8.75, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19841, 414.58, -1871.05, -8.00, 0.00, 0.00, 4.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19841, 421.69, -1866.86, -9.18, 18.00, 0.00, -2.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19840, 427.89, -1869.89, 0.60, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(898, 423.39, -1865.16, -0.20, 0.00, 0.00, 280.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(898, 412.21, -1865.13, -0.20, 0.00, 0.00, 58.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(905, 419.86, -1862.82, 4.64, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(905, 419.26, -1861.90, 4.64, 0.00, 0.00, -47.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(898, 433.74, -1851.07, -0.20, 0.00, 0.00, 258.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(905, 432.66, -1855.67, 4.64, 0.00, 0.00, -47.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(905, 432.24, -1855.38, 4.64, 0.00, 0.00, -15.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19375, 414.44, -1856.06, 11.50, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19375, 414.44, -1846.43, 11.50, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19943, 428.35, -1844.31, 4.50, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19375, 424.94, -1846.43, 11.50, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19448, 421.43, -1856.06, 11.50, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19448, 424.92, -1856.06, 11.50, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19429, 428.42, -1852.03, 11.50, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19356, 428.42, -1854.44, 11.50, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1693, 421.49, -1857.73, 13.08, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1693, 421.49, -1850.87, 13.08, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1693, 421.49, -1844.24, 13.08, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1696, 412.30, -1856.86, 12.59, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1697, 413.23, -1851.44, 13.21, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1696, 413.23, -1845.40, 12.59, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(898, 411.23, -1836.02, 0.56, 0.00, 0.00, 238.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(898, 411.66, -1830.21, 0.56, 0.00, 0.00, 171.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(898, 412.33, -1826.85, 0.56, 0.00, 0.00, 171.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19381, 421.26, -1836.76, 4.58, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19381, 421.26, -1827.73, 4.56, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19872, 428.96, -1836.44, 2.74, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1408, 429.35, -1841.62, 5.04, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1408, 426.52, -1838.90, 5.04, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(638, 432.49, -1839.98, 5.16, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(640, 432.56, -1835.72, 5.16, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19870, 421.26, -1818.88, 4.65, 0.00, 0.00, -60.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19840, 421.22, -1836.69, 1.54, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19840, 420.96, -1829.81, 2.06, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19817, 427.65, -1824.91, 3.20, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(826, 417.29, -1832.77, 4.46, 0.00, 0.00, 55.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(869, 419.25, -1838.72, 5.10, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(869, 419.07, -1834.64, 5.10, 0.00, 0.00, 62.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1408, 421.12, -1837.49, 5.04, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1408, 421.12, -1831.97, 5.04, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1408, 418.52, -1829.08, 5.04, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19439, 422.84, -1839.84, 4.60, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(947, 425.27, -1840.13, 6.78, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2114, 425.15, -1840.56, 4.74, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2114, 425.46, -1840.54, 4.74, 0.00, -22.00, -30.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(894, 413.69, -1835.05, 5.52, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(889, 419.17, -1829.39, 5.13, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(837, 418.51, -1826.71, 4.96, 0.00, 0.00, -28.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(882, 432.82, -1860.19, 4.49, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(905, 432.45, -1861.14, 4.64, 0.00, 0.00, -47.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(905, 431.84, -1860.37, 4.64, 0.00, 0.00, -47.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(832, 414.03, -1824.83, 6.64, 0.00, 0.00, 156.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(950, 426.07, -1843.22, 8.60, 0.00, 0.00, -30.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(949, 426.96, -1845.15, 8.68, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(950, 426.87, -1844.13, 8.60, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1506, 422.05, -1841.69, 4.48, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19383, 411.97, -1854.89, 6.22, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19353, 413.49, -1856.57, 6.22, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19353, 413.50, -1859.14, 6.22, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3498, 414.25, -1852.04, 3.42, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2526, 410.85, -1858.98, 4.69, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2140, 410.88, -1855.52, 4.15, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2139, 410.90, -1856.50, 4.69, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1523, 412.70, -1854.87, 4.45, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1208, 410.66, -1857.34, 4.67, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1778, 412.86, -1855.22, 4.68, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2525, 412.94, -1858.55, 4.69, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2613, 413.46, -1857.19, 4.68, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2524, 412.90, -1856.07, 4.68, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2527, 412.89, -1859.21, 4.65, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2390, 410.60, -1858.11, 6.06, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3920, 418.40, -1860.99, 10.40, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(808, 430.26, -1859.94, 6.04, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19439, 420.56, -1844.31, 9.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19439, 419.86, -1843.60, 9.72, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2336, 414.74, -1844.74, 4.65, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2851, 414.52, -1844.78, 5.70, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2341, 414.47, -1842.40, 4.65, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2335, 414.76, -1845.74, 4.65, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2140, 414.79, -1846.80, 4.65, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2139, 415.20, -1846.80, 4.65, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2029, 417.61, -1843.78, 4.67, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2124, 418.13, -1842.95, 5.50, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2029, 417.61, -1844.76, 4.67, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2029, 417.61, -1845.74, 4.67, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2029, 417.61, -1846.72, 4.67, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2029, 417.61, -1847.70, 4.67, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19429, 415.94, -1850.46, 4.60, 90.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1491, 421.30, -1850.39, 2.90, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19429, 421.30, -1843.65, 4.60, 90.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19429, 421.30, -1847.14, 4.60, 90.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19429, 419.43, -1850.46, 4.60, 90.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19429, 419.61, -1850.45, 4.61, 90.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2124, 420.14, -1843.86, 5.50, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2124, 420.14, -1845.03, 5.50, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2124, 420.14, -1846.20, 5.50, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2124, 420.14, -1847.46, 5.50, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2124, 416.32, -1847.46, 5.50, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2124, 416.32, -1846.20, 5.50, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2124, 416.32, -1845.03, 5.50, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2124, 416.32, -1843.86, 5.50, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2865, 417.57, -1844.49, 5.49, 0.00, 0.00, -84.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2864, 418.05, -1845.84, 5.53, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2866, 414.49, -1845.40, 5.71, 0.00, 0.00, -47.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2223, 417.49, -1845.71, 5.55, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2222, 418.17, -1843.72, 5.55, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2124, 418.13, -1849.04, 5.50, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2863, 418.04, -1846.83, 5.45, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2221, 418.75, -1847.50, 5.56, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2222, 418.72, -1845.17, 5.53, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2167, 414.29, -1844.40, 6.24, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2253, 418.14, -1846.28, 5.71, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2253, 418.14, -1844.76, 5.71, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2257, 413.62, -1857.95, 6.38, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19806, 418.14, -1845.80, 10.52, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19806, 418.14, -1845.80, 9.04, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11726, 418.14, -1845.85, 7.64, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2138, 414.79, -1847.81, 4.65, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2817, 422.40, -1843.11, 4.67, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2270, 424.83, -1842.78, 6.08, 0.00, 0.00, -30.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2267, 426.00, -1842.90, 6.56, 0.00, 0.00, -30.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2268, 426.37, -1844.77, 6.66, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2269, 426.37, -1845.79, 5.68, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2270, 426.37, -1844.79, 5.60, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2266, 426.37, -1846.04, 6.74, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19786, 428.48, -1846.58, 6.52, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1826, 427.73, -1847.11, 4.65, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2826, 428.02, -1847.16, 5.47, 0.00, 0.00, 55.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19563, 428.88, -1847.20, 5.49, 0.00, 0.00, -33.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19807, 428.67, -1847.34, 5.55, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2314, 426.38, -1844.60, 4.64, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2311, 427.43, -1849.95, 4.65, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2311, 427.43, -1850.88, 4.65, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2311, 427.43, -1851.81, 4.65, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2827, 429.05, -1850.20, 5.15, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2826, 427.52, -1851.33, 5.16, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2824, 428.15, -1850.89, 5.14, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2823, 428.13, -1851.51, 5.17, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19893, 428.70, -1851.81, 5.15, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18871, 428.99, -1851.99, 5.13, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2223, 427.35, -1850.38, 5.23, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2222, 428.08, -1852.01, 5.22, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2028, 427.97, -1850.19, 5.23, 0.00, 0.00, -16.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3017, 429.23, -1851.27, 5.16, 0.00, 0.00, -91.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2632, 427.81, -1848.51, 4.67, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1703, 428.91, -1853.54, 4.61, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1703, 425.45, -1851.94, 4.61, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11729, 424.21, -1842.93, 4.62, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19174, 427.47, -1857.55, 6.36, 0.00, 0.00, -150.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19165, 429.93, -1854.43, 6.32, 90.00, 90.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19564, 417.60, -1847.84, 5.49, 0.00, 0.00, -69.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19559, 426.46, -1845.84, 5.44, 0.00, 0.00, -62.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2654, 426.28, -1844.98, 5.36, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19173, 423.09, -1860.66, 6.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19171, 423.08, -1857.40, 4.70, 0.00, 0.00, 47.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19170, 420.13, -1854.76, 4.68, 0.00, 0.00, -40.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19168, 424.09, -1855.53, 4.70, 0.00, 0.00, -33.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19169, 419.47, -1856.28, 4.70, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2631, 421.68, -1856.01, 4.69, 0.00, 0.00, 69.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(638, 413.97, -1858.13, 5.21, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2251, 421.30, -1859.96, 5.54, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(638, 423.03, -1860.11, 5.21, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2248, 414.27, -1860.10, 5.23, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2240, 419.33, -1860.07, 5.19, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1594, 417.47, -1857.69, 5.17, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2223, 417.28, -1857.72, 5.64, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2221, 417.74, -1857.67, 5.63, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2824, 421.47, -1856.88, 4.74, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1670, 421.68, -1855.65, 4.76, 0.00, 0.00, 149.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(911, 425.23, -1858.30, 5.22, 0.00, 0.00, -150.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(910, 425.01, -1822.34, 5.76, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11665, 422.97, -1850.00, 8.75, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2233, 427.30, -1846.68, 4.66, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2233, 430.04, -1846.88, 4.66, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2841, 424.61, -1847.94, 8.07, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2289, 426.40, -1846.00, 10.20, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2284, 425.57, -1846.13, 9.20, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19174, 422.58, -1851.83, 9.83, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2265, 421.12, -1844.28, 9.99, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2265, 419.89, -1843.01, 9.84, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2264, 421.14, -1844.30, 9.72, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2276, 414.80, -1843.01, 9.52, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2257, 422.87, -1852.03, 10.18, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2285, 429.10, -1854.42, 9.78, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2286, 429.57, -1852.07, 10.08, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2288, 429.10, -1850.33, 9.58, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(14446, 427.85, -1854.26, 8.65, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19805, 420.27, -1852.05, 10.23, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1985, 418.36, -1854.19, 11.41, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2381, 429.11, -1848.52, 9.86, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2708, 429.27, -1847.70, 8.06, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2387, 425.46, -1850.60, 8.02, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2269, 422.34, -1860.15, 10.11, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2272, 423.43, -1860.14, 9.32, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2276, 422.30, -1860.13, 8.77, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2290, 414.95, -1852.53, 8.05, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2627, 416.23, -1859.21, 8.05, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2628, 423.46, -1859.76, 8.03, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2629, 420.04, -1853.27, 8.02, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2915, 420.65, -1852.40, 8.11, 0.00, 0.00, 159.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2916, 420.60, -1852.80, 8.97, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2916, 419.54, -1852.80, 8.97, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2630, 418.03, -1859.26, 8.05, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2630, 419.58, -1859.19, 8.05, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2627, 421.10, -1859.06, 8.05, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2173, 422.80, -1852.52, 8.05, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2632, 415.90, -1855.45, 8.05, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2631, 425.37, -1854.05, 8.05, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2632, 418.82, -1855.47, 8.05, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19893, 423.35, -1852.58, 8.84, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2824, 422.73, -1852.49, 8.84, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11705, 423.91, -1852.48, 8.83, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1714, 423.30, -1853.72, 8.03, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2166, 422.94, -1854.37, 8.05, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2230, 422.07, -1852.07, 8.04, 0.00, 0.00, 33.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2223, 422.11, -1853.42, 8.92, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2853, 423.03, -1854.53, 8.83, 0.00, 0.00, 22.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2843, 424.88, -1853.53, 8.10, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2844, 424.56, -1855.65, 8.10, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2845, 425.10, -1854.86, 8.10, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2966, 424.93, -1854.53, 8.12, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2654, 429.07, -1848.16, 8.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10, 419.35, -1819.13, 5.48, 90.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1702, 412.87, -1853.46, 8.05, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1702, 412.87, -1858.16, 8.05, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2073, 417.19, -1857.09, 10.68, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2073, 425.30, -1853.33, 10.68, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2073, 428.32, -1847.60, 10.68, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2073, 414.86, -1843.21, 10.68, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2073, 412.62, -1848.39, 10.68, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2257, 412.21, -1851.76, 10.23, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2073, 428.65, -1849.01, 7.15, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2073, 425.60, -1844.49, 7.15, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2073, 417.62, -1858.00, 7.15, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);

    CreateDynamicObject(1655, 1510.56, 2145.93, 10.00, 0.00, 0.00, -55.44, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3458, 2991.90, -1984.70, 85.20, 40.00, 0.00, 103.93, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3458, 2988.50, -1985.70, 89.00, 40.00, 0.00, 97.93, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3458, 2981.50, -1983.60, 92.60, 40.00, 0.00, 89.93, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3458, 2978.70, -1981.70, 94.60, 40.00, 0.00, 83.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3458, 2984.30, -1985.10, 90.10, 40.00, 0.00, 89.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3458, 2986.00, -1986.80, 90.10, 40.00, 0.00, 95.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7998, 3164.20, 1953.20, 0.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7998, 3123.90, 1952.80, 0.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7998, 3204.66, 1953.20, 0.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18450, 3179.80, 2021.10, 2.10, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3749, 3179.80, 2038.90, 8.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3176.70, 1979.50, -0.25, 0.00, 0.00, 92.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3183.60, 1979.90, -0.25, 0.00, 0.00, 92.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3176.44, 1977.47, -0.31, 0.00, 0.00, -23.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3183.70, 1978.40, -0.25, 0.00, 0.00, 104.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3185.40, 1975.90, -0.25, 0.00, 0.00, 110.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3178.40, 1975.50, -0.25, 0.00, 0.00, 109.99, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3181.23, 1979.39, -0.19, 0.00, 0.00, 129.99, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3182.50, 1969.70, -0.17, 0.00, 0.00, 137.99, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3187.90, 1971.60, -0.17, 0.00, 0.00, 149.99, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3189.90, 1966.20, -0.25, 0.00, 0.00, 167.98, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3190.40, 1965.10, -0.19, 0.00, 0.00, 175.98, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3193.20, 1963.90, -0.25, 0.00, 0.00, 179.98, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3192.01, 1974.50, -0.31, 0.00, 0.00, 145.98, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3193.60, 1969.70, -0.25, 0.00, 0.00, 244.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3195.36, 1962.12, -0.31, 0.00, 0.00, 185.97, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3198.87, 1969.16, -0.17, 0.00, 0.00, 185.97, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3200.20, 1966.20, -0.13, 0.00, 0.00, 185.97, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3201.30, 1963.40, -0.25, 0.00, 0.00, 191.96, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3202.30, 1966.90, -0.25, 0.00, 0.00, 195.96, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3208.62, 1962.52, -0.29, 0.00, 0.00, 219.95, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3208.60, 1970.30, -0.25, 0.00, 0.00, 231.95, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3205.70, 1964.00, -0.21, 0.00, 0.00, 245.95, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3208.26, 1971.83, -0.19, 0.00, 0.00, 222.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3217.00, 1976.60, -0.15, 0.00, 0.00, 263.95, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3212.00, 1966.50, -0.25, 0.00, 0.00, 311.94, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3213.90, 1973.20, -0.25, 0.00, 0.00, 345.94, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3187.90, 1980.60, 3.30, 0.00, 0.00, 100.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3192.20, 1972.80, 3.30, 0.00, 0.00, 138.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3199.80, 1970.30, 3.30, 0.00, 0.00, 185.99, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3207.70, 1973.50, 3.30, 0.00, 0.00, 227.99, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3212.20, 1981.10, 3.30, 0.00, 0.00, 249.99, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3174.20, 1976.70, 3.30, 0.00, 0.00, 106.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3184.60, 1963.80, 2.90, 0.00, 0.00, 153.99, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3217.30, 1972.30, -0.17, 0.00, 0.00, 155.95, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3193.10, 1968.80, -0.25, 0.00, 0.00, 244.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3191.50, 1961.40, 2.90, 0.00, 0.00, 167.99, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3200.70, 1960.60, 2.90, 0.00, 0.00, 185.99, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3208.70, 1963.10, 2.90, 0.00, 0.00, 211.98, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3215.30, 1968.50, 2.90, 0.00, 0.00, 227.98, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3220.40, 1976.10, 2.90, 0.00, 0.00, 253.98, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3222.90, 1983.40, 2.90, 0.00, 0.00, 253.98, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3214.80, 1989.50, 2.90, 0.00, 0.00, 253.98, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3211.62, 1977.53, -0.29, 0.00, 0.00, 241.95, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3215.00, 1979.30, -0.25, 0.00, 0.00, 241.95, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3219.96, 1977.96, -0.19, 0.00, 0.00, 241.95, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3214.10, 1982.80, -0.25, 0.00, 0.00, 349.95, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3218.70, 1981.80, -0.25, 0.00, 0.00, 241.95, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3217.00, 1986.60, -0.19, 0.00, 0.00, 241.95, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3219.50, 1984.00, -0.25, 0.00, 0.00, 241.95, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3214.26, 1989.66, -0.29, 0.00, 0.00, 241.95, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3221.44, 1983.40, -0.17, 0.00, 0.00, 241.95, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3177.80, 1968.40, 2.90, 0.00, 0.00, 131.99, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3223.10, 1989.40, -0.25, 0.00, 0.00, 353.95, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3218.90, 1991.30, -0.25, 0.00, 0.00, 349.94, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3217.40, 1991.90, -0.25, 0.00, 0.00, 349.94, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7998, 3243.60, 1959.00, 0.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3217.80, 1997.50, -0.29, 0.00, 0.00, 5.94, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3220.50, 1996.30, -0.25, 0.00, 0.00, 5.93, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3223.70, 1994.90, -0.21, 0.00, 0.00, 357.93, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3223.20, 2000.70, -0.25, 0.00, 0.00, 11.93, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3216.90, 2000.70, -0.19, 0.00, 0.00, 11.93, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3215.70, 2004.60, -0.21, 0.00, 0.00, 27.93, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3218.20, 2004.90, -0.25, 0.00, 0.00, 27.93, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3221.60, 2004.90, -0.23, 0.00, 0.00, 27.93, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3219.70, 2010.40, -0.25, 0.00, 0.00, 357.93, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3217.10, 2010.90, -0.25, 0.00, 0.00, 357.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3214.40, 2010.40, -0.25, 0.00, 0.00, 357.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3218.60, 2015.30, -0.17, 0.00, 0.00, 75.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3222.75, 2012.84, -0.19, 0.00, 0.00, 61.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3216.30, 2016.80, -0.19, 0.00, 0.00, 61.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3222.50, 2019.00, -0.25, 0.00, 0.00, 43.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3219.50, 2021.90, -0.25, 0.00, 0.00, 43.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3216.70, 2022.11, -0.29, 0.00, 0.00, 43.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3225.10, 2020.70, -0.25, 0.00, 0.00, 33.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3221.70, 2024.10, -0.25, 0.00, 0.00, 33.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3223.10, 2025.90, -0.15, 0.00, 0.00, 11.91, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3227.00, 2022.00, -0.17, 0.00, 0.00, 29.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3223.62, 2028.97, -0.21, 0.00, 0.00, 29.91, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3232.50, 2023.84, -0.19, 0.00, 0.00, 3.91, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3230.62, 2019.79, -0.23, 0.00, 0.00, 29.91, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3227.40, 2029.30, -0.25, 0.00, 0.00, 29.91, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3229.90, 2029.30, -0.25, 0.00, 0.00, 9.90, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3215.50, 1997.90, 2.90, 0.00, 0.00, 277.98, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3213.60, 2006.80, 2.90, 0.00, 0.00, 283.98, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3224.70, 1992.80, 2.90, 0.00, 0.00, 263.97, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3224.20, 2002.40, 2.90, 0.00, 0.00, 281.97, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3223.09, 1991.05, -0.29, 0.00, 0.00, 353.94, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3215.44, 1968.60, -0.21, 0.00, 0.00, 141.95, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3213.25, 2006.93, -0.29, 0.00, 0.00, 113.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3232.20, 2020.10, 2.90, 0.00, 0.00, 195.96, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3241.30, 2020.90, 2.90, 0.00, 0.00, 169.95, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3248.60, 2016.90, 2.90, 0.00, 0.00, 127.95, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3252.50, 2008.90, 2.90, 0.00, 0.00, 101.95, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3253.40, 2000.40, 2.90, 0.00, 0.00, 89.94, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3253.30, 1991.70, 2.90, 0.00, 0.00, 89.94, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3225.30, 2015.47, -0.29, 0.00, 0.00, 185.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3222.40, 2010.60, -0.25, 0.00, 358.00, 5.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3231.90, 2029.20, -0.25, 0.00, 0.00, 3.91, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3235.35, 2022.91, -0.31, 0.00, 0.00, 3.91, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3234.20, 2029.30, -0.21, 0.00, 0.00, 3.91, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3241.20, 2024.90, -0.25, 0.00, 0.00, 3.91, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3238.96, 2030.71, -0.31, 0.00, 0.00, 3.91, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3242.79, 2023.11, -0.31, 0.00, 0.00, 335.91, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3241.90, 2029.20, -0.19, 0.00, 0.00, 359.90, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3244.80, 2030.60, -0.25, 0.00, 0.00, 359.90, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3242.60, 2030.40, -0.25, 0.00, 0.00, 359.90, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3248.68, 2018.55, -0.37, 0.00, 0.00, 316.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3247.50, 2027.30, -0.21, 0.00, 0.00, 329.90, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3247.10, 2029.90, -0.25, 0.00, 0.00, 359.90, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3250.20, 2028.30, -0.31, 0.00, 0.00, 339.90, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3248.80, 2021.80, -0.17, 0.00, 0.00, 319.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3251.30, 2020.00, -0.25, 0.00, 0.00, 319.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3253.30, 2017.60, -0.25, 0.00, 0.00, 319.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3254.80, 2015.00, -0.25, 0.00, 0.00, 319.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3252.30, 2027.90, -0.25, 0.00, 0.00, 319.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3255.88, 2025.53, -0.31, 0.00, 0.00, 319.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3260.10, 2019.10, -0.17, 0.00, 0.00, 319.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3257.10, 2013.20, -0.25, 0.00, 0.00, 319.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3261.00, 2017.33, -0.13, 0.00, 0.00, 319.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3256.80, 2007.40, -0.25, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3254.55, 2010.16, -0.31, 0.00, 0.00, 113.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3253.44, 2020.46, -0.19, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3256.60, 1998.60, -0.29, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3256.40, 1995.90, -0.17, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3259.61, 1993.23, -0.11, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3263.70, 1992.40, -0.25, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3263.00, 1996.30, -0.19, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3263.10, 1998.50, -0.25, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3264.30, 2003.10, -0.15, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3264.00, 2005.90, -0.25, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3263.70, 2008.80, -0.25, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3261.33, 2008.67, -0.17, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3264.30, 2011.50, -0.25, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3265.30, 2012.30, -0.25, 0.00, 0.00, 319.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3262.95, 2019.39, -0.15, 0.00, 0.00, 319.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3262.70, 2016.30, -0.25, 0.00, 0.00, 319.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3261.82, 2013.71, -0.19, 0.00, 0.00, 319.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3212.90, 2015.40, 2.90, 0.00, 0.00, 267.97, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3222.00, 2030.10, 2.90, 0.00, 0.00, 219.96, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3230.60, 2032.80, 2.90, 0.00, 0.00, 183.96, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3240.10, 2033.60, 2.90, 0.00, 0.00, 183.96, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3249.50, 2032.10, 2.90, 0.00, 0.00, 153.96, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3257.30, 2027.20, 2.90, 0.00, 0.00, 139.96, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3264.20, 2020.70, 2.90, 0.00, 0.00, 127.96, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3267.70, 2012.90, 2.90, 0.00, 0.00, 101.95, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3267.80, 2005.30, 2.90, 0.00, 358.00, 89.95, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3267.20, 1996.00, 2.90, 0.00, 357.99, 89.94, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3260.61, 2001.84, -0.31, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3264.67, 1994.99, -0.29, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3216.30, 2024.20, 2.90, 0.00, 0.00, 233.96, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3224.60, 2011.60, 2.90, 0.00, 2.00, 255.96, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3256.30, 1990.40, -0.25, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3256.10, 1987.60, -0.25, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3255.50, 1984.40, -0.21, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3261.30, 1984.90, -0.25, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3262.90, 1987.80, -0.25, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3263.10, 1990.30, -0.19, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3263.60, 1980.60, -0.25, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3263.60, 1984.20, -0.19, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3255.00, 1980.70, -0.25, 0.00, 0.00, 245.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3260.40, 1978.40, -0.25, 0.00, 0.00, 245.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3257.86, 1980.00, -0.19, 0.00, 0.00, 231.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3259.20, 1974.30, -0.25, 0.00, 0.00, 231.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3251.90, 1983.40, 2.90, 0.00, 0.00, 71.94, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3247.80, 1975.80, 2.90, 0.00, 0.00, 49.94, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3241.20, 1970.20, 2.90, 0.00, 0.00, 31.94, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3236.00, 1963.90, 2.90, 0.00, 0.00, 73.94, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3235.00, 1954.50, 2.90, 0.00, 0.00, 97.93, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3238.70, 1946.60, 2.90, 0.00, 0.00, 137.93, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3243.60, 1938.90, 2.90, 0.00, 0.00, 109.93, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3242.30, 1930.70, 2.90, 0.00, 0.00, 53.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3235.90, 1927.70, 2.90, 0.00, 0.00, 7.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3266.60, 1987.30, 2.90, 0.00, 357.99, 81.94, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3264.50, 1978.80, 2.90, 0.00, 357.99, 69.94, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3260.30, 1971.30, 2.90, 0.00, 357.98, 49.94, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3253.70, 1964.60, 2.90, 0.00, 357.98, 41.94, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3250.80, 1957.00, 2.90, 0.00, 0.00, 95.93, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3253.80, 1948.90, 2.90, 0.00, 2.00, 127.93, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3258.30, 1941.10, 2.90, 0.00, 2.00, 113.93, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3259.20, 1932.10, 2.90, 0.00, 0.00, 69.93, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3255.10, 1923.80, 2.90, 0.00, 0.00, 55.93, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3249.40, 1916.80, 2.90, 0.00, 0.00, 33.93, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3241.90, 1913.40, 2.90, 0.00, 0.00, 5.93, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3233.80, 1912.70, 2.90, 0.00, 0.00, 5.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3228.70, 1928.50, 2.90, 0.00, 0.00, 337.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3221.70, 1931.80, 2.90, 0.00, 0.00, 333.91, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3213.00, 1934.70, 2.90, 0.00, 0.00, 351.91, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3204.60, 1934.60, 2.90, 0.00, 0.00, 7.91, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3196.70, 1930.30, 2.90, 0.00, 0.00, 45.90, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3190.70, 1923.30, 2.90, 0.00, 0.00, 53.90, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3187.20, 1919.40, 2.90, 0.00, 0.00, 47.90, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3179.50, 1915.30, 2.90, 0.00, 0.00, 3.90, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3171.70, 1916.10, 2.90, 0.00, 0.00, 343.90, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3165.90, 1921.70, 2.90, 0.00, 0.00, 285.90, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3163.80, 1945.20, 2.90, 0.00, 0.00, 263.90, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3225.80, 1912.40, 2.90, 0.00, 0.00, 353.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3218.50, 1914.60, 2.90, 0.00, 0.00, 329.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3214.00, 1917.10, 2.90, 0.00, 0.00, 331.91, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3206.40, 1916.60, 2.90, 0.00, 0.00, 31.91, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3200.80, 1911.50, 2.90, 0.00, 0.00, 45.91, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3194.30, 1905.90, 2.90, 0.00, 0.00, 29.91, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3187.20, 1901.60, 2.90, 0.00, 0.00, 23.91, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3178.50, 1899.80, 2.90, 0.00, 0.00, 1.90, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3170.50, 1901.20, 2.90, 0.00, 0.00, 339.90, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3163.10, 1905.20, 2.90, 0.00, 0.00, 321.90, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3157.40, 1911.10, 2.90, 0.00, 0.00, 297.90, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3153.90, 1918.20, 2.90, 0.00, 0.00, 297.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3151.30, 1925.80, 2.90, 0.00, 0.00, 275.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3150.20, 1934.20, 2.90, 0.00, 0.00, 275.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3150.50, 1941.70, 2.90, 0.00, 0.00, 261.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3151.30, 1950.00, 2.90, 0.00, 0.00, 259.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3261.40, 1983.00, -0.25, 0.00, 0.00, 269.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3259.30, 1976.00, -0.25, 0.00, 0.00, 241.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3256.78, 1972.43, -0.17, 0.00, 0.00, 217.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3252.30, 1976.10, -0.25, 0.00, 0.00, 229.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3255.62, 1969.11, -0.21, 0.00, 0.00, 229.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3256.02, 1974.63, -0.29, 0.00, 0.00, 229.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3251.00, 1975.43, -0.31, 0.00, 0.00, 229.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3248.00, 1970.30, -0.25, 0.00, 0.00, 229.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3250.20, 1965.10, -0.25, 0.00, 0.00, 229.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3246.40, 1968.50, -0.25, 0.00, 0.00, 229.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3244.40, 1966.90, -0.19, 0.00, 0.00, 229.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3248.20, 1964.30, -0.21, 0.00, 0.00, 229.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3242.40, 1966.60, -0.25, 0.00, 0.00, 229.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3239.60, 1965.60, -0.25, 0.00, 0.00, 229.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3244.00, 1962.50, -0.25, 0.00, 0.00, 229.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3247.90, 1962.00, -0.25, 0.00, 0.00, 229.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3246.70, 1959.00, -0.25, 0.00, 0.00, 229.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3242.10, 1960.30, -0.17, 0.00, 0.00, 229.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3238.60, 1963.10, -0.13, 0.00, 0.00, 229.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3238.80, 1960.90, -0.25, 0.00, 0.00, 259.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3239.20, 1957.90, -0.25, 0.00, 0.00, 259.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3239.10, 1955.20, -0.21, 0.00, 0.00, 259.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3243.60, 1956.50, -0.19, 0.00, 0.00, 259.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3247.70, 1956.10, -0.25, 0.00, 0.00, 259.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3245.80, 1953.50, -0.15, 0.00, 0.00, 259.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3247.80, 1951.80, -0.19, 0.00, 0.00, 259.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3249.50, 1949.00, -0.25, 0.00, 0.00, 279.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3244.60, 1948.90, -0.25, 0.00, 0.00, 279.87, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3240.60, 1952.50, -0.25, 0.00, 0.00, 279.87, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3239.80, 1953.80, -0.25, 0.00, 0.00, 279.87, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3240.40, 1950.00, -0.25, 0.00, 0.00, 279.87, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3243.92, 1941.81, -0.17, 0.00, 0.00, 295.87, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3246.30, 1943.60, -0.25, 0.00, 0.00, 307.87, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3245.50, 1944.90, -0.13, 0.00, 0.00, 307.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3250.80, 1946.70, -0.25, 0.00, 0.00, 291.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3252.50, 1943.90, -0.25, 0.00, 0.00, 291.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3247.20, 1940.60, -0.25, 0.00, 0.00, 291.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3254.40, 1941.80, -0.15, 0.00, 0.00, 291.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3248.30, 1938.30, -0.15, 0.00, 0.00, 291.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3255.70, 1939.10, -0.25, 0.00, 0.00, 291.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3257.40, 1936.90, -0.25, 0.00, 0.00, 291.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3163.80, 1936.10, 2.90, 0.00, 0.00, 279.90, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3164.50, 1929.40, 2.90, 0.00, 0.00, 279.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3164.70, 1954.40, 2.90, 0.00, 0.00, 263.90, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3248.80, 1935.50, -0.25, 0.00, 0.00, 275.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3251.50, 1934.80, -0.25, 0.00, 0.00, 275.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3256.50, 1934.40, -0.25, 0.00, 0.00, 275.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3247.40, 1933.10, -0.19, 0.00, 0.00, 275.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3252.10, 1931.60, -0.21, 0.00, 0.00, 275.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3255.40, 1932.00, -0.25, 0.00, 0.00, 275.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3252.90, 1938.20, -0.17, 0.00, 0.00, 275.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3225.60, 1916.10, -0.21, 0.00, 0.00, 173.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3246.80, 1930.00, -0.25, 0.00, 0.00, 275.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3253.80, 1928.80, -0.25, 0.00, 0.00, 275.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3251.55, 1921.15, -0.19, 0.00, 0.00, 263.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3245.77, 1923.79, -0.21, 0.00, 0.00, 263.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3245.20, 1928.00, -0.25, 0.00, 0.00, 263.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3243.83, 1926.92, -0.29, 0.00, 0.00, 263.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3249.10, 1919.60, -0.25, 0.00, 0.00, 263.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3240.00, 1925.80, -0.25, 0.00, 0.00, 123.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3240.10, 1922.40, -0.17, 0.00, 0.00, 123.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3241.10, 1918.40, -0.25, 0.00, 0.00, 123.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3246.40, 1916.50, -0.25, 0.00, 0.00, 123.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3243.20, 1916.50, -0.25, 0.00, 0.00, 123.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3240.20, 1916.30, -0.19, 0.00, 0.00, 123.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3237.10, 1922.10, -0.25, 0.00, 0.00, 123.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3236.00, 1926.30, -0.21, 0.00, 0.00, 93.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3234.73, 1925.20, -0.29, 0.00, 0.00, 93.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3234.50, 1918.10, -0.21, 0.00, 0.00, 93.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3234.10, 1914.60, -0.25, 0.00, 0.00, 93.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3230.40, 1914.50, -0.25, 0.00, 0.00, 93.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3229.20, 1916.10, -0.19, 0.00, 0.00, 93.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3230.10, 1925.10, -0.25, 0.00, 0.00, 93.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3236.30, 1915.70, -0.25, 0.00, 0.00, 93.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3229.00, 1926.60, -0.15, 0.00, 0.00, 71.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3224.30, 1923.20, -0.17, 0.00, 0.00, 71.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3225.28, 1920.22, -0.13, 0.00, 0.00, 71.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3224.50, 1928.20, -0.25, 0.00, 0.00, 71.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3223.80, 1925.30, -0.25, 0.00, 0.00, 71.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3224.70, 1917.10, -0.25, 0.00, 0.00, 71.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3222.60, 1914.90, -0.25, 0.00, 0.00, 71.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3219.30, 1918.10, -0.15, 0.00, 0.00, 71.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3220.00, 1923.10, -0.25, 0.00, 0.00, 71.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3220.60, 1927.00, -0.25, 0.00, 0.00, 71.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3216.00, 1922.90, -0.29, 0.00, 0.00, 71.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3215.40, 1918.00, -0.21, 0.00, 0.00, 71.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3164.90, 1963.00, 2.90, 0.00, 0.00, 273.90, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3164.50, 1971.30, 2.90, 0.00, 0.00, 267.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3164.00, 1978.90, 2.90, 0.00, 0.00, 275.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18450, 3156.50, 2020.50, 2.10, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3149.00, 1976.70, 2.90, 0.00, 0.00, 275.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3149.90, 1967.70, 2.90, 0.00, 0.00, 275.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3151.10, 1959.30, 2.90, 0.00, 0.00, 281.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3225.06, 1926.19, -0.21, 0.00, 0.00, 86.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3216.50, 1931.40, -0.25, 0.00, 0.00, 71.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3214.00, 1925.80, -0.25, 0.00, 0.00, 71.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3211.70, 1924.90, -0.25, 0.00, 0.00, 71.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3211.50, 1924.80, -0.25, 0.00, 0.00, 71.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3206.20, 1922.70, -0.19, 0.00, 0.00, 71.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3206.30, 1927.90, -0.25, 0.00, 0.00, 71.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3209.30, 1929.10, -0.19, 0.00, 0.00, 71.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3209.42, 1933.47, -0.21, 0.00, 0.00, 1.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3211.90, 1922.10, -0.17, 0.00, 0.00, 71.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3204.10, 1925.80, -0.25, 0.00, 0.00, 71.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3209.71, 1916.82, -0.25, 0.00, 0.00, 105.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3204.50, 1916.80, -0.25, 0.00, 0.00, 127.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3204.40, 1920.00, -0.21, 0.00, 0.00, 127.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3201.70, 1919.90, -0.23, 0.00, 0.00, 127.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3201.29, 1928.66, -0.17, 0.00, 0.00, 127.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3204.40, 1931.40, -0.25, 0.00, 0.00, 127.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3202.90, 1932.90, -0.25, 0.00, 0.00, 127.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3197.70, 1929.80, -0.25, 0.00, 0.00, 127.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3197.80, 1927.10, -0.19, 0.00, 0.00, 127.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3197.90, 1922.60, -0.25, 0.00, 0.00, 127.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3198.00, 1919.40, -0.25, 0.00, 0.00, 127.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3198.10, 1914.70, -0.25, 0.00, 0.00, 127.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3198.20, 1911.20, -0.15, 0.00, 0.00, 127.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3200.30, 1916.50, -0.25, 0.00, 0.00, 127.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3194.90, 1922.60, -0.25, 0.00, 0.00, 127.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3193.40, 1923.60, -0.21, 0.00, 0.00, 145.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3196.25, 1915.69, -0.13, 0.00, 0.00, 145.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3193.80, 1914.40, -0.25, 0.00, 0.00, 145.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3190.30, 1915.10, -0.21, 0.00, 0.00, 145.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3190.20, 1919.30, -0.19, 0.00, 0.00, 145.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3185.83, 1914.88, -0.23, 0.00, 0.00, 105.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3194.00, 1910.60, -0.25, 0.00, 0.00, 215.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3195.28, 1906.36, -0.19, 0.00, 0.00, 215.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3189.79, 1904.37, -0.31, 0.00, 0.00, 215.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3181.93, 1911.85, -0.05, 0.00, 0.00, 215.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3186.17, 1910.72, -0.25, 0.00, 0.00, 193.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3186.40, 1907.30, -0.17, 0.00, 0.00, 193.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3186.10, 1905.00, -0.25, 0.00, 0.00, 193.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3183.10, 1905.30, -0.25, 0.00, 0.00, 193.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3176.60, 1911.20, -0.25, 0.00, 0.00, 193.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3168.63, 1909.89, -0.13, 0.00, 0.00, 171.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3172.53, 1914.33, -0.29, 0.00, 0.00, 159.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3179.60, 1905.40, -0.19, 0.00, 0.00, 159.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3181.50, 1903.40, -0.25, 0.00, 0.00, 159.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3176.80, 1904.50, -0.25, 0.00, 0.00, 159.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3174.10, 1903.70, -0.21, 0.00, 0.00, 169.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3172.00, 1904.60, -0.25, 0.00, 0.00, 157.83, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3169.30, 1905.40, -0.25, 0.00, 0.00, 157.83, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3169.40, 1913.20, -0.25, 0.00, 2.00, 337.83, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3166.90, 1906.90, -0.25, 0.00, 2.00, 337.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3166.70, 1914.00, -0.25, 0.00, 2.00, 337.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3164.50, 1908.20, -0.25, 0.00, 2.00, 337.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3163.40, 1910.00, -0.25, 0.00, 2.00, 325.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3160.90, 1911.60, -0.25, 0.00, 2.00, 325.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3164.00, 1914.90, -0.25, 0.00, 2.00, 325.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3163.40, 1917.50, -0.25, 0.00, 2.00, 305.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3159.60, 1913.10, -0.25, 0.00, 2.00, 305.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3158.40, 1915.80, -0.25, 0.00, 358.00, 287.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3163.50, 1920.20, -0.25, 0.00, 357.99, 287.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3158.20, 1918.70, -0.25, 0.00, 357.99, 287.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3156.90, 1920.30, -0.25, 0.00, 357.99, 287.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3155.90, 1922.60, -0.25, 0.00, 357.99, 287.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3162.70, 1922.00, -0.25, 0.00, 357.99, 287.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3162.70, 1922.00, -0.25, 0.00, 357.99, 287.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3162.30, 1923.80, -0.25, 0.00, 357.99, 287.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3161.40, 1926.70, -0.25, 0.00, 357.99, 287.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3161.20, 1931.60, -0.25, 0.00, 357.99, 287.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3159.90, 1928.30, -0.25, 0.00, 357.99, 287.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3155.50, 1924.00, -0.25, 0.00, 357.99, 287.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3154.30, 1927.80, -0.25, 0.00, 357.99, 287.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3155.00, 1931.40, -0.25, 0.00, 357.99, 287.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3161.50, 1933.90, -0.25, 0.00, 357.99, 287.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3160.60, 1937.00, -0.25, 0.00, 357.99, 287.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3154.40, 1934.10, -0.25, 0.00, 357.99, 287.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3154.60, 1937.60, -0.25, 0.00, 357.99, 287.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3154.00, 1940.40, -0.25, 0.00, 357.99, 287.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3160.20, 1939.30, -0.25, 0.00, 357.99, 287.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3160.10, 1942.80, -0.25, 0.00, 357.99, 269.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3153.60, 1942.90, -0.25, 0.00, 357.99, 269.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3153.80, 1945.90, -0.25, 0.00, 357.99, 261.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3160.50, 1945.50, -0.25, 0.00, 357.98, 261.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3160.70, 1948.50, -0.25, 0.00, 357.98, 261.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3154.20, 1948.80, -0.25, 0.00, 357.98, 261.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3154.60, 1951.50, -0.25, 0.00, 357.98, 261.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3160.60, 1950.90, -0.25, 0.00, 357.98, 261.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3161.50, 1954.10, -0.25, 0.00, 357.98, 261.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3155.30, 1954.40, -0.25, 0.00, 357.98, 261.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3155.50, 1958.10, -0.25, 0.00, 357.98, 261.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3162.20, 1956.70, -0.25, 0.00, 357.98, 261.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3155.50, 1957.10, -0.25, 0.00, 357.98, 261.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3154.90, 1960.60, -0.25, 0.00, 357.98, 279.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3161.30, 1959.00, -0.25, 0.00, 357.98, 279.81, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3161.60, 1962.10, -0.25, 0.00, 357.98, 279.81, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3161.30, 1964.80, -0.25, 0.00, 357.98, 279.81, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3154.30, 1964.00, -0.25, 0.00, 357.98, 279.81, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3154.70, 1962.00, -0.25, 0.00, 357.98, 279.81, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3153.70, 1966.60, -0.25, 0.00, 357.98, 279.81, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3160.70, 1967.40, -0.25, 0.00, 357.98, 279.81, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3160.90, 1972.60, -0.25, 0.00, 357.98, 269.81, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3160.70, 1978.90, -0.25, 0.00, 357.98, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3153.80, 1969.50, -0.25, 0.00, 357.98, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3153.70, 1972.30, -0.25, 0.00, 357.98, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3154.00, 1975.70, -0.25, 0.00, 357.98, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3153.60, 1978.70, -0.25, 0.00, 357.98, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3154.80, 2061.80, -0.21, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3152.30, 2061.80, -0.25, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3148.70, 2063.10, 2.90, 0.00, 0.00, 275.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3163.50, 2064.90, 2.90, 0.00, 0.00, 275.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3161.50, 2073.10, 2.90, 0.00, 0.00, 291.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3156.10, 2080.40, 2.90, 0.00, 0.00, 317.89, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3148.80, 2084.80, 2.90, 0.00, 0.00, 343.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3140.30, 2085.20, 2.90, 0.00, 0.00, 7.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3132.00, 2082.90, 2.90, 0.00, 0.00, 25.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3124.70, 2077.70, 2.90, 0.00, 0.00, 45.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3119.90, 2070.00, 2.90, 0.00, 0.00, 67.87, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3118.00, 2061.50, 2.90, 0.00, 0.00, 85.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7998, 3150.20, 2102.70, 0.00, 0.00, 0.00, 270.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3145.20, 2071.30, 2.90, 0.00, 0.00, 315.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3138.30, 2072.30, 2.90, 0.00, 0.00, 33.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3131.60, 2067.00, 2.90, 0.00, 0.00, 51.88, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3128.60, 2058.70, 2.90, 0.00, 0.00, 87.87, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3128.90, 2049.90, 2.90, 0.00, 0.00, 95.87, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3128.50, 2041.30, 2.90, 0.00, 0.00, 75.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3125.90, 2033.30, 2.90, 0.00, 0.00, 65.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7998, 3083.30, 1953.20, 0.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3117.50, 2052.30, 2.90, 0.00, 0.00, 85.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3115.60, 2043.00, 2.90, 0.00, 0.00, 71.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3109.90, 2036.10, 2.90, 0.00, 0.00, 27.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3102.00, 2035.70, 2.90, 0.00, 0.00, 335.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3094.30, 2040.60, 2.90, 0.00, 0.00, 319.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3090.10, 2048.60, 2.90, 0.00, 2.00, 279.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3079.70, 2046.50, 2.90, 0.00, 0.00, 271.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3113.90, 2022.40, 2.90, 0.00, 0.00, 11.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3104.90, 2022.30, 2.90, 0.00, 0.00, 347.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3096.20, 2025.00, 2.90, 0.00, 0.00, 329.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3089.10, 2030.80, 2.90, 0.00, 0.00, 311.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3082.80, 2037.70, 2.90, 0.00, 2.00, 309.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3089.80, 2056.10, 2.90, 0.00, 2.00, 265.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3079.50, 2053.90, 2.90, 0.00, 2.00, 265.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3079.90, 2062.00, 2.90, 0.00, 2.00, 265.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3090.00, 2065.10, 2.90, 0.00, 0.00, 265.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3080.30, 2071.30, 2.90, 0.00, 2.00, 265.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3090.80, 2073.80, 2.90, 0.00, 2.00, 265.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3159.80, 2061.40, -0.29, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3159.60, 2064.50, -0.25, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3152.50, 2067.10, -0.25, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3159.40, 2066.80, -0.29, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3152.10, 2070.10, -0.19, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3148.50, 2071.20, -0.21, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3153.50, 2074.70, -0.25, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3156.97, 2072.11, -0.17, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3157.60, 2074.30, -0.21, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3147.60, 2074.10, -0.25, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3144.00, 2075.20, -0.25, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3140.10, 2078.70, -0.25, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3144.10, 2078.20, -0.25, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3149.00, 2077.50, -0.25, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3155.90, 2076.90, -0.27, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3154.20, 2079.60, -0.25, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3149.40, 2080.20, -0.29, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3150.70, 2082.00, -0.23, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3144.50, 2081.40, -0.21, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3144.50, 2084.50, -0.25, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3138.40, 2081.40, -0.21, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3137.80, 2084.00, -0.25, 0.00, 0.00, 269.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3138.00, 2073.90, -0.25, 0.00, 0.00, 303.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3133.80, 2071.60, -0.21, 0.00, 0.00, 303.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3137.20, 2076.00, -0.29, 0.00, 0.00, 303.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3133.40, 2081.60, -0.27, 0.00, 0.00, 303.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3131.40, 2078.70, -0.25, 0.00, 0.00, 303.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3131.60, 2072.10, -0.29, 0.00, 0.00, 303.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3127.80, 2075.90, -0.23, 0.00, 0.00, 303.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3127.60, 2078.80, -0.27, 0.00, 0.00, 303.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3123.30, 2074.00, -0.29, 0.00, 0.00, 327.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3125.10, 2071.30, -0.25, 0.00, 0.00, 327.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3126.80, 2068.80, -0.19, 0.00, 0.00, 327.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3128.80, 2067.00, -0.25, 0.00, 0.00, 327.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3127.30, 2061.10, -0.29, 0.00, 0.00, 357.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3127.00, 2055.20, -0.25, 0.00, 0.00, 357.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3123.80, 2065.00, -0.21, 0.00, 0.00, 357.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3122.10, 2068.90, -0.25, 0.00, 0.00, 357.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3120.10, 2066.90, -0.25, 0.00, 0.00, 357.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3125.40, 2060.30, -0.25, 0.00, 0.00, 357.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3121.10, 2061.70, -0.29, 0.00, 0.00, 357.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3118.90, 2061.20, -0.19, 0.00, 0.00, 357.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3120.50, 2057.30, -0.21, 0.00, 0.00, 357.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3118.60, 2053.20, -0.25, 0.00, 0.00, 357.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3123.40, 2051.60, -0.25, 0.00, 0.00, 357.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3121.80, 2052.40, -0.19, 0.00, 0.00, 357.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3127.10, 2050.20, -0.25, 0.00, 0.00, 357.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3125.30, 2048.90, -0.21, 0.00, 0.00, 357.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3118.60, 2047.30, -0.25, 0.00, 0.00, 357.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3123.50, 2045.90, -0.15, 0.00, 0.00, 357.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3127.20, 2044.60, -0.25, 0.00, 0.00, 357.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3125.50, 2041.60, -0.17, 0.00, 0.00, 357.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3123.00, 2041.30, -0.25, 0.00, 0.00, 337.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3125.50, 2037.30, -0.25, 0.00, 0.00, 337.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3122.80, 2037.00, -0.25, 0.00, 0.00, 337.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3117.40, 2042.20, -0.21, 0.00, 0.00, 337.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3119.60, 2040.90, -0.19, 0.00, 0.00, 337.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3120.10, 2036.70, -0.21, 0.00, 0.00, 337.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3113.70, 2037.70, -0.23, 0.00, 0.00, 319.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3115.70, 2035.40, -0.25, 0.00, 0.00, 319.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3118.50, 2033.00, -0.25, 0.00, 0.00, 319.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3110.60, 2034.60, -0.25, 0.00, 0.00, 295.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3116.60, 2032.80, -0.19, 0.00, 0.00, 295.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3115.40, 2027.70, -0.25, 0.00, 0.00, 295.79, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3119.60, 2026.90, -0.25, 0.00, 0.00, 307.78, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3114.80, 2024.50, -0.25, 0.00, 0.00, 281.78, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3111.90, 2026.50, -0.21, 0.00, 0.00, 275.78, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3104.10, 2033.60, -0.25, 0.00, 0.00, 261.77, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3106.70, 2029.60, -0.23, 0.00, 0.00, 261.77, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3106.50, 2026.30, -0.25, 0.00, 0.00, 261.77, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3107.00, 2023.20, -0.25, 0.00, 0.00, 261.77, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3101.30, 2025.30, -0.25, 0.00, 0.00, 241.77, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3101.70, 2028.60, -0.17, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3100.50, 2031.20, -0.19, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3096.80, 2029.90, -0.25, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3098.80, 2035.50, -0.25, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3096.10, 2033.00, -0.21, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3090.30, 2030.60, -0.19, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3094.00, 2039.40, -0.23, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3090.90, 2036.10, -0.27, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3088.30, 2035.10, -0.23, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3089.50, 2040.40, -0.21, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3085.50, 2039.70, -0.25, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3083.70, 2038.20, -0.25, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3088.40, 2044.40, -0.25, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3085.00, 2042.60, -0.17, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3082.40, 2041.30, -0.25, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3082.60, 2043.80, -0.25, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3084.00, 2046.50, -0.25, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3086.70, 2049.10, -0.21, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3086.10, 2051.80, -0.25, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3085.80, 2055.50, -0.25, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3086.70, 2058.90, -0.21, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3081.65, 2063.54, -0.21, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3083.70, 2061.10, -0.25, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3082.40, 2058.10, -0.25, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3081.60, 2055.20, -0.25, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3081.70, 2049.50, -0.19, 0.00, 0.00, 241.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3082.08, 2067.00, -0.29, 0.00, 0.00, 341.77, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3084.80, 2068.00, -0.25, 0.00, 0.00, 341.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3088.10, 2068.50, -0.29, 0.00, 0.00, 341.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3082.70, 2072.90, -0.25, 0.00, 0.00, 341.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3088.90, 2074.10, -0.31, 0.00, 0.00, 341.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3088.60, 2066.20, -0.28, 0.00, 0.00, 357.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3082.20, 2076.50, -0.27, 0.00, 0.00, 341.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3086.00, 2077.50, -0.19, 0.00, 0.00, 261.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3089.20, 2078.10, -0.29, 0.00, 0.00, 15.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3084.30, 2083.00, -0.21, 0.00, 0.00, 279.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3083.00, 2085.50, -0.25, 0.00, 0.00, 289.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3080.50, 2080.10, 2.90, 0.00, 2.00, 267.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3078.90, 2088.60, 2.90, 0.00, 2.00, 293.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3073.10, 2094.70, 2.90, 0.00, 2.00, 329.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3065.50, 2097.00, 2.90, 0.00, 2.00, 355.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3056.60, 2095.40, 2.90, 0.00, 2.00, 21.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3049.80, 2091.20, 2.90, 0.00, 2.00, 43.83, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3044.40, 2083.30, 2.90, 0.00, 4.00, 67.83, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3042.50, 2074.50, 2.90, 0.00, 3.99, 83.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3090.20, 2073.00, -0.29, 0.00, 0.00, 357.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3086.50, 2087.30, -0.19, 0.00, 0.00, 295.75, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3086.40, 2089.90, -0.21, 0.00, 0.00, 295.75, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3077.50, 2094.50, -0.25, 0.00, 0.00, 295.75, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3090.40, 2082.80, 2.90, 0.00, 2.00, 277.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3087.70, 2091.60, 2.90, 0.00, 2.00, 293.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3090.00, 2083.20, -0.25, 0.00, 0.00, 191.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3083.70, 2093.70, -0.25, 0.00, 0.00, 295.75, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3081.40, 2096.70, -0.19, 0.00, 0.00, 315.75, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3080.20, 2099.20, -0.25, 0.00, 0.00, 315.75, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3074.80, 2097.20, -0.29, 0.00, 0.00, 321.75, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3077.60, 2102.30, -0.21, 0.00, 0.00, 333.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3074.80, 2103.10, -0.19, 0.00, 0.00, 333.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3070.60, 2099.60, -0.21, 0.00, 0.00, 333.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3072.30, 2103.90, -0.29, 0.00, 0.00, 333.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3068.30, 2099.70, -0.27, 0.00, 0.00, 349.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3070.30, 2103.60, -0.31, 0.00, 0.00, 349.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3065.50, 2100.20, -0.25, 0.00, 0.00, 349.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3067.90, 2103.90, -0.29, 0.00, 0.00, 349.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3065.60, 2103.80, -0.23, 0.00, 0.00, 349.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3062.70, 2100.20, -0.27, 0.00, 0.00, 5.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3059.80, 2100.40, -0.21, 0.00, 0.00, 5.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3060.90, 2103.00, -0.25, 0.00, 0.00, 5.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3058.50, 2102.30, -0.29, 0.00, 0.00, 5.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3056.30, 2099.10, -0.29, 0.00, 0.00, 21.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3053.80, 2097.70, -0.25, 0.00, 0.00, 29.73, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3051.10, 2100.10, -0.25, 0.00, 0.00, 29.73, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3048.20, 2098.30, -0.21, 0.00, 0.00, 29.73, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3050.80, 2096.20, -0.17, 0.00, 0.00, 29.73, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3048.50, 2094.00, -0.19, 0.00, 0.00, 39.73, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3046.00, 2096.50, -0.25, 0.00, 0.00, 39.73, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3044.60, 2094.40, -0.25, 0.00, 0.00, 49.73, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3046.00, 2092.50, -0.25, 0.00, 0.00, 49.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3044.70, 2090.10, -0.21, 0.00, 0.00, 49.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3042.20, 2091.80, -0.23, 0.00, 0.00, 49.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3041.80, 2084.80, -0.25, 0.00, 0.00, 77.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3039.30, 2087.70, -0.29, 0.00, 0.00, 77.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3040.30, 2081.60, -0.23, 0.00, 0.00, 77.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3039.00, 2078.30, -0.25, 0.00, 0.00, 77.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3036.00, 2076.10, -0.21, 0.00, 0.00, 77.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3038.10, 2075.30, -0.25, 0.00, 0.00, 83.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3038.50, 2072.10, -0.19, 0.00, 0.00, 83.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3035.80, 2073.50, -0.29, 0.00, 0.00, 83.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3039.20, 2068.40, -0.23, 0.00, 0.00, 91.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3083.90, 2098.20, 2.90, 0.00, 2.00, 307.83, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3083.40, 2098.60, 2.90, 0.00, 2.00, 307.83, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3077.50, 2103.50, 2.90, 0.00, 2.00, 327.83, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3069.60, 2106.70, 2.90, 0.00, 2.00, 349.83, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3053.10, 2105.60, 2.90, 0.00, 2.00, 25.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3046.10, 2100.50, 2.90, 0.00, 2.00, 41.81, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3040.60, 2094.50, 2.90, 0.00, 2.00, 53.81, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3035.90, 2087.30, 2.90, 0.00, 358.00, 63.81, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3032.90, 2078.50, 2.90, 0.00, 357.99, 81.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7998, 3044.90, 1953.30, 0.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3749, 3156.40, 2039.10, 8.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3037.70, 2083.00, -0.25, 0.00, 0.00, 77.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3032.70, 2069.40, 2.90, 0.00, 357.99, 97.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3036.30, 2069.00, -0.25, 0.00, 0.00, 91.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3037.00, 2066.60, -0.25, 0.00, 0.00, 101.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3037.30, 2063.90, -0.19, 0.00, 0.00, 101.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3039.90, 2065.50, -0.29, 0.00, 0.00, 101.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3040.20, 2063.30, -0.25, 0.00, 0.00, 101.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3038.80, 2061.90, -0.25, 0.00, 0.00, 113.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3043.30, 2057.90, -0.21, 0.00, 0.00, 113.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3045.50, 2055.20, -0.19, 0.00, 0.00, 125.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3047.70, 2052.80, -0.25, 0.00, 0.00, 137.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3050.70, 2051.90, -0.21, 0.00, 0.00, 161.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3054.60, 2051.60, -0.23, 0.00, 0.00, 179.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3058.20, 2051.50, -0.25, 0.00, 0.00, 185.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3061.70, 2052.10, -0.25, 0.00, 0.00, 185.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3066.00, 2052.48, -0.19, 0.00, 0.00, 185.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3068.50, 2052.60, -0.25, 0.00, 0.00, 185.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3063.90, 2047.50, -0.23, 0.00, 0.00, 185.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3066.90, 2047.40, -0.25, 0.00, 0.00, 185.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3069.40, 2047.20, -0.27, 0.00, 0.00, 185.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3061.90, 2046.80, -0.19, 0.00, 0.00, 185.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3058.40, 2046.40, -0.25, 0.00, 0.00, 185.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3040.40, 2055.20, -0.21, 0.00, 0.00, 135.69, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3039.30, 2057.80, -0.25, 0.00, 0.00, 119.69, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3048.80, 2050.00, -0.25, 0.00, 0.00, 161.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1655, 3066.10, 2051.70, 0.80, 0.00, 0.00, 278.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1655, 3066.70, 2047.70, 0.80, 0.00, 0.00, 278.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3055.10, 2046.10, -0.17, 0.00, 0.00, 185.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3045.50, 2048.00, -0.25, 0.00, 0.00, 149.69, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3043.90, 2065.50, 2.90, 0.00, 3.99, 107.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3049.00, 2058.00, 2.90, 0.00, 0.00, 135.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3058.00, 2055.50, 2.90, 0.00, 0.00, 185.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3059.50, 2043.00, 2.90, 0.00, 0.00, 185.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3040.50, 2051.40, 2.90, 0.00, 0.00, 135.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3035.00, 2059.30, 2.90, 0.00, 0.00, 107.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3047.70, 2045.90, 2.90, 0.00, 0.00, 149.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3050.60, 2044.30, 2.90, 0.00, 0.00, 155.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3094.70, 2050.30, -0.25, 0.00, 0.00, 187.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3093.60, 2056.20, -0.19, 0.00, 0.00, 187.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3096.60, 2056.40, -0.17, 0.00, 0.00, 187.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3097.89, 2049.24, -0.31, 0.00, 0.00, 187.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3096.40, 2046.60, 2.90, 0.00, 2.00, 185.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3095.60, 2059.40, 2.90, 0.00, 2.00, 185.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3099.60, 2050.60, -0.17, 0.00, 0.00, 205.76, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3099.80, 2055.40, -0.25, 0.00, 0.00, 205.75, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3102.00, 2052.40, -0.25, 0.00, 0.00, 219.75, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3103.90, 2054.40, -0.25, 0.00, 0.00, 227.75, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3102.20, 2057.50, -0.25, 0.00, 0.00, 229.75, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3105.20, 2056.80, -0.21, 0.00, 0.00, 237.75, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3106.00, 2059.20, -0.25, 0.00, 0.00, 247.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3103.62, 2063.90, -0.15, 0.00, 0.00, 257.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3107.40, 2064.50, -0.23, 0.00, 0.00, 257.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3105.10, 2065.50, -0.25, 0.00, 0.00, 257.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3105.50, 2068.30, -0.21, 0.00, 0.00, 265.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3108.30, 2067.10, -0.29, 0.00, 0.00, 265.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3104.10, 2049.80, 2.90, 0.00, 2.00, 223.85, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3109.20, 2057.50, 2.90, 0.00, 2.00, 249.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3111.80, 2066.90, 2.90, 0.00, 2.00, 261.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3100.90, 2064.50, 2.90, 0.00, 2.00, 261.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3102.20, 2074.00, 2.90, 0.00, 2.00, 261.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3108.80, 2070.30, -0.19, 0.00, 0.00, 265.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3105.10, 2070.80, -0.25, 0.00, 0.00, 265.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3105.50, 2073.00, -0.25, 0.00, 0.00, 265.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3109.20, 2072.70, -0.25, 0.00, 0.00, 265.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3110.40, 2075.40, -0.25, 0.00, 0.00, 255.74, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3105.70, 2075.70, -0.31, 0.00, 0.00, 255.73, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3111.60, 2078.10, -0.25, 0.00, 0.00, 241.73, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3112.50, 2080.20, -0.19, 0.00, 0.00, 229.73, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3115.80, 2081.53, -0.21, 0.00, 0.00, 213.73, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3116.70, 2084.10, -0.25, 0.00, 0.00, 203.73, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3118.50, 2085.70, -0.17, 0.00, 0.00, 193.73, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3120.90, 2086.80, -0.19, 0.00, 0.00, 179.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3114.80, 2075.60, 2.90, 0.00, 2.00, 247.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3120.80, 2082.00, 2.90, 0.00, 2.00, 211.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3105.00, 2081.60, 2.90, 0.00, 0.00, 233.83, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3109.50, 2082.00, -0.25, 0.00, 0.00, 227.73, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3112.00, 2083.50, -0.17, 0.00, 0.00, 217.73, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3110.52, 2088.51, -0.19, 0.00, 0.00, 217.73, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3119.10, 2092.60, 2.90, 0.00, 0.00, 211.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3120.00, 2088.50, -0.25, 0.00, 0.00, 179.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3115.90, 2088.50, -0.25, 0.00, 0.00, 303.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3122.60, 2092.10, -0.25, 0.00, 0.00, 303.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3126.07, 2091.07, -0.21, 0.00, 0.00, 303.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3130.25, 2093.88, -0.25, 0.00, 0.00, 303.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3132.70, 2090.30, -0.21, 0.00, 0.00, 303.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3138.50, 2093.30, -0.25, 0.00, 0.00, 285.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3144.50, 2093.40, -0.25, 0.00, 358.00, 271.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3144.80, 2096.40, -0.25, 0.00, 357.99, 271.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3143.50, 2099.60, -0.25, 0.00, 357.99, 271.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3140.20, 2096.50, -0.25, 0.00, 357.99, 271.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3134.30, 2094.80, -0.25, 0.00, 357.99, 271.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3132.10, 2097.20, -0.25, 0.00, 357.99, 271.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3136.20, 2098.60, -0.25, 0.00, 357.99, 271.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3136.70, 2099.10, -0.25, 0.00, 357.99, 271.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3127.20, 2097.50, 2.90, 0.00, 0.00, 211.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3135.80, 2100.90, 2.90, 0.00, 0.00, 191.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3145.30, 2101.80, 2.90, 0.00, 0.00, 183.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3128.80, 2086.10, 2.90, 0.00, 2.00, 201.83, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3135.60, 2089.80, 2.90, 0.00, 2.00, 209.83, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3144.30, 2092.20, 2.90, 0.00, 0.00, 179.83, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3148.60, 2097.80, -0.25, 0.00, 357.99, 11.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3149.90, 2094.20, -0.25, 0.00, 0.00, 349.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3150.40, 2097.60, -0.25, 0.00, 0.00, 349.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3153.10, 2096.80, -0.25, 0.00, 0.00, 349.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3152.30, 2093.90, -0.17, 0.00, 0.00, 349.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3154.79, 2092.33, -0.21, 0.00, 0.00, 339.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3152.30, 2090.70, 2.90, 0.00, 0.00, 151.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3159.50, 2085.50, 2.90, 0.00, 0.00, 135.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3164.70, 2078.00, 2.90, 0.00, 0.00, 115.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3168.90, 2069.50, 2.90, 0.00, 0.00, 115.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3170.60, 2065.90, 2.90, 0.00, 0.00, 115.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3186.10, 2065.80, 2.90, 0.00, 0.00, 115.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3182.20, 2073.80, 2.90, 0.00, 0.00, 115.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3178.50, 2081.80, 2.90, 0.00, 0.00, 115.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3174.80, 2088.40, 2.90, 0.00, 0.00, 127.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3168.20, 2094.60, 2.90, 0.00, 0.00, 149.82, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3160.10, 2098.80, 2.90, 0.00, 0.00, 157.81, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 3154.20, 2101.30, 2.90, 0.00, 0.00, 167.81, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3158.90, 2095.50, -0.19, 0.00, 0.00, 349.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3161.27, 2089.24, -0.25, 0.00, 0.00, 349.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3164.70, 2093.40, -0.31, 0.00, 0.00, 349.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3167.50, 2091.70, -0.25, 0.00, 0.00, 349.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3164.86, 2084.03, -0.21, 0.00, 0.00, 349.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3165.20, 2080.10, -0.25, 0.00, 0.00, 201.71, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3167.90, 2074.10, -0.25, 0.00, 0.00, 201.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3166.42, 2077.50, -0.21, 0.00, 0.00, 201.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3174.33, 2082.27, -0.19, 0.00, 0.00, 201.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3173.00, 2086.10, -0.25, 0.00, 0.00, 201.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3173.50, 2076.70, -0.25, 0.00, 0.00, 201.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3171.90, 2073.80, -0.19, 0.00, 0.00, 201.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3172.40, 2069.00, -0.25, 0.00, 0.00, 201.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3174.40, 2065.30, -0.25, 0.00, 0.00, 201.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3169.49, 2070.22, -0.21, 0.00, 0.00, 201.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3176.80, 2078.00, -0.25, 0.00, 0.00, 201.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3181.30, 2069.00, -0.25, 0.00, 0.00, 201.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3177.42, 2061.96, -0.21, 0.00, 0.00, 201.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3184.28, 2063.98, -0.29, 0.00, 0.00, 113.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3180.70, 2073.50, -0.25, 0.00, 0.00, 201.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3146.50, 2026.40, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3145.10, 2031.70, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3146.50, 2022.30, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3143.80, 2025.80, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3148.00, 2018.80, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3147.10, 2015.40, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3145.10, 2011.50, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3146.30, 2006.00, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3166.70, 2005.80, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3167.50, 2010.20, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3169.50, 2015.20, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3167.60, 2019.00, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3169.20, 2025.20, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3169.90, 2018.50, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3169.90, 2018.50, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3142.30, 2022.60, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3140.40, 2011.90, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3168.40, 1996.20, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3167.00, 2019.80, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3190.70, 2033.00, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3192.00, 2024.00, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3192.00, 2012.90, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3191.50, 2003.90, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3191.50, 2003.90, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16113, 3012.50, 2101.50, 0.00, 0.00, 0.00, 320.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16113, 3038.70, 2130.10, 0.00, 0.00, 6.00, 252.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16113, 3068.50, 2135.10, 0.00, 0.00, 6.00, 227.99, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16113, 3098.00, 2135.30, 0.00, 0.00, 6.00, 227.98, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16113, 3098.00, 2135.30, 0.00, 0.00, 6.00, 227.99, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16113, 3120.40, 2133.20, 0.00, 0.00, 6.00, 227.98, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3190.20, 1998.90, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3191.70, 1988.80, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3194.80, 1977.80, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3205.10, 1975.90, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3208.80, 1982.30, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3212.50, 1989.00, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3206.50, 1993.30, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3196.40, 1995.80, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3201.50, 1986.50, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(9237, 3234.90, 2012.80, 10.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1378, 3240.20, 1998.40, 26.40, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(13077, 3243.50, 2053.10, 7.00, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7998, 3243.00, 1958.50, -0.25, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7998, 3203.60, 1958.00, -0.25, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7998, 3157.10, 1959.20, -0.25, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7998, 3216.90, 1951.80, 0.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7998, 3044.90, 1953.30, -0.25, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7998, 3074.30, 1990.80, -0.25, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(17017, 3184.40, 1952.80, 54.20, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(17012, 3209.40, 1873.00, 8.10, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3202.40, 1956.80, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3201.29, 1946.28, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3171.60, 1968.80, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3217.40, 1956.80, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3198.00, 1955.20, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3211.70, 1953.00, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3208.70, 1957.70, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3228.88, 1951.11, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3224.30, 1978.70, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3224.30, 1978.70, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3231.50, 1968.60, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16113, 3006.50, 2066.00, 0.00, 0.00, 0.00, 346.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16113, 3023.10, 2036.80, 0.00, 0.00, 0.00, 346.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16113, 3052.80, 2023.20, 0.00, 0.00, 0.00, 346.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16113, 3090.30, 2007.20, 0.00, 0.00, 0.00, 14.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16113, 3113.60, 1967.80, 0.00, 0.00, 0.00, 328.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16113, 3129.30, 1929.90, 0.00, 0.00, 0.00, 327.99, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16113, 3137.00, 1902.40, 0.00, 0.00, 0.00, 327.99, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16113, 3272.90, 1958.80, 2.30, 0.00, 0.00, 327.99, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16113, 3283.10, 1924.00, 2.30, 0.00, 0.00, 133.99, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16113, 3269.30, 1898.40, 2.30, 0.00, 0.00, 111.99, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3146.20, 1989.40, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3146.00, 1965.50, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3144.50, 1969.70, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3141.40, 1984.10, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3142.60, 1997.20, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3144.90, 1977.80, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3145.90, 1953.90, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3140.30, 1990.00, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3142.70, 1960.70, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3167.90, 1948.90, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18248, 3178.82, 1937.22, 7.32, 0.00, 0.00, 194.38, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18248, 3218.67, 1935.15, 7.32, 0.00, 0.00, 75.61, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3189.06, 1957.10, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3185.46, 1932.43, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3194.40, 1937.49, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3231.24, 1962.44, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3221.50, 1965.40, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3232.36, 1952.02, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3234.38, 1946.03, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3237.20, 1941.48, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3238.86, 1934.81, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3230.33, 1931.71, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(650, 3225.66, 1933.95, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3221.05, 1937.45, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(744, 3230.83, 1941.77, 2.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3169.27, 2089.31, -0.23, 0.00, 0.00, 200.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18773, 3185.76, 1915.69, 0.01, 0.00, 1.94, 280.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // //沙漠漂移赛道 By:Yuki
    // //HUNT SRF 测试的OBJ


    // //狂野机场测试
    // //

    // // RemoveBuildingForPlayer(playerid, 3187, 26.4531, -2648.5703, 43.5078, 0.25, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(5191, -124.03, -2962.23, 40.11, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(5191, -123.97, -3065.88, 54.77, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18791, -117.41, -2866.23, 45.52, 0.00, 0.00, -111.24, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18796, -82.86, -2819.07, 44.50, 0.00, -5.00, -133.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18794, -56.99, -2790.22, 36.32, 0.00, 0.00, -129.78, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18791, -116.12, -2929.46, 61.14, 0.00, 0.00, -112.44, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18789, -123.90, -3054.09, 45.45, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(5191, -123.99, -3186.49, 54.77, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(5191, -124.03, -3186.49, 40.11, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3578, -49.90, -2791.46, 39.63, 0.00, 0.00, 67.08, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3578, -46.10, -2781.95, 39.64, 0.00, 0.00, 69.54, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3578, -42.50, -2772.33, 39.94, -10.00, 0.00, 69.54, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3578, -39.21, -2762.75, 40.28, -10.00, 0.06, 73.20, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3578, -36.60, -2752.96, 40.28, -10.00, 0.06, 76.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3578, -35.24, -2742.87, 40.28, -10.00, 0.06, 88.50, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19427, -62.99, -2783.45, 39.29, 90.00, 0.00, -42.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19427, -64.09, -2786.55, 39.29, 90.00, 0.00, -40.56, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19427, -61.85, -2783.92, 39.29, 90.00, 0.00, -40.56, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19427, -59.74, -2781.35, 39.29, 90.00, 0.00, -38.34, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19427, -60.64, -2780.90, 39.29, 90.00, 0.00, -42.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19427, -58.48, -2779.26, 38.33, 0.00, 0.00, -195.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19427, -58.89, -2779.10, 38.33, 0.00, 0.00, -228.54, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19427, -65.27, -2785.91, 39.29, 90.00, 0.00, -42.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19427, -65.97, -2787.75, 38.33, 0.00, 0.00, -145.86, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19428, -44.99, -2778.82, 41.00, 90.00, 0.00, -21.66, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19428, -46.23, -2782.05, 41.01, 90.00, 0.00, -20.46, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19428, -47.42, -2785.16, 41.00, 90.00, 0.00, -21.66, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19428, -48.74, -2788.39, 41.01, 90.00, 0.00, -22.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19428, -50.12, -2791.59, 41.01, 90.00, 0.00, -23.70, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19428, -51.38, -2794.50, 41.00, 90.00, 0.00, -23.20, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19428, -43.76, -2775.60, 41.01, 90.00, 0.00, -20.28, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19428, -42.58, -2772.33, 41.01, 90.00, 0.00, -19.26, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19428, -41.42, -2769.12, 41.00, 90.00, 0.00, -20.28, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19428, -40.35, -2765.89, 41.01, 90.00, 0.00, -16.62, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19428, -39.37, -2762.61, 41.00, 90.00, 0.00, -16.62, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19428, -38.41, -2759.35, 41.01, 90.00, 0.00, -16.62, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19428, -37.55, -2756.05, 41.00, 90.00, 0.00, -12.54, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19428, -36.74, -2752.68, 41.01, 90.00, 0.00, -13.38, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19428, -35.92, -2749.33, 41.00, 90.00, 0.00, -13.38, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19428, -35.47, -2745.93, 41.01, 90.00, 0.00, -1.50, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19428, -35.37, -2742.45, 41.00, 90.00, 0.00, -1.50, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19428, -35.29, -2739.47, 41.01, 90.00, 0.00, -1.50, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1237, -57.74, -2778.03, 38.88, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1237, -56.87, -2777.95, 38.88, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1237, -56.75, -2776.94, 38.88, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1237, -57.50, -2779.13, 38.88, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(5191, -124.00, -3307.94, 40.11, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(5191, -124.00, -3307.94, 54.77, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(5191, -124.00, -3429.95, 40.11, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(5191, -124.00, -3429.95, 54.77, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -128.93, -3520.55, 65.31, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -118.38, -3520.10, 65.31, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -118.35, -3520.05, 50.75, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -128.81, -3520.11, 50.75, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -128.89, -3570.46, 65.31, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -118.36, -3570.09, 65.31, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -118.38, -3569.94, 50.75, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -128.80, -3570.03, 50.75, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -128.88, -3620.25, 65.31, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -118.38, -3620.02, 65.31, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -118.46, -3619.66, 50.75, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -128.75, -3619.86, 50.75, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -118.58, -3669.53, 50.75, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -128.75, -3669.83, 50.75, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -118.41, -3669.96, 65.31, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -129.04, -3670.25, 65.31, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18753, -125.56, -3752.99, 45.33, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18824, -106.47, -3714.79, 65.29, 90.00, 0.00, 44.52, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18824, -66.97, -3715.15, 65.29, 90.00, 0.00, 134.52, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -54.81, -3671.85, 60.87, 0.00, -80.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -54.79, -3638.90, 55.00, 0.00, -80.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -192.69, -3636.07, 55.00, 0.00, -80.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18826, -70.70, -3604.56, 50.75, 90.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -86.68, -3640.08, 50.75, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -86.65, -3690.10, 50.75, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -192.69, -3669.31, 60.87, 0.00, -80.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18824, -141.29, -3714.87, 65.29, 90.00, 0.00, 134.52, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18824, -180.81, -3714.54, 65.29, 90.00, 0.00, 44.52, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18826, -176.83, -3601.72, 50.75, 90.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -160.96, -3637.25, 50.75, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -160.96, -3687.26, 50.75, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11693, -125.87, -3734.65, 0.06, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11694, -121.81, -3739.31, 14.76, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(4587, -103.38, -3791.01, 104.68, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(4141, -157.77, -3772.25, 56.70, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(14675, -155.60, -3770.33, 56.82, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18755, -119.80, -3800.24, 47.77, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(987, -133.15, -3495.31, 60.69, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(987, -114.36, -3494.60, 60.69, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(987, -133.15, -3495.31, 46.09, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(987, -114.36, -3494.60, 46.09, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -110.54, -3804.91, 135.68, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19454, -120.61, -3805.15, 135.68, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19454, -120.61, -3807.97, 135.67, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -110.54, -3796.24, 135.70, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19454, -120.61, -3796.19, 135.68, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19454, -120.61, -3793.17, 135.67, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -130.48, -3799.49, 135.67, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19362, -127.18, -3793.07, 135.68, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19362, -127.17, -3805.91, 135.68, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19362, -127.18, -3808.11, 135.68, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19362, -130.66, -3805.91, 135.68, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19435, -131.66, -3805.11, 135.68, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19435, -130.68, -3792.27, 135.68, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19435, -133.20, -3793.87, 135.68, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19435, -110.54, -3792.14, 54.34, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19435, -107.04, -3792.04, 54.34, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -100.04, -3805.02, 135.68, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -100.04, -3795.79, 135.67, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -89.53, -3810.13, 135.68, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -89.53, -3801.29, 135.68, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -89.53, -3791.66, 135.69, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -89.53, -3782.03, 135.69, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -79.03, -3781.25, 135.67, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -79.03, -3810.13, 135.68, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -79.03, -3800.50, 135.69, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -79.03, -3790.87, 135.68, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -81.31, -3771.74, 135.68, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -75.78, -3810.13, 135.67, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -75.78, -3800.60, 135.68, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -75.78, -3790.96, 135.70, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -75.78, -3781.34, 135.68, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -75.80, -3771.89, 135.67, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18788, -123.83, -2988.39, 60.06, 0.00, -1.50, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18791, -61.09, -2874.26, 61.14, 0.00, 0.00, -157.38, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18791, 176.08, -2850.95, 61.14, 0.00, 0.00, 24.80, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18794, 55.87, -2618.94, 38.83, 0.00, 15.00, -178.02, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18807, 91.45, -2617.70, 48.54, 0.00, 4.00, -178.02, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18788, -2.43, -2865.66, 60.58, 0.00, 0.00, -177.78, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18788, 234.44, -2734.46, 60.58, 0.00, 0.00, 92.26, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18788, 37.53, -2864.11, 60.58, 0.00, 0.00, -177.78, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18788, 77.53, -2862.58, 60.58, 0.00, 0.00, -177.78, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18808, -27.59, -2866.89, 35.80, 0.00, 0.00, -177.78, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18815, -27.59, -2866.89, -14.20, 0.00, 180.00, -177.78, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18788, 117.48, -2861.03, 60.58, 0.00, 0.00, -177.78, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16089, -124.15, -2901.36, 46.50, 0.00, 0.00, 88.62, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18791, 229.00, -2793.68, 61.14, 0.00, 0.00, 69.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18791, 224.44, -2675.84, 61.14, 0.00, 0.00, 114.66, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18791, 167.33, -2622.78, 61.14, 0.00, 0.00, 159.56, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18783, 48.72, -2621.66, 38.32, 0.00, 0.00, -178.24, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18783, 28.74, -2622.26, 38.32, 0.00, 0.00, -178.24, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18785, 29.36, -2642.25, 38.32, 0.00, 0.00, -178.24, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3437, 46.02, -2611.52, 40.54, 0.00, 90.00, 1.98, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3437, 34.31, -2611.92, 40.54, 0.00, 90.00, -178.02, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3437, 24.28, -2612.26, 40.54, 0.00, 90.00, -178.02, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3660, 19.49, -2622.36, 43.11, 0.00, 0.00, 91.98, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18808, 137.96, -2860.14, 35.80, 0.00, 0.00, -177.78, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18815, 137.96, -2860.14, -14.21, 0.00, 180.00, -168.56, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18808, 234.44, -2770.18, 35.80, 0.00, 0.00, -177.78, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18815, 234.44, -2770.18, -14.19, 0.00, 180.00, -174.54, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18808, 200.63, -2643.36, 35.80, 0.00, 0.00, -177.78, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18815, 200.63, -2643.36, -14.20, 0.00, 180.00, -178.24, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3749, 4.50, -2625.35, 44.92, 0.00, 0.00, 12.36, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1568, 15.49, -2631.53, 39.40, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3498, 15.20, -2634.30, 36.30, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3498, 15.57, -2638.82, 39.49, 0.00, 90.00, 94.44, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1215, 15.20, -2634.31, 40.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1216, 16.56, -2631.41, 40.07, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3877, 58.57, -2631.01, 42.48, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3337, 28.11, -2664.88, 40.09, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(967, 17.00, -2653.02, 39.50, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(966, 16.54, -2654.24, 39.51, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3498, 16.23, -2647.33, 39.49, 0.00, 90.00, 94.44, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3498, 16.57, -2651.81, 36.30, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3498, 15.57, -2638.82, 39.99, 0.00, 90.00, 94.44, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3498, 16.23, -2647.33, 39.99, 0.00, 90.00, 94.44, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3498, 15.57, -2638.82, 40.51, 0.00, 90.00, 94.44, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3498, 16.23, -2647.33, 40.51, 0.00, 90.00, 94.44, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1215, 16.57, -2651.82, 40.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1215, 16.41, -2649.55, 40.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1215, 16.18, -2646.88, 40.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1215, 15.96, -2643.91, 40.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1215, 15.71, -2640.82, 40.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1215, 15.48, -2637.78, 40.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1237, 24.05, -2650.67, 39.48, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1238, 16.76, -2662.43, 39.85, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2222, 15.08, -2631.06, 41.49, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1422, 19.86, -2663.76, 39.95, 0.00, 0.00, -30.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1423, 24.72, -2649.73, 40.18, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1427, 24.70, -2651.43, 39.99, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(968, 16.54, -2654.26, 40.28, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1422, 17.65, -2662.06, 39.95, 0.00, 0.00, -45.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1238, 21.42, -2664.43, 39.85, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1238, 16.74, -2661.86, 39.85, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1422, 35.06, -2663.86, 39.95, 0.00, 0.00, 30.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1422, 39.01, -2659.90, 39.95, 0.00, 0.00, 60.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1422, 37.27, -2662.16, 39.95, 0.00, 0.00, 45.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1238, 33.60, -2664.53, 39.85, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1238, 39.67, -2658.35, 39.85, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1423, 24.72, -2647.17, 40.18, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1423, 24.72, -2644.63, 40.18, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1423, 24.72, -2642.08, 40.18, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1423, 24.72, -2639.49, 40.18, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1423, 24.72, -2636.92, 40.42, 0.00, -13.80, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1423, 24.72, -2633.82, 41.19, 0.00, -13.80, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1423, 23.80, -2631.38, 41.51, 0.00, 0.00, -45.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1423, 21.96, -2629.52, 41.51, 0.00, 0.00, -45.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1237, 18.23, -2653.46, 39.48, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1568, 17.11, -2664.46, 39.40, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1568, 39.98, -2664.64, 39.40, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3585, 23.11, -2646.13, 40.34, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(958, 23.09, -2641.23, 40.34, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(959, 23.09, -2641.23, 40.34, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2304, 22.52, -2649.06, 39.42, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2294, 23.52, -2649.04, 39.43, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2130, 22.54, -2648.47, 39.43, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2067, 22.78, -2647.07, 39.56, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2632, 23.09, -2645.29, 39.51, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19325, 24.39, -2646.29, 39.91, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2606, 23.07, -2646.43, 41.67, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(2602, 21.44, -2647.20, 39.98, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11717, 23.73, -2646.05, 39.56, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1794, 22.79, -2646.19, 39.19, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1318, 28.01, -2664.74, 43.18, 90.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18816, 137.96, -2860.14, -64.21, 0.00, 0.00, -168.56, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18816, -27.59, -2866.89, -64.20, 0.00, 0.00, -177.78, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 220.99, -2656.75, 66.15, 0.00, 0.00, 37.08, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 226.73, -2666.18, 66.15, 0.00, 0.00, 29.34, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 232.19, -2677.58, 66.15, 0.00, 0.00, 15.54, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 236.36, -2689.73, 66.15, 0.00, 0.00, 13.08, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 239.01, -2702.28, 66.15, 0.00, 0.00, 7.50, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 240.32, -2714.49, 66.15, 0.00, 0.00, 7.50, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 240.82, -2726.06, 66.15, 0.00, 0.00, 7.50, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 240.82, -2726.06, 66.15, 0.00, 0.00, 7.50, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 241.19, -2736.50, 66.15, 0.00, 0.00, 7.50, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 241.62, -2746.82, 66.15, 0.00, 0.00, 0.84, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 241.81, -2756.90, 66.15, 0.00, 0.00, -3.66, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 241.55, -2766.65, 66.15, 0.00, 0.00, -0.72, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 240.31, -2776.04, 66.15, 0.00, 0.00, -7.32, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 238.35, -2785.38, 66.15, 0.00, 0.00, -16.44, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 235.66, -2794.68, 66.15, 0.00, 0.00, -16.44, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 231.91, -2803.37, 66.15, 0.00, 0.00, -26.40, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 227.67, -2811.89, 66.15, 0.00, 0.00, -32.22, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 222.60, -2820.23, 66.15, 0.00, 0.00, -32.22, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 216.65, -2827.66, 66.15, 0.00, 0.00, -38.58, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 209.89, -2835.10, 66.15, 0.00, 0.00, -47.16, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 202.85, -2841.72, 66.15, 0.00, 0.00, -47.16, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 195.18, -2847.44, 66.15, 0.00, 0.00, -55.08, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 187.11, -2852.48, 66.15, 0.00, 0.00, -61.68, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 178.67, -2857.08, 66.15, 0.00, 0.00, -67.50, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 169.76, -2860.46, 66.15, 0.00, 0.00, -71.04, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 160.63, -2863.26, 66.15, 0.00, 0.00, -76.20, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 151.40, -2865.38, 66.15, 0.00, 0.00, -85.50, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 214.25, -2647.81, 66.15, 0.00, 0.00, 42.96, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 206.43, -2639.62, 66.15, 0.00, 0.00, 42.96, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 197.54, -2632.18, 66.15, 0.00, 0.00, 49.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 188.36, -2625.90, 66.15, 0.00, 0.00, 58.92, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 178.60, -2620.55, 66.15, 0.00, 0.00, 65.04, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1226, 168.37, -2616.15, 66.15, 0.00, 0.00, 74.16, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -40.01, -3748.67, 50.75, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, -40.01, -3738.04, 50.75, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, 10.00, -3738.04, 50.75, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, 10.00, -3748.67, 50.75, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, 60.01, -3738.04, 50.75, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, 60.01, -3748.67, 50.75, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, 110.01, -3738.04, 50.75, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18809, 110.01, -3748.67, 50.75, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18789, 333.76, -3743.36, 45.61, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(5191, 200.14, -3743.20, 40.26, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(987, 135.16, -3733.20, 46.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(987, 146.99, -3753.40, 46.00, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18808, 269.14, -3743.38, 20.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18815, 269.14, -3743.38, -29.21, 0.00, 180.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18808, 408.56, -3743.24, 20.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18789, 483.76, -3743.36, 45.61, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18815, 408.56, -3743.24, -29.19, 0.00, 180.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18808, 558.78, -3743.28, 20.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18789, 633.76, -3743.36, 45.61, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18815, 558.78, -3743.28, -29.20, 0.00, 180.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18808, 707.71, -3743.31, 20.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18789, 783.76, -3743.36, 45.61, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18815, 707.71, -3743.31, -29.21, 0.00, 180.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18808, 859.64, -3743.43, 20.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18789, 933.75, -3743.36, 45.61, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18815, 859.64, -3743.43, -29.18, 0.00, 180.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18789, 1083.75, -3743.36, 45.61, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18808, 1008.63, -3743.38, 20.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18815, 1008.63, -3743.38, -29.21, 0.00, 180.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18808, 1142.44, -3743.47, 20.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18788, 1178.74, -3743.36, 45.05, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18788, 1218.74, -3743.36, 45.05, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18815, 1142.44, -3743.47, -29.19, 0.00, 180.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18791, 1277.72, -3735.65, 45.61, 0.00, 0.00, 22.44, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18791, 1332.89, -3680.57, 45.61, 0.00, 0.00, 67.46, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18808, 1273.67, -3736.87, 20.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18815, 1273.67, -3736.87, -29.21, 0.00, 180.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18808, 1340.43, -3641.84, 20.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18789, 1340.67, -3566.60, 45.60, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18815, 1340.43, -3641.84, -29.19, 0.00, 180.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18808, 1340.62, -3490.81, 20.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18789, 1340.67, -3416.60, 45.60, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18815, 1340.62, -3490.81, -29.19, 0.00, 180.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18808, 1340.65, -3341.58, 20.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18789, 1340.67, -3266.60, 45.60, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18815, 1340.65, -3341.58, -29.20, 0.00, 180.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18808, 1340.65, -3191.41, 20.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18789, 1340.67, -3116.60, 45.60, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18815, 1340.65, -3191.41, -29.21, 0.00, 180.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18808, 1340.69, -3041.99, 20.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18789, 1340.67, -2966.60, 45.60, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18815, 1340.69, -3041.99, -29.20, 0.00, 180.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18808, 1340.60, -2892.09, 20.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18789, 1340.67, -2816.60, 45.60, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18815, 1340.60, -2892.09, -29.21, 0.00, 180.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18789, 1340.67, -2666.60, 45.60, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18808, 1340.73, -2740.63, 20.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18788, 1340.67, -2571.61, 45.04, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18802, 1340.67, -2428.95, 12.78, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18807, 1340.67, -2514.12, 35.60, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18794, 1340.67, -2459.28, 17.97, 0.00, 25.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19130, 1349.41, -2541.53, 12.35, 0.00, 90.00, -135.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19130, 1344.57, -2541.32, 12.35, 0.00, 90.00, -135.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19130, 1349.42, -2552.68, 12.35, 0.00, 90.00, -135.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19130, 1344.26, -2552.99, 12.35, 0.00, 90.00, -135.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19132, 1360.04, -2512.28, 12.35, 0.00, 90.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19132, 1355.19, -2511.81, 12.35, 0.00, 90.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19130, 1354.00, -2534.62, 12.35, 0.00, 90.00, -135.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19130, 1348.38, -2531.28, 12.35, 0.00, 90.00, -135.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1319, 1352.37, -2510.00, 12.93, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7033, 1338.93, -2401.19, 17.05, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(4597, 1340.73, -2424.17, 12.96, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1238, 1349.98, -2512.35, 12.67, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1237, 1351.34, -2511.08, 12.37, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1422, 1339.05, -2517.70, 12.95, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1423, 1347.28, -2540.97, 13.08, 0.00, 0.00, 59.52, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1424, 1346.66, -2543.25, 12.90, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1427, 1352.26, -2510.56, 12.90, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3330, 1339.18, -2500.76, 25.42, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(978, 1346.73, -2507.85, 13.21, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1237, 1348.41, -2513.23, 12.37, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1238, 1346.86, -2514.41, 12.67, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1237, 1345.46, -2515.29, 12.37, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1238, 1343.84, -2516.40, 12.67, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1237, 1342.05, -2517.51, 12.37, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1427, 1341.14, -2517.80, 12.98, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1427, 1346.64, -2563.63, 12.90, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1423, 1348.66, -2538.54, 13.08, 0.00, 0.00, 59.52, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1423, 1350.01, -2536.14, 13.08, 0.00, 0.00, 59.52, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1423, 1351.37, -2533.89, 13.08, 0.00, 0.00, 59.52, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1423, 1352.60, -2531.62, 13.08, 0.00, 0.00, 61.98, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1423, 1353.74, -2529.26, 13.08, 0.00, 0.00, 64.14, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(979, 1331.80, -2507.85, 13.21, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1424, 1346.66, -2545.37, 12.90, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1424, 1346.66, -2547.43, 12.90, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1424, 1346.66, -2549.53, 12.90, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1424, 1346.66, -2551.61, 12.90, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1424, 1346.66, -2553.71, 12.90, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1424, 1346.66, -2555.81, 12.90, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1424, 1346.66, -2557.90, 12.90, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1424, 1346.66, -2559.98, 12.90, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1424, 1346.66, -2562.08, 12.90, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1427, 1337.18, -2517.75, 12.98, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1422, 1335.06, -2517.62, 12.77, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1422, 1332.09, -2517.59, 12.77, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1422, 1329.02, -2517.59, 12.77, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1422, 1327.15, -2515.72, 12.77, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1422, 1327.15, -2512.59, 12.77, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1422, 1327.14, -2509.35, 12.77, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1238, 1327.11, -2517.69, 12.67, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10816, -150.44, -3093.20, 22.06, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10816, -150.44, -3472.28, 22.02, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11694, -18.50, -3556.31, -4.16, 0.00, 0.00, -98.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11694, -4.11, -3421.17, -3.98, 0.00, 0.00, -171.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11694, -26.17, -3195.45, -4.00, 0.00, 0.00, -193.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11694, -22.45, -3027.68, -4.00, 0.00, 0.00, -280.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(4726, 31.11, -3510.92, 26.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3816, -123.76, -3304.81, 31.62, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3816, -123.40, -3434.59, 31.66, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3816, -124.94, -3191.61, 31.98, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -89.53, -3777.04, 135.67, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -87.67, -3774.19, 135.66, 0.00, 90.00, -59.55, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -95.13, -3788.10, 135.68, 0.00, 90.00, -50.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -101.19, -3793.23, 135.69, 0.00, 90.00, -50.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19909, -120.51, -3791.23, 45.84, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(993, -133.33, -3793.07, 47.02, 0.00, 0.00, -18.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18850, -120.77, -3777.28, 42.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(993, -133.33, -3793.07, 49.41, 0.00, 0.00, -18.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19913, -112.31, -3778.31, 54.00, 90.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19913, -112.24, -3789.29, 54.00, 90.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16082, -112.76, -3756.75, 50.74, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18753, -125.56, -3752.99, 44.32, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18753, -125.56, -3752.99, 43.34, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18788, -103.01, -3671.73, 44.92, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18788, -103.01, -3631.74, 44.92, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18788, -103.01, -3592.96, 41.46, 0.00, 10.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18788, -103.01, -3553.58, 34.52, 0.00, 10.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18788, -103.01, -3515.67, 24.36, 0.00, 20.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18850, 41.40, -3575.04, 15.00, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18850, -41.64, -3573.40, 15.00, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11508, -101.94, -3616.62, 5.00, 0.00, 0.00, 47.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16713, -172.34, -3630.04, 23.58, 0.00, 0.00, 84.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10763, -129.77, -3530.87, 55.00, 0.00, 0.00, -45.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7916, -184.03, -3624.44, 30.00, 23.00, 0.00, -156.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19841, -209.00, -3634.09, 18.00, 0.00, 0.00, -149.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7916, -229.18, -3654.92, 15.00, 40.00, 0.00, -113.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19841, -225.81, -3653.25, 18.00, 0.00, 0.00, -149.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19841, -238.12, -3668.52, 18.00, 0.00, 0.00, -149.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19841, -230.32, -3662.63, 26.00, 0.00, 0.00, -149.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19841, -180.18, -3624.52, 28.00, -25.00, 0.00, -164.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19841, -155.49, -3629.92, 28.00, -25.00, 0.00, -164.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7916, -116.96, -3595.29, 30.00, 23.00, 0.00, -156.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19841, -143.04, -3628.70, 29.00, -25.00, 0.00, -116.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19841, -157.10, -3617.19, 32.00, -25.00, 0.00, 207.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3781, -103.38, -3791.01, 94.84, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3781, -103.38, -3791.01, 122.10, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3781, -103.38, -3791.01, 67.58, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19454, -124.91, -3800.95, 135.70, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -101.19, -3793.23, 142.00, 0.00, 90.00, -50.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -95.13, -3788.10, 142.09, 0.00, 90.00, -50.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -89.53, -3782.03, 142.10, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -87.67, -3774.19, 142.07, 0.00, 90.00, -59.55, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -89.53, -3777.04, 142.08, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -81.31, -3771.74, 142.09, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -75.80, -3771.89, 142.08, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -79.03, -3781.25, 142.08, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -75.78, -3781.34, 142.09, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -75.78, -3790.96, 142.11, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -79.03, -3790.87, 142.09, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -89.53, -3791.66, 142.10, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -75.78, -3800.60, 142.09, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -79.03, -3800.50, 142.10, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -89.53, -3801.29, 142.09, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -75.78, -3810.13, 142.08, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -89.53, -3810.13, 142.09, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -79.03, -3810.13, 142.09, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -100.04, -3795.79, 142.08, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -100.04, -3805.02, 142.09, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -110.54, -3804.91, 142.09, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -110.54, -3796.24, 142.11, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19454, -120.61, -3807.97, 142.08, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19454, -120.61, -3805.15, 142.09, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19454, -120.61, -3796.19, 142.09, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19454, -120.61, -3793.17, 142.08, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19454, -124.91, -3800.95, 142.11, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -130.48, -3799.49, 142.08, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19362, -127.18, -3808.11, 142.09, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19362, -127.17, -3805.91, 142.09, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19435, -131.66, -3805.11, 142.09, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19362, -130.66, -3805.91, 142.09, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19435, -130.68, -3792.27, 142.09, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19435, -130.68, -3793.87, 142.09, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19435, -130.68, -3793.87, 135.68, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19435, -133.20, -3793.87, 142.09, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -101.19, -3793.23, 148.31, 0.00, 90.00, -50.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -95.13, -3788.10, 148.40, 0.00, 90.00, -50.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -89.53, -3782.03, 148.41, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -87.67, -3774.19, 148.38, 0.00, 90.00, -59.55, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -89.53, -3777.04, 148.39, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -81.31, -3771.74, 148.40, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -75.80, -3771.89, 148.39, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -75.78, -3781.34, 148.40, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -79.03, -3781.25, 148.39, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -75.78, -3790.96, 148.42, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -79.03, -3790.87, 148.40, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -89.53, -3791.66, 148.41, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -75.78, -3800.60, 148.40, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -79.03, -3800.50, 148.41, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -89.53, -3801.29, 148.40, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -75.78, -3810.13, 148.39, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -79.03, -3810.13, 148.40, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -89.53, -3810.13, 148.40, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -100.04, -3795.79, 148.39, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -100.04, -3805.02, 148.40, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -110.54, -3796.24, 148.42, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -110.54, -3804.91, 148.40, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19454, -120.61, -3807.97, 148.39, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19454, -120.61, -3805.15, 148.40, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19454, -120.61, -3796.19, 148.40, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19454, -120.61, -3793.17, 148.39, 0.00, 90.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19454, -124.91, -3800.95, 148.42, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19362, -127.18, -3808.11, 148.40, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19362, -127.17, -3805.91, 148.40, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19377, -130.48, -3799.49, 148.39, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19362, -130.66, -3805.91, 148.40, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19435, -131.66, -3805.11, 148.40, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19362, -127.18, -3793.07, 148.40, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19435, -130.68, -3792.27, 148.40, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19362, -127.18, -3793.07, 142.09, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19435, -130.68, -3793.87, 148.40, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19435, -133.20, -3793.87, 148.40, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(4605, -82.80, -3788.06, 40.47, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16082, -130.00, -3756.75, 50.74, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(993, -110.41, -3795.25, 45.82, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(993, -112.79, -3795.23, 45.82, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3244, -208.44, -3823.22, 42.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3244, -125.33, -3824.63, 42.14, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10357, -52.65, -3823.37, 100.00, 0.00, 0.00, 33.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1595, -150.91, -3755.20, 74.92, 0.00, 0.00, 142.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1595, -180.50, -3772.56, 87.22, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1694, -141.02, -3775.55, 78.60, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7381, -151.32, -3754.89, 48.90, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7368, -151.32, -3754.89, 48.90, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10816, -379.86, -2916.09, 22.00, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10816, -379.86, -3036.18, 22.00, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10816, -379.86, -3156.27, 22.00, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10816, -379.86, -3276.37, 22.00, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10816, -498.56, -3276.37, 21.84, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11508, -649.46, -2831.38, 16.57, 0.00, 0.00, -4.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10230, -288.96, -3380.25, 11.02, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10229, -289.28, -3381.71, 9.86, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10231, -289.82, -3381.73, 12.40, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11694, -853.84, -3428.06, 18.73, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(5184, -258.57, -3430.00, 39.76, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(5184, -388.46, -3430.00, 39.76, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(5184, -388.46, -3455.00, 39.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(5184, -258.57, -3455.00, 39.72, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3474, -227.35, -3419.80, 27.38, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1378, -302.02, -3413.64, 30.61, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3620, -328.69, -3402.53, 33.98, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1378, -261.77, -3413.64, 45.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3474, -443.77, -3420.00, 27.38, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3620, -356.53, -3403.11, 33.98, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1378, -390.44, -3414.72, 45.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3621, -387.88, -3437.74, 26.20, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3673, -421.05, -3419.49, 39.00, 0.00, 0.00, -180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3636, -438.87, -3459.74, 27.86, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3636, -378.81, -3459.74, 27.86, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1378, -410.76, -3469.89, 45.00, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3474, -351.49, -3463.37, 27.38, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3620, -336.47, -3484.15, 33.98, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3637, -337.02, -3456.02, 28.88, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3753, -213.77, -3484.60, 13.00, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3753, -244.46, -3484.60, 2.92, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3753, -244.46, -3493.62, 2.94, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3753, -213.77, -3493.62, 13.00, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3621, -266.52, -3447.13, 26.20, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3665, -1032.90, -3293.83, 23.04, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3665, -1032.90, -3173.79, 23.04, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3665, -1032.90, -3053.89, 23.04, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(5126, -313.97, -3465.99, 35.40, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1378, -285.05, -3473.52, 30.61, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10833, -183.59, -3630.79, 4.40, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3753, -223.26, -3511.10, -0.40, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(9829, -238.29, -3561.27, -60.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(4565, -219.18, -3554.78, 5.20, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(4565, -219.18, -3526.75, 5.20, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(6295, -306.91, -3553.35, 29.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3620, -255.26, -3451.63, 33.98, 0.00, 0.00, 149.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3620, -219.54, -3458.08, 30.00, 0.00, 0.00, 229.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(6295, -786.80, -3500.71, 68.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(6295, -883.10, -3688.89, 68.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(6295, -935.03, -3575.54, 68.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16613, -888.11, -3406.62, 47.51, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16614, -885.62, -3508.29, 47.64, 0.00, 0.00, 55.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10145, -155.26, -3771.84, 74.58, 180.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18750, -796.77, -3412.62, 47.54, 0.00, 0.00, 91.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11539, -124.70, -3812.57, -2.80, 0.00, 0.00, 159.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11541, -50.90, -3746.61, 24.32, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19841, -20.04, -3680.79, 20.00, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19841, -23.62, -3672.82, 20.00, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19841, -24.07, -3663.25, 20.00, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19841, -20.60, -3686.52, 20.00, 0.00, 0.00, 47.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7916, -230.31, -3739.99, 15.00, 8.00, 0.00, -105.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19841, -222.20, -3725.39, 26.00, 0.00, 0.00, -84.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19841, -223.73, -3788.19, 26.00, 0.00, 0.00, -127.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18850, -235.50, -3595.56, 0.50, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16082, -244.97, -3575.25, 9.30, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(14414, -200.59, -3473.88, 19.08, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(14414, -204.59, -3473.88, 19.08, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(14414, -208.60, -3473.88, 19.08, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19911, -209.72, -3476.75, 19.08, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19911, -210.60, -3478.65, 19.08, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(17299, -36.71, -3668.08, 26.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(17299, -11.65, -3660.84, -0.02, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16613, -666.31, -2856.42, 42.00, 0.00, 0.00, 360.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3743, -210.80, -3508.07, 9.45, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3743, -202.01, -3508.07, 9.45, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3743, -205.94, -3508.07, 9.45, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3743, -213.74, -3508.07, 9.45, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3743, -216.53, -3508.07, 9.45, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10816, -880.91, -3276.37, 21.84, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10816, -880.85, -3156.27, 21.82, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10816, -880.91, -3036.18, 21.82, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10816, -880.91, -2915.97, 21.82, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10816, -498.56, -3036.18, 21.84, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10816, -498.56, -3156.27, 21.84, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10816, -498.56, -2916.09, 21.84, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11508, -783.28, -2841.26, 16.00, 0.00, 0.00, -18.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10816, -683.68, -3508.41, 21.64, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16139, -693.95, -3246.66, 17.94, 0.00, 0.00, 76.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16139, -694.00, -3126.34, 17.94, 0.00, 0.00, 76.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16139, -692.21, -3006.33, 17.94, 0.00, 0.00, 76.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16139, -693.83, -2885.95, 18.00, 0.00, 0.00, 76.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3665, -1032.90, -2933.71, 23.04, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3816, -710.72, -3406.57, 31.24, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3816, -710.72, -3532.38, 31.24, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11694, -863.20, -3620.37, 18.73, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7381, -696.62, -3662.34, 25.70, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11556, -742.14, -3619.76, 29.94, 0.00, 0.00, 81.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11556, -754.30, -3611.95, 39.00, -10.00, 0.00, 81.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11556, -750.59, -3587.96, 29.94, 0.00, 0.00, 81.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16614, -884.92, -3624.28, 47.64, 0.00, 0.00, 171.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11541, -1049.91, -3234.55, 9.16, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11541, -1049.91, -3072.71, 9.15, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11541, -1049.92, -2916.28, 9.16, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16713, -1056.28, -3329.07, -0.16, 0.00, 0.00, -135.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7368, -1032.78, -3263.87, 26.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7371, -1068.39, -3083.26, 22.88, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7371, -1068.39, -2950.98, 22.88, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7371, -1068.33, -2857.97, 22.88, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7371, -948.39, -2846.03, 22.88, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7371, -979.69, -3323.60, 22.88, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11541, -198.48, -3750.72, 24.32, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11541, -725.59, -3677.41, 8.94, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(11541, -656.00, -3626.41, 8.92, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16713, -651.84, -3717.14, 0.00, 0.00, 0.00, -120.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16713, -631.94, -3692.25, 0.00, 0.00, 0.00, -33.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7916, -738.98, -3706.15, 1.42, -12.00, 0.00, -16.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7916, -689.36, -3710.77, 1.86, 4.00, 0.00, -25.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7916, -622.03, -3654.94, 1.86, -8.00, 0.00, 84.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7916, -623.69, -3603.70, 3.00, -18.00, 0.00, 84.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7916, -1083.03, -3260.12, 1.86, 0.00, 0.00, -102.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7916, -1075.41, -3219.45, 1.86, -20.00, -2.00, -102.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7916, -1079.02, -3179.63, 2.50, -20.00, -2.00, -102.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7916, -1080.98, -3104.83, 2.50, -20.00, -2.00, -102.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7916, -1082.18, -3038.58, 2.50, -20.00, -2.00, -102.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7916, -1080.94, -2944.47, 2.50, -20.00, -2.00, -102.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7916, -1080.11, -2881.68, 2.50, -20.00, -2.00, -102.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16713, -631.58, -3533.66, 0.00, 0.00, 0.00, -33.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18753, -3.39, -3549.21, 26.41, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18753, -3.39, -3424.23, 26.41, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18753, -3.39, -3299.24, 26.41, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18753, -3.39, -3174.28, 26.41, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18753, -3.39, -3049.28, 26.41, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18850, 0.01, -3573.40, 15.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(4726, -22.96, -3510.92, 26.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18850, 41.40, -3453.09, 15.00, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18850, 0.01, -3453.09, 15.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18850, -41.64, -3453.09, 15.00, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(4726, 31.11, -3401.13, 26.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(4726, -22.96, -3401.13, 26.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18796, -85.30, -3383.98, 27.03, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18796, -125.01, -3383.97, 23.50, 0.00, -10.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18796, -134.11, -3384.00, 23.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3749, -147.47, -3384.02, 28.86, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18753, -3.39, -3549.21, 25.40, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18753, -3.39, -3424.23, 25.40, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18753, -3.39, -3299.24, 25.40, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18753, -3.39, -3174.28, 25.40, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18753, -3.39, -3049.28, 25.40, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18753, -3.39, -3549.21, 24.38, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(17019, 27.93, -3210.09, 32.80, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1378, 39.47, -3230.65, 45.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3621, -56.46, -3265.91, 32.32, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3621, -35.44, -3265.91, 32.32, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(17019, 27.27, -3264.37, 32.80, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(17019, 28.35, -3319.84, 32.80, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1378, -21.19, -3317.15, 50.80, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1378, -23.88, -3214.05, 50.80, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10769, 94.25, -3113.60, 2.00, 0.00, 0.00, -45.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10828, 175.44, -3074.55, 9.90, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10769, 94.25, -3207.54, 2.00, 0.00, 0.00, -45.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10828, 175.35, -3168.65, 9.90, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(9958, 140.23, -3087.71, 43.12, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1378, 177.16, -3115.32, 36.82, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(1378, 121.25, -3115.32, 36.82, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(9958, 229.36, -3187.09, 5.06, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(9958, 231.52, -3147.86, 5.06, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(4842, 10.75, -3733.11, -4.70, 0.00, 0.00, 113.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -15.61, -3830.86, -5.00, 0.00, 0.00, -40.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -0.75, -3797.69, -5.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -13.00, -3815.05, -5.00, 0.00, 0.00, -47.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -27.14, -3855.56, -5.00, 0.00, 0.00, 135.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -27.86, -3848.49, -5.00, 0.00, 0.00, 135.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -61.03, -3842.84, -5.00, 0.00, 0.00, 105.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -91.43, -3851.46, -5.00, 0.00, 0.00, 105.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -58.15, -3854.75, -5.00, 0.00, 0.00, 105.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -123.29, -3856.82, -5.00, 0.00, 0.00, 105.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -180.02, -3855.70, -5.00, 0.00, 0.00, 105.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -213.24, -3849.93, -5.00, 0.00, 0.00, 258.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -237.70, -3852.16, -5.00, 0.00, 0.00, 258.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -243.53, -3813.75, -5.00, 0.00, 0.00, 331.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -229.08, -3816.58, -3.00, 0.00, 0.00, 331.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -236.26, -3784.90, -3.00, 0.00, 0.00, 331.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -224.73, -3792.77, 5.00, 0.00, 0.00, 331.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -221.62, -3826.84, 5.00, 0.00, 0.00, 331.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -177.12, -3853.17, 5.00, 0.00, 0.00, 331.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -231.43, -3698.63, -3.00, 0.00, 0.00, 331.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -233.78, -3680.53, -10.00, 0.00, 0.00, 331.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -236.05, -3641.33, -10.00, 0.00, 0.00, 331.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -233.33, -3625.05, -10.00, 0.00, 0.00, 331.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -229.97, -3625.66, -10.00, 0.00, 0.00, 251.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -234.60, -3665.48, -10.00, 0.00, 0.00, 331.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7916, -258.59, -3687.76, 5.00, 6.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16118, -234.98, -3748.47, -7.90, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16118, -245.14, -3786.33, -5.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16118, -248.49, -3747.03, -7.90, 0.00, 0.00, -14.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7916, -262.85, -3735.68, 4.00, 2.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16118, -257.41, -3802.38, -7.90, 0.00, 0.00, -14.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16118, -203.44, -3871.51, -7.90, 0.00, 0.00, 62.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -191.88, -3871.14, -9.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -174.56, -3870.06, -9.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -176.55, -3871.29, -9.00, 0.00, 0.00, 91.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(16114, -162.09, -3856.71, -9.00, 0.00, 0.00, 91.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(726, 12.56, -3753.60, 4.54, 0.00, 0.00, -4.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(726, -3.77, -3742.76, 4.54, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(726, -1.48, -3677.84, 4.54, 0.00, 0.00, 76.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(726, 2.31, -3661.33, 4.54, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(726, 1.63, -3766.64, 4.54, 0.00, 0.00, -36.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(726, 8.42, -3731.91, 4.54, 0.00, 0.00, -4.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(726, 8.08, -3696.32, 4.54, 0.00, 0.00, -4.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(726, 6.67, -3714.37, 4.54, 0.00, 0.00, -4.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(726, -9.73, -3726.50, 4.54, 0.00, 0.00, 69.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(726, -10.77, -3707.40, 4.54, 0.00, 0.00, -91.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(726, -8.11, -3690.72, 4.54, 0.00, 0.00, 98.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3259, -815.45, -3613.23, 48.88, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3673, -830.93, -3578.08, 70.10, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3636, -891.15, -3564.59, 53.92, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3636, -798.66, -3540.13, 53.92, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3636, -798.38, -3584.01, 53.92, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3673, -816.17, -3691.39, 70.10, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3636, -789.43, -3669.58, 53.42, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3673, -813.01, -3645.76, 70.10, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3259, -844.49, -3537.30, 48.88, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7379, -636.99, -3609.32, 22.60, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7371, -624.98, -3323.48, 22.88, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7371, -637.21, -3443.96, 22.88, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7371, -637.21, -3576.35, 22.88, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7371, -492.49, -3323.48, 22.88, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7371, -360.13, -3323.48, 22.88, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7371, -187.73, -3359.62, 22.88, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7371, -187.73, -3605.69, 22.88, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3279, -193.78, -3348.23, 23.09, 0.00, 0.00, 69.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3279, -194.52, -3334.28, 23.09, 0.00, 0.00, 69.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3279, -204.67, -3318.88, 23.09, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3279, -225.21, -3319.37, 23.09, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3268, -45.47, -3163.74, 26.88, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3268, -45.47, -3127.00, 26.88, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3268, -45.47, -3055.24, 26.88, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3268, -45.47, -3014.92, 26.88, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3279, -39.43, -3083.11, 27.01, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3279, -39.43, -3094.44, 27.01, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3620, 19.42, -3193.96, 39.88, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3620, 45.44, -3299.54, 39.88, 0.00, 0.00, 47.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3620, 10.93, -3278.87, 39.88, 0.00, 0.00, 156.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18766, -120.21, -3784.40, 45.00, 0.00, 90.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18981, -99.62, -3773.23, 41.70, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(18981, -124.62, -3773.23, 41.70, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3279, 220.94, -3206.39, 12.60, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3279, 220.94, -3226.81, 12.60, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(8613, 63.19, -3196.33, 23.52, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(8613, 71.15, -3204.54, 15.88, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10009, 103.30, -3150.08, 9.24, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10009, 85.68, -3055.83, 9.24, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7371, -61.72, -3609.48, 26.89, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7371, 58.24, -3597.25, 26.89, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7371, 58.24, -3464.91, 26.89, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7371, -64.79, -3645.54, 26.89, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(7371, -64.79, -3513.31, 26.89, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19841, -74.72, -3306.40, 0.90, -6.00, 0.00, 33.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19841, -56.75, -3307.01, 0.90, -6.00, 0.00, -25.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3244, -820.39, -3370.91, 47.01, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3244, -788.40, -3346.87, 47.01, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(3244, -767.45, -3476.15, 47.01, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(791, -814.38, -3480.53, 45.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(791, -832.15, -3437.32, 45.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(791, -833.65, -3390.12, 45.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(791, -853.97, -3655.52, 45.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(791, -210.63, -3857.93, 10.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(8356, -226.51, -4013.91, 7.85, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(8171, -63.69, -4013.98, 7.85, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(8357, 111.69, -4014.15, 7.85, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(8172, 575.31, -4014.50, 7.85, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(8171, 287.08, -4014.26, 7.85, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(8356, -251.06, -4073.41, 7.83, 0.00, 0.00, 45.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(8171, -135.90, -4188.64, 7.83, 0.00, 0.00, 45.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(8357, -11.79, -4312.81, 7.83, 0.00, 0.00, 45.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(8171, 262.78, -4587.31, 7.83, 0.00, 0.00, -135.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(8172, 368.74, -4693.45, 7.83, 0.00, 0.00, 45.06, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(8356, 449.69, -4712.76, 7.81, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(8171, 449.94, -4549.86, 7.81, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(8171, 449.96, -4411.68, 7.85, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(8357, 449.83, -4236.34, 7.87, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(8357, 138.54, -4463.19, 7.83, 0.00, 0.00, -135.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(8356, 449.66, -3898.25, 7.85, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(8171, 425.38, -4014.41, 7.87, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(8171, 449.83, -4060.95, 7.97, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, -254.01, -4030.12, 3.98, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, -80.39, -4029.97, 3.98, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 20.90, -4029.89, 3.96, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 194.68, -4029.87, 3.98, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 296.18, -4029.79, 4.02, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 427.47, -3884.98, 4.00, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 643.94, -3996.98, 4.02, 0.00, 0.00, 90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10764, 697.39, -4014.76, 7.72, 0.00, 0.00, -45.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 469.97, -3996.94, 3.96, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 433.40, -4121.46, 4.00, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 434.06, -4295.14, 4.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 434.37, -4396.61, 4.06, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 434.55, -4570.38, 4.00, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 434.55, -4671.95, 3.94, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 434.57, -4792.95, 3.96, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(10764, 450.43, -4847.70, 7.67, 0.00, 0.00, -135.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 369.14, -4715.56, 3.94, 0.00, 0.00, 225.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 297.45, -4644.31, 3.98, 0.00, 0.00, 45.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 174.58, -4521.69, 3.94, 0.00, 0.00, 225.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 102.98, -4450.10, 3.92, 0.00, 0.00, 45.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, -19.82, -4327.11, 3.94, 0.00, 0.00, 225.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, -91.18, -4255.57, 3.92, 0.00, 0.00, 45.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, -214.19, -4132.86, 4.00, 0.00, 0.00, 225.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, -289.74, -4057.28, 3.96, 0.00, 0.00, 45.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, -179.37, -4107.53, 3.92, 0.00, 0.00, -45.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 382.69, -4120.51, 4.08, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 346.08, -4589.05, 4.04, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 345.29, -4415.80, 4.04, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 344.98, -4241.64, 4.04, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 294.29, -4387.76, 4.04, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 294.35, -4216.09, 4.04, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 243.56, -4465.60, 4.04, 0.00, 0.00, 45.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 120.52, -4342.48, 4.04, 0.00, 0.00, 225.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 49.12, -4270.80, 4.04, 0.00, 0.00, 45.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, -73.22, -4148.05, 4.04, 0.00, 0.00, 225.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, -17.11, -4118.11, 3.96, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 156.80, -4118.42, 3.96, 0.00, 0.00, -270.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 252.04, -4118.48, 3.92, 0.00, 0.00, -90.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, -105.90, -4116.31, 4.02, 0.00, 0.00, 225.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 285.05, -4507.49, 3.98, 0.00, 0.00, 225.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 74.15, -4168.86, 3.95, 0.00, 0.00, -270.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 205.72, -4249.22, 4.04, 0.00, 0.00, 0.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 211.14, -4326.90, 4.00, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 117.17, -4234.89, 4.08, 0.00, 0.00, 180.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // CreateDynamicObject(19340, 80.95, -4203.61, 3.99, 0.00, 0.00, -270.00, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1, 0);
    // AddStaticVehicle(592, -265.5860, -4059.1699, 18.0885);
}


stock ChangePlayerColor(const playerid, const page) {
    new msg[512];
    format(msg, sizeof(msg), "{0088FF}当前第 %d 页 - 共 %d 页",page, GetMaxPage(sizeof(PlayerColors), 10));
    format(msg, sizeof(msg), "%s\n上一页", msg);
    for (new i = (page - 1) * 10; i < page * 10; i++) {
        format(msg, sizeof(msg), "%s\nColorID\t%d\t{%06x}▇\t▇\t▇", msg, i, PlayerColors[i] >>> 8);
    }
    format(msg, sizeof(msg), "%s\n下一页", msg);
    Dialog_Show(playerid, Player_Color, DIALOG_STYLE_LIST, "修改颜色", msg, "更改", "关闭");
    return 1;
}


stock InitializationNetWork(const playerid) {
    network_txtdraw[playerid][0] = CreatePlayerTextDraw(playerid, 265.934539, 0.618553, "0.00KB/s 0pkt/s");
    PlayerTextDrawLetterSize(playerid, network_txtdraw[playerid][0], 0.180000, 0.899999);
    PlayerTextDrawAlignment(playerid, network_txtdraw[playerid][0], 1);
    PlayerTextDrawColor(playerid, network_txtdraw[playerid][0], -1);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][0], 0);
    PlayerTextDrawSetOutline(playerid, network_txtdraw[playerid][0], 0);
    PlayerTextDrawBackgroundColor(playerid, network_txtdraw[playerid][0], 255);
    PlayerTextDrawFont(playerid, network_txtdraw[playerid][0], 1);
    PlayerTextDrawSetProportional(playerid, network_txtdraw[playerid][0], 1);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][0], 0);

    network_txtdraw[playerid][1] = CreatePlayerTextDraw(playerid, 257.435607, 2.577767, "LD_BEAT:up");
    PlayerTextDrawLetterSize(playerid, network_txtdraw[playerid][1], 0.000000, 0.000000);
    PlayerTextDrawTextSize(playerid, network_txtdraw[playerid][1], 6.000000, 6.000000);
    PlayerTextDrawAlignment(playerid, network_txtdraw[playerid][1], 1);
    PlayerTextDrawColor(playerid, network_txtdraw[playerid][1], -1);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, network_txtdraw[playerid][1], 0);
    PlayerTextDrawBackgroundColor(playerid, network_txtdraw[playerid][1], 255);
    PlayerTextDrawFont(playerid, network_txtdraw[playerid][1], 4);
    PlayerTextDrawSetProportional(playerid, network_txtdraw[playerid][1], 0);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][1], 0);

    network_txtdraw[playerid][2] = CreatePlayerTextDraw(playerid, 333.200561, 0.651888, "0.00KB/s 0pkt/s");
    PlayerTextDrawLetterSize(playerid, network_txtdraw[playerid][2], 0.180000, 0.899999);
    PlayerTextDrawAlignment(playerid, network_txtdraw[playerid][2], 1);
    PlayerTextDrawColor(playerid, network_txtdraw[playerid][2], -1);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][2], 0);
    PlayerTextDrawSetOutline(playerid, network_txtdraw[playerid][2], 0);
    PlayerTextDrawBackgroundColor(playerid, network_txtdraw[playerid][2], 255);
    PlayerTextDrawFont(playerid, network_txtdraw[playerid][2], 1);
    PlayerTextDrawSetProportional(playerid, network_txtdraw[playerid][2], 1);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][2], 0);

    network_txtdraw[playerid][3] = CreatePlayerTextDraw(playerid, 324.701080, 3.207582, "LD_BEAT:down");
    PlayerTextDrawLetterSize(playerid, network_txtdraw[playerid][3], 0.000000, 0.000000);
    PlayerTextDrawTextSize(playerid, network_txtdraw[playerid][3], 6.000000, 6.000000);
    PlayerTextDrawAlignment(playerid, network_txtdraw[playerid][3], 1);
    PlayerTextDrawColor(playerid, network_txtdraw[playerid][3], -1);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][3], 0);
    PlayerTextDrawSetOutline(playerid, network_txtdraw[playerid][3], 0);
    PlayerTextDrawBackgroundColor(playerid, network_txtdraw[playerid][3], 255);
    PlayerTextDrawFont(playerid, network_txtdraw[playerid][3], 4);
    PlayerTextDrawSetProportional(playerid, network_txtdraw[playerid][3], 0);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][3], 0);

    network_txtdraw[playerid][4] = CreatePlayerTextDraw(playerid, 259.734954, 0.718510, "LD_BEAT:up");
    PlayerTextDrawLetterSize(playerid, network_txtdraw[playerid][4], 0.000000, 0.000000);
    PlayerTextDrawTextSize(playerid, network_txtdraw[playerid][4], 6.000000, 6.000000);
    PlayerTextDrawAlignment(playerid, network_txtdraw[playerid][4], 1);
    PlayerTextDrawColor(playerid, network_txtdraw[playerid][4], -1);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][4], 0);
    PlayerTextDrawSetOutline(playerid, network_txtdraw[playerid][4], 0);
    PlayerTextDrawBackgroundColor(playerid, network_txtdraw[playerid][4], 255);
    PlayerTextDrawFont(playerid, network_txtdraw[playerid][4], 4);
    PlayerTextDrawSetProportional(playerid, network_txtdraw[playerid][4], 0);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][4], 0);

    network_txtdraw[playerid][5] = CreatePlayerTextDraw(playerid, 326.900878, 1.292769, "LD_BEAT:down");
    PlayerTextDrawLetterSize(playerid, network_txtdraw[playerid][5], 0.000000, 0.000000);
    PlayerTextDrawTextSize(playerid, network_txtdraw[playerid][5], 6.000000, 6.000000);
    PlayerTextDrawAlignment(playerid, network_txtdraw[playerid][5], 1);
    PlayerTextDrawColor(playerid, network_txtdraw[playerid][5], -1);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][5], 0);
    PlayerTextDrawSetOutline(playerid, network_txtdraw[playerid][5], 0);
    PlayerTextDrawBackgroundColor(playerid, network_txtdraw[playerid][5], 255);
    PlayerTextDrawFont(playerid, network_txtdraw[playerid][5], 4);
    PlayerTextDrawSetProportional(playerid, network_txtdraw[playerid][5], 0);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][5], 0);

    network_txtdraw[playerid][6] = CreatePlayerTextDraw(playerid, 329.000000, 0.000000, "box");
    PlayerTextDrawLetterSize(playerid, network_txtdraw[playerid][6], 0.000000, 0.966665);
    PlayerTextDrawTextSize(playerid, network_txtdraw[playerid][6], 0.000000, 202.000000);
    PlayerTextDrawAlignment(playerid, network_txtdraw[playerid][6], 2);
    PlayerTextDrawColor(playerid, network_txtdraw[playerid][6], 255);
    PlayerTextDrawUseBox(playerid, network_txtdraw[playerid][6], 1);
    PlayerTextDrawBoxColor(playerid, network_txtdraw[playerid][6], 130);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][6], 0);
    PlayerTextDrawSetOutline(playerid, network_txtdraw[playerid][6], 0);
    PlayerTextDrawBackgroundColor(playerid, network_txtdraw[playerid][6], 255);
    PlayerTextDrawFont(playerid, network_txtdraw[playerid][6], 1);
    PlayerTextDrawSetProportional(playerid, network_txtdraw[playerid][6], 1);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][6], 0);

    network_txtdraw[playerid][7] = CreatePlayerTextDraw(playerid, 228.631988, 1.559249, "LD_BEAT:chit");
    PlayerTextDrawLetterSize(playerid, network_txtdraw[playerid][7], 0.000000, 0.000000);
    PlayerTextDrawTextSize(playerid, network_txtdraw[playerid][7], 5.000000, 6.000000);
    PlayerTextDrawAlignment(playerid, network_txtdraw[playerid][7], 1);
    PlayerTextDrawColor(playerid, network_txtdraw[playerid][7], 16711935);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][7], 0);
    PlayerTextDrawSetOutline(playerid, network_txtdraw[playerid][7], 0);
    PlayerTextDrawBackgroundColor(playerid, network_txtdraw[playerid][7], 255);
    PlayerTextDrawFont(playerid, network_txtdraw[playerid][7], 4);
    PlayerTextDrawSetProportional(playerid, network_txtdraw[playerid][7], 0);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][7], 0);

    network_txtdraw[playerid][8] = CreatePlayerTextDraw(playerid, 234.033920, 0.348182, "0ms");
    PlayerTextDrawLetterSize(playerid, network_txtdraw[playerid][8], 0.180000, 0.899999);
    PlayerTextDrawAlignment(playerid, network_txtdraw[playerid][8], 1);
    PlayerTextDrawColor(playerid, network_txtdraw[playerid][8], -1);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][8], 0);
    PlayerTextDrawSetOutline(playerid, network_txtdraw[playerid][8], 0);
    PlayerTextDrawBackgroundColor(playerid, network_txtdraw[playerid][8], 255);
    PlayerTextDrawFont(playerid, network_txtdraw[playerid][8], 1);
    PlayerTextDrawSetProportional(playerid, network_txtdraw[playerid][8], 1);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][8], 0);

    network_txtdraw[playerid][9] = CreatePlayerTextDraw(playerid, 429.833984, 0.444479, "0%");
    PlayerTextDrawLetterSize(playerid, network_txtdraw[playerid][9], 0.180000, 0.899999);
    PlayerTextDrawAlignment(playerid, network_txtdraw[playerid][9], 3);
    PlayerTextDrawColor(playerid, network_txtdraw[playerid][9], -1);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][9], 0);
    PlayerTextDrawSetOutline(playerid, network_txtdraw[playerid][9], 0);
    PlayerTextDrawBackgroundColor(playerid, network_txtdraw[playerid][9], 255);
    PlayerTextDrawFont(playerid, network_txtdraw[playerid][9], 1);
    PlayerTextDrawSetProportional(playerid, network_txtdraw[playerid][9], 1);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][9], 0);

    network_txtdraw[playerid][10] = CreatePlayerTextDraw(playerid, 397.867950, 0.429632, "LOSS");
    PlayerTextDrawLetterSize(playerid, network_txtdraw[playerid][10], 0.180000, 0.899999);
    PlayerTextDrawAlignment(playerid, network_txtdraw[playerid][10], 1);
    PlayerTextDrawColor(playerid, network_txtdraw[playerid][10], -1);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][10], 0);
    PlayerTextDrawSetOutline(playerid, network_txtdraw[playerid][10], 0);
    PlayerTextDrawBackgroundColor(playerid, network_txtdraw[playerid][10], 255);
    PlayerTextDrawFont(playerid, network_txtdraw[playerid][10], 3);
    PlayerTextDrawSetProportional(playerid, network_txtdraw[playerid][10], 1);
    PlayerTextDrawSetShadow(playerid, network_txtdraw[playerid][10], 0);
    PlayerInfo[playerid][lastBytesSent] = 0; //sent 
    PlayerInfo[playerid][lastMessagesSent] = 0; //pkt receive 
    PlayerInfo[playerid][lastBytesReceived] = 0; //received
    PlayerInfo[playerid][lastMessagesReceived] = 0; //pkt received

    if(PlayerInfo[playerid][netStats]) 
    {
        for (new i = 0; i <= 10; i++) 
        {
            PlayerTextDrawShow(playerid, PlayerText:network_txtdraw[playerid][i]);
        }
    } 
}



stock updatePlayerNetWorkState(const i){
    if(PlayerInfo[i][netStats])
    {
        // sent(upload)
        new string[32];
        format(string, sizeof(string), "%.2fKB/s %ipkt/s", float(NetStats_BytesSent(i) - PlayerInfo[i][lastBytesSent]) / 1024.0, NetStats_MessagesSent(i) - PlayerInfo[i][lastMessagesSent]);
        PlayerTextDrawSetString(i, PlayerText:network_txtdraw[i][0], string);
        // received(download)
        format(string, sizeof(string), "%.2fKB/s %ipkt/s", float(NetStats_BytesReceived(i) - PlayerInfo[i][lastBytesReceived]) / 1024.0, NetStats_MessagesReceived(i) - PlayerInfo[i][lastMessagesReceived]);
        PlayerTextDrawSetString(i, PlayerText:network_txtdraw[i][2], string);
        // ping & color
        new ping = GetPlayerPing(i);
        format(string, sizeof(string), "%ims", ping);
        if(ping <= 80) PlayerTextDrawColor(i, network_txtdraw[i][7], 16711935);
        else if(ping > 80 && ping <= 130) PlayerTextDrawColor(i, network_txtdraw[i][7], Color_Yellow);
        else PlayerTextDrawColor(i, network_txtdraw[i][7], Color_Red);
        PlayerTextDrawShow(i, network_txtdraw[i][7]);
        PlayerTextDrawSetString(i, network_txtdraw[i][8], string);
        // loss
        new Float:loss = NetStats_PacketLossPercent(i);
        format(string, sizeof(string), "%.0f%", loss);
        PlayerTextDrawSetString(i, network_txtdraw[i][9], string);
        PlayerTextDrawColor(i, network_txtdraw[i][9], Color_White);
        if(loss >= 1.0) PlayerTextDrawColor(i, network_txtdraw[i][9], Color_Red);
        PlayerTextDrawShow(i, network_txtdraw[i][9]);
        PlayerInfo[i][lastBytesSent] = NetStats_BytesSent(i); //sent 
        PlayerInfo[i][lastMessagesSent] = NetStats_MessagesSent(i); //pkt receive 
        PlayerInfo[i][lastBytesReceived] = NetStats_BytesReceived(i); //received
        PlayerInfo[i][lastMessagesReceived] = NetStats_MessagesReceived(i); //pkt received
    }
}

Dialog:Player_Color(playerid, response, listitem, inputtext[]) {
    if(response) {
        new idx, tmp[128];
        tmp = strtok(inputtext, idx);
        if(strcmp(tmp, "上一页") == 0) {
            if(PlayerColorPage[playerid] == 1) return SCM(playerid, Color_White, "[系统] 翻页失败,原因:这是第一页.");
            PlayerColorPage[playerid]--;
            ChangePlayerColor(playerid, PlayerColorPage[playerid]);
            return 1;
        }
        if(strcmp(tmp, "下一页") == 0) {
            if(PlayerColorPage[playerid] == GetMaxPage(sizeof(PlayerColors)) - 1) return SCM(playerid, Color_White, "[系统] 翻页失败,原因:这是最后一页.");
            PlayerColorPage[playerid]++;
            ChangePlayerColor(playerid, PlayerColorPage[playerid]);
            return 1;
        }
        if(strcmp(tmp, "ColorID", true) == 0) {
            tmp = strtok(inputtext, idx);
            new id = strval(tmp);
            SetPlayerColor(playerid, PlayerColors[id]);
            SendClientMessage(playerid, Color_White, "[系统]修改颜色成功");
        }
    }
    return 1;
}

Dialog:Ban_Choose(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        if(!AccountCheck(inputtext)) {
            SCM(playerid, Color_Red, "[Ban] 操作失败,该玩家账号不存在.");
            return 1;
        }
        format(aInfo[playerid][WannaBanName], MAX_PLAYER_NAME, "%s", inputtext);
        Dialog_Show(playerid, Ban_ChooseReason, DIALOG_STYLE_LIST, "封杀天数", "{04FB04}1天\n{CAFB04}3天\n{E6FF00}7天\n{F2970D}30天\n{FF0000}365天", "确定", "取消");
        return 1;
    }
    return 1;
}
Dialog:Ban_ChooseReason(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        switch (listitem) {
            case 0:{
                aInfo[playerid][WannaBanTime] = 1;
                Dialog_Show(playerid, Ban_ChooseDays, DIALOG_STYLE_LIST, "封杀原因", "赛车作弊/外挂/花屏/喷车\n恶意骚扰等", "确定", "取消");
                return 1;
            }
            case 1:{
                aInfo[playerid][WannaBanTime] = 3;
                Dialog_Show(playerid, Ban_ChooseDays, DIALOG_STYLE_LIST, "封杀原因", "赛车作弊/外挂/花屏/喷车\n恶意骚扰等", "确定", "取消");
                return 1;
            }
            case 2:{
                aInfo[playerid][WannaBanTime] = 7;
                Dialog_Show(playerid, Ban_ChooseDays, DIALOG_STYLE_LIST, "封杀原因", "赛车作弊/外挂/花屏/喷车\n恶意骚扰等", "确定", "取消");
                return 1;
            }
            case 3:{
                aInfo[playerid][WannaBanTime] = 30;
                Dialog_Show(playerid, Ban_ChooseDays, DIALOG_STYLE_LIST, "封杀原因", "赛车作弊/外挂/花屏/喷车\n恶意骚扰等", "确定", "取消");
                return 1;
            }
            case 4:{
                aInfo[playerid][WannaBanTime] = 365;
                Dialog_Show(playerid, Ban_ChooseDays, DIALOG_STYLE_LIST, "封杀原因", "赛车作弊/外挂/花屏/喷车\n恶意骚扰等", "确定", "取消");
                return 1;
            }
        }
    }
    return 1;
}
Dialog:Ban_ChooseDays(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        switch (listitem) {
            case 0:{
                new msgs[256];
                new t = gettime() + 86400 * aInfo[playerid][WannaBanTime];
                mysql_format(g_Sql, msgs, sizeof(msgs), "UPDATE `users` SET `BanTime` = %d,`BanReason` = 999 WHERE `Name` = '%e'", t, aInfo[playerid][WannaBanName]);
                mysql_pquery(g_Sql, msgs);
                format(msgs, sizeof(msgs), "[系统] %s 把 %s 封杀了 %d 天,原因:使用第三方辅助严重影响了游戏公平性 ", GetName(playerid), aInfo[playerid][WannaBanName], aInfo[playerid][WannaBanTime]);
                SCMALL(Color_Red, msgs);
                for (new i = GetPlayerPoolSize(); i >= 0; i--) {
                    if(IsPlayerConnected(i)) {
                        if(PlayerInfo[i][Login] == true) {
                            if(strcmp(GetName(i), aInfo[playerid][WannaBanName]) == 0) {
                                PlayerInfo[i][BanTime] = t;
                                // SetTimerEx("KickEx", 250, false, "i", i);
                                DelayedKick(i);
                                break;
                            }
                        }
                    }
                }
                format(msgs, sizeof(msgs), "[管理员] %s 使用了命令 Ban. ", GetName(playerid));
                SCMToAdmins(Color_AdminMessage, msgs);
                AdminCommandRecord(playerid, "Ban", "系统检测到最近使用第三方辅助强烈影响游戏公平性");
                format(aInfo[playerid][WannaBanName], MAX_PLAYER_NAME, "");
                aInfo[playerid][WannaBanTime] = -1;
                aInfo[playerid][WannaBanReason] = -1;
                return 1;
            }
            case 1:{
                new str[256];
                new t = gettime() + 86400 * aInfo[playerid][WannaBanTime];
                mysql_format(g_Sql, str, sizeof(str), "UPDATE `users` SET `BanTime` = %d,`BanReason` = '999' WHERE `Name` = '%e'", t, aInfo[playerid][WannaBanName]);
                mysql_pquery(g_Sql, str);
                
                format(str, sizeof(str), "[系统] %s 把 %s 封杀了 %d 天,原因:恶意骚扰玩家或欺诈骗钱 ", GetName(playerid), aInfo[playerid][WannaBanName], aInfo[playerid][WannaBanTime]);
                SCMALL(Color_Red, str);
                for (new i = GetPlayerPoolSize(); i >= 0; i--) {
                    if(IsPlayerConnected(i)) {
                        if(PlayerInfo[i][Login] == true) {
                            if(strcmp(GetName(i), aInfo[playerid][WannaBanName]) == 0) {
                                PlayerInfo[i][BanTime] = t;
                                // SetTimerEx("KickEx", 250, false, "i", i);
                                DelayedKick(i);
                            }
                        }
                    }
                }
                format(str, sizeof(str), "[管理员] %s 使用了命令 Ban. ", GetName(playerid));
                SCMToAdmins(Color_AdminMessage, str);
                AdminCommandRecord(playerid, "Ban", "恶意骚扰玩家或欺诈骗钱");
                format(aInfo[playerid][WannaBanName], MAX_PLAYER_NAME, "None");
                aInfo[playerid][WannaBanTime] = -1;
                aInfo[playerid][WannaBanReason] = -1;
                return 1;
            }
        }
    }
    return 1;
}
Dialog:Ban_Unban(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        if(!AccountCheck(inputtext)) {
            SCM(playerid, Color_Red, "[Ban] 操作失败,该玩家账号不存在.");
            return 1;
        }
        new str[128];
        mysql_format(g_Sql, str, sizeof(str), "UPDATE `users` SET `BanTime` = 0,`BanReason` = -1 WHERE `Name` = '%e'", inputtext);
        mysql_pquery(g_Sql, str);
        format(str, sizeof(str), "[系统] %s 把 %s 的帐号 解封了", GetName(playerid), inputtext);
        SCMALL(Color_Red, str);
        format(str, sizeof(str), "[管理员] %s 使用了命令 UnBan. ", GetName(playerid));
        SCMToAdmins(Color_AdminMessage, str);
        AdminCommandRecord(playerid, "UnBan", "解封");
        return 1;
    }
    return 1;
}

stock GetIP(playerid)//获取IP
{
	new IP[19];
	GetPlayerIp(playerid,IP,sizeof(IP));
	return IP;
}

stock IsPlayerAlreadyConnected(playerid)
{
	new ip[19];
	format(ip, sizeof ip, GetIP(playerid));
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i) == 1 && !(IsPlayerNPC(i)))
		{
			if(i != playerid && mk_strcmp(GetIP(i), ip) == 0)
			{
				return i;
			}
		}
	}
	return -1;
}



stock IsAlreadyEmailed(const mailQuery[]) {
    new Cache:result;
    new query[128];
    mysql_format(g_Sql, query, sizeof(query), "SELECT `email` FROM `users` where `email` = '%e'", mailQuery);
    result = mysql_query(g_Sql, query);
    new cut = cache_num_rows();
    cache_delete(result);
    if(cut) return true;
    return false;
}

stock PlayerPlaySoundEx(const playerid, const soundid) {
    new Float:pX, Float:pY, Float:pZ;
    GetPlayerPos(playerid, pX, pY, pZ);
    PlayerPlaySound(playerid, soundid, pX, pY, pZ);
    return 1;
}


static randSounds[7] = {
    1062,
    1068,
    1076,
    1097,
    1183,
    1185,
    1187
};
// 登录随机移动背景并播放音乐
function LoginMusicCamera(const playerid) {
    new rand = random(sizeof(randSounds));
    PlayerPlaySoundEx(playerid, randSounds[rand]);
    InterpolateCameraPos(playerid, random(6000) - 3000, random(6000) - 3000, random(120) + 50, random(6000) - 3000, random(6000) - 3000, random(120) + 50, 60000, CAMERA_MOVE);
}
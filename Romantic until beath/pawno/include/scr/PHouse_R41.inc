//ushio_p
//dylan
//修改、转载请保留ID
// #include <a_samp>
// #include <mkstr>
// #include <streamer>
//P-House定义区

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

//  相信我，能在开源里写鸣谢前辈的人的人真的是少之又少。
//  本服修改自5F，Prace,PHouse,Goods等开源 特别鸣谢：[Ghost]Rui ushio_p [Ghost]Dylan
//  致敬自由居民区开源,制作者:[ITC]dyq  [ITC]fangye  [ITC]Super_wlc [ITC]RR_LXD  mk124  Shindo(aka. ssh)  vvg, yezizhu(aka. yzz)
//  特别鸣谢 ryddawn 技术及OBJ指导；[Fire]KiVen JoshenKM
//  特别鸣谢 GoodsSys Episodes KiVen juse
//  特别鸣谢 那些始终支持并使用Shoebill的人。  
//  特别鸣谢 GTAUN GTABBS 
// RST团队服务器开发主要由[R_ST]Hygen（YuCarl77）领导

#define PHOUSE_CFG "PHouse/config.cfg" //配置文件的路径
// #define PHOUSE_DB "PHouse/house.db" //数据库的路径
#define PHOUSE_TXTLEN 40 //房子文档名字的最大长度
// #define 0xFF0000AA 0xAA3333AA
// #define COLOR_YELLOW 0xFFFF00AA

// #include <izcmd>

// new DB:data;
//P-House Config函数区
new config_Debug = 0; //debug模式是否开启,0不开,1开启,配置文件中设置
new config_Noobj = 0; //是否禁用文档内的obj,0不禁用,1禁用
new config_Objdis = 400; //默认OBJ显示距离,如果obj标记中不包含绘制距离,则使用该距离绘制
new config_Autoloadtxt = 1; //是否自动加载所有文档,0表示不自动加载,1表示自动加载
new config_Nosell = 0; //是否禁用文档内的sell,0不禁用,1禁用
new config_Selldis = 20; //默认sell显示距离,如果sell标记中不包含绘制距离，则使用该距离绘制
new config_Acl = 1; //是否使用acl,0不使用,1使用
new config_Changeobjcommandrange = 3; //默认替换类型obj的操作范围
new config_Nochangeobj = 0; //是否禁用文档内的changeobj,0不禁用,1禁用
new config_Nomoveobj = 0; //是否禁用文档内的moveobj,0不禁用,1禁用
new config_Moveobjcommandrange = 3; //默认移动类型obj的操作范围
// new PlayerText:pAreaDraw[MAX_PLAYERS]; //区域的显示



new SelectHousePage[MAX_PLAYERS];//选择房屋页 2020.2.24 By YuCarl77

stock LoadConfigValue() {
    new File:c = fopen(PHOUSE_CFG, io_read), info[512], msg[128], idx = 0, v, d;
    while (fread(c, info, sizeof(info))) {
        msg = strtok(info, idx);
        v = strval(strtok(info, idx));
        if(mk_strcmp(msg, "Debug") == 0) config_Debug = v, d = 1;
        if(mk_strcmp(msg, "Noobj") == 0) config_Noobj = v, d = 1;
        if(mk_strcmp(msg, "Objdis") == 0) config_Objdis = v, d = 1;
        if(mk_strcmp(msg, "Autoloadtxt") == 0) config_Autoloadtxt = v, d = 1;
        if(mk_strcmp(msg, "Nosell") == 0) config_Nosell = v, d = 1;
        if(mk_strcmp(msg, "Selldis") == 0) config_Selldis = v, d = 1;
        if(mk_strcmp(msg, "Acl") == 0) config_Acl = v, d = 1;
        if(mk_strcmp(msg, "Changeobjcommandrange") == 0) config_Changeobjcommandrange = v, d = 1;
        if(mk_strcmp(msg, "Nochangeobj") == 0) config_Nochangeobj = v, d = 1;
        if(mk_strcmp(msg, "Nomoveobj") == 0) config_Nomoveobj = v, d = 1;
        if(mk_strcmp(msg, "Moveobjcommandrange") == 0) config_Moveobjcommandrange = v, d = 1;
        if(d == 1) printf("[PHouse]配置:%s %i", msg, v);
        idx = 0, d = 0;
    }
    fclose(c);
}

stock Initialize_PHouse() {
    // print("[PHouse]加载");
    // if(fexist(PHOUSE_DB) == 0) {
    //     print("[PHouse]未找到数据库'house.db',导入功能将无法使用");
    // } else {
    //     data = db_open(PHOUSE_DB);
    //     db_free_result(db_query(data, "PRAGMA synchronous = NORMAL"));
    //     /*PRAGMA synchronous = FULL; (2)最慢
	// 		PRAGMA synchronous = NORMAL; (1)一般
	// 		PRAGMA synchronous = OFF; (0)最快*/
    // }
    if(fexist(PHOUSE_CFG) == 0) {
        print("[PHouse]找不到配置文件'config.cfg',将使用默认配置.");
    } else {
        LoadConfigValue();
        print("[PHouse]配置加载完毕,0表示该功能关闭,1表示该功能开启.");
        if(config_Autoloadtxt == 1) LoadAllTxt(-1);
    }
}

stock ReLoadConfig(const playerid) {
    if(fexist(PHOUSE_CFG) == 0) {
        SendClientMessage(playerid, Color_White, "[PHouse]找不到配置文件'config.cfg',停止重新载入,将使用之前的配置.");
        // print("");
    } else {
        UnLoadAllTxt(playerid);
        LoadConfigValue();
        SendClientMessage(playerid, Color_White, "[PHouse]配置重载完毕");
        LoadAllTxt(playerid);
    }
}

//P-House 普通Obj函数区
#define MAX_OBJ 100000 //OBJ最大数量 默认10000
enum Objtype {
    obj_dbid, //该普通obj在该文档数据表内的id
    obj_txt[PHOUSE_TXTLEN], //该普通obj所属文档
    obj_mod, //该普通obj的模型
    obj_id, //普通obj创建的返回id
    Float:obj_x, //....x
    Float:obj_y, //....y
    Float:obj_z, //....z
    Float:obj_rx, //....rx
    Float:obj_ry, //....ry
    Float:obj_rz, //....rz
    Float:obj_dis
}; //....显示距离

new p_Obj[MAX_OBJ][Objtype]; //普通OBJ数组
stock AddObj(const str[], const txt[], const dbid) //添加一只普通obj
{
    new id = -1;
    for (new i = 0; i < MAX_OBJ; i++) {
        if(p_Obj[i][obj_dbid] == 0) {
            id = i;
            i = MAX_OBJ + 1;
        }
    }

    if(id == -1) return print("[PHouse]普通OBJ数量已达上线,加载失败");
    new idx = 0;
    p_Obj[id][obj_dbid] = dbid;
    format(p_Obj[id][obj_txt], PHOUSE_TXTLEN, "%s", txt);
    p_Obj[id][obj_mod] = strval(strtok(str, idx));
    p_Obj[id][obj_x] = floatstr(strtok(str, idx));
    p_Obj[id][obj_y] = floatstr(strtok(str, idx));
    p_Obj[id][obj_z] = floatstr(strtok(str, idx));
    p_Obj[id][obj_rx] = floatstr(strtok(str, idx));
    p_Obj[id][obj_ry] = floatstr(strtok(str, idx));
    p_Obj[id][obj_rz] = floatstr(strtok(str, idx));
    p_Obj[id][obj_dis] = floatstr(strtok(str, idx));
    if(p_Obj[id][obj_dis] <= 0) p_Obj[id][obj_dis] = config_Objdis;
    //new msg[128];
    //	format(msg,128,"%s|mod:%i|x:%f|y:%f|z:%f|rx:%f|ry:%f|rz:%f|dis:%f|dbid:%i",p_Obj[id][obj_txt],p_Obj[id][obj_mod],p_Obj[id][obj_x],p_Obj[id][obj_y],p_Obj[id][obj_z],p_Obj[id][obj_rx],p_Obj[id][obj_ry],p_Obj[id][obj_rz],p_Obj[id][obj_dis],p_Obj[id][obj_dbid]);
    //	DebugMsg(msg);
    if(config_Noobj == 0) p_Obj[id][obj_id] = CreateDynamicObjectEx(p_Obj[id][obj_mod], p_Obj[id][obj_x], p_Obj[id][obj_y], p_Obj[id][obj_z], p_Obj[id][obj_rx], p_Obj[id][obj_ry], p_Obj[id][obj_rz], p_Obj[id][obj_dis], p_Obj[id][obj_dis], {0});
    return 1;
}
//material object add by Dylan
stock AddMaterialObj(const info[])
{
    new id = -1;
    for(new i = 0; i < MAX_OBJ; i++)
    {
        if(p_Obj[i][obj_dbid] == 0)
        {
            id = i - 1;
            i = MAX_OBJ + 1;
        }
    }
    if(id == -1) return printf("PHouse * AddMaterialObj id = -1");
    new index, modelid, txdname[MAX_PLAYER_NAME], texturename[MAX_PLAYER_NAME], color, idx;
    index = strval(strtok(info, idx));
    modelid = strval(strtok(info, idx));
    format(txdname, MAX_PLAYER_NAME, strtok(info, idx));
    format(texturename, MAX_PLAYER_NAME, strtok(info, idx));
    color = strval(strtok(info, idx));
    SetDynamicObjectMaterial(p_Obj[id][obj_id],index, modelid, txdname, texturename, color);
    return 1;
}
stock RedObj(const index) {
    new id = index;
    if(p_Obj[id][obj_dbid] == 0) return 1;
    p_Obj[id][obj_dbid] = 0;
    format(p_Obj[id][obj_txt], PHOUSE_TXTLEN, "");
    if(p_Obj[id][obj_id] != 0) DestroyDynamicObject(p_Obj[id][obj_id]), p_Obj[id][obj_id] = 0;
    return 1;
}

stock RedAllObj(const txt[]) {
    for (new i = 0; i < MAX_OBJ; i++) {
        if(mk_strcmp(p_Obj[i][obj_txt], txt) == 0) {
            RedObj(i);
        }
    }
}


//P-House 加载函数区
#define MAX_LOADTXT 1000//最大文档加载数量 默认100
enum Loadtype {
    load_name[PHOUSE_TXTLEN], //文档名字
        load_id
}; //文档在数据库中的id
new p_Txtlist[MAX_LOADTXT][Loadtype]; //文档列表
stock LoadTxt(const txt[],const playerid) {
    if(IsTxtInstallation(txt) == 0) 
    {
        new msg[128];
        format(msg, sizeof(msg), "[PHouse]文档'%s'加载失败,因为他没被安装",txt);
        return SendClientMessage(playerid, Color_White, msg);
    }
    if(IsLoadTxt(txt) != -1) {
        new msg[128];
        format(msg, sizeof(msg), "[PHouse]文档'%s'加载失败,因为他已经被加载了",txt);
        return SendClientMessage(playerid, Color_White, msg);
    }
    new id = -1;
    for (new i = 0; i < MAX_LOADTXT; i++) {
        if(p_Txtlist[i][load_id] == 0) {
            id = i;
            i = MAX_LOADTXT + 1;
        }
    }
    if(id == -1) return SendClientMessage(playerid, Color_White, "[PHouse]文档数量已达上线,加载失败");
    format(p_Txtlist[id][load_name], PHOUSE_TXTLEN, "%s", txt);
    p_Txtlist[id][load_id] = GetTxtInstallationId(txt);
    new Query[128], Cache:result;
    mysql_format(g_Sql, Query, sizeof(Query), "SELECT * FROM `%i`;", p_Txtlist[id][load_id]);
    result = mysql_query(g_Sql, Query);
    new s = cache_num_rows();
    new t = GetTickCount();
    new sellf = 0;
    for (new i = 0; i < s; i++) {
        new did, type[128], value[256];
        cache_get_value_index_int(i, 0, did);
        cache_get_value_index(i, 1, type);
        cache_get_value_index(i, 2, value);
        if(mk_strcmp(type, "obj") == 0) AddObj(value, txt, did);
        if(mk_strcmp(type, "sell") == 0 && sellf == 0) AddSell(p_Txtlist[id][load_id], txt, value, did), sellf = 1;
        if(mk_strcmp(type, "changeobj") == 0) AddChangeObj(txt, value, did);
        if(mk_strcmp(type, "moveobj") == 0) AddMoveObj(txt, value, did);
        if(mk_strcmp(type, "material") == 0) AddMaterialObj(value); //新增支持纹理
        if(mk_strcmp(type, "area") == 0) AddArea(value,txt); //新增支持area显示
    }
    cache_delete(result);
    new msg[128];
    format(msg, sizeof(msg),"[PHouse]文档'%s'加载完成,耗时:%ims", txt, GetTickCount() - t);
    if(playerid >= 0) SendClientMessage(playerid, Color_White, msg);
    else DebugMsg(msg);
    Streamer_UpdateAll();
    return 1;
}
stock UnLoadTxt(const txt[],const playerid) {
    new id = IsLoadTxt(txt), t = GetTickCount();
    if(id == -1) {
        new msg[128];
        format(msg, sizeof(msg), "[PHouse]文档'%s'卸载失败,因为他没有被加载", txt);
        return SendClientMessage(playerid, Color_White, msg);
    }
    format(p_Txtlist[id][load_name], PHOUSE_TXTLEN, "");
    p_Txtlist[id][load_id] = 0;
    //删除该文档内的所有普通obj
    RedAllObj(txt);
    //删除该文档内的所有sell
    RedAllSell(txt);
    //删除该文档内的所有替换类obj
    RedAllChangeObj(txt);
    //删除该文档内的所有移动类obj
    RedAllMoveObj(txt);
    new msg[128];
    format(msg, sizeof(msg), "[PHouse]卸载文档'%s'完成,耗时:%ims", txt, GetTickCount() - t);
    // printf();
    SendClientMessage(playerid, Color_White, msg);
    return 1;
}

stock IsLoadTxt(const txt[]) {
    new id = -1;
    for (new i = 0; i < MAX_LOADTXT; i++) {
        if(mk_strcmp(p_Txtlist[i][load_name], txt) == 0) {
            id = i;
            i = MAX_LOADTXT + 1;
        }
    }
    return id;
}

stock GetLoadTxt(const playerid) {
    printf("[PHouse]已加载文档列表:");
    new s = 0;
    new msg[128];
    for (new i = 0; i < MAX_LOADTXT; i++) {
        if(mk_strcmp(p_Txtlist[i][load_name], "") != 0) {
            format(msg, sizeof(msg), "[PHouse]文档名称:%s,文档数据库表ID:%i,文档索引:%i", p_Txtlist[i][load_name], p_Txtlist[i][load_id], i);
            SendClientMessage(playerid, Color_White, msg);
            // printf();
            s++;
        }
    }
    printf("[PHouse]共%i个", s);
}

stock LoadAllTxt(const playerid) {
    new Query[128], Cache:result;
    mysql_format(g_Sql, Query, sizeof(Query), "SELECT * FROM `main`");
    result = mysql_query(g_Sql, Query);
    new t = GetTickCount();
    new s = cache_num_rows();
    for (new i = 0; i < s; i++) {
        new txtname[PHOUSE_TXTLEN];
        cache_get_value_index(i, 1, txtname);
        LoadTxt(txtname, playerid);
    }
    cache_delete(result);
    new msg[128];
    format(msg, sizeof(msg), "[PHouse]所有文档加载完成,耗时:%ims", GetTickCount() - t);
    if(playerid != -1) SendClientMessage(playerid, Color_White, msg);
    else DebugMsg(msg);
    return 1;
}
stock UnLoadAllTxt(const playerid) {
    new msg[PHOUSE_TXTLEN];
    for (new i = 0; i < MAX_LOADTXT; i++) {
        if(mk_strcmp(p_Txtlist[i][load_name], "") != 0) {
            format(msg, sizeof(msg), "%s", p_Txtlist[i][load_name]);
            UnLoadTxt(msg, playerid);
        }
    }
}

//P-House 安装(导入)函数区
stock Installation(const txt[],const playerid) {
    new file[128];
    format(file, 128, "PHouse/txt/%s.txt", txt);
    if(fexist(file) == 0) 
    {
        new msg[128];
        format(msg, sizeof(msg), "[PHouse]找不到文档'%s',停止安装'%s'", txt, txt);
        DebugMsg(msg);
        return SendClientMessage(playerid, Color_White, msg);
    }
    if(IsTxtInstallation(txt) != 0) {
        new msg[128];
        format(msg, sizeof(msg), "[PHouse]文档'%s'停止安装,因为该文档已经被安装过了", txt);
        DebugMsg(msg);
        return SendClientMessage(playerid, Color_White, msg);
    }
    new File:c = fopen(file, io_read), info[1024], msg[256], idx = 0, t = GetTickCount(), iid;

    mysql_tquery(g_Sql, "START TRANSACTION;");
    mysql_format(g_Sql, msg, sizeof(msg), "INSERT INTO `main` (`txtname`) VALUES ('%e');", txt);
    mysql_tquery(g_Sql, msg);
    // iid = GetTxtInstallationId(txt);
    mysql_format(g_Sql, msg, sizeof(msg), "CREATE TABLE `%i`(`id` INT PRIMARY KEY AUTO_INCREMENT,`type` VARCHAR(127),`value` VARCHAR(255));", cache_insert_id());
    mysql_tquery(g_Sql, msg);

    new allmsg[64], jump = 0, one, two;
    format(allmsg, 64, "");
    while (fread(c, info, sizeof(info))) {
        msg = strtok(info, idx);
        if(mk_strcmp(msg, "allobj") == 0) format(allmsg, 64, "obj"), jump = 1;
        if(mk_strcmp(msg, "allchangeobj") == 0) format(allmsg, 64, "changeobj"), jump = 1;
        if(mk_strcmp(msg, "allmoveobj") == 0) format(allmsg, 64, "moveobj"), jump = 1;
        one = msg[0];
        two = msg[1];
        if(one == 47) {
            if(two == 47) jump = 1;
        }
        if(mk_strcmp(msg, "//") != 0 && mk_strcmp(msg, "user") != 0 && mk_strcmp(msg, "name") != 0 && mk_strcmp(msg, "") != 0 && jump == 0) {
            if(CheckVar(msg) == 1) {
                mysql_format(g_Sql, msg, sizeof(msg), "INSERT INTO `%i` (`type`,`value`) VALUES ('%e','%e %e');", iid, allmsg, msg, strrest(info, idx));
            } else {
                mysql_format(g_Sql, msg, sizeof(msg), "INSERT INTO `%i` (`type`,`value`) VALUES ('%e','%e');", iid, msg, strrest(info, idx));
            }
            //print(msg);
            mysql_tquery(g_Sql, msg);
        }
        idx = 0;
        jump = 0;
    }
    fclose(c);
    mysql_tquery(g_Sql, "COMMIT;");
    format(msg, sizeof(msg), "[PHouse]文档'%s'已安装", txt);
    SendClientMessage(playerid, Color_White, msg);
    format(msg, sizeof(msg), "[PHouse]文档'%s'安装耗时:%ims", txt, GetTickCount() - t);
    DebugMsg(msg);
    LoadTxt(txt,playerid);
    return 1;
}

stock UnInstallation(const txt[],const playerid) {
    if(IsTxtInstallation(txt) == 0) {
        new msg[128];
        format(msg, sizeof(msg), "[PHouse]文档'%s'停止删除，因为该文档没有被安装", txt);
        return SendClientMessage(playerid, Color_White, msg);
    }
    if(IsLoadTxt(txt) != -1) {
        new msg[128];
        format(msg, sizeof(msg), "[PHouse]文档'%s'停止删除,因为该文档没有被卸载", txt);
        return SendClientMessage(playerid, Color_White, msg);
    }
    new msg[256], t = GetTickCount();
    new iid = GetTxtInstallationId(txt);
    mysql_tquery(g_Sql, "START TRANSACTION;");

    mysql_format(g_Sql, msg, sizeof(msg), "DROP TABLE `%i`;", iid);
    mysql_tquery(g_Sql, msg);

    mysql_format(g_Sql, msg, sizeof(msg), "DELETE FROM `main` WHERE `txtname` = '%e';", txt);
    mysql_tquery(g_Sql, msg);

    mysql_tquery(g_Sql, "COMMIT;");
    // printf("[PHouse]文档'%s'已删除", txt);
    format(msg, 128, "[PHouse]删除文档'%s'耗时:%ims", txt, GetTickCount() - t);
    DebugMsg(msg);
    return 1;
}
// 这个可能会因为在线程中调用非线程出现问题
stock IsTxtInstallation(const txt[]) {
    new Cache:result, s, Query[256];
    mysql_format(g_Sql, Query, sizeof(Query), "SELECT * FROM `main` WHERE `txtname`='%e'", txt);
    result = mysql_query(g_Sql, Query);
    s = cache_num_rows();
    cache_delete(result);
    return s;
}
// 这个可能会因为在线程中调用非线程出现问题
stock GetTxtInstallationId(const txt[]) {
    new Cache:result, s, Query[256];
    mysql_format(g_Sql, Query, sizeof(Query), "SELECT * FROM `main` WHERE `txtname`='%e'", txt);
    result = mysql_query(g_Sql, Query);
    cache_get_value_index_int(0, 0, s);
    cache_delete(result);
    return s;
}


//P-House sell函数区
//1273 - 绿牌子
//1272 - 蓝牌子
#define MAX_SELL MAX_LOADTXT//最大出售牌子数量(与最大文档数量相等,一个文档仅有一个sell能生效)
#define MAX_SELL_NAME 60//最大sell名字长度
enum Selltype {
    sell_dbid, //该房子在文档库中的id
    sell_txt[PHOUSE_TXTLEN], //该房子所属文档
    sell_player[MAX_PLAYER_NAME], //...房子拥有者 空白表示当前没人买 可用for判定玩家买的几套房以及显示玩家的房子
    sell_name[MAX_SELL_NAME], //sell的名字
    sell_money, //...价格
    sell_pickupid, //...pickupid
    Text3D:sell_text, //...3dtxt提示
    Float:sell_x, //...x
    Float:sell_y, //...y
    Float:sell_z, //...z
    Float:sell_dis,
    sell_mapicon, //2020.2.11 By YuCarl77/[R_ST]Hygen 用于保存mapicon的id以及更新
        /*area add by Dylan*/
    Float:area_x,
    Float:area_y,
    Float:area_r,//r = range
    area_id
}; //...距离
new p_Sell[MAX_SELL][Selltype]; //出售标记的数组
stock ChangeSellPlayer(const index, const newplayer[]) {
    format(p_Sell[index][sell_player], MAX_PLAYER_NAME, "%s", newplayer);
    new msg[512], tid = IsLoadTxt(p_Sell[index][sell_txt]), t = GetTickCount();

    mysql_format(g_Sql, msg, sizeof(msg), "UPDATE `%i` SET value = `%e` WHERE `type` = 'user'", p_Txtlist[tid][load_id], p_Sell[index][sell_player]);
    mysql_pquery(g_Sql, msg);

    UpDataSellPickup(index);
    format(msg, sizeof(msg), "[PHouse]修改文档'%s'的牌子完成,耗时:%ims", p_Txtlist[tid][load_name], GetTickCount() - t);
    DebugMsg(msg);

}
stock GetSellPlayer(const txt, const txtname[]) {
    new Cache:result, s, msg[512], st = GetTickCount(), rm[MAX_PLAYER_NAME];
    mysql_format(g_Sql, msg, sizeof(msg), "SELECT * FROM `%i` WHERE `type`='user'", txt);
    result = mysql_query(g_Sql, msg);
    s = cache_num_rows();
    if(s == 0) {
        mysql_format(g_Sql, msg, sizeof(msg), "INSERT INTO `%i` (`type`,`value`) VALUES ('user','')", txt);
        mysql_pquery(g_Sql, msg);
        format(rm, MAX_PLAYER_NAME, "");
    } else {
        cache_get_value_index(0, 2, rm, MAX_PLAYER_NAME);
    }

    cache_delete(result);
    format(msg, sizeof(msg), "[PHouse]文档'%s'查询所有者完成,耗时:%ims", txtname, GetTickCount() - st);
    DebugMsg(msg);
    return rm;
}
stock ChangeSellName(const index, const newname[]) {
    if(mk_strlen(newname) >= MAX_SELL_NAME) return -1;
    format(p_Sell[index][sell_name], MAX_SELL_NAME, "%s", newname);
    new msg[512], tid = IsLoadTxt(p_Sell[index][sell_txt]), at = GetTickCount();
    mysql_format(g_Sql, msg, sizeof(msg), "UPDATE `%i` SET value = '%e' WHERE type = 'name'", p_Txtlist[tid][load_id], p_Sell[index][sell_name]);
    mysql_pquery(g_Sql, msg);
    UpDataSellPickup(index);
    format(msg, sizeof(msg), "[PHouse]修改文档'%s'的姓名完成,耗时:%ims", p_Txtlist[tid][load_name], GetTickCount() - at);
    DebugMsg(msg);
    return 1;
}
stock GetSellName(const txt, const txtname[]) {
    new Cache:result, s, msg[512], t = GetTickCount(), rm[MAX_SELL_NAME];
    mysql_format(g_Sql, msg, sizeof(msg), "SELECT * FROM `%i` WHERE `type`='name'", txt);
    result = mysql_query(g_Sql, msg);
    s = cache_num_rows();
    if(s == 0) {
        mysql_format(g_Sql, msg, sizeof(msg), "INSERT INTO `%i` (`type`,`value`) VALUES ('name','')", txt);
        mysql_pquery(g_Sql, msg);
        format(rm, MAX_SELL_NAME, "");
    } else {
        cache_get_value_index(0, 2, rm, MAX_PLAYER_NAME);
    }
    cache_delete(result);
    format(msg, sizeof(msg), "[PHouse]文档'%s'查询sell姓名完成,耗时:%ims", txtname, GetTickCount() - t);
    DebugMsg(msg);
    return rm;
}
stock AddSell(const txtid, const txt[], const value[], const dbid) {
    new id = -1;
    for (new i = 0; i < MAX_SELL; i++) {
        if(p_Sell[i][sell_dbid] == 0) {
            id = i;
            i = MAX_SELL + 1;
        }
    }
    if(id == -1) return print("[PHouse]出售标记已达上限,添加失败");
    p_Sell[id][sell_dbid] = dbid;
    format(p_Sell[id][sell_txt], PHOUSE_TXTLEN, "%s", txt);
    new idx = 0;
    p_Sell[id][sell_money] = strval(strtok(value, idx));
    p_Sell[id][sell_x] = floatstr(strtok(value, idx));
    p_Sell[id][sell_y] = floatstr(strtok(value, idx));
    p_Sell[id][sell_z] = floatstr(strtok(value, idx));
    p_Sell[id][sell_dis] = floatstr(strtok(value, idx));
    format(p_Sell[id][sell_player], MAX_PLAYER_NAME, "%s", GetSellPlayer(txtid, txt));
    format(p_Sell[id][sell_name], MAX_SELL_NAME, "%s", GetSellName(txtid, txt));
    if(p_Sell[id][sell_dis] <= 0) p_Sell[id][sell_dis] = config_Selldis;
    if(config_Nosell == 0) {
        p_Sell[id][sell_text] = CreateDynamic3DTextLabelEx(" ", 0x98CDFEFF, p_Sell[id][sell_x], p_Sell[id][sell_y], p_Sell[id][sell_z], p_Sell[id][sell_dis], INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, p_Sell[id][sell_dis]);
        UpDataSellPickup(id);
    }
    return 1;
}

stock RedSell(const index) {
    new id = index;
    if(p_Sell[id][sell_dbid] == 0) return 1;
    p_Sell[id][sell_dbid] = 0;
    format(p_Sell[id][sell_txt], PHOUSE_TXTLEN, "");
    DestroyDynamic3DTextLabel(p_Sell[id][sell_text]);
    if(p_Sell[id][sell_pickupid] != 0) {
        DestroyDynamicPickup(p_Sell[id][sell_pickupid]);
        p_Sell[id][sell_pickupid] = 0;
    }
    if(p_Sell[id][sell_mapicon] != 0) { //2020.2.11写  用于删除mapicon
        DestroyDynamicMapIcon(p_Sell[id][sell_mapicon]);
        p_Sell[id][sell_mapicon] = 0;
    }
    return 1;
}

stock GetSellMax() { //2020.2.24发现 可能是说有多少个房子正在售卖吧？还是说有多少个房子？
    new s = 0;
    for (new i = 0; i < MAX_SELL; i++) {
        if(p_Sell[i][sell_dbid] != 0) {
            s++;
        }
    }
    return s;
}

stock GotoSell(const playerid, const index) {
    if(p_Sell[index][sell_dbid] == 0) return -1;
    SetPlayerPos(playerid, p_Sell[index][sell_x], p_Sell[index][sell_y], p_Sell[index][sell_z]);
    return 1;
}

stock GetPlayerInSell(const playerid) {
    if(config_Nosell == 1) return -2;
    new id = -1;
    for (new i = 0; i < MAX_SELL; i++) {
        if(p_Sell[i][sell_dbid] != 0) {
            if(GetPlayerDistanceFromPoint(playerid, p_Sell[i][sell_x], p_Sell[i][sell_y], p_Sell[i][sell_z]) <= 1) {
                id = i;
                i = MAX_SELL + 1;
            }
        }
    }
    return id;
}

stock BuySell(const playerid, const index) {
    new pmoney = PlayerInfo[playerid][Cash];
    if(pmoney < p_Sell[index][sell_money]) return -1;
    new name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, MAX_PLAYER_NAME);
    PlayerInfo[playerid][Cash] = PlayerInfo[playerid][Cash] - p_Sell[index][sell_money];
    // GivePlayerMoney(playerid, -p_Sell[index][sell_money]);
    AddPlayerToTxt(name, p_Sell[index][sell_txt]);
    ChangeSellPlayer(index, name);
    return 1;
}

stock SellSell(const playerid, const index) {
    new name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, MAX_PLAYER_NAME);
    if(mk_strcmp(name, p_Sell[index][sell_player]) != 0) return -1;
    PlayerInfo[playerid][Cash] = PlayerInfo[playerid][Cash] + p_Sell[index][sell_money];
    // GivePlayerMoney(playerid, p_Sell[index][sell_money]);
    RedAllPlayerToTxt(p_Sell[index][sell_txt]);
    AddPlayerToTxt("*", p_Sell[index][sell_txt]);
    ChangeSellPlayer(index, "");
    return 1;
}

stock UpDataSellPickup(const index) {
    new id = index;
    if(p_Sell[id][sell_pickupid] != 0) {
        DestroyDynamicPickup(p_Sell[id][sell_pickupid]);
        p_Sell[id][sell_pickupid] = 0;
    }
    if(p_Sell[id][sell_mapicon] != 0) { //2020.2.11写  用于删除mapicon
        DestroyDynamicMapIcon(p_Sell[id][sell_mapicon]);
        p_Sell[id][sell_mapicon] = 0;
    }
    new pid = 1272, sm[128];
    if(mk_strcmp(p_Sell[id][sell_player], "") == 0) {
        pid = 1273;
        format(sm,128,"{00FF00}该房产出售中... (ID: %d)\n价格: %i", id+1, p_Sell[id][sell_money]);
        p_Sell[id][sell_mapicon] = CreateDynamicMapIcon(p_Sell[id][sell_x], p_Sell[id][sell_y], p_Sell[id][sell_z], 31, Color_White, 0, -1, -1);
    } else {
        format(sm,256,"%s\n{FF8000}拥有者:{33CCCC} %s\n{FF8000}价格:{33CCCC} %d\n{FF8000}ID:{33CCCC} %i",p_Sell[id][sell_name],p_Sell[id][sell_player],p_Sell[id][sell_money],id+1);
        p_Sell[id][sell_mapicon] = CreateDynamicMapIcon(p_Sell[id][sell_x], p_Sell[id][sell_y], p_Sell[id][sell_z], 32, Color_White, 0, -1, -1);
    }
    UpdateDynamic3DTextLabelText(p_Sell[id][sell_text], 0x98CDFEFF, sm);
    p_Sell[id][sell_pickupid] = CreateDynamicPickupEx(pid, 1, p_Sell[id][sell_x], p_Sell[id][sell_y], p_Sell[id][sell_z], p_Sell[id][sell_dis]);
    Streamer_UpdateAll();
}
stock RedAllSell(const txt[]) {
    for (new i = 0; i < MAX_SELL; i++) {
        if(mk_strcmp(p_Sell[i][sell_txt], txt) == 0) {
            RedSell(i);
        }
    }
}

/*Area add by Dylan*/

// new pAreaID[MAX_PLAYERS];

stock AddArea(const value[], const txt[])
{
    new id = -1;
    for(new i = 0; i < MAX_SELL; i++)
    {
        if(mk_strcmp(p_Sell[i][sell_txt], txt) == 0)
        {
            id = i; 
            i = MAX_SELL + 1;
        }
    }
    if(id == -1) return print("PHouse * area添加失败, 原因:未找到sell.");
    new idx = 0;
    p_Sell[id][area_x] = floatstr(strtok(value, idx));
    p_Sell[id][area_y] = floatstr(strtok(value, idx));
    p_Sell[id][area_r] = floatstr(strtok(value, idx));
    p_Sell[id][area_id] = CreateDynamicCircle(p_Sell[id][area_x], p_Sell[id][area_y], p_Sell[id][area_r]);
    return 1;
}

stock AreaIsPHouseArea(areaid)
{
    new id = -1;
    for(new i = 0; i < MAX_SELL; i++)
    {
        if(p_Sell[i][sell_dbid] != 0 && p_Sell[i][area_id] == areaid)
        {
            id = i;
            i = MAX_SELL + 1;
        }
    }
    return id;
}
// stock PHouse_OnPlayerDisconnect(const playerid)
// {
//     PlayerTextDrawDestroy(playerid, pAreaDraw[playerid]);
// }

// stock PHouse_PlayerEnterArea(const playerid, const areaid)
// {
//     pAreaID[playerid] = areaid;
//     new id = AreaIsPHouseArea(areaid);
//     if(id == -1) return 0;
//     PlayerTextDrawShow(playerid, pAreaDraw[playerid]);
//     if(GetPlayerAclFromTxt(GetName(playerid),p_Sell[id][sell_txt]) == 1)
//     {
//         pAreaID[playerid] = -1;
//     }
//     return 1;
// }

// stock PHouse_PlayerLevelArea(const playerid, const areaid)
// {
//     pAreaID[playerid] = -1;
//     new id = AreaIsPHouseArea(areaid);
//     if(id == -1) return 0;
//     PlayerTextDrawHide(playerid, pAreaDraw[playerid]);
//     return 1;
// }

// stock PHouse_PlayerLogin(const playerid)
// {
//     pAreaID[playerid] = -1;
//     pAreaDraw[playerid] = CreatePlayerTextDraw(playerid, 532.500000, 331.959869, "");
//     PlayerTextDrawLetterSize(playerid, pAreaDraw[playerid], 0.000000, 0.000000);
//     PlayerTextDrawTextSize(playerid, pAreaDraw[playerid], 124.000000, 96.000000);
//     PlayerTextDrawAlignment(playerid, pAreaDraw[playerid], 1);
//     PlayerTextDrawColor(playerid, pAreaDraw[playerid], -520093697);
//     PlayerTextDrawSetShadow(playerid, pAreaDraw[playerid], 0);
//     PlayerTextDrawSetOutline(playerid, pAreaDraw[playerid], 0);
//     PlayerTextDrawBackgroundColor(playerid, pAreaDraw[playerid], 0);
//     PlayerTextDrawFont(playerid, pAreaDraw[playerid], 5);
//     PlayerTextDrawSetProportional(playerid, pAreaDraw[playerid], 0);
//     PlayerTextDrawSetShadow(playerid, pAreaDraw[playerid], 0);
//     PlayerTextDrawSetPreviewModel(playerid, pAreaDraw[playerid], 4002);
//     PlayerTextDrawSetPreviewRot(playerid, pAreaDraw[playerid], 0.000000, 0.000000, 0.000000, 1.000000);
// }

//P-House Acl(文档权限)函数区
#define MAX_PAGES 20 //权限管理列表每页显示多少个人
// #define DIALOG_ACL_LIST 16859//显示权限列表的对话框ID
// #define HouseListDialog DIALOG_ACL_LIST+1 //显示房子列表的对话框ID 2020.2.24 By YuCarl77
// stock GetPages(const s) {
//     new p = s % MAX_PAGES, ps;
//     if(p == 0) {
//         ps = s / MAX_PAGES;
//     } else {
//         ps = (s / MAX_PAGES) + 1;
//     }
//     return ps;
// }
stock GetTxtPlayer(const txt[]) {
    new Cache:result, s, msg[512], tid = IsLoadTxt(txt);
    mysql_format(g_Sql, msg, sizeof(msg), "SELECT * FROM `%i` WHERE `type`='player'", p_Txtlist[tid][load_id]);
    result = mysql_query(g_Sql, msg);
    s = cache_num_rows();
    cache_delete(result);
    return s;
}

stock ShowAclList(const playerid, const txt[]) {
    new tid = IsLoadTxt(txt);
    new Query[256];
    mysql_format(g_Sql, Query, sizeof(Query), "SELECT * FROM `%i` WHERE `type`='player'", p_Txtlist[tid][load_id]);
    mysql_pquery(g_Sql, Query, "ShowAclListQuery", "ds", playerid, txt);
}
function ShowAclListQuery(playerid, txt[]) {
    new nowpage = GetPVarInt(playerid, "PHouse_AclList_NowSelect");
    new maxpage = GetMaxPage(GetTxtPlayer(txt), MAX_PAGES);
    new info[512], title[96], msg[1024];
    new startid, endid;
    if(maxpage == 0) maxpage = 1, format(msg, sizeof(msg), "* 该文档的权限表为空..\n* 添加权限请用/adp");
    if(nowpage > maxpage) {
        nowpage = maxpage;
        SetPVarInt(playerid, "PHouse_AclList_NowSelect", maxpage);
    }
    startid = MAX_PAGES * (nowpage - 1);
    endid = MAX_PAGES * nowpage;
    if(nowpage != 1) format(msg, sizeof(msg), "上一页");

    new s = cache_num_rows();
    for (new i = 0; i < s; i++) {
        if(i >= startid) {
            if(i < endid) {
                cache_get_value_index(i, 2, info);
                new uid = ReturnPlayerId(info);
                if(uid == -1) format(msg, sizeof(msg), "%s\n%s", msg, info);
                else format(msg, sizeof(msg), "%s\n{98CDFE}%s [当前在线,ID:%i]{FFFFFF}", msg, info, uid);
            }
        }
    }
    if(nowpage != maxpage) format(msg, sizeof(msg), "%s\n下一页", msg);
    format(title, 96, "权限列表,共%i名,当前第%i页,共%i页", GetTxtPlayer(txt), nowpage, maxpage);
    Dialog_Show(playerid, DIALOG_ACL_LIST, DIALOG_STYLE_LIST, title, msg, "删除", "关闭");
}

stock AddPlayerToTxt(const player[], const txt[]) {
    new Cache:result, s, msg[512], tid = IsLoadTxt(txt);
    mysql_format(g_Sql, msg, sizeof(msg), "SELECT * FROM `%i` WHERE `type`='player' and `value`='%e'", p_Txtlist[tid][load_id], player);
    result = mysql_query(g_Sql, msg);
    s = cache_num_rows();
    cache_delete(result);
    if(s == 0) {
        mysql_format(g_Sql, msg, sizeof(msg), "INSERT INTO `%i` (`type`,`value`) VALUES ('player','%e')", p_Txtlist[tid][load_id], player);
        mysql_pquery(g_Sql, msg);
        return 1;
    }
    return -1;
}
stock RedPlayerToTxt(const player[], const txt[]) {
    new Cache:result, s, msg[512], tid = IsLoadTxt(txt);
    mysql_format(g_Sql, msg, sizeof(msg), "SELECT * FROM `%i` WHERE `type`='player' and `value`='%e'", p_Txtlist[tid][load_id], player);
    result = mysql_query(g_Sql, msg);
    s = cache_num_rows();
    cache_delete(result);
    if(s == 0) return -1;
    mysql_format(g_Sql, msg, sizeof(msg), "DELETE FROM `%i` WHERE `type` = 'player' and `value`='%e'", p_Txtlist[tid][load_id], player);
    mysql_pquery(g_Sql, msg);
    return 1;
}

stock RedAllPlayerToTxt(const txt[]) {
    new msg[256], tid = IsLoadTxt(txt);
    mysql_format(g_Sql, msg, sizeof(msg), "DELETE FROM `%i` WHERE `type` = 'player'", p_Txtlist[tid][load_id]);
    mysql_pquery(g_Sql, msg);
}

stock GetPlayerAclFromTxt(const player[], const txt[]) {
    if(config_Acl == 0) return 1;
    new Cache:result, s, msg[256], tid = IsLoadTxt(txt);
    mysql_format(g_Sql, msg, sizeof(msg), "SELECT * FROM `%i` WHERE `type`='player' and `value`='*'", p_Txtlist[tid][load_id]);
    result = mysql_query(g_Sql, msg);
    s = cache_num_rows();
    cache_delete(result);
    if(s != 0) return 1;

    mysql_format(g_Sql, msg, sizeof(msg), "SELECT * FROM `%i` WHERE `type`='player' and `value`='%e'", p_Txtlist[tid][load_id], player);
    result = mysql_query(g_Sql, msg);
    s = cache_num_rows();
    cache_delete(result);
    return s;
}

forward PHouse_ChangePlayerNameToName(name[], toname[]);
public PHouse_ChangePlayerNameToName(name[], toname[]) {
    new msg[512], t = GetTickCount();
    mysql_tquery(g_Sql, "START TRANSACTION;");
    for (new i = 0; i < MAX_LOADTXT; i++) {
        if(p_Txtlist[i][load_id] != 0) {
            format(msg, sizeof(msg), "UPDATE `%i` SET `value` = '%e' WHERE `type` = 'player' or `type`='user' and `value`='%e';", p_Txtlist[i][load_id], toname, name);
            mysql_tquery(g_Sql, msg);
        }
    }
    mysql_tquery(g_Sql, "COMMIT;", "PChangePNameQuery", "dss", t, name, toname);
}
function PChangePNameQuery(t,name[],toname[]) {
    new msg[256];
    format(msg, sizeof(msg), "[PHouse]玩家权限转移完成('%s' > '%s'),耗时:%ims", name, toname, GetTickCount() - t);
    DebugMsg(msg);
    for (new i = 0; i < MAX_SELL; i++) {
        if(mk_strcmp(name, p_Sell[i][sell_player]) == 0) {
            format(p_Sell[i][sell_player], MAX_PLAYER_NAME, "%s", toname);
            UpDataSellPickup(i);
        }
    }
}

//P-House changobj(替换obj)函数区
#define MAX_CHANGEOBJ 10000//替换型obj数量上限
enum Changeobjtype {
    changeobj_dbid, //表内ID
    changeobj_txt[PHOUSE_TXTLEN], //所属文档
    changeobj_mod, //模型1
    changeobj_mod1, //模型2
    Float:changeobj_range, //操作范围
    changeobj_state, //模型状态,0为模型1,非0为模型2
    changeobj_id, //obj的id
    Float:changeobj_dis, //..dis
    Float:changeobj_x, //..x
    Float:changeobj_y, //..y
    Float:changeobj_z, //..z
    Float:changeobj_rx, //..rx
    Float:changeobj_ry, //..ry
    Float:changeobj_rz
}; //..rz
new p_Changeobj[MAX_CHANGEOBJ][Changeobjtype]; //..替换obj数组
stock AddChangeObj(const txt[], const value[], const dbid) {
    new id = -1;
    for (new i = 0; i < MAX_CHANGEOBJ; i++) {
        if(p_Changeobj[i][changeobj_dbid] == 0) {
            id = i;
            i = MAX_CHANGEOBJ + 1;
        }
    }
    if(id == -1) return print("[PHouse]替换OBJ已达上限,添加失败");
    new idx = 0;
    p_Changeobj[id][changeobj_dbid] = dbid;
    format(p_Changeobj[id][changeobj_txt], PHOUSE_TXTLEN, "%s", txt);
    p_Changeobj[id][changeobj_mod] = strval(strtok(value, idx));
    p_Changeobj[id][changeobj_x] = floatstr(strtok(value, idx));
    p_Changeobj[id][changeobj_y] = floatstr(strtok(value, idx));
    p_Changeobj[id][changeobj_z] = floatstr(strtok(value, idx));
    p_Changeobj[id][changeobj_rx] = floatstr(strtok(value, idx));
    p_Changeobj[id][changeobj_ry] = floatstr(strtok(value, idx));
    p_Changeobj[id][changeobj_rz] = floatstr(strtok(value, idx));
    p_Changeobj[id][changeobj_mod1] = strval(strtok(value, idx));
    p_Changeobj[id][changeobj_state] = strval(strtok(value, idx));
    p_Changeobj[id][changeobj_range] = floatstr(strtok(value, idx));
    p_Changeobj[id][changeobj_dis] = floatstr(strtok(value, idx));
    if(p_Changeobj[id][changeobj_range] <= 0) p_Changeobj[id][changeobj_range] = config_Changeobjcommandrange;
    if(p_Changeobj[id][changeobj_dis] <= 0) p_Changeobj[id][changeobj_dis] = config_Objdis;
    //printf("%i|%f|%f",p_Changeobj[id][changeobj_state],p_Changeobj[id][changeobj_range],p_Changeobj[id][changeobj_dis]);
    if(config_Nochangeobj == 0) {
        ShowChangeObj(id);
    }
    return 1;
}
stock RedChangeObj(const index) {
    new id = index;
    if(p_Changeobj[id][changeobj_dbid] == 0) return 1;
    p_Changeobj[id][changeobj_dbid] = 0;
    format(p_Changeobj[id][changeobj_txt], PHOUSE_TXTLEN, "");
    if(p_Changeobj[id][changeobj_id] != 0) DestroyDynamicObject(p_Changeobj[id][changeobj_id]), p_Changeobj[id][changeobj_id] = 0;
    return 1;
}
stock RedAllChangeObj(const txt[]) {
    for (new i = 0; i < MAX_CHANGEOBJ; i++) {
        if(mk_strcmp(txt, p_Changeobj[i][changeobj_txt]) == 0) {
            RedChangeObj(i);
        }
    }
}
stock ShowChangeObj(const index) {
    new ms, id = index;
    if(p_Changeobj[id][changeobj_state] == 0) {
        ms = p_Changeobj[id][changeobj_mod];
    } else {
        ms = p_Changeobj[id][changeobj_mod1];
    }
    if(p_Changeobj[id][changeobj_id] != 0) DestroyDynamicObject(p_Changeobj[id][changeobj_id]);
    p_Changeobj[id][changeobj_id] = CreateDynamicObjectEx(ms, p_Changeobj[id][changeobj_x], p_Changeobj[id][changeobj_y], p_Changeobj[id][changeobj_z], p_Changeobj[id][changeobj_rx], p_Changeobj[id][changeobj_ry], p_Changeobj[id][changeobj_rz], p_Changeobj[id][changeobj_dis], p_Changeobj[id][changeobj_dis],{0});
    Streamer_UpdateAll();
}
stock ChangeChangeObj(const index) {
    new id = index;
    if(p_Changeobj[id][changeobj_state] == 0) {
        p_Changeobj[id][changeobj_state] = 1;
    } else {
        p_Changeobj[id][changeobj_state] = 0;
    }
    new msg[512];
    format(msg, sizeof(msg), "%i %f %f %f %f %f %f %i %i %f %f", p_Changeobj[id][changeobj_mod], p_Changeobj[id][changeobj_x], p_Changeobj[id][changeobj_y], p_Changeobj[id][changeobj_z], p_Changeobj[id][changeobj_rx], p_Changeobj[id][changeobj_ry], p_Changeobj[id][changeobj_rz], p_Changeobj[id][changeobj_mod1], p_Changeobj[id][changeobj_state], p_Changeobj[id][changeobj_range], p_Changeobj[id][changeobj_dis]);
    new info[1024], tid = p_Txtlist[IsLoadTxt(p_Changeobj[id][changeobj_txt])][load_id];
    mysql_format(g_Sql, info, sizeof(info), "UPDATE `%i` SET `value` = '%e' WHERE `id` = '%i'", tid, msg, p_Changeobj[id][changeobj_dbid]);
    mysql_pquery(g_Sql, info);
    ShowChangeObj(id);
}
stock UseChangeObj(const playerid) {
    new name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, MAX_PLAYER_NAME);
    for (new i = 0; i < MAX_CHANGEOBJ; i++) {
        if(p_Changeobj[i][changeobj_dbid] != 0) {
            if(GetPlayerAclFromTxt(name, p_Changeobj[i][changeobj_txt]) == 1) {
                if(GetPlayerDistanceFromPoint(playerid, p_Changeobj[i][changeobj_x], p_Changeobj[i][changeobj_y], p_Changeobj[i][changeobj_z]) <= p_Changeobj[i][changeobj_range]) {
                    ChangeChangeObj(i);
                }
            }
        }
    }
}
//P-House 移动类obj函数区
#define MAX_MOVEOBJ 10000//移动类数量OBJ上限 默认10000
enum Moveobjtype {
    moveobj_dbid, //文档表内的id
    moveobj_txt[PHOUSE_TXTLEN], //所属文档名
    moveobj_state, //..状态,0为xyz,非0为x1y1z1
    moveobj_mod, //..模型
    moveobj_id, //...id
    moveobj_timer, //timer
    Float:moveobj_time, //..自动返回时间,负数为不自动返回,非负数为自动返回
    Float:moveobj_range, //..指令范围
    Float:moveobj_dis, //绘制距离
    Float:moveobj_x, //..x
    Float:moveobj_y, //..y
    Float:moveobj_z, //..z
    Float:moveobj_rx, //rx
    Float:moveobj_ry, //ry
    Float:moveobj_rz, //rz
    Float:moveobj_speed, //..speed
    Float:moveobj_x1, //x1
    Float:moveobj_y1, //y1
    Float:moveobj_z1, //z1
    Float:moveobj_rx1, //rx1
    Float:moveobj_ry1, //ry1
    Float:moveobj_rz1, //rz1
    Float:moveobj_speed1
}; //speed1
new p_Moveobj[MAX_MOVEOBJ][Moveobjtype];
stock AddMoveObj(const txt[], const value[], const dbid) {
    new id = -1;
    for (new i = 0; i < MAX_MOVEOBJ; i++) {
        if(p_Moveobj[i][moveobj_dbid] == 0) {
            id = i;
            i = MAX_MOVEOBJ + 1;
        }
    }
    if(id == -1) return printf("[PHouse]移动类obj已达上限,添加失败");
    new idx = 0;
    //moveobj 模型id x y z rx ry rz speed x1 y1 z1 rx1 ry1 rz1 speed1 [time(在移动完成后的指定时间后返回xyz的位置,如果为负数则不自动返回,不填为0),不填为0] [初始位置(0为xyz,非0为x1y1z1),不填为0] [指令操作范围(不填则采用值Moveobjcommandrange)] [绘制距离(不填则采用值Objdis)]

    p_Moveobj[id][moveobj_dbid] = dbid;
    format(p_Moveobj[id][moveobj_txt], PHOUSE_TXTLEN, "%s", txt);
    p_Moveobj[id][moveobj_mod] = strval(strtok(value, idx));
    p_Moveobj[id][moveobj_x] = floatstr(strtok(value, idx));
    p_Moveobj[id][moveobj_y] = floatstr(strtok(value, idx));
    p_Moveobj[id][moveobj_z] = floatstr(strtok(value, idx));
    p_Moveobj[id][moveobj_rx] = floatstr(strtok(value, idx));
    p_Moveobj[id][moveobj_ry] = floatstr(strtok(value, idx));
    p_Moveobj[id][moveobj_rz] = floatstr(strtok(value, idx));
    p_Moveobj[id][moveobj_speed] = floatstr(strtok(value, idx));
    p_Moveobj[id][moveobj_x1] = floatstr(strtok(value, idx));
    p_Moveobj[id][moveobj_y1] = floatstr(strtok(value, idx));
    p_Moveobj[id][moveobj_z1] = floatstr(strtok(value, idx));
    p_Moveobj[id][moveobj_rx1] = floatstr(strtok(value, idx));
    p_Moveobj[id][moveobj_ry1] = floatstr(strtok(value, idx));
    p_Moveobj[id][moveobj_rz1] = floatstr(strtok(value, idx));
    p_Moveobj[id][moveobj_speed1] = floatstr(strtok(value, idx));
    p_Moveobj[id][moveobj_time] = floatstr(strtok(value, idx));
    p_Moveobj[id][moveobj_range] = floatstr(strtok(value, idx));
    p_Moveobj[id][moveobj_dis] = floatstr(strtok(value, idx));
    p_Moveobj[id][moveobj_state] = strval(strtok(value, idx));
    if(p_Moveobj[id][moveobj_time] >= 0) p_Moveobj[id][moveobj_state] = 0;
    if(p_Moveobj[id][moveobj_range] <= 0) p_Moveobj[id][moveobj_range] = config_Moveobjcommandrange;
    if(p_Moveobj[id][moveobj_dis] <= 0) p_Moveobj[id][moveobj_dis] = config_Objdis;
    if(config_Nomoveobj == 0) {
        new Float:ox, Float:oy, Float:oz, Float:orx, Float:ory, Float:orz;
        if(p_Moveobj[id][moveobj_state] == 0) {
            ox = p_Moveobj[id][moveobj_x], oy = p_Moveobj[id][moveobj_y], oz = p_Moveobj[id][moveobj_z];
            orx = p_Moveobj[id][moveobj_rx], ory = p_Moveobj[id][moveobj_ry], orz = p_Moveobj[id][moveobj_rz];
        } else {
            ox = p_Moveobj[id][moveobj_x1], oy = p_Moveobj[id][moveobj_y1], oz = p_Moveobj[id][moveobj_z1];
            orx = p_Moveobj[id][moveobj_rx1], ory = p_Moveobj[id][moveobj_ry1], orz = p_Moveobj[id][moveobj_rz1];
        }
        p_Moveobj[id][moveobj_id] = CreateDynamicObjectEx(p_Moveobj[id][moveobj_mod], ox, oy, oz, orx, ory, orz, p_Moveobj[id][moveobj_dis], p_Moveobj[id][moveobj_dis],{0});
    }
    return 1;
}

stock RedMoveObj(const index) {
    new id = index;
    if(p_Moveobj[id][moveobj_dbid] == 0) return 1;
    p_Moveobj[id][moveobj_dbid] = 0;
    format(p_Moveobj[id][moveobj_txt], PHOUSE_TXTLEN, "");
    if(p_Moveobj[id][moveobj_id] != 0) DestroyDynamicObject(p_Moveobj[id][moveobj_id]), p_Moveobj[id][moveobj_id] = 0;
    return 1;
}

stock RedAllMoveObj(const txt[]) {
    for (new i = 0; i < MAX_MOVEOBJ; i++) {
        if(mk_strcmp(txt, p_Moveobj[i][moveobj_txt]) == 0) {
            RedMoveObj(i);
        }
    }
}

forward MoveMoveObj(index);
public MoveMoveObj(index) {
    new id = index, Float:ox, Float:oy, Float:oz, Float:orx, Float:ory, Float:orz, Float:ospeed;
    if(p_Moveobj[id][moveobj_state] == 0) {
        ox = p_Moveobj[id][moveobj_x1], oy = p_Moveobj[id][moveobj_y1], oz = p_Moveobj[id][moveobj_z1];
        orx = p_Moveobj[id][moveobj_rx1], ory = p_Moveobj[id][moveobj_ry1], orz = p_Moveobj[id][moveobj_rz1];
        ospeed = p_Moveobj[id][moveobj_speed1];
    } else {
        ox = p_Moveobj[id][moveobj_x], oy = p_Moveobj[id][moveobj_y], oz = p_Moveobj[id][moveobj_z];
        orx = p_Moveobj[id][moveobj_rx], ory = p_Moveobj[id][moveobj_ry], orz = p_Moveobj[id][moveobj_rz];
        ospeed = p_Moveobj[id][moveobj_speed];
    }
    if(p_Moveobj[id][moveobj_timer] != 0) KillTimer(p_Moveobj[id][moveobj_timer]), p_Moveobj[id][moveobj_timer] = 0;
    MoveDynamicObject(p_Moveobj[id][moveobj_id], ox, oy, oz, ospeed, orx, ory, orz);
}

stock MoveMoveObjed(const index) {
    new id = index, s = -1;
    for (new i = 0; i < MAX_MOVEOBJ; i++) {
        if(p_Moveobj[i][moveobj_id] == id) {
            id = i;
            s = 0;
            i = MAX_MOVEOBJ + 1;
        }
    }
    if(s == -1) return -1;
    if(p_Moveobj[id][moveobj_state] == 0) {
        p_Moveobj[id][moveobj_state] = 1;
    } else {
        p_Moveobj[id][moveobj_state] = 0;
    }
    new msg[512], sta = p_Moveobj[id][moveobj_state];
    if(p_Moveobj[id][moveobj_time] >= 0) sta = 0;
    //栗子:moveobj 模型id x y z rx ry rz speed x1 y1 z1 rx1 ry1 rz1 speed1 [time(在移动完成后的指定时间后返回xyz的位置,如果为负数则不自动返回,不填为0),不填为0] [初始位置(0为xyz,非0为x1y1z1),不填为0] [指令操作范围(不填则采用值Moveobjcommandrange)] [绘制距离(不填则采用值Objdis)]
    format(msg, sizeof(msg), "%i %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %i", p_Moveobj[id][moveobj_mod], p_Moveobj[id][moveobj_x], p_Moveobj[id][moveobj_y], p_Moveobj[id][moveobj_z], p_Moveobj[id][moveobj_rx], p_Moveobj[id][moveobj_ry], p_Moveobj[id][moveobj_rz], p_Moveobj[id][moveobj_speed],
        p_Moveobj[id][moveobj_x1], p_Moveobj[id][moveobj_y1], p_Moveobj[id][moveobj_z1], p_Moveobj[id][moveobj_rx1], p_Moveobj[id][moveobj_ry1], p_Moveobj[id][moveobj_rz1], p_Moveobj[id][moveobj_speed1], p_Moveobj[id][moveobj_time], p_Moveobj[id][moveobj_range], p_Moveobj[id][moveobj_dis], sta);
    new Query[256], tid = p_Txtlist[IsLoadTxt(p_Moveobj[id][moveobj_txt])][load_id];
    mysql_format(g_Sql, Query, sizeof(Query), "UPDATE `%i` SET value = '%e' WHERE id = '%i'", tid, msg, p_Moveobj[id][moveobj_dbid]);
    mysql_pquery(g_Sql, Query);
    if(p_Moveobj[id][moveobj_state] != 0) {

        if(p_Moveobj[id][moveobj_time] >= 0) {
            new t[16];
            format(t, 16, "%f", p_Moveobj[id][moveobj_time] * 1000);
            //printf("%f|%i",p_Moveobj[id][moveobj_time],strval(t));
            if(p_Moveobj[id][moveobj_timer] != 0) KillTimer(p_Moveobj[id][moveobj_timer]), p_Moveobj[id][moveobj_timer] = 0;
            p_Moveobj[id][moveobj_timer] = SetTimerEx("MoveMoveObj", strval(t), 0, "i", id);
        }
    }
    return 1;
}

stock UseMoveObj(const playerid) {
    new name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, MAX_PLAYER_NAME);
    for (new i = 0; i < MAX_MOVEOBJ; i++) {
        if(p_Moveobj[i][moveobj_dbid] != 0) {
            if(GetPlayerAclFromTxt(name, p_Moveobj[i][moveobj_txt]) == 1) {
                new id = i, Float:ox, Float:oy, Float:oz;
                if(p_Moveobj[id][moveobj_state] == 0) {
                    ox = p_Moveobj[id][moveobj_x], oy = p_Moveobj[id][moveobj_y], oz = p_Moveobj[id][moveobj_z];
                } else {
                    ox = p_Moveobj[id][moveobj_x1], oy = p_Moveobj[id][moveobj_y1], oz = p_Moveobj[id][moveobj_z1];
                }
                if(GetPlayerDistanceFromPoint(playerid, ox, oy, oz) <= p_Moveobj[i][moveobj_range]) {
                    MoveMoveObj(i);
                }
            }
        }
    }
}
//P-House 通用函数区
stock DebugMsg(const msg[]) {
    if(config_Debug != 0) {
        print(msg);
    }
}

stock Streamer_UpdateAll() {
    for (new i = 0; i < MAX_PLAYERS; i++) {
        if(IsPlayerConnected(i) == 1) {
            Streamer_Update(i);
        }
    }
}

stock ReturnPlayerId(const msg[]) {
    new id = -1, name[MAX_PLAYER_NAME];
    if(CheckVar(msg) != 1) {
        for (new i = 0; i < MAX_PLAYERS; i++) {
            if(IsPlayerConnected(i) == 1) {
                GetPlayerName(i, name, MAX_PLAYER_NAME);
                if(mk_strcmp(name, msg) == 0) {
                    id = i;
                    i = MAX_PLAYERS + 1;
                }
            }
        }
        return id;
    }
    if(IsPlayerConnected(strval(msg)) == 1) id = strval(msg);
    return id;
}
// public OnFilterScriptInit()
// {

// 	print("[PHouse]加载");
// 	if(fexist(PHOUSE_DB)==0)
// 	    {
// 	        print("[PHouse]未找到数据库'house.db',导入功能将无法使用");
// 	    }
// 	else
// 	    {
// 	        data=db_open(PHOUSE_DB);
// 	        db_free_result(db_query(data,"PRAGMA synchronous = NORMAL"));
// 	        /*PRAGMA synchronous = FULL; (2)最慢
// 			PRAGMA synchronous = NORMAL; (1)一般
// 			PRAGMA synchronous = OFF; (0)最快*/
// 	    }
//     LoadConfig();
// 	return 1;

// }

// public OnFilterScriptExit()
// {
// 	print("[PHouse]卸载");
// 	db_close(data);
// 	return 1;
// }
// public OnPlayerCommandText(playerid, cmdtext[]) {
//     if(!data) return 0;
//     new tmp[128], cmd[128], msg[256], idx;
//     cmd = strtok(cmdtext, idx);
//     if(config_Nosell == 0) {
//         if(mk_strcmp(cmd, "/buyhouse") == 0) {
//             new id = GetPlayerInSell(playerid);
//             if(id == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]..你想买什么房子?请站在房子图标附近..");
//             if(mk_strcmp(p_Sell[id][sell_player], "") != 0) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]..这个房子已经被买了.");
//             if(BuySell(playerid, id) == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]..你的钱不够");
//             ShowPlayerDialog(playerid, 98098, DIALOG_STYLE_LIST, "房子系统(V2.6)", "{FF0000}房子帮助\n输入/sellhouse 卖出房子 \n输入/changehouse 改房名\n输入/adp 添加玩家开门权限\n /rdp - 删除某人某房子的权限\n/gdp - 查看玩家是否有开门权限\n/plist - 查看指定房子的权限列表", "..::阅读完毕::..", "");
//             SendClientMessage(playerid, 0x98CDFEFF, "[提示]您购买这间房屋成功...");
//             return 1;
//         }
//         if(mk_strcmp(cmd, "/sellhouse") == 0) {
//             new id = GetPlayerInSell(playerid);
//             if(id == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]..你想卖什么房子?请站在房子图标附近..");
//             if(mk_strcmp(p_Sell[id][sell_player], "") == 0) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]..这个房子还在卖呢...");
//             if(SellSell(playerid, id) == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]..这不是你的房子...");
//             SendClientMessage(playerid, 0x98CDFEFF, "[提示]出售房屋成功...");
//             return 1;
//         }
//         if(mk_strcmp(cmd, "/changehouse") == 0) {
//             new id = GetPlayerInSell(playerid);
//             if(id == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]..你想改什么房子的名字?请站在房子图标附近..");
//             if(mk_strcmp(p_Sell[id][sell_player], "") == 0) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]..这个房子还在卖呢...");
//             new name[MAX_PLAYER_NAME];
//             GetPlayerName(playerid, name, MAX_PLAYER_NAME);
//             if(mk_strcmp(p_Sell[id][sell_player], name) != 0) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]..这不是你的房子...");
//             tmp = strrest(cmdtext, idx);
//             if(mk_strcmp(tmp, "") == 0) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]...改成空名是什么节奏?");
//             if(ChangeSellName(id, tmp) == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[提示] 房名太长");
//             format(msg, 256, "[提示]..你把你的房子名字修改为 {98CDFE}%s{FFFFFF} 了", tmp);
//             SendClientMessage(playerid, 0xFFFFFFFF, msg);
//             return 1;
//         }
//         if(mk_strcmp(cmd, "/fz") == 0) {
//             tmp = strtok(cmdtext, idx);
//             if(mk_strcmp(tmp, "") == 0) return SendClientMessage(playerid, 0x98CDFEFF, "[提示] /fz [房子ID]");
//             new id = strval(tmp);
//             if(id < 0) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]..精神病院左转，谢谢");
//             if(GotoSell(playerid, id) == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[提示] 错误房子id.");
//             SendClientMessage(playerid, 0x98CDFEFF, "[提示]传送成功..");
//             return 1;
//         }
//     }
//     if(config_Acl == 1) {
//         if(mk_strcmp(cmd, "/adp") == 0) {
//             tmp = strtok(cmdtext, idx);
//             if(mk_strcmp(tmp, "") == 0) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]..添加权限? 请用/adp [玩家名字]");
//             new pid = GetPlayerInSell(playerid);
//             if(pid == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]...请站在你想添加权限的那个牌子上");
//             new name[MAX_PLAYER_NAME];
//             GetPlayerName(playerid, name, MAX_PLAYER_NAME);
//             if(mk_strcmp(name, p_Sell[pid][sell_player]) != 0) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]..这不是你的房子");
//             new id = ReturnPlayerId(tmp);
//             if(id == -1) {
//                 if(AddPlayerToTxt(tmp, p_Sell[pid][sell_txt]) == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]..这货已经拥有这个房子的权限了");
//                 format(msg, 256, "[提示]..你砰的一声扔了一份该房权限给 %s ", tmp);
//                 SendClientMessage(playerid, 0x98CDFEFF, msg);
//                 return 1;
//             } else {
//                 GetPlayerName(id, tmp, MAX_PLAYER_NAME);
//                 if(AddPlayerToTxt(tmp, p_Sell[pid][sell_txt]) == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]..这货已经拥有这个房子的权限了");
//                 format(msg, 256, "[提示]..你砰的一声扔了一份该房权限给 %s ", tmp);
//                 SendClientMessage(playerid, 0x98CDFEFF, msg);
//                 format(msg, 256, "[提示]..%s 砰的一声扔了一份该房权限给你", name);
//                 SendClientMessage(id, 0x98CDFEFF, msg);
//                 return 1;
//             }
//         }
//         if(mk_strcmp(cmd, "/rdp") == 0) {
//             tmp = strtok(cmdtext, idx);
//             if(mk_strcmp(tmp, "") == 0) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]..删除权限?请用/rdp [玩家名字]");
//             new pid = GetPlayerInSell(playerid);
//             if(pid == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]...请站在你想删除权限的那个牌子上");
//             new name[MAX_PLAYER_NAME];
//             GetPlayerName(playerid, name, MAX_PLAYER_NAME);
//             if(mk_strcmp(name, p_Sell[pid][sell_player]) != 0) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]..这看起来不是你的房子");
//             new id = ReturnPlayerId(tmp);
//             if(id == -1) {
//                 if(RedPlayerToTxt(tmp, p_Sell[pid][sell_txt]) == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]..这货没有你这的权限");
//                 format(msg, 256, "[提示]..你引爆了 %s 那里的权限", tmp);
//                 SendClientMessage(playerid, 0x98CDFEFF, msg);
//                 return 1;
//             } else {
//                 GetPlayerName(id, tmp, MAX_PLAYER_NAME);
//                 if(RedPlayerToTxt(tmp, p_Sell[pid][sell_txt]) == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]..这货没有你这的权限");
//                 format(msg, 256, "[提示]..你默默的把锁换成了 %s 身上开不了的", tmp);
//                 SendClientMessage(playerid, 0x98CDFEFF, msg);
//                 format(msg, 256, "[提示]..你感觉到 %s 在换锁..", name);
//                 SendClientMessage(id, 0x98CDFEFF, msg);
//                 return 1;
//             }
//         }

//         if(mk_strcmp(cmd, "/gdp") == 0) {
//             tmp = strtok(cmdtext, idx);
//             if(mk_strcmp(tmp, "") == 0) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]..查看权限?请用/gdp [玩家名字]");
//             new pid = GetPlayerInSell(playerid);
//             if(pid == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]...请站在你想查看权限的那个牌子上");
//             new name[MAX_PLAYER_NAME];
//             GetPlayerName(playerid, name, MAX_PLAYER_NAME);
//             if(mk_strcmp(name, p_Sell[pid][sell_player]) != 0) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]..这看起来不是你的房子");
//             new id = ReturnPlayerId(tmp);
//             if(id == -1) {
//                 if(GetPlayerAclFromTxt(tmp, p_Sell[pid][sell_txt]) == 0) {
//                     format(msg, 256, "[提示]..%s 没有你屋子的权限..想进屋?用找房主要权限吧!", tmp);
//                 } else {
//                     format(msg, 256, "[提示]..%s 拥有你屋子的权限,做(pa)坏(pa)事(pa)记得关上门", tmp);
//                 }
//                 SendClientMessage(playerid, 0x98CDFEFF, msg);
//                 return 1;
//             } else {
//                 GetPlayerName(id, tmp, MAX_PLAYER_NAME);
//                 if(GetPlayerAclFromTxt(tmp, p_Sell[pid][sell_txt]) == 0) {
//                     format(msg, 256, "[提示]..%s 没有你屋子的权限..想进屋?用找房主要权限吧!", tmp);
//                 } else {
//                     format(msg, 256, "[提示]..%s 拥有你屋子的权限,做(pa)坏(pa)事(pa)记得关上门", tmp);
//                 }
//                 SendClientMessage(playerid, 0x98CDFEFF, msg);
//                 return 1;
//             }
//         }
//         if(mk_strcmp(cmd, "/plist") == 0) {
//             new pid = GetPlayerInSell(playerid);
//             if(pid == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]...请站在你想查看权限列表的那个牌子上");
//             new name[MAX_PLAYER_NAME];
//             GetPlayerName(playerid, name, MAX_PLAYER_NAME);
//             if(mk_strcmp(name, p_Sell[pid][sell_player]) != 0) return SendClientMessage(playerid, 0x98CDFEFF, "[提示]..这看起来不是你的房子");
//             SetPVarInt(playerid, "PHouse_AclList_NowSelect", 1);
//             SetPVarString(playerid, "PHouse_AclList_TxtNmae", p_Sell[pid][sell_txt]);
//             ShowAclList(playerid, p_Sell[pid][sell_txt]);
//             return 1;
//         }
//     }
//     if(config_Nochangeobj == 0) {
//         if(mk_strcmp(cmd, "/open") == 0) {
//             OnPlayerKeyStateChange(playerid, 65536, 0);
//             return 1;
//         }
//     }
//     if(config_Nomoveobj == 0) {
//         if(mk_strcmp(cmd, "/move") == 0) {
//             OnPlayerKeyStateChange(playerid, KEY_LOOK_BEHIND, 0);
//             return 1;
//         }
//     }
//     return 0;
// }
// public OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {

//     if(newkeys == 65536) {
//         if(config_Nochangeobj == 0) {
//             UseChangeObj(playerid);
//             return 1;
//         }

//     }
//     if(newkeys == 262144 || newkeys == 2) {
//         if(config_Nomoveobj == 0) {
//             UseMoveObj(playerid);
//             return 1;
//         }
//     }
//     return 1;
// }
// public OnRconCommand(cmd[]) {

//     return 1;
// }
public OnDynamicObjectMoved(objectid) {
    MoveMoveObjed(objectid);
    return 1;
}

enum hstatype
{
	HID,
	Buyer,
	Seller,
	Price
};

new dyHouseSell[MAX_PLAYERS][hstatype];

stock HouseSellPlayerInitialize(const playerid)
{
	dyHouseSell[playerid][HID] = -1;
	dyHouseSell[playerid][Buyer] = -1;
	dyHouseSell[playerid][Seller] = -1;
	dyHouseSell[playerid][Price] = 0;
}

stock HouseSellAgree(const playerid)
{
	if(dyHouseSell[playerid][Seller] == -1) return SendClientMessage(playerid, 0xFF0000AA, "[房产 ]暂时还没有人向你卖房子");
	if(PlayerInfo[playerid][Cash] < dyHouseSell[playerid][Price]) return SendClientMessage(playerid, 0xFF0000AA, "[房产 ]你的金币不足");
	if(IsPlayerConnected(dyHouseSell[playerid][Seller]) == 0 || !PlayerInfo[dyHouseSell[playerid][Seller]][Login]) return SendClientMessage(playerid, 0xFF0000AA, "that son of a bitch just off line.");
	if(mk_strcmp(GetName(dyHouseSell[playerid][Seller]), p_Sell[dyHouseSell[playerid][HID]][sell_player]) != 0) return SendClientMessage(playerid,0x98CDFEFF,"that son of a bitch just sell it.");
	PlayerInfo[playerid][Cash] = PlayerInfo[playerid][Cash] - dyHouseSell[playerid][Price];
    PlayerInfo[dyHouseSell[playerid][Seller]][Cash] = PlayerInfo[dyHouseSell[playerid][Seller]][Cash] + dyHouseSell[playerid][Price];
	// GivePlayeriXyxb(dyHouseSell[playerid][Seller], dyHouseSell[playerid][Price]);
	new msg[128];
	format(msg, sizeof msg, "sold $%d", dyHouseSell[playerid][Price]);
	SendClientMessage(dyHouseSell[playerid][Seller], 0x98CDFEFF, msg);
	format(msg, sizeof msg, "sold -%d$", dyHouseSell[playerid][Price]);
	SendClientMessage(playerid, 0x98CDFEFF, msg);
	RedAllPlayerToTxt(p_Sell[dyHouseSell[playerid][HID]][sell_txt]);
	AddPlayerToTxt(GetName(playerid), p_Sell[dyHouseSell[playerid][HID]][sell_txt]);
	ChangeSellPlayer(dyHouseSell[playerid][HID], GetName(playerid));
	return 1;
}

stock HouseSellDeal(const hid, const bid, const sid, const price)
{
	dyHouseSell[bid][HID] = hid;
	dyHouseSell[bid][Buyer] = bid;
	dyHouseSell[bid][Seller] = sid;
	dyHouseSell[bid][Price] = price;
	new msg[128];
	format(msg, sizeof msg, "[房产 *] %s 想出售给你这套房产, 价格 %d, 同意输入'/house buyit'", GetName(sid), price);
	SendClientMessage(bid, 0x98CDFEFF, msg);
	format(msg, sizeof msg, "[房产 *] 出售信息已发送至 %s 请等待它的回复.", GetName(bid));
	SendClientMessage(sid, 0x98CDFEFF, msg);
}

stock House_ShowListDialog(const playerid,const page)//为指定玩家显示赛道选择菜单,page表示赛道页数
{
	new title[64],msg[1024],maxpage;
    maxpage = GetSellMax() / 15 + 1; // 一页15个
    // printf("housemaxpage:%i",GetSellMax());
	format(title,64,"房屋列表 - %i/%i",page,maxpage); 
    format(msg,1024,"房子ID\t拥有者\t价格%s\n",msg);
    if(page > 1 && page == maxpage) format(msg,1024,"%s\n上一页",msg);
	if(page ==1 && page!= maxpage) format(msg,1024,"%s\n下一页\n",msg);
	if(page > 1 && page < maxpage) format(msg,1024,"%s\n上一页\n下一页",msg);
    for(new i=(page-1)*15;i<page*15;i++)
	{
        if(p_Sell[i][sell_dbid]!=0){
            if(mk_strcmp(p_Sell[i][sell_player], "") == 0) format(msg,sizeof(msg),"%s\n%d\t出售中\t%d\n",msg,i+1,p_Sell[i][sell_money]);
            else format(msg,sizeof(msg),"%s\n%d\t%s\t%d\n",msg,i+1,p_Sell[i][sell_player],p_Sell[i][sell_money]);//房子ID,户主,价格
            // printf("%d %s %d",p_Sell[i][sell_dbid],p_Sell[i][sell_player],p_Sell[i][sell_money]);
        }
	}
	Dialog_Show(playerid,HouseListDialog,DIALOG_STYLE_TABLIST_HEADERS,title,msg,"传送","关闭");
}

stock House_howMany(const playerid){
    new cut = 0;
    for (new i = 0; i < MAX_SELL; i++) 
    {
        if(mk_strcmp(p_Sell[i][sell_player], GetName(playerid)) == 0) cut++;
    }
    return cut;
}

// public OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA:areaid)
// {
//     if(PHouse_PlayerEnterArea(playerid, areaid) == 1) return 1;
//     return 1;
// }

// public OnPlayerLeaveDynamicArea(playerid, STREAMER_TAG_AREA:areaid)
// {
//     if(PHouse_PlayerLevelArea(playerid, areaid) == 1) return 1;
//     return 1;
// }


CMD:house(playerid,cmdtext[]){
    // if(data) {
    new tmp[128],idx,msg[256];
    tmp = strtok(cmdtext, idx);
    if(!strlen(tmp) || !strcmp(tmp, "help", true)) {
        ShowPlayerDialog(playerid, 5, DIALOG_STYLE_LIST, "房产系统", "/house goto 可传送至房屋.\n\
            /house buy 可购买房屋.\n/house sell 可出售房屋.\n\
            /house text 可更改简介.\n/house list 可查看房屋列表.\n\
            /house glist 可查看房屋权限列表.\n/house adp 可给予房屋操作权限.\n\
            /house rdp 可移除房屋操作权限.\n/house gdp 可查看房屋操作权限.\n\
            /house sellto 可出售给指定他人.\n\
            /house buyit 可接受他人发来的卖方申请\n\
            ", "确定", "返回");
        // SendClientMessage(playerid, 0x98CDFEFF, "[房产 *] /house goto 可传送至房屋.");
        // SendClientMessage(playerid, 0x98CDFEFF, "[房产 *] /house buy 可购买房屋.");
        // SendClientMessage(playerid, 0x98CDFEFF, "[房产 *] /house sell 可出售房屋.");
        // SendClientMessage(playerid, 0x98CDFEFF, "[房产 *] /house text 可更改简介.");
        // SendClientMessage(playerid, 0x98CDFEFF, "[房产 *] /house list 可查看房屋权限列表.");
        // SendClientMessage(playerid, 0x98CDFEFF, "[房产 *] /house adp 可给予房屋操作权限.");
        // SendClientMessage(playerid, 0x98CDFEFF, "[房产 *] /house rdp 可移除房屋操作权限.");
        // SendClientMessage(playerid, 0x98CDFEFF, "[房产 *] /house gdp 可查看房屋操作权限.");
        // SendClientMessage(playerid,0x98CDFEFF,"[房产 *] 输入 /house sellto 可出售给指定他人.");
        return 1;
    }
    if(config_Nosell == 0) {
        if(strcmp(tmp, "buy", true) == 0) {
            // if(mk_strcmp(cmd, "/buyhouse") == 0) {
            if(House_howMany(playerid) >= 4) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *] 一个人只能拥有4套房哦");
            new id = GetPlayerInSell(playerid);
            if(id == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]..你想买什么房子?请站在房子图标附近..");
            if(mk_strcmp(p_Sell[id][sell_player], "") != 0) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]..这个房子已经被买了.");
            if(BuySell(playerid, id) == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]..你的钱不够");
            ShowPlayerDialog(playerid, 98098, DIALOG_STYLE_LIST, "房屋系统", "{FF0000}房子帮助\n输入/sellhouse 卖出房子 \n输入/changehouse 改房名\n输入/adp 添加玩家开门权限\n /rdp - 删除某人某房子的权限\n/gdp - 查看玩家是否有开门权限\n/plist - 查看指定房子的权限列表", "..::阅读完毕::..", "");
            SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]您购买这间房屋成功...");
            return 1;
        }
        if(strcmp(tmp, "sell", true) == 0) {
            // if(mk_strcmp(cmd, "/sellhouse") == 0) {
            new id = GetPlayerInSell(playerid);
            if(id == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]..你想卖什么房子?请站在房子图标附近..");
            if(mk_strcmp(p_Sell[id][sell_player], "") == 0) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]..这个房子还在卖呢...");
            if(SellSell(playerid, id) == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]..这不是你的房子...");
            SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]出售房屋成功...");
            return 1;
        }
        if(strcmp(tmp, "sellto", true) == 0) {
            new hid = GetPlayerInSell(playerid);
            if(hid == -1) return SendClientMessage(playerid, 0x98CDFEFF, "房产 * 操作失败,原因:请站在牌子上输入该命令.");
            tmp = strtok(cmdtext, idx);
            if(mk_strcmp(tmp, "") == 0) return SendClientMessage(playerid, 0x98CDFEFF, "房产 * 用法:/house sellto [对方ID] [价格]");
            new name[MAX_PLAYER_NAME];
            GetPlayerName(playerid, name, MAX_PLAYER_NAME);
            if(mk_strcmp(name, p_Sell[hid][sell_player]) != 0) return SendClientMessage(playerid, 0x98CDFEFF, "房产 * 操作失败,原因:该房屋不属于你.");
            new pid = strval(tmp);
            if(IsPlayerConnected(pid) == 0) return SendClientMessage(playerid, 0xFF0000AA, "房产 * 错误的对方ID.");
            if(pid == playerid) return SendClientMessage(playerid, 0xFF0000AA, "房产 * 你不可以卖给你自己.");
            new Float:x, Float:y, Float:z;
            GetPlayerPos(pid, x, y, z);
            if(GetPlayerDistanceFromPoint(playerid, x, y, z) >= 2.0) return SendClientMessage(playerid, 0xFF0000AA, "房产 * 他不在你身边.");
            tmp = strtok(cmdtext, idx);
            if(!strlen(tmp)) return SendClientMessage(playerid, 0xFF0000AA, "房产 * 出售价格不能为空.");
            new price = strval(tmp);
            if(price < 1 || price > 10000000) return SendClientMessage(playerid, 0xFF0000AA, "房产 * 出售价格范围 1 - 1千万.");
            HouseSellDeal(hid, pid, playerid, price);
            return 1;
        }
        if(strcmp(tmp, "buyit", true) == 0) {
            HouseSellAgree(playerid);
            return 1;
        }
        if(strcmp(tmp, "text", true) == 0) {
            // if(mk_strcmp(cmd, "/changehouse") == 0) {
            new id = GetPlayerInSell(playerid);
            if(id == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]..你想改什么房子的名字?请站在房子图标附近..");
            if(mk_strcmp(p_Sell[id][sell_player], "") == 0) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]..这个房子还在卖呢...");
            new name[MAX_PLAYER_NAME];
            GetPlayerName(playerid, name, MAX_PLAYER_NAME);
            if(mk_strcmp(p_Sell[id][sell_player], name) != 0) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]..这不是你的房子...");
            tmp = strrest(cmdtext, idx);
            if(mk_strcmp(tmp, "") == 0) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]...改成空名是什么节奏?");
            if(ChangeSellName(id, tmp) == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *] 房名太长");
            format(msg, 256, "[房产 *]..你把你的房子名字修改为 {98CDFE}%s{FFFFFF} 了", tmp);
            SendClientMessage(playerid, 0xFFFFFFFF, msg);
            return 1;
        }
        if(strcmp(tmp, "goto", true) == 0) {
            // if(!mk_strcmp(cmd, "/fz") || !mk_strcmp(cmd, "/house")) {
            tmp = strtok(cmdtext, idx);
            if(mk_strcmp(tmp, "") == 0) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *] /house goto [房子ID]");
            new id = strval(tmp);
            if(id <= 0) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]..错误的房子ID");
            if(GotoSell(playerid, id - 1) == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *] 错误房子id.");
            format(msg, sizeof(msg), "[房产 *] %s(%d) {FFFF00}传送到{FF00FF}/house goto %d (房产 %d)", GetName(playerid), playerid, id, id);
            SendClientMessageToAll(0x98CDFEFF, msg);
            // SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]传送成功..");
            return 1;
        }
    }
    if(config_Acl == 1) {
        if(strcmp(tmp, "adp", true) == 0) {
            // if(mk_strcmp(cmd, "/adp") == 0) {
            tmp = strtok(cmdtext, idx);
            if(mk_strcmp(tmp, "") == 0) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]..添加权限? 请用/adp [玩家名字]");
            new pid = GetPlayerInSell(playerid);
            if(pid == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]...请站在你想添加权限的那个牌子上");
            new name[MAX_PLAYER_NAME];
            GetPlayerName(playerid, name, MAX_PLAYER_NAME);
            if(mk_strcmp(name, p_Sell[pid][sell_player]) != 0) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]..这不是你的房子");
            new id = ReturnPlayerId(tmp);
            if(id == -1) {
                if(AddPlayerToTxt(tmp, p_Sell[pid][sell_txt]) == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]..这货已经拥有这个房子的权限了");
                format(msg, 256, "[房产 *]..你砰的一声扔了一份该房权限给 %s ", tmp);
                SendClientMessage(playerid, 0x98CDFEFF, msg);
                return 1;
            } else {
                GetPlayerName(id, tmp, MAX_PLAYER_NAME);
                if(AddPlayerToTxt(tmp, p_Sell[pid][sell_txt]) == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]..这货已经拥有这个房子的权限了");
                format(msg, 256, "[房产 *]..你砰的一声扔了一份该房权限给 %s ", tmp);
                SendClientMessage(playerid, 0x98CDFEFF, msg);
                format(msg, 256, "[房产 *]..%s 砰的一声扔了一份该房权限给你", name);
                SendClientMessage(id, 0x98CDFEFF, msg);
                return 1;
            }
        }
        if(strcmp(tmp, "rdp", true) == 0) {
            // if(mk_strcmp(cmd, "/rdp") == 0) {
            tmp = strtok(cmdtext, idx);
            if(mk_strcmp(tmp, "") == 0) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]..删除权限?请用/rdp [玩家名字]");
            new pid = GetPlayerInSell(playerid);
            if(pid == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]...请站在你想删除权限的那个牌子上");
            new name[MAX_PLAYER_NAME];
            GetPlayerName(playerid, name, MAX_PLAYER_NAME);
            if(mk_strcmp(name, p_Sell[pid][sell_player]) != 0) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]..这看起来不是你的房子");
            new id = ReturnPlayerId(tmp);
            if(id == -1) {
                if(RedPlayerToTxt(tmp, p_Sell[pid][sell_txt]) == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]..这货没有你这的权限");
                format(msg, 256, "[房产 *]..你引爆了 %s 那里的权限", tmp);
                SendClientMessage(playerid, 0x98CDFEFF, msg);
                return 1;
            } else {
                GetPlayerName(id, tmp, MAX_PLAYER_NAME);
                if(RedPlayerToTxt(tmp, p_Sell[pid][sell_txt]) == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]..这货没有你这的权限");
                format(msg, 256, "[房产 *]..你默默的把锁换成了 %s 身上开不了的", tmp);
                SendClientMessage(playerid, 0x98CDFEFF, msg);
                format(msg, 256, "[房产 *]..你感觉到 %s 在换锁..", name);
                SendClientMessage(id, 0x98CDFEFF, msg);
                return 1;
            }
        }
        if(strcmp(tmp, "gdp", true) == 0) {
            // if(mk_strcmp(cmd, "/gdp") == 0) {
            tmp = strtok(cmdtext, idx);
            if(mk_strcmp(tmp, "") == 0) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]..查看权限?请用/gdp [玩家名字]");
            new pid = GetPlayerInSell(playerid);
            if(pid == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]...请站在你想查看权限的那个牌子上");
            new name[MAX_PLAYER_NAME];
            GetPlayerName(playerid, name, MAX_PLAYER_NAME);
            if(mk_strcmp(name, p_Sell[pid][sell_player]) != 0) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]..这看起来不是你的房子");
            new id = ReturnPlayerId(tmp);
            if(id == -1) {
                if(GetPlayerAclFromTxt(tmp, p_Sell[pid][sell_txt]) == 0) {
                    format(msg, 256, "[房产 *]..%s 没有你屋子的权限..想进屋?用找房主要权限吧!", tmp);
                } else {
                    format(msg, 256, "[房产 *]..%s 拥有你屋子的权限", tmp);
                }
                SendClientMessage(playerid, 0x98CDFEFF, msg);
                return 1;
            } else {
                GetPlayerName(id, tmp, MAX_PLAYER_NAME);
                if(GetPlayerAclFromTxt(tmp, p_Sell[pid][sell_txt]) == 0) {
                    format(msg, 256, "[房产 *]..%s 没有你屋子的权限..想进屋?用找房主要权限吧!", tmp);
                } else {
                    format(msg, 256, "[房产 *]..%s 拥有你屋子的权限", tmp);
                }
                SendClientMessage(playerid, 0x98CDFEFF, msg);
                return 1;
            }
        }
        if(strcmp(tmp, "list", true) == 0) {
            House_ShowListDialog(playerid, SelectHousePage[playerid]);
            return 1;
        }
        if(strcmp(tmp, "glist", true) == 0) {
            // if(mk_strcmp(cmd, "list") == 0) {
            new pid = GetPlayerInSell(playerid);
            if(pid == -1) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]...请站在你想查看权限列表的那个牌子上");
            new name[MAX_PLAYER_NAME];
            GetPlayerName(playerid, name, MAX_PLAYER_NAME);
            if(mk_strcmp(name, p_Sell[pid][sell_player]) != 0) return SendClientMessage(playerid, 0x98CDFEFF, "[房产 *]..这看起来不是你的房子");
            SetPVarInt(playerid, "PHouse_AclList_NowSelect", 1);
            SetPVarString(playerid, "PHouse_AclList_TxtNmae", p_Sell[pid][sell_txt]);
            ShowAclList(playerid, p_Sell[pid][sell_txt]);
            return 1;
        }
    }
    // }
    return 1;
}
CMD:open(playerid,cmdtext[]){
    if(config_Nochangeobj == 0) {
        CallRemoteFunction("OnPlayerKeyStateChange", "iii", playerid, 65536, 0);
        // OnPlayerKeyStateChange(playerid, 65536, 0);
        return 1;
    }
    return 1;
}
CMD:move(playerid,cmdtext[]){
    if(config_Nomoveobj == 0) {
        CallRemoteFunction("OnPlayerKeyStateChange", "iii", playerid, KEY_LOOK_BEHIND, 0);
        // OnPlayerKeyStateChange(playerid, KEY_LOOK_BEHIND, 0);
        return 1;
    }
    return 1;
}
CMD:houseedit(playerid,cmdtext[]){
    // if(data) {
    if(PlayerInfo[playerid][AdminLevel] < 5) return SendClientMessage(playerid, Color_White, "[PHouse]权限不足");
    new tmp[128],idx,msg[256];
    tmp = strtok(cmdtext, idx);
    if(!strlen(tmp)) {
        SendClientMessage(playerid, Color_White, "[房产]/houseedit ins <文档名> - 安装一个文档,文档需放置在scriptfiles/PHouse/txt下,后缀为.txt");
        SendClientMessage(playerid, Color_White, "[房产]/houseedit unins <文档名> - 删除一个文档(从数据库),需要先卸载");
        SendClientMessage(playerid, Color_White, "[房产]/houseedit load <文档名> - 加载一个文档(从数据库)");
        SendClientMessage(playerid, Color_White, "[房产]/houseedit unload <文档名> - 卸载一个文档，将该文档的OBJ之类的全部删除,可以在用load加载");
        SendClientMessage(playerid, Color_White, "[房产]/houseedit txtlist - 显示当前已加载的所有文档的信息(文档名、数据库ID、索引ID)");
        SendClientMessage(playerid, Color_White, "[房产]/houseedit loadall - 加载所有已安装文档");
        SendClientMessage(playerid, Color_White, "[房产]/houseedit unloadall  - 卸载所有已加载文档");
        SendClientMessage(playerid, Color_White, "[房产]/houseedit reloadconfig - 重载配置,会卸载所有文档并重新加载所有文档");
        SendClientMessage(playerid, Color_White, "[房产]/houseedit owner <文档名> <拥有者> - 给一个文档添加拥有者(系统 = 没有拥有者) add by Dylan");
        return 1;
    }
    if(mk_strcmp(tmp, "owner") == 0) {
        tmp = strtok(cmdtext, idx);
        if(!strlen(msg)) {
            /*/houseedit 文档名 新主人*/
            SendClientMessage(playerid, Color_White, "HouseEdit * 文档名不能为空.");
            return 1;
        }
        new id = -1;
        for (new i = 0; i < MAX_SELL; i++) {
            if(p_Sell[i][sell_dbid] != 0 && mk_strcmp(p_Sell[i][sell_txt], msg) == 0) {
                id = i;
                i = MAX_SELL + 1;
            }
        }
        if(id == -1) {
            SendClientMessage(playerid, Color_White, "HouseEdit * 错误的文档名.");
            return 1;
        }
        tmp = strtok(cmdtext, idx);
        if(!strlen(msg)) {
            SendClientMessage(playerid, Color_White, "HouseEdit * 拥有者不能为空.");
            return 1;
        }
        if(mk_strcmp(msg, "系统") == 0) {
            AddPlayerToTxt("(●'?'●))", p_Sell[id][sell_txt]);
            ChangeSellPlayer(id, "");
        } else {
            AddPlayerToTxt(msg, p_Sell[id][sell_txt]);
            ChangeSellPlayer(id, msg);
        }
        return 1;
    }
    if(mk_strcmp(tmp, "ins") == 0) {
        tmp = strrest(cmdtext, idx);
        if(mk_strcmp(tmp, "") != 0) {
            if(mk_strcmp(tmp, "main") == 0) return SendClientMessage(playerid, Color_White, "[PHouse]这个文档名不让用..");
            if(mk_strlen(tmp) < PHOUSE_TXTLEN) {
                Installation(tmp, playerid);
                return 1;
            } else {
                SendClientMessage(playerid, Color_White, "[PHouse]文档名字过长");
                return 1;
            }
        } else {
            SendClientMessage(playerid, Color_White, "[PHouse]请输入文档名");
            return 1;
        }
    }
    if(mk_strcmp(tmp, "unins") == 0) {
        tmp = strrest(cmdtext, idx);
        if(mk_strcmp(tmp, "") != 0) {
            if(mk_strcmp(tmp, "main") == 0) return SendClientMessage(playerid, Color_White, "[PHouse]这个文档名不让用..");
            UnInstallation(tmp, playerid);
            return 1;
        }
    }
    if(mk_strcmp(tmp, "load") == 0) {
        tmp = strrest(cmdtext, idx);
        if(mk_strcmp(tmp, "") != 0) {
            if(mk_strcmp(tmp, "main") == 0) return SendClientMessage(playerid, Color_White, "[PHouse]这个文档名不让用..");
            if(mk_strlen(tmp) < PHOUSE_TXTLEN) {
                LoadTxt(tmp, playerid);
                return 1;
            } else {
                SendClientMessage(playerid, Color_White, "[PHouse]文档名字过长");
                return 1;
            }
        }
    }
    if(mk_strcmp(tmp, "unload") == 0) {
        tmp = strrest(cmdtext, idx);
        if(mk_strcmp(tmp, "") != 0) {
            if(mk_strcmp(tmp, "main") == 0) return SendClientMessage(playerid, Color_White, "[PHouse]这个文档名不让用..");
            UnLoadTxt(tmp, playerid);
            return 1;
        }

    }
    if(!strcmp(tmp, "txtlist")) {
        GetLoadTxt(playerid);
        return 1;
    }
    if(mk_strcmp(tmp, "unloadall") == 0) {
        UnLoadAllTxt(playerid);
        return 1;
    }
    if(mk_strcmp(tmp, "loadall") == 0) {
        LoadAllTxt(playerid);
        return 1;
    }
    if(mk_strcmp(tmp, "reloadconfig") == 0) {
        ReLoadConfig(playerid);
        return 1;
    }
    // }
    return 1;
}
Dialog:HouseListDialog(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        new msg[128], idx;
        msg = strtok(inputtext, idx);
        if(strcmp(msg, "上一页") == 0) {
            SelectHousePage[playerid]--;
            House_ShowListDialog(playerid, SelectHousePage[playerid]);
            return 1;
        }
        if(strcmp(msg, "下一页") == 0) {
            SelectHousePage[playerid]++;
            House_ShowListDialog(playerid, SelectHousePage[playerid]);
            return 1;
        }
        format(msg, sizeof msg, "/house goto %s", inputtext);
        AntiCommand[playerid] = 0;
        OnPlayerCommandText(playerid, msg);
        return 1;
    }
    return 1;
}
Dialog:DIALOG_ACL_LIST(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        new txt[PHOUSE_TXTLEN], idx;
        GetPVarString(playerid, "PHouse_AclList_TxtNmae", txt, PHOUSE_TXTLEN);
        if(mk_strcmp(inputtext, "下一页") == 0) {
            SetPVarInt(playerid, "PHouse_AclList_NowSelect", GetPVarInt(playerid, "PHouse_AclList_NowSelect") + 1);
            ShowAclList(playerid, txt);
            return 1;
        }
        if(mk_strcmp(inputtext, "上一页") == 0) {
            SetPVarInt(playerid, "PHouse_AclList_NowSelect", GetPVarInt(playerid, "PHouse_AclList_NowSelect") - 1);
            ShowAclList(playerid, txt);
            return 1;
        }
        if(GetTxtPlayer(txt) != 0) {
            RedPlayerToTxt(strtok(inputtext, idx), txt);
            ShowAclList(playerid, txt);
            return 1;
        }
        return 1;
    }
    return 1;
}
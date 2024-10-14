/*[Ghost]Rui
**ushio_p
**[Ghost]Dylan
**修改请保留ID..
PRAGMA synchronous = FULL; (2)最慢
PRAGMA synchronous = NORMAL; (1)一般
PRAGMA synchronous = OFF; (0)最快
*/

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
//  特别鸣谢 GTAUN GTABBS 
// RST团队服务器开发主要由[R_ST]Hygen（YuCarl77）领导
// #include <izcmd>
#include <common/Cars> //2020.3.23适配 爱车不允许加入所以include
// #define COLOR 0x98CDFEFF

// new PlayerName[MAX_PLAYERS][128]; //玩家名字

// stock pCamera(const playerid)
// {
// 	new Float:Ppos[3]; 
// 	GetPlayerPos(playerid, Ppos[0], Ppos[1], Ppos[2]);
// 	SetPlayerCameraPos(playerid,Ppos[0]+-random(15),Ppos[1]+-random(12),Ppos[2]+3.1);
// 	SetPlayerCameraLookAt(playerid, Ppos[0], Ppos[1], Ppos[2]);
// }



/*forward FilterScriptExit();//退出脚本
public FilterScriptExit()
{
    SendRconCommand("unloadfs PRace");
}*/

/*赛道相关*/
#define RACE_MAX 520//赛道数量上线,系统提示数量不足时修改
#define RACE_PAGES 20//对话框每页的数量
// #define RACE_EDIT_MAIN_DIALOG 2333//编辑赛道的主界面,显示赛道基本信息
// #define RACE_EDIT_MIAOSHU_DIALOG RACE_EDIT_MAIN_DIALOG+1//编辑赛道的修改描述界面
// #define RACE_EDIT_CP_DIALOG RACE_EDIT_MAIN_DIALOG+2//编辑赛道的CP点操作界面
// #define RACE_EDIT_CP_SCRIPT_DIALOG RACE_EDIT_MAIN_DIALOG+3//编辑赛道CP点触发的界面
// #define RACE_GAME_MAIN_DIALOG RACE_EDIT_MAIN_DIALOG+4//赛道信息界面
// #define RACE_GAME_MAIN_DIALOG_SEL RACE_EDIT_MAIN_DIALOG+5//选择赛道界面
// #define RACE_EDIT_CP_SCRIPT_HELP RACE_EDIT_MAIN_DIALOG+6//触发说明界面
// #define RACE_EDIT_CP_SCRIPT_HELP_MSG RACE_EDIT_MAIN_DIALOG+7//触发说明界面
// #define RACE_RESPAWNDIALOG RACE_EDIT_MAIN_DIALOG+8//2020.2.5新增 提示是否继续观战的对话框
#define RACE_CP_MAX_FUNCTION 100//CP点触发每一行的长度
#define RACE_MAP_ICON_TYPE 56//赛车CP点的预先浏览图标
#define RACE_MAP_ICON_INDEX 1//赛车CP点的预先浏览图标索引
#define RACE_GAME_MAX 100 //比赛房间上限
#define RACE_RECENTLY_LIMIT 100 //查看最近比赛的上限 默认100条
// #define CPSCRIPT:%0(%1,%2,%3,%4) function Race_Cp_Script_Function_%0(%1,%2,%3,%4)
#define CPSCRIPT:%0(%1,%2,%3,%4) function RaceCpScript_func_%0(%1,%2,%3,%4)
// #define RACE_HOUSE_MAXPLAYER 6 //最大玩家数上限 目前只能最大10人房间 因为数据库中的写法问题 排名只能支持10个人


// 见更新日志2020.8.2 >> 重做 PRace 主库，删除并重写原有的排行榜字段
#define RACE_HOUSE_MAXPLAYER 16 //一个房间内最大玩家数上限 现在理论可以无限个玩家 该定义自动适配排名等其他textdraw计算 不可注释 不可乱改
// 默认一个房间6个人，2021.2.16尝试适配16人一个房间


// 赛道的话最好是支持无限扩展人数 那么就得废除原先的PRace排行榜写法
// top榜通过个人记录表去获取 加个LIMIT 和 ASC记录就可以了
// 那么最大的问题就是之前的一些记录只能人工转移了
// 创建没创建过的那些人的账号
// 然后PRace的Name也得去适配UID，废弃掉, 然后用外键啊啥的

// forward Race_Cp_Script_Function_%0(%1,%2,%3,%4);
// public Race_Cp_Script_Function_%0(%1,%2,%3,%4)

stock UnixToDate(msg[4], unix)//返回unix戳,0表示天数,1表示小时,2表示分钟,3表示秒
{
	new d,h,m;
	while(unix>=60)
	{
		unix=unix-60;
		m++;
		if(m>=60)
		{
			m=0;
			h++;
		}
		if(h>=24)
		{
			h=0;
			d++;
		}
	}
	msg[0]=d;
	msg[1]=h;
	msg[2]=m;
	msg[3]=unix;
	return msg;
}
stock ms2time(msg[3],tick)
{
	new m,
	s;
	while(tick >= 1000)
	{
		tick = tick - 1000;
		s ++;
		{
			if(s >= 60)
			{
				s = 0;
				m ++;
			}
		}
	}
	msg[0] = m;
	msg[1] = s;
	msg[2] = tick;
	return msg;
}
stock MsToS(ms)//返回秒数,将毫秒转换为秒
{
	new s,time[32];
	while(ms>=1000)
	{
		ms=ms-1000;
		s++;
	}
	if(ms<10)
	{
		format(time,32,"%i.00%i",s,ms);
		return time;
	}
	if(ms<100)
	{
		format(time,32,"%i.0%i",s,ms);
		return time;
	}
	if(ms<1000)
	{
		format(time,32,"%i.%i",s,ms);
		return time;
	}
	return time;
}


stock SetPlayerFacingAngleEx(const playerid,&Float:ang)//设置玩家面向,如果在车上则设置汽车
{
	if(GetPlayerVehicleID(playerid)==0)
	{
		SetPlayerFacingAngle(playerid,ang);
	}
	else
	{
		SetVehicleZAngle(GetPlayerVehicleID(playerid),ang);
	}
}


stock GetPlayerPosEx(const playerid,&Float:x,&Float:y,&Float:z)//获取玩家坐标,如果在车上则获取车辆坐标
{
	if(GetPlayerVehicleID(playerid)==0)
	{
		GetPlayerPos(playerid,x,y,z);
	}
	else
	{
		GetVehiclePos(GetPlayerVehicleID(playerid),x,y,z);
	}
}

stock SetPlayerPosEx(const playerid,Float:x,Float:y,Float:z)//设置玩家坐标，如果在车上则设置车辆坐标
{
	if(GetPlayerVehicleID(playerid)==0)
	{
		SetPlayerPos(playerid,x,y,z);
	}
	else
	{
		SetVehiclePos(GetPlayerVehicleID(playerid),x,y,z);
	}
}

stock Float:GetVehicleSpeed(const vehicleid)//获取车辆速度
{
	new Float:x,Float:y,Float:z;
	GetVehicleVelocity(vehicleid,x,y,z);
	return floatpower((x * x) + (y * y) + (z * z),0.5) * 180;
}

stock F2S(Float:s)//将一个float转换为字符串
{
	new msg[64];
	format(msg,64,"%f",s);
	return msg;
}


// , Text:ReSpawningText[MAX_PLAYERS];
// new Timekg[MAX_PLAYERS];
// new jishu[MAX_PLAYERS]=-1;
new pHouseid[MAX_PLAYERS];
new pRaceing[MAX_PLAYERS];
new SelectRecentlyRacePage[MAX_PLAYERS];
new SelectRecentlyClicked[MAX_PLAYERS];

enum racetype{
    rid,
    rname[32],
    rpassword[32],
    rcreatedate,
    // rauthor[32],
    rauthor, //2020.8.2修改为UID 高度依赖UID
    rauthorName[MAX_PLAYER_NAME], //UID对应的名字 只在更新赛道的时候获取对方名字 不作为数据库保存列
    rcps,//cp总数
    rstate,//赛道状态 0 - 开启 , 1 - 编辑
    reditplayer,//编辑该赛道的玩家ID
    rmiaoshu[64],
    // 这三个新增的
    Text3D:Dynamic3DText,
    DynamicCP,
    DynamicMapIcon
};
				
// enum racetoptype{
//     rtop_name[32],
//     rtop
// };
enum raceedittype{
    rraceid,//正在编辑的赛道
    rtestcp,//测试赛道的CP点
    rdialogpage,//编辑赛道dialog框的翻页
    rdialogcp,//选择的CP点
    rdialogcppage,//选择的cp点的触发页数
    rdialogcplist,//选择的cp点的第几条触发
    Float:rcpsize//cp大小
};
					
enum racecptype{
    rcpid,//cp点在数据库中的id
    rrid,//cp点所属赛道的id
    rcp,//表示第几个cp点
    Float:rcpx,//cp x
    Float:rcpy,//cp y
    Float:rcpz,//cp z
    Float:rcpa,//cp a
    Float:rcppsize//cp大小
};

enum racegametype{
    rendcount,//比赛结束倒计时
    rplayerrank,//实时排名
    rplayers[RACE_HOUSE_MAXPLAYER],//实时排名玩家的playerid
    rRaceRunTime,//这个房间比赛的计时器
    rraceid,//这个房间的赛道ID
    rtop,//比赛名次
    rps,//房间内人数
    rtimer,//倒计时的计时器
    rtimes,//倒计时的秒
    rplayerid,//房主ID
    rstate//房间状态 0 - 准备中 1 - 倒计时 2 - 已经开始
};
enum raceplayergametype{					
    rgameid,//比赛房间的ID
    rtime,//开始时间
    rgamecp
};
enum cpscripttype{
    cpsname[64],//cp触发函数名
    cpsmiaoshu[512] //cp触发函数的描述
 };

enum cpscriptreturntype{
    cpsrname[64],//cp触发返回函数名
    cpsrmiaoshu[512]  //cp触发返回函数的描述
 };

new cpscriptr[][cpscriptreturntype]={//cp触发返回函数列表
    {"#pname","#pname\n功能\n返回玩家姓名"},
    {"#rname","#rname\n功能\n返回赛道名字"},
    {"#aname","#aname\n功能\n返回赛道作者名字"},
    {"#vs","#vs\n功能\n返回车辆速度(KM/H)"},
    {"#va","#va\n功能\n返回车辆角度"},
    {"#vzs","#vzs\n功能\n返回车辆Z轴速度"},
    {"#ncpx","#ncpx\n功能\n返回下一个检查点的X坐标,如果当前检查点是最后一个,则返回0"},
    {"#ncpy","#ncpy\n功能\n返回下一个检查点的Y坐标,如果当前检查点是最后一个,则返回0"},
    {"#ncpz","#ncpz\n功能\n返回下一个检查点的Z坐标,如果当前检查点是最后一个,则返回0"},
    {"#ncpa","#ncpa\n功能\n返回当前检查点与下一个检查点的角度,如果当前检查点是最后一个检查点则返回0"}
};
new cpscript[][cpscripttype]={//cp触发函数列表
    {"spawnpos","spawnpos\n功能\n赛道重生的位置 By YuCarl77"},
    {"time","time\n功能\n更换玩家的时间 By YuCarl77\time 时 分"},
    {"weather","weather\n功能\n更换玩家的天气 By YuCarl77\n用法weather 天气ID 有效天气ID为0~255\n0-LA极睛 1-LA晴 2-LA极晴雾 3-LA晴雾\n4-LA阴\n5-SF晴\n6-SF极睛\n7-SF阴 8-SF雨 9-SF雾 10-LV晴 11-LV热睛\n12-LV阴\n13-乡村极睛\n14-乡村晴\n15-乡村阴\n16-乡村雨 17-沙漠极睛 18-沙漠睛 19-沙漠沙尘暴\n20-水下（绿，雾\n请输入天气，格式:整形 例如12"},
    {"cveh","cveh\n功能\n更换玩家的载具By YuCarl77适配Gamemode\n用法cveh 车辆id 有效车辆范围为400~611"},
    {"msg","msg\n功能\n向玩家发送一条消息\n用法\nmsg 内容1 内容2 内容3 内容4 内容5 ....\n例:msg 啊啊啊啊哦哦哦\n例:msg 两只老虎 两只老虎 跑得快 跑得快~ ~"},
    {"speed","speed\n功能\n设置车辆速度\n用法\nspeed 角度运算模式 角度 速度运算模式 速度(KM/H)\n参数不能为空,否则报错\n角度/速度运算模式的可选值为\n| > 设置角/速度\n+ > 与当前角/速度进行加运算\n- > 与当前角/速度进行减运算\n* > 与当前角/速度进行乘运算\n/ > 与当前角/速度进行除运算\n不符合+-*/的参数会被重置为|"},
    {"angle","angle\n功能\n设置车辆角度\n用法\nangle 角度运算模式 角度\n参数不能为空，否则报错\n角度运算模式的可选值为\n | > 设置角度\n + > 与当前车辆角度进行加计算\n - > 与当前车辆角度进行减计算\n * > 与当前车辆角度进行乘计算\n / > 与当前车辆角度进行除计算\n不符合+-*/的参数会被重置为|"},
    {"zspeed","zspeed\n功能\n设置车辆Z轴速度\n用法\nzspeed 速度运算模式 速度\n参数不能为空，否则报错\n速度运算模式的可选值为\n | > 设置Z轴速度\n + > 与当前车辆Z轴速度进行加计算\n - > 与当前车辆Z轴速度进行减计算\n * > 与当前车辆Z轴速度进行乘计算\n / > 与当前车辆Z轴速度进行除计算\n不符合+-*/的参数会被重置为|"},
    {"speedex","speedex\n功能\n设置车辆速度的同时设置车辆Z轴速度\n用法\nspeedex 角度运算模式 角度 速度运算模式 速度(KM/H) Z轴速度运算模式 Z轴速度\n参数不能为空,否则报错\n运算模式的可选值为\n| >设置角/速/Z轴度\n+ >与当前角/速/Z轴度进行加运算\n- >与当前角/速/Z轴度进行减运算\n* >与当前角/速/Z轴度进行乘运算\n/ >与当前角/速/Z轴度进行除运算\n不符合+-*/的参数会被重置为|"},
    {"vgoto","vgoto\n功能\n传送汽车到指定坐标\n用法\nvgoto 执行模式 x坐标 y坐标 z坐标\n参数不能为空,否则报错\n执行模式的可选值为\n s > 传送到指定坐标,但不保留速度\n v > 传送到指定坐标并保留传送前的速度\n不符合s的参数会被重置为v"},
    {"fix","fix\n功能\n修复汽车\n用法\nfix 执行模式\n参数不能为空，否则报错\n执行模式的可选值为\n f > 仅修复车辆HP,不修复外观(比如轮胎)\n r > 修复车辆外观与HP(比如轮胎)\n不符合r的参数会重置为f"},
    {"damage","damage\n功能\n对汽车轮胎造成伤害\n用法\ndamage 破坏模式\n参数不能为空,否则报错\n破坏模式的可选值为\n0>无伤害\n1>右后轮胎破坏\n2>右前轮胎破坏\n3>右前/右后轮胎破坏\n4>左后轮胎破坏\n5>左后/右后轮胎破坏\n6>左后/右前轮胎破坏\n7>左后/右前/右后轮胎破坏\8>左前轮胎破坏\n9>左前/右后轮胎破坏\n10>左前/右前轮胎破坏\n11>左前/右前/右后轮胎破坏\n12>左前/左后轮胎破坏\n13>左前/左后/右后轮胎破坏\n14>左前/左后/右前轮胎破坏\n15>全部轮胎破坏\n其他值会被重置为0"}
};
new Race[RACE_MAX][racetype];//赛道
// new RaceTop[RACE_MAX][10][racetoptype];//赛道记录
new EditRace[MAX_PLAYERS][raceedittype];//编辑赛道
new GameRace[MAX_PLAYERS][raceplayergametype];//玩家比赛房间
new RaceHouse[RACE_GAME_MAX][racegametype];//比赛房间
new SelectRacePage[MAX_PLAYERS];//选择赛道页
new PlayerText:CpTextDraw[MAX_PLAYERS];//显示CP点的TEXTDRAW
new PlayerText:Time[MAX_PLAYERS]; //显示比赛时间
new PlayerText:p_record[MAX_PLAYERS]; //个人记录时间
new PlayerText:Top[MAX_PLAYERS]; //显示房间时间
new RaceEd;//脚本是否执行
new NowRaceId;//当前赛道ID索引
// new DB:Racedb;//赛道数据库



stock Race_Cp_Script_Return(const playerid, const raceid, cpid, const str[]) //查询指定返回函数是否存在,如果存在则执行并返回结果
{
    new id = -1, msg[128];
    for (new i = 0; i < sizeof(cpscriptr); i++) {
        if(strcmp(str, cpscriptr[i][cpsrname]) == 0) {
            id = i;
            i = sizeof(cpscriptr) + 1;
        }
    }
    if(id == -1) {
        format(msg, sizeof(msg), "%s", str);
        return msg;
    }
    if(strcmp(str, "#pname") == 0) //返回姓名
    {
        format(msg, sizeof(msg), "%s", GetName(playerid));
        // format(msg,128,"%s",PlayerName[playerid]);
        return msg;
    }
    if(strcmp(str, "#rname") == 0) //返回赛道名
    {
        format(msg, sizeof(msg), "%s", Race[raceid][rname]);
        return msg;
    }
    if(strcmp(str, "#aname") == 0) //返回赛道作者名
    {
        // format(msg, sizeof(msg), "%s", Race[raceid][rauthor]);
        format(msg, sizeof(msg), "%s", Race[raceid][rauthorName]);
        return msg;
    }
    if(strcmp(str, "#vs") == 0) //返回车辆速度
    {
        format(msg, sizeof(msg), "%f", GetVehicleSpeed(GetPlayerVehicleID(playerid)));
        return msg;
    }
    if(strcmp(str, "#va") == 0) //返回车辆角度
    {
        new Float:a;
        GetPlayerFacingAngleEx(playerid, a);
        format(msg, sizeof(msg), "%f", a);
        return msg;
    }
    if(strcmp(str, "#vzs") == 0) //返回车辆角度
    {
        new Float:x, Float:y, Float:z;
        GetVehicleVelocity(GetPlayerVehicleID(playerid), x, y, z);
        format(msg, sizeof(msg), "%f", z);
        return msg;
    }
    if(strcmp(str, "#ncpa") == 0) //返回下一个检查点角度
    {
        if(cpid == Race[raceid][rcps]) {
            format(msg, sizeof(msg), "0");
            return msg;
        }
        new trcp[racecptype], nrcp[racecptype];
        Race_GetCp(raceid, cpid, trcp);
        Race_GetCp(raceid, cpid + 1, nrcp);
        new Float:a = atan2(trcp[rcpy] - nrcp[rcpy], trcp[rcpx] - nrcp[rcpx]);
        if(a > 360) a -= 360;
        a = 90 + a;
        format(msg, sizeof(msg), "%f", a);
        return msg;
    }
    if(strcmp(str, "#ncpx") == 0) //返回下一个检查点X
    {
        if(cpid == Race[raceid][rcps]) {
            format(msg, sizeof(msg), "0");
            return msg;
        }
        new trcp[racecptype];
        Race_GetCp(raceid, cpid + 1, trcp);
        format(msg, sizeof(msg), "%f", trcp[rcpx]);
        return msg;
    }
    if(strcmp(str, "#ncpy") == 0) //返回下一个检查点Y
    {
        if(cpid == Race[raceid][rcps]) {
            format(msg, sizeof(msg), "0");
            return msg;
        }
        new trcp[racecptype];
        Race_GetCp(raceid, cpid + 1, trcp);
        format(msg, sizeof(msg), "%f", trcp[rcpy]);
        return msg;
    }
    if(strcmp(str, "#ncpz") == 0) //返回下一个检查点z
    {
        if(cpid == Race[raceid][rcps]) {
            format(msg, sizeof(msg), "0");
            return msg;
        }
        new trcp[racecptype];
        Race_GetCp(raceid, cpid + 1, trcp);
        format(msg, sizeof(msg), "%f", trcp[rcpz]);
        return msg;
    }
    return msg;
}

CPSCRIPT:spawnpos(playerid, raceid, cpid, script[]) //赛道重生坐标,先临时放着还没写
{
    new tmp[128], idx;
    new Float:reSpawnPos[4];
    tmp = strtok(script, idx);
    tmp = strtok(script, idx);
    if(!strlen(tmp)) {
        format(tmp, 128, "[赛车]赛道[%s]第[%i]个检查点触发错误:spawnpos X轴为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, tmp);
        return 1;
    }
    reSpawnPos[0] = strval(tmp);
    tmp = strtok(script, idx);
    if(!strlen(tmp)) {
        format(tmp, 128, "[赛车]赛道[%s]第[%i]个检查点触发错误:spawnpos Y轴为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, tmp);
        return 1;
    }
    reSpawnPos[1] = strval(tmp);
    tmp = strtok(script, idx);
    if(!strlen(tmp)) {
        format(tmp, 128, "[赛车]赛道[%s]第[%i]个检查点触发错误:spawnpos Z轴为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, tmp);
        return 1;
    }
    reSpawnPos[2] = strval(tmp);
    tmp = strtok(script, idx);
    if(!strlen(tmp)) {
        format(tmp, 128, "[赛车]赛道[%s]第[%i]个检查点触发错误:spawnpos A轴为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, tmp);
        return 1;
    }
    reSpawnPos[3] = strval(tmp);
    SetPlayerPos(playerid, reSpawnPos[0], reSpawnPos[1], reSpawnPos[2]);
    if(PlayerInfo[playerid][BuyID] != 0) //如果玩家有车的话重生车
    {
        SetVehiclePos(PlayerInfo[playerid][BuyID], reSpawnPos[0], reSpawnPos[1], reSpawnPos[2]);
        SetVehicleZAngle(PlayerInfo[playerid][BuyID], reSpawnPos[3]); //2020.1.12写 修复 赛道CP点重生后车辆的朝向角度不是CP点的朝向角度
        SetVehicleVirtualWorld(PlayerInfo[playerid][BuyID], 6666 - GameRace[playerid][rgameid]);
        PutPlayerInVehicle(playerid, PlayerInfo[playerid][BuyID], 0);
        LinkVehicleToInterior(PlayerInfo[playerid][BuyID], GetPlayerInterior(playerid));
        AddVehicleComponent(PlayerInfo[playerid][BuyID], 1010); //给车一个氮气
    }
    return 1;
}
CPSCRIPT:time(playerid, raceid, cpid, script[]) //cp触发 更换时间 2020.2.22增加
{
    new tmp[128], idx;
    tmp = strtok(script, idx);
    tmp = strtok(script, idx);
    if(!strlen(tmp)) {
        format(tmp, 128, "[赛车]赛道[%s]第[%i]个检查点触发错误:time 更换时为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, tmp);
        return 1;
    }
    new hour;
    hour = strval(tmp);
    tmp = strtok(script, idx);
    if(!strlen(tmp)) {
        format(tmp, 128, "[赛车]赛道[%s]第[%i]个检查点触发错误:time 更换分为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, tmp);
        return 1;
    }
    new minute;
    minute = strval(tmp);
    if(hour < 0 || hour > 24) {
        format(tmp, 128, "[赛车]赛道[%s]第[%i]个检查点触发错误:time 时格式错误,为0~24", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, tmp);
        return 1;
    }
    if(minute < 0 || minute > 59) {
        format(tmp, 128, "[赛车]赛道[%s]第[%i]个检查点触发错误:time 分格式错误,为0~59", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, tmp);
    }
    SetPlayerTime(playerid, hour, minute);
    return 1;
}

//2020.2.18 新增 By YuCarl77
CPSCRIPT:weather(playerid, raceid, cpid, script[]) //cp触发 更换天气
{
    new tmp[128], idx;
    tmp = strtok(script, idx);
    tmp = strtok(script, idx);
    if(!strlen(tmp)) {
        format(tmp, 128, "[赛车]赛道[%s]第[%i]个检查点触发错误:weather 更换天气的ID为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, tmp);
        return 1;
    }
    new weatherid;
    weatherid = strval(tmp);
    if(weatherid < 0 || weatherid > 255) {
        format(tmp, 128, "[赛车]赛道[%s]第[%i]个检查点触发错误:weather 更换天气的范围错误", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, tmp);
        return 1;
    }
    SetPlayerWeather(playerid, weatherid);
    return 1;
}

//2020.2.18 新增 By YuCarl77 适配gamemode刷车
CPSCRIPT:cveh(playerid, raceid, cpid, script[]) //cp触发 更换汽车
{
    new tmp[128], idx;
    tmp = strtok(script, idx);
    tmp = strtok(script, idx);
    if(!strlen(tmp)) {
        format(tmp, 128, "[赛车]赛道[%s]第[%i]个检查点触发错误:cveh 车辆ID不能为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, tmp);
        return 1;
    }
    new car;
    car = strval(tmp);
    if(car < 400 || car > 611) {
        format(tmp, 128, "[赛车]赛道[%s]第[%i]个检查点触发错误:cveh 车辆ID错误", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, tmp);
        return 1;
    }
    new Float:vx, Float:vz, Float:vy;
    GetVehicleVelocity(GetPlayerVehicleID(playerid), vx, vy, vz);
    SpawnVehicle(playerid, car);
    SetVehicleVelocity(GetPlayerVehicleID(playerid), vx, vy, vz);
    return 1;
}

CPSCRIPT:damage(playerid, raceid, cpid, script[]) //cp触发 轮胎破坏
{
    if(GetPlayerVehicleID(playerid) == 0) return 1;
    new tmp[128], idx;
    tmp = strtok(script, idx);
    tmp = strtok(script, idx); //破坏模式
    if(strcmp(tmp, "") == 0) {
        format(tmp, 128, "[赛车]赛道[%s]第[%i]个检查点触发错误:damage 破坏模式不能为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, tmp);
        return 1;
    }
    new panels, doors, lights, tires, vid = GetPlayerVehicleID(playerid);
    GetVehicleDamageStatus(vid, panels, doors, lights, tires);
    UpdateVehicleDamageStatus(vid, panels, doors, lights, strval(Race_Cp_Script_Return(playerid, raceid, cpid, tmp)));
    return 1;
}

CPSCRIPT:fix(playerid, raceid, cpid, script[]) //cp触发 修复汽车
{
    if(GetPlayerVehicleID(playerid) == 0) return 1;
    new tmp[128], idx;
    tmp = strtok(script, idx);
    tmp = strtok(script, idx); //执行模式
    if(strcmp(tmp, "") == 0) {
        format(tmp, 128, "[赛车]赛道[%s]第[%i]个检查点触发错误:fix 执行模式不能为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, tmp);
        return 1;
    }
    new b = -1;
    if(strcmp(tmp, "r") == 0) b = 1;
    if(b == -1) {
        SetVehicleHealth(GetPlayerVehicleID(playerid), 1000);
    } else {
        RepairVehicle(GetPlayerVehicleID(playerid));
    }
    return 1;
}

CPSCRIPT:vgoto(playerid, raceid, cpid, script[]) //cp触发 传送汽车
{
    if(GetPlayerVehicleID(playerid) == 0) return 1;
    new tmp[128], msg[128], tmp1[128], idx;
    tmp = strtok(script, idx);
    tmp = strtok(script, idx); //执行模式
    if(strcmp(tmp, "") == 0) {
        format(msg, sizeof(msg), "[赛车]赛道[%s]第[%i]个检查点触发错误:vgoto 执行模式不能为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, msg);
        return 1;
    }
    tmp1 = strtok(script, idx); //x坐标
    if(strcmp(tmp1, "") == 0) {
        format(msg, sizeof(msg), "[赛车]赛道[%s]第[%i]个检查点触发错误:vgoto x坐标不能为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, msg);
        return 1;
    }
    new tmp2[128];
    tmp2 = strtok(script, idx); //y坐标
    if(strcmp(tmp2, "") == 0) {
        format(msg, sizeof(msg), "[赛车]赛道[%s]第[%i]个检查点触发错误:vgoto y坐标不能为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, msg);
        return 1;
    }
    new tmp3[128];
    tmp3 = strtok(script, idx); //z坐标
    if(strcmp(tmp3, "") == 0) {
        format(msg, sizeof(msg), "[赛车]赛道[%s]第[%i]个检查点触发错误:vgoto z坐标不能为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, msg);
        return 1;
    }
    new b = -1, Float:nx, Float:ny, Float:nz;
    nx = floatstr(Race_Cp_Script_Return(playerid, raceid, cpid, tmp1));
    ny = floatstr(Race_Cp_Script_Return(playerid, raceid, cpid, tmp2));
    nz = floatstr(Race_Cp_Script_Return(playerid, raceid, cpid, tmp3));
    if(strcmp(tmp, "s") == 0) b = 1;
    if(b == 1) {
        SetPlayerPosEx(playerid, nx, ny, nz);
    } else {
        new Float:vx, Float:vz, Float:vy;
        GetVehicleVelocity(GetPlayerVehicleID(playerid), vx, vy, vz);
        SetPlayerPosEx(playerid, nx, ny, nz);
        SetVehicleVelocity(GetPlayerVehicleID(playerid), vx, vy, vz);
    }
    return 1;
}

CPSCRIPT:zspeed(playerid, raceid, cpid, script[]) //cp触发 设置车辆Z轴速度
{
    if(GetPlayerVehicleID(playerid) == 0) return 1;
    new tmp[128], msg[128], tmp1[128], idx;
    tmp = strtok(script, idx);
    tmp = strtok(script, idx); //速度运算模式
    if(strcmp(tmp, "") == 0) {
        format(msg, sizeof(msg), "[赛车]赛道[%s]第[%i]个检查点触发错误:zspeed 速度运算模式不能为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, msg);
        return 1;
    }
    tmp1 = strtok(script, idx); //速度
    if(strcmp(tmp1, "") == 0) {
        format(msg, sizeof(msg), "[赛车]赛道[%s]第[%i]个检查点触发错误:zspeed 速度不能为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, msg);
        return 1;
    }
    new Float:z, Float:nz, b = -1, Float:nx, Float:ny;
    z = floatstr(Race_Cp_Script_Return(playerid, raceid, cpid, tmp1));
    GetVehicleVelocity(GetPlayerVehicleID(playerid), nx, ny, nz);
    if(strcmp(tmp, "+") == 0) nz = nz + z, b = 1;
    if(strcmp(tmp, "-") == 0) nz = nz - z, b = 1;
    if(strcmp(tmp, "*") == 0) nz = nz * z, b = 1;
    if(strcmp(tmp, "/") == 0) nz = nz / z, b = 1;
    if(b == -1) nz = z;
    SetVehicleVelocity(GetPlayerVehicleID(playerid), nx, ny, nz);
    return 1;
}

CPSCRIPT:angle(playerid, raceid, cpid, script[]) //cp触发 设置车辆角度
{
    if(GetPlayerVehicleID(playerid) == 0) return 1;
    new tmp[128], msg[128], tmp1[128], idx;
    tmp = strtok(script, idx);
    tmp = strtok(script, idx); //角度运算模式
    if(strcmp(tmp, "") == 0) {
        format(msg, sizeof(msg), "[赛车]赛道[%s]第[%i]个检查点触发错误:angle 角度运算模式不能为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, msg);
        return 1;
    }
    tmp1 = strtok(script, idx); //角度
    if(strcmp(tmp1, "") == 0) {
        format(msg, sizeof(msg), "[赛车]赛道[%s]第[%i]个检查点触发错误:angle 角度不能为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, msg);
        return 1;
    }
    new Float:a, Float:na, b = -1;
    a = floatstr(Race_Cp_Script_Return(playerid, raceid, cpid, tmp1));
    GetPlayerFacingAngleEx(playerid, na);
    if(strcmp(tmp, "+") == 0) na = na + a, b = 1;
    if(strcmp(tmp, "-") == 0) na = na - a, b = 1;
    if(strcmp(tmp, "*") == 0) na = na * a, b = 1;
    if(strcmp(tmp, "/") == 0) na = na / a, b = 1;
    if(b == -1) na = a;
    SetPlayerFacingAngleEx(playerid, na);
    return 1;
}

CPSCRIPT:speedex(playerid, raceid, cpid, script[]) //cp触发 设置车辆速度的同时设置Z轴速度
{
    if(GetPlayerVehicleID(playerid) == 0) return 1;
    new msg[128], tmp[128], tmp1[128], tmp2[128], tmp3[128], idx, Float:a, Float:speed, vid = GetPlayerVehicleID(playerid);
    tmp = strtok(script, idx);
    tmp = strtok(script, idx); //角度运算模式
    if(strcmp(tmp, "") == 0) {
        format(msg, sizeof(msg), "[赛车]赛道[%s]第[%i]个检查点触发错误:speedex 角度运算模式不能为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, msg);
        return 1;
    }
    tmp1 = strtok(script, idx); //角度
    if(strcmp(tmp1, "") == 0) {
        format(msg, sizeof(msg), "[赛车]赛道[%s]第[%i]个检查点触发错误:speedex 角度不能为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, msg);
        return 1;
    }
    tmp2 = strtok(script, idx); //速度运算模式
    if(strcmp(tmp2, "") == 0) {
        format(msg, sizeof(msg), "[赛车]赛道[%s]第[%i]个检查点触发错误:speedex 速度运算模式不能为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, msg);
        return 1;
    }
    tmp3 = strtok(script, idx); //速度
    if(strcmp(tmp3, "") == 0) {
        format(msg, sizeof(msg), "[赛车]赛道[%s]第[%i]个检查点触发错误:speedex 速度运算模式不能为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, msg);
        return 1;
    }
    new tmp4[128];
    tmp4 = strtok(script, idx); //Z轴运算模式
    if(strcmp(tmp4, "") == 0) {
        format(msg, sizeof(msg), "[赛车]赛道[%s]第[%i]个检查点触发错误:speedex Z轴运算模式不能为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, msg);
        return 1;
    }
    new tmp5[128];
    tmp5 = strtok(script, idx); //Z轴速度
    if(strcmp(tmp5, "") == 0) {
        format(msg, sizeof(msg), "[赛车]赛道[%s]第[%i]个检查点触发错误:speedex Z轴速度不能为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, msg);
        return 1;
    }
    new Float:z;
    a = floatstr(Race_Cp_Script_Return(playerid, raceid, cpid, tmp1));
    speed = floatstr(Race_Cp_Script_Return(playerid, raceid, cpid, tmp3));
    z = floatstr(Race_Cp_Script_Return(playerid, raceid, cpid, tmp5));
    new Float:na, Float:nspeed, b = -1;
    new Float:x, Float:y, Float:nz;
    GetVehicleVelocity(vid, x, y, nz);
    GetPlayerFacingAngleEx(playerid, na);
    na = na + 90;
    nspeed = GetVehicleSpeed(vid);
    if(strcmp(tmp, "+") == 0) na = na + a, b = 1;
    if(strcmp(tmp, "-") == 0) na = na - a, b = 1;
    if(strcmp(tmp, "*") == 0) na = na * a, b = 1;
    if(strcmp(tmp, "/") == 0) na = na / a, b = 1;
    if(b == -1) na = a + 90;
    b = -1;
    if(strcmp(tmp2, "+") == 0) nspeed = nspeed + speed, b = 1;
    if(strcmp(tmp2, "-") == 0) nspeed = nspeed - speed, b = 1;
    if(strcmp(tmp2, "*") == 0) nspeed = nspeed * speed, b = 1;
    if(strcmp(tmp2, "/") == 0) nspeed = nspeed / speed, b = 1;
    if(b == -1) nspeed = speed;
    b = -1;
    if(strcmp(tmp4, "+") == 0) nz = nz + z, b = 1;
    if(strcmp(tmp4, "-") == 0) nz = nz - z, b = 1;
    if(strcmp(tmp4, "*") == 0) nz = nz * z, b = 1;
    if(strcmp(tmp4, "/") == 0) nz = nz / z, b = 1;
    if(b == -1) nz = z;
    nspeed = nspeed * 0.005556;
    x = nspeed * floatcos(na, degrees);
    y = nspeed * floatsin(na, degrees);
    SetVehicleVelocity(vid, x, y, nz);
    return 1;
}

CPSCRIPT:speed(playerid, raceid, cpid, script[]) //cp触发 设置车辆速度
{
    if(GetPlayerVehicleID(playerid) == 0) return 1;
    new msg[128], tmp[128], tmp1[128], tmp2[128], tmp3[128], idx, Float:a, Float:speed, vid = GetPlayerVehicleID(playerid);
    tmp = strtok(script, idx);
    tmp = strtok(script, idx); //角度运算模式
    if(strcmp(tmp, "") == 0) {
        format(msg, sizeof(msg), "[赛车]赛道[%s]第[%i]个检查点触发错误:speed 角度运算模式不能为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, msg);
        return 1;
    }
    tmp1 = strtok(script, idx); //角度
    if(strcmp(tmp1, "") == 0) {
        format(msg, sizeof(msg), "[赛车]赛道[%s]第[%i]个检查点触发错误:speed 角度不能为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, msg);
        return 1;
    }
    tmp2 = strtok(script, idx); //速度运算模式
    if(strcmp(tmp2, "") == 0) {
        format(msg, sizeof(msg), "[赛车]赛道[%s]第[%i]个检查点触发错误:speed 速度运算模式不能为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, msg);
        return 1;
    }
    tmp3 = strtok(script, idx); //速度
    if(strcmp(tmp3, "") == 0) {
        format(msg, sizeof(msg), "[赛车]赛道[%s]第[%i]个检查点触发错误:speed 速度运算模式不能为空", Race[raceid][rname], cpid);
        SendClientMessage(playerid, Color_Race, msg);
        return 1;
    }
    a = floatstr(Race_Cp_Script_Return(playerid, raceid, cpid, tmp1));
    speed = floatstr(Race_Cp_Script_Return(playerid, raceid, cpid, tmp3));
    new Float:na, Float:nspeed, b = -1;
    GetPlayerFacingAngleEx(playerid, na);
    na = na + 90;
    nspeed = GetVehicleSpeed(vid);
    if(strcmp(tmp, "+") == 0) na = na + a, b = 1;
    if(strcmp(tmp, "-") == 0) na = na - a, b = 1;
    if(strcmp(tmp, "*") == 0) na = na * a, b = 1;
    if(strcmp(tmp, "/") == 0) na = na / a, b = 1;
    if(b == -1) na = a + 90;
    b = -1;
    if(strcmp(tmp2, "+") == 0) nspeed = nspeed + speed, b = 1;
    if(strcmp(tmp2, "-") == 0) nspeed = nspeed - speed, b = 1;
    if(strcmp(tmp2, "*") == 0) nspeed = nspeed * speed, b = 1;
    if(strcmp(tmp2, "/") == 0) nspeed = nspeed / speed, b = 1;
    if(b == -1) nspeed = speed;
    new Float:x, Float:y, Float:z;
    GetVehicleVelocity(vid, x, y, z);
    nspeed = nspeed * 0.005556;
    x = nspeed * floatcos(na, degrees);
    y = nspeed * floatsin(na, degrees);
    SetVehicleVelocity(vid, x, y, z);
    return 1;
}

CPSCRIPT:msg(playerid, raceid, cpid, script[]) //cp触发 显示消息
{
    new msg[256], idx, tmp[128];
    tmp = strtok(script, idx);
    while (strcmp(tmp, "") != 0) {
        tmp = strtok(script, idx);
        if(strcmp(tmp, "") != 0) {
            format(msg, sizeof(msg), "%s%s", msg, Race_Cp_Script_Return(playerid, raceid, cpid, tmp));
        }
    }
    SendClientMessage(playerid, Color_Race, msg);
}


stock Race_Cp_Script_Check(const funname[]) //查询指定函数是否存在
{
    new s = -1;
    for (new i = 0; i < sizeof(cpscript); i++) {
        if(strcmp(funname, cpscript[i][cpsname]) == 0) {
            s = i;
            i = sizeof(cpscript) + 1;
        }
    }
    return s;
}


stock Race_Init() //初始化赛道变量
{
    NowRaceId = 0;
    for (new i = 0; i < RACE_MAX; i++) {
        Race[i][rid] = -1;
        Race[i][reditplayer] = -1;
        // for (new t = 0; t < 10; t++) {
        //     format(RaceTop[i][t][rtop_name], 32, "无");
        //     RaceTop[i][t][rtop] = -1;
        // }
    }
}

stock Race_Game_Init() //初始化房间变量
{
    for (new i = 0; i < RACE_GAME_MAX; i++) {
        RaceHouse[i][rraceid] = -1;
        RaceHouse[i][rtop] = 1;
        RaceHouse[i][rstate] = 0;
        RaceHouse[i][rps] = 0;
        RaceHouse[i][rtimer] = -1;
        RaceHouse[i][rplayerrank] = -1;
        RaceHouse[i][rendcount] = -1;
        RaceHouse[i][rRaceRunTime] = -1;
        RaceHouse[i][rtimes] = 5;
        RaceHouse[i][rplayerid] = -1;
    }
}

stock Race_Game_End(const playerid, const raceid, const time) //指定玩家完成指定赛道,刷新记录返回记录,没刷新返回-1
{
    // new ttime, id = -1; //,ti=GetTickCount();
    // for (new i = 0; i < 10; i++) {
    //     if(RaceTop[raceid][i][rtop] == -1) {
    //         ttime = 999999999;
    //     } else {
    //         ttime = RaceTop[raceid][i][rtop];
    //     }
    //     if(time < ttime) {
    //         id = i;
    //         i = 11;
    //     }
    // }
    // if(id == -1) return id;
    // new tpm[1024];
    // mysql_tquery(g_Sql, "START TRANSACTION;");
    // for (new i = 9; i > id; i--) {
    //     RaceTop[raceid][i][rtop] = RaceTop[raceid][i - 1][rtop];
    //     format(RaceTop[raceid][i][rtop_name], 32, "%s", RaceTop[raceid][i - 1][rtop_name]);
    //     mysql_format(g_Sql, tpm, sizeof(tpm), "UPDATE Race SET top%i_name = '%e' ,top%i='%i' WHERE rname = '%e'", i + 1, RaceTop[raceid][i][rtop_name], i + 1, RaceTop[raceid][i][rtop], Race[raceid][rname]);
    //     mysql_tquery(g_Sql, tpm);
    // }
    // RaceTop[raceid][id][rtop] = time;
    // format(RaceTop[raceid][id][rtop_name], 32, "%s", GetName(playerid));
    // mysql_format(g_Sql, tpm, sizeof(tpm), "UPDATE Race SET top%i_name = '%e' ,top%i='%i' WHERE rname = '%e'", id + 1, RaceTop[raceid][id][rtop_name], id + 1, RaceTop[raceid][id][rtop], Race[raceid][rname]);
    // mysql_tquery(g_Sql, tpm);
    // // db_free_result(db_query(Racedb, tpm));
    // mysql_tquery(g_Sql, "COMMIT;");
    //printf(tpm);
    //printf("  >P-Race 更新赛道[%s]的记录,耗时:%ims",Race[raceid][rname],GetTickCount()-ti);
    // return id;

    // 2020.8.2重写
    new Query[256];
    mysql_format(g_Sql, Query, sizeof(Query), "SELECT `p_record` FROM `race_record` WHERE `rid` = %d ORDER BY `p_record` ASC LIMIT 10 ", Race[raceid][rid]);
    mysql_pquery(g_Sql, Query, "RaceGameEndQuery", "ddd", playerid, raceid, time);
}

function RaceGameEndQuery(playerid, raceid, time) {
    new cut = cache_num_rows();
    new id = -1;
    if(cut)
    {
        for(new i = 0; i < cache_num_rows(); i++)
        {
            new top;
            cache_get_value_index_int(i, 0, top);
            if(time < top)
            {
                id = i;
                break;
            }
        }
    }
    if(id != -1)
    {
        new msg[128];
        format(msg, sizeof(msg), "[赛车] 恭喜 %s 登上了赛道 %s 的排行榜 {FFFF00}No.%i! 祝贺Ta!", GetName(playerid), Race[raceid][rname], id + 1);
        SendClientMessageToAll(Color_Race, msg);
        printf("%s", msg);
    }
    pHouseid[playerid] = -1;
}

//测试实时排名 by ryddawn & YuCarl77
function RaceRunRank(houseid) {
    new string[32];
    // new playerCP[7], //当前cp
    //     Float:playerDist[7]; //距离下一个cp距离
    // playerCP[6] = -1;


    new playerCP[RACE_HOUSE_MAXPLAYER+1], //当前cp
        Float:playerDist[RACE_HOUSE_MAXPLAYER+1]; //距离下一个cp距离
    playerCP[RACE_HOUSE_MAXPLAYER] = -1;

    // new rank[7] = {6, 6, 6, 6, 6, 6, 6}; //排行,默认6个排行都是player0,也就是基准CP点-1
    
    new rank[RACE_HOUSE_MAXPLAYER+1] = {RACE_HOUSE_MAXPLAYER,...}; //排行,默认房间内所有玩家的排行都是player0,也就是基准CP点-1
    // 如果出问题 可能是不能用简写吧 {xxx,...}

    //new tick = GetTickCount();
    new trcp[racecptype];
    // for (new i = 0; i < RaceHouse[houseid][rps]; i++) { //2020.4.1改 优化算法 实时根据人数去算
    for (new i = 0; i < RACE_HOUSE_MAXPLAYER; i++) { //2021.2.14 改回正常优化版本
        // for (new i = 0; i < 6; i++) {
        new player = RaceHouse[houseid][rplayers][i];
        if(player == INVALID_PLAYER_ID) continue;
        playerCP[i] = GameRace[player][rgamecp];
        if(playerCP[i] > Race[RaceHouse[GameRace[player][rgameid]][rraceid]][rcps]) continue;
        //如果已经完赛的话就跳过吧  2020.2.9写 理论上把下面那一条改了就可以判断终点的了？？？
        // if(playerCP[i]==Race[RaceHouse[GameRace[player][rgameid]][rraceid]][rcps]) continue;
        //2020.2.9  发现还没吃第一个点之前和终点不检测 
        Race_GetCp(RaceHouse[GameRace[player][rgameid]][rraceid], GameRace[player][rgamecp], trcp);
        playerDist[i] = GetPlayerDistanceFromPoint(player, trcp[rcpx], trcp[rcpy], trcp[rcpz]);
        /*printf("x:%f .... y:%f .... z:%f",trcp[rcpx],trcp[rcpy],trcp[rcpz]);
        printf("playerid:%d,CP:%i,Dist%f",player,playerCP[i],playerDist[i]);*/
    }
    // for (new i = 0; i < 6; i++) {//
    new test;
    // for (new i = 0; i < RaceHouse[houseid][rps]; i++) {
    for (new i = 0; i < RACE_HOUSE_MAXPLAYER; i++) { //2021.2.14 改回正常优化版本
        // for (new j = 0; j < RaceHouse[houseid][rps]; j++) {
        for (new j = 0; j < RACE_HOUSE_MAXPLAYER; j++) { //2021.2.14 改回正常优化版本
            // for (new j = 0; j < 6; j++) {
            if(test == 1) { //如果已经找到了位置
                test = 0; //重置为0
                break; //跳出循环
            }
            new playera = rank[j];
            if(playerCP[i] < playerCP[playera]) //如果 i 的cp小于 排行 j 的cp,就结束本次循环,进行下一次
                continue;
            if(playerCP[i] > playerCP[playera] || (playerCP[i] == playerCP[playera] && playerDist[i] < playerDist[playera])) {
                //如果 i 的cp大于排行 j 的cp,或者 i 的cp等于 排行 j的cp,并且 i 的距离 小于排行 j 的距离
                // for (new temp = 6; temp >= j; temp--) { //从最后一位开始
                for (new temp = RaceHouse[houseid][rps]; temp >= j; temp--) { //从最后一位开始
                    if(temp == j) { //把其他玩家都往下一位,当前的名词就设置为 i 
                        rank[temp] = i;
                        test = 1; //告诉外面的循环,这里已经找到了位置,并重新排序了排名
                        break;
                    }
                    rank[temp] = rank[temp - 1]; //把玩家往下移一位.
                }
            }
        }
    }
    // for (new i = 0; i < RaceHouse[houseid][rps]; i++) {
    for (new i = 0; i < RACE_HOUSE_MAXPLAYER; i++) { //2021.2.14 改回正常优化版本
        // for (new i = 0; i < 6; i++) {
        new idx = RaceHouse[houseid][rplayers][rank[i]];
        if(idx == INVALID_PLAYER_ID) continue;
        if(i == 0) {
            format(string, sizeof(string), "RANK / %i st", (i + 1));
        } else if(i == 1) {
            format(string, sizeof(string), "RANK / %i nd", (i + 1));
        } else if(i == 2) {
            format(string, sizeof(string), "RANK / %i rd", (i + 1));
        } else {
            format(string, sizeof(string), "RANK / %i th", (i + 1));
        }
        // format(string, sizeof(string), "房间排名 / %i",(i+1));
        PlayerTextDrawSetString(idx, Top[idx], string);
        // 展示给所有观战他的人
        for (new j = GetPlayerPoolSize(); j >= 0; j--) {
            if(IsPlayerConnected(j) && PlayerInfo[j][Login])
            {
                if(PlayerInfo[j][tvid] == idx && PlayerInfo[j][tvzt]) {
                    PlayerTextDrawSetString(j, Top[j], string);
                }
            }
        }
    }
    //printf("房间%d,本次处理实时排名耗时[%i]ms", houseid,GetTickCount() - tick);
    return 0;
}
function RaceRunTime(houseid) { //莫个房间的比赛时间计时器
    // for (new i = 0; i < RaceHouse[houseid][rps]; i++) {
    for (new i = 0; i < RACE_HOUSE_MAXPLAYER; i++) {
        // 2021.2.14 难不成是因为退了玩家 然后循环次数问题 改成  i <= RaceHouse[houseid][rps]  ?
        // 但是不太现实呀 应该不是这个问题 计时器应该还是在走的只是现实给玩家出不来了
        // 如果玩家退了的话 只会循环剩下房间内的总人数了而不是开赛之前的玩家总数，所以当房主离开后，遍历不到房间里最后一个加入的玩家
        // 所以这种 i < RaceHouse[houseid][rps] 的写法是8月2改出来的BUG 还是得配合判断玩家ID是否为有效ID进行
        // 所有用了这种的还是改成 i < RACE_HOUSE_MAXPLAYER
        new pid = RaceHouse[houseid][rplayers][i];
        // 2021.2.14 新增 如果是无效玩家ID就不循环
        if(pid == INVALID_PLAYER_ID) continue;
        new msg[3], string[32];
        ms2time(msg, GetTickCount() - GameRace[pid][rtime]);
        msg[2] /= 10;
        format(string, sizeof(string), "TIME / %02d:%02d:%02d", msg[0], msg[1], msg[2]);
        PlayerTextDrawSetString(pid, Time[pid], string);
        // 展示给所有观战他的人
        for (new j = GetPlayerPoolSize(); j >= 0; j--) {
            if(IsPlayerConnected(j) && PlayerInfo[j][Login])
            {
                if(PlayerInfo[j][tvid] == pid && PlayerInfo[j][tvzt]) {
                    PlayerTextDrawSetString(j, Time[j], string);
                }
            }
        }
    }
}
function Race_Game_Start_s(houseid) { //倒计时某个比赛房间
    new msg[128];
    if(RaceHouse[houseid][rstate] == 1) {
        if(RaceHouse[houseid][rtimes] == 0) {
            KillTimer(RaceHouse[houseid][rtimer]);
            //2020.1.12写，如果是1个人的房间没必要启动排名的计时，浪费系统资源计算
            RaceHouse[houseid][rstate] = 2;

            // if(RaceHouse[houseid][rps] > 1) RaceHouse[houseid][rplayerrank] = SetTimerEx("RaceRunRank", 200, true, "i", houseid);
            if(RaceHouse[houseid][rps] > 1) RaceHouse[houseid][rplayerrank] = SetTimerEx_("RaceRunRank", 200, 200, -1, "i", houseid);
            // RaceHouse[houseid][rRaceRunTime] = SetTimerEx("RaceRunTime", 10, true, "i", houseid);
            RaceHouse[houseid][rRaceRunTime] = SetTimerEx_("RaceRunTime", 10, 10, -1, "i", houseid);
            // format(msg,128,"[赛车]比赛开始!gogogogo!");
            // for (new i = GetPlayerPoolSize(); i >= 0; i--) {
            //     if(IsPlayerConnected(i) == 1) {
            //         if(GameRace[i][rgameid] == houseid) {
            //             pHouseid[i] = houseid;
            //             GameTextForPlayer(i, "~y~Start!", 850, 3); //倒计时
            //             Race_ShowCp(i, RaceHouse[houseid][rraceid], GameRace[i][rgamecp]);
            //             // jishu[i] = SetTimerEx("RaceRunTime", 10, true, "i", i);
            //             /*TogglePlayerControllable(i,1);*/
            //             GameRace[i][rtime] = GetTickCount();
            //             new Float:pX, Float:pY, Float:pZ;
            //             GetPlayerPos(i, pX, pY, pZ);
            //             PlayerPlaySound(i, 1057, pX, pY, pZ);
            //             pRaceing[i] = 1;
            //         }
            //     }
            // }
            // for (new i = 0; i < RaceHouse[houseid][rps]; i++) { //2020.8.2改 优化算法 实时根据人数去算
            for (new i = 0; i < RACE_HOUSE_MAXPLAYER; i++) { //2021.2.14 改回正常优化版本
                new player = RaceHouse[houseid][rplayers][i];
                if(player == INVALID_PLAYER_ID) continue;
                pHouseid[player] = houseid;
                GameTextForPlayer(player, "~y~Start!", 850, 3); //倒计时
                Race_ShowCp(player, RaceHouse[houseid][rraceid], GameRace[player][rgamecp]);
                GameRace[player][rtime] = GetTickCount();
                PlayerPlaySoundEx(player, 1057);
                pRaceing[player] = 1;
            }
            //Race_Game_Msg(houseid,msg);
        } else {
            format(msg, sizeof(msg), "~y~%i", RaceHouse[houseid][rtimes]);
            //Race_Game_Msg(houseid,msg);
            // for (new i = 0; i < RaceHouse[houseid][rps]; i++) { //2020.8.2改 优化算法 实时根据人数去算
            for (new i = 0; i < RACE_HOUSE_MAXPLAYER; i++) { //2021.2.14 改回正常优化版本
                new player = RaceHouse[houseid][rplayers][i];
                if(player == INVALID_PLAYER_ID) continue;
                if(RaceHouse[houseid][rtimes] <= 1) {
                    SetCameraBehindPlayer(player);
                }
                GameTextForPlayer(player, msg, 850, 3);
                PlayerPlaySoundEx(player, 1056);
                // 展示给所有观战他的人
                for (new j = GetPlayerPoolSize(); j >= 0; j--) {
                    if(IsPlayerConnected(j) && PlayerInfo[j][Login])
                    {
                        if(PlayerInfo[j][tvid] == player && PlayerInfo[j][tvzt]) {
                            GameTextForPlayer(j, msg, 850, 3);
                            PlayerPlaySoundEx(j, 1056);
                        }
                    }
                }
                /*TogglePlayerControllable(i,1);*/
            }
            // for (new i = GetPlayerPoolSize(); i >= 0; i--) {
            //     if(IsPlayerConnected(i) == 1) {
            //         if(GameRace[i][rgameid] == houseid) {
            //             if(RaceHouse[houseid][rtimes] <= 1) {
            //                 SetCameraBehindPlayer(i);
            //             }
            //             /*if(RaceHouse[houseid][rtimes]>=3)
            //             {
            //             	pCamera(i);
            //             }*/
            //             GameTextForPlayer(i, msg, 850, 3);
            //             new Float:pX, Float:pY, Float:pZ;
            //             GetPlayerPos(i, pX, pY, pZ);
            //             PlayerPlaySound(i, 1056, pX, pY, pZ);
            //             /*TogglePlayerControllable(i,1);*/
            //         }
            //     }
            // }
            RaceHouse[houseid][rtimes]--;
        }
    }
}

stock Race_Game_Start(const houseid) //使指定房间开始比赛
{
    RaceHouse[houseid][rstate] = 1;
    RaceHouse[houseid][rtimes] = 5;
    // RaceHouse[houseid][rtimer] = SetTimerEx("Race_Game_Start_s", 1000, 1, "i", houseid);
    RaceHouse[houseid][rtimer] = SetTimerEx_("Race_Game_Start_s", 1000, 1000, -1, "i", houseid);
    Race_Game_Msg(houseid, "[赛车] 倒计时开始...");
}

stock Race_Game_Msg(const houseid, const str[]) //向指定房间的玩家发送消息
{
    // for (new i = GetPlayerPoolSize(); i >= 0; i--) {
    //     if(IsPlayerConnected(i) == 1) {
    //         if(GameRace[i][rgameid] == houseid) {
    //             SendClientMessage(i, Color_Race, str);
    //         }
    //     }
    // }
    // for (new i = 0; i < RaceHouse[houseid][rps]; i++) { //2020.8.2改 优化算法 实时根据人数去算
    for (new i = 0; i < RACE_HOUSE_MAXPLAYER; i++) { //2021.2.14 改回正常优化版本
        new player = RaceHouse[houseid][rplayers][i];
        if(player == INVALID_PLAYER_ID) continue;
        SendClientMessage(player, Color_Race, str);
    }
    return 1;
}

stock Race_Game_Quit(const playerid) //使指定玩家退出房间
{
    // SendClientMessage(playerid, Color_Race, "[赛车] 遗憾! 你离开了比赛！");

    new hid = GameRace[playerid][rgameid];
    // new msg[96];
    // format(msg, sizeof(msg), "[赛车] 遗憾! %s 离开了比赛", GetName(playerid));
    // Race_Game_Msg(hid, msg);

    GameRace[playerid][rgameid] = -1;
    GameRace[playerid][rtime] = 0;
    GameRace[playerid][rgamecp] = 0; //2020.1.14改 如果有问题就改回1       15:20
    // KillTimer(jishu[playerid]);
    // TextDrawDestroy(ReSpawningText[playerid]);
    // TextDrawHideForPlayer(playerid,CpTextDraw[playerid]);
    DestroyPRaceTextDraw(playerid);
    // 销毁所有观战他的人的textdraw
    for (new j = GetPlayerPoolSize(); j >= 0; j--) {
        if(IsPlayerConnected(j) && PlayerInfo[j][Login])
        {
            if(PlayerInfo[j][tvid] == playerid && PlayerInfo[j][tvzt]) {
                DestroyPRaceTextDraw(j);
            }
        }
    }
    PlayerInfo[playerid][AFKTimes] = 0; //重置玩家挂机时长 2020-3-15 19:29:11
    Race_HideCp(playerid);
    SetPlayerWeather(playerid, PlayerInfo[playerid][tWeather]); //2020.2.18新增 适配高级赛道函数weather 自动变更回原来的天气
    RaceHouse[hid][rps]--;

    for (new i = 0; i < RaceHouse[hid][rps]; i++) {
        if(RaceHouse[hid][rplayers][i] == playerid) { //找到玩家在的槽位
            RaceHouse[hid][rplayers][i] = INVALID_PLAYER_ID;
            break; //把这个槽位清零
        }
    }
    if(IsPlayerInAnyVehicle(playerid)) {
        SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), 0);
    }
    SetPlayerVirtualWorld(playerid, 0);
    // OnPlayerCommandText(playerid,"/c wode");  有可能会造成玩家的命令频繁？
    if(!PlayerInfo[playerid][NoCrash]) DisableRemoteVehicleCollisions(playerid, false); //如果玩家没开启屏蔽碰撞则取消
    //TogglePlayerControllable(playerid,1);冻结玩家解除
    // RaceHouse[playerid][rtimes]=0; //最新的倒计时BUG好像是因为这个
    SetCameraBehindPlayer(playerid);
    pRaceing[playerid] = 0;
    for (new i = GetPlayerPoolSize(); i >= 0; i--) { //如果这个玩家切换了世界，有人在观看他，那么对应的观看他的人也放到他的世界并观看他
        if(IsPlayerConnected(i)) {
            if(PlayerInfo[i][tvid] == playerid && i != playerid) {
                SetPlayerVirtualWorld(i, GetPlayerVirtualWorld(playerid));
                SetPlayerInterior(i, GetPlayerInterior(playerid));
                if(IsPlayerInAnyVehicle(playerid)) PlayerSpectateVehicle(i, GetPlayerVehicleID(playerid));
                else PlayerSpectatePlayer(i, playerid);
                // TextDrawShowForPlayer(i, CpTextDraw[PlayerInfo[i][id]][a]);
                // TextDrawShowForPlayer(i, Time[PlayerInfo[i][id]][a]);
                // TextDrawShowForPlayer(i, Top[PlayerInfo[i][id]][a]);
                SendClientMessage(i, Color_Orange, "[TV]:对方未处于赛道世界.");

            }
        }
    }
    if(RaceHouse[hid][rps] == 0) //当玩家退出时检测这个房间的人数是否为0，如果为0则杀死排名计算和倒计时计算
    {
        // RaceHouse[playerid][rtimes] = 0; //以前没的 2020.2.9拿下来了 不然倒计时有BUG
        // KillTimer(RaceHouse[hid][rplayerrank]);
        // KillTimer(RaceHouse[hid][rendcount]);
        Race_Game_Delete(hid);
        return 1;
    }
    // 	for (new i = GetPlayerPoolSize(); i >= 0; i--) {
    // 		if(IsPlayerConnected(i)) {
    // 			if(PlayerInfo[i][tvid] != i)  //如果房间没人了 他还在观战则弹窗是否需要继续观看
    // 			{
    // 				ShowPlayerDialog(i, RACE_MSGBOX, DIALOG_STYLE_MSGBOX, "房间", "对方当前未在赛道中，是否继续观看？", "Yes", "No");
    // 			}
    // 		}
    // 	}
    // 	//完成比赛后选择弹出dialog选择是否继续观战
    // }
    // else {
    // 	for (new i = GetPlayerPoolSize(); i >= 0; i--) {
    // 		if(IsPlayerConnected(i) && GameRace[i][rgameid] == hid) { //如果有某个玩家的房间ID等于这个玩家刚退出来的ID则自动观看那个玩家
    // 			new msg[16];
    // 			format(msg, 16, "/tv %i", i);
    // 			AntiCommand[i] = 0;
    //             OnPlayerCommandText(playerid, msg);
    // 		}
    // 	}
    // }
    if(playerid == RaceHouse[hid][rplayerid] && RaceHouse[hid][rstate] == 0) {
        Race_Game_Msg(hid, "[赛车] 赛车发起者离开了房间,因此房间将被关闭");
        Race_Game_Delete(hid);
        // KillTimer(jishu[playerid]); 上面写了理论不用写
        // TextDrawDestroy(Time[playerid]);
        // TextDrawDestroy(Top[playerid]);
        return 1;
    }
    return 1;
}

stock Race_Game_Join(const playerid, const houseid) //使指定玩家加入指定房间.失败返回-1
{
    if(RaceHouse[houseid][rps] == RACE_HOUSE_MAXPLAYER) return -1; //是否满人
    if(RaceHouse[houseid][rraceid] == -1) return -1;
    if(RaceHouse[houseid][rstate] != 0) return -1;
    for (new i = 0; i < RACE_HOUSE_MAXPLAYER; i++) { //也是循环所有槽位
        if(RaceHouse[houseid][rplayers][i] == INVALID_PLAYER_ID) { //证明这个槽位是空的
            RaceHouse[houseid][rplayers][i] = playerid; //吧这个槽位赋值成这个玩家的id
            break; //结束循环
        }
    }

    DisableRemoteVehicleCollisions(playerid, 1); //开启无碰撞
    new msg[128];
    format(msg, sizeof(msg), "[赛车] %s 进入了房间!.", GetName(playerid));
    Race_Game_Msg(houseid, msg);
    SendClientMessage(playerid, Color_Race, "[赛车] 比赛中翻车、失误，下车或/kill可自动重生√");
    RaceHouse[houseid][rps]++;
    GameRace[playerid][rgameid] = houseid;
    GameRace[playerid][rtime] = 0;
    GameRace[playerid][rgamecp] = 1;
    new trcp[racecptype];
    Race_GetCp(RaceHouse[houseid][rraceid], GameRace[playerid][rgamecp], trcp);
    PlayerInfo[playerid][lastXMoved] = trcp[rcpx]; //适配反作弊
    PlayerInfo[playerid][lastYMoved] = trcp[rcpy];
    PlayerInfo[playerid][lastZMoved] = trcp[rcpz];
    pRaceing[playerid] = 1;
    SetPlayerPos(playerid, trcp[rcpx], trcp[rcpy], trcp[rcpz]);
    SetPlayerVirtualWorld(playerid, 6666 - houseid);
    SetPlayerFacingAngle(playerid, trcp[rcpa]);
    //PlayerInfo[playerid][pVehicleEntered] = GetPlayerVehicleID(playerid);
    if(PlayerInfo[playerid][BuyID] != 0) //如果玩家有车的话重生车
    {
        LinkVehicleToInterior(PlayerInfo[playerid][BuyID], GetPlayerInterior(playerid));
        SetVehiclePos(PlayerInfo[playerid][BuyID], trcp[rcpx], trcp[rcpy], trcp[rcpz]);
        SetVehicleZAngle(PlayerInfo[playerid][BuyID], trcp[rcpa]);
        SetVehicleVirtualWorld(PlayerInfo[playerid][BuyID], 6666 - houseid);
        PutPlayerInVehicle(playerid, PlayerInfo[playerid][BuyID], 0);
    }
    if(trcp[rcpz] >= 150.0) DynUpdateStart(playerid); //如果赛道的第一个点大于150的Z轴高度就用5F那个加载一下OBJ
    // SetTimerEx("CarCheck", 500, false, "i", playerid);
    // CpTextDraw[playerid] = CreatePlayerTextDraw(playerid,500, 118.000000, "~~");
    // PlayerTextDrawBackgroundColor(playerid, CpTextDraw[playerid], 255);
    // PlayerTextDrawFont(playerid, CpTextDraw[playerid], 2);
    // PlayerTextDrawLetterSize(playerid, CpTextDraw[playerid], 0.238000, 1.190000);
    // PlayerTextDrawColor(playerid, CpTextDraw[playerid], -1);
    // PlayerTextDrawSetOutline(playerid, CpTextDraw[playerid], 0);
    // PlayerTextDrawSetProportional(playerid, CpTextDraw[playerid], 1);
    // PlayerTextDrawSetShadow(playerid, CpTextDraw[playerid], 1);
    // /*TextDrawUseBox(playerid, CpTextDraw[playerid], 1);
    // PlayerTextDrawBoxColor(playerid, CpTextDraw[playerid], 50);*/
    // PlayerTextDrawTextSize(playerid, CpTextDraw[playerid], 626.000000, 391.000000);

    CreatePRaceTextDraw(playerid);
    format(msg, sizeof(msg), "C  P / ~p~0~w~/~y~%i", Race[RaceHouse[houseid][rraceid]][rcps]);
    PlayerTextDrawSetString(playerid, CpTextDraw[playerid], msg);
    // PlayerTextDrawShow(playerid, CpTextDraw[playerid]);


    // Time[playerid] = CreatePlayerTextDraw(playerid,500, 136.000000, "比赛时间 / 00:00:00");
    // PlayerTextDrawBackgroundColor(playerid, Time[playerid], 255);
    // PlayerTextDrawFont(playerid, Time[playerid], 2);
    // PlayerTextDrawLetterSize(playerid, Time[playerid], 0.238000, 1.190000);
    // PlayerTextDrawColor(playerid, Time[playerid], -1);
    // PlayerTextDrawSetOutline(playerid, Time[playerid], 0);
    // PlayerTextDrawSetProportional(playerid, Time[playerid], 1);
    // PlayerTextDrawSetShadow(playerid, Time[playerid], 1);
    // /*TextDrawUseBox(playerid, Time[playerid], 1);
    // TextDrawBoxColor(playerid, Time[playerid], 50);*/
    // PlayerTextDrawTextSize(playerid, Time[playerid], 626.000000, 10.000000);
    // PlayerTextDrawShow(playerid, Time[playerid]);

    // p_record[playerid] = CreatePlayerTextDraw(playerid, 500, 154.000000, "个人记录 / 00:00:00");
    // PlayerTextDrawBackgroundColor(playerid, p_record[playerid], 255);
    // PlayerTextDrawFont(playerid, p_record[playerid], 2);
    // PlayerTextDrawLetterSize(playerid, p_record[playerid], 0.238000, 1.190000);
    // PlayerTextDrawColor(playerid, p_record[playerid], -1);
    // PlayerTextDrawSetOutline(playerid, p_record[playerid], 0);
    // PlayerTextDrawSetProportional(playerid, p_record[playerid], 1);
    // PlayerTextDrawSetShadow(playerid, p_record[playerid], 1);
    // PlayerTextDrawTextSize(playerid, p_record[playerid], 626.000000, 10.000000);
    // PlayerTextDrawShow(playerid, p_record[playerid]);


    // ///
    // Top[playerid] = CreatePlayerTextDraw(playerid, 500, 172.000000, "房间排名 / 1 st");
    // // Top[playerid] = CreatePlayerTextDraw(playerid,500, 154.000000, "房间排名 / 1 st");
    // PlayerTextDrawBackgroundColor(playerid, Top[playerid], 255);
    // PlayerTextDrawFont(playerid, Top[playerid], 2);
    // PlayerTextDrawLetterSize(playerid, Top[playerid], 0.238000, 1.190000);
    // PlayerTextDrawColor(playerid, Top[playerid], -1);
    // PlayerTextDrawSetOutline(playerid, Top[playerid], 0);
    // PlayerTextDrawSetProportional(playerid, Top[playerid], 1);
    // PlayerTextDrawSetShadow(playerid, Top[playerid], 1);
    // /*TextDrawUseBox(playerid, Top[playerid], 1);
    // TextDrawBoxColor(playerid, Top[playerid], 50);*/
    // PlayerTextDrawTextSize(playerid, Top[playerid], 626.000000, 10.000000);
    // PlayerTextDrawShow(playerid, Top[playerid]);

    // ReSpawningText[playerid] = TextDrawCreate(307.333374, 127.362937, "重生中...");
    // TextDrawLetterSize(ReSpawningText[playerid], 0.375666, 1.512889);
    // TextDrawTextSize(ReSpawningText[playerid], 18.000000, 187.000000);
    // TextDrawAlignment(ReSpawningText[playerid], 2);
    // TextDrawColor(ReSpawningText[playerid], 6736383);
    // TextDrawUseBox(ReSpawningText[playerid], 1);
    // TextDrawBoxColor(ReSpawningText[playerid], 73);
    // TextDrawSetShadow(ReSpawningText[playerid], 3);
    // TextDrawSetOutline(ReSpawningText[playerid], 1);
    // TextDrawBackgroundColor(ReSpawningText[playerid], 255);
    // TextDrawFont(ReSpawningText[playerid], 0);
    // TextDrawSetProportional(ReSpawningText[playerid], 1);
    // TextDrawSetShadow(ReSpawningText[playerid], 3);
    Race_ShowCp(playerid, RaceHouse[houseid][rraceid], GameRace[playerid][rgamecp]);
    new p_record_time = Race_GetPlayerRecord(playerid,Race[RaceHouse[houseid][rraceid]][rid]);
    if(p_record_time == -1) 
    {
        PlayerTextDrawSetString(playerid, p_record[playerid], "BEST / 99:99:99");
    }
    else
    {
        new time[3];
        ms2time(time, p_record_time);
        format(msg, sizeof msg, "BEST / %02d:%02d:%02d", time[0], time[1], time[2]/10);
        PlayerTextDrawSetString(playerid, p_record[playerid], msg);
    }
    for (new i = GetPlayerPoolSize(); i >= 0; i--) { //如果这个玩家进入了赛道，有人在观看他，那么对应的观看他的人也放到他的世界并观看他
        if(IsPlayerConnected(i)) {
            if(PlayerInfo[i][tvid] == playerid && i != playerid) {
                SetPlayerVirtualWorld(i, GetPlayerVirtualWorld(playerid));
                SetPlayerInterior(i, GetPlayerInterior(playerid));
                if(IsPlayerInAnyVehicle(playerid)) PlayerSpectateVehicle(i, GetPlayerVehicleID(playerid));
                else PlayerSpectatePlayer(i, playerid);
                // TextDrawHideForPlayer(i, CpTextDraw[PlayerInfo[i][tvid]][a]);
                // TextDrawHideForPlayer(i, Time[PlayerInfo[i][tvid]][a]);
                // TextDrawHideForPlayer(i, Top[PlayerInfo[i][tvid]][a]);
                PlayerTextDrawShow(i, CpTextDraw[PlayerInfo[i][tvid]]);
                PlayerTextDrawShow(i, Time[PlayerInfo[i][tvid]]);
                PlayerTextDrawShow(i, Top[PlayerInfo[i][tvid]]);
                PlayerTextDrawShow(i, p_record[PlayerInfo[i][tvid]]);
                SendClientMessage(i, Color_Orange, "[TV]:对方处于赛道世界，自动追踪观看.");
            }
        }
    }
    return 1;
}

stock Race_Game_DeleteByRid(const raceid) //关闭指定赛道的比赛房间
{
    for (new i = 0; i < RACE_GAME_MAX; i++) {
        if(RaceHouse[i][rraceid] == raceid) {
            Race_Game_Msg(i, "[赛车]赛道被编辑,因此房间将被关闭");
            Race_Game_Delete(i);
        }
    }
}

stock Race_Game_Delete(const houseid) //关闭比赛房间
{
    //	printf("关闭房间:%i",houseid);
    // for (new i = GetPlayerPoolSize(); i >= 0; i--) {
    //     if(IsPlayerConnected(i) == 1) {
    //         if(GameRace[i][rgameid] == houseid) {
    //             Race_Game_Quit(i);
    //         }
    //     }
    // }
    for (new i = 0; i < RACE_HOUSE_MAXPLAYER; i++) { //2020.8.2改 优化算法 实时根据人数去算
        new player = RaceHouse[houseid][rplayers][i];
        if(player == INVALID_PLAYER_ID) continue;
        if(GameRace[player][rgameid] == houseid) Race_Game_Quit(player);
    }
    RaceHouse[houseid][rraceid] = -1;
    RaceHouse[houseid][rplayerid] = -1;
    // RaceHouse[houseid][rtimes] = 0; //理论上这句话不用写的
    // 判断比赛倒计时计时器是否运行 如果运行则自动删除重置
    if(RaceHouse[houseid][rtimer] != -1) {
        //printf("Delete:Killtimer");
        KillTimer(RaceHouse[houseid][rtimer]);
        RaceHouse[houseid][rtimer] = -1;
    }
    // 判断比赛排名计时器是否运行 如果运行则自动删除重置
    if(RaceHouse[houseid][rplayerrank] != -1) {
        KillTimer(RaceHouse[houseid][rplayerrank]);
        RaceHouse[houseid][rplayerrank] = -1;
    }
    if(RaceHouse[houseid][rendcount] != -1) {
        KillTimer(RaceHouse[houseid][rendcount]);
        RaceHouse[houseid][rendcount] = -1;
    }
    if(RaceHouse[houseid][rRaceRunTime] != -1) {
        KillTimer(RaceHouse[houseid][rRaceRunTime]);
        RaceHouse[houseid][rRaceRunTime] = -1;
    }
    
    
}

stock Race_Game_Create(const playerid, const raceid) //创建比赛房间，赛道为raceid,失败返回-1,成功返回房间ID
{
    new id = -1;
    for (new i = 0; i < RACE_GAME_MAX; i++) {
        if(RaceHouse[i][rraceid] == -1) {
            id = i;
            i = RACE_GAME_MAX + 1;
        }
    }
    if(id == -1) return id;
    //新建比赛的时候,先把这个房间的players数组清零 排名初始化
    for (new i = 0; i < RACE_HOUSE_MAXPLAYER; i++) {
        RaceHouse[id][rplayers][i] = INVALID_PLAYER_ID;
    }
    RaceHouse[id][rraceid] = raceid;
    RaceHouse[id][rtop] = 1;
    RaceHouse[id][rstate] = 0;
    RaceHouse[id][rps] = 0;
    RaceHouse[id][rtimer] = -1;
    RaceHouse[id][rplayerrank] = -1;
    RaceHouse[id][rendcount] = -1;
    RaceHouse[id][rRaceRunTime] = -1;
    RaceHouse[id][rtimes] = 5;
    RaceHouse[id][rplayerid] = playerid;
    // printf("[赛车]创建房间:%i", id);
    return id;
}

// 为了全局调用，放在main头文件里了
// stock GetMaxPage(const s) //给定总数,求出页数
// {
//     new maxpage;
//     maxpage = s % RACE_PAGES;
//     if(maxpage == 0) {
//         maxpage = s / RACE_PAGES;
//     } else {
//         maxpage = (s / RACE_PAGES) + 1;
//     }
//     return maxpage;
// }

stock Race_ShowTrgFunDialog(const playerid, const fun[]) //为指定玩家显示触发函数说明
{
    new id = -1;
    for (new i = 0; i < sizeof(cpscript); i++) {
        if(strcmp(cpscript[i][cpsname], fun) == 0) {
            id = i;
            i = sizeof(cpscript) + 1;
        }
    }
    Dialog_Show(playerid, RACE_EDIT_CP_SCRIPT_HMSG, DIALOG_STYLE_MSGBOX, "触发说明", cpscript[id][cpsmiaoshu], "关闭", "");
}

stock Race_ShowTrgRetDialog(const playerid, const fun[]) //为指定玩家显示触发转义符说明
{
    new id = -1;
    for (new i = 0; i < sizeof(cpscriptr); i++) {
        if(strcmp(cpscriptr[i][cpsrname], fun) == 0) {
            id = i;
            i = sizeof(cpscriptr) + 1;
        }
    }
    Dialog_Show(playerid, RACE_EDIT_CP_SCRIPT_HMSG, DIALOG_STYLE_MSGBOX, "触发说明", cpscriptr[id][cpsrmiaoshu], "关闭", "");
}

stock Race_ShowTrgDialog(const playerid, const type[]) //为指定玩家显示触发说明
{
    new msg[1024];
    if(strcmp(type, "main") == 0) {
        Dialog_Show(playerid, RACE_EDIT_CP_SCRIPT_H, DIALOG_STYLE_LIST, "触发说明", "查看语法\n查看函数\n查看转义符", "选择", "关闭");
    }
    if(strcmp(type, "语法") == 0) {
        Dialog_Show(playerid, RACE_EDIT_CP_SCRIPT_H, DIALOG_STYLE_MSGBOX, "触发说明", "所有触发的用法都是\n函数 参数 参数1 参数2 参数3 参数4 参数......\n函数、参数之间利用空格分割\n参数可以是转义符或任意字符,根据函数不同执行效果不同\n{00FF00}注意:{FF0000}不{FFFFFF}支持数字计算,比如speed 2+5,不会设置为7!\n需要进行计算的函数会有参数决定运算方式,请查看对应函数的描述\n{00FF00}注意2:{FFFFFF}所有触发只有{00FF00}主驾驶{FFFFFF}的玩家可以触发", "关闭", "");
    }
    if(strcmp(type, "函数") == 0) {
        for (new i = 0; i < sizeof(cpscript); i++) {
            format(msg, sizeof(msg), "%s\n函数> %s", msg, cpscript[i][cpsname]);
        }
        Dialog_Show(playerid, RACE_EDIT_CP_SCRIPT_H, DIALOG_STYLE_LIST, "触发说明", msg, "选择", "关闭");
    }
    if(strcmp(type, "转义符") == 0) {
        for (new i = 0; i < sizeof(cpscriptr); i++) {
            format(msg, sizeof(msg), "%s\n转义符> %s", msg, cpscriptr[i][cpsrname]);
        }
        Dialog_Show(playerid, RACE_EDIT_CP_SCRIPT_H, DIALOG_STYLE_LIST, "触发说明", msg, "选择", "关闭");
    }
}

stock Race_ShowGameSelDialog(const playerid, const page) //为指定玩家显示赛道选择菜单,page表示赛道页数
{
    new title[128], msg[2048], maxpage, ps = -1;
    format(title, sizeof(title), "{B0E2FF}Prace高级赛道系统 by ushio_p 修改:Hygen");
    if(NowRaceId != 0) {
        if(NowRaceId > RACE_PAGES) {
            maxpage = GetMaxPage(NowRaceId, RACE_PAGES);
            if(page >= maxpage) //原来这里是==的
            {
                format(msg, sizeof(msg), "%s\n- 第 %i 页 - 共 %i 页 -", msg, page, maxpage);
            } else {
                format(msg, sizeof(msg), "%s\n- 第 %i 页 - 共 %i 页 -\n{FFFF00}下一页", msg, page, maxpage);
            }
            if(page != 1) {
                ps = 1;
            }
        }
        for (new i = (page - 1) * RACE_PAGES; i < page * RACE_PAGES; i++) {
            if(i < NowRaceId && Race[i][rid] != -1) {
                format(msg, sizeof(msg), "%s\n{9AFF9A}赛道 %s {FFFFFF}By:{9AFF9A}%s", msg, Race[i][rname], Race[i][rauthorName]);
            }
        }
        if(ps == 1) {
            format(msg, sizeof(msg), "%s\n{FFFF00}上一页", msg);
        }
    }
    Dialog_Show(playerid, RACE_GAME_MAIN_SEL, DIALOG_STYLE_LIST, title, msg, "选择", "关闭");
}


stock Race_ShowGameDialog(const playerid, const id) //为指定玩家显示指定赛道的详细属性
{
    // 2020.7.14新增显示赛道创建日期

    new Query[256];
    mysql_format(g_Sql, Query, sizeof(Query), "SELECT `race_record`.*, `users`.`Name` FROM `race_record` JOIN `users` ON  \
    `race_record`.`uid` = `users`.`ID`\
    WHERE `rid` = %d ORDER BY `p_record` ASC LIMIT 10", Race[id][rid]);
    mysql_pquery(g_Sql, Query, "RaceShowQuery", "dd", playerid, id);
    // ShowPlayerDialog(playerid, RACE_GAME_MAIN_DIALOG, DIALOG_STYLE_LIST, title, msg, "选择", "关闭");
}
// 2020-08-02 22:37:40 修改个人记录算法
function RaceShowQuery(playerid, id) {
    new year, month, day, hour, minute, second;
    TimestampToDate(Race[id][rcreatedate], year, month, day, hour, minute, second, 8);

    new title[128], msg[1024];
    format(title, sizeof(title), "{FFFF00}赛道 - %s", Race[id][rname]);
    format(msg, sizeof(msg), "赛道作者:%s\n创建日期:%d-%d-%d %02d:%02d:%02d\n赛道描述:\n%s\n赛道CP数量:%i\n \n{00FF00}开始 %s Start Race!", \
    Race[id][rauthorName], year, month, day, hour, minute, second, Race[id][rmiaoshu], Race[id][rcps], Race[id][rname]);

    new cut = cache_num_rows();
    if(cut)
    {
        for (new i = 0; i < cut; i++) {
            new recordtime;
            new time[3];
            new pName_temp[MAX_PLAYER_NAME];
            cache_get_value_name(i, "Name", pName_temp);
            cache_get_value_name_int(i, "p_record", recordtime);

            ms2time(time, recordtime);

            format(msg, sizeof(msg), "%s\n#第%i名:%s 时间:%d:%d:%d\t[%s秒] ", msg, i + 1, pName_temp, \
            time[0], time[1], time[2], MsToS(recordtime));
        }
    }
    Dialog_Show(playerid, RACE_GAME_MAIN_DIALOG, DIALOG_STYLE_LIST, title, msg, "选择", "关闭");
    return 1;
}
stock Race_ShowEditDialog(const playerid, const id, const page) //为指定玩家显示指定赛道的详细属性,page表示检查点页数
{
    new title[128], msg[2048], maxpage, ps = -1;
    format(title, sizeof(title), "赛道编辑 - %s", Race[id][rname]);
    format(msg, sizeof(msg), "赛道作者:%s\n赛道描述 < 点击这行来修改描述\n%s\n赛道CP数量:%i\n测试该赛道\n清空赛道记录\n{00FF00}删除该赛道{FFFFFF}\n修改所有检查点尺寸为 %f", Race[id][rauthorName], Race[id][rmiaoshu], Race[id][rcps], EditRace[playerid][rcpsize]);
    if(Race[id][rcps] != 0) {
        if(Race[id][rcps] > RACE_PAGES) {
            maxpage = GetMaxPage(Race[id][rcps], RACE_PAGES);
            if(page == maxpage) {
                format(msg, sizeof(msg), "%s\n- 第 %i 页 - 共 %i 页 -", msg, page, maxpage);
            } else {
                format(msg, sizeof(msg), "%s\n- 第 %i 页 - 共 %i 页 -\n下一页", msg, page, maxpage);
            }
            if(page != 1) {
                ps = 1;
            }
        } else {
            format(msg, sizeof(msg), "%s\n-", msg);
        }
        for (new i = (page - 1) * RACE_PAGES; i < page * RACE_PAGES; i++) {
            if(i < Race[id][rcps]) {
                format(msg, sizeof(msg), "%s\n检查点 %i", msg, i + 1);
            }
        }
        if(ps == 1) {
            format(msg, sizeof(msg), "%s\n上一页", msg);
        }
    }
    Dialog_Show(playerid, RACE_EDIT_MAIN, DIALOG_STYLE_LIST, title, msg, "选择", "关闭");
}

stock Race_ShowEditMiaoshuDialog(const playerid, const id, const tip[]) //为指定玩家显示指定赛道的描述修改界面
{
    new msg[768], title[128];
    format(title, sizeof(title), "赛道编辑 - %s - 修改描述", Race[id][rname]);
    format(msg, sizeof(msg), "当前赛道[%s]的描述\n{FF0000}%s{FFFFFF}\n请输入要修改的描述~最多64个字符(中文1个算2个)%s", Race[id][rname], Race[id][rmiaoshu], tip);
    Dialog_Show(playerid, RACE_EDIT_MIAOSHU, DIALOG_STYLE_INPUT, title, msg, "修改", "取消");
}

stock Race_ShowEditCpScriptDialog(const playerid, const id, cpid, const tip[]) //为指定玩家显示指定赛道的指定CP点的触发修改界面
{
    new msg[768], title[128], trigger[RACE_CP_MAX_FUNCTION], trcp[racecptype];
    format(title, sizeof(title), "赛道编辑 - %s - 检查点 %i - 触发编辑", Race[id][rname], cpid);
    if(EditRace[playerid][rdialogcplist] == -1) {
        format(msg, sizeof(msg), "----------------------------------------------------为了显示标题而存在的一行----------------------------------------------------\n为赛道[%s]的第[%i]个检查点新建触发\n请输入触发~最多%i个字符(中文1个算2个)%s", Race[id][rname], cpid, RACE_CP_MAX_FUNCTION, tip);
    } else {
        Race_GetCp(id, cpid, trcp);
        Race_GetCp_Script(trcp[rcpid], EditRace[playerid][rdialogcplist], trigger);
        format(msg, sizeof(msg), "----------------------------------------------------为了显示标题而存在的一行----------------------------------------------------\n当前赛道[%s]第[%i]个检查点的第[%i]条触发\n{FF0000}%s{FFFFFF}\n请输入要修改的触发~最多%i个字符(中文1个算2个)\n输入'插入'在这一行之上添加新触发\n输入'删除'来删除这一条触发%s", Race[id][rname], cpid, EditRace[playerid][rdialogcplist], trigger, RACE_CP_MAX_FUNCTION, tip);
    }
    Dialog_Show(playerid, RACE_EDIT_CP_SCRIPT, DIALOG_STYLE_INPUT, title, msg, "确认", "取消");
}

stock Race_ShowEditCpDialog(const playerid, const id, cpid, const page) //为玩家显示指定赛道的指定cp点信息
{
    new title[128], msg[1024], trcp[racecptype], ts, maxpage, p = -1, fc[RACE_CP_MAX_FUNCTION];
    format(title, sizeof(title), "赛道编辑 - %s - 检查点 %i", Race[id][rname], cpid);
    Race_GetCp(id, cpid, trcp);
    format(msg, sizeof(msg), "x %f|y %f|z %f|a %f\nsize %f < 点击这行将尺寸修改为%f\n{00FF00}删除检查点{FFFFFF}\n插入检查点\n移动检查点\n传送到检查点\n从这检查点开始测试赛道\n添加新触发\n删除所有触发", trcp[rcpx], trcp[rcpy], trcp[rcpz], trcp[rcpa], trcp[rcppsize], EditRace[playerid][rcpsize]);
    ts = Race_GetCp_Scripts(trcp[rcpid]);
    if(ts != 0) {
        if(ts > RACE_PAGES) {
            maxpage = GetMaxPage(ts, RACE_PAGES);
            if(maxpage == page) {
                format(msg, sizeof(msg), "%s\n- 第 %i 页 - 共 %i 页 -", msg, page, maxpage);
            } else {
                format(msg, sizeof(msg), "%s\n- 第 %i 页 - 共 %i 页 -\n下一页", msg, page, maxpage);
            }
            if(page != 1) {
                p = 1;
            }
        } else {
            format(msg, sizeof(msg), "%s\n-", msg);
        }
        for (new i = RACE_PAGES * (page - 1); i < RACE_PAGES * page; i++) {
            if(i < ts) {
                Race_GetCp_Script(trcp[rcpid], i, fc);
                format(msg, sizeof(msg), "%s\n触发 %i %s", msg, i, fc);
            }
        }
        if(p == 1) {
            format(msg, sizeof(msg), "%s\n上一页", msg);
        }
    }
    Dialog_Show(playerid, RACE_EDIT_CP, DIALOG_STYLE_LIST, title, msg, "选择", "关闭");
}



stock Race_Test(const playerid, const raceid, cpid) //让指定玩家从指定cp点开始测试赛道
{
    if(Race[raceid][rcps] == 0) return -1;
    new trcp[racecptype];
    Race_GetCp(raceid, cpid, trcp);
    SetPlayerPosEx(playerid, trcp[rcpx], trcp[rcpy], trcp[rcpz]);
    SetPlayerFacingAngleEx(playerid, trcp[rcpa]);
    EditRace[playerid][rtestcp] = cpid;
    Race_ShowCp(playerid, raceid, cpid);
    return 1;
}
stock Race_ShowCp(const playerid, const id, cpid) //为指定玩家显示指定赛道的指定cp点
{
    new trcp[racecptype], nrcp[racecptype];
    Race_GetCp(id, cpid, trcp);
    if(cpid != Race[id][rcps]) {
        Race_GetCp(id, cpid + 1, nrcp);
        SetPlayerRaceCheckpoint(playerid, 0, trcp[rcpx], trcp[rcpy], trcp[rcpz], nrcp[rcpx], nrcp[rcpy], nrcp[rcpz], trcp[rcppsize]);
        SetPlayerMapIcon(playerid, RACE_MAP_ICON_INDEX, nrcp[rcpx], nrcp[rcpy], nrcp[rcpz], RACE_MAP_ICON_TYPE, 0, 1);
        for (new i = GetPlayerPoolSize(); i >= 0; i--) { //如果有观战的也显示他观战的地图上的CP检查点
            if(IsPlayerConnected(i) && PlayerInfo[i][tvid] == playerid && i != playerid) SetPlayerRaceCheckpoint(i, 0, trcp[rcpx], trcp[rcpy], trcp[rcpz], nrcp[rcpx], nrcp[rcpy], nrcp[rcpz], trcp[rcppsize]);
        }
    } else {
        SetPlayerRaceCheckpoint(playerid, 1, trcp[rcpx], trcp[rcpy], trcp[rcpz], 0, 0, 0, trcp[rcppsize]);
        for (new i = GetPlayerPoolSize(); i >= 0; i--) {
            if(IsPlayerConnected(i) && PlayerInfo[i][tvid] == playerid && i != playerid) SetPlayerRaceCheckpoint(i, 1, trcp[rcpx], trcp[rcpy], trcp[rcpz], 0, 0, 0, trcp[rcppsize]);
        }
    }
}
stock Race_HideCp(const playerid) //为指定玩家关闭cp点和小地图点
{
    for (new i = GetPlayerPoolSize(); i >= 0; i--) { //如果有观战的也关闭CP点
        if(IsPlayerConnected(i) && PlayerInfo[i][tvid] == playerid && i != playerid) {
            DisablePlayerRaceCheckpoint(i); //2020.2.9 大概是因为这个 观战没去掉CP点
            RemovePlayerMapIcon(i, RACE_MAP_ICON_INDEX);
        }
    }
    DisablePlayerRaceCheckpoint(playerid);
    RemovePlayerMapIcon(playerid, RACE_MAP_ICON_INDEX);
}


stock Race_SetMiaoshu(const id, const miaoshu[]) //设置指定赛道的描述
{
    new msg[512]; //,ti=GetTickCount();
    format(Race[id][rmiaoshu], 64, "%s", miaoshu);
    mysql_format(g_Sql, msg, sizeof(msg), "UPDATE `race` SET `miaoshu` = '%e' WHERE `rname` = '%e'", Race[id][rmiaoshu], Race[id][rname]);
    mysql_pquery(g_Sql, msg);
    //printf("  >P-Race 设置赛道[%s]的描述为[%s],耗时[%i]ms",Race[id][rname],miaoshu,GetTickCount()-ti);
}

stock Race_NewCp_Script(const raceid, cpid, const script[]) //为指定cp点增加一条触发
{
    new msg[512], tr[racecptype];
    Race_GetCp(raceid, cpid, tr);
    mysql_format(g_Sql, msg, sizeof(msg), "INSERT INTO `race_cp_script` (`rid`,`cpid`,`list`,`script`) VALUES ('%i','%i','%i','%e')", \
    Race[raceid][rid], tr[rcpid], Race_GetCp_Scripts(tr[rcpid]), script);
    mysql_pquery(g_Sql, msg);
    //printf("  >P-Race 新增一条触发于赛道[%s]的第[%i]个CP点,耗时[%i]ms",Race[raceid][rname],cpid,GetTickCount()-ti);
}

stock Race_InsCp_Script(const raceid, cpid, const list, const script[]) //为指定cp点插入一条触发
{
    new msg[512], tr[racecptype];
    mysql_tquery(g_Sql, "START TRANSACTION;"); // Start transaction.
    Race_GetCp(raceid, cpid, tr);
    mysql_format(g_Sql, msg, sizeof(msg), "UPDATE `race_cp_script` SET `list` = `list`+1 WHERE `cpid` = '%i' and `list`>='%i'", tr[rcpid], list);
    mysql_tquery(g_Sql, msg);
    mysql_format(g_Sql, msg, sizeof(msg), "INSERT INTO `race_cp_script` (`rid`,`cpid`,`list`,`script`) VALUES ('%i','%i','%i','%e')", Race[raceid][rid], \
    tr[rcpid], list, script);
    mysql_tquery(g_Sql, msg);
    mysql_tquery(g_Sql, "COMMIT;"); // Commit transaction. 
    //printf("  >P-Race 插入一条触发于赛道[%s]第[%i]个CP点的第[%i]条触发之上",Race[raceid][rname],cpid,list,GetTickCount()-ti);
}

stock Race_SetCp_Script(const raceid, cpid, const list, const script[]) // 修改指定CP点第list条触发
{
    new msg[512], tr[racecptype];
    Race_GetCp(raceid, cpid, tr);
    mysql_format(g_Sql, msg, sizeof(msg), "UPDATE `race_cp_script` SET `script`='%e' WHERE `cpid` = '%i' and `list`='%i' ", \
    script, tr[rcpid], list);
    mysql_pquery(g_Sql, msg);
    //printf("  >P-Race 修改赛道[%s]第[%i]个CP点的第[%i]条触发,耗时[%i]ms",Race[raceid][rname],cpid,list,GetTickCount()-ti);
}

stock Race_GetCp_Script(const cpdb, const list, script[RACE_CP_MAX_FUNCTION]) //返回指定cp点第list条触发
{
    new msg[512], Cache:result; //,ti=GetTickCount();
    mysql_format(g_Sql, msg, sizeof(msg), "SELECT `script` FROM `race_cp_script` WHERE `cpid`='%i' and `list`='%i'", cpdb, list);
    result = mysql_query(g_Sql, msg);
    cache_get_value_index(0, 0, script); //有可能会有问题
    // db_get_field(rf, 0, script, RACE_CP_MAX_FUNCTION);
    cache_delete(result);
    //printf("  >P-Race 读取CP点[%i]的第[%i]条触发,耗时[%i]ms",cpdb,list,GetTickCount()-ti);
    return script;
}

stock Race_DelCp_Script(const raceid, cpid, const list) //删除指定cp点的指定触发
{
    new msg[512], tr[racecptype];
    mysql_tquery(g_Sql, "START TRANSACTION;"); // Start transaction.
    Race_GetCp(raceid, cpid, tr);
    mysql_format(g_Sql, msg, sizeof(msg), "DELETE FROM `race_cp_script` WHERE `cpid` = '%i' and `list`='%i'", tr[rcpid], list);
    mysql_tquery(g_Sql, msg);
    mysql_format(g_Sql, msg, sizeof(msg), "UPDATE `race_cp_script` SET `list` = `list`-1 WHERE `cpid` = '%i' and `list`>'%i'", tr[rcpid], list);
    mysql_tquery(g_Sql, msg);
    mysql_tquery(g_Sql, "COMMIT;"); // Commit transaction. 
    //printf("  >P-Race 删除赛道[%s]的第[%i]个CP点的第[%i]条触发,耗时[%i]ms",Race[raceid][rname],cpid,list,GetTickCount()-ti);
}

stock Race_DelCp_Scripts(const raceid, cpid) //删除指定cp点触发
{
    new msg[512], tr[racecptype];
    Race_GetCp(raceid, cpid, tr);
    mysql_format(g_Sql, msg, sizeof(msg), "DELETE FROM `race_cp_script` WHERE `cpid` = '%i'", tr[rcpid]);
    mysql_pquery(g_Sql, msg);
    //printf("  >P-Race 删除赛道[%s]第[%i]个CP点的触发,耗时[%i]ms",Race[raceid][rname],cpid,GetTickCount()-ti);
}

stock Race_GetCp_Scripts(const cpdb) //返回指定CP点的触发数
{
    new s, msg[512], Cache:result;
    mysql_format(g_Sql, msg, sizeof(msg), "SELECT * FROM `race_cp_script` WHERE `cpid`='%i'", cpdb);
    result = mysql_query(g_Sql, msg);
    s = cache_num_rows();
    cache_delete(result);
    //	printf("  >P-Race 读取赛道[%s]的CP数量,耗时[%i]ms",Race[id][rname],GetTickCount()-ti);
    return s;
}

stock Race_NewCp(const id, const playerid) //为指定赛道增加一个CP点，位置为playerid所在的位置
{
    new msg[512], Float:px, Float:py, Float:pz, Float:pa; //,ti=GetTickCount();
    GetPlayerPosEx(playerid, px, py, pz);
    GetPlayerFacingAngleEx(playerid, pa);
    mysql_format(g_Sql, msg, sizeof(msg), "INSERT INTO `race_cp` (`rid`,`cp`,`x`,`y`,`z`,`a`,`size`) VALUES ('%i','%i','%e','%e','%e','%e','%e')", \
    Race[id][rid], Race[id][rcps], F2S(px), F2S(py), F2S(pz), F2S(pa), F2S(EditRace[playerid][rcpsize]));
    mysql_pquery(g_Sql, msg);
    Race[id][rcps]++;
    return 1;
    //printf("  >P-Race 新增一个CP点于赛道[%s],耗时[%i]ms",Race[id][rname],GetTickCount()-ti);
}

stock Race_InsCp(const id, const playerid, cpid) //为指定赛道插入一个CP点，位置为playerid所在的位置
{
    cpid--;
    new msg[512], Float:px, Float:py, Float:pz, Float:pa; //,ti=GetTickCount();
    GetPlayerPosEx(playerid, px, py, pz);
    GetPlayerFacingAngleEx(playerid, pa);
    mysql_format(g_Sql, msg, sizeof(msg), "UPDATE `race_cp` SET `cp` = `cp`+1 WHERE `rid` = '%i' and `cp`>='%i'", Race[id][rid], cpid);
    mysql_tquery(g_Sql, msg);
    mysql_format(g_Sql, msg, sizeof(msg), "INSERT INTO `race_cp` (`rid`,`cp`,`x`,`y`,`z`,`a`,`size`) VALUES ('%i','%i','%e','%e','%e','%e','%e')", \
    Race[id][rid], cpid, F2S(px), F2S(py), F2S(pz), F2S(pa), F2S(EditRace[playerid][rcpsize]));
    mysql_tquery(g_Sql, msg);
    mysql_tquery(g_Sql, "COMMIT;");
    Race[id][rcps]++;
    //	printf("  >P-Race 在第[%i]个CP点插入一个CP点于赛道[%s],耗时[%i]ms",cpid+1,Race[id][rname],GetTickCount()-ti);
}

stock Race_SetCpPos(const playerid, const id, cpid) // 修改指定赛道的指定CP点位置
{
    cpid--;
    new msg[128], Float:x, Float:y, Float:z, Float:a; //,ti=GetTickCount();
    GetPlayerPosEx(playerid, x, y, z);
    GetPlayerFacingAngleEx(playerid, a);
    mysql_format(g_Sql, msg, sizeof(msg), "UPDATE `race_cp` SET `x` = '%f',`y` = '%f',`z` = '%f',`a`='%f' WHERE `rid` = '%i' and `cp`='%i' ", \
    x, y, z, a, Race[id][rid], cpid);
    mysql_pquery(g_Sql, msg);
    //	printf("  >P-Race 修改赛道[%s]的第[%i]个CP点位置,耗时[%i]ms",Race[id][rname],cpid+1,GetTickCount()-ti);
}

stock Race_SetAllCpSize(const playerid, const id) // 修改指定赛道的所有CP点尺寸
{
    new msg[128]; //,ti=GetTickCount();
    mysql_format(g_Sql, msg, sizeof(msg), "UPDATE `race_cp` SET `size`='%f' WHERE `rid` = '%i'", EditRace[playerid][rcpsize], Race[id][rid]);
    mysql_pquery(g_Sql, msg);
    //	printf("  >P-Race 修改赛道[%s]的所有CP点尺寸,耗时[%i]ms",Race[id][rname],GetTickCount()-ti);
}

stock Race_SetCpSize(const playerid, const id, cpid) // 修改指定赛道的指定CP点尺寸
{
    cpid--;
    new msg[128]; //,ti=GetTickCount();
    mysql_format(g_Sql, msg, sizeof(msg), "UPDATE `race_cp` SET `size`='%f' WHERE `rid` = '%i' and `cp`='%i' ",\
    EditRace[playerid][rcpsize], Race[id][rid], cpid);
    mysql_pquery(g_Sql, msg);
    //	printf("  >P-Race 修改赛道[%s]的第[%i]个CP点尺寸,耗时[%i]ms",Race[id][rname],cpid+1,GetTickCount()-ti);
}

stock Race_DelCp(const id, cpid) //删除指定赛道的cp点
{
    new rf[racecptype], msg[512]; //,ti=GetTickCount();
    mysql_tquery(g_Sql, "START TRANSACTION;");
    Race_GetCp(id, cpid, rf);
    Race_DelCp_Scripts(id, cpid);
    cpid--;
    format(msg, sizeof(msg), "DELETE FROM `race_cp` WHERE `rid` = '%i' and `cp`='%i';", Race[id][rid], cpid);
    mysql_tquery(g_Sql, msg);

    format(msg, sizeof(msg), "UPDATE `race_cp` SET `cp` = `cp`-1 WHERE `rid` = '%i' and `cp`>'%i';", Race[id][rid], cpid);
    mysql_tquery(g_Sql, msg);
    
    mysql_tquery(g_Sql, "COMMIT;");
    // Race[id][rcps] = Race_GetCps(id);
    Race_GetCps(id);
    //	printf("  >P-Race 删除赛道[%s]的第[%i]个CP点,耗时[%i]ms",Race[id][rname],cpid+1,GetTickCount()-ti);

}

stock Race_GetCp(const id, cpid, cps[racecptype]) //返回指定赛道的cp点数据
{
    cpid--;
    new msg[512], Cache:result; //,ti=GetTickCount();
    mysql_format(g_Sql, msg, sizeof(msg), "SELECT * FROM `race_cp` WHERE `rid`='%i' and `cp`='%i'", Race[id][rid], cpid);
    result = mysql_query(g_Sql, msg);
    cache_get_value_index_int(0, 0, cps[rcpid]);
    cache_get_value_index_int(0, 1, cps[rrid]);
    cache_get_value_index_int(0, 2, cps[rcp]);
    cache_get_value_index_float(0, 3, cps[rcpx]);
    cache_get_value_index_float(0, 4, cps[rcpy]);
    cache_get_value_index_float(0, 5, cps[rcpz]);
    cache_get_value_index_float(0, 6, cps[rcpa]);//Angle角度数值
    cache_get_value_index_float(0, 7, cps[rcppsize]);
    cache_delete(result);
    //	printf("  >P-Race 读取赛道[%s]的第[%i]个CP点,耗时[%i]ms",Race[id][rname],cpid+1,GetTickCount()-ti);
    return cps;
}

stock Race_GetCps(const id) //返回指定赛道的CP总数
{
    new Query[128];
    mysql_format(g_Sql, Query, sizeof(Query), "SELECT COUNT(*) FROM `race_cp` WHERE `rid`='%i'", Race[id][rid]);
    mysql_pquery(g_Sql, Query, "RaceGetCpsQuery", "d", id);
    // s = cache_num_rows();
    // printf("%d %d", id, s);
    // cache_delete(result);
    //	printf("  >P-Race 读取赛道[%s]的CP数量,耗时[%i]ms",Race[id][rname],GetTickCount()-ti);
    // return s;
}
function RaceGetCpsQuery(i) {
    cache_get_value_index_int(0, 0, Race[i][rcps]);
    if(Race[i][rcps] > 0) { //如果rcps大于0的话就创建CP点
        new trcp[racecptype], ttmp[128];
        Race_GetCp(i, 1, trcp);
        //Race[i][DynamicCP] = CreateDynamicCP(trcp[rcpx], trcp[rcpy], trcp[rcpz], 4.0, 0, -1, -1);
        //Race[i][DynamicMapIcon] = CreateDynamicMapIcon(trcp[rcpx], trcp[rcpy], trcp[rcpz], 53, Color_Race, 0, -1, -1); //创建3D的地图标记
        format(ttmp, sizeof(ttmp), "[赛车] 输入/r s %s 来发起这个赛道吧~", Race[i][rname]);
        Race[i][Dynamic3DText] = CreateDynamic3DTextLabel(ttmp, Color_Race, trcp[rcpx], trcp[rcpy], trcp[rcpz] + random(10) * 0.1, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, -1, -1, STREAMER_3D_TEXT_LABEL_SD);
    }
    // printf("[赛车] [%i]:%s(%i)|密码:%s|作者:%s|耗时:%ims|CP:%i", i, Race[i][rname], Race[i][rid], Race[i][rpassword], Race[i][rauthor], GetTickCount() - ti, Race[i][rcps]);
    printf("[赛车] [%i]:%s(%i)|密码:%s|作者:%s|CP:%i", i, Race[i][rname], Race[i][rid], Race[i][rpassword], Race[i][rauthorName], Race[i][rcps]);
    // 因为MYSQL拿多线程跑的所以不是顺序来的 取缔以往的rcps写法
    // [赛车] [166]:fxxk_stupid2(214)|密码:|作者:Waunny|CP:4
    // [赛车] [153]:FISH_TEST(207)|密码:|作者:[kaka.AF]CPT.FISH|CP:10
    // [赛车] [155]:fly(221)|密码:|作者:[DE]cj_abb|CP:18
    // [赛车] [168]:ggg(384)|密码:|作者:none|CP:0
    // [赛车] [157]:flying(232)|密码:|作者:[DE]Dark_Eagle|CP:17
    // [赛车] [159]:FlyTeamRealSpeed(279)|密码:|作者:none|CP:12
    // 可以看到多线程 不是按顺序来的
}
stock Race_CheckName(const name[]) //检查赛道是否存在,返回-1表示不存在,其他数字表示赛道的ID
{
    new s = -1;
    for (new i = 0; i < RACE_MAX; i++) {
        if(Race[i][rid] != -1) {
            if(strcmp(Race[i][rname], name) == 0) {
                s = i;
                i = RACE_MAX + 1;
            }
        }
    }
    return s;
}

stock Race_ReTop(const raceid) //清空指定赛道的top10
{
    // new tpm[512]; //,ti=GetTickCount();
    // mysql_tquery(g_Sql, "START TRANSACTION;");
    // for (new i = 0; i < 10; i++) {
    //     format(RaceTop[raceid][i][rtop_name], 32, "unknow");
    //     RaceTop[raceid][i][rtop] = -1;
    //     mysql_format(g_Sql, tpm, sizeof(tpm), "UPDATE `race` SET `top%i_name` = 'unknow' ,`top%i`='-1' WHERE `rname` = '%e';", i + 1, i + 1, Race[raceid][rname]);
    //     mysql_tquery(g_Sql, tpm);
    // }
    // mysql_tquery(g_Sql, "COMMIT;");

    // 2020-08-02 22:31:31 修改适配个人记录表
    new Query[256];
    mysql_format(g_Sql, Query, sizeof(Query), "DELETE FROM `race_record` WHERE `rid` = %d", raceid);
    mysql_pquery(g_Sql, Query);
    // printf("  >P-Race 初始化赛道[%s]的记录,耗时:%ims",Race[raceid][rname],GetTickCount()-ti);
    printf("[高等级操作] >P-Race 初始化赛道[%s]的记录",Race[raceid][rname]);
}

stock Race_Delete(const raceid) //删除赛道
{

    new msg[512], playerid = Race[raceid][reditplayer];
	CallRemoteFunction("OnPlayerCommandText","is",playerid, "/r edit q");
    //OnPlayerCommandText(playerid, "");
    Race_Game_DeleteByRid(raceid);
    mysql_tquery(g_Sql, "START TRANSACTION;");
    mysql_format(g_Sql, msg, sizeof(msg), "DELETE FROM `race_cp_script` WHERE `rid` = '%i';", Race[raceid][rid]);
    mysql_tquery(g_Sql, msg);
    mysql_format(g_Sql, msg, sizeof(msg), "DELETE FROM `race_cp` WHERE `rid`='%i';", Race[raceid][rid]);
    mysql_tquery(g_Sql, msg);
    mysql_format(g_Sql, msg, sizeof(msg), "DELETE FROM `race` WHERE `rname`='%e';", Race[raceid][rname]);
    mysql_tquery(g_Sql, msg);
    mysql_tquery(g_Sql, "COMMIT;", "reloadRaceMapIcon", "d", raceid);
    // 2020.3.12 同步在赛道删除后 让CP点和3D文字 地图图标也消失
    Race[raceid][rid] = -1;
    // UpdateRace(); //更新赛道排序并删除对应3D文字 CP点等
    //	printf("  >P-Race 删除赛道[%s],耗时[%i]ms",Race[raceid][rname],GetTickCount()-ti);
}
function reloadRaceMapIcon(i) {
    //删除这个赛道图标
    DestroyDynamicMapIcon(Race[i][DynamicMapIcon]);
    DestroyDynamicCP(Race[i][DynamicCP]);
    DestroyDynamic3DTextLabel(Race[i][Dynamic3DText]);
    if(Race[i][rcps] > 0) { //如果rcps大于0的话就创建CP点
        new trcp[racecptype], ttmp[128];
        Race_GetCp(i, 1, trcp);
        //Race[i][DynamicCP] = CreateDynamicCP(trcp[rcpx], trcp[rcpy], trcp[rcpz], 4.0, 0, -1, -1);
        //Race[i][DynamicMapIcon] = CreateDynamicMapIcon(trcp[rcpx], trcp[rcpy], trcp[rcpz], 53, Color_Race, 0, -1, -1); //创建3D的地图标记
        format(ttmp, sizeof(ttmp), "[赛车] 输入/r s %s 来发起这个赛道吧~", Race[i][rname]);
        Race[i][Dynamic3DText] = CreateDynamic3DTextLabel(ttmp, Color_Race, trcp[rcpx], trcp[rcpy], trcp[rcpz] + random(10) * 0.1, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, -1, -1, STREAMER_3D_TEXT_LABEL_SD);
    }

}
stock Race_Create(const name[], const password[], const author) //创建赛道,返回-1表示失败
{
    if(NowRaceId >= RACE_MAX) return -1;
    new id = NowRaceId;
    NowRaceId++;
    //	new ti=GetTickCount();
    new tpm[1024];
    //	printf(name);
    format(Race[id][rname], 32, "%s", name);
    format(Race[id][rpassword], 32, "%s", password);
    // format(Race[id][rauthor], 32, "%s", author);
    format(Race[id][rmiaoshu], 64, "无");
    Race[id][rcreatedate] = gettime();
    Race[id][reditplayer] = -1;
    Race[id][rcps] = 0;
    mysql_tquery(g_Sql, "START TRANSACTION;");
    mysql_format(g_Sql, tpm, sizeof(tpm), "INSERT INTO `race` (`rname`,`rpassword`,`rcreatedate`,`rauthor`,`miaoshu`) VALUES ('%e','%e','%d','%d','%e');", \
    name, password, gettime(), author, "无");
    mysql_tquery(g_Sql, tpm, "CreateRaceQuery", "d", id);
    // format(tpm, 1024, "SELECT rid FROM Race WHERE rname='%s'", name);
    // mysql_tquery(g_Sql, tpm);
    // db_get_field(rf, 0, tpm, 512);
    // Race[id][rid] = strval(tpm);
    // for (new i = 0; i < 10; i++) {
    //     format(RaceTop[id][i][rtop_name], 32, "unknow");
    //     RaceTop[id][i][rtop] = -1;
    //     mysql_format(g_Sql, tpm, sizeof(tpm), "UPDATE `race` SET `top%i_name` = 'unknow' ,`top%i`='-1' WHERE `rname` = '%e';", i + 1, i + 1, name);
    //     mysql_tquery(g_Sql, tpm);
    // }
    mysql_tquery(g_Sql, "COMMIT;", "reloadRaceMapIcon", "d", id);
    // UpdateRace();
    // 2020.3.12 新增创建赛道同步CP更新 不能这么写 如果说他赛道是没CP点的话就会出错的 所以写在退出编辑的地方
    // new trcp[racecptype],ttmp[128];
    // Race_GetCp(id, 1, trcp);
    // format(ttmp, sizeof(ttmp), "[赛车] 输入/r s %s 来发起这个赛道吧~", Race[id][rname]);
    // Race[id][DynamicCP] = CreateDynamicCP(trcp[rcpx], trcp[rcpy], trcp[rcpz], 4.0, 0, -1, -1);
    // Race[id][DynamicMapIcon] = CreateDynamicMapIcon(trcp[rcpx], trcp[rcpy], trcp[rcpz], 53, COLOR, 0, -1, -1); //创建3D的地图标记
    // Race[id][Dynamic3DText] = CreateDynamic3DTextLabel(ttmp, COLOR, trcp[rcpx], trcp[rcpy], trcp[rcpz] + random(10) * 0.1, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, -1, -1, STREAMER_3D_TEXT_LABEL_SD);
    //	printf("  >P-Race 创建赛道[%i]:%s(%i)|密码:%s|作者:%s|耗时:%ims",id,Race[id][rname],Race[id][rid],Race[id][rpassword],Race[id][rauthor],GetTickCount()-ti);
    return 0;
}
function CreateRaceQuery(id) {
    Race[id][rid] = cache_insert_id();
}
function UpdateRace() { //2020.3.12新增 在创建赛道和删除赛道时调用 用于创建和赛道后触发更新CP点和/r s里按字母排序
    for (new i = 0; i < sizeof(Race); i++) { //删除所有图标
        DestroyDynamicMapIcon(Race[i][DynamicMapIcon]);
        DestroyDynamicCP(Race[i][DynamicCP]);
        DestroyDynamic3DTextLabel(Race[i][Dynamic3DText]);
    }
    // format(msg, sizeof(msg), "SELECT * FROM Race");
    //2020.3.12写 根据赛道名升序 A-Z   
    // 需要优化:删除赛道和创建赛道的时候对应创建和删除地图和下标
    new Query[128];
    mysql_format(g_Sql, Query, sizeof(Query), "SELECT `race`.*,`users`.`Name` FROM `race` join `users` on\
    `race`.`rauthor` = `users`.`ID`\
    ORDER BY `rname` ASC");
    mysql_pquery(g_Sql, Query, "UpdateRaceQuery");
}
function UpdateRaceQuery() {
    new rs = cache_num_rows();
    if(rs >= RACE_MAX) {
        printf("[赛车] [警告]赛道数量超过赛道上线,请修改PRace.inc中的RACE_MAX,系统关闭");
        //SetTimer("FilterScriptExit",3000,0);
        RaceEd = -1;
        return 1;
    }
    for (new i = 0; i < rs; i++) {
        // new ti = GetTickCount();
        cache_get_value_name_int(i, "rid", Race[i][rid]);
        cache_get_value_name(i, "rname", Race[i][rname], 32);
        cache_get_value_name(i, "rpassword", Race[i][rpassword], 32);
        cache_get_value_name_int(i, "rcreatedate", Race[i][rcreatedate]); // 2021.10.7修复已知问题 cache_get_value_name_int: value 'xxx' is not a number
        // cache_get_value_index(i, 4, Race[i][rauthor], 32);
        // cache_get_value_name_int(i, "rauthor", Race[i][rauthor]);
        cache_get_value_name_int(i, "rauthor", Race[i][rauthor]); 
        cache_get_value_name(i, "Name", Race[i][rauthorName]);
        cache_get_value_name(i, "miaoshu", Race[i][rmiaoshu], 64); // 2021.10.7修复已知问题 cache_get_value_name: field 'rmiaoshu' not found问题
        // for (new n = 0; n < 10; n++) {
        //     new msg[32];
        //     format(msg, sizeof(msg), "top%i_name", n+1);
        //     cache_get_value_name(i, msg, RaceTop[i][n][rtop_name], 32);
        //     format(msg, sizeof(msg), "top%i", n+1);
        //     cache_get_value_name_int(i, msg, RaceTop[i][n][rtop]);
        // }
        // 因为MYSQL拿线程跑的所以不是顺序来的 取缔以往的rcps写法
        Race_GetCps(i);
        // Race[i][rcps] = Race_GetCps(i); //应该是问题在这里
        Race[i][rstate] = 0;
        Race[i][reditplayer] = -1;
    }
    NowRaceId = rs;
    return 1;
}
stock Race_Load() //载入赛道
{
    UpdateRace();
}
/*赛道相关*/

stock Race_OnFilterScriptInit() {
    SetupRaceTable();
    printf("[赛车] >P-Race 载入..");
    RaceEd = 0;
    Race_Init();
    printf("[赛车] >P-Race 初始化..");
    Race_Load();
    Race_Game_Init();
    return 1;
}

stock Race_OnFilterScriptExit() {
    printf("[赛车] >P-Race 退出..");
    // db_close(Racedb);
    return 1;
}

stock Race_OnPlayerConnect(const playerid) {
    EditRace[playerid][rraceid] = -1;
    EditRace[playerid][rtestcp] = -1;
    EditRace[playerid][rdialogcp] = 1;
    EditRace[playerid][rcpsize] = 9;
    EditRace[playerid][rdialogpage] = 1;
    EditRace[playerid][rdialogcppage] = 1;
    EditRace[playerid][rdialogcplist] = -1;
    GameRace[playerid][rgameid] = -1;
    GameRace[playerid][rtime] = 0;
    GameRace[playerid][rgamecp] = 0;
    SelectRacePage[playerid] = 1;
    SelectRecentlyRacePage[playerid] = 1;//最近比赛选择页
    SelectRecentlyClicked[playerid] = playerid;//最近比赛选择页
    // Timekg[playerid] = 0;
    // jishu[playerid] = -1; //2020.3.24改 原本是0
    pRaceing[playerid] = 0;
    // GetPlayerName(playerid, GetName(playerid), 32);
    return 1;
}

stock Race_OnPlayerDisconnect(const playerid) {
    if(pRaceing[PlayerInfo[playerid][tvid]]) { //如果玩家TV的那个人还在跑比赛的话隐藏掉CP
        PlayerTextDrawHide(playerid, CpTextDraw[PlayerInfo[playerid][tvid]]);
        PlayerTextDrawHide(playerid, Time[PlayerInfo[playerid][tvid]]);
        PlayerTextDrawHide(playerid, Top[PlayerInfo[playerid][tvid]]);
        PlayerTextDrawHide(playerid, p_record[PlayerInfo[playerid][tvid]]);
    }
    //2020.3.30写 如果有问题再注释

    if(EditRace[playerid][rraceid] != -1) {
        Race[EditRace[playerid][rraceid]][rstate] = 0;
        Race[EditRace[playerid][rraceid]][reditplayer] = -1;
    }
    if(GameRace[playerid][rgameid] != -1) {
        Race_Game_Quit(playerid);
    }
    //TextDrawDestroy(CpTextDraw[playerid]);
    // KillTimer(jishu[playerid]);
    // //KillTimer(RaceHouse[houseid][rplayerrank][playerid]);
    // TextDrawDestroy(Time[playerid]);
    // TextDrawDestroy(Top[playerid]);
    return 1;
}
stock Race_Cp_Script_Start(const playerid, const raceid, cpid) //为指定玩家执行指定赛道指定cp点的触发
{
    new trcp[racecptype];
    Race_GetCp(raceid, cpid, trcp);
    new ss = Race_GetCp_Scripts(trcp[rcpid]), script[RACE_CP_MAX_FUNCTION], idx, tmp[128], msg[256];
    // new Float:pX, Float:pY, Float:pZ;
    // GetPlayerPos(playerid, pX, pY, pZ);
    for (new i = 0; i < ss; i++) {
        Race_GetCp_Script(trcp[rcpid], i, script);
        format(script, RACE_CP_MAX_FUNCTION, "%s", script);
        tmp = strtok(script, idx);
        if(strcmp(tmp, "spawnpos", false) == 0) return 1; //如果是赛道重生的话，碰到下一个点并不触发 只是说重生时拿去提取坐标
        // 如果没写就默认是上一个CP点的坐标
        if(Race_Cp_Script_Check(tmp) != -1) {
            format(msg, sizeof(msg), "RaceCpScript_func_%s", tmp);
            CallLocalFunction(msg, "iiis", playerid, raceid, cpid, script);
        } else {
            format(msg, sizeof(msg), "[赛车]赛道[%s]的第[%i]个检查点触发第[%i]条错误", Race[raceid][rname], cpid, i);
            SendClientMessage(playerid, Color_Race, msg);
            format(msg, sizeof(msg), "[赛车]不存在函数[%s]", tmp);
            SendClientMessage(playerid, Color_Race, msg);
        }
        idx = 0;
    }
    return 1;
}




CMD:r(playerid, params[]) //赛道操作
{
    if(RaceEd == -1) return SendClientMessage(playerid, Color_Race, "[赛车] 系统关闭中…");
    if(GetPlayerCreditpoints(playerid) <= 95) return SendClientMessage(playerid, Color_Yellow, "[赛车]您的游戏信誉分过低,请健康游戏,将会自动补回");
    new msg[128], idx;
    new racetmp[128];
    racetmp = strtok(params, idx);
    if(isnull(racetmp)) //显示赛道帮助
    // if(!strlen(racetmp)) //显示赛道帮助
    {
        Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "赛车系统", "[赛车] 帮助:/r s (赛道名),开启/开始比赛,赛道名可省略\n[赛车] 帮助:/r j,加入比赛\n[赛车] 帮助:/r l,离开比赛\n[赛车] 帮助:/r page 页数,跳转到你想要的页数\n/r info 赛道 快速显示赛道详细页\n[赛车] 帮助:/r create,创建赛道\n[赛车] 帮助:/r edit,编辑赛道帮助\n赛道系统中翻车请下车或/kill重生", "确定", "返回");
        // SendClientMessage(playerid, Color_Race, "[赛车] 帮助:/r s,开启/开始比赛");
        // SendClientMessage(playerid, Color_Race, "[赛车] 帮助:/r j,加入比赛");
        // SendClientMessage(playerid, Color_Race, "[赛车] 帮助:/r l,离开比赛");
        // SendClientMessage(playerid, Color_Race, "[赛车] 帮助:/r page 页数,跳转到你想要的页数");
        // SendClientMessage(playerid, Color_Race, "[赛车] 帮助:/r create,创建赛道");
        // SendClientMessage(playerid, Color_Race, "[赛车] 帮助:/r edit,编辑赛道帮助");
        return 1;
    }
    if(strcmp(racetmp, "info") == 0) {
        racetmp = strtok(params, idx);
        new s = -1;
        for (new i = 0; i < RACE_MAX; i++) {
            if(Race[i][rid] != -1) {
                if(strcmp(Race[i][rname], racetmp) == 0) {
                    s = i;
                    break;
                }
            }
        }
        if(s == -1) return SendClientMessage(playerid, Color_Yellow, "[赛车]该赛道不存在");
        Race_ShowGameDialog(playerid, s);
        return 1;
    }
    if(strcmp(racetmp, "s") == 0) {
        if(EditRace[playerid][rraceid] != -1) {
            format(msg, sizeof(msg), "[赛车] 你正在编辑赛道[%s],因此无法开启比赛", Race[EditRace[playerid][rraceid]][rname]);
            SendClientMessage(playerid, Color_Race, msg);
            return 1;
        }
        if(GameRace[playerid][rgameid] != -1) {
            if(playerid == RaceHouse[GameRace[playerid][rgameid]][rplayerid]) {
                if(RaceHouse[GameRace[playerid][rgameid]][rstate] == 0) {
                    Race_Game_Start(GameRace[playerid][rgameid]); //开始游戏
                }
            } else {
                // if(playerid == RaceHouse[GameRace[playerid][rgameid]][rplayerid]) format(msg, sizeof(msg), "[赛车] 您是房主,可输入指令/r s开始比赛~");
                // else 
                format(msg, sizeof(msg), "[赛车] 请耐心等待发起者'%s'开始比赛~", GetName(RaceHouse[GameRace[playerid][rgameid]][rplayerid]));
                // format(msg, sizeof(msg), "[赛车] 请耐心等待发起者'%s'开始比赛~", PlayerName[RaceHouse[GameRace[playerid][rgameid]][rplayerid]]);
                SendClientMessage(playerid, Color_Race, msg);
            }
            return 1;
        }
        racetmp = strtok(params, idx);
        if(isnull(racetmp)) {
        // if(!strlen(racetmp)) {
        // if(strcmp(racetmp, "") == 0) {
            Race_ShowGameSelDialog(playerid, SelectRacePage[playerid]);
            return 1;
        }
        new id = Race_CheckName(racetmp);
        if(id == -1) {
            format(msg, sizeof(msg), "[赛车] 赛道[%s]不存在....", racetmp);
            SendClientMessage(playerid, Color_Race, msg);
            return 1;
        }
        if(Race[id][rcps] == 0) {
            format(msg, sizeof(msg), "[赛车] 赛道[%s]无法开启.因为其检查点数量为0", racetmp);
            SendClientMessage(playerid, Color_Race, msg);
            return 1;
        }
        if(Race[id][rstate]) {
            format(msg, sizeof(msg), "[赛车] 赛道[%s]无法开启.因为正在被编辑中", racetmp); //2020.1.14加
            SendClientMessage(playerid, Color_Race, msg);
            return 1;
        }
        new hid = Race_Game_Create(playerid, id);
        if(hid == -1) {
            format(msg, sizeof(msg), "[赛车] 创建房间失败...当前赛车房间已达上限~请等待其他房间完成比赛..");
            SendClientMessage(playerid, Color_Race, msg);
            return 1;
        }
        // if(!IsPlayerInAnyVehicle(playerid)) {
        //     SendClientMessage(playerid, Color_Race, "[赛车] 你没有在任何载具里无法发起比赛");
        //     return 1;
        // }
        // 2020.3.23加一个提示 因爱车适配赛道重生系统不便，并且爱车的OBJ会影响比赛公平性，不支持使用爱车进入.
        if(IsPlayerInAnyVehicle(playerid)) {
            for (new i = 0; i < CarCount; i++) {
                if(GetPlayerVehicleID(playerid) == CarInfo[i][GotoID]) {
                    SendClientMessage(playerid, Color_Race, "[赛车] 因爱车适配赛道重生系统不便,且爱车装扮会影响公平性,不支持加入");
                    return 1;
                }
            }
        }
        format(msg, sizeof(msg), "[赛车] %s 创建了关于'%s'的赛道,你可以输入'{FFFF00}/r j %i'来加入比赛!", GetName(playerid), Race[id][rname], playerid);
        SendClientMessageToAll(Color_Race, msg);
        new removecolor = strfind(msg, "{", true); //自动屏蔽颜色输出 节省日志空间 By YuCarl77
        while (removecolor != -1) {
            if(removecolor + 8 <= strlen(msg) && msg[removecolor + 7] == '}') {
                strdel(msg, removecolor, removecolor + 8);
                removecolor = strfind(msg, "{", true);
            } else {
                break;
            }
        }
        printf("%s", msg);
        Race_Game_Join(playerid, hid);
        return 1;
    }
    if(strcmp(racetmp, "j") == 0) {
        if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) {
            // if(!IsPlayerInAnyVehicle(playerid)) {
            SendClientMessage(playerid, Color_Race, "[赛车] 你没有在任何载具里或你处在副驾驶，无法加入比赛!");
            return 1;
        }
        if(EditRace[playerid][rraceid] != -1) {
            format(msg, sizeof(msg), "[赛车] 你正在编辑赛道[%s],因此无法加入比赛", Race[EditRace[playerid][rraceid]][rname]);
            SendClientMessage(playerid, Color_Race, msg);
            return 1;
        }
        if(GameRace[playerid][rgameid] != -1) {
            format(msg, sizeof(msg), "[赛车] 你已经加入了一场比赛");
            SendClientMessage(playerid, Color_Race, msg);
            return 1;
        }
        racetmp = strtok(params, idx);
        // if(strcmp(racetmp, "") == 0) {
        if(isnull(racetmp)) {
            format(msg, sizeof(msg), "[赛车] 用法:/r j ID[任何一个在比赛房间内的人ID]");
            SendClientMessage(playerid, Color_Race, msg);
            return 1;
        }
        new id = strval(racetmp);
        if(GameRace[id][rgameid] == -1) {
            format(msg, sizeof(msg), "[赛车] %s 不在房间内", GetName(id));
            SendClientMessage(playerid, Color_Race, msg);
            return 1;
        }
        for (new i = 0; i < CarCount; i++) {
            if(GetPlayerVehicleID(playerid) == CarInfo[i][GotoID]) {
                SendClientMessage(playerid, Color_Race, "[赛车] 因爱车适配赛道重生系统不便,且爱车装扮会影响公平性,不支持加入");
                return 1;
            }
        }
        new ri = Race_Game_Join(playerid, GameRace[id][rgameid]);
        if(ri == -1) {
            format(msg, sizeof(msg), "[赛车] 加入房间失败,这个房间已经满人或开始比赛了..");
            SendClientMessage(playerid, Color_Race, msg);
            return 1;
        }
        return 1;
    }
    if(strcmp(racetmp, "l") == 0 || strcmp(racetmp, "leave") == 0) {
        if(GameRace[playerid][rgameid] == -1) {
            format(msg, sizeof(msg), "[赛车] 你没有参与任何比赛~请输入/r j或者/r s来加入/开启新的比赛~");
            SendClientMessage(playerid, Color_Race, msg);
            return 1;
        }
        // AntiCommand[playerid] = 0;
        // pRaceing[playerid] = 0;
        new hid = GameRace[playerid][rgameid];
        Race_Game_Quit(playerid);
        format(msg, sizeof(msg), "[赛车] 遗憾! %s 离开了比赛", GetName(playerid));
        Race_Game_Msg(hid, msg);
        SendClientMessage(playerid, Color_Race, "[赛车] 遗憾! 你离开了比赛！");
        // printf("%s", msg);
        return 1;
        // TextDrawDestroy(ReSpawningText[playerid]);
        //TextDrawDestOoForPlayer(playerid, ReSpawningText[playerid]); //隐藏重生中的显示 2020.1.12写
        /*KillTimer(jishu[playerid]);
        //KillTimer(playerrank[houseid]);
        TextDrawDestroy(Time[playerid]);
        TextDrawDestroy(Top[playerid]);*/
        // return 1;
    }
    if(strcmp(racetmp, "edit") == 0) //编辑赛道
    {
        racetmp = strtok(params, idx);
        // if(strcmp(racetmp, "") == 0) {
        if(isnull(racetmp)) {
            Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "赛车系统", "如不是管理员则只可编辑自己创建的赛道\n[赛车]帮助:/r edit [赛道名] [赛道编辑密码],编辑赛道\n[赛车]帮助:/r edit q 退出编辑模式\n[赛车]帮助:/r edit cpsize,设置/查看当前编辑cp的尺寸\n[赛车]帮助:/r edit d,界面操作\n[赛车]帮助:/r edit cp,在当前位置放置一个cp点\n[赛车]帮助:/r edit trg,查看触发说明", "确定", "返回");
            // SendClientMessage(playerid, Color_Race, "[赛车]帮助:/r edit [赛道名] [赛道编辑密码],编辑赛道");
            // SendClientMessage(playerid, Color_Race, "[赛车]帮助:/r edit q 退出编辑模式");
            // SendClientMessage(playerid, Color_Race, "[赛车]帮助:/r edit cpsize,设置/查看当前编辑cp的尺寸");
            // SendClientMessage(playerid, Color_Race, "[赛车]帮助:/r edit d,界面操作");
            // SendClientMessage(playerid, Color_Race, "[赛车]帮助:/r edit cp,在当前位置放置一个cp点");
            // SendClientMessage(playerid, Color_Race, "[赛车]帮助:/r edit trg,查看触发说明");
            return 1;
        }
        if(strcmp(racetmp, "trg") == 0) {
            Race_ShowTrgDialog(playerid, "main");
            return 1;
        }
        if(strcmp(racetmp, "cp") == 0) {
            if(EditRace[playerid][rraceid] == -1) {
                SendClientMessage(playerid, Color_Race, "[赛车]你没有正在编辑的赛道..");
                return 1;
            }
            Race_NewCp(EditRace[playerid][rraceid], playerid);
            format(msg, sizeof(msg), "[赛车]你为赛道[%s]在这个位置放置了第[%i]个CP点", Race[EditRace[playerid][rraceid]][rname], Race[EditRace[playerid][rraceid]][rcps]);
            SendClientMessage(playerid, Color_Race, msg);
            Race_ShowCp(playerid, EditRace[playerid][rraceid], Race[EditRace[playerid][rraceid]][rcps]);
            return 1;
        }
        if(strcmp(racetmp, "q") == 0) {
            if(EditRace[playerid][rraceid] == -1) {
                SendClientMessage(playerid, Color_Race, "[赛车]你没有正在编辑的赛道..");
                return 1;
            }
            format(msg, sizeof(msg), "[赛车]你退出了赛道[%s]的编辑模式", Race[EditRace[playerid][rraceid]][rname]);
            SendClientMessage(playerid, Color_Race, msg);
            // 2020.3.12新增退出编辑的时候更新赛道的地图标记点等  如果出问题可能是在destroy这里
            //如果造成其他3D文字误删的话应该就是这个造成的
            DestroyDynamicMapIcon(Race[EditRace[playerid][rraceid]][DynamicMapIcon]);
            DestroyDynamicCP(Race[EditRace[playerid][rraceid]][DynamicCP]);
            DestroyDynamic3DTextLabel(Race[EditRace[playerid][rraceid]][Dynamic3DText]);
            if(Race[EditRace[playerid][rraceid]][rcps]) { //如果CP点大于1的话
                new trcp[racecptype], ttmp[128];
                Race_GetCp(EditRace[playerid][rraceid], 1, trcp);
                format(ttmp, sizeof(ttmp), "[赛车] 输入/r s %s 来发起这个赛道吧~", Race[EditRace[playerid][rraceid]][rname]);
                //Race[EditRace[playerid][rraceid]][DynamicCP] = CreateDynamicCP(trcp[rcpx], trcp[rcpy], trcp[rcpz], 4.0, 0, -1, -1);
                Race[EditRace[playerid][rraceid]][DynamicMapIcon] = CreateDynamicMapIcon(trcp[rcpx], trcp[rcpy], trcp[rcpz], 53, Color_Race, 0, -1, -1); //创建3D的地图标记
                Race[EditRace[playerid][rraceid]][Dynamic3DText] = CreateDynamic3DTextLabel(ttmp, Color_Race, trcp[rcpx], trcp[rcpy], trcp[rcpz] + random(10) * 0.1, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, -1, -1, STREAMER_3D_TEXT_LABEL_SD);
                // 2020.3.12新增退出编辑的时候更新赛道的地图标记点等
            }
            Race[EditRace[playerid][rraceid]][rstate] = 0;
            Race[EditRace[playerid][rraceid]][reditplayer] = -1;
            EditRace[playerid][rraceid] = -1;
            EditRace[playerid][rtestcp] = -1;
            EditRace[playerid][rdialogpage] = 1;
            EditRace[playerid][rdialogcp] = 1;
            EditRace[playerid][rdialogcppage] = 1;
            EditRace[playerid][rdialogcplist] = -1;
            Race_HideCp(playerid);
            return 1;
        }
        if(strcmp(racetmp, "cpsize") == 0) {
            racetmp = strtok(params, idx);
            // if(strcmp(racetmp, "") == 0) {
            if(isnull(racetmp)) {
                format(msg, sizeof(msg), "[赛车]你的CP点尺寸为:%f", EditRace[playerid][rcpsize]);
                SendClientMessage(playerid, Color_Race, msg);
                return 1;
            }
            if(strlen(racetmp) > 12) {
                format(racetmp, 128, "%.7f", floatstr(racetmp));
            }
            new Float:f = floatstr(racetmp);
            if(f < 0) f = 0;
            if(f > 1000) f = 1000;
            EditRace[playerid][rcpsize] = f;
            format(msg, sizeof(msg), "[赛车]你将cp点尺寸设置为:%f", f);
            SendClientMessage(playerid, Color_Race, msg);
            return 1;
        }
        if(strcmp(racetmp, "d") == 0) {
            if(EditRace[playerid][rraceid] == -1) return SendClientMessage(playerid, Color_Race, "[赛车]你没有正在编辑的赛道..");
            Race_ShowEditDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogpage]);
            return 1;
        }
        new id = Race_CheckName(racetmp);
        if(id == -1) {
            format(msg, sizeof(msg), "[赛车]赛道[%s]不存在....", racetmp);
            SendClientMessage(playerid, Color_Race, msg);
            return 1;
        }
        racetmp = strtok(params, idx);
        // if(strcmp(racetmp, "") == 0) return SendClientMessage(playerid, Color_Race, "[赛车]赛道维护密码不能为空..");
        if(isnull(racetmp)) return SendClientMessage(playerid, Color_Race, "[赛车]赛道维护密码不能为空..");
        if(strcmp(racetmp, Race[id][rpassword]) != 0) {
            format(msg, sizeof(msg), "[赛车]赛道[%s]的维护密码错误...", Race[id][rname]);
            SendClientMessage(playerid, Color_Race, msg);
            return 1;
        }
        if(EditRace[playerid][rraceid] != -1) {
            format(msg, sizeof(msg), "[赛车]你正在编辑[%s]赛道,因此无法编辑[%s]", Race[EditRace[playerid][rraceid]][rname], Race[id][rname]);
            SendClientMessage(playerid, Color_Race, msg);
            return 1;
        }
        if(Race[id][rstate] == 1) {
            format(msg, sizeof(msg), "[赛车]赛道[%s]正在被玩家[-{00FF00}%s{9393AA}-]编辑,因此你无法编辑..", Race[id][rname], GetName(Race[id][reditplayer]));
            // format(msg, sizeof(msg), "[赛车]赛道[%s]正在被玩家[-{00FF00}%s{9393AA}-]编辑,因此你无法编辑..", Race[id][rname], PlayerName[Race[id][reditplayer]]);
            SendClientMessage(playerid, Color_Race, msg);
            return 1;
        }
        if(GameRace[playerid][rgameid] != -1) {
            format(msg, sizeof(msg), "[赛车]你正处于赛车状态,因此无法编辑赛道[%s]~", Race[id][rname]);
            SendClientMessage(playerid, Color_Race, msg);
            return 1;
        }
        // if(PlayerInfo[playerid][AdminLevel] >= 1 || strcmp(Race[id][rauthor], GetName(playerid), true) == 0) {
        if(PlayerInfo[playerid][AdminLevel] >= 1 || Race[id][rauthor] == PlayerInfo[playerid][ID]) {
            Race[id][rstate] = 1;
            Race[id][reditplayer] = playerid;
            EditRace[playerid][rraceid] = id;
            EditRace[playerid][rtestcp] = -1;
            EditRace[playerid][rdialogpage] = 1;
            EditRace[playerid][rdialogcp] = 1;
            EditRace[playerid][rdialogcppage] = 1;
            EditRace[playerid][rdialogcplist] = -1;
            Race_Game_DeleteByRid(id);
            format(msg, sizeof(msg), "[赛车]你已进入[%s]的编辑模式,该赛道将无法进行新的比赛,正在比赛的将被关闭", Race[id][rname]);
            SendClientMessage(playerid, Color_Race, msg);
            SendClientMessage(playerid, Color_Race, "[赛车]编辑模式下,你可以使用/r edit cpsize来查看/设置尺寸");
            SendClientMessage(playerid, Color_Race, "[赛车]创建的新CP点将使用cpsize所设置的尺寸");
            SendClientMessage(playerid, Color_Race, "[赛车]创建新CP点使用/r edit cp");
            SendClientMessage(playerid, Color_Race, "[赛车]重新打开此界面使用/r edit d");
            AntiCommand[playerid] = 0;
			CallRemoteFunction("OnPlayerCommandText","is",playerid, "/r edit d");
            //OnPlayerCommandText(playerid, "/r edit d");
        } else {
            SendClientMessage(playerid, Color_Race, "[赛车]你不是本赛道创始者或管理员，如需要请联系服主");
            return 1;
        }
        return 1;
    }
    if(strcmp(racetmp, "create") == 0) //创建赛道
    {
        if(PlayerInfo[playerid][Score] < 60) return SendClientMessage(playerid, Color_Race, "[赛车]游戏时间不足60分钟，不可创建赛道");
        new raname[128];
        raname = strtok(params, idx);
        // if(strcmp(raname, "") == 0) return SendClientMessage(playerid, Color_Race, "[赛车]帮助:/r create [赛道名称] [编辑密码],创建一次需3000金币");
        if(isnull(racetmp)) return SendClientMessage(playerid, Color_Race, "[赛车]帮助:/r create [赛道名称] [编辑密码],创建一次需3000金币");
        if(PlayerInfo[playerid][Cash] < 3000) return SendClientMessage(playerid, Color_Race, "[赛车]创建一次赛道需3000金币哦"); //2020.2.23新增
        if(strlen(raname) >= 32) return SendClientMessage(playerid, Color_Race, "[赛车]赛道名字过长……请缩短一点吧");
        if(Race_CheckName(raname) != -1) {
            format(msg, sizeof(msg), "[赛车]赛道[%s]已存在,请换一个名字吧~", raname);
            SendClientMessage(playerid, Color_Race, msg);
            return 1;
        }
        racetmp = strtok(params, idx);
        // if(strcmp(racetmp, "") == 0) return SendClientMessage(playerid, Color_Race, "[赛车]赛道编辑密码不能为空……");
        if(isnull(racetmp)) return SendClientMessage(playerid, Color_Race, "[赛车]赛道编辑密码不能为空……");
        if(strlen(racetmp) >= 32 || strlen(racetmp) <= 3) return SendClientMessage(playerid, Color_Race, "[赛车]赛道编辑密码过长或过短……");
        // if(Race_Create(raname, racetmp, GetName(playerid)) == -1) {
        if(Race_Create(raname, racetmp, PlayerInfo[playerid][ID]) == -1) {
            format(msg, sizeof(msg), "[赛车]赛道[%s]创建失败,没有可用的赛道ID", raname);
            SendClientMessage(playerid, 0x000088FF, msg);
        } else {
            format(msg, sizeof(msg), "[赛车]赛道[%s]创建成功,编辑密码[%s],请牢记~", raname, racetmp);
            SendClientMessage(playerid, Color_Race, msg);
            GivePlayerCash(playerid, -3000); //创建一次 -2000金币
        }
        return 1;
    }
    if(!strcmp(racetmp, "page")) {
        racetmp = strtok(params, idx);
        // if(!strlen(racetmp)) return SendClientMessage(playerid, Color_Race, "[赛车]请输入页数");
        if(isnull(racetmp)) return SendClientMessage(playerid, Color_Race, "[赛车]请输入页数");
        new pageid = strval(racetmp);
        if(pageid <= 0 || pageid > GetMaxPage(NowRaceId, RACE_PAGES)) return SendClientMessage(playerid, Color_Race, "[赛车]请输入正确的页数!");
        SelectRacePage[playerid] = pageid;
        Race_ShowGameSelDialog(playerid, pageid);
        return 1;
    }
    SendClientMessage(playerid, Color_Race, "[赛车]命令不存在哦");
    return 1;
}


Dialog:RACE_GAME_MAIN_DIALOG(playerid, response, listitem, inputtext[]) {//赛道信息界面
    if(response == 1) {
        new msg[128], idx;
        msg = strtok(inputtext, idx);
        if(strcmp(msg, "开始") == 0) {
            msg = strtok(inputtext, idx);
            new id = Race_CheckName(msg);
            if(id != -1) {
                format(msg, sizeof(msg), "/r s %s", Race[id][rname]);
                // OnPlayerCommandText(playerid, msg);
                CallRemoteFunction("OnPlayerCommandText", "is", playerid, msg);
                return 1;
            }
            SendClientMessage(playerid, Color_Race, "[赛车]发生了一个错误:GAME_MAIN_DIALOG_RESPONSE");
            return 1;
        }
    }
    return 1;
}

Dialog:RACE_RESPAWNDIALOG(playerid, response, listitem, inputtext[]) { //2020.2.5新增 提示是否继续观战的对话框
    if(response) {
        new Float:POS[3];
        GetPlayerPos(playerid, POS[0], POS[1], POS[2]);
        SetPlayerPos(playerid, POS[0], POS[1], POS[2]);
        SetPlayerHealth(playerid, -1.0);
        return 1;
    }
    return 1;
}


Dialog:RACE_EDIT_CP_SCRIPT_HMSG(playerid, response, listitem, inputtext[]) { //触发说明界面
    return 1;
}

Dialog:RACE_EDIT_CP_SCRIPT_H(playerid, response, listitem, inputtext[]) { //触发说明界面
    if(response == 1) {
        new msg[128], idx;
        msg = strtok(inputtext, idx);
        if(strcmp(msg, "查看语法") == 0) {
            Race_ShowTrgDialog(playerid, "语法");
            return 1;
        }
        if(strcmp(msg, "查看函数") == 0) {
            Race_ShowTrgDialog(playerid, "函数");
            return 1;
        }
        if(strcmp(msg, "查看转义符") == 0) {
            Race_ShowTrgDialog(playerid, "转义符");
            return 1;
        }
        if(strcmp(msg, "函数>") == 0) {
            msg = strtok(inputtext, idx);
            Race_ShowTrgFunDialog(playerid, msg);
            return 1;
        }
        if(strcmp(msg, "转义符>") == 0) {
            msg = strtok(inputtext, idx);
            Race_ShowTrgRetDialog(playerid, msg);
            return 1;
        }
    }
    return 1;
}

Dialog:RACE_GAME_MAIN_SEL(playerid, response, listitem, inputtext[]) { //选择赛道界面
    if(response == 1) {
        new msg[128], idx;
        msg = strtok(inputtext, idx);
        if(strcmp(msg, "上一页") == 0) {
            SelectRacePage[playerid]--;
            Race_ShowGameSelDialog(playerid, SelectRacePage[playerid]);
            return 1;
        }
        if(strcmp(msg, "下一页") == 0) {
            SelectRacePage[playerid]++;
            Race_ShowGameSelDialog(playerid, SelectRacePage[playerid]);
            return 1;
        }
        if(strcmp(msg, "赛道") == 0) {
            msg = strtok(inputtext, idx);
            new id = Race_CheckName(msg);
            if(id != -1) {
                Race_ShowGameDialog(playerid, id);
                return 1;
            }
            SendClientMessage(playerid, Color_Race, "[赛车]发生了一个错误:RACE_GAME_MAIN_DIALOG_SEL");
            return 1;
        }
        Race_ShowGameSelDialog(playerid, SelectRacePage[playerid]);
        return 1;
    } else {
        SelectRacePage[playerid] = 1;
        return 1;
    }
}

Dialog:RACE_EDIT_CP_SCRIPT(playerid, response, listitem, inputtext[]) { //编辑赛道CP点触发的界面
    if(response == 1) {
        if(EditRace[playerid][rdialogcplist] == -1) {
            if(strcmp(inputtext, "删除") == 0 || strcmp(inputtext, "插入") == 0) {
                Race_ShowEditCpScriptDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], "\n建立新触发时'插入'与'删除'指令无法使用");
                return 1;
            }
            if(strlen(inputtext) >= RACE_CP_MAX_FUNCTION) {
                Race_ShowEditCpScriptDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], "\n{00FF00}触发太长了,请缩短一点吧...");
                return 1;
            }
            Race_NewCp_Script(EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], inputtext);
            Race_ShowEditCpDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], EditRace[playerid][rdialogcppage]);
            return 1;
        } else {
            if(strcmp(inputtext, "删除") == 0) {
                Race_DelCp_Script(EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], EditRace[playerid][rdialogcplist]);
                EditRace[playerid][rdialogcplist] = -1;
                new tr[racecptype];
                Race_GetCp(EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], tr);
                new s = GetMaxPage(Race_GetCp_Scripts(tr[rcpid]), RACE_PAGES);
                if(EditRace[playerid][rdialogcppage] > s) {
                    EditRace[playerid][rdialogcppage] = s;
                }
                Race_ShowEditCpDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], EditRace[playerid][rdialogcppage]);
                return 1;
            }
            if(strcmp(inputtext, "插入") == 0) {
                Race_InsCp_Script(EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], EditRace[playerid][rdialogcplist], "新插入的触发,请修改触发内容~");
                Race_ShowEditCpScriptDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], "");
                return 1;
            }
            if(strlen(inputtext) >= RACE_CP_MAX_FUNCTION) {
                Race_ShowEditCpScriptDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], "\n{00FF00}触发太长了,请缩短一点吧...");
                return 1;
            }
            Race_SetCp_Script(EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], EditRace[playerid][rdialogcplist], inputtext);
            Race_ShowEditCpDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], EditRace[playerid][rdialogcppage]);
            return 1;
        }
    }
    EditRace[playerid][rdialogcplist] = -1;
    Race_ShowEditCpDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], EditRace[playerid][rdialogcppage]);
    return 1;    
}

Dialog:RACE_EDIT_CP(playerid, response, listitem, inputtext[]) {
    if(response == 1) 
    {
        new msg[128], idx;
        msg = strtok(inputtext, idx);
        if(strcmp(msg, "从这检查点开始测试赛道") == 0) 
        {
            for (new i = 0; i < CarCount; i++) {
                if(GetPlayerVehicleID(playerid) == CarInfo[i][GotoID]) {
                    SendClientMessage(playerid, Color_Race, "[赛车] 因爱车适配赛道重生系统不便,且爱车装扮会影响公平性,不支持加入");
                    return 1;
                }
            }
            if(Race_Test(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp]) == -1) {
                SendClientMessage(playerid, Color_Race, "[赛车]无法测试这个赛道,因为其检查点数量为0");
                Race_ShowEditCpDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], EditRace[playerid][rdialogcppage]);
            } else {
                EditRace[playerid][rdialogpage] = 1;
                EditRace[playerid][rdialogcp] = 1;
                EditRace[playerid][rdialogcppage] = 1;
                EditRace[playerid][rdialogcplist] = -1;
            }
            return 1;
        }
        if(strcmp(msg, "上一页") == 0) {
            EditRace[playerid][rdialogcppage]--;
            Race_ShowEditCpDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], EditRace[playerid][rdialogcppage]);
            return 1;
        }
        if(strcmp(msg, "下一页") == 0) {
            EditRace[playerid][rdialogcppage]++;
            Race_ShowEditCpDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], EditRace[playerid][rdialogcppage]);
            return 1;
        }
        if(strcmp(msg, "触发") == 0) {
            msg = strtok(inputtext, idx);
            EditRace[playerid][rdialogcplist] = strval(msg);
            Race_ShowEditCpScriptDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], "");
            return 1;
        }
        if(strcmp(msg, "删除所有触发") == 0) {
            Race_DelCp_Scripts(EditRace[playerid][rraceid], EditRace[playerid][rdialogcp]);
            EditRace[playerid][rdialogcplist] = -1;
            EditRace[playerid][rdialogcppage] = 1;
            Race_ShowEditCpDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], EditRace[playerid][rdialogcppage]);
            return 1;
        }
        if(strcmp(msg, "添加新触发") == 0) {
            EditRace[playerid][rdialogcplist] = -1;
            Race_ShowEditCpScriptDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], "");
            return 1;
        }
        if(strcmp(msg, "插入检查点") == 0) {
            Race_InsCp(EditRace[playerid][rraceid], playerid, EditRace[playerid][rdialogcp]);
            EditRace[playerid][rdialogpage] = 1;
            EditRace[playerid][rdialogcp] = 1;
            EditRace[playerid][rdialogcplist] = -1;
            Race_ShowEditDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcppage]);
            return 1;
        }
        if(strcmp(msg, "删除检查点") == 0) {
            new s;
            Race_HideCp(playerid);
            Race_DelCp(EditRace[playerid][rraceid], EditRace[playerid][rdialogcp]);
            s = GetMaxPage(Race[EditRace[playerid][rraceid]][rcps], RACE_PAGES);
            if(EditRace[playerid][rdialogpage] > s) {
                EditRace[playerid][rdialogpage] = s;
            }
            EditRace[playerid][rdialogcp] = 1;
            EditRace[playerid][rdialogcppage] = 1;
            EditRace[playerid][rdialogcplist] = -1;
            Race_ShowEditDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogpage]);
            return 1;
        }
        if(strcmp(msg, "传送到检查点") == 0) {
            new trcp[racecptype];
            Race_ShowCp(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp]);
            Race_GetCp(EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], trcp);
            SetPlayerPosEx(playerid, trcp[rcpx], trcp[rcpy], trcp[rcpz]);
            SetPlayerFacingAngleEx(playerid, trcp[rcpa]);
            Race_ShowEditCpDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], EditRace[playerid][rdialogcppage]);
            return 1;
        }
        if(strcmp(msg, "移动检查点") == 0) {
            Race_SetCpPos(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp]);
            Race_ShowCp(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp]);
            Race_ShowEditCpDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], EditRace[playerid][rdialogcppage]);
            return 1;
        }
        if(strcmp(msg, "size") == 0) {
            Race_SetCpSize(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp]);
            Race_ShowCp(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp]);
            Race_ShowEditCpDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], EditRace[playerid][rdialogcppage]);
            return 1;
        }
        Race_ShowEditCpDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], EditRace[playerid][rdialogcppage]);
        return 1;
    } 
    EditRace[playerid][rdialogcp] = 1;
    EditRace[playerid][rdialogcppage] = 1;
    EditRace[playerid][rdialogcplist] = -1;
    Race_ShowEditDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogpage]);
    return 1;
}
Dialog:RACE_EDIT_MIAOSHU(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        if(strlen(inputtext) >= 64) {
            Race_ShowEditMiaoshuDialog(playerid, EditRace[playerid][rraceid], "\n{00FF00}描述过长~请适当的缩短一点");
            return 1;
        }
        Race_SetMiaoshu(EditRace[playerid][rraceid], inputtext);
        Race_ShowEditDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogpage]);
        return 1;
    } else {
        Race_ShowEditDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogpage]);
        return 1;
    }
}

Dialog:RACE_EDIT_MAIN(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        new msg[128], idx;
        msg = strtok(inputtext, idx);
        if(strcmp(msg, "清空赛道记录") == 0) {
            if(PlayerInfo[playerid][AdminLevel] < 4)
            {
                if(gettime() - Race[EditRace[playerid][rraceid]][rcreatedate] >= 60 * 60 * 24 * 180)
                {
                    Race_ShowEditDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogpage]);
                    SendClientMessage(playerid, Color_Red, "[赛车] 该赛道创建时间已大于180天,启用保护机制,如仍需清空记录请联系4级以上管理员");
                    return 1;
                }
            }
            Race_ReTop(EditRace[playerid][rraceid]);
            SendClientMessage(playerid, Color_Red, "[赛车] 你清空了该赛道的所有记录");
            Race_ShowEditDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogpage]);
            return 1;
        }
        if(strcmp(msg, "删除该赛道") == 0) {
            if(PlayerInfo[playerid][AdminLevel] < 4)
            {
                if(gettime() - Race[EditRace[playerid][rraceid]][rcreatedate] >= 60 * 60 * 24 * 90)
                {
                    Race_ShowEditDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogpage]);
                    SendClientMessage(playerid, Color_Red, "[赛车] 该赛道创建时间已大于90天,启用保护机制,如仍需删除请联系4级和以上管理员");
                    return 1;
                }
            }
            Race_Delete(EditRace[playerid][rraceid]);
            return 1;
        }
        if(strcmp(msg, "测试该赛道") == 0) {
            // 2020.3.23新增
            for (new i = 0; i < CarCount; i++) {
                if(GetPlayerVehicleID(playerid) == CarInfo[i][GotoID]) {
                    SendClientMessage(playerid, Color_Race, "[赛车] 因爱车适配赛道重生系统不便,且爱车装扮会影响公平性,不支持加入");
                    return 1;
                }
            }
            if(Race_Test(playerid, EditRace[playerid][rraceid], 1) == -1) {
                SendClientMessage(playerid, Color_Race, "[赛车]无法测试这个赛道,因为其检查点数量为0");
                Race_ShowEditDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogpage]);
            } else {
                EditRace[playerid][rdialogpage] = 1;
                EditRace[playerid][rdialogcp] = 1;
                EditRace[playerid][rdialogcppage] = 1;
                EditRace[playerid][rdialogcplist] = -1;
            }
            return 1;
        }
        if(strcmp(msg, "修改所有检查点尺寸为") == 0) {
            Race_HideCp(playerid);
            Race_SetAllCpSize(playerid, EditRace[playerid][rraceid]);
            Race_ShowEditDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogpage]);
            return 1;
        }
        if(strcmp(msg, "检查点") == 0) {
            msg = strtok(inputtext, idx);
            EditRace[playerid][rdialogcp] = strval(msg);
            Race_ShowEditCpDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogcp], EditRace[playerid][rdialogcppage]);
            return 1;
        }
        if(strcmp(msg, "赛道描述") == 0) {
            Race_ShowEditMiaoshuDialog(playerid, EditRace[playerid][rraceid], "");
            return 1;
        }
        if(strcmp(msg, "上一页") == 0) {
            EditRace[playerid][rdialogpage]--;
            Race_ShowEditDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogpage]);
            return 1;
        }
        if(strcmp(msg, "下一页") == 0) {
            EditRace[playerid][rdialogpage]++;
            Race_ShowEditDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogpage]);
            return 1;
        }
        Race_ShowEditDialog(playerid, EditRace[playerid][rraceid], EditRace[playerid][rdialogpage]);
        return 1;
    } else {
        EditRace[playerid][rdialogpage] = 1;
        EditRace[playerid][rdialogcp] = 1;
        EditRace[playerid][rdialogcppage] = 1;
        EditRace[playerid][rdialogcplist] = -1;
        return 1;
    }
}

stock Race_GetPlayerRecordCut(const playerid) {
    
    new Cache:result, msg[128];
    // format(msg, sizeof msg, "SELECT * FROM `race_record` WHERE `UID` = %d", PlayerInfo[playerid][ID]);
    mysql_format(g_Sql, msg, sizeof msg, "SELECT COUNT(`uid`) FROM `race_record` WHERE `uid` = %d", PlayerInfo[playerid][ID]);
    result = mysql_query(g_Sql, msg);
    new cut;
    cache_get_value_index_int(0, 0, cut);
    // cut = db_num_rows(rf);
    cache_delete(result);
    return cut;
}

stock Race_GetPlayerRecord(const playerid, const raceid) //为指定玩家查询该赛道的最快个人记录 传入rid
{
    new s, msg[128], Cache:result;
    mysql_format(g_Sql, msg, sizeof msg, "SELECT * FROM `race_record` WHERE `rid` = %d AND `uid` = %d ORDER BY `p_record` LIMIT 1", raceid, PlayerInfo[playerid][ID]);
    result = mysql_query(g_Sql, msg);
    s = cache_num_rows();
    if(s==0) //如果没有的话 就返回-1
    {
        cache_delete(result);
        // db_free_result(rf);     
        return -1;
    }
    // 获取比赛完成时间tick
    new tick;
    cache_get_value_name_int(0, "p_record", tick);
    cache_delete(result);
    return tick; //返回tick
}

stock Race_InsertRecord(const playerid, const raceid,const rec_time){
    new msg[128];
    mysql_format(g_Sql, msg, sizeof msg, "INSERT INTO `race_record` VALUES (%d,%d,%d,%d)", PlayerInfo[playerid][ID], raceid, rec_time, gettime());
    mysql_pquery(g_Sql, msg);
    return 1;
}

function RaceGameEndCount(houseid, winner) { //完成赛道后倒计时
    new msg[128];
    // for (new i = 0; i < RaceHouse[houseid][rps]; i++) { //2020.8.2改 优化算法 实时根据人数去算
    for (new i = 0; i < RACE_HOUSE_MAXPLAYER; i++) { //2021.2.14 改回正常优化版本
        new player = RaceHouse[houseid][rplayers][i];
        if(player == INVALID_PLAYER_ID) continue;
        if(RaceHouse[houseid][rtimes] < 1) {
            if(GameRace[player][rgamecp] - 1 != Race[RaceHouse[GameRace[player][rgameid]][rraceid]][rcps]) {
                GameTextForPlayer(player, "~w~unFinished!", 1000, 1);
                PlayerPlaySoundEx(player, 1056);
                format(msg, sizeof(msg), "[赛车]遗憾,%s 未能完成 %s ", GetName(player), Race[RaceHouse[GameRace[player][rgameid]][rraceid]][rname]);
                Race_Game_Msg(houseid, msg);
            }
            RaceHouse[houseid][rtimes] = 0;
            Race_Game_Quit(player);
            KillTimer(RaceHouse[houseid][rendcount]);
        } else {
            //大概是显示倒计时
            format(msg, sizeof(msg), "~w~%i", RaceHouse[houseid][rtimes]);
            GameTextForPlayer(player, msg, 1000, 3);
            PlayerPlaySoundEx(player, 1056);
            // 展示给观战的人
            for (new j = GetPlayerPoolSize(); j >= 0; j--) {
                if(IsPlayerConnected(j) && PlayerInfo[j][Login])
                {
                    if(PlayerInfo[j][tvid] == player && PlayerInfo[j][tvzt]) {
                        GameTextForPlayer(j, msg, 1000, 3);
                        PlayerPlaySoundEx(j, 1056);
                    }
                }
            }
        }
    }
    // for (new i = GetPlayerPoolSize(); i >= 0; i--) {
    //     if(IsPlayerConnected(i)) {
    //         if(GameRace[i][rgameid] == houseid) {
    //             // if(RaceHouse[GameRace[i][rgameid]][rtop]-1 == RaceHouse[GameRace[i][rgameid]][rps]) 
    //             // //不能这么写 主要是……房主什么的退了之后吧 人数就会只剩他一个 就满足了条件就退出房间了直接...
    //             // //满人结束计时  注意要减个1 因为下一个进到终点的人是rtop的数字 也就是说人家还没进终点
    //             // {           
    //             //     Race_Game_Quit(i);
    //             //     KillTimer(RaceHouse[houseid][rendcount]);
    //             //     RaceHouse[houseid][rtimes] = 0;
    //             //     return 1;
    //             // }
    //             if(RaceHouse[houseid][rtimes] < 1) {
    //                 if(GameRace[i][rgamecp] - 1 != Race[RaceHouse[GameRace[i][rgameid]][rraceid]][rcps]) {
    //                     GameTextForPlayer(i, "~w~unFinished!", 1000, 1);
    //                     PlayerPlaySoundEx(i, 1056);
    //                     format(msg, sizeof(msg), "[赛车]遗憾,%s 未能完成 %s ", GetName(i), Race[RaceHouse[GameRace[i][rgameid]][rraceid]][rname]);
    //                     Race_Game_Msg(houseid, msg);
    //                 }
    //                 RaceHouse[houseid][rtimes] = 0;
    //                 Race_Game_Quit(i);
    //                 KillTimer(RaceHouse[houseid][rendcount]);
    //             } else {
    //                 //大概是显示倒计时
    //                 format(msg, sizeof(msg), "~w~%i", RaceHouse[houseid][rtimes]);
    //                 GameTextForPlayer(i, msg, 1000, 3);
    //                 PlayerPlaySoundEx(i, 1056);
    //             }
    //         }
    //     }
    // }
    RaceHouse[houseid][rtimes]--; //时间减1
    return 1;
}


// 最近100场比赛
Dialog:RecentlyRacing(playerid, response, listitem, inputtext[]) {
    if(!response) return 1;
    new idx, tmp[128];
    tmp = strtok(inputtext, idx);
    if(strcmp(tmp, "上一页") == 0) {
        // new mxpage = GetMaxPage(Race_GetPlayerRecordCut(playerid));
        // if(SelectRecentlyRacePage[playerid] == 1 || SelectRecentlyRacePage[playerid] == mxpage) return Race_ShowRecentlyRacing(playerid, 1, SelectRecentlyClicked[playerid]);
        // 如果是第一页或是最大页不进行减去页数操作
        if(SelectRecentlyRacePage[playerid] != 1) 
        {
            SelectRecentlyRacePage[playerid]--;
        }
        Race_ShowRecentlyRacing(playerid, SelectRecentlyRacePage[playerid], SelectRecentlyClicked[playerid]);
        return 1;
    }
    if(strcmp(tmp, "下一页") == 0) {
        // new mxpage = GetMaxPage(Race_GetPlayerRecordCut(playerid));
        // 2021.2.14尝试修复 点击页统计错玩家页数问题（点击其他人统计总页数还是自己的）
        new mxpage = GetMaxPage(Race_GetPlayerRecordCut(SelectRecentlyClicked[playerid]));
        // if(SelectRecentlyRacePage[playerid] == 5 || SelectRecentlyRacePage[playerid] == mxpage) return Race_ShowRecentlyRacing(playerid, mxpage, SelectRecentlyClicked[playerid]);
        // 如果是第五页（每页20条，只能显示最近100条记录）或是最大页不进行增加页数操作
        if(SelectRecentlyRacePage[playerid] != 5 && SelectRecentlyRacePage[playerid] != mxpage)
        {
            SelectRecentlyRacePage[playerid]++;
        }
        Race_ShowRecentlyRacing(playerid, SelectRecentlyRacePage[playerid], SelectRecentlyClicked[playerid]);
        return 1;
    }
    SendClientMessage(playerid, Color_White, "[举报]正在开发中...");
    return 1;
}

stock Race_ShowRecentlyRacing(const playerid, const page, const clickedplayerid) //为指定玩家显示最近比赛记录,page表示页数
{
    // format(msg, sizeof msg, "SELECT `p_record`,`rid`,`finish_time` FROM `race_record` WHERE `uid` = %d ORDER BY `finish_time` DESC LIMIT %d,%d", PlayerInfo[clickedplayerid][ID], (page-1)*20, page * 20);
    new Query[256];
    mysql_format(g_Sql, Query, sizeof(Query), "SELECT `race_record`.`rid`,`race`.`rname`,`race_record`.`p_record`,`race_record`.`finish_time` FROM `race_record` \
    join `race` on `race_record`.`rid` = `race`.`rid` WHERE `uid` = %d ORDER BY `finish_time` DESC LIMIT %d",PlayerInfo[clickedplayerid][ID], RACE_RECENTLY_LIMIT);
    mysql_pquery(g_Sql, Query, "RaceRecentlyRacingQuery", "dd", playerid, page);
    return 1;
}
function RaceRecentlyRacingQuery(playerid, page) {
    new s = cache_num_rows();
    if(s == 0) return SendClientMessage(playerid, Color_White, "[系统]暂无任何记录");
    new title[64], string[1200];
    format(title, sizeof(title), "{0088FF}最近比赛%d/%d页",page, GetMaxPage(s));
    format(string, sizeof(string), "上一页", string);
    // for (new i = 0; i < s; i++) {
    new cutPage = page * 20;
    if(cutPage > s) cutPage = s;
    for (new i = (page - 1) * 20; i < cutPage; i++) {
        // 获取比赛完成时间tick
        new tick;
        cache_get_value_name_int(i, "p_record", tick);

        new ridtemp;
        cache_get_value_name_int(i, "rid", ridtemp);

        new finish_time;
        cache_get_value_name_int(i, "finish_time", finish_time);

        new rnametemp[32];
        cache_get_value_name(i, "rname", rnametemp);

        new year, month, day, hour, minute, second;
        TimestampToDate(finish_time, year, month, day, hour, minute, second, 8);

        new time[3];
        ms2time(time, tick);
    
        // 上面改inner join吧 线程里是只能调用一次普通进程的查询的 后面就查不到数据了

        format(string, sizeof(string), "%s\n%d\t%s \t%02d:%02d:%02d [%s秒]\t%d-%d-%d %02d:%02d:%02d", string, \
        i + 1, rnametemp, time[0], time[1], time[2], MsToS(tick), year, month, day, hour, minute, second);
        // for (new i = (page - 1) * 20; i < page * 20; i++) {
        //     format(string, sizeof(string), "%s\n赛道名 %s\t\t记录 %02d:%02d:%02d [%s秒]\t\t完成日期 %d-%d-%d %02d:%02d:%02d", string, rnametemp, time[0], time[1], time[2], MsToS(tick), year, month, day, hour, minute, second);
        // }
    }
    format(string, sizeof(string), "%s\n下一页", string);
    Dialog_Show(playerid, RecentlyRacing, DIALOG_STYLE_LIST, title, string, "举报", "关闭");
    return 1;
}


stock SetupRaceTable() {
    // 初始化赛道表
    mysql_pquery(g_Sql,"CREATE TABLE IF NOT EXISTS `race`  (\
    `rid` int(10) UNSIGNED NOT NULL AUTO_INCREMENT,\
    `rname` varchar(31) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,\
    `rpassword` varchar(31) CHARACTER SET gbk COLLATE gbk_chinese_ci NOT NULL,\
    `rcreatedate` varchar(12) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,\
    `rauthor` varchar(10) CHARACTER SET gbk COLLATE gbk_chinese_ci NOT NULL,\
    `miaoshu` varchar(63) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,\
    PRIMARY KEY (`rid`) USING BTREE,\
    UNIQUE INDEX `Race_index`(`rname`, `rid`) USING BTREE\
    ) ENGINE = InnoDB CHARACTER SET = gbk COLLATE = gbk_chinese_ci ROW_FORMAT = Compact;");
    // 初始化赛道cp表
    mysql_pquery(g_Sql,"CREATE TABLE IF NOT EXISTS `race_cp`  (\
    `cpid` int(11) NOT NULL AUTO_INCREMENT,\
    `rid` int(11) NULL DEFAULT NULL,\
    `cp` int(11) NULL DEFAULT NULL,\
    `x` varchar(13) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,\
    `y` varchar(13) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,\
    `z` varchar(13) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,\
    `a` varchar(13) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,\
    `size` varchar(13) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,\
    PRIMARY KEY (`cpid`) USING BTREE,\
    INDEX `Rid_Cp_index`(`rid`, `cp`) USING BTREE\
    ) ENGINE = InnoDB CHARACTER SET = gbk COLLATE = gbk_chinese_ci ROW_FORMAT = Compact;");
    // 初始化赛道个人记录表
    mysql_pquery(g_Sql,"CREATE TABLE IF NOT EXISTS `race_record`  (\
    `uid` int(10) UNSIGNED NOT NULL,\
    `rid` int(11) NOT NULL,\
    `p_record` int(11) NOT NULL,\
    `finish_time` int(11) NOT NULL\
    ) ENGINE = InnoDB CHARACTER SET = gbk COLLATE = gbk_chinese_ci ROW_FORMAT = Compact;");
    
    // 初始化赛道触发点表
    mysql_pquery(g_Sql,"CREATE TABLE IF NOT EXISTS `race_cp_script`  (\
    `rid` int(11) NULL DEFAULT NULL,\
    `cpid` int(11) NULL DEFAULT NULL,\
    `list` int(11) NULL DEFAULT NULL,\
    `script` varchar(99) CHARACTER SET gbk COLLATE gbk_chinese_ci NULL DEFAULT NULL,\
    INDEX `Rid_Cp_Script_index`(`cpid`, `rid`, `list`) USING BTREE\
    ) ENGINE = InnoDB CHARACTER SET = gbk COLLATE = gbk_chinese_ci ROW_FORMAT = Compact;");
}


stock CreatePRaceTextDraw(const playerid) {
    // CpTextDraw[playerid] = CreatePlayerTextDraw(playerid,500, 118.000000, "~~");
    CpTextDraw[playerid] = CreatePlayerTextDraw(playerid,500, 118.000000, "C  P / ~p~0~w~/~y~0");
    PlayerTextDrawBackgroundColor(playerid, CpTextDraw[playerid], 255);
    PlayerTextDrawFont(playerid, CpTextDraw[playerid], 2);
    PlayerTextDrawLetterSize(playerid, CpTextDraw[playerid], 0.238000, 1.190000);
    PlayerTextDrawColor(playerid, CpTextDraw[playerid], -1);
    PlayerTextDrawSetOutline(playerid, CpTextDraw[playerid], 0);
    PlayerTextDrawSetProportional(playerid, CpTextDraw[playerid], 1);
    PlayerTextDrawSetShadow(playerid, CpTextDraw[playerid], 1);
    /*TextDrawUseBox(playerid, CpTextDraw[playerid], 1);
    PlayerTextDrawBoxColor(playerid, CpTextDraw[playerid], 50);*/
    PlayerTextDrawTextSize(playerid, CpTextDraw[playerid], 626.000000, 391.000000);
    PlayerTextDrawShow(playerid, CpTextDraw[playerid]);

    Time[playerid] = CreatePlayerTextDraw(playerid,500, 136.000000, "TIME / 00:00:00");
    PlayerTextDrawBackgroundColor(playerid, Time[playerid], 255);
    PlayerTextDrawFont(playerid, Time[playerid], 2);
    PlayerTextDrawLetterSize(playerid, Time[playerid], 0.238000, 1.190000);
    PlayerTextDrawColor(playerid, Time[playerid], -1);
    PlayerTextDrawSetOutline(playerid, Time[playerid], 0);
    PlayerTextDrawSetProportional(playerid, Time[playerid], 1);
    PlayerTextDrawSetShadow(playerid, Time[playerid], 1);
    /*TextDrawUseBox(playerid, Time[playerid], 1);
    TextDrawBoxColor(playerid, Time[playerid], 50);*/
    PlayerTextDrawTextSize(playerid, Time[playerid], 626.000000, 10.000000);
    PlayerTextDrawShow(playerid, Time[playerid]);

    p_record[playerid] = CreatePlayerTextDraw(playerid, 500, 154.000000, "BEST / 00:00:00");
    PlayerTextDrawBackgroundColor(playerid, p_record[playerid], 255);
    PlayerTextDrawFont(playerid, p_record[playerid], 2);
    PlayerTextDrawLetterSize(playerid, p_record[playerid], 0.238000, 1.190000);
    PlayerTextDrawColor(playerid, p_record[playerid], -1);
    PlayerTextDrawSetOutline(playerid, p_record[playerid], 0);
    PlayerTextDrawSetProportional(playerid, p_record[playerid], 1);
    PlayerTextDrawSetShadow(playerid, p_record[playerid], 1);
    PlayerTextDrawTextSize(playerid, p_record[playerid], 626.000000, 10.000000);
    PlayerTextDrawShow(playerid, p_record[playerid]);


    ///
    Top[playerid] = CreatePlayerTextDraw(playerid, 500, 172.000000, "RANK / 1 st");
    // Top[playerid] = CreatePlayerTextDraw(playerid,500, 154.000000, "房间排名 / 1 st");
    PlayerTextDrawBackgroundColor(playerid, Top[playerid], 255);
    PlayerTextDrawFont(playerid, Top[playerid], 2);
    PlayerTextDrawLetterSize(playerid, Top[playerid], 0.238000, 1.190000);
    PlayerTextDrawColor(playerid, Top[playerid], -1);
    PlayerTextDrawSetOutline(playerid, Top[playerid], 0);
    PlayerTextDrawSetProportional(playerid, Top[playerid], 1);
    PlayerTextDrawSetShadow(playerid, Top[playerid], 1);
    /*TextDrawUseBox(playerid, Top[playerid], 1);
    TextDrawBoxColor(playerid, Top[playerid], 50);*/
    PlayerTextDrawTextSize(playerid, Top[playerid], 626.000000, 10.000000);
    PlayerTextDrawShow(playerid, Top[playerid]);
}

stock DestroyPRaceTextDraw(const playerid) {
    PlayerTextDrawDestroy(playerid, CpTextDraw[playerid]);
    PlayerTextDrawDestroy(playerid, Time[playerid]);
    PlayerTextDrawDestroy(playerid, Top[playerid]);
    PlayerTextDrawDestroy(playerid, p_record[playerid]);
}


public OnPlayerEnterRaceCheckpoint(playerid) {
    // new Float:pX, Float:pY, Float:pZ;
    // if(GetPlayerVehicleID(playerid) != 0) {
    // GetPlayerPos(playerid, pX, pY, pZ);
    if(EditRace[playerid][rraceid] != -1 && EditRace[playerid][rtestcp] != -1) {
        new msg[128], raid = EditRace[playerid][rraceid];
        if(GetPlayerState(playerid) == 2) {
            Race_Cp_Script_Start(playerid, raid, EditRace[playerid][rtestcp]);
        }
        format(msg, sizeof(msg), "[赛车]赛道[%s]检查点:%i/%i", Race[raid][rname], EditRace[playerid][rtestcp], Race[raid][rcps]);
        SendClientMessage(playerid, Color_Race, msg);
        // PlayerPlaySound(playerid, 1056, pX, pY, pZ);
        PlayerPlaySoundEx(playerid, 1056);
        if(EditRace[playerid][rtestcp] == Race[raid][rcps]) {
            format(msg, sizeof(msg), "[赛车]赛道[%s]已完成", Race[raid][rname]);
            SendClientMessage(playerid, Color_Race, msg);
            EditRace[playerid][rtestcp] = -1;
            Race_HideCp(playerid);
            return 1;
        }
        EditRace[playerid][rtestcp]++;
        Race_ShowCp(playerid, raid, EditRace[playerid][rtestcp]);
        return 1;
    }
    if(GameRace[playerid][rgameid] != -1) {
        if(RaceHouse[GameRace[playerid][rgameid]][rstate] == 0) {
            new msg[128];
            if(playerid == RaceHouse[GameRace[playerid][rgameid]][rplayerid]) format(msg, sizeof(msg), "[赛车] 您是房主,可输入指令/r s开始比赛~");
            else format(msg, sizeof(msg), "[赛车]请耐心等待比赛开始..");
            SendClientMessage(playerid, Color_Race, msg);
            return 1;
        }
        if(RaceHouse[GameRace[playerid][rgameid]][rstate] == 1) {
            new msg[128];
            format(msg, sizeof(msg), "[赛车]比赛正在倒计时,请做好准备!");
            SendClientMessage(playerid, Color_Race, msg);
            return 1;
        }
        if(RaceHouse[GameRace[playerid][rgameid]][rstate] == 2) {
            new msg[128], raid = RaceHouse[GameRace[playerid][rgameid]][rraceid];
            if(GetPlayerState(playerid) == 2) {
                Race_Cp_Script_Start(playerid, raid, GameRace[playerid][rgamecp]);
            }
            if(GameRace[playerid][rgamecp] > 1 && GameRace[playerid][rgamecp] < Race[raid][rcps]) {
                new Float:x, Float:y, Float:z, Float:distance;
                new p1[racecptype], p2[racecptype];
                Race_GetCp(raid, GameRace[playerid][rgamecp], p1);
                Race_GetCp(raid, GameRace[playerid][rgamecp] + 1, p2); //没判断是否CP点>=最后一个cp点 不然返回应该是3个0 这样子在最后一个点会被判定为G
                x = floatabs(p1[rcpx] - p2[rcpx]);
                y = floatabs(p1[rcpy] - p2[rcpy]);
                z = floatabs(p2[rcpz] - p2[rcpz]);
                distance = sqrt(x * x + y * y + z * z); //x*x+y*y+z*z
                // if(GameRace[playerid][rgamecp]+1==Race[raid][rcps]) distance=GetPlayerDistanceFromPoint(playerid,p2[rcpx],p2[rcpy],p2[rcpz]);
                if(GetSpeed(playerid) < 0.01 && distance > 20.0) {
                    new ss = Race_GetCp_Scripts(p1[rcpid]); //返回当前CP点有几个高级CP函数，如果是0的话 就直接封; 如果p2还封的话就是p1了
                    if(!ss) {
                        return FuckAnitCheat(playerid, "赛车红点传送", 0); //反作弊m0d等红点传送 2020.2.20升级 防止误封
                    } else {
                        if(Race_CheckPlayerCheat(ss, p1[rcpid])) return FuckAnitCheat(playerid, "赛车红点传送", 0);
                    }
                }
            }
            format(msg, sizeof(msg), "C  P / ~p~%i~w~/~y~%i", GameRace[playerid][rgamecp], Race[raid][rcps]);
            PlayerTextDrawSetString(playerid, CpTextDraw[playerid], msg);
            // PlayerTextDrawShow(playerid, CpTextDraw[playerid]);


            // GetPlayerPos(playerid, pX, pY, pZ);

            // 展示给所有观战他的人 并且播放音效
            for (new i = GetPlayerPoolSize(); i >= 0; i--) {
                if(IsPlayerConnected(i) && PlayerInfo[i][Login]) 
                {
                    if(PlayerInfo[i][tvid] == playerid && PlayerInfo[i][tvzt])
                    {
                        PlayerTextDrawSetString(i, CpTextDraw[i], msg);
                        // PlayerPlaySound(i, 1056, pX, pY, pZ);
                        PlayerPlaySoundEx(i, 1056);
                    }
                }
            }
            //2020.2.11修复没声音

            // PlayerPlaySound(playerid, 1056, pX, pY, pZ);
            PlayerPlaySoundEx(playerid, 1056);
            //如果玩家的CP点等于这个赛道的总CP数 也就是说玩家到了终点
            if(GameRace[playerid][rgamecp] == Race[raid][rcps]) { //2020.1.13修改
                //如果rtop也就是在房间内是第一名且房间人数 > 1 也就是还有其他玩家的话
                if(GetPlayerState(playerid) == 2) {
                    // PlayerInfo[playerid][Cash] += rand;
                    new time[3];
                    new tickracetime = GetTickCount() - GameRace[playerid][rtime];
                    // 获取曾经的个人记录时间
                    new rand;
                    if(Race[raid][rcps] < 10) {
                        rand = random(80);
                    } else if(Race[raid][rcps] < 20) {
                        rand = random(800) + 300;
                    } else rand = random(1500) + 300;
                    if(time[1] == 0 && time[2] < 50) {
                        rand = random(80);
                    }
                    new p_record_time = Race_GetPlayerRecord(playerid,Race[raid][rid]);
                    // 如果从来没跑过或者是这一次比之前的个人记录快
                    if(p_record_time==-1 || tickracetime<p_record_time)
                    {
                        rand+=random(50); //突破个人记录加金币和经验值
                        SendClientMessage(playerid, Color_Race, "[赛车] 恭喜你刷新了此赛道个人记录!");
                    }
                    // 播放完赛音乐 音效为Mission passed
                    PlayerPlaySoundEx(playerid, 183);
                    // 计时器清除结束音效
                    SetTimerEx_("ClearFinishSound", 7200, 7200, 1, "d", playerid);
                    // 插入个人记录
                    // ms2time(time, GetTickCount() - GameRace[playerid][rtime]);
                    // 给玩家金币
                    GivePlayerCash(playerid, rand);
                    // if(level>201) level = 201;
                    new Float:randExp;
                    if(Race[raid][rcps] < 10) {
                        randExp = PlayerInfo[playerid][plevel]*(random(20)+10)-(PlayerInfo[playerid][plevel]-1) * random(10);
                    } else if(Race[raid][rcps] < 20) {
                        randExp = PlayerInfo[playerid][plevel]*(random(30)+15)-(PlayerInfo[playerid][plevel]-1) * random(10);
                    } else randExp = PlayerInfo[playerid][plevel]*random(50)+10-(PlayerInfo[playerid][plevel]-1) * random(10);
                    if(time[1] == 0 && time[2] < 50) {
                        randExp = PlayerInfo[playerid][plevel]*(random(20)+10)-(PlayerInfo[playerid][plevel]-1) * random(10);
                    }
                    randExp*=exp_multiple;
                    // 方便一键控制全服经验活动
                    // 给玩家经验
                    GivePlayerExp(playerid, randExp);
                    ms2time(time, tickracetime);
                    format(msg, sizeof(msg), "[赛车] %s 完成了赛道[%s],耗时%d:%d:%d [%s秒] (小组第%i) 获得 %d 金币 %.0f 经验", GetName(playerid), Race[raid][rname], time[0], time[1], time[2], MsToS(tickracetime), RaceHouse[GameRace[playerid][rgameid]][rtop], rand, randExp); //完成后显示他的排名
                    // format(msg, sizeof(msg), "[赛车] %s 完成了赛道[%s],耗时%d:%d:%d [%s秒] (小组第%i) 获得 %d 金币", GetName(playerid), Race[raid][rname], time[0], time[1], time[2], MsToS(GetTickCount() - GameRace[playerid][rtime]), RaceHouse[GameRace[playerid][rgameid]][rtop], rand); //完成后显示他的排名
                    SendClientMessageToAll(Color_Race, msg);
                    printf("%s", msg);
                    RaceHouse[GameRace[playerid][rgameid]][rtop]++; //那么接下来的人的排名就会+1
                    // KillTimer(jishu[playerid]);
                    // //									KillTimer(playerrank[GameRace[playerid][rgameid]]);
                    // TextDrawDestroy(Time[playerid]);
                    // TextDrawDestroy(Top[playerid]);
                    // new top = Race_Game_End(playerid, raid, tickracetime);
                    Race_Game_End(playerid, raid, tickracetime);
                    Race_InsertRecord(playerid, Race[raid][rid], tickracetime);
                    // new top = Race_Game_End(playerid, raid, GetTickCount() - GameRace[playerid][rtime]);
                    pRaceing[playerid] = 0;
                    // if(top != -1) {
                    //     format(msg, sizeof(msg), "[赛车] 恭喜 %s 登上了赛道 %s 的排行榜 {FFFF00}No.%i! 祝贺Ta!", GetName(playerid), Race[raid][rname], top + 1);
                    //     SendClientMessageToAll(Color_Race, msg);
                    //     printf("%s", msg);
                    //     pHouseid[playerid] = -1;
                    // }
                }
                //如果说他是第一 且 总人数  > 1
                // KillTimer(RaceHouse[GameRace[playerid][rgameid]][playerrank]);//这句话在这可能有些问题 1个人好像是没必要启用这个的.. 
                // 放在delete和race quit那里了 应该会触发的吧 
                if(RaceHouse[GameRace[playerid][rgameid]][rtop] == 2) //因为上面已经++了
                {
                    //包括玩家本身 以及观战他的都收到win胜利
                    new time[3];
                    ms2time(time, GetTickCount() - GameRace[playerid][rtime]);
                    for (new i = GetPlayerPoolSize(); i >= 0; i--) {
                        if(IsPlayerConnected(i) && PlayerInfo[i][tvid] == playerid || GameRace[i][rgameid] == GameRace[playerid][rgameid]) {
                            format(msg, sizeof(msg), "~n~~n~~n~~n~~n~~n~~n~~n~~w~%s FINISHED 1ST \n~y~%d:%d:%d [%s second]", GetName(playerid), time[0], time[1], time[2], MsToS(GetTickCount() - GameRace[playerid][rtime])); //完成后显示他的排名
                            GameTextForPlayer(i, msg, 5000, 3);
                        }
                    }
                    if(RaceHouse[GameRace[playerid][rgameid]][rps] > 1) {
                        for (new i = 0; i < 6; i++) {
                            if(RaceHouse[GameRace[playerid][rgameid]][rplayers][i] == playerid) { //找到玩家在的槽位
                                RaceHouse[GameRace[playerid][rgameid]][rplayers][i] = INVALID_PLAYER_ID;
                                break; //把这个槽位清零
                            }
                        }
                        for (new i = GetPlayerPoolSize(); i >= 0; i--) {
                            if(IsPlayerConnected(i) && i != playerid) {
                                if(GameRace[i][rgameid] == GameRace[playerid][rgameid]) GameTextForPlayer(i, "~w~玩家已冲线！你还有20秒时间，请抓紧!", 1000, 3);
                                // else GameTextForPlayer(playerid, "~w~您已冲线！请等待其他玩家冲线，请抓紧!", 1000, 3);Race_Game_Quit(playerid);
                            }
                        }
                        RaceHouse[GameRace[playerid][rgameid]][rtimes] = 20;
                        // RaceHouse[GameRace[playerid][rgameid]][rendcount] = SetTimerEx("RaceGameEndCount", 1000, true, "i", GameRace[playerid][rgameid], playerid);
                        RaceHouse[GameRace[playerid][rgameid]][rendcount] = SetTimerEx_("RaceGameEndCount", 1000, 1000, -1, "i", GameRace[playerid][rgameid], playerid);
                    }
                } else {
                    //包括玩家本身 以及观战他的都收到finished完成
                    new time[3];
                    ms2time(time, GetTickCount() - GameRace[playerid][rtime]);
                    for (new i = GetPlayerPoolSize(); i >= 0; i--) {
                        if(IsPlayerConnected(i) && PlayerInfo[i][tvid] == playerid || GameRace[i][rgameid] == GameRace[playerid][rgameid]) {
                            //或同一个房间内的人 (检测房间人数是否>1) 也显示那个人完成了
                            if(RaceHouse[GameRace[playerid][rgameid]][rtop] == 3) {
                                format(msg, sizeof(msg), "~n~~n~~n~~n~~n~~n~~n~~n~~w~%s FINISHED 2ND \n~y~%d:%d:%d [%s second]", GetName(playerid), time[0], time[1], time[2], MsToS(GetTickCount() - GameRace[playerid][rtime])); //完成后显示他的排名
                            } else if(RaceHouse[GameRace[playerid][rgameid]][rtop] == 4) {
                                format(msg, sizeof(msg), "~n~~n~~n~~n~~n~~n~~n~~n~~w~%s FINISHED 3RD \n~y~%d:%d:%d [%s second]", GetName(playerid), time[0], time[1], time[2], MsToS(GetTickCount() - GameRace[playerid][rtime])); //完成后显示他的排名
                            } else {
                                format(msg, sizeof(msg), "~n~~n~~n~~n~~n~~n~~n~~n~~w~%s FINISHED %dTH \n~y~%d:%d:%d [%s second]", GetName(playerid), RaceHouse[GameRace[playerid][rgameid]][rtop], time[0], time[1], time[2], MsToS(GetTickCount() - GameRace[playerid][rtime])); //完成后显示他的排名
                            }
                            GameTextForPlayer(i, msg, 5000, 3);
                            //2020.3.26改 仿youtube的某个赛道显示
                        }
                        // 2020.3.25发现问题 修改playerid为i
                    }
                }
                Race_ShowGameDialog(playerid, raid); //2020,2,10修改 原本是在这的
                new tmp = GameRace[playerid][rgameid];
                Race_Game_Quit(playerid);
                // Race_ShowGameDialog(i, raid); //改到了这里并直接取消tv
                if(RaceHouse[tmp][rps] == 0) //检查房间内是否还有人 有的话就检查是否还有人观战
                {
                    for (new i = GetPlayerPoolSize(); i >= 0; i--) {
                        if(IsPlayerConnected(i) && PlayerInfo[i][Login]) {
                            if(PlayerInfo[i][tvid] == playerid && i != playerid) //如果房间没人了 他还在观战则弹窗是否需要继续观看
                            {
                                AntiCommand[i] = 0;
                                CallRemoteFunction("OnPlayerCommandText", "is", i, "/tv off");
                                // OnPlayerCommandText(i, "/tv off");
                                // ShowPlayerDialog(i, RACE_MSGBOX, DIALOG_STYLE_MSGBOX, "房间", "对方当前未在赛道中，是否继续观看？", "Yes", "No");
                            }
                        }
                    }
                    //完成比赛后选择弹出dialog选择是否继续观战
                } else {
                    for (new i = GetPlayerPoolSize(); i >= 0; i--) {
                        if(IsPlayerConnected(i) && GameRace[i][rgameid] == tmp) { //如果有某个玩家的房间ID等于这个玩家刚退出来的ID则自动观看那个玩家
                            new tmped[16];
                            format(tmped, 16, "/tv %i", i);
                            AntiCommand[i] = 0;
                            CallRemoteFunction("OnPlayerCommandText", "is", playerid, tmped);
                            // OnPlayerCommandText(playerid, tmped);
                            break; //2020.3.28写 不写的话好像有问题 如果有问题再把他移了
                        }
                    }
                }
            } else //如果不是终点的话那就CP点数加1
            {
                GameRace[playerid][rgamecp]++;
                Race_ShowCp(playerid, raid, GameRace[playerid][rgamecp]); //显示CP
            }
        }
        return 1;
    }
    // }
    return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid) {
    return 1;
}



stock Race_CheckPlayerCheat(const ss, const tempcpid) { //赛道高级CP反作弊
    new tmp[128], script[RACE_CP_MAX_FUNCTION], idx;
    new flag;
    for (new i = 0; i < ss; i++) {
        flag = false;
        Race_GetCp_Script(tempcpid, i, script);
        format(script, RACE_CP_MAX_FUNCTION, "%s", script);
        tmp = strtok(script, idx); //tmp取出函数的名字
        if(Race_Cp_Script_Check(tmp) != -1) {
            //如果某个CP点列表里的高级函数 除了特定的会影响反作弊的函数
            // 其他的函数 返回true 如果整个for下来 只要有一个是会影响反作弊判断的 最终导向false
            // 如果整个for判断完了还是true 那就是挂了     
            if(!strcmp(tmp, "spawnpos", false)) flag = true; //重生坐标 如果误封再看着改咯
            if(!strcmp(tmp, "time", false)) flag = true;
            if(!strcmp(tmp, "weather", false)) flag = true;
            if(!strcmp(tmp, "msg", false)) flag = true;
            if(!strcmp(tmp, "fix", false)) flag = true;
            if(!strcmp(tmp, "damage", false)) flag = true;
            // 如果以后写了新的函数并且是不影响反作弊的函数就要往里面加
        }
        idx = 0;
    }
    if(flag) return 1;
    return 0; //返回0意味着玩家没作弊
}


stock ReSpawnRaceVehicle(playerid) { //赛道重生
    // TextDrawHideForPlayer(playerid, ReSpawningText[playerid]); //隐藏重生中的显示
    new raid = RaceHouse[GameRace[playerid][rgameid]][rraceid];
    new trcp[racecptype];
    Race_GetCp(raid, GameRace[playerid][rgamecp] - 1, trcp);
    if(GameRace[playerid][rgamecp] - 1 <= 0) Race_GetCp(raid, 1, trcp);

    new ss = Race_GetCp_Scripts(trcp[rcpid]);
    for(new i=0; i < ss; i++)
    {
        new idx;
        new script[RACE_CP_MAX_FUNCTION];
        Race_GetCp_Script(trcp[rcpid], i, script);
        format(script, RACE_CP_MAX_FUNCTION, "%s", script);
        new tmp[128];
        tmp = strtok(script, idx);
        if(strcmp(tmp, "spawnpos") == 0)
        {
            CallLocalFunction("RaceCpScript_func_spawnpos", "iiis", playerid, raid, GameRace[playerid][rgamecp] - 1, script);
            return 1;
        }
    }

    // 到时候去获取上一个CP点的高级函数是否有spawnpos有的话就执行设置坐标
    SetPlayerPos(playerid, trcp[rcpx], trcp[rcpy], trcp[rcpz]);
    if(PlayerInfo[playerid][BuyID] != 0) //如果玩家有车的话重生车
    {
        SetVehiclePos(PlayerInfo[playerid][BuyID], trcp[rcpx], trcp[rcpy], trcp[rcpz]);
        SetVehicleZAngle(PlayerInfo[playerid][BuyID], trcp[rcpa]); //2020.1.12写 修复 赛道CP点重生后车辆的朝向角度不是CP点的朝向角度
        SetVehicleVirtualWorld(PlayerInfo[playerid][BuyID], 6666 - GameRace[playerid][rgameid]);
        PutPlayerInVehicle(playerid, PlayerInfo[playerid][BuyID], 0);
        LinkVehicleToInterior(PlayerInfo[playerid][BuyID], GetPlayerInterior(playerid));
        AddVehicleComponent(PlayerInfo[playerid][BuyID], 1010); //给车一个氮气
    }
    return 1;
}


// 清除完赛后音效
function ClearFinishSound(const playerid){
    PlayerPlaySoundEx(playerid, 0);
    return 1;
}
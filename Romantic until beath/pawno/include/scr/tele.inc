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

//  相信我，能在开源里写鸣谢前辈的人的人真的是少之又少。
//  本服修改自5F，Prace,PHouse,Goods等开源 特别鸣谢：[Ghost]Rui ushio_p [Ghost]Dylan
//  致敬自由居民区开源,制作者:[ITC]dyq  [ITC]fangye  [ITC]Super_wlc [ITC]RR_LXD  mk124  Shindo(aka. ssh)  vvg, yezizhu(aka. yzz)
//  特别鸣谢 ryddawn 技术及OBJ指导；[Fire]KiVen JoshenKM
//  特别鸣谢 GoodsSys Episodes KiVen juse
//  特别鸣谢 那些始终支持并使用Shoebill的人。  
//  特别鸣谢 GTAUN GTABBS 
// RST团队服务器开发主要由[R_ST]Hygen（YuCarl77）领导

#define MAX_TRANSFER        	500//最大传送点500个 //默认10000个
// #define TeleListDialog			714 //700是广告牌的 占用一个714
stock GetPlayerFacingAngleEx(const playerid, & Float:ang) {
    if(GetPlayerVehicleID(playerid) == 0) {
        GetPlayerFacingAngle(playerid, ang);
    } else {
        GetVehicleZAngle(GetPlayerVehicleID(playerid), ang);
    }
}
enum maketype {
    mbuff,
    Float:mx,
    Float:my,
    Float:mz,
    Float:ma,
    mname[128]
};
enum makesystype {
    mbuff,
    Float:mx,
    Float:my,
    Float:mz,
    Float:ma,
    mname[128],
    tdescribe[48]
};
new vmake[MAX_TRANSFER][maketype], TransferColor = 0xFFFF00FF;
new vsysmake[MAX_TRANSFER][makesystype];
new TelePage[MAX_PLAYERS];
stock pVmakePos(const playerid, const id, const text[]) {
    new str[128];
    format(str, sizeof(str), "[传送] 你创建了一个'//%s'的传送点", text);
    SendClientMessage(playerid, TransferColor, str);
    format(vmake[id][mname], 128, "%s", text);
    vmake[id][mbuff] = 1;
    GetPlayerPos(playerid, vmake[id][mx], vmake[id][my], vmake[id][mz]);
    GetPlayerFacingAngleEx(playerid, vmake[id][ma]);
    new File:m = fopen("Teleport/Transfer.pos", io_append), info[1024];
    format(info, 1024, "%s %f %f %f %f\r\n", vmake[id][mname], vmake[id][mx], vmake[id][my], vmake[id][mz], vmake[id][ma]);
    make_fwrite(m, info);
    fclose(m);
    return 1;
}
stock pVmakeSysPos(const playerid, const id, const text[], const describe[]) {
    new str[128];
    format(str, sizeof(str), "[传送] 你创建了一个'/%s'的系统传送点", text);
    SendClientMessage(playerid, TransferColor, str);
    format(vsysmake[id][mname], 128, "%s", text);
    vsysmake[id][mbuff] = 1;
    GetPlayerPos(playerid, vsysmake[id][mx], vsysmake[id][my], vsysmake[id][mz]);
    GetPlayerFacingAngleEx(playerid, vsysmake[id][ma]);
    format(vsysmake[id][tdescribe], 48, "%s", describe);
    new File:m = fopen("Teleport/SysTransfer.pos", io_append), info[1024];
    format(info, 1024, "%s %f %f %f %f %s\r\n", vsysmake[id][mname], vsysmake[id][mx], vsysmake[id][my], vsysmake[id][mz], vsysmake[id][ma], vsysmake[id][tdescribe]);
    make_fwrite(m, info); //有问题在把\r\n换到最后
    fclose(m);
    format(str, sizeof(str), "您现在位于%s 指令/%s\n坐标%f,%f,%f", vsysmake[id][tdescribe], vsysmake[id][mname], vsysmake[id][mx], vsysmake[id][my], vsysmake[id][mz]);
    CreateDynamic3DTextLabel(str, 0x2BA2D5C8, vsysmake[id][mx], vsysmake[id][my], vsysmake[id][mz], 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, -1, -1, STREAMER_3D_TEXT_LABEL_SD, -1, 0);
    return 1;
}
stock Initialize_Transfer() {
    print("[提示]加载玩家传送");
    if(fexist("Teleport/Transfer.pos") == 1) {
        new File:mk = fopen("Teleport/Transfer.pos", io_read), mid, makeinfo[1024], name[128], idx = 0, loads = 0;
        while (fread(mk, makeinfo)) {
            format(name, 128, "%s", strtok(makeinfo, idx));
            mid = make_getindex();
            if(mid == -1) {
                printf("[提示]玩家自制传送点载入[%s]错误,没有空余makeid.", name);
            } else {
                format(vmake[mid][mname], 128, "%s", name);
                vmake[mid][mx] = floatstr(strtok(makeinfo, idx));
                vmake[mid][my] = floatstr(strtok(makeinfo, idx));
                vmake[mid][mz] = floatstr(strtok(makeinfo, idx));
                vmake[mid][ma] = floatstr(strtok(makeinfo, idx));
                vmake[mid][mbuff] = 1;
                loads++;
            }
            idx = 0;
        }
        fclose(mk);
        printf("[提示]读取传送[%d]个,剩余可用传送[%d]个", loads, MAX_TRANSFER - loads);
    }
    return 1;
}
stock Initialize_SysTransfer() {
    print("[提示]加载系统传送");
    if(fexist("Teleport/SysTransfer.pos") == 1) {
        new File:mk = fopen("Teleport/SysTransfer.pos", io_read), mid, makeinfo[1024], name[128], idx = 0, loads = 0;
        while (fread(mk, makeinfo)) {
            format(name, 128, "%s", strtok(makeinfo, idx));
            mid = make_sysgetindex();
            if(mid == -1) {
                printf("[警告]系统自制传送点载入[%s]错误,已达上限. ", name);
            } else {
                format(vsysmake[mid][mname], 128, "%s", name);
                vsysmake[mid][mx] = floatstr(strtok(makeinfo, idx));
                vsysmake[mid][my] = floatstr(strtok(makeinfo, idx));
                vsysmake[mid][mz] = floatstr(strtok(makeinfo, idx));
                vsysmake[mid][ma] = floatstr(strtok(makeinfo, idx));
                //创建地图上的3D显示
                format(vsysmake[mid][tdescribe], 48, "%s", strtok(makeinfo, idx));
                vsysmake[mid][mbuff] = 1;
                loads++;
                new str[128] = "";
                //2020.2.4新增 大世界 世界ID0 显示3D文字
                format(str, sizeof(str), "您现在位于%s 指令/%s\n坐标%f,%f,%f", vsysmake[mid][tdescribe], vsysmake[mid][mname], vsysmake[mid][mx], vsysmake[mid][my], vsysmake[mid][mz]);
                // Create3DTextLabel(str, 0x1EA7E1C8, vsysmake[mid][mx], vsysmake[mid][my], vsysmake[mid][mz], 40.0, 0, 0);
                //改用流光创建3D文字
                CreateDynamic3DTextLabel(str, 0x2BA2D5C8, vsysmake[mid][mx], vsysmake[mid][my], vsysmake[mid][mz], 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, -1, -1, STREAMER_3D_TEXT_LABEL_SD, -1, 0);
            }
            idx = 0;
        }
        fclose(mk);
        printf("[提示]读取系统传送[%d]个,剩余可用传送[%d]个 <<<<<<", loads, MAX_TRANSFER - loads);
    }
    return 1;
}
stock make_fwrite(File:handle, const text[]) {
    new l = strlen(text);
    for (new i = 0; i < l; i++) {
        fputchar(handle, text[i], false);
    }
}
stock make_getindex() {
    new s = -1;
    for (new i = 0; i < MAX_TRANSFER; i++) {
        if(vmake[i][mbuff] == 0) {
            s = i;
            i = MAX_TRANSFER + 1;
        }
    }
    return s;
}
stock make_sysgetindex() {
    new s = -1;
    for (new i = 0; i < MAX_TRANSFER; i++) {
        if(vsysmake[i][mbuff] == 0) {
            s = i;
            i = MAX_TRANSFER + 1;
        }
    }
    return s;
}
stock make_findgo(const name[]) {
    new s = -1;
    for (new i = 0; i < MAX_TRANSFER; i++) {
        if(vmake[i][mbuff] == 1) {
            if(strcmp(vmake[i][mname], name, true) == 0) {
                s = i;
                i = MAX_TRANSFER + 1;
            }
        }
    }
    return s;
}
stock make_Sysfindgo(const name[]) {
    new s = -1;
    for (new i = 0; i < MAX_TRANSFER; i++) {
        if(vsysmake[i][mbuff] == 1) {
            if(strcmp(vsysmake[i][mname], name, true) == 0) {
                s = i;
                i = MAX_TRANSFER + 1;
            }
        }
    }
    return s;
}

stock Tele_OnPlayerLogin(const playerid){
	TelePage[playerid] = 1;
}

stock Tele_ShowListDialog(const playerid, const page) //为指定玩家显示赛道选择菜单,page表示赛道页数
{
    new title[64], msg[1024], maxpage;
    maxpage = make_sysgetindex() / 10;
	if(maxpage % 10 != 0) maxpage++;
    format(title, 64, "系统传送 - %i/%i", page, maxpage);
    format(msg, sizeof(msg), "传送点\t描述\tID\n", msg);
    if(page > 1 && page == maxpage) format(msg, sizeof(msg), "%s\n上一页", msg);
    if(page == 1 && page != maxpage) format(msg, sizeof(msg), "%s\n下一页\n", msg);
    if(page > 1 && page < maxpage) format(msg, sizeof(msg), "%s\n上一页\n下一页", msg);
    for (new i = (page - 1) * 10; i < page * 10; i++) {
        if(vsysmake[i][mbuff] == 1) { //如果存在的话
    	format(msg, sizeof(msg), "%s\n%s\t%s\t%d\n", msg, vsysmake[i][mname], vsysmake[i][tdescribe], i + 1);
        }
    }
    Dialog_Show(playerid, TeleListDialog, DIALOG_STYLE_TABLIST_HEADERS, title, msg, "传送", "关闭");
    return 1;
}

Dialog:TeleListDialog(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        new msg[128], idx;
        msg = strtok(inputtext, idx);
        if(strcmp(msg, "上一页") == 0) {
            TelePage[playerid]--;
            Tele_ShowListDialog(playerid, TelePage[playerid]);
            return 1;
        }
        if(strcmp(msg, "下一页") == 0) {
            TelePage[playerid]++;
            Tele_ShowListDialog(playerid, TelePage[playerid]);
            return 1;
        }
        format(msg, sizeof msg, "/%s", inputtext);
        AntiCommand[playerid] = 0;
        CallRemoteFunction("OnPlayerCommandText", "is", playerid, msg);
        // OnPlayerCommandText(playerid, msg);
        return 1;
    }
    return 1;
}
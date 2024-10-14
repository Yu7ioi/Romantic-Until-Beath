//问答系统

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


#define MAX_QA		100
#define QAPosition  "QA/questionAnswer.cfg"

enum qatype {
    Questions[256],
        Answers[256]
};
new QA_Info[MAX_QA][qatype], QA_Count, QA_Timer[4];
stock Initialize_QA() {
    for (new i = 0; i < MAX_QA; i++) {
        format(QA_Info[i][Questions], 2, "0");
        format(QA_Info[i][Answers], 2, "0");
    }
    QA_Count = 0; //问答总数量
    QA_Timer[0] = 0; //计算运行了多少分钟
    QA_Timer[1] = 2; //几分钟一次问答 最低1分钟
    QA_Timer[2] = 0; //计算到第几个问题了
    QA_Timer[3] = 0; //开启问答 1 = 开, 0 = 关
    print("[提示]问答系统初始化");
    if(fexist(QAPosition) == 1) {
        new File:m = fopen(QAPosition, io_read), info[256], msg[256], idx = 0;
        while (fread(m, info)) {
            msg = strtok(info, idx);
            if(mk_strcmp(msg, "qa") == 0) Common_AddQA(info);
            idx = 0;
        }
        fclose(m);
        printf("[问答]共加载[%d/%d]个问答.", QA_Count, MAX_QA);
    } else {
        printf("未找到问答系统(questionAnswer.cfg from questionAnswer.inc)");
    }
}
stock Common_AddQA(const info[]) {
    new id = -1;
    for (new i = 0; i < MAX_QA; i++) {
        if(strlen(QA_Info[i][Questions]) < 2) {
            id = i;
            i = MAX_QA + 1;
        }
    }
    if(id == -1) {
        printf("问答已达到上限(MAX_QA from questionAnswer.inc)");
    }
    new msg[512], idx;
    msg = strtok(info, idx);
    // printf("%s", strtok(info, idx));
    new question[256];
    question = strtok(info, idx);
    new answer[256];
    answer = strtok(info, idx);
    format(QA_Info[id][Questions], 256, question);
    format(QA_Info[id][Answers], 256, answer);
    printf("[问答]问题%s? 答案[%s]",QA_Info[id][Questions],QA_Info[id][Answers]);
    QA_Count++;
}
stock Common_Running_QA() {
    if(QA_Count == 0) return 1;
    QA_Timer[0]++;
    if(QA_Timer[0] >= QA_Timer[1]) {
        QA_Timer[0] = 0;
        // if(QA_Timer[2] >= QA_Count - 1) {
        if(QA_Timer[2] >= QA_Count) { //不减1的话好像抽不到最后一个题 2020.3.2.1014尝试修复
            QA_Timer[2] = 0;
        }
        else  {
            new msg[256];
            format(msg, sizeof msg, "[小提示] 上一问[%s]? 答案 [%s]", QA_Info[QA_Timer[2]][Questions],QA_Info[QA_Timer[2]][Answers]);
            SendClientMessageToAll(0x22DDB8C8, msg);
            // 2020.3.1 新增大量怀旧问答并支持小提示了 修改了问答的频率
            // QA_Timer[2]++; 原来是按顺序来的 我们直接随机
            QA_Timer[2] = random(QA_Count); //本来是Qa_Count+1的 如果还出现0?这种情况可能要去掉了
            QA_Timer[3] = 1;
            format(msg, sizeof msg, "[我问你答] %s?", QA_Info[QA_Timer[2]][Questions]);
            SendClientMessageToAll(0x22DDB8C8, msg);
        }
    }
    return 1;
}
stock Common_Answer_QA(const playerid, const answer[]) {
    if(QA_Timer[3] == 1 && mk_strcmp(answer, QA_Info[QA_Timer[2]][Answers],false) == 0) {
        QA_Timer[3] = 0;
        new msg[256], money = random(300);
        PlayerInfo[playerid][Cash] += money;
        format(msg, sizeof msg, "[我问你答] 恭喜 %s(%d) 回答正确. {00FF40}$%d", GetName(playerid), playerid, money);
        SendClientMessageToAll(Color_Yellow, msg);
    }
}
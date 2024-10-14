/*
 /\\\\\\\\\\     /\\\\\\\\\\ /\\\\\\\\\\\\
 \/\\\____/\\\  /\\\_______/ \/___/\\\___/
  \/\\\   \/\\\ \/\\\             \/\\\
   \/\\\\\\\\/   \/\\\\\\\\\\      \/\\\
    \/\\\  \/\\\  \/_______/\\\     \/\\\
     \/\\\   \/\\\         \/\\\     \/\\\
      \/\\\   \/\\\  /\\\\\\\\\/      \/\\\
       \/_/    \/_/  \/_______/        \/_/
       
								The MIT License  
								
	Copyright (c) <2019-2021> <YuCarl77>  
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
	��Ȩ(c) <2019-2021> <YuCarl77>   

	ʹ�ø����֤���������������Ȩ�ޣ���ѣ��κ��˿��Եõ���������������ĵ���һ��������
	���Ҿ�Ӫ����������κ����ƣ����������Ƶ�ʹ�á����ơ��޸ġ��ϲ������桢���С����Ŵ���֤�顢���߳��۸�����Ŀ�����Ȩ����
	ͬʱ��������Щ������û�������ЩȨ����ʹ����������������  
	
	���ϵİ�Ȩ֪ͨ��Ȩ��֪ͨӦ�ð��������и�����Ŀ����л������������������ʵ�����С�
	
	������������������ṩ��û���κ���ʽ�ĵ�������������ȷ�ػ��߰����ģ�������Щ���ǲ�����ҵ���ʵĵ��������ơ�
	�ʺ�һ���ض�����;���Ҳ����ַ������ߺͰ�Ȩ���������κγ��϶�ʹ�ø�����漰���κ�Ҫ���𺦻����������ζ���Ӧ����
	�����������������û���ֻ�Ǻ�ͬ��ʽ��������Ȩ����������ʽ�������������������÷�Χ�ڡ�����������ϵ���������ʹ�û����������������������Ϊ��  
	=====================================   
*/

// RaceSpeedTime
// ��Ȩ���� 2021 RaceSpeedTime(YuCarl77) ��������Ȩ����
// RaceSpeedTime������ �ĵ����벻�� 5F(���ɾ�����) ��Դ��Ŀ�Լ�������Դ�ű�/��������ߡ�
// ������鿴Credits
#include "common/Credits"




#include <a_samp>
// #include "ASAN" //����������    �ܶ�����õ�mk124��NPatcher ����ûlinux�汾
#define FIX_strcmp 0 //��ΪҪ��mk124��strcmp���Ծ͹ر����޸�samp�Դ�һЩ�����inc�����������
#define FIX_PutPlayerInVehicle 0 //��Ȼ�Ļ�������ը��
#define FIX_KEY_AIM 0
#define FIX_GogglesSync 0
#define FIX_OnPlayerSpawn 0
#define FIX_GetServerVarAsString 0
#define FIX_GetServerVarAsFloat  0
#define FIX_GetServerVarAsInt    0
#define FIX_GetServerVarAsBool   0
#include <common/fixes> //��Ҫ����a_samp����
//https://github.com/pawn-lang/sa-mp-fixes
//https://wiki.sa-mp.com/wiki/Fixes.inc#Expansion 
//�޸�SAMP�������Դ���һЩ����

#include <common/main>
#include <common/tele>
#include <common/HRace>
#include <common/PHouse>
#include <common/Goods_Sys>
#include <common/questionAnswer>
#include <common/npc>
#include <common/AntiCheat/Rogue-AC>//������
#include <common/Attire> //2020.2.29����
#include <common/billboard> //2020.3.1����
//#include <common/Cars> //2020.3.3����
#include <common/Team> //2020.3.14д
#include <common/camera> //2020-3-15����16��д�ľ�ͷ
#include <common/DeathMatch> // 2020.3.16����
#include <common/World> // 2020.3.28�ϲ� ����
#include <common/sampmailjs>//������֤
#include <common/Gps_system>//GPS
#include <common/Monument>//2021.9.2 �����
// #include <crashdetect> //���Ա���
#define strlen mk_strlen
#define strcmp mk_strcmp
#define strfind mk_strfind
#define MailDialogContent "\n��ѡ�����������\n��������\n������֤\n�޸�����\n�޸��û���\n"
// #define PlayerInfoDialog 1300
// #define PRESSED(%0) \ (((newkeys & ( % 0)) == ( % 0)) && ((oldkeys & ( % 0)) != ( % 0)))
#define HOLDING(%0) \
((newkeys & ( % 0)) == ( % 0))
#pragma dynamic 30000 

//��ջ���� Stack/heap size:16384 bytes; estimated max. usage:unknown, due to recursion 
// ����һֱû���Ż��� ��Ҫ�����ܼ��ٱ����ĳ��� �ر���ȫ�ֱ���   �ַ��Ͳ�Ҫ����1024
// �����������߸���15W��ջ �Ҿ��û���̫���� 30000�����ȥ12W ���
// Ĭ��ջ��16384bytes �����16KB��

// #pragma compat 1 //2020.2.15���� �±�����pawncc https://github.com/pawn-lang/compiler/ ����ģʽ

/*
 SetTimer_
 * ����SetTimer�ĸĽ��汾��
 * ����1:func ��������������.
 * ����2:interval ��������룩.
 * ����3:delayTime ��һ�ε��������ʱ���ӳ�ʱ�䣨���룩
 * ����4:count ������ɱ��֮ǰӦ���ظ����ٴ�(-1��ʾ����)��
 * ����ֵ ��ʱ����ID

 SetTimerEx_
 * ����SetTimerEx�ĸĽ��汾��
 * ����1:func ��������������.
 * ����2:interval ��������룩.
 * ����3:delayTime ��һ�ε��������ʱ���ӳ�ʱ�䣨���룩
 * ����4:count ������ɱ��֮ǰӦ���ظ����ٴ�(-1��ʾ����)��
 * ����5:format ָʾ��ʱ�������ݵ�ֵ�����͵������ʽ��
 * ����ֵ ��ʱ����ID
 */

new g_MysqlRaceCheck[MAX_PLAYERS];
new StopTimer[4];
public OnQueryError(errorid, const error[], const callback[], const query[], MySQL:handle) {
    print("-----------------------------------");
    printf("Error ID:%d, Error:%s", errorid, error);
    printf("Callback:%s", callback);
    printf("Query:%s", query);
    print("-----------------------------------");
    return 1;
}

// #include <YSI\y_ini>

//�޵�ʱ�����
// new Text3D:NoDieTime[MAX_PLAYERS];
// PlayerText:LetterForYou[MAX_PLAYERS][4]


AntiDeAMX() {
    new a[][] = {
        "Unarmed (Fist)",
        "Brass K"
    };
    #pragma unused a
}

// new StopSecondsTimer = -1, StopGATimer = -1, StopMinuteTimer = -1, StopupdateSpeedometer = -1; //���ʱ�� �����Զ��޳� �����жϵ� ϵͳ��ʱ��



// stock IsNumeric(const string[]) {
//     for (int i = 0, j = strlen(string); i < j; i++)
//         if(string[i] > '9' || string[i] < '0')
//             return 0;
//     return 1;
// }

new dinfobj[MAX_PLAYERS], jd[MAX_PLAYERS], wy[MAX_PLAYERS], splp[MAX_PLAYERS];
//�Ƿ񾯵�,����OBJ,β��OBJ,�Ƿ񱣴�/s ����ɫ

enum ppctype {
    Float:ppX,
    Float:ppY,
    Float:ppZ,
    Float:ppR
};


//������new��
static PPC_SpawnPos[][ppctype] = {
    {
        -1776.967285,
        576.601135,
        234.617691,
        118.473152
    },
    {
        -1806.211547,
        524.291259,
        234.617675,
        356.940093
    },
    {
        -1835.493408,
        576.112365,
        234.615264,
        242.834381
    }
    // {
    //     -1790.714599,
    //     552.736389,
    //     234.615280,
    //     322.741027
    // },
    // {
    //     -1808.469238,
    //     579.068603,
    //     234.617675,
    //     88.461364
    // },
    // {
    //     -1773.020874,
    //     578.919433,
    //     234.617996,
    //     103.577575
    // },
    // {
    //     -1804.983642,
    //     519.567504,
    //     234.618896,
    //     355.549987
    // },
    // {
    //     -1840.326049,
    //     578.858642,
    //     234.614852,
    //     237.597930
    // },
    // {
    //     -1820.136474,
    //     545.843017,
    //     234.614517,
    //     201.193878
    // }
};

new p_PPC[MAX_PLAYERS], p_ppcCar[MAX_PLAYERS];

// static Float:RandomCameraLookAt[][6] = {
//     //��ͷ����:x,y,z ע������:x,y,z
//     {
//         2005.628173,
//         1161.630615,
//         96.700424,
//         2186.356201,
//         1112.973510,
//         26.706451
//     },
//     {
//         1471.778564,
//         -941.493041,
//         117.865547,
//         1412.722290,
//         -809.165771,
//         80.828323
//     },
//     {
//         -2298.547851,
//         2200.615478,
//         70.029342,
//         -2417.087890,
//         2310.907470,
//         1.660755
//     },
//     {
//         -2565.568847,
//         1407.917358,
//         120.223480,
//         -2663.229248,
//         1587.976440,
//         109.718269
//     },
//     {
//         726.227416,
//         -1640.945312,
//         27.079547,
//         764.914245,
//         -1655.571289,
//         4.716124
//     }
// };
// new CountDown = -1; //����ʱ
new Count[MAX_PLAYERS], Timer[MAX_PLAYERS];
new bool:ServerPlayingMusic = false;
// new wdzt[MAX_PLAYERS];

new RandMsg;
//ԭ����new ANNOUNCEMENTS[7][128] = {
static ANNOUNCEMENTS[7][] = {
    "[�㲥]:����ֿ�,�ֿڹ���Ա����Ե�Ӵ! ",
    "[�㲥]:����ָ���/help ��������/sz ",
    "[�㲥]:��Ҫ���˰�Tab�����Բ鿴��ǰ�����! ",
    "[�㲥]:�����з�����ʧ���³���/kill���Զ�������",
    "[�㲥]:��������ֱ�ӷ�ɱ!������Ϸ�����������ף� ",
    "[�㲥]:���⵷�һᱻ����Ա����������Ӵ! ",
    "[�㲥]:������Ϸ����R�����ֿڣ�"
};

function GlobalAnnouncement() {
    switch (RandMsg) {
        case 0:{
            SCMALL(Color_Announcement, ANNOUNCEMENTS[0]);
            new year = 0, month = 0, day = 0;
            getdate(year, month, day);
            if(year == 2020 && month <= 3 && day <= 15) SendRconCommand("hostname RST�Ŷӹٷ���������һ��Ϊ�人����!");
            // else SendRconCommand("hostname {****�� �� �� ��****}RST�Ŷӹٷ�������2020");
            RandMsg++;
        }
        case 1:{
            SCMALL(Color_Announcement, ANNOUNCEMENTS[1]);
            //SendRconCommand("hostname RST�Ŷӹٷ�����������л�Ȱ�GTA������ ��");
            RandMsg++;
        }
        case 2:{
            SCMALL(Color_Announcement, ANNOUNCEMENTS[2]);
            //SendRconCommand("hostname [����֮ʱ] ��ƽ̨������ͨ������|����|����|������|����|�Ҿߡ�");
            RandMsg++;
        }
        case 3:{
            SCMALL(Color_Announcement, ANNOUNCEMENTS[3]);
            // SendRconCommand("hostname Race Speed Time ����֮ʱ 2020");
            RandMsg++;
        }
        case 4:{
            SCMALL(Color_Announcement, ANNOUNCEMENTS[4]);
            //SendRconCommand("hostname RST�Ŷӷ�������Bվ����RaceSpeedTime ��ƽ̨������ͨ(�ƶ�����2.0�����ϰ汾)");
            RandMsg++;
        }
        case 5:{
            SCMALL(Color_Announcement, ANNOUNCEMENTS[5]);
            //SendRconCommand("hostname RST�Ŷӹٷ�������������֮ʱ����ƽ̨������ͨ(�ƶ�����2.0�����ϰ汾)");
            RandMsg++;
        }
        case 6:{
            SCMALL(Color_Announcement, ANNOUNCEMENTS[6]);
            //SendRconCommand("hostname RST����֮ʱ - 2021 [��������������������]");
            RandMsg++;
        }
        case 7:{
            //SCMALL(Color_Announcement, ANNOUNCEMENTS[2]);
            // SendRconCommand("hostname RST�Ŷӹٷ���������24/7������...��");
            RandMsg = 0;
        }
    }
    return 1;
}

function _KickPlayerDelayed(playerid) {
    Kick(playerid);
    return 1;
}

function BansEx(playerid) {
    Ban(playerid);
    return 1;
}
main() {}
public OnGameModeInit() {
    AntiDeAMX();
    // SetGameModeText("����|����|����|����");
    SetGameModeText(GMText);

    g_Sql = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASS, MYSQL_DB);
    new errno = mysql_errno(g_Sql);
    if(g_Sql == MYSQL_INVALID_HANDLE || errno != 0) {
        new error[100];
        mysql_error(error, sizeof(error), g_Sql);
        printf("[ERROR] #%d '%s'", errno, error);
        print("[MySQL]�޷����ӵ�MYSQL������");
        SetGameModeText("Error - ���ݿ��쳣");
        new rand = random(89999) + 10000;
        new string[64];
        format(string, sizeof(string), "password %d", rand);
        SendRconCommand(string);
        SendRconCommand("hostname [����]RaceSpeedTime���ݿ����ʧ��,����ϵ����Ա");
        return 1;
    }
    // ����mysql����
    mysql_set_charset(MYSQL_charset);

    print("[��ʾ]NPC������...");
    SetNameTagDrawDistance(200); //ԭ����70 ��������������ʾ��ҵ����ơ�
    // Streamer_SetTickRate(15 ); //Ĭ��50��������tickrate
    ShowPlayerMarkers(1);
    ShowNameTags(1);
    UsePlayerPedAnims();
    // AddPlayerClass(0, 1958.3782, 1343.1572, 15.3746, 269.1424, 0, 0, 0, 0, 0, 0);

    // Race_Load();
    Initialize_Main();
    Initialize_SysTransfer();
    Initialize_Transfer();
    Initialize_QA();
    Initialize_LocalObjects(); //����OBJ���� ��addobjects
    Initialize_PHouse(); //Phouse����
    Initialize_Attire(); //װ�����
    Initialize_Cars(); //��������
    Initialize_Boards(); //�����Ƽ���
    Race_OnFilterScriptInit(); //��������
    Initialize_DeathMatch(); //����DM
    Initialize_Team(); //��ʼ���Ŷ�
    LoadNpcs(); //����NPC
    Initialize_Camera(); //��ʼ������� 2020-3-15 16:41:07
    InitGoods(); //��ʼ���Ҿ�.
    Initialize_Monument(); //��ʼ�������
    // ����Timers
    // StopGATimer = SetTimer_("GlobalAnnouncement", 250000, true); //���ع����ʱ
    // StopMinuteTimer = SetTimer_("MinuteTimer", 60000, true); //���Ӽ�ʱ��
    // StopupdateSpeedometer = SetTimer_("updateSpeedometer", 100, true); //�ٶȱ��ʱ��
    // StopSecondsTimer = SetTimer_("SecondsTimer", 1000, true); //���ʱ��
    // ��Ϊ�ò���KillTimer�� ���ԾͲ�д��Щ��������;
    StopTimer[0] = SetTimer_("GlobalAnnouncement", 250000, 250000, -1); //���ع����ʱ
    StopTimer[1] = SetTimer_("MinuteTimer", 60000, 60000, -1); //���Ӽ�ʱ��
    StopTimer[2] = SetTimer_("updateSpeedometer", 200, 200, -1); //�ٶȱ��ʱ��
    StopTimer[3] = SetTimer_("SecondsTimer", 1000, 1000, -1); //���ʱ��
    // sqlconnect = mysql_connect("127.0.0.1","�û���","����","����");

    // ������֤�õı�����players
    // ��Ҫ�Ѽ���php���� ��php�ļ����µļ���ȥ php�ļ�������Ҫ�޸��û���������ĵط�
    // if(mysql_connect("127.0.0.1","root","sampemail","123456") == 0)
    // {
    //     print("[����]������֤���ݿ�����ʧ��[�뼰ʱ�޸�.]");
    //     // print("[����]������֤���ݿ�����ʧ��[�뼰ʱ�޸�,�����޷�����������.]");
    // 	// SendRconCommand("exit");
    // }
    // else
    // {
    // 	print("[��ʾ]������֤���ݿ����ӳɹ�!���������������������뿪����ط���.");
    // }
    // mysql_set_charset("gbk");
    // mysql_debug(1);
    print("-------------------------------------------------");
    print("�����޸���5F��Դ��Prace��Դ �¾���ԴPrace�������ߣ�[Ghost]Rui ushio_p [Ghost]Dylan");
    print("�¾����ɾ�������Դ,������:[ITC]dyq  [ITC]fangye  [ITC]Super_wlc [ITC]RR_LXD  mk124  Shindo(aka. ssh)  vvg, yezizhu(aka. yzz)");
    print("�ر���л ryddawn ������OBJָ��:[Fire]KiVen JoshenKM");
    print("Powered By RaceSpeedTime");
    print("-------------------------------------------------");
    return 1;
}
public OnGameModeExit() { //print("[��ʾ]�������ر�/����");
    // SCMALL(Color_Red, "[ϵͳ] ����ˢ�·����������ڱ������������...");
    // for (StopMinuteTimeri = GetPlayerPoolSize(); i >= 0; i--) {
    //     if(IsPlayerConnected(i)) OnPlayerDisconnect(i,1); // We do that so players wouldn't lose their data upon server's close.
    // }
    // KillTimer(StopGATimer);
    // KillTimer(StopMinuteTimer);
    // KillTimer(StopupdateSpeedometer);
    // KillTimer(StopSecondsTimer);
    KillTimer_(StopTimer[0]);
    KillTimer_(StopTimer[1]);
    KillTimer_(StopTimer[2]);
    KillTimer_(StopTimer[3]);

    UnLoadNpcs(); //ж��NPC
    Destroy_Monument(); //���ټ����
    Boards_OnGameModeExit(); //������ж��
    Attire_OnGameModeExit(); //װ��ж��
    Cars_OnGameModeExit(); //����ж��
    // Team_OnGameModeExit(); //�Ŷ�ж��
    // db_close(Racedb); //PRaceж��
    db_close(pHouseData); //print("[PHouse]ж��");
    // db_close(user); //�ر��û����ݿ�
    for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++) {
        if(IsPlayerConnected(i)) {
            // reason is set to 1 for normal 'Quit'
            OnPlayerDisconnect(i, 1);
        }
    }
    mysql_close(g_Sql); //�ر�mysql���ݿ�
    SCMALL(Color_Red, "[ϵͳ] ������ݱ�����ϣ���ʼ����������..");
    return 1;
}
public OnPlayerRequestClass(playerid, classid) {
    if(IsPlayerNPC(playerid)) return 1;
    if(!PlayerInfo[playerid][Login]) {
        LoginMusicCamera(playerid);
        return 0;
    }
    new rand = random(sizeof BirthPointInfo);
    SetSpawnInfo(playerid, NO_TEAM, PlayerInfo[playerid][Skin], BirthPointInfo[rand][0], BirthPointInfo[rand][1], BirthPointInfo[rand][2], BirthPointInfo[rand][3], 0, 0, 0, 0, 0, 0);
    SpawnPlayer(playerid); //����ҳ���
    return 1;
}
public OnPlayerRequestSpawn(playerid) {
    if(IsPlayerNPC(playerid)) return 1;
    if(!PlayerInfo[playerid][Login]) {
        SendClientMessage(playerid, Color_Yellow, "[ϵͳ]�㻹û��¼!");
        return 0;
    }
    new rand = random(sizeof BirthPointInfo);
    SetSpawnInfo(playerid, NO_TEAM, PlayerInfo[playerid][Skin], BirthPointInfo[rand][0], BirthPointInfo[rand][1], BirthPointInfo[rand][2], BirthPointInfo[rand][3], 0, 0, 0, 0, 0, 0);
    SpawnPlayer(playerid); //����ҳ���
    // if(PlayerInfo[playerid][Login]) 
    // {
    //     SetSpawnInfo(playerid, NO_TEAM, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    //     SpawnPlayer(playerid);
    //     return 1;
    // }
    return 1;
}
public OnPlayerConnect(playerid) //����ҽ����ʱ��
{
    if(IsPlayerNPC(playerid)) {
        SetSpawnInfo(playerid, NO_TEAM, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        SpawnPlayer(playerid);
        SetPlayerColor(playerid, Color_Yellow);
        InitializationVelo(playerid);
        return 1;
    }
    g_MysqlRaceCheck[playerid]++;

    PlayerInfo[playerid][LoginTimer] = -1;
    // InitializationVelo(playerid); //��ʼ���ٶȱ�
    PlayerInfo[playerid][Login] = false;
    // PlayerInfo[playerid][Register] = false;
    PlayerInfo[playerid][tvzt] = false;
    PlayerInfo[playerid][tvid] = playerid;
    p_PPC[playerid] = 0;
    p_ppcCar[playerid] = 0; //������
    //PlayerInfo[playerid][CheckYesNo] = false;
    PlayerInfo[playerid][LoginAttempts] = 1;
    //PlayerInfo[playerid][AntiRaceTP] = 0;
    Initialize_DeathMatch_Game(playerid);
    // for (new i = 0; i <= 7; i++) {
    //     TextDrawShowForPlayer(playerid, Screen[i]);
    // }

    if(IsPlayerAlreadyConnected(playerid) != -1) {
        SendClientMessage(playerid, Color_White, "[ϵͳ]�������ݲ�֧�ֶ���˺�ͬʱ��¼");
        DelayedKick(playerid);
        return 1;
    }



    Welcome(playerid);
    // ����Ƿ�׿�˲� **�ӳ�** ��ȡ��Ҹ�����Ϣ ������¼��
    InitPlayerAndroid(playerid);
    SCM(playerid, Color_Blue, "��ӭ����Romantic until Death������䷽���һ��������");
    SCM(playerid, Color_Yellow, "����ɽ·�ɴ��͵���������Ѿ�������ˣ������׶��ߺʹ��滷״�ߵĴ��͵�");
    SCM(playerid, Color_White, "2024-7-21�޸�����:��������ϵͳ�Լ���¼/�Զ���¼���⣬��������");
    SCM(playerid, Color_White, "�������Ѹ���/musicȫ����賵���ǿ�������ɽƯ�Ƶ�ʱ���һ���Լ�ϲ���ĸ���");
    SCM(playerid, Color_Yellow, "���Ƿ�������������λ���ڽ�����Ǩ����˴�����ȶ���");
    SCM(playerid,Color_Blue, "2024-8-18�����������޸����벢����ӵ����ȴ ���ݿ�bugĿǰδ���");
    return 1;
}

//�����ȴ���
forward NextOne();
public NextOne()
{
    ServerPlayingMusic = false;
}

public OnPlayerDisconnect(playerid, reason) //����뿪������ ���� �˳�������
{
    if(IsPlayerNPC(playerid)) {
        UnLoadVelo(playerid);
        return 1;
    }

    g_MysqlRaceCheck[playerid]++;

    UpdatePlayerData(playerid, reason);

    if(cache_is_valid(PlayerInfo[playerid][Cache_ID])) {
        cache_delete(PlayerInfo[playerid][Cache_ID]);
        PlayerInfo[playerid][Cache_ID] = MYSQL_INVALID_CACHE;
    }

    if(PlayerInfo[playerid][LoginTimer]) {
        KillTimer(PlayerInfo[playerid][LoginTimer]);
        PlayerInfo[playerid][LoginTimer] = -1;
    }

    PlayerInfo[playerid][Login] = false;
    return 1;
}

public OnPlayerSpawn(playerid) //����ҳ���ʱ
{
    if(IsPlayerNPC(playerid)) { //��������NPC�Ļ�
        InitializationNpcs(playerid); //��ʼ��NPC
        return 1;
    }
    if(IsPlayerDeathMatch(playerid)) {
        DeathMatch_OnPlayerSpawn(playerid);
        return 1;
    }
    SetPlayerSkin(playerid, PlayerInfo[playerid][Skin]);
    SpawnAttire(playerid); //���װ��
    if(CreateCamera[playerid][CreateStatus] != 0) return 1;
    if(pRaceing[playerid] != 1) {
        // SetPlayerVirtualWorld(playerid, 0);
        SetPlayerHealth(playerid, 1000000);
        SetPlayerPos_Birth(playerid);
        // SetPlayerPos(playerid, 1958.835693, 1343.151123, 15.374607);
        // SetPlayerFacingAngle(playerid, 269.142425);
        // SetCameraBehindPlayer(playerid);
        // new Float:X, Float:Y, Float:Z;
        // GetPlayerPos(playerid, X, Y, Z);
        // NoDieTime[playerid] = CreateDynamic3DTextLabel("�޵�ʱ����...\n", 0xFF0000FF, X, Y, Z, 40.0, playerid);
        // // Attach3DTextLabelToPlayer(NoDieTime[playerid], playerid, 0.0, 0.0, 0.7);
        // SetTimerEx("CheckAso", 3000, 0, "i", playerid); //���������޵�
        if(GetPlayerScore(playerid) < 120) {
            SendClientMessage(playerid, Color_White, "[ϵͳ]��⵽����Ϸʱ��δ��120���ӣ��Զ��򿪰�����ʾ");
            AntiCommand[playerid] = 0;
            CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/help");
            // OnPlayerCommandText(playerid, "/help");
        }
    } else if(p_PPC[playerid]) { //�������Ļ�
        PPC_Veh(playerid);
    } else { //����ʱ������
        // new raid = RaceHouse[GameRace[playerid][rgameid]][rraceid];
        // new trcp[racecptype];
        // if(GameRace[playerid][rgamecp] - 1 <= 0) Race_GetCp(raid, 1, trcp); //����ǵ�һ����Ļ�����������һ���㣬��Ȼ���Ǹ���
        // else Race_GetCp(raid, GameRace[playerid][rgamecp] - 1, trcp);
        // 2020.3.17 ע�� ���������� 

        //SetTimerEx("", 1000, 0, "i", playerid);
        PlayerInfo[playerid][lastVehSpeed] = 0;
        ReSpawnRaceVehicle(playerid); //2020.1.12�ģ���������Ч��
    }
    return 1;
}

public OnPlayerDeath(playerid, killerid, reason) {
    if(p_PPC[playerid] && IsPlayerInAnyVehicle(playerid)) DestroyVehicle(p_ppcCar[playerid]);
    if(pRaceing[playerid]) { //����������������������и�λs
        PlayerInfo[playerid][lastVehSpeed] = 0;
        GameTextForPlayer(playerid, "~n~~n~~n~~n~~n~~n~~n~~n~~r~RESPAWNING", 3000, 3); //���½���ʾ
        // TextDrawShowForPlayer(playerid, ReSpawningText[playerid]);
    } else {
        GameTextForPlayer(playerid, "~n~~n~~n~~n~~n~~n~~n~~n~~r~Wasted", 3000, 3); //���½���ʾ
    }
    DeathMatch_OnPlayerDeath(playerid, killerid);
    return 1;
}

public OnVehicleDeath(vehicleid, killerid) {
    return 1;
}

public OnPlayerText(playerid, text[]) {
    /*for (new i = 0; i < BadWordsCount; ++i)
    if(strfind (text, BadWords [i], true))
    {
		SCM(playerid,Color_Red,"[ϵͳ] ����������,������Ϸ����!");
        return 0;
    }*/
    if(PlayerInfo[playerid][Login] == false) {
        SCM(playerid, Color_Red, "[ϵͳ] �㻹δ��¼!");
        return 0;
    }
    if(PlayerInfo[playerid][JailSeconds] > 0) {
        SCM(playerid, Color_Red, "[����]:�����ڼ������뱣���侲,�Ժ�Ҫ��������!. ");
        return 0;
    }
    if(AntiCommand[playerid] == 1) {
        SCM(playerid, Color_Red, "[ϵͳ]:�㷢�Ե��ٶ�̫����!");
        return 0;
    }
    new placeholder;
    for (new i = 0; i < sizeof InvalidWords; i++) //���δ��Զ���*
    {
        placeholder = strfind(text, InvalidWords[i], true);
        if(placeholder != -1) {
            for (new x = placeholder; x < placeholder + strlen(InvalidWords[i]); x++) {
                text[x] = '*';
            }
        }
    }
    AntiCommand[playerid] = 1;
    new ChatText[144];
    if(text[0] == '#') {
        if(!strcmp(PlayerInfo[playerid][Team], "null", false)) {
            SendClientMessage(playerid, Color_Team, "[�Ŷ�]��δ���κ�һ���Ŷ���");
            return 0;
        }
        if(!strcmp(PlayerInfo[playerid][Designation],"" ,false)) {
            format(ChatText, sizeof(ChatText), "[�Ŷ�]%s{%06x}%s(%d) {FFBA51}[V]:{FFFFFF} %s", PlayerInfo[playerid][Designation], GetPlayerColor(playerid) >>> 12, GetName(playerid), playerid, text);
        } else {
            format(ChatText, sizeof(ChatText), "[�Ŷ�]{%06x}[%s](%d) {FFBA51}[V]:{FFFFFF} %s", GetPlayerColor(playerid) >>> 12, GetName(playerid), playerid, text);
        }
        if(!strcmp(PlayerInfo[playerid][Tail], "", true)) {
            format(ChatText, sizeof(ChatText), "{%06x}%s(%d) {FFBA51}[V]:{FFFFFF} %s", ChatText, PlayerInfo[playerid][Tail]);
        }
        team_SCM(ChatText, PlayerInfo[playerid][Team]);
        return 0;
    }
    if(GetPlayerVirtualWorld(playerid) == 0) {
        if(!strcmp(PlayerInfo[playerid][Team], "null", false)) {
            if(!strcmp(PlayerInfo[playerid][Designation],"", false)) {
                format(ChatText, sizeof(ChatText), "[����]{%06x}%s(%d):{FFFFFF} %s", GetPlayerColor(playerid) >>> 8, GetName(playerid), playerid, text);
            } else {
			    format(ChatText, sizeof(ChatText), "[����]%s{%06x}%s(%d):{FFFFFF} %s", PlayerInfo[playerid][Designation], GetPlayerColor(playerid) >>> 8, GetName(playerid), playerid, text);
            }
        } else {
            if(!strcmp(PlayerInfo[playerid][Designation],"", false)) {
                format(ChatText, sizeof(ChatText), "[����]{%06x}%s(%d) {FFBA51}[V]:{FFFFFF} %s", GetPlayerColor(playerid) >>> 12, GetName(playerid), playerid, text);
            } else {
                format(ChatText, sizeof(ChatText), "[����]%s{%06x}%s(%d) {FFBA51}[V]:{FFFFFF} %s", PlayerInfo[playerid][Designation], GetPlayerColor(playerid) >>> 12, GetName(playerid), playerid, text);
            }
        }
        if(strcmp(PlayerInfo[playerid][Tail], "", false)) {
            format(ChatText, sizeof(ChatText), "%s         %s", ChatText, PlayerInfo[playerid][Tail]);
        }
        SCMALL(Color_White, ChatText); //GetPlayerColor(playerid)
    } else {
        if(text[0] == '!') //��С���緢���������� ��!
        {
            if(!strcmp(PlayerInfo[playerid][Team], "null", true)) {
                if(!strcmp(PlayerInfo[playerid][Designation],"", false)) {
                    format(ChatText, sizeof(ChatText), "[����]{%06x}%s(%d):{FFFFFF} %s", GetPlayerColor(playerid) >>> 12, GetName(playerid), playerid, text);
                } else {
                    format(ChatText, sizeof(ChatText), "[����]%s{%06x}%s(%d):{FFFFFF} %s", PlayerInfo[playerid][Designation], GetPlayerColor(playerid) >>> 12, GetName(playerid), playerid, text);
                }
            } else {
                if(!strcmp(PlayerInfo[playerid][Designation], "null", true)) {
                    format(ChatText, sizeof(ChatText), "[����]{%06x}%s(%d) {FFBA51}[V]:{FFFFFF} %s", GetPlayerColor(playerid) >>> 12, GetName(playerid), playerid, text);
                } else {
                    format(ChatText, sizeof(ChatText), "[����]%s{%06x}%s(%d) {FFBA51}[V]:{FFFFFF} %s", PlayerInfo[playerid][Designation], GetPlayerColor(playerid) >>> 12, GetName(playerid), playerid, text);
                }
            }
            if(strcmp(PlayerInfo[playerid][Tail], "", true)) {
                format(ChatText, sizeof(ChatText), "[%s]{FFFFFF}", ChatText, PlayerInfo[playerid][Tail]);
            }
            SCMALL(Color_White, ChatText);
        } else {
            if(!strcmp(PlayerInfo[playerid][Team], "null", false)) {
                if(!strcmp(PlayerInfo[playerid][Designation],"", false)) {
                    format(ChatText, sizeof(ChatText), "{FFB6C1}[Ƶ��-%d]{%06x}%s(%d):{FFFFFF} %s", GetPlayerVirtualWorld(playerid), GetPlayerColor(playerid) >>> 23, GetName(playerid), playerid, text);
                } else {
                    format(ChatText, sizeof(ChatText), "{FFB6C1}[Ƶ��-%d]%s{%06x}%s(%d):{FFFFFF} %s", GetPlayerVirtualWorld(playerid), PlayerInfo[playerid][Designation], GetPlayerColor(playerid) >>> 12, GetName(playerid), playerid, text);
                }
            } else {
                if(!strcmp(PlayerInfo[playerid][Designation],"" , true)) {
                    format(ChatText, sizeof(ChatText), "{FFB6C1}[Ƶ��-%d]{%06x}%s(%d) {FFBA51}[V]:{FFFFFF} %s", GetPlayerVirtualWorld(playerid), GetPlayerColor(playerid) >>> 12, GetName(playerid), playerid, text);
                } else {
                    format(ChatText, sizeof(ChatText), "{FFB6C1}[Ƶ��-%d][%s]{%06x}%s(%d) {FFBA51}[V]:{FFFFFF} %s", GetPlayerVirtualWorld(playerid), PlayerInfo[playerid][Designation], GetPlayerColor(playerid) >>> 12, GetName(playerid), playerid, text);
                }
            }
            if(strcmp(PlayerInfo[playerid][Tail], "", true)) {
                format(ChatText, sizeof(ChatText), "%s         %s", ChatText, PlayerInfo[playerid][Tail]);
            }
            for (new i = GetPlayerPoolSize(); i >= 0; i--) {
                if(IsPlayerConnected(i)) {
                    if(GetPlayerVirtualWorld(i) == GetPlayerVirtualWorld(playerid)) {
                        SCM(i, Color_White, ChatText); //GetPlayerColor(playerid)
                    }
                }
            }
        }
    }
    Common_Answer_QA(playerid, text);
    new removecolor = strfind(ChatText, "{", true); //�Զ�������ɫ��� ��ʡ��־�ռ� By YuCarl77
    while (removecolor != -1) {
        if(removecolor + 8 <= strlen(ChatText) && ChatText[removecolor + 7] == '}') {
            strdel(ChatText, removecolor, removecolor + 8);
            removecolor = strfind(ChatText, "{", true);
        } else {
            break;
        }
    }
    PlayerTextRecord(ChatText);
    // printf(ChatText);
    return 0;
}

public OnPlayerCommandReceived(const playerid, const cmdtext[]) //��ִ��commandtext֮ǰִ�� ���i-zcmd
{
    if(PlayerInfo[playerid][Login] == false) {
        SCM(playerid, Color_Red, "[ϵͳ]:�㻹û�е�¼ѽ");
        return 0;
    }
    if(PlayerInfo[playerid][JailSeconds] > 0) {
        SCM(playerid, Color_Red, "[ϵͳ]:�ڼ����н�ֹʹ������");
        return 0;
    }
    //if(TpCheck[playerid] == 1) return 1;
    if(AntiCommand[playerid] == 1) {
        SCM(playerid, Color_Red, "[ϵͳ]:������ָ����ٶ�̫����!");
        return 0;
    }
    if(p_PPC[playerid] && strcmp(cmdtext, "/ppc", true)) {
        SCM(playerid, Color_White, "[ϵͳ]:������������ֻ����ʹ��/ppc");
        return 0;
    }
    new cmd[128], idx;
    cmd = strtok(cmdtext, idx);
    if(strcmp("/pm", cmd, true) == 0) //cmdtext[1]=='p' && cmdtext[2]=='m'
    {
        // cmd_pm(playerid, strtok(cmdtext, idx));
        new Message[128], gMessage[128];
        Message = strtok(cmdtext, idx);
        if(!strlen(Message) || strlen(Message) > 5) {
            SCM(playerid, Color_White, "[pm] ��ʹ��:/pm ID Ҫ˵�Ļ���"); //PM������Ϣ
            return 0;
        }
        new id = strval(Message);
        gMessage = strrest(cmdtext, idx);
        if(!strlen(gMessage)) {
            SCM(playerid, Color_White, "[pm]��ʹ��:/pm ID Ҫ˵�Ļ���"); //PM������Ϣ
            return 0;
        }
        if(!IsPlayerConnected(id) || IsPlayerNPC(id)) {
            SCM(playerid, Color_White, "[pm]/pm :�������ID��"); //������Ϣ
            return 0;
        }
        if(playerid == id) {
            SCM(playerid, Color_White, "[pm] �㲻��PM���Լ�");
            return 0;
        }
        format(Message, sizeof(Message), "[pm] ����� %s(%d):%s", GetName(id), id, gMessage);
        SCM(playerid, Color_White, Message);
        GameTextForPlayer(playerid, "~n~~n~~n~~n~~n~~n~~n~~n~~w~Private message ~r~Sent~w~.", 3000, 3); //��A���PM��B��ҵ�ʱ�� A��һ���ʾ���
        format(Message, sizeof(Message), "[pm] �������� %s(%d):%s", GetName(playerid), playerid, gMessage);
        SCM(id, Color_White, Message);
        GameTextForPlayer(id, "~n~~n~~n~~n~~n~~n~~n~~n~~w~Private message ~r~Recieved~w~.", 3000, 3); //��B����յ�A��ҵ�PM��ʱ�� B��һ���ʾ���
        PlayerPlaySoundEx(playerid, 1057);
        PlayerPlaySoundEx(id, 1057);
        // PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
        // PlayerPlaySound(id, 1057, 0.0, 0.0, 0.0);
        AntiCommand[playerid] = 1; //�������pm�ͷ��籨��
        return 0;
    }
    if(IsPlayerDeathMatch(playerid) && strcmp(cmdtext, "/dm leave", true) && strcmp(cmdtext, "/kill", true) && strcmp(cmdtext, "/k", true)) {
        SCM(playerid, Color_White, "[DM]������ֻ��ͨ��/dm leave�뿪");
        return 0;
    }
    if(pRaceing[playerid] == 1 && strcmp("/c", cmd, true) == 0) {
        //���CP���ǵ�һ����Ļ�������
        if(GameRace[playerid][rgamecp] == 1) {
            cmd_c(playerid, strtok(cmdtext, idx));
            return 0;
        } else {
            SCM(playerid, Color_Red, "[����] ���ı����ѿ�ʼ,������ˢ��!");
            return 0;
        }
    }
    if(pRaceing[playerid] == 1 && strcmp(cmdtext, "/r l", true) != 0 && strcmp(cmdtext, "/r leave", true) != 0 && strcmp(cmdtext, "/kill", true) != 0 && strcmp(cmdtext, "/r s", true) != 0) {
        SCM(playerid, Color_Red, "[����] ������������ֻ����ʹ��˽��,����/r l�뿪����");
        return 0;
    }
    AntiCommand[playerid] = 1;
    return 1;
}

public OnPlayerCommandPerformed(const playerid, const cmdtext[], const success) // ��ִ��commandtext֮��ִ�� successΪִ�гɹ� Ҳ����1 ʧ�ܾ���0
{
    if(!success) {
        if(cmdtext[0] == '/' && cmdtext[1] == '/' && cmdtext[2]) {
            new tmp[128], str[128];
            format(tmp, 128, "%s", cmdtext);
            strdel(tmp, 0, 2);
            format(tmp, 128, "%s", tmp);
            new id = make_findgo(tmp);
            if(id == -1) return SCM(playerid, TransferColor, "[����] ��������Ĵ��͵㲻����!");
            format(str, sizeof(str), "[����] �㴫�͵���'//%s'", vmake[id][mname]);
            SCM(playerid, TransferColor, str);
            if(vmake[id][mz] >= 150.0) DynUpdateStart(playerid); //2020.3.21����
            if(IsPlayerInAnyVehicle(playerid)) {
                SetVehiclePos(GetPlayerVehicleID(playerid), vmake[id][mx], vmake[id][my], vmake[id][mz]);
                SetVehicleZAngle(GetPlayerVehicleID(playerid), vmake[id][ma]);
            } else {
                SetPlayerPos(playerid, vmake[id][mx], vmake[id][my], vmake[id][mz]);
                SetPlayerFacingAngle(playerid, vmake[id][ma]);
                if(PlayerInfo[playerid][enableInvincible] != 1) {
                    SetPlayerHealth(playerid, 100);
                }
            }
            return 1;
        }
        if(cmdtext[0] == '/' && cmdtext[1]) {
            new tmp[128], str[128];
            format(tmp, 128, "%s", cmdtext);
            strdel(tmp, 0, 1);
            format(tmp, 128, "%s", tmp);
            new id = make_Sysfindgo(tmp);
            if(id == -1) return SCM(playerid, Color_White, "[ϵͳ]:û���������! , ���������/help");
            format(str, sizeof(str), "{33CCCC}%s(%d) {82D900}������{00FF99} %s {FF0066}/%s {82D900}", GetName(playerid), playerid, vsysmake[id][tdescribe], vsysmake[id][mname]);
            SCMALL(TransferColor, str);
            if(vsysmake[id][mz] >= 150.0) DynUpdateStart(playerid); //2020.3.21����
            if(IsPlayerInAnyVehicle(playerid)) {
                SetVehiclePos(GetPlayerVehicleID(playerid), vsysmake[id][mx], vsysmake[id][my], vsysmake[id][mz]);
                SetVehicleZAngle(GetPlayerVehicleID(playerid), vsysmake[id][ma]);
            } else {
                SetPlayerPos(playerid, vsysmake[id][mx], vsysmake[id][my], vsysmake[id][mz]);
                SetPlayerFacingAngle(playerid, vsysmake[id][ma]);
            }
            return 1;
        }
        return 1;
    }
    new tmp[128];
    format(tmp, sizeof(tmp), "[ָ��]%s(%d):%s", GetName(playerid), playerid, cmdtext);
    PlayerCommandRecord(tmp);
    return 1;
}

//����DM����
// public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ) {
//     DeathMatch_OnPlayerWeaponShot(playerid, hittype, hitid);
//     return 1;
// }
//�����ʽ CMD:����(playerid,params[]) һ��Ϊ�˸�����Ǩ��ֱ�Ӱ�paramsд��cmdtext
// {
//     ��ע�⣬params[]��Զ����Ϊ�ա����������δΪ�����ṩ�κβ�����params[0]��Ϊ'\1'��ʹ�ð���isnull(string[])�渽�ĺ��������Ч�ԡ�
// iZCMD with sscanf is an efficient way to process commands
//  if(sscanf(params, "is[�ַ�������]", skinid,str))
//     return 1;
// }
// CMD:pm(const playerid, const cmdtext[]) { //PM˽����� ��������Ϊ���ƿ�PRace�ļ�� ����������ʱpm
//     new Message[128],gMessage[128],idx;
//     Message = strtok(cmdtext, idx);
//     if(!strlen(Message) || strlen(Message) > 5) {
//         SCM(playerid, Color_White, "[pm] ��ʹ��:/pm ID Ҫ˵�Ļ���"); //PM������Ϣ
//         return 0;
//     }
//     new id = strval(Message);
//     gMessage = strrest(cmdtext, idx);
//     if(!strlen(gMessage)) {
//         SCM(playerid, Color_White, "[pm]��ʹ��:/pm ID Ҫ˵�Ļ���"); //PM������Ϣ
//         return 0;
//     }
//     if(!IsPlayerConnected(id)) {
//         SCM(playerid, Color_White, "[pm]/pm :�������ID��"); //������Ϣ
//         return 0;
//     }
//     if(playerid == id) {
//         SCM(playerid, Color_White, "[pm] �㲻��PM���Լ�");
//         return 0;
//     }
//     format(Message, sizeof(Message), "[pm] ����� %s(%d):%s", GetName(id), id, gMessage);
//     SCM(playerid, Color_White, Message);
//     GameTextForPlayer(playerid, "~n~~n~~n~~n~~n~~n~~n~~n~~w~Private message ~r~Sent~w~.", 3000, 3); //��A���PM��B��ҵ�ʱ�� A��һ���ʾ���
//     format(Message, sizeof(Message), "[pm] �������� %s(%d):%s", GetName(playerid), playerid, gMessage);
//     SCM(id, Color_White, Message);
//     GameTextForPlayer(id, "~n~~n~~n~~n~~n~~n~~n~~n~~w~Private message ~r~Recieved~w~.", 3000, 3); //��B����յ�A��ҵ�PM��ʱ�� B��һ���ʾ���
//     PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
//     PlayerPlaySound(id, 1057, 0.0, 0.0, 0.0);
//     AntiCommand[playerid] = 1; //�������pm�ͷ��籨��
//     return 1;
// }
CMD:chat(const playerid, const cmdtext[]) { //С����������ط�ʹ�ô�����˵�� ǰ��Ӹ�!ͬ��
    new text[145];
    if(sscanf(cmdtext, "s[145]", text)) return SendClientMessage(playerid, Color_White, "[ϵͳ]���ں��������˵�Ļ�,�ʹ������");
    format(text, sizeof(text), "!%s", text);
    CallRemoteFunction("OnPlayerText", "is", playerid, text);
    // if(!strcmp(PlayerInfo[playerid][Team], "null", true)) {
    //     if(!isnull(PlayerInfo[playerid][Designation])) {
    //         format(ChatText, sizeof(ChatText), "[����]%s{%06x}%s(%d):{FFFFFF} %s         %s", PlayerInfo[playerid][Designation], GetPlayerColor(playerid) >>> 8, GetName(playerid), playerid, text, PlayerInfo[playerid][Tail]);
    //     } else {
    //         format(ChatText, sizeof(ChatText), "[����]{%06x}%s(%d):{FFFFFF} %s         %s", GetPlayerColor(playerid) >>> 8, GetName(playerid), playerid, text, PlayerInfo[playerid][Tail]);
    //     }
    // } else {
    //     if(!isnull(PlayerInfo[playerid][Designation])) {
    //         format(ChatText, sizeof(ChatText), "[����]%s{%06x}%s(%d) {FFBA51}[V]:{FFFFFF} %s         %s", PlayerInfo[playerid][Designation], GetPlayerColor(playerid) >>> 8, GetName(playerid), playerid, text, PlayerInfo[playerid][Tail]);
    //     } else {
    //         format(ChatText, sizeof(ChatText), "[����]{%06x}%s(%d) {FFBA51}[V]:{FFFFFF} %s         %s", GetPlayerColor(playerid) >>> 8, GetName(playerid), playerid, text, PlayerInfo[playerid][Tail]);
    //     }
    // }
    // SCMALL(Color_White, ChatText);
    // PlayerTextRecord(ChatText);
    return 1;
}
CMD:help(const playerid, const cmdtext[]) {
    Dialog_Show(playerid, HelpSystem, DIALOG_STYLE_LIST, "����ϵͳ", "����ƪ\n����ϵͳ\n���Ի�����\n�ճ�����\n�ؾ����\n�������\n�ۿ����\n����/����\n����ϵͳ\n�Ҿ�ϵͳ\nװ��ϵͳ\n�����ϵͳ\n����Աָ���ȫ\n����ϵͳ\n�Ŷ�ϵͳ\n���ϵͳ", "ȷ��", "ȡ��");
    return 1;
}
CMD:telemenu(const playerid, const cmdtext[]) {
    Tele_ShowListDialog(playerid, TelePage[playerid]);
    return 1;
}
CMD:sdb(const playerid, const cmdtext[]) {
    if(PlayerInfo[playerid][speedoMeter]) {
        for (new a = 0; a <= 21; a++) {
            PlayerTextDrawHide(playerid, velo[playerid][a]);
        }
        PlayerInfo[playerid][speedoMeter] = 0;
        SendClientMessage(playerid, Color_White, "[ϵͳ]��ر����ٶȱ�");
    } else {
        for (new a = 0; a <= 21; a++) {
            PlayerTextDrawShow(playerid, velo[playerid][a]);
        }
        PlayerInfo[playerid][speedoMeter] = 1;
        SendClientMessage(playerid, Color_White, "[ϵͳ]�㿪�����ٶȱ�");
    }
    return 1;
}
CMD:cmds(const playerid, const cmdtext[]) {
    return cmd_help(playerid, cmdtext);
}
CMD:wdch(const playerid, const params[]) {
    new string[128];
    if(!isnull(PlayerInfo[playerid][Designation])) format(string, sizeof(string), "{FFFFFF}�����ڵĳƺ���[%s{FFFFFF}]\n����һ�γƺ���Ҫ3000���\n����null����ƺ�\n��ɫ��ʹ��{}�������", PlayerInfo[playerid][Designation]);
    else format(string, sizeof(string), "��Ŀǰ���޳ƺ�\n����һ�γƺ���Ҫ3000���");
    Dialog_Show(playerid, Dialog_Designation, DIALOG_STYLE_INPUT, "�ҵĳƺ�", string, "ȷ��", "ȡ��");
    return 1;
}
CMD:motto(const playerid, const params[]) {
    new string[128];
    if(!isnull(PlayerInfo[playerid][Tail])) format(string, sizeof(string), "{FFFFFF}�����ڵ�Сβ����[%s{FFFFFF}]\n����һ��Сβ����Ҫ3000���\n����null���Сβ��\n��ɫ��ʹ��{}�������", PlayerInfo[playerid][Tail]);
    else format(string, sizeof(string), "��Ŀǰ����Сβ��\n����һ�γƺ���Ҫ3000���");
    Dialog_Show(playerid, Dialog_Tail, DIALOG_STYLE_INPUT, "�ҵ�Сβ��", string, "ȷ��", "ȡ��");
    return 1;
}
CMD:tail(const playerid, const params[]) {
    return cmd_motto(playerid, "");
}
CMD:ppc(const playerid, const cmdtext[]) {
    new msg[128];
    if(p_PPC[playerid] == 0) {
        DisableRemoteVehicleCollisions(playerid, false);
        p_PPC[playerid] = 1;
        SetPlayerVirtualWorld(playerid, 10001);
        PPC_Veh(playerid);
        format(msg, sizeof msg, "[ϵͳ*]%s(%d) ������������ /ppc", GetName(playerid), playerid);
    } else {
        p_PPC[playerid] = 0;
        OnPlayerSpawn(playerid);
        SetPlayerVirtualWorld(playerid, 0);
        format(msg, sizeof msg, "[ϵͳ*]%s(%d) �뿪��������", GetName(playerid), playerid);
        if(PlayerInfo[playerid][NoCrash]) DisableRemoteVehicleCollisions(playerid, true);
    }
    SendClientMessageToAll(Color_White, msg);
    return 1;
}
CMD:anim(const playerid, const cmdtext[]) { //�����ű�
    new tmp[128], idx;
    tmp = strtok(cmdtext, idx);
    if(isnull(tmp)) {
        new string[227];
        format(string, sizeof(string), "/anim [ID] ��F�������\n1.���� 2.���±�ͷ 3.����1 4.���� 5.����2 6.����3\n7.����4 8.�����޳� 9.��ǽ1 10.��ǽ2 11.����\n12.δ֪13.����1 14.����2 15.����3 16.����4 \n17.��̫�� 18.����19.Ͷ�� 20.���(��ǹ/ָ��) 21.����");
        Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "��������", string, "OK", "");
        return 1;
    }
    new id = strval(tmp);
    ClearAnimations(playerid); //���ԭ�ȶ���
    SetPlayerSpecialAction(playerid, 0); //������⶯��������������
    Action_Play(playerid, id);
    return 1;
}
CMD:mynetstats(const playerid, const cmdtext[]) {
    new stats[256];
    GetPlayerNetworkStats(playerid, stats, sizeof(stats)); // get your own networkstats
    Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "�ҵ� ��ǰ����״̬", stats, "Okay", "");
    return 1;
}
CMD:sound1(const playerid, const cmdtext[]) {
    PlayerPlaySoundEx(playerid, 1062);
    return 1;
}
CMD:sound2(const playerid, const cmdtext[]) {
    PlayerPlaySoundEx(playerid, 1063);
    PlayerPlaySoundEx(playerid, 1068);
    return 1;
}
CMD:sound3(const playerid, const cmdtext[]) {
    PlayerPlaySoundEx(playerid, 1069);
    PlayerPlaySoundEx(playerid, 1076);
    return 1;
}
CMD:sound4(const playerid, const cmdtext[]) {
    PlayerPlaySoundEx(playerid, 1077);
    PlayerPlaySoundEx(playerid, 1097);
    return 1;
}
CMD:sound5(const playerid, const cmdtext[]) {
    PlayerPlaySoundEx(playerid, 1098);
    PlayerPlaySoundEx(playerid, 1183);
    return 1;
}
CMD:sound6(const playerid, const cmdtext[]) {
    PlayerPlaySoundEx(playerid, 1184);
    PlayerPlaySoundEx(playerid, 1185);
    return 1;
}
CMD:sound7(const playerid, const cmdtext[]) {
    PlayerPlaySoundEx(playerid, 1186);
    PlayerPlaySoundEx(playerid, 1187);
    return 1;
}
CMD:soundstop(const playerid, const cmdtext[]) {
    //2020.1.12 �޸�soundstop�޷�ֹͣ����
    // PlayerPlaySound(playerid, 1186, 0.0, 0.0, 0.0);
    // PlayerPlaySoundEx(playerid, 1186);
    PlayerPlaySoundEx(playerid, 0);
    // ��֪��Ӱ��Ӱ�� 2021.2.16�� ά��˵����0ֹͣ
    return 1;
}
CMD:moveme(const playerid, const cmdtext[]) {
    SetPlayerCameraPos(playerid, 0, 0, 0);
    SetPlayerCameraLookAt(playerid, 0, 0, 0);
    TogglePlayerSpectating(playerid, true);
    InterpolateCameraPos(playerid, 2488.533, -1675.285, 17, 1000.0, 1000.0, 40.0, 30000, CAMERA_MOVE);
    SCM(playerid, Color_White, "���Ծ�ͷ1");
    InterpolateCameraLookAt(playerid, 2488.533, -1675.285, 17, 1000.0, 1000.0, 40.0, 30000, CAMERA_MOVE);
    SCM(playerid, Color_White, "���Ծ�ͷ2");
    //Move the player's camera from point A to B in 10000 milliseconds (10 seconds).
    return 1;
}
CMD:music(playerid, params[])
{
    if(ServerPlayingMusic) return SendClientMessage(playerid,Color_Yellow,"[����ϵͳ]Ŀǰ���������ڲ������֣����Եȼ������ڲ���");
    if (sscanf(params, "s[256]", params[1])) return SendClientMessage(playerid, -1, "ȫ�����: /music [��������] - ��ѵ�赫����ȴ");
    //if(PlayerInfo[playerid][Cash] < 1000) return SendClientMessage(playerid, -1, "ȫ�����: ��û���㹻�Ľ�Ǯ��");

    new string[144];
    
    for(new i; i < 101; i++)
    {
        new Float:Pos[3];
        GetPlayerPos(i, Pos[0], Pos[1], Pos[2]);
        PlayAudioStreamForPlayer(i, params[1], Pos[0], Pos[1], Pos[2], 1, 0);
    }
   	format(string, sizeof(string), "���'%s[%i]' ����1000������Ϊ��ҵ���һ�׸裡", GetName(playerid), playerid);
    SCMALL(Color_Yellow,string);
   // GivePlayerCash(playerid, -1000);
    ServerPlayingMusic = true;
	SetTimer("NextOne", 140000, false);
    return 1;
}

CMD:stopmusic(playerid, params[]) {
    StopAudioStreamForPlayer(playerid);
    SCM(playerid, Color_Red, "[ϵͳ] ��ֹͣ�����ֲ���!");
	return 1;
}

stock PlayerMusicForAll(url[]) {
	for(new i = 0, all = GetPlayerPoolSize()+1; i < all; i ++) {
		if(IsPlayerConnected(i) == 1) {
			PlayAudioStreamForPlayer(i, url);
		}
	}
}
CMD:kill(const playerid, const cmdtext[]) {
    new Float:Health;
    GetPlayerHealth(playerid, Health);
    if(!Health) return SendClientMessage(playerid, Color_White, "[ϵͳ]����ֵΪ��,���������,��ȴ��������³�����");
    if(PlayerInfo[playerid][tvzt]) { //�����Ҵ���TV״̬ �����������˳�TV����ɱ ��Ȼ���BUG
        AntiCommand[playerid] = 0;
        CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/tv off");
        // OnPlayerCommandText(playerid, "/tv off");
    }
    if(pRaceing[playerid]) {
        new Float:POS[3];
        GetPlayerPos(playerid, POS[0], POS[1], POS[2]);
        SetPlayerPos(playerid, POS[0], POS[1], POS[2]);
    }
    SetPlayerHealth(playerid, -1.0);
    return 1;
}
CMD:s(const playerid, const cmdtext[]) { //����Ҫ���͵�������
    GetPlayerPos(playerid, PlayerInfo[playerid][SavePos][0], PlayerInfo[playerid][SavePos][1], PlayerInfo[playerid][SavePos][2]);
    PlayerInfo[playerid][SaveInterior] = GetPlayerInterior(playerid);
    splp[playerid] = 1;
    if(IsPlayerInAnyVehicle(playerid)) GetVehicleZAngle(GetPlayerVehicleID(playerid), PlayerInfo[playerid][SavePos][3]);
    else GetPlayerFacingAngle(playerid, PlayerInfo[playerid][SavePos][3]);
    SCM(playerid, 0xFFFFFFAA, "[��������]��ǰ�����ѱ���, ����/l���ظ�����");
    return 1;
}
CMD:sp(const playerid, const cmdtext[]) return cmd_s(playerid, cmdtext); //����Ҫ���͵�������
CMD:l(const playerid, const cmdtext[]) { //���͵����������
    if(splp[playerid] != 1) return SCM(playerid, 0xFFFF00AA, "[��������]����ʹ��/s���б�������"); //�����û�б�������Ļ�����ʾ�ȱ���
    SetPlayerInterior(playerid, PlayerInfo[playerid][SaveInterior]);
    if(IsPlayerInAnyVehicle(playerid)) {
        SetVehiclePos(GetPlayerVehicleID(playerid), PlayerInfo[playerid][SavePos][0], PlayerInfo[playerid][SavePos][1], PlayerInfo[playerid][SavePos][2]);
        SetVehicleZAngle(GetPlayerVehicleID(playerid), PlayerInfo[playerid][SavePos][3]);
    } else {
        SetPlayerPos(playerid, PlayerInfo[playerid][SavePos][0], PlayerInfo[playerid][SavePos][1], PlayerInfo[playerid][SavePos][2]);
        SetPlayerFacingAngle(playerid, PlayerInfo[playerid][SavePos][3]);
    }
    return 1;
}

CMD:lp(const playerid, const cmdtext[]) return cmd_l(playerid, cmdtext); //���͵���������� ���븴��
// CMD:stunt(const playerid, const cmdtext[]) {
//     new tmp[8];
//     if(sscanf(cmdtext, "s[8]", tmp)) return SCM(playerid, Color_White, "[ϵͳ]/stunt on���� off�ر��ؼ���ʾ");
//     if(strcmp(tmp, "on", true) == 0) {
//         EnableStuntBonusForPlayer(playerid, 1);
//         SCM(playerid, Color_White, "[ϵͳ] �ѿ����ؼ�������ʾ.");
//     } else {
//         EnableStuntBonusForPlayer(playerid, 0);
//         SCM(playerid, Color_White, "[ϵͳ] �ѹر��ؼ�������ʾ.");
//     }
//     return 1;
// }

CMD:xiufu(const playerid, const cmdtext[]) { //����ҿ�ס��ʱ���������ָ������޸���������ܺ���
    new Float:X, Float:Y, Float:Z;
    GetPlayerPos(playerid, X, Y, Z);
    SetPlayerPos(playerid, X, Y, Z + 2.8);
    return 1;
}
CMD:skin(const playerid, const cmdtext[]) {
    new skinid;
    if(sscanf(cmdtext, "d", skinid)) return ShowModelSelectionMenu(playerid, skinlist, "Select Skin");
    if(0 > skinid > 311 || skinid == 74) return SCM(playerid, Color_White, "[����] �����Ƥ��ID.");
    if(IsPlayerInAnyVehicle(playerid)) {
        new seat = GetPlayerVehicleSeat(playerid);
        new vehid = GetPlayerVehicleID(playerid);
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        SetPlayerPos(playerid, x, y, z);
        SetPlayerSkin(playerid, skinid);
        PutPlayerInVehicle(playerid, vehid, seat);
    } else {
        SetPlayerSkin(playerid, skinid);
    }
    PlayerInfo[playerid][Skin] = skinid;
    SCM(playerid, Color_White, "[����] �����ɹ�.");
    return 1;
}
CMD:hf(const playerid, const cmdtext[]) return cmd_skin(playerid, cmdtext);
CMD:sz(const playerid, const cmdtext[]) {
    OnPlayerSettings(playerid);
    return 1;
}
CMD:wdsz(const playerid, const cmdtext[]) return cmd_sz(playerid, cmdtext);
stock OnPlayerSettings(const playerid) {
    Dialog_Show(playerid, PlayerInfoDialog, DIALOG_STYLE_LIST, "�ҵ�����", "��ȫ����\n�ҵ�װ��\n�ҵļҾ�\n�ҵİ���\n�ҵ���ɫ\n�ҵĳƺ�\n�ҵ�Сβ��\n���Ի�����", "����", "ȡ��");
    return 1;
}
CMD:fxq(const playerid, const cmdtext[]) { //���������/fxq��ʱ�� ����ҷ�����   ������
    SetPlayerSpecialAction(playerid, 2);
    return 1;
}
CMD:jetpack(const playerid, const cmdtext[]) { //���������/fxq��ʱ�� ����ҷ�����   ������
    SetPlayerSpecialAction(playerid, 2);
    return 1;
}
CMD:weather(const playerid, const cmdtext[]) {
    new tweather;
    if(sscanf(cmdtext, "i", tweather)) return SCM(playerid, Color_LightBlue, "[����]�÷�:/tianqi [����ID] ��ΧΪ0~255 ����0~20Ϊ����ID"); //����������ָ���ʱ��
    if(tweather < 0 || tweather > 256) return SCM(playerid, Color_LightBlue, "[����]ID���󣬷�ΧΪ0~255"); //����������ָ���ʱ��
    SetPlayerWeather(playerid, tweather);
    PlayerInfo[playerid][tWeather] = tweather;
    new str[128];
    format(str, sizeof(str), "[����]�㽫�Լ�����������Ϊ \"%d\"", tweather);
    SCM(playerid, Color_LightBlue, str); //������������������óɶ���
    return 1;
}
CMD:tianqi(const playerid, const cmdtext[]) return cmd_weather(playerid, cmdtext);
//����ʱ��
CMD:time(const playerid, const cmdtext[]) {
    new hour, minute;
    if(sscanf(cmdtext, "ii", hour, minute)) return SCM(playerid, Color_LightBlue, "[ʱ��] /time ʱ �� СʱΪ0~24,��Ϊ0~59");
    if(hour < 0 || hour > 24) return SCM(playerid, Color_LightBlue, "[ʱ��] /time ʱ �� СʱΪ0~24,��Ϊ0~59");
    if(minute < 0 || minute > 59) return SCM(playerid, Color_LightBlue, "[ʱ��] /time ʱ �� СʱΪ0~24,��Ϊ0~59");
    SetPlayerTime(playerid, hour, minute);
    PlayerInfo[playerid][tHour] = hour;
    PlayerInfo[playerid][tMinute] = minute;
    new str[90];
    format(str, sizeof(str), "[ʱ��] �㽫�Լ���ʱ������Ϊ %02d:%02d ", hour, minute);
    SCM(playerid, Color_LightBlue, str);
    return 1;
}
CMD:shijian(const playerid, const cmdtext[]) return cmd_time(playerid, cmdtext);
CMD:wuqi(const playerid, const cmdtext[]) return Dialog_Show(playerid, weapons, DIALOG_STYLE_LIST, "�����˵�", "С��\n�����\n��ͨ��ǹ\n������ǹ\nɳĮ֮ӥ\nɢ����ǹ\n�̹�ɢ��ǹ\nս��ɢ��ǹ\nTec-9ʽ΢��\n���ȳ��ǹ\nMP5΢�ͳ��ǹ\nAK-47�Զ���ǹ\nM4�Զ���ǹ\nС�;ѻ�ǹ\n��׼���;ѻ�ǹ\n���Ͳ\n��Դ׷���ͻ��Ͳ(RPG)\n�����\n����\n����\nơ��ƿ\nң�ض�ʱը��\nͿѻƿ\n�����\n�������������޵м�����", "ˢ��", "�ر�");
CMD:f(const playerid, const cmdtext[]) {
    if(!IsPlayerInAnyVehicle(playerid)) return SCM(playerid, 0xAA3333AA, "[��ͨ����] �����㲻�ڳ���");
    if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SCM(playerid, Color_Orange, "[��ͨ����] �㲻��˾��!");
    new Float:POS[3];
    new Float:ZAngle;
    GetPlayerPos(playerid, POS[0], POS[1], POS[2]);
    GetVehicleZAngle(GetPlayerVehicleID(playerid), ZAngle);
    SCM(playerid, Color_White, "[��ͨ����] �㷭���˳�����");
    SetVehiclePos(GetPlayerVehicleID(playerid), POS[0], POS[1], POS[2] + 2);
    SetVehicleZAngle(GetPlayerVehicleID(playerid), ZAngle);
    return 1;
}
//����ʱ
CMD:count(const playerid, const cmdtext[]) return cmd_djs(playerid, cmdtext);
CMD:daojishi(const playerid, const cmdtext[]) return cmd_djs(playerid, cmdtext);
CMD:djs(const playerid, const cmdtext[]) {
    // new count;
    // if(sscanf(cmdtext, "i", count)) count = 6; //������û���������� Ĭ��6-1 = 5��
    // if(count > 30) return SendClientMessage(playerid, 0xFFFF00AA, "[����ʱ]����ʱ�䲻�ɳ���30s");
    // if(CountDown == -1) {
    //     new string[128];
    //     format(string, sizeof(string), "[����ʱ] �� %s (%d) ����ĵ���ʱ��ʼ", GetName(playerid), playerid);
    //     SCMALL(0xFFFF00AA, string);
    //     CountDown = count;
    //     SetTimer_("countdown", 1000, 0);
    //     return 1;
    // } 
    // else return SCM(playerid, Color_Red, "[����ʱ] ����:����ʱ���ڽ���");
    if(Count[playerid]) return SCM(playerid, Color_Red, "[����ʱ] ����:����ʱ���ڽ���");
    new Float:X, Float:Y, Float:Z;
    GetPlayerPos(playerid, X, Y, Z);
    Timer[playerid] = SetTimerEx_("CountDown", 1000, 1000, -1, "d", playerid);
    for (new i = GetPlayerPoolSize(); i >= 0; i--) {
        if(IsPlayerConnected(i)) {
            if(IsPlayerInRangeOfPoint(i, 20, X, Y, Z) && GetPlayerVirtualWorld(i) == GetPlayerVirtualWorld(playerid)) {
                new str[16];
                Count[playerid] = 5;
                format(str, sizeof(str), "~w~%d", Count[playerid]);
                GameTextForPlayer(i, str, 3000, 3);
                PlayerPlaySound(i, 1056, 0.0, 0.0, 0.0);
            }
        }
    }
    return 1;
}
//�޸�һ�γ�
CMD:fix(const playerid, const cmdtext[]) {
    if(!IsPlayerInAnyVehicle(playerid)) return SCM(playerid, Color_White, " ����:�㲻�ڳ���");
    RepairVehicle(GetPlayerVehicleID(playerid));
    SCM(playerid, Color_Orange, "[��ͨ����] ��ĳ����޸�");
    return 1;
}
CMD:dcar(const playerid, const cmdtext[]) {
    if(PlayerInfo[playerid][AutoFix]) {
        SCM(playerid, Color_Orange, "[��ͨ����] ��ر��˳����޵У��ٴ�����/dcar����");
        PlayerInfo[playerid][AutoFix] = 0;
    } else {
        SCM(playerid, Color_Orange, "[��ͨ����] ���Ѿ������˳����޵У��ٴ�����/dcar�ر�");
        PlayerInfo[playerid][AutoFix] = 1;
    }
    return 1;
}
CMD:autofix(const playerid, const cmdtext[]) {
    if(PlayerInfo[playerid][AutoFix]) {
        SCM(playerid, Color_Orange, "[��ͨ����] ��ر��˳����޵У��ٴ�����/dcar����");
        PlayerInfo[playerid][AutoFix] = 0;
    } else {
        SCM(playerid, Color_Orange, "[��ͨ����] ���Ѿ������˳����޵У��ٴ�����/dcar�ر�");
        PlayerInfo[playerid][AutoFix] = 1;
    }
    return 1;
}
CMD:jls(const playerid, const cmdtext[]) { //���������/jls��ʱ�� �����һ������ɡ
    GivePlayerWeapon(playerid, 46, 1);
    SCM(playerid, Color_LightBlue, "������һ������ɡ");
    return 1;
}
CMD:c(const playerid, const cmdtext[]) //ˢ��
{
    if(GetPlayerCreditpoints(playerid) <= 90) return SendClientMessage(playerid, Color_Yellow, "[ϵͳ]������Ϸ�����ֹ���,�뽡����Ϸ,�����Զ�����");
    // printf("%s", cmdtext);
    new carcmd[128], str[128], idx;
    carcmd = strtok(cmdtext, idx);
    if(!strlen(carcmd)) {
        SCM(playerid, Color_LightBlue, "  ��������ˢ��������������");
        SCM(playerid, Color_LightBlue, " /c [����ID]��ˢ��������IDΪ400-611");
        SCM(playerid, Color_LightBlue, " /cc ��ɫ���� ��ɫ���� ����������ɫ");
        SCM(playerid, Color_LightBlue, " /f ���� /c kick ���� /c wode ˢ����ˢ���ĳ�");
        SCM(playerid, Color_LightBlue, " /c lock ���� /c chepai �������� /c listͼƬˢ�� /c 3d��ʾ/����3D�ٶȱ�");
        return 1;
    }
    if(strcmp(carcmd, "list", true) == 0) {
        Dialog_Show(playerid, Dialog_SpawnVehicle, DIALOG_STYLE_LIST, "------------ˢ���б�-------", "\n�ܳ�\n����\n�ɻ�\nĦ��\n��\nԽҰ\n�ϳ�\n����\n�𳵼���߳�\n������\n������", "ȷ��", "ȡ��");
        // 2020.2.10 �޸��б�ˢ������ֵ����
        return 1;
    }
    if(strcmp(carcmd, "kick", true) == 0) {
        if(PlayerInfo[playerid][BuyID] == 0) return SCM(playerid, Color_Orange, "[��ͨ����]�㶼û��, ��ʲô��?");
        new ren = 0, i;
        for (i = 0; i < MAX_PLAYERS; i++)
            if(IsPlayerConnected(i) && GetPlayerVehicleID(i) == PlayerInfo[playerid][BuyID] && IsPlayerInAnyVehicle(playerid) == 1 && (i > playerid || i < playerid)) {
                new Float:x, Float:y, Float:z;
                GetPlayerPos(i, x, y, z);
                SetPlayerPos(i, x, y, z + 5);
                format(str, sizeof(str), "[��ͨ����] �ó��������� %s , �Ѿ�����.", GetName(i));
                SCM(i, Color_Orange, str);
                ren = 1;
            }
        if(ren == 0) SCM(playerid, Color_Orange, "[��ͨ����]�㳵��û�˰�, ������.");
        return 1;
    }
    if(strcmp(carcmd, "wode", true) == 0) {
        if(PlayerInfo[playerid][BuyID] == 0) {
            SCM(playerid, Color_Orange, "[��ͨ����]�㶼û��, ��ʲô��?");
        } else {
            new Float:POS[3], Float:Angle;
            if(IsPlayerInAnyVehicle(playerid)) GetVehicleZAngle(GetPlayerVehicleID(playerid), Angle);
            else GetPlayerFacingAngle(playerid, Angle);
            GetPlayerPos(playerid, POS[0], POS[1], POS[2]);
            if(IsPlayerInAnyVehicle(playerid)) SetPlayerPos(playerid, POS[0], POS[1], POS[2]);
            // for (new i; i < MAX_PLAYERS; i++) //˵ʵ��û����д��仰������ͼ��ɶ
            // {
            //     if(IsPlayerConnected(i) && IsPlayerInAnyVehicle(i) && GetPlayerVehicleID(i) == PlayerInfo[playerid][BuyID]) SetPlayerPos(playerid, POS[0], POS[1], POS[2]);
            // }
            SetVehiclePos(PlayerInfo[playerid][BuyID], POS[0], POS[1], POS[2]);
            SetVehicleVirtualWorld(PlayerInfo[playerid][BuyID], GetVehicleVirtualWorld(playerid));
            PutPlayerInVehicle(playerid, PlayerInfo[playerid][BuyID], 0);
            //PlayerInfo[playerid][pVehicleEnteColor_Red] = GetPlayerVehicleID(playerid);
            SetVehicleZAngle(PlayerInfo[playerid][BuyID], Angle);
            LinkVehicleToInterior(PlayerInfo[playerid][BuyID], GetPlayerInterior(playerid));
            // SetTimerEx("CarCheck", 500, false, "i", playerid);
        }
        return 1;
    }
    if(strcmp(carcmd, "color", true) == 0) {
        if(!IsPlayerInAnyVehicle(playerid)) return SCM(playerid, Color_White, "[��ͨ����] ����:�㲻�ڳ���");
        new tmp[128];
        new color1, color2;
        tmp = strtok(cmdtext, idx);
        if(!strlen(tmp)) return SCM(playerid, Color_White, "[��ͨ����] ��ʽ:/cc ��ɫ ��ɫ");
        color1 = strval(tmp);
        tmp = strtok(cmdtext, idx);
        color2 = strval(tmp);
        if(color1 < 0 || color1 > 255) return SCM(playerid, Color_White, "[��ͨ����] ������ɫ���룬������ɫ����Ϊ0-255"); {
            ChangeVehicleColor(GetPlayerVehicleID(playerid), color1, color2);
            SCM(playerid, Color_White, "[��ͨ����] ������˳�������ɫ��");
        }
    }
    if(strcmp(carcmd, "lock", true) == 0) //����
    {
        if(PlayerInfo[playerid][BuyID] == 0) return SCM(playerid, Color_White, "[��ͨ����]�㶼û��, ��ʲô��?");
        if(PlayerInfo[playerid][CarLock] == 0) {
            PlayerInfo[playerid][CarLock] = 1;
            SCM(playerid, Color_White, "[��ͨ����]��ĳ�������");
        } else {
            PlayerInfo[playerid][CarLock] = 0;
            SCM(playerid, Color_White, "[��ͨ����]��ĳ��ѽ���");
        }
        return 1;
    }
    if(strcmp(carcmd, "chepai", true) == 0) //�����ò��ˡ�����������
    {
        new tmp[128];
        tmp = strtok(cmdtext, idx);
        if(!strlen(tmp)) return SCM(playerid, Color_White, "[��ͨ����] �÷�:/c chepai Ҫ���õĳ���");
        if(strlen(tmp) > 10) return SCM(playerid, Color_White, "[��ͨ����] ����:ֻ������5λ���ֻ�10λӢ����ĸ");
        if(GetPlayerVehicleID(playerid) != PlayerInfo[playerid][BuyID]) return SCM(playerid, Color_White, "[��ͨ����] ����:�㲻���Լ����ؾ���");
        SetVehicleNumberPlate(PlayerInfo[playerid][BuyID], tmp);
        SCM(playerid, Color_White, "[��ͨ����] �������Ƴɹ�");
        new Float:x, Float:y, Float:z, Float:z_angle;
        GetPlayerPos(playerid, x, y, z);
        GetVehicleZAngle(PlayerInfo[playerid][BuyID], z_angle);
        SetPlayerPos(playerid, x, y, z);
        SetVehicleToRespawn(PlayerInfo[playerid][BuyID]);
        SetVehiclePos(PlayerInfo[playerid][BuyID], x, y, z);
        SetVehicleZAngle(PlayerInfo[playerid][BuyID], z_angle);
        PutPlayerInVehicle(playerid, PlayerInfo[playerid][BuyID], 0);
        AddVehicleComponent(PlayerInfo[playerid][BuyID], 1010);
        return 1;
    }
    if(strcmp(carcmd, "3d", true) == 0) {
        if(!IsPlayerInAnyVehicle(playerid)) return SCM(playerid, Color_White, "[��ͨ����]�㻹û�����κ�һ������Ŷ");
        if(PlayerInfo[playerid][BuyID] == 0) return SCM(playerid, Color_White, "[��ͨ����]�����������û�г�");
        new vehicleid = GetPlayerVehicleID(playerid);
        if(CarInfo[vehicleid][ID] != 0) {
            if(CarInfo[vehicleid][UsersID] != PlayerInfo[playerid][ID]) return SCM(playerid, Color_White, "[��ͨ����]����������������Ŷ");
        } else {
            if(GetPlayerVehicleID(playerid) != PlayerInfo[playerid][BuyID]) return SCM(playerid, Color_White, "[��ͨ����] ����:�㲻���Լ����ؾ���");
        }
        if(PlayerInfo[playerid][tdEnabled]) {
            tdSpeedo_Toggle(playerid, 0);
            SCM(playerid, Color_White, "[��ͨ����]������3D�ٶȱ�");
            return 1;
        }
        tdSpeedo_Toggle(playerid, 1);
        SCM(playerid, Color_White, "[��ͨ����]����ʾ3D�ٶȱ�");
        return 1;
    }
    new car;
    car = strval(carcmd);
    if(car < 400 || car > 611) return SCM(playerid, Color_White, "[��ͨ����] ����ID������{FFFFFF}400-611֮�䣡");
    SpawnVehicle(playerid, car);
    //2020.1.12 16:20�䶯
    // new Float:x, Float:y, Float:z;
    // GetPlayerPos(playerid, x, y, z);
    // SetPlayerPos(playerid, x, y, z);
    // SetTimerEx("SpawnVehicle", 300, false, "ii", playerid, car);
    return 1;
}
CMD:cc(const playerid, const cmdtext[]) { //��������ؾ���ɫ
    if(!IsPlayerInAnyVehicle(playerid)) return SCM(playerid, Color_White, "[��ͨ����] ����:�㲻�ڳ���");
    new color1, color2;
    if(sscanf(cmdtext, "ii", color1, color2)) return SCM(playerid, Color_White, "[��ͨ����] ��ʽ:/cc ��ɫ ��ɫ");
    if(color1 < 0 || color1 > 255) return SCM(playerid, Color_White, "[��ͨ����] ������ɫ���룬������ɫ����Ϊ0-255"); {
        ChangeVehicleColor(GetPlayerVehicleID(playerid), color1, color2);
        SCM(playerid, Color_White, "[��ͨ����] ������˳�������ɫ��");
    }
    return 1;
}

// CMD:ww(const playerid, const cmdtext[]) { //2020.3.9����
//     // 2020.3.15�޸�����ר�����粻����ؾ߷Ž�ȥ������
//     if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), playerid + 1);
//     SetPlayerVirtualWorld(playerid, playerid + 1);
//     SCM(playerid, Color_Orange, "[ϵͳ] ����������ר������.");
//     for (new i = GetPlayerPoolSize(); i >= 0; i--) {
//         if(IsPlayerConnected(i)) {
//             if(PlayerInfo[i][tvid] == playerid && i != playerid) {
//                 SetPlayerVirtualWorld(i, GetPlayerVirtualWorld(playerid));
//                 SetPlayerInterior(i, GetPlayerInterior(playerid));
//                 if(IsPlayerInAnyVehicle(i)) PlayerSpectateVehicle(i, GetPlayerVehicleID(playerid));
//                 else PlayerSpectatePlayer(i, playerid);
//                 SCM(i, Color_Orange, "[TV]:�Է��л������磬�Զ�׷�ٹۿ�.");
//             }
//         }
//     }
//     return 1;
// }
CMD:pos(const playerid, const cmdtext[]) {
    new Float:x, Float:y, Float:z, str[128];
    GetPlayerPos(playerid, x, y, z);
    format(str, 128, "[ϵͳ] ��Ŀǰ������Ϊ X:%f, Y:%f, Z:%f", x, y, z);
    SCM(playerid, Color_LightBlue, str);
    if(IsPlayerInAnyVehicle(playerid)) {
        new Float:angle = 0.0;
        GetVehicleZAngle(GetPlayerVehicleID(playerid), angle);
        format(str, 128, "[ϵͳ] ��ĳ���Z�Ƕ�Ϊ%f", angle);
        SCM(playerid, Color_LightBlue, str);
    } else {
        new Float:angle = 0.0;
        GetPlayerFacingAngle(playerid, angle);
        format(str, 128, "[ϵͳ] ������ﳯ��Ƕ�Ϊ%f", angle);
        SCM(playerid, Color_LightBlue, str);
    }
    return 1;
}
CMD:wudi(const playerid, const cmdtext[]) { //����޵�
    if(!PlayerInfo[playerid][enableInvincible]) {
        SCM(playerid, 0x0FFF00FF, "[�޵�]�㿪�����޵�״̬,�ٴ�����/wudi�ر�");
        SetPlayerHealth(playerid, 999999999);
        PlayerInfo[playerid][enableInvincible] = 1;
    } else {
        SCM(playerid, 0x0FFF00FF, "[�޵�]��ر����޵�״̬,�ٴ�����/wudi����");
        SetPlayerHealth(playerid, 100);
        PlayerInfo[playerid][enableInvincible] = 0;
    }
    return 1;
}
CMD:hys(const playerid, const cmdtext[]) { //������ɫ��
    if(PlayerInfo[playerid][hys]) {
        PlayerInfo[playerid][hys] = false;
        SendClientMessage(playerid, Color_White, "[ϵͳ*]�رճ����Զ���ɫ");
    } else {
        PlayerInfo[playerid][hys] = true;
        SendClientMessage(playerid, Color_White, "[ϵͳ*]���������Զ���ɫ");
    }
    return 1;
}
CMD:infobj(const playerid, const cmdtext[]) { //�̳�dylanʱ���������ľ���
    if(!IsPlayerInAnyVehicle(playerid)) return SCM(playerid, Color_Announcement, "[infobj] �㲻�ڳ���!");
    new Float:pX, Float:pY, Float:pZ;
    GetPlayerPos(playerid, pX, pY, pZ);
    if(dinfobj[playerid] == 0) {
        dinfobj[playerid] = 1;
        jd[playerid] = CreateDynamicObject(1001, pX, pY, pZ, 0.0, 0.0, 0.0, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1);
        wy[playerid] = CreateDynamicObject(18646, pX, pY, pZ, 0.0, 0.0, 0.0, -1, -1, -1, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD, -1);
        AttachDynamicObjectToVehicle(jd[playerid], GetPlayerVehicleID(playerid), -0.05, -2.35, 0.3099, 0, 0, 0);
        AttachDynamicObjectToVehicle(wy[playerid], GetPlayerVehicleID(playerid), -0.48, 0.28, 0.7099, 0, 0, 0);
        // AttachObjectToVehicle(jd[playerid], GetPlayerVehicleID(playerid), -0.05, -2.35, 0.3099, 0, 0, 0);
        // AttachObjectToVehicle(wy[playerid], GetPlayerVehicleID(playerid), -0.48, 0.28, 0.7099, 0, 0, 0);
        SCM(playerid, Color_Announcement, "[infobj] ������������INFOBJ");
    } else {
        dinfobj[playerid] = 0;
        DestroyDynamicObject(jd[playerid]); //���پ���
        DestroyDynamicObject(wy[playerid]); //����β��
        // DestroyObject(jd[playerid]);
        // DestroyObject(wy[playerid]);
        SCM(playerid, Color_Announcement, "[infobj] �������ɾȥ��INFOBJ");
    }
    return 1;
}
CMD:vmake(const playerid, const cmdtext[]) { //��Ҵ�������
    if(PlayerInfo[playerid][Score] < 30) return SCM(playerid, 0xFFFF00FF, "[����] Ϊ��ֹ��ҹ��Ƚ������͵㣬��ʱ��ִﵽ30�����ϲſɴ���.");
    if(PlayerInfo[playerid][Cash] < 30) return SCM(playerid, 0xFFFF00FF, "[����] ��Ҫ30��Ҵ���һ�δ��͵�.");
    new tmp[48];
    // tmp = strtok(cmdtext, idx);
    if(sscanf(cmdtext, "s[48]", tmp)) return SCM(playerid, 0xFFFF00FF, "[����] �÷�:/vmake [�����ǳ�,���ô�'/'] ����/vmake sf");
    if(strlen(tmp) >= 48) return SCM(playerid, 0xFFFF00FF, "[����] ���ֹ���������֧��48λӢ��/���ֻ�24λ����");
    if(make_findgo(tmp) != -1) return SCM(playerid, 0xFFFF00FF, "[����] �ô��͵��Ѿ�������.");
    new sb = make_getindex();
    if(sb == -1) return SCM(playerid, 0xFFFF00FF, "[����] ���͵��Ѵﵽ����.");
    pVmakePos(playerid, sb, tmp);
    return 1;
}
CMD:vsmake(const playerid, const cmdtext[]) { //����Ա����ϵͳ����
    if(PlayerInfo[playerid][AdminLevel] < 4) return 0;
    if(PlayerInfo[playerid][Cash] < 1000) return SCM(playerid, 0xFFFF00FF, "[����] ��Ҫ1000��Ҵ���һ��ϵͳ���͵�.");
    new tmp[48], describe[48];
    if(sscanf(cmdtext, "s[48]s[48]", tmp, describe)) return SCM(playerid, 0xFFFF00FF, "[����] �÷�:/vsmake [�����ǳ�,���ô�'/'] [����<���ɴ��ո�,����ᱻ��>] ����/vsmake sf SF����");
    if(strlen(tmp) >= 48) return SCM(playerid, 0xFFFF00FF, "[����] ���ֹ���������֧��48λӢ��/���ֻ�32λ����");
    if(strlen(describe) >= 48) return SCM(playerid, 0xFFFF00FF, "[����] ��������������֧��48λӢ��/���ֻ�32λ����");
    if(strfind(describe, "{", true) != -1 || strfind(describe, " ", true) != -1 || strfind(describe, "}", true) != -1) {
        SCM(playerid, 0xFFFF00FF, "[����] �����в��ɴ��ո�������ַ�");
        return 1;
    }
    if(strfind(describe, "[", true) != -1 || strfind(describe, "]", true) != -1) {
        SCM(playerid, 0xFFFF00FF, "[����] �����в��ɴ��ո�������ַ�");
        return 1;
    }
    if(make_Sysfindgo(tmp) != -1) return SCM(playerid, 0xFFFF00FF, "[����] ��ϵͳ���͵��Ѿ�������.");
    new sb = make_sysgetindex();
    if(sb == -1) return SCM(playerid, 0xFFFF00FF, "[����] ϵͳ���͵��Ѵﵽ����.");
    pVmakeSysPos(playerid, sb, tmp, describe);
    new str[96];
    format(str, sizeof(str), "[����Ա] %s ������ϵͳ���͵� %s. ", GetName(playerid), tmp);
    SCMToAdmins(Color_AdminMessage, str);
    GivePlayerCash(playerid, -1000);
    return 1;
}

//���������tpa
CMD:tpa(const playerid, const cmdtext[]) {
    new id, tmp[128], idx;
    tmp = strtok(cmdtext, idx);
    if(strcmp(tmp, "ban", true) == 0) {
        pVShieldingChoose(playerid);
        return 1;
    }
    if(sscanf(cmdtext, "i", id)) return SCM(playerid, Color_White, "[TP] * ��ʹ��:/tpa ID");
    pVGotoRequest(playerid, id);
    // if(tpaB[id] == 1) return SCM(playerid, Color_White, "[TP] * * {FFA8FF}�Է����ڿ��Ǳ��˵Ĵ�������.");
    // if(tpaB[id] == 2) return SCM(playerid, Color_White, "[TP] * {FFA8FF}���������͵�����������.");
    // if(tpaB[id] == 3) return SCM(playerid, Color_White, "[TP] * {FFA8FF}�Է��Ѿ������˴���������Ϣ.");
    // if(playerid == id) return SCM(playerid, Color_White, "[TP] * �㲻��tpa���Լ�");
    // tpaid[playerid] = id;
    // tpaid[id] = playerid;
    // tpaB[id] = 1;
    // tpaB[playerid] = 2;
    // format(tmp, sizeof(tmp), "[TP] * �������͵� {FFFFFF}%s(%d)���,��ȴ��ظ�.", GetName(id), id);
    // SCM(playerid, Color_White, tmp);
    // format(tmp, sizeof(tmp), "[TP] * һ��TPA�������� %s(%d) /taͬ�� /td �ܾ�", GetName(playerid), playerid);
    // SCM(id, Color_White, tmp);
    // SCM(id, Color_White, "[TP] * ����{00FF80}����{FFFF00}/{FF0000}ȡ��{FFFFFF}����ģʽ,/tpa ban");
    // GameTextForPlayer(id, "~n~~n~~n~~n~~n~~n~~n~~n~~w~Player want to move ~r~you~w~.", 3000, 3);
    // tpatime[playerid] = SetTimerEx("tpaTimer", 60000, false, "i", playerid);
    // tpatime[id] = SetTimerEx("tpaTimer", 60000, false, "i", id);
    return 1;
}
CMD:tp(const playerid, const cmdtext[]) return cmd_tpa(playerid, cmdtext);
CMD:yes(const playerid, const cmdtext[]) return cmd_ta(playerid, cmdtext);
CMD:no(const playerid, const cmdtext[]) return cmd_td(playerid, cmdtext);
CMD:ta(const playerid, const cmdtext[]) {
    if(PlayerInfo[playerid][tpa_gotoid] != -1) {
        pVRequestAccept(playerid);
    }
    // if(tpaB[playerid] == 1) {
    //     new Float:x, Float:y, Float:z, Float:Angle;
    //     new id = tpaid[playerid];
    //     if(pRaceing[id]) {
    //         tpaB[playerid] = 0;
    //         tpaB[id] = 0;
    //         KillTimer(tpatime[playerid]);
    //         KillTimer(tpatime[id]);
    //         tpaid[playerid] = -1;
    //         tpaid[id] = -1;
    //         return SendClientMessage(playerid, Color_White, "�Է�������������,���ܽ���TP");
    //         //2020.2.20����
    //     }
    //     GetPlayerPos(playerid, x, y, z);
    //     if(IsPlayerInAnyVehicle(tpaid[playerid])) {
    //         GetVehicleZAngle(GetPlayerVehicleID(tpaid[playerid]), Angle);
    //     } else GetPlayerFacingAngle(tpaid[playerid], Angle);
    //     if(IsPlayerInAnyVehicle(tpaid[playerid])) {
    //         SetVehiclePos(GetPlayerVehicleID(tpaid[playerid]), x + 3, y + 1, z);
    //         LinkVehicleToInterior(GetPlayerVehicleID(tpaid[playerid]), GetPlayerInterior(tpaid[playerid]));
    //         SetVehicleZAngle(GetPlayerVehicleID(tpaid[playerid]), Angle);
    //         SCM(playerid, Color_White, "[TP] * ��ɹ�������tpa����");
    //         SCM(tpaid[playerid], Color_White, "[TP] * ���Ѵ��͵�Ta��ߣ����Ta����С���磬��˽��Ta.");
    //         tpaB[playerid] = 0;
    //         tpaB[id] = 0;
    //         KillTimer(tpatime[playerid]);
    //         KillTimer(tpatime[id]);
    //         tpaid[playerid] = -1;
    //         tpaid[id] = -1;
    //     } else {
    //         SetPlayerPos(tpaid[playerid], x + 3, y + 1, z);
    //         SetPlayerFacingAngle(tpaid[playerid], Angle);
    //         SCM(playerid, Color_White, "[TP] * ��ɹ�������tpa����");
    //         SCM(tpaid[playerid], Color_White, "[TP] * ���Ѵ��͵�Ta��ߣ����Ta����С���磬��˽��Ta.");
    //         KillTimer(tpatime[playerid]);
    //         KillTimer(tpatime[id]);
    //         tpaB[playerid] = 0;
    //         tpaB[id] = 0;
    //         tpaid[playerid] = -1;
    //         tpaid[id] = -1;
    //     }
    // } else {
    //     SCM(playerid, Color_White, "[TP] * �㵱ǰû��tpa��������");
    // }
    return 1;
}
stock pVShieldingChoose(const playerid) {
    if(PlayerInfo[playerid][tpa_ban] == 0) {
        PlayerInfo[playerid][tpa_ban] = 1;
        SCM(playerid, Color_Yellow, "[TP] * �������˴�������,�ٴ�����ɿ���.");
        return 1;
    }
    PlayerInfo[playerid][tpa_ban] = 0;
    SCM(playerid, Color_Yellow, "[TP] * �㿪���˴�������,�ٴ����������.");
    return 1;
}
stock pVGotoRequest(const playerid, const pid) {
    if(pid == playerid) return SCM(playerid, Color_White, "[TP] * �㲻�������Լ����ʹ�������.");
    if(IsPlayerConnected(pid) == 0) return SCM(playerid, Color_White, "[TP] * ����ĶԷ�ID.");
    if(IsPlayerNPC(pid)) return SCM(playerid, Color_White, "[TP] * ����tpa��NPC���");
    if(!PlayerInfo[pid][Login]) return SCM(playerid, Color_White, "[TP] * �Է���δ���ߣ�");
    if(PlayerInfo[pid][tpa_ban] == 1) return SCM(playerid, Color_White, "[TP] * �Է������˴�������.");
    if(PlayerInfo[pid][tpa_gotoid] != -1) return SCM(playerid, Color_White, "[TP] * �Է����ڿ���������.");
    if(pRaceing[pid]) return SCM(playerid, Color_White, "�Է�������������,���ܽ���TP");
    new str[128];
    format(str, sizeof(str), "[TP] * %s(%d) �����͵�������.(/ta)", GetName(playerid), playerid);
    SCM(pid, Color_Yellow, str);
    format(str, sizeof(str), "[TP] * �����ѷ��� %s(%d)", GetName(pid), pid);
    GameTextForPlayer(pid, "~n~~n~~n~~n~~n~~n~~n~~n~~w~Player want to move ~r~you~w~.", 3000, 3);
    SCM(playerid, Color_Yellow, str);
    PlayerInfo[pid][tpa_gotoid] = playerid;
    PlayerInfo[pid][tpa_requesttimer] = 18;
    return 1;
}
stock pVRequestAccept(const playerid) {
    if(PlayerInfo[playerid][tpa_gotoid] != -1) {
        if(PlayerInfo[PlayerInfo[playerid][tpa_gotoid]][tpa_ban] == 1) {
            SCM(playerid, Color_White, "[TP] * �Է������˴�������.");
            Initialize_tpa(playerid);
            return 1;
        }
        if(IsPlayerConnected(PlayerInfo[playerid][tpa_gotoid]) == 0) {
            SCM(playerid, Color_White, "[TP] * �Է�������.");
            Initialize_tpa(playerid);
            return 1;
        }
        new Float:x, Float:y, Float:z, str[128];
        GetPlayerPos(playerid, x, y, z);
        SetPlayerPosEx(PlayerInfo[playerid][tpa_gotoid], x, y, z + 3);
        format(str, sizeof(str), "[TP] * %s(%d) ͬ������Ĵ�������.", GetName(playerid), playerid);
        SCM(PlayerInfo[playerid][tpa_gotoid], Color_Yellow, str);
        format(str, sizeof(str), "[TP] * ��ͬ���� %s �Ĵ�������.", GetName(PlayerInfo[playerid][tpa_gotoid]), PlayerInfo[playerid][tpa_gotoid]);
        SCM(playerid, Color_Yellow, str);
        Initialize_tpa(playerid);
    }
    return 1;
}
stock pVRequestReject(const playerid) {
    if(PlayerInfo[playerid][tpa_gotoid] != -1) {
        if(PlayerInfo[PlayerInfo[playerid][tpa_gotoid]][tpa_ban] == 1) {
            SCM(playerid, Color_White, "[TP] * �Է������˴�������.");
            Initialize_tpa(playerid);
            return 1;
        }
        if(IsPlayerConnected(PlayerInfo[playerid][tpa_gotoid]) == 0) {
            SCM(playerid, Color_White, "[TP] * �Է�������.");
            Initialize_tpa(playerid);
            return 1;
        }
        new str[128];
        format(str, sizeof(str), "[TP] * %s(%d) �ܾ�����Ĵ�������.", GetName(playerid), playerid);
        SCM(PlayerInfo[playerid][tpa_gotoid], Color_Yellow, str);
        format(str, sizeof(str), "[TP] * ��ܾ��� %s �Ĵ�������.", GetName(PlayerInfo[playerid][tpa_gotoid]), PlayerInfo[playerid][tpa_gotoid]);
        SCM(playerid, Color_Yellow, str);
        Initialize_tpa(PlayerInfo[playerid][tpa_gotoid]);
    }
    return 1;
}
CMD:td(const playerid, const cmdtext[]) {
    if(PlayerInfo[playerid][tpa_gotoid] != -1) {
        pVRequestReject(playerid);
    }
    // new id = tpaid[playerid];
    // if(tpaB[playerid] == 1) {
    //     tpaB[playerid] = 0;
    //     tpaB[id] = 0;
    //     tpaid[playerid] = -1;
    //     tpaid[id] = -1;
    //     SCM(playerid, Color_Red, "[tp] ��ܾ��˶Է�����!");
    //     SCM(tpaid[playerid], Color_Red, "[tp] �Է��ܾ����������!");
    //     KillTimer(tpatime[playerid]);
    //     KillTimer(tpatime[id]);
    // } else {
    //     SCM(playerid, Color_Red, "[tp] ��ʱû�д�������!");
    //     KillTimer(tpatime[playerid]);
    //     KillTimer(tpatime[id]);
    //     tpaB[playerid] = 0;
    //     tpaB[id] = 0;
    //     tpaid[playerid] = -1;
    //     tpaid[id] = -1;
    // }
    return 1;
}
//����/�ر���ʾ�������
// CMD:name(const playerid, const cmdtext[]) {
//     new tmp[128];
//     if(sscanf(cmdtext, "s[128]", tmp)) return SCM(playerid, Color_White, "[ϵͳ]/name on���� off�ر�");
//     if(strcmp(tmp, "off", true) == 0) {
//         for (new i = GetPlayerPoolSize(); i >= 0; i--) 
//         {
//             ShowPlayerNameTagForPlayer(playerid, i, false);
//         }
//         SCM(playerid, Color_White, "[ϵͳ] �����������������.");
//         return 1;
//     }
//     if(strcmp(tmp, "on", true) == 0) {
//         // for (new i = GetPlayerPoolSize(); i != -1; --i) {
//         for (new i = GetPlayerPoolSize(); i >= 0; i--) {
//             ShowPlayerNameTagForPlayer(playerid, i, true);
//         }
//         SCM(playerid, Color_White, "[ϵͳ] ����ʾ�����������.");
//         return 1;
//     }
//     return 1;
// }
//��ҹ�սϵͳ
CMD:tv(const playerid, const cmdtext[]) {
    new tmp[128];
    if(sscanf(cmdtext, "s[128]", tmp)) {
        SCM(playerid, Color_Orange, "[TV] |____________TV������___________|");
        SCM(playerid, Color_Orange, "[TV] |ʹ��:/tv [ID] �������          |");
        SCM(playerid, Color_Orange, "[TV] |��ʾ:/tv off  �رռ���          |");
        return 1;
    }
    if(strcmp(tmp, "off", true) == 0) {
        if(!PlayerInfo[playerid][tvzt]) return SCM(playerid, Color_Orange, "[TV]:������û����TV״̬�£�");
        for (new a = 0; a <= 21; a++) {
            PlayerTextDrawHide(playerid, velo[PlayerInfo[playerid][tvid]][a]);
        }
        if(PlayerInfo[playerid][speedoMeter]) {
            for (new a = 0; a <= 21; a++) {
                PlayerTextDrawShow(playerid, velo[playerid][a]);
            }
        }
        // ˳��ӵ�race_game_quit��ȥ��
        if(pRaceing[PlayerInfo[playerid][tvid]]) {
            DestroyPRaceTextDraw(playerid);
            // PlayerTextDrawHide(playerid, CpTextDraw[PlayerInfo[playerid][tvid]]);
            // PlayerTextDrawHide(playerid, Time[PlayerInfo[playerid][tvid]]);
            // PlayerTextDrawHide(playerid, Top[PlayerInfo[playerid][tvid]]);
            // PlayerTextDrawHide(playerid, p_record[PlayerInfo[playerid][tvid]]);
        }
        Race_HideCp(playerid);
        TogglePlayerSpectating(playerid, false);
        SetPlayerVirtualWorld(playerid, 0);
        SCM(playerid, Color_Orange, "[TV] ��ر���TV.");
        PlayerInfo[playerid][tvzt] = false;
        PlayerInfo[playerid][tvid] = playerid;
        return 1;
    }
    new id = strval(tmp);
    if(id == playerid) return SCM(playerid, Color_Orange, "[TV]:�㲻�ܹۿ����Լ�!");
    if(!IsPlayerNPC(id) && !IsPlayerConnected(id)) return SCM(playerid, Color_Orange, "[TV]:����!�Է�δ��¼");
    if(PlayerInfo[id][tvzt]) return SCM(playerid, Color_Orange, "[TV]:�Է������ڹ�ս״̬!");
    for (new a = 0; a <= 21; a++) {
        //TextDrawHideForPlayer(playerid, velo[playerid][a]);//2020.1.12��
        PlayerTextDrawHide(playerid, velo[PlayerInfo[playerid][tvid]][a]);
        // TextDrawHideForPlayer(playerid, velo[playerid][a]); //��������Լ����ٶȱ� �����������Ǿ仰Ҳû���� �������������ȡ��ע��
    }
    if(PlayerInfo[playerid][speedoMeter]) {
        for (new a = 0; a <= 21; a++) {
            PlayerTextDrawShow(playerid, velo[id][a]); //��ʾ��ҹۿ�������ٶȱ�
        }
    }
    if(!PlayerInfo[playerid][tvzt]) SCM(playerid, Color_Orange, "[TV] ��ʾ:'/tv off' �رյ��ӻ�");
    if(pRaceing[PlayerInfo[playerid][tvid]]) {
        DestroyPRaceTextDraw(playerid);
    }
    // PlayerTextDrawHide(playerid, CpTextDraw[PlayerInfo[playerid][tvid]]);
    // PlayerTextDrawHide(playerid, Time[PlayerInfo[playerid][tvid]]);
    // PlayerTextDrawHide(playerid, Top[PlayerInfo[playerid][tvid]]);
    // PlayerTextDrawHide(playerid, p_record[PlayerInfo[playerid][tvid]]);
    if(pRaceing[id]) {
        // PlayerTextDrawShow(playerid, CpTextDraw[id]);
        // PlayerTextDrawShow(playerid, Time[id]);
        // PlayerTextDrawShow(playerid, Top[id]);
        // PlayerTextDrawShow(playerid, p_record[id]);
        CreatePRaceTextDraw(playerid);
        Race_ShowCp(playerid, GameRace[id][rgameid], GameRace[id][rgamecp]);
    }
    TogglePlayerSpectating(playerid, true);
    SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(id));
    SetPlayerInterior(playerid, GetPlayerInterior(id));
    PlayerInfo[playerid][tvzt] = true; //�������TV״̬Ϊ��
    PlayerInfo[playerid][tvid] = id;
    if(IsPlayerInAnyVehicle(id)) PlayerSpectateVehicle(playerid, GetPlayerVehicleID(id));
    else PlayerSpectatePlayer(playerid, id);
    return 1;
}



//����ʱ�ο�����7F�Ĺ���Ա�ű�
CMD:adminhelp(const playerid, const cmdtext[]) {
    if(!PlayerInfo[playerid][AdminLevel]) return 0;
    Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "����Աָ���ȫ", "LV1\nLV2\nLV3\nLV4\nLV5\nLV?", "ȷ��", "����");
    new str[96];
    format(str, sizeof(str), "[����Ա] %s ʹ���� /AdminHelp. ", GetName(playerid));
    SCMToAdmins(Color_AdminMessage, str);
    return 1;
}
CMD:goto(const playerid, const cmdtext[]) {
    if(!(PlayerInfo[playerid][AdminLevel] >= 1)) return 0;
    new str[128], Float:x, Float:y, Float:z, id;
    if(sscanf(cmdtext, "i", id)) return SCM(playerid, Color_Red, "[ϵͳ] �������ID!");
    if(!PlayerInfo[id][Login]) return SCM(playerid, Color_Red, "[ϵͳ] ��������δ��¼.");
    format(str, sizeof(str), "[����Ա] %s ʹ���� /Goto. ", GetName(playerid));
    SCMToAdmins(Color_AdminMessage, str);
    GetPlayerPos(id, x, y, z);
    if(IsPlayerInAnyVehicle(playerid)) SetVehiclePos(GetPlayerVehicleID(playerid), x, y, z + 0.35);
    else SetPlayerPos(playerid, x, y, z + 0.35);
    AdminCommandRecord(playerid, "goto", str);
    return 1;
}
CMD:get(const playerid, const cmdtext[]) {
    if(!(PlayerInfo[playerid][AdminLevel] >= 1)) return 0;
    new id, str[128], Float:x, Float:y, Float:z;
    if(sscanf(cmdtext, "i", id)) return SCM(playerid, Color_Red, "[ϵͳ] �������ID!");
    if(IsPlayerConnected(id) == 0) return SCM(playerid, Color_Red, "[ϵͳ] �������ID!");
    if(PlayerInfo[id][Login] == false) return SCM(playerid, Color_Red, "[ϵͳ] ��������δ��¼.");
    if(pRaceing[id]) return SendClientMessage(playerid, Color_White, "�Է�������������,����Get");
    format(str, sizeof(str), "[����Ա] %s ʹ���� Get. ", GetName(playerid));
    SCMToAdmins(Color_AdminMessage, str);
    GetPlayerPos(playerid, x, y, z);
    if(IsPlayerInAnyVehicle(id)) SetVehiclePos(GetPlayerVehicleID(id), x, y, z + 0.35);
    else SetPlayerPos(id, x, y, z + 0.35);
    return 1;
}
CMD:givecash(const playerid, const cmdtext[]) {
    if(!(PlayerInfo[playerid][AdminLevel] >= 4)) return 0;
    new id, cash, reason[64], str[96];
    if(sscanf(cmdtext, "iis[64]", id, cash, reason)) return SCM(playerid, Color_Red, "[ϵͳ] �÷�:/giveplayercash [���ID] [��Ǯ] [ԭ��]");
    if(PlayerInfo[id][Login] == false) return SCM(playerid, Color_Red, "[ϵͳ] ��������δ��¼.");
    if(cash < 1 || cash > 100000098) return SCM(playerid, Color_Red, "[ϵͳ] ��Ǯ������������.");
    format(str, sizeof(str), "[ϵͳ]%s(LV%d) ������� %s (%d��Ǯ),ԭ��:%s", GetName(playerid), PlayerInfo[playerid][AdminLevel], GetName(id), cash, reason);
    SCMALL(Color_Red, str);
    format(str, sizeof(str), "[����Ա] %s ʹ�������� GivePlayerCash. ", GetName(playerid));
    SCMToAdmins(Color_AdminMessage, str);
    AdminCommandRecord(playerid, "GivePlayerCash", reason);
    GivePlayerCash(id, cash);
    return 1;
}
CMD:showname(const playerid, const cmdtext[]) {
    if(!(PlayerInfo[playerid][AdminLevel] >= 4)) return 0;
    if(NPCNameShow) {
        for (new i = 0; i < sizeof(textid); i++) {
            DestroyDynamic3DTextLabel(textid[i]);
        }
        NPCNameShow = false;
        SendClientMessage(playerid, Color_Green, "[��ʾ:] NPC������ʾ�ѹر�.");
    } else {
        new str[50];

        //Drifter-LDZ
        format(str, sizeof(str), "Drifter-LDZ\n(ID:%d)", playerid);
        textid[0] = CreateDynamic3DTextLabel(str, Color_Green, 0.0, 0.0, 0.0, 15.0, INVALID_PLAYER_ID, npcCars[0]);

        //Dirfter-SF Two Circles
        format(str, sizeof(str), "Drifter-SF Two Circles\n(ID:%d)", playerid);
        textid[1] = CreateDynamic3DTextLabel(str, Color_Green, 0.0, 0.0, 0.0, 15.0, INVALID_PLAYER_ID, npcCars[1]);
        //Drifter-RunInMadd
        format(str, sizeof(str), "Drifter-RunInMadd\n(ID:%d)", playerid);
        textid[2] = CreateDynamic3DTextLabel(str, Color_Green, 0.0, 0.0, 0.0, 15.0, INVALID_PLAYER_ID, npcCars[2]);
        //Drifter-LSEyeOut
        format(str, sizeof(str), "Drifter-LSEyeOut\n(ID:%d)", playerid);
        textid[3] = CreateDynamic3DTextLabel(str, Color_Green, 0.0, 0.0, 0.0, 15.0, INVALID_PLAYER_ID, npcCars[3]);
        //Drifter-SFTestDrive
        format(str, sizeof(str), "Drifter-SFTestDrive\n(ID:%d)", playerid);
        textid[4] = CreateDynamic3DTextLabel(str, Color_Green, 0.0, 0.0, 0.0, 15.0, INVALID_PLAYER_ID, npcCars[4]);
        //Drifter-OffControl(Rally)
        format(str, sizeof(str), "Drifter-OffControl(Rally)\n(ID:%d)", playerid);
        textid[5] = CreateDynamic3DTextLabel(str, Color_Green, 0.0, 0.0, 0.0, 15.0, INVALID_PLAYER_ID, npcCars[5]);
        //Drifter-FM
        format(str, sizeof(str), "Drifter-FollowMe\n(ID:%d)", playerid);
        textid[6] = CreateDynamic3DTextLabel(str, Color_Green, 0.0, 0.0, 0.0, 15.0, INVALID_PLAYER_ID, npcCars[6]);
        //Drifter-FM2
        format(str, sizeof(str), "Drifter-FollowMe2\n(ID:%d)", playerid);
        textid[7] = CreateDynamic3DTextLabel(str, Color_Green, 0.0, 0.0, 0.0, 15.0, INVALID_PLAYER_ID, npcCars[7]);
        NPCNameShow = true;
        SendClientMessage(playerid, Color_Green, "[��ʾ:] NPC������ʾ�ѿ���.");
    }
    return 1;
}
CMD:kick(const playerid, const cmdtext[]) {
    if(!(PlayerInfo[playerid][AdminLevel] >= 1)) return 0;
    new id, reason[64], str[128];
    if(sscanf(cmdtext, "is[64]", id, reason)) return SCM(playerid, Color_Red, "[ϵͳ] �÷�:/kick [���ID] [ԭ��]");
    if(IsPlayerConnected(id) == 0 || IsPlayerNPC(id)) return SCM(playerid, Color_Red, "[ϵͳ] �������ID!��ҪT������NPC");
    // SetTimerEx("KickEx", 200, false, "i", id);
    DelayedKick(id);
    format(str, sizeof(str), "[ϵͳ]%s(LV%d) �� %s �߳��˷�����,ԭ��:%s", GetName(playerid), PlayerInfo[playerid][AdminLevel], GetName(id), reason);
    SCMALL(Color_Red, str);
    format(str, sizeof(str), "[����Ա] %s ʹ�������� Kick. ", GetName(playerid));
    SCMToAdmins(Color_AdminMessage, str);
    AdminCommandRecord(playerid, "Kick", reason);
    return 1;
}
CMD:reset(const playerid, const cmdtext[]) {
    if(!(PlayerInfo[playerid][AdminLevel] >= 5)) return 0;
    new pname[24], pPassWord[16];
    if(sscanf(cmdtext, "s[24]s[64]", pname, pPassWord)) return SCM(playerid, Color_Red, "[ϵͳ] �÷�:/reset �û��� ����");
    if(strlen(pPassWord) > 16 || strlen(pPassWord) < 6) return SCM(playerid, Color_Red, "[ϵͳ] ���볤�ȴ���,����6~16λ");
    if(strcmp(pname, GetName(playerid), true) == 0) return SCM(playerid, Color_Red, "[ϵͳ] �����������Լ����˻�,���������ǰ����ȫ����");
    if(!AccountCheck(pname)) return SCM(playerid, Color_Red, "[ϵͳ] �������õ��û�������");
    OnPlayerResetPassword(playerid, pname, pPassWord);
    new str[96];
    format(str, sizeof(str), "[����Ա] %s ʹ����������%s���˻�����. ", GetName(playerid), pname);
    SCMToAdmins(Color_AdminMessage, str);
    AdminCommandRecord(playerid, "resetPassWord", str);
    return 1;
}
CMD:gmx(const playerid, const cmdtext[]) {
    if(!(PlayerInfo[playerid][AdminLevel] >= 5)) return 0;
    SCMALL(Color_Red, "[ϵͳ] ����ˢ�·����������ڱ������������...");
    SendRconCommand("gmx");
    return 1;
}
CMD:jail(const playerid, const cmdtext[]) {
    if(!(PlayerInfo[playerid][AdminLevel] >= 1)) return 0;
    new id, reason[64];
    if(sscanf(cmdtext, "is[64]", id, reason)) return SCM(playerid, Color_Red, "[ϵͳ] �÷�:/jail [���ID] [ԭ��]");
    if(IsPlayerConnected(id) == 0) return SCM(playerid, Color_Red, "[ϵͳ] �������ID!");
    if(PlayerInfo[id][JailSeconds] > 0) return SCM(playerid, Color_Red, "[ϵͳ] ��������ڼ�����!");
    PlayerInfo[id][JailSeconds] = 60;
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerInterior(playerid, 0);
    new msg[128];
    mysql_format(g_Sql, msg, sizeof(msg), "UPDATE `users` SET `JailSeconds` = %d WHERE `Name` = '%e'", 60, GetName(id));
    mysql_pquery(g_Sql, msg);
    format(msg, sizeof(msg), "[ϵͳ] %s (LV%d)�� %s �ؽ��˼���,�Ժ��Զ��ų�,ԭ��:%s", GetName(playerid), PlayerInfo[playerid][AdminLevel], GetName(id), reason);
    SCMALL(Color_Red, msg);
    SCM(id, Color_Red, "[ϵͳ] Ϊ�˲�Ӱ���������������Ϸ,�����ܵ�����Ա�ͷ�,1���Ӻ󽫷ų�����!");
    format(msg, sizeof(msg), "[����Ա] %s ʹ�������� Jail. ", GetName(playerid));
    SCMToAdmins(Color_AdminMessage, msg);
    AdminCommandRecord(playerid, "Jail", reason);
    return 1;
}
CMD:kgobj(const playerid, const cmdtext[]) {
    // new Float:pX, Float:pY, Float:pZ;
    // GetPlayerPos(playerid, pX, pY, pZ);
    if(PlayerInfo[playerid][displayObject]) {
        PlayerInfo[playerid][displayObject] = 0;
        Streamer_UpdateEx(playerid, 0, 0, -50000);
        // Streamer_UpdateEx(playerid, 0, 0, 3343077376, 4294967295, 4294967295);
        Streamer_ToggleItemUpdate(playerid, 0, 0);
        SendClientMessage(playerid, Color_White, "[ϵͳ]:��ر��˵�ͼģ��,�ٴ����뿪��");
    } else {
        PlayerInfo[playerid][displayObject] = 1;
        // Streamer_UpdateEx(playerid, 0.0, 0.0, -50000.0);
        // Streamer_UpdateEx(playerid, pX, pY, pZ);
        Streamer_UpdateEx(playerid, 0, 0, -50000);
        Streamer_ToggleItemUpdate(playerid, 0, 1);
        SendClientMessage(playerid, Color_White, "[ϵͳ]:�㿪���˵�ͼģ��,�ٴ�����ر�");
    }
    return 1;
}
CMD:giveadmin(const playerid, const cmdtext[]) {
    if(!IsPlayerAdmin(playerid)) return 0;
    new level, name[64];
    if(sscanf(cmdtext, "s[64]i", name, level)) return SCM(playerid, Color_Red, "[ϵͳ] �÷�:/giveadmin [����ǳ�] [GM�ȼ�]");
    if(AccountCheck(name)) {
        new msg[128];
        if(level < 1 || level > 2025) return SCM(playerid, Color_Red, "[ϵͳ] GM�ȼ���Ч��ΧΪ1-5��");
        mysql_format(g_Sql, msg, sizeof(msg), "UPDATE `users` SET AdminLevel = %d WHERE `Name` = '%e'", level, name);
        mysql_pquery(g_Sql, msg);
        format(msg, sizeof(msg), "[ϵͳ] %s ���� %s Ϊ GM{FFFF00}%d��", GetName(playerid), name, level);
        for (new i = GetPlayerPoolSize(); i >= 0; i--) { //2020.2.9�޸� //2020.2.27�ٴθ���
            if(strcmp(GetName(i), name, true) == 0) {
                PlayerInfo[i][AdminLevel] = level;
                break;
            }
        }
        // PlayerInfo[playerid][AdminLevel] = level; //2020.2.9�������� ��Ӧ����playerid ������Ҳû��id��
        SCMALL(Color_Red, msg);
        AdminCommandRecord(playerid, "GiveAdmin", "��GM");
        return 1;
    } else {
        SCM(playerid, Color_Red, "[ϵͳ] ������ǳƲ�����!");
    }
    return 1;
}
CMD:unadmin(const playerid, const cmdtext[]) {
    if(!IsPlayerAdmin(playerid)) return 0;
    new tmp[128];
    if(sscanf(cmdtext, "s[128]", tmp)) return SCM(playerid, Color_Red, "[ϵͳ] �÷�:/unadmin [����ǳ�]");
    if(AccountCheck(tmp)) {
        new msg[256];
        mysql_format(g_Sql, msg, sizeof(msg), "UPDATE `users` SET `AdminLevel` = 0 where `Name` = '%e'", tmp);
        mysql_pquery(g_Sql, msg);
        for (new i = GetPlayerPoolSize(); i >= 0; i--) {
            if(strcmp(GetName(i), tmp, true) == 0) PlayerInfo[i][AdminLevel] = 0;
            break;
        }
        format(msg, sizeof(msg), "[ϵͳ] %s ȡ���� %s��GM", GetName(playerid), tmp);
        SCMALL(Color_Red, msg);
        AdminCommandRecord(playerid, "UnGiveAdmin", "ȡ��GM");
    } else {
        SCM(playerid, Color_Red, "[ϵͳ] ������ǳƲ�����!");
    }
    return 1;
}
CMD:ban(const playerid, const cmdtext[]) {
    if(!(PlayerInfo[playerid][AdminLevel] >= 2)) return 0;
    Dialog_Show(playerid, Ban_Choose, DIALOG_STYLE_INPUT, "Ban", "{FFFFFF}������:{33CCCC}Ҫ��ɱ�����{FF0000}����", "ȷ��", "ȡ��");
    return 1;
}
CMD:unban(const playerid, const cmdtext[]) {
    if(!(PlayerInfo[playerid][AdminLevel] >= 2)) return 0;
    Dialog_Show(playerid, Ban_Unban, DIALOG_STYLE_INPUT, "Ban", "{FFFFFF}������:{33CCCC}Ҫ�������{FF0000}����", "ȷ��", "ȡ��");
    return 1;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger) {
    if(GetPlayerCreditpoints(playerid) <= 90) {
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        SetPlayerPos(playerid, x + 3, y + 2, z + 1.2);
        SendClientMessage(playerid, Color_Yellow, "[ϵͳ]������Ϸ�����ֹ���,�뽡����Ϸ,�����Զ�����");
        return 1;
    }
    new Float:x, Float:y, Float:z;
    //2020.2.21�����·����� CarTroll ���ָĻ����ˡ��� ˢ��̫����
    GetVehiclePos(vehicleid, x, y, z);
    if(!IsPlayerInRangeOfPoint(playerid, 25.0, x, y, z) && !IsPlayerNPC(playerid)) {
        for (new i = 0; i < sizeof npcCars; i++) {
            if(vehicleid == npcCars[i]) return 1;
        }
        FuckAnitCheat(playerid, "�糵/����/�������", 3);
        // 2020.2.28�������ԭ�� �������NPC����һ˲��NPC���˵ط��ͻᱻ��� �͵��ж��ϵĳ��ǲ���NPC�� ������� �Ǿͷ�
        return 1;
    }

    // if(GetPlayerVehicleID(vehicleid) != PlayerInfo[playerid][BuyID]){
    //     for (new i = GetPlayerPoolSize(); i >= 0; i--) 
    //     {
    //         if(IsPlayerConnected(i) && PlayerInfo[i][CarLock] && PlayerInfo[i][BuyID] == vehicleid && i!=playerid){
    //             SendClientMessage(playerid, Color_White, "[��ͨ����] �ؾ�������");
    //             RemovePlayerFromVehicle(playerid);
    //         }
    //         return 1;
    //     }
    //     new msg[64];
    //     format(msg, sizeof(msg), "[��ͨ����]�����ؾ߲������Ŷ����Ҫӵ����һ��������/c %d",vehicleid);
    //     return SendClientMessage(playerid, Color_White, msg);
    // }
    return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid) { //������뿪�ؾ������������ϵͳʱ�����ж�Ϊ��Ҫ����
    if(pRaceing[playerid]) {
        new Float:pPos[3];
        GetPlayerPos(playerid, pPos[0], pPos[1], pPos[2]);
        SetPlayerPos(playerid, pPos[0] + 2, pPos[1] + 1.5, pPos[2] + 0.5);
        PlayerInfo[playerid][lastXMoved] = pPos[0] + 2;
        PlayerInfo[playerid][lastYMoved] = pPos[1] + 1.5;
        PlayerInfo[playerid][lastZMoved] = pPos[2] + 0.5;
        return Dialog_Show(playerid, RACE_RESPAWNDIALOG, DIALOG_STYLE_MSGBOX, "����ϵͳ By YuCarl77", "��⵽�����������³���,�Ƿ���Ҫ����?\n���뵱�˿�����.", "��", "��");
    }
    if(p_PPC[playerid]) {
        return SetPlayerHealth(playerid, -1.0);
    }

    //��ʱ�ŵ�statechange�Ǳ��� ���ܸ��ɿ�һ��ɸ���  ������ôд
    // if(pRaceing[playerid])
    // {
    //     ReSpawningText[playerid] = TextDrawCreate(307.333374, 127.362937, "������...");
    //     TextDrawLetterSize(ReSpawningText[playerid], 0.375666, 1.512889);
    //     TextDrawTextSize(ReSpawningText[playerid], 18.000000, 187.000000);
    //     TextDrawAlignment(ReSpawningText[playerid], 2);
    //     TextDrawColor(ReSpawningText[playerid], 6736383);
    //     TextDrawUseBox(ReSpawningText[playerid], 1);
    //     TextDrawBoxColor(ReSpawningText[playerid], 73);
    //     TextDrawSetShadow(ReSpawningText[playerid], 3);
    //     TextDrawSetOutline(ReSpawningText[playerid], 1);
    //     TextDrawBackgroundColor(ReSpawningText[playerid], 255);
    //     TextDrawFont(ReSpawningText[playerid], 0);
    //     TextDrawSetProportional(ReSpawningText[playerid], 1);
    //     TextDrawSetShadow(ReSpawningText[playerid], 3);
    //     TextDrawShowForPlayer(playerid, ReSpawningText[playerid]);
    //     new raid = RaceHouse[GameRace[playerid][rgameid]][rraceid];
    //     new trcp[racecptype];
    //     if(GameRace[playerid][rgamecp] - 1 <= 0) Race_GetCp(raid, 1, trcp);//����ǵ�һ����Ļ�����������һ���㣬��Ȼ���Ǹ���
    //     else Race_GetCp(raid, GameRace[playerid][rgamecp] - 1, trcp);
    //     ReSpawnRaceVehicle(playerid);//2020.1.12�ģ���������Ч��
    // }
    return 1;
}
//
public OnPlayerStateChange(playerid, newstate, oldstate) {
    if(newstate == PLAYER_STATE_DRIVER) {
        new vehicleid = GetPlayerVehicleID(playerid);
        if(CarInfo[vehicleid][ID] != 0) {
            if(CarInfo[vehicleid][UsersID] == 0) {
                new str[128];
                format(str, sizeof(str), "[����] �ð������ڳ�����,�۸�:%d �Ƿ���?", CarInfo[vehicleid][Value]);
                Dialog_Show(playerid, AC_BUY, DIALOG_STYLE_MSGBOX, "[���򰮳�]", str, "��", "��");
                // SCM(playerid,Color_ACColor,str);
            } else {
                new AC_Car;
                for (AC_Car = 0; AC_Car < MAX_VEHICLE; AC_Car++) {
                    if(CarInfo[AC_Car][GotoID] == vehicleid) {
                        break;
                    }
                }
                if(CarInfo[AC_Car][UsersID] == PlayerInfo[playerid][ID]) {
                    if(PlayerInfo[playerid][tdEnabled]) tdSpeedo_Toggle(playerid, 1);
                    SCM(playerid, Color_ACColor, "[����] ��ӭ�ص���İ���.");
                    AddVehicleComponent(vehicleid, 1010);
                } else {
                    if(CarInfo[AC_Car][Lock]) {
                        new Float:x, Float:y, Float:z;
                        GetPlayerPos(playerid, x, y, z);
                        SetPlayerPos(playerid, x, y, z);
                        SetVehiclePos(vehicleid, CarInfo[AC_Car][CarX], CarInfo[AC_Car][CarY], CarInfo[AC_Car][CarZ]);
                        SendClientMessage(playerid, Color_ACColor, "[����]�������Ѿ�������");
                        return 1;
                    }
                    new str[128];
                    format(str, sizeof(str), "[����] �ؾ�ID:%s(%d) �۸�:%d ��� ӵ����:UID %d", VehicleNames[GetVehicleModel(AC_Car) - 400], \
                        GetVehicleModel(vehicleid), CarInfo[AC_Car][Value], CarInfo[AC_Car][UsersID]);
                    SCM(playerid, Color_ACColor, str);
                    if(CarInfo[AC_Car][SellValue] != 0) {
                        format(str, sizeof(str), "[����] ���ؾ����ڳ�����,�۸�:%d �Ƿ���", CarInfo[AC_Car][SellValue]);
                        // SCM(playerid,Color_ACColor,str);
                        Dialog_Show(playerid, AC_BUY, DIALOG_STYLE_MSGBOX, "[���򰮳�]", str, "��", "��");
                    }
                }
            }
            return 1;
        }
        if(vehicleid == PlayerInfo[playerid][BuyID]) {
            AddVehicleComponent(vehicleid, 1010);
            if(PlayerInfo[playerid][tdEnabled]) tdSpeedo_Toggle(playerid, 1);
            for (new i = GetPlayerPoolSize(); i >= 0; i--) { //��ӦTV�ͱ����Լ� �ϳ���ʾ �³�����
                if(IsPlayerConnected(i)) {
                    if(PlayerInfo[i][tvid] == playerid) {
                        // for (new a = 0; a < 22; a++) {
                        //     TextDrawShowForPlayer(i, velo[playerid][a]);
                        // }
                        if(i != playerid) PlayerSpectateVehicle(i, GetPlayerVehicleID(playerid)); //������ɾ��
                    }
                    // �������������ı���
                    // if(i != playerid) PlayerSpectateVehicle(i, GetPlayerVehicleID(playerid));
                }
            }
            if(PlayerInfo[playerid][CarLock]) return SendClientMessage(playerid, Color_Green, "[�ؾ�]�ϰ延ӭ, ��ĳ�������");
        } else {
            for (new i = GetPlayerPoolSize(); i >= 0; i--) {
                if(IsPlayerConnected(i) && vehicleid == PlayerInfo[i][BuyID] && PlayerInfo[i][CarLock]) {
                    new Float:X, Float:Y, Float:Z, tmp[64];
                    GetPlayerPos(playerid, X, Y, Z);
                    SetPlayerPos(playerid, X, Y, Z + 0.5);
                    format(tmp, sizeof(tmp), "[�ؾ�]��������%s�Ĳ����Ѿ�������Ŷ..", GetName(i));
                    SendClientMessage(playerid, Color_Green, tmp);
                    // return 1;
                }
            }
        }
    }
    if(newstate == PLAYER_STATE_ONFOOT) {
        if(PlayerInfo[playerid][tdEnabled]) tdSpeedo_Toggle(playerid, 0);
        // if(pRaceing[playerid]) SetPlayerHealth(playerid, -1.0);//������������³�����ɱ��λ
        for (new i = GetPlayerPoolSize(); i >= 0; i--) { //��ӦTV�ͱ����Լ� �ϳ���ʾ �³�����
            if(IsPlayerConnected(i)) {
                if(PlayerInfo[i][tvid] == playerid) {
                    // for (new a = 0; a < 22; a++) 
                    // {
                    //     TextDrawHideForPlayer(i, velo[playerid][a]);
                    // }
                    if(i != playerid) PlayerSpectatePlayer(i, playerid);
                }
            }
        }
    }
    return 1;
}

public OnPlayerEnterCheckpoint(playerid) {
    return 1;
}

public OnPlayerLeaveCheckpoint(playerid) {

    return 1;
}



// public OnRconCommand(cmd[]) {
//     return 1;
// }

public OnPlayerObjectMoved(playerid, objectid) {
    return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid) {
    return 1;
}


public OnVehicleRespray(playerid, vehicleid, color1, color2) {
    return 1;
}

public OnPlayerSelectedMenuRow(playerid, row) {
    return 1;
}

public OnPlayerExitedMenu(playerid) {
    return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid) {
    return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    //     /*if(newkeys & KEY_FIRE)//�л�TV����
    //     {
    //     	if(PlayerInfo[playerid][tvzt]==0)
    //     	{
    // 			new rd = random(MAX_PLAYERS);
    // 			while(rd == lastrandom || rd == playerid || !IsPlayerConnected(rd))
    // 			{
    // 	    		rd = random(MAX_PLAYERS);
    //  			}
    //  			lastrandom = rd;
    // 			TogglePlayerSpectating(playerid, 1);
    //     		SetPlayerVirtualWorld(playerid,GetPlayerVirtualWorld(rd));
    //     		SetPlayerInterior(playerid,GetPlayerInterior(rd));
    // 			if(IsPlayerInAnyVehicle(rd)) PlayerSpectateVehicle(playerid, GetPlayerVehicleID(rd));
    // 			else PlayerSpectatePlayer(playerid, rd);
    // 		}
    //     }*/
    // if(PRESSED(KEY_FIRE) || PRESSED(KEY_ACTION))  �ɰ�д�� ���뱨029��
    // if(pRaceing[playerid] &&  HOLDING(KEY_CROUCH)){  // C������ϵͳ����
    // // SetTimerEx("KickEx", 100, false, "i", playerid);
    //     RaceReSpawnTextDraw(playerid);
    //     SetTimerEx("ReSpawnRaceVehicle", 1000, false, "i", playerid);
    // }

    // if((newkeys & KEY_FIRE) && !(oldkeys & KEY_FIRE) || (newkeys & KEY_ACTION) && !(oldkeys & KEY_ACTION)) { 2020.2.9ȡ��
    // if((newkeys & KEY_ACTION) && !(oldkeys & KEY_ACTION)) {
    //     if(IsPlayerInAnyVehicle(playerid)) AddVehicleComponent(GetPlayerVehicleID(playerid), 1010);
    // }
    if(Attire_Presskey(playerid, newkeys) == 1) return 1;
    if(PlayerEdit[playerid][2] != 0) {
        new Keys, ud, lr;
        GetPlayerKeys(playerid, Keys, ud, lr);
        if(Keys == KEY_LOOK_BEHIND) {
            new str[128];
            format(str, sizeof(str), "����\nǰ��\n����\nǰ��\n�෭\n��ת\n����\n{FF0000}ɾ��\n״̬:%s", TagObjectsState[VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][TagObjects]]);
            Dialog_Show(playerid, CDIALOG_CarZB, DIALOG_STYLE_LIST, "����༭", str, "ѡ��", "�˳�");
        }
        if(Keys == KEY_FIRE) {
            Dialog_Show(playerid, CDIALOG_CarZBEditSave, DIALOG_STYLE_LIST, "����༭", "?�\n?�\n?�\n\
                ����������\n����������\n------------------------------\n{FF0000}�������ø�װ��\n------------------------------\n{00FF00}���ұ����װ��\n------------------------------", "����", "����");
        }
        if(Keys == KEY_ANALOG_LEFT) {
            DestroyDynamicObject(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaObjectID]);
            VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaObjectID] = CreateDynamicObject(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][ModelID], 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
            switch (PlayerEdit[playerid][2]) {
                case 1:
                    VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaX] -= EditSpeed[playerid];
                case 2:
                    VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaY] -= EditSpeed[playerid];
                case 3:
                    VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaZ] -= EditSpeed[playerid];
                case 4:
                    VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRX] -= EditSpeed[playerid];
                case 5:
                    VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRY] -= EditSpeed[playerid];
                case 6:
                    VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRZ] -= EditSpeed[playerid];
            }
            AttachDynamicObjectToVehicle(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaObjectID], PlayerEdit[playerid][0],
                VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaX],
                VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaY],
                VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaZ],
                VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRX],
                VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRY],
                VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRZ]);
            Streamer_UpdateAll();
        }
        if(Keys == KEY_ANALOG_RIGHT) {

            DestroyDynamicObject(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaObjectID]);
            VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaObjectID] = CreateDynamicObject(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][ModelID], 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
            switch (PlayerEdit[playerid][2]) {
                case 1:
                    VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaX] += EditSpeed[playerid];
                case 2:
                    VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaY] += EditSpeed[playerid];
                case 3:
                    VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaZ] += EditSpeed[playerid];
                case 4:
                    VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRX] += EditSpeed[playerid];
                case 5:
                    VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRY] += EditSpeed[playerid];
                case 6:
                    VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRZ] += EditSpeed[playerid];
            }
            AttachDynamicObjectToVehicle(VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaObjectID], PlayerEdit[playerid][0],
                VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaX],
                VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaY],
                VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaZ],
                VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRX],
                VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRY],
                VehAttachedObject[PlayerEdit[playerid][0]][PlayerEdit[playerid][1]][VaRZ]);
            Streamer_UpdateAll();
        }
    }
    if(newkeys & KEY_FIRE) {
        CreatingCamera(playerid); //��������� ��ͷ
        if(IsPlayerDeathMatch(playerid)) {
            if(ForbiddenWeap(playerid, 1)) {
                //������뿪DM����ʾʹ�ý�ֹʹ�õ�����
                ResetPlayerWeapons(playerid);
                DeathMatch_Leave(playerid);
                Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "[ϵͳ]", "�㲻��ʹ�ò�����ʹ�õ�����", "ȷ��", "");
                return 1;
            }
        } else {
            //�������ֹʹ����Щ���� Ȼ��ֱ��û����ҵ���������
            if(GetPlayerVirtualWorld(playerid) == 0) {
                if(GetPlayerCreditpoints(playerid) <= 95) {
                    ResetPlayerWeapons(playerid);
                    SendClientMessage(playerid, Color_Yellow, "[ϵͳ]���������ֹ���,��ֹ�ڴ�����ʹ������");
                    return 1;
                }
                if(ForbiddenWeap(playerid, 0)) {
                    ResetPlayerWeapons(playerid);
                    SendClientMessage(playerid, Color_Yellow, "[ϵͳ]�������н�ֹʹ����Щ����");
                    return 1;
                }
                return 1;
            }
        }
        if(PlayerInfo[playerid][tvzt]) {
            if(pRaceing[PlayerInfo[playerid][tvid]]) {
                new tmp = GameRace[PlayerInfo[playerid][tvid]][rgameid];
                for (new i = GetPlayerPoolSize(); i >= 0; i--) {
                    if(IsPlayerConnected(i) && i != PlayerInfo[playerid][tvid] && GameRace[i][rgameid] == tmp) { //�����ĳ����ҵķ���ID���������� ���л��ۿ�
                        new tmped[16];
                        format(tmped, 16, "/tv %i", i);
                        AntiCommand[i] = 0;
                        CallRemoteFunction("OnPlayerCommandText", "is", playerid, tmped);
                        // OnPlayerCommandText(playerid, tmped);
                        break;
                    }
                }
            } else {
                new rd = random(MAX_PLAYERS);
                while (rd == PlayerInfo[playerid][tvid] || rd == playerid || !IsPlayerConnected(rd)) {
                    if(GetPlayerPoolSize() <= 1) {
                        AntiCommand[playerid] = 0;
                        CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/tv off");
                        // OnPlayerCommandText(playerid, "/tv off");
                        break;
                    }
                    rd = random(MAX_PLAYERS);
                }
                new tmped[16];
                format(tmped, 16, "/tv %i", rd);
                AntiCommand[playerid] = 0;
                CallRemoteFunction("OnPlayerCommandText", "is", playerid, tmped);
                // OnPlayerCommandText(playerid, tmped);
            }
        }
    }
    if(newkeys == 65536) { //"Y" Key
        if(KeyBoards(playerid) == 1) return 1; //����Y���ù����� ���������ڸ㹫���ƵĻ� �ǾͲ�����ִ���ˣ���Ȼ��ը����
        if(config_Nochangeobj == 0) { //PHouse��
            UseChangeObj(playerid);
            // return 1; //�����������Ҿ߾Ͷ�������
        }
        if(config_Nomoveobj == 0) { //������Phouse��
            UseMoveObj(playerid);
            //��������Ҿ߶�����
            // return 1;
        }
        if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) {
            GOODS_OPRATEID[playerid] = getClosestGOODS(playerid);
            // printf("GET ID = %d", GOODS_OPRATEID[playerid]);
            if(GOODS_STATUS[playerid] == true) return SendClientMessage(playerid, Color_Furniture, "[�Ҿ�]����,�����ڰ�һ����Ʒ!");
            if(GOODS[GOODS_OPRATEID[playerid]][Key]) return Dialog_Show(playerid, GODIOG_PASS, DIALOG_STYLE_INPUT, "��ȫ��֤��ʽ", "����������", "OK", "ȡ��");

            if(GOODS[GOODS_OPRATEID[playerid]][issale] == true) {
                new string[256];
                format(string, sizeof(string), "{FFFFFF}[��Ʒ�۸�]:{80FF80} %d\n{FFFFFF}[��ƷID]:{80FF80}%d\n{FFFFFF}[��Ʒģ��ID]:{80FF80}%d\n{FFFF80}�_��Ҫ��ô?", GOODS[GOODS_OPRATEID[playerid]][GoodPrize], GOODS_OPRATEID[playerid], GOODS[GOODS_OPRATEID[playerid]][GoodObjid]);
                Dialog_Show(playerid, GODIOG_BUY, DIALOG_STYLE_MSGBOX, "{FFFF80}__Ҫ��ô?__", string, "��", "����");
            } else {
                // new pname[65];
                // GetPlayerName(playerid, pname, 65);
                // if(!strlen((GOODS[GOODS_OPRATEID[playerid]][GoodOwner]))) return Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "{FF0000}ERROR>.<", "{FF0000}ϵͳ�ڲ�����\n�ַ���Ϊ��!\n�п��ܴ���Ʒ���ݱ���\n����ϵ������!\n---->{FFFF80}episodes27@gmail.com", "Close", "");
                if(!strlen((GOODS[GOODS_OPRATEID[playerid]][GoodOwnerName]))) return Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "{FF0000}ERROR>.<", "{FF0000}ϵͳ�ڲ�����\n�ַ���Ϊ��!\n�п��ܴ���Ʒ���ݱ���\n����ϵ������!\n---->{FFFF80}episodes27@gmail.com", "Close", "");
                // if(!strcmp(GOODS[GOODS_OPRATEID[playerid]][GoodOwner], GetName(playerid), true)) {
                if(GOODS[GOODS_OPRATEID[playerid]][GoodOwner] == PlayerInfo[playerid][ID]) {
                    if(GOODS[GOODS_OPRATEID[playerid]][topublic] == true) {
                        new title[285];
                        format(title, 285, "{FFFFFF}��ƷID:{80FF80}%d {FFFFFF}����:{80FF80}%s - {80FFFF}�����˵�", GOODS_OPRATEID[playerid], GOODS[GOODS_OPRATEID[playerid]][GoodOwner]);
                        Dialog_Show(playerid, GODIOG_PL, DIALOG_STYLE_LIST, title, PL_CONCENTS_YES, "ѡ��", "�ر�");
                    } else {
                        new title[285];
                        format(title, 285, "{FFFFFF}��ƷID:{80FF80}%d {FFFFFF}����:{80FF80}%s - {80FFFF}�����˵�", GOODS_OPRATEID[playerid], GOODS[GOODS_OPRATEID[playerid]][GoodOwner]);
                        Dialog_Show(playerid, GODIOG_PL, DIALOG_STYLE_LIST, title, PL_CONCENTS_NO, "ѡ��", "�ر�");
                    }
                } else {
                    Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "{FF0000}ERROR>.<", "{FF0000}�ⲻ�������Ʒ!", "Close", "");
                } //sale
            }
        }
    }
    // if(newkeys == 262144 || newkeys == 2) {
    //     if(config_Nomoveobj == 0) {
    //         UseMoveObj(playerid);
    //         return 1;
    //     }
    // }
    return 1;
}
public OnPlayerClickTextDraw(playerid, Text:clickedid) {

    return 1;
}

public OnRconLoginAttempt(ip[], password[], success) { //rcon��¼ʧ��ֱ�ӷ�
    if(!success) {
        new cmd[32];
        format(cmd, sizeof(cmd), "banip %s", ip);
        SendRconCommand(cmd);
    }
    return 1;
}

public OnPlayerUpdate(playerid) { //���¼Ҿ�
    GetPlayerFPS(playerid);
    if(GOODS_STATUS[playerid] == true && TAKEDOWN_STATUS[playerid] == false) {
        new up_dw, lf_rg, o_keys;
        GetPlayerKeys(playerid, o_keys, up_dw, lf_rg);
        if(o_keys == KEY_FIRE) {
            Dialog_Show(playerid, GODIOG_TAKEDOWN, DIALOG_STYLE_MSGBOX, "{FFFF80}��Ʒ����", "{FFFF80}������Ʒ?", "����", "ȡ��");
            return 1;
        }
    }
    if(g_FlyMode[playerid][flyType] == 1) {
        new keys, ud, lr;
        GetPlayerKeys(playerid, keys, ud, lr);
        new Float:CP[3], Float:FV[3], olddir = g_FlyMode[playerid][flyDirection];
        GetPlayerCameraPos(playerid, CP[0], CP[1], CP[2]);
        GetPlayerCameraFrontVector(playerid, FV[0], FV[1], FV[2]);

        if(g_FlyMode[playerid][flyKeys][0] != ud || g_FlyMode[playerid][flyKeys][1] != lr) {
            if((g_FlyMode[playerid][flyKeys][0] != 0 || g_FlyMode[playerid][flyKeys][1] != 0) && ud == 0 && lr == 0) {
                StopDynamicObject(g_FlyMode[playerid][flyObject]);

                g_FlyMode[playerid][flyDirection] = 0;
            } else {
                if(lr < 0) {
                    if(ud < 0) {
                        g_FlyMode[playerid][flyDirection] = MOVE_FORWARD_LEFT;
                    } else if(ud > 0) {
                        g_FlyMode[playerid][flyDirection] = MOVE_BACK_LEFT;
                    } else {
                        g_FlyMode[playerid][flyDirection] = MOVE_LEFT;
                    }

                    MovePlayerCamera(playerid, CP, FV);
                } else if(lr > 0) {
                    if(ud < 0) {
                        g_FlyMode[playerid][flyDirection] = MOVE_FORWARD_RIGHT;
                    } else if(ud > 0) {
                        g_FlyMode[playerid][flyDirection] = MOVE_BACK_RIGHT;
                    } else {
                        g_FlyMode[playerid][flyDirection] = MOVE_RIGHT;
                    }

                    MovePlayerCamera(playerid, CP, FV);
                } else if(ud < 0) {
                    g_FlyMode[playerid][flyDirection] = MOVE_FORWARD;

                    MovePlayerCamera(playerid, CP, FV);
                } else if(ud > 0) {
                    g_FlyMode[playerid][flyDirection] = MOVE_BACK;

                    MovePlayerCamera(playerid, CP, FV);
                } else {
                    g_FlyMode[playerid][flyDirection] = -1;
                }
            }

            g_FlyMode[playerid][flyKeys][0] = ud;
            g_FlyMode[playerid][flyKeys][1] = lr;
        } else if(g_FlyMode[playerid][flyDirection] && (GetTickCount() - g_FlyMode[playerid][flyTick] > 100)) {
            if((g_FlyMode[playerid][flyKeys][0] != 0 || g_FlyMode[playerid][flyKeys][1] != 0) && ud == 0 && lr == 0) {
                StopDynamicObject(g_FlyMode[playerid][flyObject]);

                g_FlyMode[playerid][flyDirection] = 0;
            } else {
                MovePlayerCamera(playerid, CP, FV);
            }
        }

        if(funcidx("OnPlayerCameraUpdate") != -1) {
            new
            Float:NP[3];
            GetPlayerCameraPos(playerid, NP[0], NP[1], NP[2]);

            CallLocalFunction("OnPlayerCameraUpdate", "ifffifffi", playerid, CP[0], CP[1], CP[2], olddir, NP[0], NP[1], NP[2], g_FlyMode[playerid][flyDirection]);
        }
    }
    return 1;
}

public OnPlayerStreamIn(playerid, forplayerid) {
    return 1;
}

public OnPlayerStreamOut(playerid, forplayerid) {
    return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid) {
    return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid) {
    return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source) {
    new id = clickedplayerid;
    if(!PlayerInfo[id][Login]) return SendClientMessage(playerid, Color_White, "�Է�����û�е�¼..");
    new msgs[32], msg[512];
    if(PlayerInfo[id][AdminLevel]) format(msgs, sizeof(msgs), "[����ԱLV%d]%s����Ϣ", PlayerInfo[id][AdminLevel], GetName(id));
    else format(msgs, sizeof(msgs), "%s����Ϣ", GetName(id));
    new year, month, day, hour, minute, second;
    new nextlevel = PlayerInfo[id][plevel] * 144;
    new racing_cut = Race_GetPlayerRecordCut(id);

    TimestampToDate(PlayerInfo[id][regdate], year, month, day, hour, minute, second, 8);
    format(msg, sizeof(msg), "UID %d", PlayerInfo[id][ID]);
    format(msg, sizeof(msg), "%s\n�ȼ� %d(%s)\n���� %d/%d", msg, PlayerInfo[id][plevel], GetPlayerLevelName(id), PlayerInfo[id][exp], nextlevel);
    format(msg, sizeof(msg), "%s\nƤ�� %d", msg, PlayerInfo[id][Skin]);
    format(msg, sizeof(msg), "%s\n��� %d", msg, PlayerInfo[id][Cash]);
    if(!strcmp(PlayerInfo[id][Designation], "null", false)) format(msg, sizeof(msg), "%s\n�ƺ� %s", msg, PlayerInfo[id][Designation]);
    if(!strcmp(PlayerInfo[id][Tail], "null", false)) format(msg, sizeof(msg), "%s\n������ %s", msg, PlayerInfo[id][Tail]);
    format(msg, sizeof(msg), "%s\n����ʱ�� %d(��)", msg, PlayerInfo[id][Score]);
    // new level = GetPlayerLevel(id);
    // if(PlayerInfo[playerid][plevel]>201) format(msg, sizeof(msg), "%s\n�ȼ�:201\t%s\t%d/--\n��������:%d\n�������", msg, GetPlayerLevelName(playerid), PlayerInfo[id][exp], racing_cut);
    format(msg, sizeof(msg), "%s\n\n�������� %d\n�������", msg, racing_cut);
    if(strcmp(PlayerInfo[id][Team], "null", false)) {
        format(msg, sizeof(msg), "%s\n�Ŷ� %s", msg, PlayerInfo[id][Team]);
    } else {
        if(playerid != id) format(msg, sizeof(msg), "%s\n�Ŷ� �������", msg);
    }
    format(msg, sizeof(msg), "%s\nע������ %d-%d-%d %02d:%02d:%02d", msg, year, month, day, hour, minute, second);

    if(GetPlayerCreditpoints(id) <= 80) format(msg, sizeof(msg), "%s\n������ {FFFF00}%d", msg, GetPlayerCreditpoints(id));
    else if(GetPlayerCreditpoints(id) <= 50) format(msg, sizeof(msg), "%s\n������ {FF0000}%d", msg, GetPlayerCreditpoints(id));
    else format(msg, sizeof(msg), "%s\n������ {00FF00}%d", msg, GetPlayerCreditpoints(id));
    format(msg, sizeof(msg), "%s\n�����͵�TA���", msg);
    SelectRecentlyClicked[playerid] = id;
    // printf("%s", PlayerInfo[id][regdate]);
    Dialog_Show(playerid, ClickPlayer, DIALOG_STYLE_LIST, msgs, msg, "ȷ��", "�ر�");
    //2020.2.11�޸�ΪLIST��ʽ
    return 1;
}


// forward PlaySoundForAll(soundid);
// public PlaySoundForAll(soundid) {
//     for (new i = GetPlayerPoolSize(); i >= 0; i--) {
//         new Float:pX, Float:pY, Float:pZ;
//         GetPlayerPos(i, pX, pY, pZ);
//         PlayerPlaySound(i, soundid, pX, pY, pZ);
//     }
//     return 1;
// }

function CountDown(playerid) {
    new Float:X, Float:Y, Float:Z;
    GetPlayerPos(playerid, X, Y, Z);
    Count[playerid]--;
    if(Count[playerid] == 0) {
        for (new i = GetPlayerPoolSize(); i >= 0; i--) {
            if(IsPlayerConnected(i)) {
                if(IsPlayerInRangeOfPoint(i, 20, X, Y, Z) && GetPlayerVirtualWorld(i) == GetPlayerVirtualWorld(playerid)) {
                    GameTextForPlayer(i, "~g~GO~r~!~n~~g~GO~r~!~n~~g~GO~r~!", 3000, 3);
                    PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
                }
            }
        }
        KillTimer(Timer[playerid]);
        return 1;
    }
    new str[16];
    format(str, sizeof(str), "~w~%d", Count[playerid]);
    for (new i = GetPlayerPoolSize(); i >= 0; i--) {
        if(IsPlayerConnected(i)) {
            if(IsPlayerInRangeOfPoint(i, 20, X, Y, Z) && GetPlayerVirtualWorld(i) == GetPlayerVirtualWorld(playerid)) {
                GameTextForPlayer(i, str, 3000, 3);
                PlayerPlaySound(i, 1056, 0.0, 0.0, 0.0);
            }
        }
    }
    return 1;
}
// forward countdown();
// public countdown() {
//     CountDown--;
//     if(CountDown == 0) {
//         GameTextForAll("~g~GO~r~!~n~~g~GO~r~!~n~~g~GO~r~!", 1000, 3); //����ʱ������ ��Ļ�г��ֵ����塣
//         CountDown = -1;
//         for (new i = GetPlayerPoolSize(); i >= 0; i--) {
//             PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
//         }
//         return 0;
//     } else {
//         new text[7];
//         format(text, sizeof(text), "~w~%d", CountDown);
//         for (new i = GetPlayerPoolSize(); i >= 0; i--) {
//             PlayerPlaySound(i, 1056, 0.0, 0.0, 0.0);
//         }
//         GameTextForAll(text, 1000, 3);
//     }

//     SetTimer_("countdown", 1000, 0);
//     return 0;
// }
function SpawnVehicle(playerid, car) { //ˢ�� ����������ʱ��
    new str[128];
    new Float:pos[3], Float:Angle;
    GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
    //��ȡ������ڵ�����
    if(IsPlayerInAnyVehicle(playerid)) GetVehicleZAngle(GetPlayerVehicleID(playerid), Angle); //�������ڳ����ȡ����ǰ�������ڵĽǶ�
    else GetPlayerFacingAngle(playerid, Angle); //������ڳ���ͻ�ȡ��ҳ���ĽǶ�

    // �ж�����Ƿ��Ѿ��й�һ�����ˣ��й��Ļ��Ͱ���ҷŵ�����Ȼ��ɾ��ԭ���ĳ�
    if(PlayerInfo[playerid][CreateCar] == 1) SetPlayerPos(playerid, pos[0], pos[1], pos[2]);
    DestroyVehicle(PlayerInfo[playerid][BuyID]);
    PlayerInfo[playerid][CreateCar] = 0;

    PlayerInfo[playerid][BuyID] = CreateVehicle(car, pos[0], pos[1], pos[2], 0.0, random(128), random(128), 60); //60���ӳ٣�������˵Ļ��͸Ĵ�һ��
    SetVehicleVirtualWorld(PlayerInfo[playerid][BuyID], GetPlayerVirtualWorld(playerid));
    //�����ؾߵ�����Ϊ��ҵ�����
    PutPlayerInVehicle(playerid, PlayerInfo[playerid][BuyID], 0);
    //����ҷŵ�������
    //PlayerInfo[playerid][pVehicleEnteColor_Red] = GetPlayerVehicleID(playerid);
    PlayerInfo[playerid][CreateCar] = 1;
    PlayerInfo[playerid][CarLock] = 0;

    SetVehicleZAngle(PlayerInfo[playerid][BuyID], Angle); //���ó����ĽǶ�Ϊ֮ǰ������ڵĳ����Ƕ�
    LinkVehicleToInterior(PlayerInfo[playerid][BuyID], GetPlayerInterior(playerid));
    AddVehicleComponent(PlayerInfo[playerid][BuyID], 1010); //����һ������
    if(!pRaceing[playerid]) { //�����Ҳ��������л����Ļ�����ʾ��� ��Ϊ��������cveh
        format(str, sizeof(str), "[��ͨ����]ˢ���ɹ�������/cc �ɻ���ɫ������ģ��(%s[%d])", VehicleNames[car - 400], car);
        SCM(playerid, Color_White, str);
    }
    for (new i = GetPlayerPoolSize(); i >= 0; i--) { //��ӦTV�ͱ����Լ� �ϳ���ʾ �³�����
        if(IsPlayerConnected(i)) {
            if(PlayerInfo[i][tvid] == playerid && PlayerInfo[i][speedoMeter]) {
                for (new a = 0; a <= 21; a++) {
                    PlayerTextDrawShow(i, velo[playerid][a]);
                }
                if(i != playerid) PlayerSpectateVehicle(i, GetPlayerVehicleID(playerid));
            }
        }
    }
    return 1;
}

function MinuteTimer() { //���Ӽ�ʱ��
    //ʱ��� ��ҵ�ʱ���ܹ����� ������ʱ����DB���ݿ�
    for (new i = GetPlayerPoolSize(); i >= 0; i--) {
        if(PlayerInfo[i][Login] == true) {
            if(PlayerInfo[i][bantotal] != 0) //��������������Ϊ0
            {
                new rand = random(2); //ȡ�����
                if(rand) GiveCreditpoints(i, -1); // �����������㷨�Զ�ִ��
            }
            PlayerInfo[i][Score] = PlayerInfo[i][Score] + 1;
            SetPlayerScore(i, PlayerInfo[i][Score]);
            PlayerInfo[i][Cash] = PlayerInfo[i][Cash] + random(30);
            // ������
            new rand = random(2);
            if(rand) {
                new Float:randExp;
                randExp = PlayerInfo[i][plevel] * (random(10) + 10) - (PlayerInfo[i][plevel] - 1) * random(5);
                randExp *= exp_multiple;
                // printf("%f",randExp);
                GivePlayerExp(i, randExp, 0);
            }
        }
    }
    Common_Running_QA(); //ִ���ʴ�
    return 1;
}

stock GetName(const playerid) {
    new GPlayerName[MAX_PLAYER_NAME];
    GetPlayerName(playerid, GPlayerName, sizeof(GPlayerName));
    return GPlayerName;
}

// forward UpdatePlayerIni(playerid); //�����������
function OnPlayerLogin(playerid, inputtext[]) { //��ҵ�¼
    if(PlayerInfo[playerid][LoginAttempts] >= 3) {
        Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "��¼", "�㱻����˷�������ԭ��:�����������������{FF0000}����!", "ȷ��", "");
        // SetTimerEx("KickEx", 100, false, "i", playerid);
        DelayedKick(playerid);
        return 1;
    }
    new Salted_Key[65];

    SHA256_PassHash(inputtext, PlayerInfo[playerid][Salt], Salted_Key, 65); //����65
    if(strcmp(Salted_Key, PlayerInfo[playerid][Password], true) == 0) {
        // RegLoginTDrawDestroy(playerid);
        if(!IsPlayerAndroid(playerid)) {
            for (new i = 0; i <= 7; i++) {
                TextDrawHideForPlayer(playerid, Screen[i]);
            }
        }
        for (new i = GetPlayerPoolSize(); i >= 0; i--) {
            ShowPlayerNameTagForPlayer(playerid, i, true);
        }
        cache_set_active(PlayerInfo[playerid][Cache_ID]);
        new Checked = AssignPlayerData(playerid);
        cache_delete(PlayerInfo[playerid][Cache_ID]);
        PlayerInfo[playerid][Cache_ID] = MYSQL_INVALID_CACHE;
        KillTimer(PlayerInfo[playerid][LoginTimer]);
        PlayerInfo[playerid][LoginTimer] = -1;

        if(Checked) {
            PlayerInfo[playerid][Login] = true;
            // printf("[���]%s(%d) �ѵ�¼.", GetName(playerid), playerid);
            new string[128];
            format(string, sizeof(string), "[ϵͳ]:%s (%d) �����˷����� ^^^", GetName(playerid), playerid);
            SCMALL(Color_LightBlue, string);
            SetPlayerColor(playerid, PlayerColors[random(200)]); //�������С��ͼ��ɫ
            // SetSpawnInfo(playerid, NO_TEAM, 0, Player[playerid][X_Pos], Player[playerid][Y_Pos], Player[playerid][Z_Pos], Player[playerid][A_Pos], 0, 0, 0, 0, 0, 0);
            new rand = random(sizeof BirthPointInfo);
            SetSpawnInfo(playerid, NO_TEAM, PlayerInfo[playerid][Skin], BirthPointInfo[rand][0], BirthPointInfo[rand][1], BirthPointInfo[rand][2], BirthPointInfo[rand][3], 0, 0, 0, 0, 0, 0);
            SpawnPlayer(playerid); //����ҳ���
            PlayerPlaySound(playerid, 1186, 0.0, 0.0, 0.0); //ֹͣ��������
            new temp[512];
            format(temp, sizeof(temp), "����������ddos����\n���������������κξ���\n��лsamp�����ǵ����һֱ������\n��������һֱ����ȥ\n");
            Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "Romantic until Death", temp, "info", "");
            CheckPlayerEmailEd(playerid);
        }
        return 1;
    }
    PlayerInfo[playerid][LoginAttempts]++;
    new string[128];
    format(string, sizeof(string), "{FF0000}�������{00FFFF}�㻹��{80FF80}%d��{00FFFF}�����¼!", 4 - PlayerInfo[playerid][LoginAttempts]);
    // ���ݰ�׿
    if(IsPlayerAndroid(playerid)) {
        Dialog_Show(playerid, Dialog_Login, DIALOG_STYLE_INPUT, "��¼", string, "��¼", "�һ�����");
    } else {
        Dialog_Show(playerid, Dialog_Login, DIALOG_STYLE_PASSWORD, "��¼", string, "��¼", "�һ�����");
    }
    // db_free_result(uf);
    return 1;
}

function OnPlayerRegister(playerid, inputtext[]) //���ע��
{
    if(AccountCheck(GetName(playerid))) //2020.3.29�޸�������ص�BUG
    {
        SCM(playerid, Color_Red, "[ϵͳ]ע���ʺ�ʱ��������,���ܸ��ʺ��Ѿ�����,����������");
        // SetTimerEx("KickEx", 100, false, "i", playerid);
        DelayedKick(playerid);
        return 1;
    }
    // if(strfind(inputtext, "123", true) != -1 || strfind(inputtext, "456", true) != -1) {
    //     Dialog_Show(playerid, Dialog_Register, DIALOG_STYLE_PASSWORD, "ע��", "�벻Ҫ�������ƴ���{22DD22}123��{FFFFFF}�򵥵�����!\n�����·������������{22DD22}ע��..\n��������ɢ�м���,�����ע��", "ע��", "�˳�");
    //     return 1;
    // }
    new msg[256], string[160], temp[64 + 1];
    for (new i = 0; i < 11; i++) {
        PlayerInfo[playerid][Salt][i] = random(25) + 97;
    }
    SHA256_PassHash(inputtext, PlayerInfo[playerid][Salt], temp, sizeof temp);
    mysql_format(g_Sql, msg, sizeof(msg), "INSERT INTO  `users` (`Name`,`Password`,`Salt`,`RegDate`) VALUES ('%s','%e','%e','%d')", GetName(playerid), temp, PlayerInfo[playerid][Salt], gettime());
    mysql_pquery(g_Sql, msg, "pRegisterInsert", "s", playerid);
    PlayerInfo[playerid][regdate] = gettime();
    format(PlayerInfo[playerid][Password], 65, temp);
	//new msg[2048],
	//format(msg,sizeof,msg,"INSERT INTO users (Name,Password,Salt,RegDate,) VALUES ('%s',MD5('%s'),'%s','%d')", GetName(playerid), PlayerInfo[playerid][Salt], gettime());
	//mysql_query(mariadb, msg, false);
       // new Cache:result, uid, query[128];
        //format(query, sizeof query, "SELECT uid FROM users WHERE name = '%s'", GetName(playerid));
        //result = mysql_query(mariadb, query);
        //cache_get_value_name_int(0, "uid", uid);
       // cache_delete(result);
    // ������֤�ĳ�ʼ��ע���û�����
    // �����Ż��£�ȫ������uid,�ϲ����ݿ⣬���û����ݿ��dbȫ���ĵ�mysqlR41�汾���󹤳̣�
    // new Query2[256];
    // format(string, sizeof string, "INSERT INTO `players` (`name`,`code`,`email`,`yz`) VALUES('%s',0,0,0)", GetName(playerid));
    // mysql_free_result(mysql_query(string));
    // mysql_pquery(g_Sql, "INSERT INTO `players` (`name`,`code`,`email`,`yz`) VALUES('%s',0,0,0)", GetName(playerid));
    // printf(Query2);


    format(string, sizeof(string), "ע��ɹ�\n�ʺ�:%s\n ����:%s\n{FF0000}���μ��ʺ�����.", GetName(playerid), inputtext);
    if(IsPlayerAndroid(playerid)) {
        Dialog_Show(playerid, Dialog_Login, DIALOG_STYLE_INPUT, "��¼", string, "��¼", "�һ�����");
    } else {
        Dialog_Show(playerid, Dialog_Login, DIALOG_STYLE_PASSWORD, "��¼", string, "��¼", "�һ�����");
    }
    return 1;
}

function pRegisterInsert(playerid) {
    PlayerInfo[playerid][ID] = cache_insert_id();
}
stock OnPlayerReloadRegister(const playerid, const inputtext[], const pKick = 1) //����ע��
{
    // if(strfind(inputtext, "123", true) != -1 || strfind(inputtext, "qwerty", true) != -1 || strfind(inputtext, "456", true) != -1 || strfind(inputtext, "789", true) != -1) {
    //     Dialog_Show(playerid, PlayerPassWordChange, DIALOG_STYLE_INPUT, "{FFFF00}��ȫ����", "{9AFF9A}��ӭ!\n����������������������޸�!\n���μ������˺�����!\n�Ѳ���ɢ�м��������赣������й¶", "ȷ��", "ȡ��");
    //     return 1;
    // }
    new temp[65]; // ɢ�м�������ɢ����
    for (new i = 0; i < 11; i++) {
        PlayerInfo[playerid][Salt][i] = random(25) + 97;
    }
    // PlayerInfo[playerid][Salt][10] = 0;
    SHA256_PassHash(inputtext, PlayerInfo[playerid][Salt], temp, 65); //�涨65�̶�
    new msg[256];
    mysql_format(g_Sql, msg, sizeof(msg), "UPDATE `users` SET `Password` = '%e',`Salt` = '%e' WHERE `Name` = '%e'", temp, PlayerInfo[playerid][Salt], GetName(playerid));
    mysql_pquery(g_Sql, msg);
    format(msg, sizeof(msg), "[ϵͳ]:�ʺ�:%s ����:%s\n{FF0000}���μ��ʺ�����\n������Ҫ���µ�¼", GetName(playerid), inputtext);
    SendClientMessage(playerid, Color_White, msg);
    Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "ϵͳ", msg, "ȷ��", "");
    // SetTimerEx("KickEx", 100, false, "i", playerid);
    if(pKick) DelayedKick(playerid);
    return 1;
}
stock OnPlayerResetPassword(const playerid, const changeidName[], const inputtext[]) { //����Ա����������� ����Ҷ�ʧʱ��ʹ��
    new temp[65], pSalt[12]; // ɢ�м�������ɢ����
    for (new i = 0; i < 11; i++) {
        pSalt[i] = random(25) + 97;
    }
    SHA256_PassHash(inputtext, pSalt, temp, 65); //�涨65�̶�
    new msg[256];
    mysql_format(g_Sql, msg, sizeof(msg), "UPDATE `users` SET `Password` = '%e',`Salt` = '%e' WHERE `Name` = '%e'", temp, pSalt, changeidName);
    mysql_pquery(g_Sql, msg);
    format(msg, sizeof(msg), "[ϵͳ]:���������ʺ�:%s ����:%s\n����ϵ�Է����µ�¼���޸�����", changeidName, inputtext);
    SendClientMessage(playerid, Color_White, msg);
    Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "ϵͳ", msg, "ȷ��", "");
    return 1;
}

stock AssignPlayerData(playerid) {
    cache_get_value_name_int(0, "BanTime", PlayerInfo[playerid][BanTime]);
    cache_get_value_name_int(0, "BanReason", PlayerInfo[playerid][BanReason]);


    // ��ȡ����ֵ

    //db_get_field(uf,2,Race[i][rpassword],32);

    if(PlayerInfo[playerid][BanTime]) { //�ж�����Ƿ񱻷�� ��������ʱ�䵽����<0�Ϳ��Խ���˷������ʾ�����߳�
        if(PlayerInfo[playerid][BanTime] - gettime() > 0) {
            new day[4];
            UnixToDate(day, PlayerInfo[playerid][BanTime] - gettime());
            new msg[256];
            format(msg, sizeof(msg), "ԭ��:ϵͳ��⵽�����ʹ�õ�������������Ӱ������Ϸ��ƽ��,Code violation #%d\n����������Ϸ�Ƿ��в�����ʹ�õ�CLEO��������\n�����ʱ��:%d�� %dʱ %d�� %d��", PlayerInfo[playerid][BanReason], day[0], day[1], day[2], day[3]);
            Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "[ϵͳ] ��������Ͽ�����", msg, "ȷ��", "");
            DelayedKick(playerid);
            return false;
        }
        PlayerInfo[playerid][BanTime] = 0;
        PlayerInfo[playerid][BanReason] = 0;
        new query[128];
        mysql_format(g_Sql, query, sizeof(query), "UPDATE `users` SET `BanTime` = '0',`BanReason` = '-1' WHERE `Name` = '%e'", GetName(playerid));
        mysql_pquery(g_Sql, query);
    }

    // cache_get_value_name_int(0, "Yzwrong", PlayerInfo[playerid][yzwrong]);
    // cache_get_value_name_int(0, "YzBanTime", PlayerInfo[playerid][yzbantime]);
    // if(PlayerInfo[playerid][yzbantime] - gettime() < 21600000) { //������֤ʱ������⣬���һ�쳬����6����֤�ͻ��24Сʱ����֤
    //     PlayerInfo[playerid][yzwrong] = 0;
    //     PlayerInfo[playerid][yzbantime] = 0;
    // }
    cache_get_value_name_int(0, "ID", PlayerInfo[playerid][ID]);
    cache_get_value_name_int(0, "AdminLevel", PlayerInfo[playerid][AdminLevel]);
    cache_get_value_name_int(0, "Skin", PlayerInfo[playerid][Skin]);
    cache_get_value_name_int(0, "Score", PlayerInfo[playerid][Score]);
    cache_get_value_name_int(0, "Cash", PlayerInfo[playerid][Cash]);
    cache_get_value_name_int(0, "JailSeconds", PlayerInfo[playerid][JailSeconds]);
    cache_get_value_name_int(0, "RegDate", PlayerInfo[playerid][regdate]);
    cache_get_value_name(0, "Designation", PlayerInfo[playerid][Designation], 19);
    cache_get_value_name(0, "Tail", PlayerInfo[playerid][Tail], 33);
    cache_get_value_name_int(0, "exp", PlayerInfo[playerid][exp]);
    cache_get_value_name_int(0, "pLevel", PlayerInfo[playerid][plevel]);
    cache_get_value_name_int(0, "tHour", PlayerInfo[playerid][tHour]);
    cache_get_value_name_int(0, "tMinute", PlayerInfo[playerid][tMinute]);
    cache_get_value_name_int(0, "tWeather", PlayerInfo[playerid][tWeather]);
    cache_get_value_name_int(0, "displayObject", PlayerInfo[playerid][displayObject]);
    cache_get_value_name_int(0, "NoCrash", PlayerInfo[playerid][NoCrash]);
    cache_get_value_name_int(0, "speedoMeter", PlayerInfo[playerid][speedoMeter]); //����ٶȱ�
    cache_get_value_name_int(0, "AutoFix", PlayerInfo[playerid][AutoFix]); //�Զ��޳�
    cache_get_value_name_int(0, "AutoFlip", PlayerInfo[playerid][AutoFlip]); //�Զ�����
    cache_get_value_name_int(0, "enableInvincible", PlayerInfo[playerid][enableInvincible]); //����޵�״̬
    cache_get_value_name_int(0, "netStats", PlayerInfo[playerid][netStats]); //��ҿ���/�ر��������
    cache_get_value_name_int(0, "bantotal", PlayerInfo[playerid][bantotal]); //��������������ڼ���������;
    if(GetPlayerCreditpoints(playerid) <= 90) SendClientMessage(playerid, Color_White, "[ϵͳ]�����������е����,�뱣�ֽ�����Ϸ,�Զ�����!");
    // ��ȡ��������б����һЩ����
    SetPlayerScore(playerid, PlayerInfo[playerid][Score]);
    SetPlayerTime(playerid, PlayerInfo[playerid][tHour], PlayerInfo[playerid][tMinute]); //���ʱ��
    SetPlayerWeather(playerid, PlayerInfo[playerid][tWeather]);
    if(!PlayerInfo[playerid][displayObject]) {
        Streamer_UpdateEx(playerid, 0, 0, -50000);
        Streamer_ToggleItemUpdate(playerid, 0, 0);
    }
    if(PlayerInfo[playerid][NoCrash]) DisableRemoteVehicleCollisions(playerid, true); //������γ�����ײ
    InitializationVelo(playerid); //��ʼ���ٶȱ�
    if(!PlayerInfo[playerid][speedoMeter]) {
        for (new a = 0; a <= 21; a++) {
            PlayerTextDrawHide(playerid, velo[playerid][a]);
        }
    }
    if(PlayerInfo[playerid][enableInvincible]) SetPlayerHealth(playerid, 999999999); //����޵�״̬
    InitializationNetWork(playerid); //��ʼ���������

    // format(PlayerInfo[playerid][Email], MAX_EMAIL_LENGTH, "-1");
    Race_OnPlayerConnect(playerid);
    tdSpeedo_Connect(playerid); //��ʼ��3D�ٶȱ�
    team_OnPlayerLogin(playerid);
    Camera_OnPlayerConnect(playerid);
    DeathMatch_PlayerLogin(playerid);
    Initialize_pWorld(playerid);
    // PHouse_PlayerLogin(playerid); //��ʼ��PHouse��Area
    HouseSellPlayerInitialize(playerid); //��֪�� ������PHouse���ʼ��sellto��buyit��
    Attire_ReadData(playerid); //��ȡ���װ������
    Boards_OnPlayerConnect(playerid); //��ʼ��������
    Cars_OnPlayerConnect(playerid); //������ҵ�����Ĭ�ϲ���
    Tele_OnPlayerLogin(playerid); //��Ҳ鿴�����б�ʱ�ĳ�ʼ��ҳ��
    EnableStuntBonusForPlayer(playerid, 1); //�ر���Ч������ʾ
    GivePlayerMoney(playerid, 99999999);
    Initialize_tpa(playerid);
    //Race_OnPlayerConnect(playerid); 2020.1.13�ĵ�onplayerconnect
    //���ܻᵼ�����ݴ���
    PlayerInfo[playerid][showStunt] = 1;
    PlayerInfo[playerid][showName] = 1;
    PlayerColorPage[playerid] = 1; //����޸���ɫ�Ĳ˵�Ĭ�ϵ�һҳ
    PlayerInfo[playerid][hys] = false; //Ĭ����ҳ�������ɫ�ر�
    PlayerInfo[playerid][lastZpos] = 0; //��һ���Z������ ���ڷ��ؾ������쳣����
    // PlayerInfo[playerid][lastVehSpeed] = 0; //��һ��ĳ��� ���ڷ��ؾ������쳣����

    SelectHousePage[playerid] = 1; //���ѡ���ӵ�ҳ��Ĭ����1
    splp[playerid] = 0; //����Ƿ�ʹ����/sp��״̬�ж�
    PlayerInfo[playerid][AFKTimes] = 0; //��ҹһ�ʱ���ʼ��0�� ���ڼ������Ƿ��������йһ�
    PlayerInfo[playerid][yssyjsq] = -1;
    return true;
}



// ��ȫ������easydialog,����ԭ�е�dialogresponse
// public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) {
//     return 1;
// }


// �ٶȱ��TV��ս���ٶȱ�����趨
function updateSpeedometer() {
    for (new i = GetPlayerPoolSize(); i >= 0; i--) {
        if(IsPlayerConnected(i) && PlayerInfo[i][Login]) {
            tdSpeedo_Update(i, GetSpeed(i));
            if(PlayerInfo[i][speedoMeter]) {
                new string[32];
                // format(string, sizeof(string), "%03d", GetSpeed(i));
                format(string, sizeof(string), "%03d", GetSpeed(PlayerInfo[i][tvid]));
                PlayerTextDrawSetString(i, velo[i][1], string);
                // switch (GetSpeed(i)) {
                switch (GetSpeed(PlayerInfo[i][tvid])) {
                    case 0:{
                        for (new a = 2; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xC0C0C0C8);
                        }
                        for (new a = 19; a < 22; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0x9D4D4DFF);
                        }
                    }
                    case 1..9:{
                        PlayerTextDrawColor(i, velo[i][2], 0xFFCC00C8);
                        // TextDrawShowForPlayer(i, velo[i][2]);
                        for (new a = 3; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xC0C0C0C8);
                        }
                        for (new a = 19; a < 22; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0x9D4D4DFF);
                        }
                    }
                    case 10..19:{
                        for (new a = 2; a < 3; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xFFCC00C8);
                        }
                        for (new a = 4; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xC0C0C0C8);
                        }
                        for (new a = 19; a < 22; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0x9D4D4DFF);
                        }
                    }
                    case 20..29:{
                        for (new a = 2; a < 4; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xFFCC00C8);
                        }
                        for (new a = 5; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xC0C0C0C8);
                        }
                        for (new a = 19; a < 22; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0x9D4D4DFF);
                        }
                    }
                    case 30..39:{
                        for (new a = 2; a < 5; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xFFCC00C8);
                        }
                        for (new a = 6; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xC0C0C0C8);
                        }
                        for (new a = 19; a < 22; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0x9D4D4DFF);
                        }
                    }
                    case 40..49:{
                        for (new a = 2; a < 6; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xFFCC00C8);
                        }
                        for (new a = 7; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xC0C0C0C8);
                        }
                        for (new a = 19; a < 22; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0x9D4D4DFF);
                        }
                    }
                    case 50..59:{
                        for (new a = 2; a < 7; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xFFCC00C8);
                        }
                        for (new a = 8; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xC0C0C0C8);
                        }
                        for (new a = 19; a < 22; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0x9D4D4DFF);
                        }
                    }
                    case 60..69:{
                        for (new a = 2; a < 8; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xFFCC00C8);
                        }
                        for (new a = 9; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xC0C0C0C8);
                        }
                        for (new a = 19; a < 22; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0x9D4D4DFF);
                        }
                    }
                    case 70..79:{
                        for (new a = 2; a < 9; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xFFCC00C8);
                        }
                        for (new a = 10; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xC0C0C0C8);
                        }
                        for (new a = 19; a < 22; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0x9D4D4DFF);
                        }
                    }
                    case 80..89:{
                        for (new a = 2; a < 10; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xFFCC00C8);
                        }
                        for (new a = 11; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xC0C0C0C8);
                        }
                        for (new a = 19; a < 22; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0x9D4D4DFF);
                        }
                    }
                    case 90..99:{
                        for (new a = 2; a < 11; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xFFCC00C8);
                        }
                        for (new a = 12; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xC0C0C0C8);
                        }
                        for (new a = 19; a < 22; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0x9D4D4DFF);
                        }
                    }
                    case 100..109:{
                        for (new a = 2; a < 12; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xFFCC00C8);
                        }
                        for (new a = 13; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xC0C0C0C8);
                        }
                        for (new a = 19; a < 22; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0x9D4D4DFF);
                        }
                    }
                    case 110..119:{
                        for (new a = 2; a < 13; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xFFCC00C8);
                        }
                        for (new a = 14; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xC0C0C0C8);
                        }
                        for (new a = 19; a < 22; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0x9D4D4DFF);
                        }
                    }
                    case 120..129:{
                        for (new a = 2; a < 14; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xFFCC00C8);
                        }
                        for (new a = 15; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xC0C0C0C8);
                        }
                        for (new a = 19; a < 22; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0x9D4D4DFF);
                        }
                    }
                    case 130..139:{
                        for (new a = 2; a < 15; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xFFCC00C8);
                        }
                        for (new a = 16; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xC0C0C0C8);
                        }
                        for (new a = 19; a < 22; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0x9D4D4DFF);
                        }
                    }
                    case 140..149:{
                        for (new a = 2; a < 16; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xFFCC00C8);
                        }
                        for (new a = 17; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xC0C0C0C8);
                        }
                        for (new a = 19; a < 22; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0x9D4D4DFF);
                        }
                    }
                    case 150..159:{
                        for (new a = 2; a < 17; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xFFCC00C8);
                        }
                        for (new a = 18; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xC0C0C0C8);
                        }
                        for (new a = 19; a < 22; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0x9D4D4DFF);
                        }
                    }
                    case 160..169:{
                        for (new a = 2; a < 18; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xFFCC00C8);
                        }
                        for (new a = 18; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xC0C0C0C8);
                        }
                        for (new a = 19; a < 22; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0x9D4D4DFF);
                        }
                    }
                    case 170..179:{
                        for (new a = 2; a < 19; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xFFCC00C8);
                        }
                        for (new a = 21; a < 22; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0x9D4D4DFF);
                        }
                    }
                    case 180..189:{
                        for (new a = 2; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xFFCC00C8);
                        }
                        PlayerTextDrawColor(i, velo[i][19], 0x9D4D4DFF);
                        PlayerTextDrawColor(i, velo[i][20], 0x9D4D4DFF);
                        PlayerTextDrawColor(i, velo[i][21], 0x9D4D4DFF);
                    }
                    case 190..199:{
                        for (new a = 2; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xFFCC00C8);
                        }
                        PlayerTextDrawColor(i, velo[i][19], 0xF20D0DC8);
                        PlayerTextDrawColor(i, velo[i][20], 0x9D4D4DFF);
                        PlayerTextDrawColor(i, velo[i][21], 0x9D4D4DFF);
                    }
                    case 200..209:{
                        for (new a = 2; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xFFCC00C8);
                        }
                        PlayerTextDrawColor(i, velo[i][19], 0xF20D0DC8);
                        PlayerTextDrawColor(i, velo[i][20], 0xF20D0DC8);
                        PlayerTextDrawColor(i, velo[i][21], 0x9D4D4DFF);
                    }
                    case 210..999:{
                        for (new a = 2; a < 20; a++) {
                            PlayerTextDrawColor(i, velo[i][a], 0xFFCC00C8);
                        }
                        PlayerTextDrawColor(i, velo[i][19], 0xF20D0DC8);
                        PlayerTextDrawColor(i, velo[i][20], 0xF20D0DC8);
                        PlayerTextDrawColor(i, velo[i][21], 0xF20D0DC8);
                    }
                }
                for (new a = 1; a <= 21; a++) {
                    PlayerTextDrawShow(i, velo[i][a]); //drawcolor���������show�Ż�ˢ��  ���ٶȲ��� ��������Ҫ��ôд
                }
                // // 2020.3.29ע�� ���������ȡ����ȥ
                // for (new j = GetPlayerPoolSize(); j >= 0; j--) {
                //     if(IsPlayerConnected(j)) {
                //         if(PlayerInfo[j][tvid] == i && PlayerInfo[j][speedoMeter]) {
                //             for (new a = 1; a <= 21; a++) {
                //                 PlayerTextDrawShow(j, velo[i][a]); //drawcolor���������show�Ż�ˢ��  ���ٶȲ��� ��������Ҫ��ôд
                //             }
                //         }
                //     }
                // }
            }
        }
    }
    return 0;
}



/*else
{
	for(new a; a<22; a++) TextDrawHideForPlayer(i,velo[i][a]);
	PlayerTextDrawSetString(i,velo[i][1],"");
}*/
/*else
				{
					if(GetPlayerState(PlayerInfo[i][tvid]) == PLAYER_STATE_DRIVER) 
					{
						for(new a=0; a<24; a++)
						{
							TextDrawShowForPlayer(i,velo[PlayerInfo[i][tvid]][a]);
						}
					}
					if(GetPlayerState(PlayerInfo[i][tvid]) == PLAYER_STATE_ONFOOT)
					{
						for(new a=0; a<24; a++)
						{
							TextDrawHideForPlayer(i,velo[PlayerInfo[i][tvid]][a]);
						}
					}
				format(msg, sizeof(msg), "�ٶ�:%.1f km/h",GetSpeed(PlayerInfo[i][tvid]));//ԭ�����ݵ��ٶȱ�
				TextDrawSetString(Speedtextdraw[PlayerInfo[i][tvid]], msg);	
				}
			}*/


// function tpaTimer(playerid) {
//     if(tpaB[playerid] == 1) {
//         SCM(playerid, Color_White, "[tp]����1������û�н��ܴ�������,������������ʧЧ");
//     }
//     if(tpaB[playerid] == 2) {
//         SCM(playerid, Color_White, "[tp]�Է���1������û�н�����Ĵ�������,������������ʧЧ.");
//     }
//     tpaB[playerid] = 0;
//     tpaid[playerid] = -1;
//     return 1;
// }
public OnPlayerModelSelection(playerid, response, listid, modelid) {
    if(listid == planelist || listid == chaopaolist || listid == Motorolalist || listid == Shiplist || listid == Otherlist || listid == trainlist || listid == minzhen || listid == yueyelist || listid == tuoche || listid == huoche || listid == jingchelist) {
        if(response) {
            new Float:x, Float:y, Float:z;
            GetPlayerPos(playerid, x, y, z);
            SetPlayerPos(playerid, x + 1, y, z + 2);
            SetTimerEx_("SpawnVehicle", 200, 200, 1, "ii", playerid, modelid);
        }
        return 1;
    }
    if(listid == skinlist) {
        if(response) {
            SendClientMessage(playerid, Color_White, "[Ƥ��] �Ѹ�������Ƥ��.");
            SetPlayerSkin(playerid, modelid);
            PlayerInfo[playerid][Skin] = modelid;
        } else SendClientMessage(playerid, Color_White, "[Ƥ��] ����ȡ������Ƥ��.");
        return 1;
    }
    return 1;
}

function SecondsTimer() {
    new n, y, day;
    getdate(n, y, day);
    if(day == 4 && y == 4) {
        new hour, minute, second;
        gettime(hour, minute, second);
        new str[90];
        // h = 24-h;
        // m = 60-m;
        // s = 60-s;
        hour = 24 - hour;
        if(hour == 24) hour = 0;

        if(minute != 0) hour--;
        minute = 60 - minute;
        if(minute == 60) minute = 0;

        if(second != 0) minute--;
        second = 60 - second;
        if(second == 60) second = 0;

        format(str, sizeof(str), "hostname [RST�Ŷ�]�廳���ߣ��¾�Ӣ�� %02d:%02d:%02d", hour, minute, second);
        SendRconCommand(str);
        SendRconCommand("password !?@#^WAgse30ut");
        if(day >= 5) SendRconCommand("password");
    }
    if(day == 31 && y == 12) {
        new hour, minute, second;
        gettime(hour, minute, second);
        new str[90];
        // h = 24-h;
        // m = 60-m;
        // s = 60-s;
        hour = 24 - hour;
        if(hour == 24) hour = 0;

        if(minute != 0) hour--;
        minute = 60 - minute;
        if(minute == 60) minute = 0;

        if(second != 0) minute--;
        second = 60 - second;
        if(second == 60) second = 0;
        format(str, sizeof(str), "hostname [RUD�Ŷ�]����%d���� %02d:%02d:%02d", n, hour, minute, second);
        SendRconCommand(str);
        if(hour == 23 && minute == 59) {
            format(str, sizeof(str), "[����]����%d���� %02d:%02d:%02d", n, hour, minute, second);
            SendClientMessageToAll(Color_White, str);
            if(second + 1 == 0) {
                format(str, sizeof(str), "[����]�ټ�%d,���%d", n, n + 1);
                SendClientMessageToAll(Color_White, str);
                SendClientMessageToAll(Color_White, "һƬ�����������������죬ɢ�����������ķ����������帣���߿գ�������˳�ģ�����ӭ���꣬�˷��պ������þ�����");
            }
        }
    }
    if(day == 1 && y == 1) {
        new hour, minute, second;
        gettime(hour, minute, second);
        new str[90];
        format(str, sizeof(str), "hostname Romantic until Deathף������������! %02d:%02d:%02d", hour, minute, second);
        SendRconCommand(str);
    }
    for (new i = GetPlayerPoolSize(); i >= 0; i--) {
        if(IsPlayerConnected(i)) {
            if(IsPlayerNPC(i)) {
                if(gettime() - PlayerInfo[i][nitro] >= 15) //ÿ15������һ������ ԭ���ĵ����㷨
                {
                    AddVehicleComponent(GetPlayerVehicleID(i), 1010);
                    PlayerInfo[i][nitro] = gettime();
                }
                continue;
            }
            if(PlayerInfo[i][Yztime] != 0) { //�����������֤��ʱ�䣬�����Ϊ0�Ļ���һֱ����0
                PlayerInfo[i][Yztime]--;
                if(PlayerInfo[i][Yztime] < 0) PlayerInfo[i][Yztime] = 0;
            }
            if(PlayerInfo[i][Login] == true) {
                // tpa����ʱ��
                if(PlayerInfo[i][tpa_requesttimer] != 0) {
                    PlayerInfo[i][tpa_requesttimer]--;
                    if(PlayerInfo[i][tpa_requesttimer] <= 0) Initialize_tpa(i);
                }
                updatePlayerNetWorkState(i); //�����������
                if(PlayerInfo[i][JailSeconds] != 0) { //�����ұ�������Ǹ�ʱ���������
                    PlayerInfo[i][JailSeconds]--;
                    if(PlayerInfo[i][JailSeconds] <= 0) {
                        PlayerInfo[i][JailSeconds] = 0; //����0�˾ͷų���������DB���ݿ�
                        // SetSpawnInfo(i, NO_TEAM, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
                        SpawnPlayer(i);
                        new query[128];
                        mysql_format(g_Sql, query, sizeof(query), "UPDATE `users` SET JailSeconds = 0 WHERE `Name` = '%e'", GetName(i));
                        mysql_pquery(g_Sql, query);
                    } else {
                        SetPlayerPos(i, 1607.480, 1670.444, 10.549);
                    }
                }
                running_PPC(i);
                Attire_Preview_Range(i); //װ��Ԥ����Χ �����˾��Զ�ж��
                AntiCommand[i] = 0; //��ҵĹ��ȷ�������0
                //PlayerInfo[i][pLastCheck] = GetTickCount(); 
                //���˵GetTickCount() > pLastCheck+2000�Ļ�˵����ҹһ� 2000ָ2������Ҷ�û���µĻ� ���ǲ��ܷ��������ж�
                //����˵�ٿ�����ʱ��ȥ���Ǹ�lastcheck
                if(GetPlayerState(i) == PLAYER_STATE_DRIVER) //�������ڳ��ϵĻ�
                {
                    new VehID = GetPlayerVehicleID(i);
                    // pACEdit[playerid]����ҵİ���ID �����-1�Ļ���û�а���
                    //   ���� Ӧ����pACEdit[i][gotoId]
                    if(VehID == PlayerInfo[i][BuyID] || VehID == CarInfo[pACEdit[i]][GotoID]) {
                        if(gettime() - PlayerInfo[i][nitro] >= 15) //ÿ15������һ������ ԭ���ĵ����㷨
                        {
                            AddVehicleComponent(VehID, 1010);
                            PlayerInfo[i][nitro] = gettime();
                        }
                        if(PlayerInfo[i][hys]) ChangeVehicleColor(VehID, random(256), random(256)); //�����ҿ��˻���ɫ���Զ���������ɫ
                        // ��⳵���Ƿ�ת By Babul https://forum.sa-mp.com/member.php?u=64500
                        if(PlayerInfo[i][AutoFlip]) {
                            new Float:VehPosX, Float:VehPosY, Float:VehPosZ, Float:VehAngle, Float:Q[4];
                            GetVehiclePos(VehID, VehPosX, VehPosY, VehPosZ);
                            GetVehicleZAngle(VehID, VehAngle);
                            GetVehicleRotationQuat(VehID, Q[0], Q[1], Q[2], Q[3]);
                            new Float:sqw = Q[0] * Q[0];
                            new Float:sqx = Q[1] * Q[1];
                            new Float:sqy = Q[2] * Q[2];
                            new Float:sqz = Q[3] * Q[3];
                            new Float:bank = atan2(2 * (Q[2] * Q[3] + Q[1] * Q[0]), -sqx - sqy + sqz + sqw);
                            if(floatabs(bank) > 160 && GetSpeed(i) < 0.01) {
                                SetVehiclePos(VehID, VehPosX, VehPosY, VehPosZ);
                                SetVehicleZAngle(VehID, VehAngle);
                                SendClientMessage(i, Color_White, "[ϵͳ]�㿪���˳����Զ���������,��ת�ɹ�������/sz�ر�");
                                GameTextForPlayer(i, "~w~vehicle ~g~fl~h~ip~h~pe~h~d", 2000, 4);
                            }
                        }
                        //�������ؾ������쳣������ By:[R_ST]Hygen
                        if(pRaceing[i]) { //�������������� �����ڳ��� 2020.2.23���� ��Ȼ�Ļ������  //�����ͣ״̬�Ļ�Ҫ�����㷨 ��Ȼ���˷���Դ
                            if(IsPlayerInAnyVehicle(i)) {
                                new Float:VehPosX, Float:VehPosY, Float:VehPosZ;
                                GetVehiclePos(VehID, VehPosX, VehPosY, VehPosZ);
                                //2020.3.15���� ---��ʼ
                                new Float:secondsxyz;
                                secondsxyz = GetPlayerDistanceFromPoint(i, PlayerInfo[i][lastXMoved], PlayerInfo[i][lastYMoved], PlayerInfo[i][lastZMoved]);
                                //ÿ�����һ��ľ��� ������������������쳣 ������˲��
                                new Float:difference = VehPosZ - PlayerInfo[i][lastZpos];
                                if(difference <= -1.0) difference *= -1; //ȡ����Z��߶Ȳ�ľ���ֵ
                                if(difference <= 3) {
                                    if(secondsxyz >= 79.333333) {
                                        if(GameRace[i][rgamecp] > 1 && PlayerInfo[i][lastVehSpeed]) { //2020.3.17д��һ��lastVehSpeed  ��Ȼ�������������
                                            if(!IsModelAPlane(VehID)) {
                                                if(GetSpeed(i) >= 225 || GetSpeed(i) <= 90) {
                                                    //�����ҿ��ĳ����Ƿɻ��Ļ�
                                                    //2020.4.3���� �ٶ��Ƿ����223.123�ٽ����ж� �� �ٶ�̫��˲��  ��ֹ��ping̫����ը������
                                                    new trcp[racecptype];
                                                    Race_GetCp(RaceHouse[GameRace[i][rgameid]][rraceid], GameRace[i][rgamecp] - 1, trcp);
                                                    new ss = Race_GetCp_Scripts(trcp[rcpid]); //���ص�ǰCP���м����߼�CP�����������0�Ļ� ��ֱ�ӷ�;
                                                    // new Float:health;
                                                    // GetPlayerHealth(i, health);
                                                    if(!ss) {
                                                        //return FuckAnitCheat(i, "�°淴˲�Ʋ���1", 11);
                                                    } else {
                                                        //if(Race_CheckPlayerCheat(ss, trcp[rcpid])) return FuckAnitCheat(i, "�°淴˲�Ʋ���2", 11);
                                                    }
                                                }
                                            } else //��Ȼ�Ļ����÷ɻ������ÿ���ƶ�����������
                                            {
                                                if(secondsxyz >= 89.333333) {
                                                    new trcp[racecptype];
                                                    Race_GetCp(RaceHouse[GameRace[i][rgameid]][rraceid], GameRace[i][rgamecp] - 1, trcp);
                                                    new ss = Race_GetCp_Scripts(trcp[rcpid]); //���ص�ǰCP���м����߼�CP�����������0�Ļ� ��ֱ�ӷ�;
                                                    if(!ss) {
                                                        //return FuckAnitCheat(i, "�°淴˲�Ʋ���3", 11);
                                                    } else {
                                                        //if(Race_CheckPlayerCheat(ss, trcp[rcpid])) return FuckAnitCheat(i, "�°淴˲�Ʋ���4", 11);
                                                    }
                                                    return 1;
                                                }
                                            }
                                        }
                                    }
                                    if(secondsxyz <= 0.001 && GameRace[i][rgamecp] > 1) { //����Ƿ�һ�  �һ�ʱ��̫���Ļ� �˳�����
                                        PlayerInfo[i][AFKTimes]++;
                                        if(PlayerInfo[i][AFKTimes] >= 45) {
                                            Race_Game_Quit(i);
                                            Dialog_Show(i, MessageBox, DIALOG_STYLE_MSGBOX, "��Դ��ʡ", "�һ�ʱ�����,�����Ƴ�����", "�յ�", "");
                                            return 1;
                                        }
                                    }
                                }
                                PlayerInfo[i][lastXMoved] = VehPosX;
                                PlayerInfo[i][lastYMoved] = VehPosY;
                                PlayerInfo[i][lastZMoved] = VehPosZ;
                                //2020.3.15���� ---��β

                                //����ֵ
                                //����߶Ȳ�����... ��ΪMTA���������̫���� MTA�������ǽ����������ķ����׷���
                                if(VehPosZ < 300.0 && !IsModelAPlane(VehID) && GetSpeed(i) >= 233.456 && difference <= 0.1987) { //�����һ���ڸ߶�ƫ��̫��Ҳ����
                                    if(GameRace[i][rgamecp] > 1) {
                                        new trcp[racecptype];
                                        Race_GetCp(RaceHouse[GameRace[i][rgameid]][rraceid], GameRace[i][rgamecp] - 1, trcp);
                                        new ss = Race_GetCp_Scripts(trcp[rcpid]); //���ص�ǰCP���м����߼�CP�����������0�Ļ� ��ֱ�ӷ�;
                                        // new Float:health;
                                        // GetPlayerHealth(i, health);
                                        if(!ss) {
                                            if(GameRace[i][rgamecp] > 2) {
                                                //�����һ����Ҳ���Ǹ߼�CP����ôֱ��΢���� ���дд�� ΢����Ҫ��˲ʱ�ٶ�ȥд �Ͷ�һ����ʱ���ٶȺܿ�ܿ� 
                                                // updateҲ�� ��������ȥ������һ�μ����ٶ����̫������ ֱ�ӷ�ͺ���
                                                Race_GetCp(RaceHouse[GameRace[i][rgameid]][rraceid], GameRace[i][rgamecp] - 2, trcp);
                                                ss = Race_GetCp_Scripts(trcp[rcpid]);
                                                if(!ss) {
                                                    Race_Game_Quit(i);
                                                    //Dialog_Show(i, MessageBox, DIALOG_STYLE_MSGBOX, "[ϵͳ]", "�����쳣,���Ժ��� Code:10", "ȷ��", "");
                                                    // FuckAnitCheat(i,"˫�߼�CP����΢����",10);
                                                    //return 1;
                                                } else {
                                                    //return 1;
                                                }
                                            }
                                            Race_Game_Quit(i);
                                            //Dialog_Show(i, MessageBox, DIALOG_STYLE_MSGBOX, "[ϵͳ]", "�����쳣,���Ժ��� Code:10", "ȷ��", "");
                                            //return 1;
                                            // FuckAnitCheat(i,"����΢����",10);
                                        }
                                    } else if(GameRace[i][rgamecp] == 1) {
                                        Race_Game_Quit(i);
                                        //Dialog_Show(i, MessageBox, DIALOG_STYLE_MSGBOX, "[ϵͳ]", "�����쳣,���Ժ��� Code:10", "ȷ��", "");
                                        //return 1;
                                        // FuckAnitCheat(i,"�׸�CP���쳣",10);
                                    }
                                    //return 1;
                                }
                                if(VehPosZ > PlayerInfo[i][lastZpos] && GetSpeed(i) > 290 && difference >= 25.0 && GameRace[i][rgamecp] > 1) { //�����һ���ڸ߶�ƫ��̫��Ҳ����
                                    new trcp[racecptype];
                                    Race_GetCp(RaceHouse[GameRace[i][rgameid]][rraceid], GameRace[i][rgamecp] - 1, trcp);
                                    new ss = Race_GetCp_Scripts(trcp[rcpid]); //���ص�ǰCP���м����߼�CP�����������0�Ļ� ��ֱ�ӷ�;
                                    // new Float:health;
                                    // GetPlayerHealth(i, health);
                                    if(!ss) {
                                        //return FuckAnitCheat(i, "�������ؾ�+Z��߶Ⱥ��ٶ��쳣", 1);
                                    } else {
                                        //if(Race_CheckPlayerCheat(ss, trcp[rcpid])) return FuckAnitCheat(i, "�������ؾ�+Z��߶Ⱥ��ٶ��쳣", 1);
                                    }
                                }
                                if(difference <= 3 && GameRace[i][rgamecp] > 1) { //�������Z����������ľ���ֵ  < 4 �Ļ�
                                    //�����һ��ĳ��ٱ���εͣ�����һ���ȥ��һ���ֵ��̫�� ��һ����ٶȶ���70�� ���� z��ûʲô�仯
                                    // Ҳ����˵����ڼ���û�ı�߶ȵ�����³�����˲������ȥ����ô�����Ͼ������ݻ���CLEO���쳣
                                    if((PlayerInfo[i][lastVehSpeed] >= 0 && PlayerInfo[i][lastVehSpeed] < GetSpeed(i) && !IsModelAPlane(VehID) && GetSpeed(i) - PlayerInfo[i][lastVehSpeed] >= 104.0) || (PlayerInfo[i][lastVehSpeed] >= 85 && PlayerInfo[i][lastVehSpeed] < GetSpeed(i) && GetSpeed(i) - PlayerInfo[i][lastVehSpeed] >= 75) || (GetSpeed(i) > 266 && !IsModelAPlane(VehID)) || (GetSpeed(i) > 290 && IsModelAPlane(VehID))) {
                                        //�ж���ҵ�CP���Ƿ��Ǹ߼�CP�㣬����������ɱ
                                        // Ӧ���ǿ��Լ���ؾ������쳣 / ΢���� / �������ʱ�ٸı�
                                        new trcp[racecptype];
                                        Race_GetCp(RaceHouse[GameRace[i][rgameid]][rraceid], GameRace[i][rgamecp] - 1, trcp);
                                        new ss = Race_GetCp_Scripts(trcp[rcpid]); //���ص�ǰCP���м����߼�CP�����������0�Ļ� ��ֱ�ӷ�;
                                        if(!ss) {
                                            //return FuckAnitCheat(i, "�������ؾ�+Z��߶Ⱥ��ٶ��쳣", 2);
                                        } else {
                                            //if(Race_CheckPlayerCheat(ss, trcp[rcpid])) return FuckAnitCheat(i, "�������ؾ�+Z��߶Ⱥ��ٶ��쳣", 1);
                                        }
                                    }
                                }
                                PlayerInfo[i][lastZpos] = VehPosZ; //��¼��һ���Z��
                                PlayerInfo[i][lastVehSpeed] = GetSpeed(i); //��¼��һ��ĳ���
                            }
                        }
                    }
                    if(PlayerInfo[i][AutoFix] && p_PPC[i] == 0 && GetPlayerVehicleID(i)) RepairVehicle(GetPlayerVehicleID(i)); //�����ҿ����Զ��޳��в������������޳�
                } else //���û�ڳ��ϵ�ʱ���״̬
                {
                    if(pRaceing[i] && GetPlayerState(i) == PLAYER_STATE_ONFOOT) //�������е��ǲ��ڳ���
                    {
                        new Float:pPos[3], Float:secondsxyz;
                        GetPlayerPos(i, pPos[0], pPos[1], pPos[2]);
                        secondsxyz = GetPlayerDistanceFromPoint(i, PlayerInfo[i][lastXMoved], PlayerInfo[i][lastYMoved], PlayerInfo[i][lastZMoved]);
                        if(secondsxyz <= 0.001 && GameRace[i][rgamecp] > 1) { //����Ƿ�һ�  �һ�ʱ��̫���Ļ� �˳�����
                            PlayerInfo[i][AFKTimes]++;
                            if(PlayerInfo[i][AFKTimes] >= 45) {
                                Race_Game_Quit(i);
                                Dialog_Show(i, MessageBox, DIALOG_STYLE_MSGBOX, "��Դ��ʡ", "�һ�ʱ�����,�����Ƴ�����", "�յ�", "");
                                return 1;
                            }
                        }
                        PlayerInfo[i][lastXMoved] = pPos[0];
                        PlayerInfo[i][lastYMoved] = pPos[1];
                        PlayerInfo[i][lastZMoved] = pPos[2];
                        // printf("%f", secondsxyz);
                        new Float:difference = pPos[2] - PlayerInfo[i][lastZpos];
                        PlayerInfo[i][lastZpos] = pPos[2];
                        if(difference <= -1.0) difference *= -1; //ȡ����Z��߶Ȳ�ľ���ֵ
                        if(difference <= 1.55) {
                            if(GetSpeed(i) > 48.00 || secondsxyz > 13.00) {
                                Race_Game_Quit(i);
                                //Dialog_Show(i, MessageBox, DIALOG_STYLE_MSGBOX, "[ϵͳ]", "�����쳣,���Ժ��� Code:13.1", "ȷ��", "");
                                //return 1;
                            }
                        } else // if(difference>=3)
                        {
                            if(secondsxyz > 61.33) {
                                Race_Game_Quit(i);
                                //Dialog_Show(i, MessageBox, DIALOG_STYLE_MSGBOX, "[ϵͳ]", "�����쳣,���Ժ��� Code:13.2", "ȷ��", "");
                                //return 1;
                            }
                        }
                        return 1;
                    }
                    if(IsPlayerDeathMatch(i) && DeathMatch_Game[i][g_anticheat]) { //2020.3.31д ���������ֱ��ȡ����
                        new Float:health;
                        GetPlayerHealth(i, health);
                        if(health) {
                            new Float:pPos[3], Float:secondsxyz;
                            GetPlayerPos(i, pPos[0], pPos[1], pPos[2]);
                            secondsxyz = GetPlayerDistanceFromPoint(i, PlayerInfo[i][lastXMoved], PlayerInfo[i][lastYMoved], PlayerInfo[i][lastZMoved]);
                            PlayerInfo[i][lastXMoved] = pPos[0];
                            PlayerInfo[i][lastYMoved] = pPos[1];
                            PlayerInfo[i][lastZMoved] = pPos[2];
                            // printf("%f", secondsxyz);
                            new Float:difference = pPos[2] - PlayerInfo[i][lastZpos];
                            PlayerInfo[i][lastZpos] = pPos[2];
                            if(difference <= -1.0) difference *= -1; //ȡ����Z��߶Ȳ�ľ���ֵ
                            if(difference <= 1.55) {
                                if(GetSpeed(i) > 48.00 || secondsxyz > 13.00) {
                                    DeathMatch_Leave(i);
                                    //Dialog_Show(i, MessageBox, DIALOG_STYLE_MSGBOX, "[ϵͳ]", "�����쳣,���Ժ��� Code:13.1", "ȷ��", "");
                                    //return 1;
                                }
                            } else // if(difference>=3)
                            {
                                if(secondsxyz > 61.33) {
                                    DeathMatch_Leave(i);
                                    //Dialog_Show(i, MessageBox, DIALOG_STYLE_MSGBOX, "[ϵͳ]", "�����쳣,���Ժ��� Code:13.2", "ȷ��", "");
                                    //return 1;
                                }
                            }
                        }
                        return 1;
                    }
                }
            }
        }
    }
    seconds_Monument_Act(); // ���ʱ��
    return 1;
}

/*if(GetPlayerState(i) == PLAYER_STATE_ONFOOT || (GetPlayerState(i) == PLAYER_STATE_DRIVER && JB::GetPlayerSpeed (i, true) < 30))
{
    new var,Float:x,y,z;
    var = (floatround (floatsqroot (YukiGetSquaColor_Redistance (x, y, z, PlayerInfo [i][pCurrentPos][0], PlayerInfo [i][pCurrentPos][1], PlayerInfo [i][pCurrentPos][2])) * 3600) / (GetTickCount () - PlayerInfo [i][pLastCheck]));
    if(var >= 300 && var <= 1500)
    {
        FuckAnitCheat (i, "�����ƶ�");
    }
}*/

// ���ܻ�Ƚ�ռ������ ����
// public OnVehicleDamageStatusUpdate(vehicleid, playerid) //0.3a�ĺ���������������ʱ��������Զ��޸����޸�
// {
//     if(PlayerInfo[playerid][AutoFix] && p_PPC[playerid] == 0) RepairVehicle(vehicleid);
//     return 1;
// }

/*stock GetRaceGameFinishList(playerid)
{
	if(pRaceing[playerid]=0 && GameRace[playerid][rgameid]]==kiven)
	{
 		new string[512];
		format(string,sizeof(string),"%s\n%s (%d:%d:%d) ��%i��",string,PlayerName[playerid],time[0],time[1],time[2],RaceHouse[GameRace[playerid][rgameid]][rtop]);
		ShowPlayerDialog(playerid,0, DIALOG_STYLE_MSGBOX,"����ս��",string,"ȷ��","");
	}
	return 1;
}*/

/*stock JB::GetPlayerSpeed (playerid, get3d)
{
	if(IsPlayerInAnyVehicle (playerid))
		GetVehicleVelocity (GetPlayerVehicleID (playerid), x, y, z);
	else
		GetPlayerVelocity (playerid, x, y, z);
#define JB_Speed(%0,%1,%2,%3,%4)	floatround(floatsqroot((%4)?(%0*%0+%1*%1+%2*%2):(%0*%0+%1*%1))*%3*1.6)
	//return JB::Speed(x, y, z, 100.0, get3d);
	return floatround(floatsqroot((false)?(x*x+y*y+z*z):(x*x+y*y))*100.0*1.6)
}*/
stock SCMToAdmins(const color, const string[]) {
    for (new i = GetPlayerPoolSize(); i >= 0; i--) {
        if(IsPlayerConnected(i)) {
            if(PlayerInfo[i][AdminLevel] >= 1) {
                SCM(i, color, string);
            }
        }
    }
    printf(string);
    return 1;
}

stock bigstrtok(const string[], & idx) {
    new length = strlen(string);
    while ((idx < length) && (string[idx] <= ' ')) {
        idx++;
    }
    new offset = idx;
    new result[128];
    while ((idx < length) && ((idx - offset) < (sizeof(result) - 1))) {
        result[idx - offset] = string[idx];
        idx++;
    }
    result[idx - offset] = EOS;
    return result;
}
stock AdminCommandRecord(const playerid, const command[], const text[]) {
    new str[256], File:hFile;
    hFile = fopen("Users/AdminCommandRecord.log", io_append);
    if(hFile) {
        new h = 0, m = 0, s = 0, n = 0, y = 0, day = 0;
        gettime(h, m, s), getdate(n, y, day);
        format(str, sizeof(str), "%s ʹ�������� %s ԭ��:%s,ʱ��:%d/%d/%d %d:%d:%d,IP:%s\r\n", GetName(playerid), command, text, n, y, day, h, m, s, GetIP(playerid));
        pfwrite(hFile, str);
        fclose(hFile);
    }
    return 1;
}
stock PlayerChestRecord(const playerid, const text[]) {
    new str[256], File:hFile;
    hFile = fopen("Users/ChestRecord.log", io_append);
    if(hFile) {
        new h = 0, m = 0, s = 0, n = 0, y = 0, day = 0;
        gettime(h, m, s), getdate(n, y, day);
        format(str, sizeof(str), "%s��%s��ϵͳ��ɱ %d/%d/%d %d:%d:%d,IP:%s\r\n", GetName(playerid), text, n, y, day, h, m, s, GetIP(playerid));
        pfwrite(hFile, str);
        fclose(hFile);
    }
    DelayedKick(playerid); //�ӳ�T�����
    return 1;
}
stock PlayerTextRecord(const text[]) { //��¼�������
    new str[183], File:hFile;
    new n = 0, y = 0, day = 0;
    new h = 0, m = 0, s = 0;
    gettime(h, m, s);
    getdate(n, y, day);
    format(str, sizeof(str), "Users/Text/%d.%d.%d.txt", n, y, day);
    hFile = fopen(str, io_append);
    if(hFile) {
        format(str, sizeof(str), "[%d:%d:%d]%s\r\n", h, m, s, text);
        pfwrite(hFile, str);
        fclose(hFile);
    }
    return 1;
}
stock PlayerCommandRecord(const text[]) { //��¼�������
    new str[200], File:hFile;
    new n = 0, y = 0, day = 0;
    new h = 0, m = 0, s = 0;
    gettime(h, m, s);
    getdate(n, y, day);
    format(str, sizeof(str), "Users/Command/%d.%d.%d.txt", n, y, day);
    hFile = fopen(str, io_append);
    if(hFile) {
        format(str, sizeof(str), "[%d:%d:%d]%s\r\n", h, m, s, text);
        pfwrite(hFile, str);
        fclose(hFile);
    }
    return 1;
}
stock pfwrite(File:handle, const text[]) {
    new l = strlen(text);
    for (new i = 0; i < l; i++) {
        fputchar(handle, text[i], false);
    }
}
stock FuckAnitCheat(const playerid, const text[], const violationCode) {
    // violationCode -1:�����˺� 0:��㴫�� 1:Z���쳣 2:�ؾ������쳣 3:�����糵 999:����Ա��ɱ 7:Fakekilling 8:CarSpam 10:ֱ��΢���� 997 ���δ��¼����
    //new msgs[256];
    // new t=3600;
    //new t = gettime() + 600;
    // new t = gettime() + 86400 * 7; Ĭ��7�� ̫����
    //if(PlayerInfo[playerid][bantotal] > 0 && PlayerInfo[playerid][bantotal] <= 100) GiveCreditpoints(playerid, 10);
    //mysql_format(g_Sql, msgs, sizeof(msgs), "UPDATE `users` SET `BanTime` = %d,`BanReason` = %d,`bantotal` = %d WHERE `Name` = '%e'", t, violationCode, PlayerInfo[playerid][bantotal], GetName(playerid));
    //mysql_pquery(g_Sql, msgs);

    //format(msgs, sizeof(msgs), "[ϵͳ] ���:%s ʹ�õ�������������Ӱ������Ϸ��ƽ��,Code violation #%d �������׷�ɱ��! ", GetName(playerid), violationCode);
    //SCMALL(Color_Red, msgs);
    //format(msgs, sizeof(msgs), "ԭ��:ϵͳ��⵽���������ʹ�õ�������������Ӱ������Ϸ��ƽ��,Code violation #%d\n��·ǧ���� ��ȫ��һ��\n�г����淶 ����������\n���������ֽ���%d\n����������Ϸ�Ƿ��в�����ʹ�õ�CLEO��������\n���ʱ��:0�� 0ʱ 10�� 0��,�˹��޷���Ԥ", violationCode, GetPlayerCreditpoints(playerid));
    //Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "[ϵͳ] ��������Ͽ�����", msgs, "ȷ��", "");
    //PlayerChestRecord(playerid, text);
    //return 1;
}
// forward CheckAso(playerid);
// public CheckAso(playerid) {
//     DestroyDynamic3DTextLabel(NoDieTime[playerid]);
//     if(wdzt[playerid] != 1) SetPlayerHealth(playerid, 100);
//     return 1;
// }
// forward PutPlayerInVehicleEx(playerid, vehicleid, seatid);
// public PutPlayerInVehicleEx(playerid, vehicleid, seatid) {
//     if(IsPlayerConnected(playerid) && vehicleid != INVALID_VEHICLE_ID) {
//         if(PutPlayerInVehicle(playerid, vehicleid, seatid)) {
//             PlayerInfo[playerid][pVehicleEnteColor_Red] = vehicleid;
//             return 1;
//         }
//     }
//     return 0;
// }
// public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid) {
//     if(playertextid == LetterForYou[playerid][3]) {
//         for (new i; i < 4; i++) {
//             PlayerTextDrawDestroy(playerid, LetterForYou[playerid][i]);
//         }
//         CancelSelectTextDraw(playerid);
//         SendClientMessage(playerid, 0xFFFFFFAA, "лл��������GTA:SAMP");
//         if(GetPlayerScore(playerid) < 120) {
//             SendClientMessage(playerid, Color_White, "[ϵͳ]��⵽����Ϸʱ��δ��120���ӣ��Զ��򿪰�����ʾ");
//             AntiCommand[playerid] = 0;
//             OnPlayerCommandText(playerid, "/help");
//         }
//         return 1;
//     }
//     return 0;
// }

stock Action_Play(const playerid, const aid) {
    if(aid < 1 || aid > 21) return SCM(playerid, Color_White, "[����] ������Ķ���ID������.");
    if(aid == 1) SetPlayerSpecialAction(playerid, 68);
    if(aid == 2) ApplyAnimation(playerid, "ped", "cower", 3.0, 1, 0, 0, 0, 0);
    if(aid == 3) ApplyAnimation(playerid, "BEACH", "bather", 4.0, 1, 0, 0, 0, 0);
    if(aid == 4) ApplyAnimation(playerid, "BEACH", "ParkSit_M_loop", 4.0, 1, 0, 0, 0, 0);
    if(aid == 5) ApplyAnimation(playerid, "BEACH", "lay_bac_loop", 4.0, 1, 0, 0, 0, 0);
    if(aid == 6) ApplyAnimation(playerid, "BEACH", "sitnwait_loop_w", 4.0, 1, 0, 0, 0, 0);
    if(aid == 7) ApplyAnimation(playerid, "SUNBATHE", "batherdown", 3.0, 0, 1, 1, 1, 0);
    if(aid == 8) ApplyAnimation(playerid, "CAR", "Fixn_Car_Loop", 4.0, 1, 0, 0, 0, 0);
    if(aid == 9) ApplyAnimation(playerid, "SMOKING", "M_smklean_loop", 4.0, 1, 0, 0, 0, 0);
    if(aid == 10) ApplyAnimation(playerid, "SMOKING", "F_smklean_loop", 4.0, 1, 0, 0, 0, 0);
    if(aid == 11) ApplyAnimation(playerid, "SMOKING", "M_smkstnd_loop", 4.0, 1, 0, 0, 0, 0);
    if(aid == 12) ApplyAnimation(playerid, "SMOKING", "M_smk_out", 4.0, 1, 0, 0, 0, 0);
    if(aid == 13) SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE1);
    if(aid == 14) SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE2);
    if(aid == 15) SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE3);
    if(aid == 16) SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE4);
    if(aid == 17) ApplyAnimation(playerid, "PARK", "Tai_Chi_Loop", 4.0, 1, 0, 0, 0, 0);
    if(aid == 18) ApplyAnimation(playerid, "ped", "SEAT_DOWN", 4.0, 0, 0, 0, 1, 0);
    // if(aid == 19) ApplyAnimation(playerid,"PAULNMAC","wank_loop",4.1,1,0,0,0,0,1); //luguan
    if(aid == 19) SetPlayerSpecialAction(playerid, 10);
    if(aid == 20) ApplyAnimation(playerid, "SHOP", "ROB_StickUp_In", 4.0, 0, 0, 0, 1, 0);
    if(aid == 21) ApplyAnimation(playerid, "PARACHUTE", "FALL_skyDive_DIE", 4.0, 0, 0, 0, 1, 0);
    return 1;
}
// 430, 446, 452, 453, 454, 472, 473, 484, 483, 595
stock PPC_Veh(const playerid) {
    DestroyVehicle(p_ppcCar[playerid]);
    new rand = random(sizeof(PPC_SpawnPos));
    new veh = random(200) + 400;
    p_ppcCar[playerid] = CreateVehicle(veh, PPC_SpawnPos[rand][ppX], PPC_SpawnPos[rand][ppY], PPC_SpawnPos[rand][ppZ], 0, -1, -1, 60);
    while (IsModelAPlane(p_ppcCar[playerid]) || IsModelASpecial(p_ppcCar[playerid]) || IsModelABoat(p_ppcCar[playerid])) { //�����ж��Ƿ��Ƿɻ� �ǵĻ��Ͳ�ˢ
        DestroyVehicle(p_ppcCar[playerid]);
        veh = random(200) + 400;
        p_ppcCar[playerid] = CreateVehicle(veh, PPC_SpawnPos[rand][ppX], PPC_SpawnPos[rand][ppY], PPC_SpawnPos[rand][ppZ], 0, -1, -1, 60);
    }
    //���0~211֮������� + 400 ������IDΪ400~611
    SetVehicleVirtualWorld(p_ppcCar[playerid], 10001);
    SetPlayerPos(playerid, PPC_SpawnPos[rand][ppX], PPC_SpawnPos[rand][ppY], PPC_SpawnPos[rand][ppZ]);
    SetPlayerFacingAngle(playerid, PPC_SpawnPos[rand][ppR]);
    SetVehiclePos(p_ppcCar[playerid], PPC_SpawnPos[rand][ppX], PPC_SpawnPos[rand][ppY], PPC_SpawnPos[rand][ppZ]);
    // LinkVehicleToInterior(PlayerInfo[playerid][BuyID], GetPlayerInterior(playerid));
    SetVehicleZAngle(p_ppcCar[playerid], PPC_SpawnPos[rand][ppR]);
    PutPlayerInVehicle(playerid, p_ppcCar[playerid], 0);
    return 1;
}
stock running_PPC(const playerid) {
    if(p_PPC[playerid]) {
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        // if(z >= 233 && z<=270 && GetSpeed(playerid)>399) { //�����㷨̫������ �����RҲ��
        //     FuckAnitCheat(playerid, "�����������쳣");
        //     return 1;
        // }
        if(z < 150 || z > 260) return PPC_Veh(playerid);
        //�����ҵĸ߶�< 150 �Ǿ��ǵ���ȥ��  ̫��Ҳ����
    }
    return 1;
}
public OnPlayerEditDynamicObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz) {
    Board_EditMode(playerid, objectid, response, x, y, z, rx, ry, rz);
    //First you should detete 3dtext otherwise u will create so many 3dtext labels
    DestroyDynamic3DTextLabel(GOODS[GOODS_OPRATEID[playerid]][Good3DTextLabel]);
    new Float:oldX, Float:oldY, Float:oldZ, Float:oldRotX, Float:oldRotY, Float:oldRotZ;
    GetDynamicObjectPos(objectid, oldX, oldY, oldZ);
    GetDynamicObjectRot(objectid, oldRotX, oldRotY, oldRotZ);

    /*
    	if(!playerDynamicobject) // If this is a global object, move it for other players
    	{
     	if(!IsValidObject(objectid)) return;
     	MoveObject(objectid, fX, fY, fZ, 10.0, fRotX, fRotY, fRotZ);
    	}
    	*/
    if(response == EDIT_RESPONSE_FINAL) {
        GOODS[GOODS_OPRATEID[playerid]][GoodX] = x;
        GOODS[GOODS_OPRATEID[playerid]][GoodY] = y;
        GOODS[GOODS_OPRATEID[playerid]][GoodZ] = z;
        GOODS[GOODS_OPRATEID[playerid]][GoodRX] = rx;
        GOODS[GOODS_OPRATEID[playerid]][GoodRY] = ry;
        GOODS[GOODS_OPRATEID[playerid]][GoodRZ] = rz;
        GOODS[GOODS_OPRATEID[playerid]][WID] = GetPlayerVirtualWorld(playerid);
        SetDynamicObjectPos(objectid, x, y, z);
        SetDynamicObjectRot(objectid, rx, ry, rz);
        new string[285];
        if(GOODS[GOODS_OPRATEID[playerid]][issale] == true) {

            format(string, sizeof(string), "{80FF80}��Ʒ��������...\n{FFFFFF}[��ƷID]:{80FF80} %d\n{FFFFFF}[���˳��ID]:{80FF80} %d\n{FFFFFF}[ģ��ID]:{80FF80} %d\n{FFFFFF}[�۸�]:{80FF80} %d\n{FFFFFF}��{80FF80}Y{FFFFFF}����", GOODS_OPRATEID[playerid], GOODS[GOODS_OPRATEID[playerid]][OrderId], GOODS[GOODS_OPRATEID[playerid]][GoodObjid], GOODS[GOODS_OPRATEID[playerid]][GoodPrize]);
            GOODS[GOODS_OPRATEID[playerid]][Good3DTextLabel] = CreateDynamic3DTextLabel(string, Color_White, GOODS[GOODS_OPRATEID[playerid]][GoodX], GOODS[GOODS_OPRATEID[playerid]][GoodY], GOODS[GOODS_OPRATEID[playerid]][GoodZ], 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, -1, GOODS[GOODS_OPRATEID[playerid]][WID]);
        } else {
            format(string, sizeof(string), "{80FF80}%s\n{FFFFFF}[����]:{80FF80} %s\n{FFFFFF}[��ƷID]:{80FF80} %d\n{FFFFFF}[���˳��ID]:{80FF80} %d\n{FFFFFF}[ģ��ID]:{80FF80} %d\n{FFFFFF}��{80FF80}Y{FFFFFF}����", GOODS[GOODS_OPRATEID[playerid]][GoodName], GOODS[GOODS_OPRATEID[playerid]][GoodOwner], GOODS_OPRATEID[playerid], GOODS[GOODS_OPRATEID[playerid]][OrderId], GOODS[GOODS_OPRATEID[playerid]][GoodObjid]);
            GOODS[GOODS_OPRATEID[playerid]][Good3DTextLabel] = CreateDynamic3DTextLabel(string, Color_White, GOODS[GOODS_OPRATEID[playerid]][GoodX], GOODS[GOODS_OPRATEID[playerid]][GoodY], GOODS[GOODS_OPRATEID[playerid]][GoodZ], 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, -1, GOODS[GOODS_OPRATEID[playerid]][WID]);
        }
        SaveGoods(GOODS_OPRATEID[playerid]);

    }

    if(response == EDIT_RESPONSE_CANCEL) {

        SetDynamicObjectPos(objectid, oldX, oldY, oldZ);
        SetDynamicObjectRot(objectid, oldRotX, oldRotY, oldRotZ);
        GOODS[GOODS_OPRATEID[playerid]][GoodX] = oldX;
        GOODS[GOODS_OPRATEID[playerid]][GoodY] = oldY;
        GOODS[GOODS_OPRATEID[playerid]][GoodZ] = oldZ;
        GOODS[GOODS_OPRATEID[playerid]][GoodRX] = oldRotX;
        GOODS[GOODS_OPRATEID[playerid]][GoodRY] = oldRotY;
        GOODS[GOODS_OPRATEID[playerid]][GoodRZ] = oldRotZ;
        GOODS[GOODS_OPRATEID[playerid]][WID] = GetPlayerVirtualWorld(playerid);
        new string[285];
        if(GOODS[GOODS_OPRATEID[playerid]][issale] == true) {

            format(string, sizeof(string), "{80FF80}��Ʒ��������...\n{FFFFFF}[��ƷID]:{80FF80} %d\n{FFFFFF}[���˳��ID]:{80FF80} %d\n{FFFFFF}[ģ��ID]:{80FF80} %d\n{FFFFFF}[�۸�]:{80FF80} %d\n{FFFFFF}��{80FF80}Y{FFFFFF}����", GOODS_OPRATEID[playerid], GOODS[GOODS_OPRATEID[playerid]][OrderId], GOODS[GOODS_OPRATEID[playerid]][GoodObjid], GOODS[GOODS_OPRATEID[playerid]][GoodPrize]);
            GOODS[GOODS_OPRATEID[playerid]][Good3DTextLabel] = CreateDynamic3DTextLabel(string, Color_White, GOODS[GOODS_OPRATEID[playerid]][GoodX], GOODS[GOODS_OPRATEID[playerid]][GoodY], GOODS[GOODS_OPRATEID[playerid]][GoodZ], 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, -1, GOODS[GOODS_OPRATEID[playerid]][WID]);
        } else {
            format(string, sizeof(string), "{80FF80}%s\n{FFFFFF}[����]:{80FF80} %s\n{FFFFFF}[��ƷID]:{80FF80} %d\n{FFFFFF}[���˳��ID]:{80FF80} %d\n{FFFFFF}[ģ��ID]:{80FF80} %d\n{FFFFFF}��{80FF80}Y{FFFFFF}����", GOODS[GOODS_OPRATEID[playerid]][GoodName], GOODS[GOODS_OPRATEID[playerid]][GoodOwner], GOODS_OPRATEID[playerid], GOODS[GOODS_OPRATEID[playerid]][OrderId], GOODS[GOODS_OPRATEID[playerid]][GoodObjid]);
            GOODS[GOODS_OPRATEID[playerid]][Good3DTextLabel] = CreateDynamic3DTextLabel(string, Color_White, GOODS[GOODS_OPRATEID[playerid]][GoodX], GOODS[GOODS_OPRATEID[playerid]][GoodY], GOODS[GOODS_OPRATEID[playerid]][GoodZ], 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, -1, GOODS[GOODS_OPRATEID[playerid]][WID]);
        }
        SaveGoods(GOODS_OPRATEID[playerid]);
    }
    return 1;
}


// 2020.3.21����  ��5Fֱ�Ӱ������ ��Ҵ���ʱ�ȴ�һ����OBJ�ȼ���

stock DynUpdateStart(const playerid) {
    TogglePlayerControllable(playerid, false); // Freeze
    new string[64];
    format(string, sizeof(string), "~g~Objects~n~~r~Loading");
    GameTextForPlayer(playerid, string, 3000, 6);
    PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
    SetTimerEx_("DynUpdateEnd", 2000, 2000, 1, "i", playerid);
    return 1;
}

function DynUpdateEnd(playerid) {
    TogglePlayerControllable(playerid, true); // Unfreeze
    new string[64];
    format(string, sizeof(string), "~g~Objects~n~~r~Loaded!");
    GameTextForPlayer(playerid, string, 3000, 6);
    PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
    return 1;
}

stock IsModelABoat(const vehicleid) {
    switch (GetVehicleModel(vehicleid)) {
        case 430, 446, 452, 453, 454, 472, 473, 484, 483, 595:
            return true;
    }
    return false;
}

stock IsModelASpecial(const vehicleid) // RC trailer
{
    switch (GetVehicleModel(vehicleid)) {
        case 435, 441, 449, 450, 464, 465, 501, 537, 538, 539, 545, 564, 569..572, 583, 584, 590, 591, 594, 606, 607, 608, 610:
            return true;
    }
    return false;
}

stock IsModelAPlane(const vehicleid) {
    switch (GetVehicleModel(vehicleid)) {
        case 592, 577, 511, 512, 593, 520, 553, 476, 519, 460, 513, 464:
            return true;
        case 548, 425, 417, 487, 488, 497, 563, 447, 469, 465, 501:
            return true;
    }
    return false;
}


CMD:yssy(const playerid, const cmdtext[]) { //��ʱ��Ӱ by KiVen & YuCarl77
    // if() //�������Ѿ�����ʱ��Ӱ��������ֹ
    // �����ҵ����˵�ʱ���ж��Ƿ�����ʱ��Ӱ �������ɱ����ʱ��
    if(PlayerInfo[playerid][yssyjsq] != -1) return SendClientMessage(playerid, Color_White, "[��ʱ]�����ڽ�����ʱ��Ӱ");
    // KillTimer(PlayerInfo[playerid][yssyjsq]);
    new endminute, endseconds, howlong;
    if(sscanf(cmdtext, "iii", endminute, endseconds, howlong)) return SendClientMessage(playerid, Color_White, "[��ʱ]��������� /yssy ʱ �� �೤����");
    if(endminute > 24 || endminute < 0) return SendClientMessage(playerid, Color_White, "[��ʱ]�����Сʱ");
    if(endseconds > 60 || endseconds < 0) return SendClientMessage(playerid, Color_White, "[��ʱ]����ķ���");
    if(howlong > 300 || howlong < 3) return SendClientMessage(playerid, Color_White, "[��ʱ]ʱ�䲻�ɵ���3���Ҳ��ɴ���5����");
    new howlonginterval = min_test(PlayerInfo[playerid][tHour], PlayerInfo[playerid][tMinute], endminute, endseconds);
    // if(endminute < PlayerInfo[playerid][tHour]) endminute+=24;
    PlayerInfo[playerid][yssym] = endseconds; //PlayerInfo[playerid][tMinute];//endseconds;
    PlayerInfo[playerid][yssyh] = endminute; //PlayerInfo[playerid][tHour];//endminute;
    PlayerInfo[playerid][yssyjsq] = SetTimerEx_("SetPlayerYssyJisuan", 20, 20, -1, "ii", playerid, howlonginterval / howlong);
    // ������ҵ���ʱ��Ӱ״̬Ϊ��

    //����������ʱ��Ӱ��״̬��ɱ����ʱ��
    return 1;
}

function SetPlayerYssyJisuan(playerid, howlong) {
    //�������100���� Խ��Խƽ�� ����ҲҪ����ʱ�䳤��ȥ����ÿִ�е�Ƶ��Ӧ�ø����ٷ���ÿִ��Ƶ��
    //������ʱ��420�� Ȼ�����˵Ҫ3��󵽴�420�� ��ô�͵�Ҫÿ��7���� ÿ100�������0.7����
    // PlayerInfo[playerid][yssym]=PlayerInfo[playerid][yssym]+(howlong/10);
    PlayerInfo[playerid][tMinute] = PlayerInfo[playerid][tMinute] + (howlong / 50);
    if(PlayerInfo[playerid][tMinute] >= 60) {
        PlayerInfo[playerid][tMinute] = 0; //���ӹ�0
        PlayerInfo[playerid][tHour]++; //Сʱ��1
        if(PlayerInfo[playerid][tHour] >= 24) {
            PlayerInfo[playerid][tHour] = 0; //Сʱ���賿
        }
    }
    SetPlayerYssy(playerid, PlayerInfo[playerid][tHour], PlayerInfo[playerid][tMinute]);
    return 1;
}

stock SetPlayerYssy(playerid, hour, minute) {
    printf("hour:%d minute:%d", hour, minute);
    SetPlayerTime(playerid, hour, minute);
    // if(PlayerInfo[playerid][yssyh]>24) 
    // {
    //     if(hour>=PlayerInfo[playerid][yssyh] && minute>= PlayerInfo[playerid][yssym])
    //     {
    //         SendClientMessage(playerid, Color_White, "[��ʱ]��ʱ������");
    //         KillTimer(PlayerInfo[playerid][yssyjsq]);
    //         PlayerInfo[playerid][yssyjsq]=-1;
    //         // ���������ʱ��Ӱ״̬Ϊ��
    //     }
    // }
    // if(hour>=PlayerInfo[playerid][tHour] && minute>= PlayerInfo[playerid][tMinute])
    if(hour >= PlayerInfo[playerid][yssyh] && minute >= PlayerInfo[playerid][yssym]) {
        SendClientMessage(playerid, Color_White, "[��ʱ]��ʱ������");
        KillTimer(PlayerInfo[playerid][yssyjsq]);
        PlayerInfo[playerid][yssyjsq] = -1;
        // ���������ʱ��Ӱ״̬Ϊ��
    }
    return 1;
}


stock min_test(sh, sm, eh, em) //��ʼʱ���֣�����ʱ���� ���ؽ���ʱ��
{
    new min_count, hour, minute;
    if(eh < sh) {
        sh = sh - 12;
        eh = eh + 12; //������
    }
    hour = (eh - sh) * 60; //��ʵ����Ϸ��ķ�  //������Сʱ��ȥ��ʼ��Сʱ ��ʵ��ʹ���� 1���� =60����Ǽ�����*60��
    minute = (em - sm); //��ʵ����Ϸ�����
    if(hour < 0) hour *= -1; //ȡ����ֵ �������ʱ��ȿ�ʼʱ��С
    if(minute < 0) minute *= -1; //ȡ����ֵ
    min_count = hour + minute; //���ص������� 
    printf("%d", min_count);
    //��������Ǽ����ԭ����ʱ�䵽��Ҫ��ʱ�� ��ԭ����ʱ��������Ҫ������

    // if(sh < eh)//�жϿ�ʼʱ�ǲ��Ǵ��ڽ���ʱ���������ϵ��ڶ�����������
    // {
    //     sh = sh - 12;
    //     eh = eh + 12;//������
    // }
    // min_count = (sh - eh) * 60;//���Сʱ�ļ�ķ���
    // if(sm > em) min_count += sm - em; 
    // else min_count -= em -sm;
    //���Ƿ��ӣ����ھ���Ҫ�� С�����ҪҪ��
    return min_count;
}
Dialog:HelpSystem(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        new msg[128], idx;
        msg = strtok(inputtext, idx);
        if(!strcmp(msg, "����ƪ")) {
            Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "����ƪ", "/sound1~7 ��SAMP�Դ������֣���Щ���־�����SAMP����������ʹ��Ŷ ����/sound1\n/soundstop ֹͣ��������", "ȷ��", "����");
            return 1;
        }
        if(!strcmp(msg, "����ϵͳ")) {
            Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "����ϵͳ", "��������\n�༭����", "ȷ��", "����");
            return 1;
        }
        if(!strcmp(msg, "���Ի�����")) {
            Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "���Ի�����", "/mynetstats �Ի�����ϸ����״̬\n/sz ��������\n/tianqi �����Լ�������\n/time �����Լ���ʱ��\n/skin��/hf �����Լ�������Ƥ��\n/kgobj ������ر�OBJģ�͵���ʾ\n/kgname ������ر�������ҵ�����\n/name on/off ����/�ر�������ҵ�����\n/stunt on/off ����/�ر���Ļ���·����ؼ���ʾ", "ȷ��", "����");
            return 1;
        }
        if(!strcmp(msg, "�ճ�����")) {
            Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "���Ի�����", "/pm id ˽�� ����/pm 1\n/xiufu �����ﱻ��ס��ʱ��ɳ���ʹ�ø�����Ŷ\n/fxq ˢ��һ����������\n/jlsˢ��һ������ɡ���������ֵ���ɡ֮��\n/wuqi ˢ�����������������������ҿ�ǹŶ��һ������Ƶ�����ֵ�������;ʹ��\n/count��/djs ����ʱ����С��Χ�ĸ�����Ҷ����յ��㷢��ĵ���ʱ�󣬿���һ���������Ƶ��쭳���\n/wudi ���������޵�\n/ppc һ������������", "ȷ��", "����");
            return 1;
        }
        if(!strcmp(msg, "�ؾ����")) {
            Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "�ؾ����", "/c [����ID]��ˢ��������IDΪ400-611\n/cc ��ɫ���� ��ɫ���� ����������ɫ ����/cc 0 0 \n/c ti û����ͬ������ҳ�������Ū��ȥ\n/c wode ��С�İѳ�Ū���ˣ�����Զ�ˣ��ٻ���֮ǰˢ�������ĳ�\n/c suo ���� \n/c chepai\n/fix �޸�һ����ĳ���\n/dcar����/�ر��ؾ��޵�\n/f ����ʱʹ��.������ϵͳ����ʹ��/kill���³�������\n/infobj ���ƺ�β��\n/hys ��ɫ��", "ȷ��", "����");
            return 1;
        }
        if(!strcmp(msg, "�������")) {
            Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "�������", "/w �ص������磬�������Ĭ�϶����������\n/w id �����ض�������\n���Ժ�С���һ��ȥ'С����'��������Ƶ��һ����ˣ�Ҳ������˴���", "ȷ��", "����");
            return 1;
        }
        if(!strcmp(msg, "�ۿ����")) {
            Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "�ۿ����", "/tv ���id ����/tv 1\n/tv off �رչۿ����", "ȷ��", "����");
            return 1;
        }
        if(!strcmp(msg, "����/����")) {
            Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "����/����", "/sp��/s����Ŀǰ������λ��\n/lp��/l���͵�֮ǰ�����λ��\n/vmake����һ������ ��ʽΪ/vmake ������(�淶Ӣ��) ����vmake ldz\n/telemenu�鿴����ϵͳ���͵�\n��������һ����Ϊldz�������´μ���ʹ��/ldz����\n/tpa����һ�������������� ����/tpa playerid\n/ta����������Ҹ��㷢��������\n/td�ܾ�������Ҹ��㷢��������\n/tpa ban����tpa�����ٴ�����ر�", "ȷ��", "����");
            return 1;
        }
        if(!strcmp(msg, "����Աָ���ȫ")) {
            if(!PlayerInfo[playerid][AdminLevel]) return SendClientMessage(playerid, Color_White, "[ϵͳ]�㻹���ǹ���ԱŶ~");
            Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "����Աָ���ȫ", "LV1\nLV2\nLV3\nLV4\nLV5\nLV?", "ȷ��", "����");
            return 1;
        }
        if(!strcmp(msg, "����ϵͳ")) {
            Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "����ϵͳ", "�����ճ�����\n��������Ա����", "ȷ��", "����");
            return 1;
        }
        if(!strcmp(msg, "�Ҿ�ϵͳ")) {
            Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "�Ҿ�ϵͳ", "�Ҿ��ճ�����\n�Ҿ߹���Ա����", "ȷ��", "����");
            return 1;
        }
        if(!strcmp(msg, "װ��ϵͳ")) {
            Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "װ��ϵͳ", "/wdzb��/sz�鿴�ҵ�װ��\n/buyattire����װ��\nװ�繺���/yyk\n��������˻ؽ��", "ȷ��", "����");
            return 1;
        }
        if(!strcmp(msg, "�����ϵͳ")) {
            Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "�����ϵͳ", "�ڹ���Ƹ�����Y�ɲ��������,�ɻ��ս��\n/board buy��������.\n/board list �鿴������б�\n/board goto id���͵������", "ȷ��", "����");
            return 1;
        }
        if(!strcmp(msg, "����ϵͳ")) {
            Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "����ϵͳ", "Ŀǰ����ֻ���ɹ���Ա����./cars buy �ɹ��򰮳� /cars list �鿴�����б�\n /aczb ����װ�� /wdac �ҵİ���\n", "ȷ��", "����");
            return 1;
        }
        if(!strcmp(msg, "�Ŷ�ϵͳ")) {
            Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "�Ŷ�ϵͳ", "�����Ŷ�/t create �Ŷ��б�/t list\n�ҵ��Ŷ�/t wode��/wdtd\n��ȡ����/t get ���͵���Ա/t goto\n/t invite������Ҽ����Ŷ�\n�б�ʽ����\n�Ŷ��û��ȼ���Ϊ0,1,2\n0Ϊ��Ա,1Ϊ����Ա,2Ϊ��ʼ��\n����Ա����ȡ�������Ϊ�Ŷӳ�Ա\n��ʼ�߱ȹ���Ա��һ��ת�úͽ�ɢȨ��", "ȷ��", "����");
            return 1;
        }
        if(!strcmp(msg, "���ϵͳ")) {
            Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "���ϵͳ", "/cam�����\nͨ�����������д���,�Ƚϼ�,�Ͳ�������ϸ������", "ȷ��", "����");
            return 1;
        }
    }
    return 1;
}

Dialog:CustomSettings(playerid, response, listitem, inputtext[]) {
    if(response) {
        switch (listitem) {
            case 0:{ //������
                AntiCommand[playerid] = 0;
                CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/tpa ban");
                ShowCustomSettings(playerid);
                // OnPlayerCommandText(playerid, "/tpa ban");
                // AntiCommand[playerid] = 0;
                // CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/sz");
                return 1;
                // break;
            }
            case 1:{ //�����޵�
                AntiCommand[playerid] = 0;
                CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/wudi");
                ShowCustomSettings(playerid);
                // AntiCommand[playerid] = 0;
                // CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/sz");
                return 1;
                // break;
            }
            case 2:{ //�����޵�
                AntiCommand[playerid] = 0;
                CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/dcar");
                ShowCustomSettings(playerid);
                // AntiCommand[playerid] = 0;
                // CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/sz");
                return 1;
                // break;
            }
            case 3:{ //����OBJ
                AntiCommand[playerid] = 0;
                CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/kgobj");
                ShowCustomSettings(playerid);
                // AntiCommand[playerid] = 0;
                // CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/sz");
                return 1;
                // break;
            }
            case 4:{ //
                if(PlayerInfo[playerid][AutoFlip]) {
                    PlayerInfo[playerid][AutoFlip] = 0;
                    SendClientMessage(playerid, Color_White, "[ϵͳ]�����Զ������ѹر�");
                } else {
                    PlayerInfo[playerid][AutoFlip] = 1;
                    SendClientMessage(playerid, Color_White, "[ϵͳ]�����Զ������ѿ���");
                }
                ShowCustomSettings(playerid);
                // AntiCommand[playerid] = 0;
                // CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/sz");
                return 1;
            }
            case 5:{ //������ײ
                if(PlayerInfo[playerid][NoCrash]) {
                    PlayerInfo[playerid][NoCrash] = 0;
                    DisableRemoteVehicleCollisions(playerid, false);
                    SendClientMessage(playerid, Color_White, "[ϵͳ]������ײ�ѹر�");
                } else {
                    PlayerInfo[playerid][NoCrash] = 1;
                    DisableRemoteVehicleCollisions(playerid, true);
                    SendClientMessage(playerid, Color_White, "[ϵͳ]������ײ�ѿ���");
                }
                ShowCustomSettings(playerid);
                // AntiCommand[playerid] = 0;
                // CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/sz");
                return 1;
            }
            case 6:{ //�ٶȱ�
                cmd_sdb(playerid, "");
                ShowCustomSettings(playerid);
                // AntiCommand[playerid] = 0;
                // CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/sz");
                return 1;
            }
            case 7:{
                if(PlayerInfo[playerid][netStats]) {
                    SendClientMessage(playerid, Color_White, "[ϵͳ]�������������");
                    for (new i = 0; i <= 10; i++) {
                        PlayerTextDrawHide(playerid, PlayerText:network_txtdraw[playerid][i]);
                    }
                    PlayerInfo[playerid][netStats] = false;
                } else {
                    SendClientMessage(playerid, Color_White, "[ϵͳ]�����������ʾ");
                    for (new i = 0; i <= 10; i++) {
                        PlayerTextDrawShow(playerid, PlayerText:network_txtdraw[playerid][i]);
                    }
                    PlayerInfo[playerid][netStats] = true;
                }
                ShowCustomSettings(playerid);
                // AntiCommand[playerid] = 0;
                // CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/sz");
                return 1;
            }
            case 8:{
                if(PlayerInfo[playerid][showName]) {
                    for (new i = GetPlayerPoolSize(); i >= 0; i--) {
                        ShowPlayerNameTagForPlayer(playerid, i, false);
                    }
                    SCM(playerid, Color_White, "[ϵͳ] �����������������.");
                    PlayerInfo[playerid][showName] = 0;
                    ShowCustomSettings(playerid);
                    return 1;
                }
                for (new i = GetPlayerPoolSize(); i >= 0; i--) {
                    ShowPlayerNameTagForPlayer(playerid, i, true);
                }
                SCM(playerid, Color_White, "[ϵͳ] ����ʾ�����������.");
                PlayerInfo[playerid][showName] = 1;
                ShowCustomSettings(playerid);
                return 1;
            }
            case 9:{
                if(PlayerInfo[playerid][showStunt]) {
                    EnableStuntBonusForPlayer(playerid, 0);
                    SCM(playerid, Color_White, "[ϵͳ] �ѹر��ؼ�������ʾ.");
                    PlayerInfo[playerid][showStunt] = 0;
                    ShowCustomSettings(playerid);
                    return 1;
                }
                EnableStuntBonusForPlayer(playerid, 1);
                SCM(playerid, Color_White, "[ϵͳ] �ѿ����ؼ�������ʾ.");
                PlayerInfo[playerid][showStunt] = 1;
                ShowCustomSettings(playerid);
                return 1;
            }
            case 10:{ //ʱ��
                Dialog_Show(playerid, PlayerInfo_Time, DIALOG_STYLE_INPUT, "ʱ������", "������ʱ��,��ʽ:ʱ ��,����5 30", "ȷ��", "ȡ��");
                // break;
                return 1;
            }
            case 11:{ //����
                Dialog_Show(playerid, PlayerInfo_Weather, DIALOG_STYLE_INPUT, "��������", "0-LA����\n1-LA��\n2-LA������\n3-LA����\n4-LA��\n5-SF��\n6-SF����\n7-SF��\n8-SF��\n9-SF��\n10-LV��\n11-LV�Ⱦ�\n12-LV��\n13-��弫��\n14-�����\n15-�����\n16-�����\n17-ɳĮ����\n18-ɳĮ��\n19-ɳĮɳ����\n20-ˮ�£��̣���\n��������������ʽ:���� ����12", "ȷ��", "ȡ��");
                return 1;
            }
        }
        return 1;
    }
    // ShowCustomSettings(playerid);
    AntiCommand[playerid] = 0;
    CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/sz");
    return 1;
}


stock tdSpeedo_Connect(const playerid) {
    PlayerInfo[playerid][tdSpeed] = INVALID_OBJECT_ID;
    PlayerInfo[playerid][tdEnabled] = 0;
    // gTickCount[playerid] = 0;
}

stock tdSpeedo_Disconnect(const playerid) {
    DestroyDynamicObject(PlayerInfo[playerid][tdSpeed]);
}

stock tdSpeedo_Update(const playerid, Float:velocity) {
    if(!PlayerInfo[playerid][tdEnabled]) return 1;
    // if(!IsPlayerInAnyVehicle(playerid)) {
    //     tdSpeedo_Toggle(playerid, 0);
    //     return 1;
    // } else 
    if(!IsPlayerNPC(playerid)) {
        new speedText[48];
        format(speedText, 48, "%03.0f KMH", velocity);
        SetDynamicObjectMaterialText(PlayerInfo[playerid][tdSpeed], 0, speedText, OBJECT_MATERIAL_SIZE_512x256, "Arial", 52, false, Color_White, 0, OBJECT_MATERIAL_TEXT_ALIGN_RIGHT);
    }
    return 1;
}

stock tdSpeedo_Toggle(const playerid, const activate) {
    if(activate) {
        if(PlayerInfo[playerid][tdEnabled]) return 1;
        new vid = GetPlayerVehicleID(playerid);
        new vmod = GetVehicleModel(vid);
        PlayerInfo[playerid][tdSpeed] = CreateDynamicObject(19482, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, playerid, 200.0);
        new Float:vX, Float:vY, Float:vZ;
        GetVehicleModelInfo(vmod, VEHICLE_MODEL_INFO_SIZE, vX, vY, vZ);
        if(vX < 1.5) vX = 1.5;
        AttachDynamicObjectToVehicle(PlayerInfo[playerid][tdSpeed], vid, vX - 1.75, -1.0, -0.6, 0.0, 0.0, -110.0);
        Streamer_Update(playerid);
        PlayerInfo[playerid][tdEnabled] = 1;
        return 1;
    }
    if(PlayerInfo[playerid][tdSpeed] != INVALID_OBJECT_ID) {
        DestroyDynamicObject(PlayerInfo[playerid][tdSpeed]);
        PlayerInfo[playerid][tdSpeed] = INVALID_OBJECT_ID;
    }
    PlayerInfo[playerid][tdEnabled] = 0;
    return 1;
}

stock ShowCustomSettings(const playerid) {
    new tpaState[32], wdState[32], fixState[32], objShow[32], autoflip[32], time[32], tweather[32], pbpz[32], sdb[32], net[32];
    new showName_[32], showStunt_[32], tmp[384];
    if(PlayerInfo[playerid][tpa_ban] == 0) format(tpaState, sizeof(tpaState), "������Ҵ�����Ϣ:{00FF00}��");
    else format(tpaState, sizeof(tpaState), "������Ҵ�����Ϣ:{FF0000}��");

    if(PlayerInfo[playerid][enableInvincible]) format(wdState, sizeof(wdState), "α�޵�״̬:{00FF00}��");
    else format(wdState, sizeof(wdState), "α�޵�״̬:{FF0000}��");

    if(PlayerInfo[playerid][AutoFix]) format(fixState, sizeof(fixState), "�Զ��޳�:{00FF00}��");
    else format(fixState, sizeof(fixState), "�Զ��޳�:{FF0000}��");

    if(!PlayerInfo[playerid][displayObject]) format(objShow, sizeof(objShow), "OBJ��ʾ:{FF0000}��");
    else format(objShow, sizeof(objShow), "OBJ��ʾ:{00FF00}��");

    if(PlayerInfo[playerid][AutoFlip]) format(autoflip, sizeof(autoflip), "��������:{00FF00}��");
    else format(autoflip, sizeof(autoflip), "��������:{FF0000}��");

    if(PlayerInfo[playerid][NoCrash]) format(pbpz, sizeof(pbpz), "������ײ:{00FF00}��");
    else format(pbpz, sizeof(pbpz), "������ײ:{FF0000}��");

    if(PlayerInfo[playerid][speedoMeter]) format(sdb, sizeof(sdb), "�ٶȱ�:{00FF00}��");
    else format(sdb, sizeof(sdb), "�ٶȱ�:{FF0000}��");

    if(PlayerInfo[playerid][netStats]) format(net, sizeof(net), "�������:{00FF00}��");
    else format(net, sizeof(net), "�������:{FF0000}��");

    if(PlayerInfo[playerid][showName]) format(showName_, sizeof(showName_), "�������:{00FF00}��");
    else format(showName_, sizeof(showName_), "�������:{FF0000}��");

    if(PlayerInfo[playerid][showStunt]) format(showStunt_, sizeof(showStunt_), "��Ч����:{00FF00}��");
    else format(showStunt_, sizeof(showStunt_), "��Ч����:{FF0000}��");

    format(time, sizeof(time), "�ҵ�ʱ��:%02d:%02d", PlayerInfo[playerid][tHour], PlayerInfo[playerid][tMinute]);
    format(tweather, sizeof(tweather), "�ҵ�����:%d", PlayerInfo[playerid][tWeather]);
    format(tmp, sizeof(tmp), "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s", tpaState, wdState, fixState, objShow, autoflip, pbpz, sdb, net, showName_, showStunt_, time, tweather);
    // format(tmp, sizeof(tmp), "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s",tpaState,?wdState,?fixState,?objShow,?autoflip,?pbpz,?sdb,?net,?showName_, showStunt_, time,?tweather);
    Dialog_Show(playerid, CustomSettings, DIALOG_STYLE_LIST, "���Ի�����", tmp, "ȷ��", "����");
}

Dialog:helpMessageBox(playerid, response, listitem, inputtext[]) {
    if(!response) {
        AntiCommand[playerid] = 0;
        CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/help");
        return 1;
        // OnPlayerCommandText(playerid, "/help");
    }
    new msg[128], idx;
    msg = strtok(inputtext, idx);
    if(!strcmp(msg, "LV1")) {
        Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "����ԱLV1", "����:/jail\n�߳�:/kick����:/goto\n����:/get\n�����༭/r edit", "ȷ��", "����");
        return 1;
    }
    if(!strcmp(msg, "LV2")) {
        Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "����ԱLV2", "(���)��ɱ:/(un)ban\n�����ؾ�:/jyzj", "ȷ��", "����");
        return 1;
    }
    if(!strcmp(msg, "LV3")) {
        Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "����ԱLV3", "ͼ���ɫ:/acolor(�ݲ�����)", "ȷ��", "����");
        return 1;
    }
    if(!strcmp(msg, "LV4")) {
        Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "����ԱLV4", "������Ǯ:/givecash\n/creategoods �����Ҿ�\n/delgoodsɾ���Ҿ� /board delete ɾ�������\n/attire ����װ�� /cars create ��������\n/selectnpc ����NPC /showname �رջ���ʾNPC����", "ȷ��", "����");
        return 1;
    }
    if(!strcmp(msg, "LV5")) {
        Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "����ԱLV5", "ˢ�·�����:/gmx\n����ϵͳ���͵�:/vsmake\n/houseedit �����༭ /attireװ�� /reset�����û�����", "ȷ��", "����");
        return 1;
    }
    if(!strcmp(msg, "LV?")) {
        Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "����ԱLV?", "�������GM:/admin\nȡ���������GM:/undmin", "ȷ��", "����");
        return 1;
    }
    if(!strcmp(msg, "�����ճ�����")) {
        Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "����ϵͳ", "/house goto �ɴ���������.\n/house buy �ɹ�����.\n/house sell �ɳ��۷���.\n/house text �ɸ��ļ��.\n/house list �ɲ鿴�����б�.\n/house glist �ɲ鿴����Ȩ���б�.\n/house adp �ɸ��跿�ݲ���Ȩ��.\n/house rdp ���Ƴ����ݲ���Ȩ��.\n/house gdp �ɲ鿴���ݲ���Ȩ��.\n/house sellto �ɳ��۸�ָ������.\n/house buyit �ɽ������˷�������������", "ȷ��", "����");
        return 1;
    }
    if(!strcmp(msg, "��������Ա����")) {
        SendClientMessage(playerid, Color_White, "[ϵͳ] ������/houseedit");
        return 1;
    }
    if(!strcmp(msg, "�Ҿ��ճ�����")) {
        AntiCommand[playerid] = 0;
        CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/goodshelp");
        // OnPlayerCommandText(playerid, "/goodshelp");
        return 1;
    }
    if(!strcmp(msg, "�Ҿ߹���Ա����")) {
        Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "�Ҿ߹���Աϵͳ", "/creategoods �����Ҿ�\n/delgoodsɾ���Ҿ�\n/resetgoods ���üҾ�", "ȷ��", "����");
        return 1;
    }
    if(!strcmp(msg, "��������")) {
        Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "����ϵͳ", "[����] ����:/r s (������),����/��ʼ����,��������ʡ��\n[����] ����:/r j,�������\n[����] ����:/r l,�뿪����\n[����] ����:/r page ҳ��,��ת������Ҫ��ҳ��\n[����] ����:/r create,��������\n[����] ����:/r edit,�༭��������\n����ϵͳ�з������³���/kill����", "ȷ��", "����");
        return 1;
    }
    if(!strcmp(msg, "�༭����")) {
        Dialog_Show(playerid, helpMessageBox, DIALOG_STYLE_LIST, "����ϵͳ", "�粻�ǹ���Ա��ֻ�ɱ༭�Լ�����������\n[����]����:/r edit [������] [�����༭����],�༭����\n[����]����:/r edit q �˳��༭ģʽ\n[����]����:/r edit cpsize,����/�鿴��ǰ�༭cp�ĳߴ�\n[����]����:/r edit d,�������\n[����]����:/r edit cp,�ڵ�ǰλ�÷���һ��cp��\n[����]����:/r edit trg,�鿴����˵��", "ȷ��", "����");
        return 1;
    }
    return 1;
}
Dialog:Dialog_Register(playerid, response, listitem, inputtext[]) {
    if(!response) return Kick(playerid);
    if(!strlen(inputtext)) {
        if(IsPlayerAndroid(playerid)) {
            Dialog_Show(playerid, Dialog_Register, DIALOG_STYLE_INPUT, "ע��", "���ʺŻ�û�б�{22DD22}ע��{FFFFFF},�������������{22DD22}ע��..", "ע��", "�˳�");
            return 1;
        }
        Dialog_Show(playerid, Dialog_Register, DIALOG_STYLE_PASSWORD, "ע��", "���ʺŻ�û�б�{22DD22}ע��{FFFFFF},�������������{22DD22}ע��..", "ע��", "�˳�");
        return 1;
    }
    if(strlen(inputtext) < 6 || strlen(inputtext) > 16) {
        if(IsPlayerAndroid(playerid)) {
            Dialog_Show(playerid, Dialog_Register, DIALOG_STYLE_INPUT, "ע��", "{FFFFFF}�������������{22DD22}ע��{FFFFFF},�������Ϊ{22DD22}6-16λ֮��!.", "ע��", "�˳�");
            return 1;
        }
        Dialog_Show(playerid, Dialog_Register, DIALOG_STYLE_PASSWORD, "ע��", "{FFFFFF}�������������{22DD22}ע��{FFFFFF},�������Ϊ{22DD22}6-16λ֮��!.", "ע��", "�˳�");
        return 1;
    }
    if(!IsValidPassword(inputtext)) {
        if(IsPlayerAndroid(playerid)) {
            Dialog_Show(playerid, Dialog_Register, DIALOG_STYLE_INPUT, "ע��", "���ʺŻ�û�б�ע��,�������������ע��.", "ע��", "�˳�");
            return 1;
        }
        Dialog_Show(playerid, Dialog_Register, DIALOG_STYLE_PASSWORD, "ע��", "���ʺŻ�û�б�ע��,�������������ע��.", "ע��", "�˳�");
        return 1;
    }
    OnPlayerRegister(playerid, inputtext);
    return 1;
}
Dialog:Dialog_Login(playerid, response, listitem, inputtext[]) {
    if(!response) {
        // ShowPlayerDialog(playerid, PlayerSafeCenter, DIALOG_STYLE_LIST, "��ȫ������", MailDialogContent, "ȷ��", "ȡ��");
        // return 1;
        // return Kick(playerid); //û��¼�Ļ�Ȼ�󵯳���Ұ�ȫ���� ����ֻ���һ���������
        if(PlayerInfo[playerid][yzbantime] != 0) {
            SendClientMessage(playerid, Color_Red, "[��ȫ]�����˺Ŵ�����ʱ��ֹ��֤״̬");
            return 1;
        }
        if(PlayerInfo[playerid][Yztime] != 0) {
            new str[128];
            format(str, sizeof(str), "[��ȫ]����%d������ٴη���", PlayerInfo[playerid][Yztime]);
            SendClientMessage(playerid, Color_Yellow, str);
            if(IsPlayerAndroid(playerid)) {
                Dialog_Show(playerid, Dialog_ForgetPass, DIALOG_STYLE_INPUT, "�һ�����", "�����������䷢����֤��\n���Լӵȴ�,�糤ʱ���޷��յ��ٳ������·���", "ȷ��", "����");
                return 1;
            }
            Dialog_Show(playerid, Dialog_ForgetPass, DIALOG_STYLE_PASSWORD, "�һ�����", "�����������䷢����֤��\n���Լӵȴ�,�糤ʱ���޷��յ��ٳ������·���", "ȷ��", "����");
            return 1;
        }
        if(PlayerInfo[playerid][Confirmed] == 1) {
            if(PlayerInfo[playerid][yzwrong] != 0) {
                new str[128];
                format(str, sizeof(str), "[��ȫ]�㻹��%d��������֤�����,�����������ʱ��ֹ��֤", 6 - PlayerInfo[playerid][yzwrong]);
                SendClientMessage(playerid, Color_Yellow, str);
            }
            new str[128], pMailTemp[MAX_EMAIL_LENGTH];
            format(pMailTemp, sizeof(pMailTemp), "%s", PlayerInfo[playerid][Email]);
            // ���������ַ
            new length_temp = strlen(pMailTemp) - 6;
            if(length_temp <= 6) length_temp = strlen(pMailTemp) - 3;
            for (new i = 6; i < length_temp; i++) {
                pMailTemp[i] = '*';
            }
            PlayerInfo[playerid][ConfCode] = random(89999) + 10000; //������֤�� 10000��99999
            new text[96];
            format(text, sizeof(text), "PNAME:%s#PADDRESS:%s#CONFCODE:%d", GetName(playerid), GetIP(playerid), PlayerInfo[playerid][ConfCode]);
            SendEmail(playerid, "Romantic until Death--��ȫ����", PlayerInfo[playerid][Email], "[Romantic until Death]��ȫ����--��ȷ����������", text, true, "confcode_RST.html");
            PlayerInfo[playerid][Yztime] = 90;

            format(str, sizeof(str), "���İ�ȫ����Ϊ%s\n���������˻�������֤��,�뼰ʱ����.", pMailTemp);
            if(IsPlayerAndroid(playerid)) {
                Dialog_Show(playerid, Dialog_ForgetPass, DIALOG_STYLE_INPUT, "�һ�����", str, "ȷ��", "����");
                return 1;
            }
            Dialog_Show(playerid, Dialog_ForgetPass, DIALOG_STYLE_PASSWORD, "�һ�����", str, "ȷ��", "����");
            return 1;
        }
        SendClientMessage(playerid, Color_Yellow, "[��ȫ]�����˺Ż�û�а�����,�ݲ�֧���һ�����");
        if(IsPlayerAndroid(playerid)) {
            Dialog_Show(playerid, Dialog_Login, DIALOG_STYLE_INPUT, "��¼", "{00FFFF}����˺���ע��\n{00FFFF}�������������������.", "��¼", "�һ�����");
            return 1;
        }
        Dialog_Show(playerid, Dialog_Login, DIALOG_STYLE_PASSWORD, "��¼", "{00FFFF}����˺���ע��\n{00FFFF}�������������������.", "��¼", "�һ�����");
        return 1;
    }
    if(!strlen(inputtext)) {
        if(IsPlayerAndroid(playerid)) {
            Dialog_Show(playerid, Dialog_Login, DIALOG_STYLE_INPUT, "��¼", "{00FFFF}����˺���ע��\n{00FFFF}�������������������.�㻹��{80FF80}3�λ���{00FFFF}��������", "��¼", "�һ�����");
            return 1;
        }
        Dialog_Show(playerid, Dialog_Login, DIALOG_STYLE_PASSWORD, "��¼", "{00FFFF}����˺���ע��\n{00FFFF}�������������������.�㻹��{80FF80}3�λ���{00FFFF}��������", "��¼", "�һ�����");
        return 1;
    }
    OnPlayerLogin(playerid, inputtext);
    return 1;
}
Dialog:weapons(playerid, response, listitem, inputtext[]) {
    if(GetPlayerCreditpoints(playerid) <= 90) return SendClientMessage(playerid, Color_Yellow, "[ϵͳ]������Ϸ�����ֹ���,�뽡����Ϸ,�����Զ�����");
    if(response == 1) {
        switch (listitem) {
            case 0:{
                GivePlayerWeapon(playerid, 4, 1);
                return 1;
            }
            case 1:{
                GivePlayerWeapon(playerid, 5, 1);
                return 1;
            }
            case 2:{
                GivePlayerWeapon(playerid, 22, 5000);
                return 1;
            }
            case 3:{
                GivePlayerWeapon(playerid, 23, 5000);
                return 1;
            }
            case 4:{
                GivePlayerWeapon(playerid, 24, 5000);
                return 1;
            }
            case 5:{
                GivePlayerWeapon(playerid, 25, 5000);
                return 1;
            }
            case 6:{
                GivePlayerWeapon(playerid, 26, 5000);
                return 1;
            }
            case 7:{
                GivePlayerWeapon(playerid, 27, 5000);
                return 1;
            }
            case 8:{
                GivePlayerWeapon(playerid, 32, 5000);
                return 1;
            }
            case 9:{
                GivePlayerWeapon(playerid, 28, 5000);
                return 1;
            }
            case 10:{
                GivePlayerWeapon(playerid, 29, 5000);
                return 1;
            }
            case 11:{
                GivePlayerWeapon(playerid, 30, 5000);
                return 1;
            }
            case 12:{
                GivePlayerWeapon(playerid, 31, 5000);
                return 1;
            }
            case 13:{
                GivePlayerWeapon(playerid, 33, 5000);
                return 1;
            }
            case 14:{
                GivePlayerWeapon(playerid, 34, 5000);
                return 1;
            }
            case 15:{
                GivePlayerWeapon(playerid, 35, 5000);
                return 1;
            }
            case 16:{
                GivePlayerWeapon(playerid, 36, 5000);
                return 1;
            }
            case 17:{
                GivePlayerWeapon(playerid, 37, 5000);
                return 1;
            }
            case 18:{
                GivePlayerWeapon(playerid, 16, 5000);
                return 1;
            }
            case 19:{
                GivePlayerWeapon(playerid, 17, 5000);
                return 1;
            }
            case 20:{
                GivePlayerWeapon(playerid, 18, 5000);
                return 1;
            }
            case 21:{
                GivePlayerWeapon(playerid, 39, 5000);
                GivePlayerWeapon(playerid, 40, 1);
                return 1;
            }
            case 22:{
                GivePlayerWeapon(playerid, 41, 5000);
                return 1;
            }
            case 23:{
                GivePlayerWeapon(playerid, 42, 5000);
                return 1;
            }
            case 24:{
                GivePlayerWeapon(playerid, 38, 5000);
                return 1;
            }
        }
    }
    return 1;
}
Dialog:MessageBox(playerid, response, listitem, inputtext[]) {
    return 1;
}
Dialog:Dialog_Tail(playerid, response, listitem, inputtext[]) {
    if(response) {
        if(PlayerInfo[playerid][Cash] < 3000) return SendClientMessage(playerid, Color_White, "[Сβ��]���Ľ�Ҳ���3000");
        if(strlen(inputtext) < 4 || strlen(inputtext) > 58) {
            SendClientMessage(playerid, Color_White, "[Сβ��]�����쳣,��ΧΪ2~16������,4~32�������ַ�");
            return cmd_motto(playerid, "");
        }
        if(strfind(inputtext, "\\", true) != -1 || strfind(inputtext, " ", true) != -1 || strfind(inputtext, "", true) != -1 || strfind(inputtext, "", true) != -1) {
            SendClientMessage(playerid, Color_White, "[Сβ��]����Ҫ��[],������ʹ�ÿո�,\\�ȷ���");
            return cmd_motto(playerid, "");
        }
        if(strfind(inputtext, "����", true) != -1 || strfind(inputtext, "GM", false) != -1 || strfind(inputtext, "admin", false) != -1) {
            SendClientMessage(playerid, Color_White, "[Сβ��]������ʹ�ù���Ա�����ǰ׺");
            return cmd_motto(playerid, "");
        }
        if(strcmp(inputtext, "null", false) == 0) {
            format(PlayerInfo[playerid][Tail], 33, "");
            SendClientMessage(playerid, Color_White, "[Сβ��]����ɹ�!");
            return 1;
        }
        new placeholder;
        for (new i = 0; i < sizeof InvalidWords; i++) //���δ��Զ���*
        {
            placeholder = strfind(inputtext, InvalidWords[i], true);
            if(placeholder != -1) return SendClientMessage(playerid, Color_White, "[Сβ��]���ڲ�����ʹ�õ�����");
        }
        format(PlayerInfo[playerid][Tail], 33, inputtext);
        GivePlayerCash(playerid, -3000);
        SendClientMessage(playerid, Color_White, "[Сβ��]�����ɹ�!");
    }
    return 1;
}
Dialog:Dialog_Designation(playerid, response, listitem, inputtext[]) {
    if(response) {
        if(PlayerInfo[playerid][Cash] < 3000) return SendClientMessage(playerid, Color_White, "[�ƺ�]���Ľ�Ҳ���3000");
        if(strfind(inputtext, "{", true) == 0 && strfind(inputtext, "}", true) == 7) //�ж��Ƿ���������ɫ �еĻ����Ȳ��ܳ���5������
        {
            if(strlen(inputtext) < 2 || strlen(inputtext) > 58) {
                SendClientMessage(playerid, Color_White, "[�ƺ�]�ƺų����쳣,��ΧΪ1~5������,2~10�������ַ�");
                return cmd_wdch(playerid, "");
            }
        } else {
            if(strlen(inputtext) < 2 || strlen(inputtext) > 58) {
                SendClientMessage(playerid, Color_White, "[�ƺ�]�ƺų����쳣,��ΧΪ1~5������,2~10�������ַ�");
                return cmd_wdch(playerid, "");
            }
        }
        if(strfind(inputtext, "\\", true) != -1 || strfind(inputtext, " ", true) != -1 || strfind(inputtext, "", true) != -1 || strfind(inputtext, "", true) != -1) {
            SendClientMessage(playerid, Color_White, "[�ƺ�]�ƺŲ���Ҫ��[],������ʹ�ÿո�,\\�ȷ���");
            return cmd_wdch(playerid, "");
        }
        if(strfind(inputtext, "����", true) != -1 || strfind(inputtext, "GM", false) != -1 || strfind(inputtext, "admin", false) != -1) {
            SendClientMessage(playerid, Color_White, "[�ƺ�]������ʹ�ù���Ա�����ǰ׺");
            return cmd_wdch(playerid, "");
        }
        if(strcmp(inputtext, "null", false) == 0) {
            format(PlayerInfo[playerid][Designation], 19, "");
            SendClientMessage(playerid, Color_White, "[�ƺ�]����ɹ�!");
            return 1;
        }
        new placeholder;
        for (new i = 0; i < sizeof InvalidWords; i++) //���δ��Զ���*
        {
            placeholder = strfind(inputtext, InvalidWords[i], true);
            if(placeholder != -1) return SendClientMessage(playerid, Color_White, "[�ƺ�]�ƺ��д��ڲ�����ʹ�õ�����");
        }
        format(PlayerInfo[playerid][Designation], 19, inputtext);
        GivePlayerCash(playerid, -3000);
        SendClientMessage(playerid, Color_White, "[�ƺ�]�����ƺųɹ�!");
    }
    return 1;
}
Dialog:Dialog_SpawnVehicle(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        switch (listitem) {
            case 0:{
                ShowModelSelectionMenu(playerid, chaopaolist, "->Cars<-");
            }
            case 1:{
                ShowModelSelectionMenu(playerid, jingchelist, "->Cars<-");
            }
            case 2:{
                ShowModelSelectionMenu(playerid, planelist, "->Planes<-");
            }
            case 3:{
                ShowModelSelectionMenu(playerid, Motorolalist, "->Motorolas<-");
            }
            case 4:{
                ShowModelSelectionMenu(playerid, Shiplist, "->Ships<-");
            }
            case 5:{
                ShowModelSelectionMenu(playerid, yueyelist, "->Cars<-");
            }
            case 6:{
                ShowModelSelectionMenu(playerid, tuoche, "->Cars<-");
            }
            case 7:{
                ShowModelSelectionMenu(playerid, huoche, "->Cars<-");
            }
            case 8:{
                ShowModelSelectionMenu(playerid, trainlist, "->Trains<-");
            }
            case 9:{
                ShowModelSelectionMenu(playerid, minzhen, "->Cars<-");
            }
            case 10:{
                ShowModelSelectionMenu(playerid, Otherlist, "->Others<-");
            }
        }
    }
    return 1;
}
Dialog:ClickPlayer(playerid, response, listitem, inputtext[]) {
    if(!response) return 1;
    new idx, tmp[128];
    tmp = strtok(inputtext, idx);
    if(strcmp(tmp, "�������") == 0) {
        // �оٱ���ʱ�� ������ ��¼ ���¼��ŵ�ʱ��
        // Ȼ����һҳ����һҳ
        Race_ShowRecentlyRacing(playerid, SelectRecentlyRacePage[playerid], SelectRecentlyClicked[playerid]);
        return 1;
    }
    if(strcmp(tmp, "�Ŷ�", false) == 0) {
        if(SelectRecentlyClicked[playerid] == playerid) return cmd_wdtd(playerid, "");
        new string[32];
        format(string, sizeof(string), "invite %d", SelectRecentlyClicked[playerid]);
        cmd_t(playerid, string);
        return 1;
    }
    if(strcmp(tmp, "�����͵�TA���", false) == 0) {
        new string[5];
        format(string, sizeof(string), "%d", SelectRecentlyClicked[playerid]);
        cmd_tpa(playerid, string);
        return 1;
    }
    return 1;
}

function OnLoginTimeout(playerid) {
    PlayerInfo[playerid][LoginTimer] = -1;
    if(!PlayerInfo[playerid][Login]) {
        Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "��¼", "�㱻����˷�������ԭ��:��ʱδ��¼!", "ȷ��", "");
        DelayedKick(playerid);
    }
    return 1;
}

stock DelayedKick(const playerid, const time = 500) {
    SetTimerEx_("_KickPlayerDelayed", time, time, 1, "d", playerid);
    return 1;
}

Dialog:PlayerInfo_ChangeName(playerid, response, listitem, inputtext[]) {
    if(response) {
        if(PlayerInfo[playerid][Cash] < 6000) return SendClientMessage(playerid, Color_White, "[ϵͳ]�����˻�����6000,���㹻���ڸ���");
        if(strlen(inputtext) < 4 || strlen(inputtext) > 24) return SendClientMessage(playerid, Color_White, "[ϵͳ]�û������ȴ���");
        new placeholder;
        for (new i = 0; i < sizeof InvalidWords; i++) //���δ��Զ���*
        {
            placeholder = strfind(inputtext, InvalidWords[i], true);
            if(placeholder != -1) {
                SendClientMessage(playerid, Color_White, "[ϵͳ]����ʹ�÷Ƿ��û���");
                return 1;
            }
        }
        if(strfind(inputtext, "{", true) != -1 || strfind(inputtext, "}", true) != -1) {
            return SendClientMessage(playerid, Color_White, "[ϵͳ]�û������ɴ���{}");
        }
        new flag = false;
        for (new i = 0; i < strlen(inputtext); i++) {
            if((inputtext[i] >= 48 && inputtext[i] <= 57) || (inputtext[i] >= 65 && inputtext[i] <= 125)) flag = true;
            else flag = false;
        }
        if(!flag) return SendClientMessage(playerid, Color_White, "[ϵͳ]�û�����[0-9a-Z_]���");
        // ֻҪ�ж��Ƿ��������ģ�û�о�ֱ�Ӹ���UID UPDATE�û����Ϳ�����
        if(AccountCheck(inputtext)) return SendClientMessage(playerid, Color_White, "[ϵͳ]����û����Ѿ�����ʹ����Ŷ");
        for (new i = 0; i < MAX_SELL; i++) {
            if(mk_strcmp(p_Sell[i][sell_player], GetName(playerid)) == 0) ChangeSellPlayer(i, inputtext); // ��������û�з��� �еĻ���������ȫת����
        }


        if(NowRaceId != 0) {
            for (new i = 0; i < NowRaceId; i++) {
                if(Race[i][rauthor] == PlayerInfo[playerid][ID])
                    format(Race[i][rauthorName], MAX_PLAYER_NAME, "%s", GetName(playerid));
            }
        }


        for (new i = 0; i < loadcount; i++) { // ��������û�мҾ� �еĻ��Ҿ�ת��
            if(GOODS[i][GoodOwner] == PlayerInfo[playerid][ID]) {
                format(GOODS[i][GoodOwnerName], MAX_PLAYER_NAME, inputtext);
                format(GOODS[i][GoodName], 125, "%s ����Ʒ", inputtext);
                UpdateGoods3dtextlabel(i);
                SaveGoods(i);
            }
        }
        new query[128];
        mysql_format(g_Sql, query, sizeof(query), "UPDATE `users` SET `Name` = '%e' WHERE `ID` = %d", inputtext, PlayerInfo[playerid][ID]);
        mysql_pquery(g_Sql, query);
        GivePlayerCash(playerid, -6000);
        SetPlayerName(playerid, inputtext);
        SendClientMessage(playerid, Color_White, "[ϵͳ]�����û����ɹ�");
    }
    return 1;
}
Dialog:PlayerInfoDialog(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        switch (listitem) {
            case 0:{ //�ҵ�����
                // if(!strcmp(msg, "�ҵ�����")) {
                // if(PlayerInfo[playerid][yzwrong] >= 6) {
                //     new msgs[512];
                //     new t = gettime() + 86400;
                //     mysql_format(g_Sql, msgs, sizeof(msgs), "UPDATE `users` SET `YzBanTime` = %d where `Name` = '%e'", t, GetName(playerid));
                //     mysql_pquery(g_Sql, msgs);
                //     Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "ϵͳ", "����֤��������,��������������!", "ȷ��", "");
                //     return 1;
                // } else {
                Dialog_Show(playerid, PlayerSafeCenter, DIALOG_STYLE_LIST, "��ȫ������", MailDialogContent, "ȷ��", "ȡ��");
                // }
                return 1;
                // break;
            }
            case 1:{ //װ��
                ShowPlayerAttireDialog(playerid);
                return 1;
            }
            case 2:{ //�Ҿ�
                AntiCommand[playerid] = 0;
                cmd_mygoods(playerid, "");
                return 1;
            }
            case 3:{
                pViewMyCar(playerid);
                return 1;
            }
            case 4:{
                ChangePlayerColor(playerid, PlayerColorPage[playerid]);
                return 1;
            }
            case 5:{
                cmd_wdch(playerid, "");
                return 1;
            }
            case 6:{
                cmd_motto(playerid, "");
                return 1;
            }
            case 7:{
                ShowCustomSettings(playerid);
                return 1;
            }
        }
        return 1;
    }
    return 1;
}
Dialog:PlayerSafeCenter(playerid, response, listitem, inputtext[]) {
    if(!response) {
        if(!PlayerInfo[playerid][Login]) {
            // ShowPlayerDialog(playerid, PlayerSafeCenter, DIALOG_STYLE_LIST, "��ȫ������", MailDialogContent, "ȷ��", "ȡ��");
            Kick(playerid);
            return 1;
        }
        AntiCommand[playerid] = 0;
        CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/sz");
        return 1;
    }
    switch (listitem) {
        case 0:{
            Dialog_Show(playerid, PlayerSafeCenter, DIALOG_STYLE_LIST, "��ȫ������", MailDialogContent, "ȷ��", "ȡ��");
            return 1;
        }
        case 1:{
            // ���·���
            if(PlayerInfo[playerid][yzbantime] != 0) {
                SendClientMessage(playerid, Color_Red, "[��ȫ]�����˺Ŵ�����ʱ��ֹ��֤״̬");
                return 1;
            }
            if(PlayerInfo[playerid][Yztime] != 0) {
                new str[128];
                format(str, sizeof(str), "[��ȫ]����%d����Է���", PlayerInfo[playerid][Yztime]);
                SendClientMessage(playerid, Color_Yellow, str);
                Dialog_Show(playerid, PlayerSafeCenter, DIALOG_STYLE_LIST, "��ȫ������", MailDialogContent, "ȷ��", "ȡ��");
                return 1;
            }
            if(pMailConfirmedCuts(playerid)) return 1;
            if(strcmp(PlayerInfo[playerid][Email], "-1") == 0 || strcmp(PlayerInfo[playerid][Email], "null", true) == 0) {
                Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "ϵͳ", "��ʱ��֧����֤/����������������", "ȷ��", "");
                return 1;
            }
            if(PlayerInfo[playerid][Confirmed] == 1) {
                Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "ϵͳ", "�����������䱣��,���˺������������������һ�!", "ȷ��", "");
                return 1;
            }
            format(PlayerInfo[playerid][Email], MAX_EMAIL_LENGTH, "%s", inputtext); //��������
            PlayerInfo[playerid][ConfCode] = random(89999) + 10000; //������֤�� 10000��99999
            // PlayerInfo[playerid][Confirmed] = 0; // ���δȷ��״̬
            PlayerInfo[playerid][Yztime] = 60;

            new text[96];
            format(text, sizeof(text), "PNAME:%s#PADDRESS:%s#CONFCODE:%d", GetName(playerid), GetIP(playerid), PlayerInfo[playerid][ConfCode]);
            // formats the string with the code standard that SAMPMailJS accepts. tag1:value1#tag2:value2

            SendEmail(playerid, "Romantic until Death--��ȫ����", PlayerInfo[playerid][Email], "[RaceSpeedTime]��ȫ����--��ȷ����������", text, true, "confcode_RST.html");
            Dialog_Show(playerid, PlayerSafeCenter, DIALOG_STYLE_LIST, "��ȫ������", "\n��������(��Ҫ60���)\n������֤��\n�޸�����\n�޸��û���\n", "ȷ��", "ȡ��");
            // if(responded != 200) 
            // {
            //     format(text, sizeof(text), "[����]��ȫ����������֤ϵͳ����ʧ��,�������[%d]", responded); 
            //     format(PlayerInfo[playerid][Email], MAX_EMAIL_LENGTH, "null"); //��������
            //     SendClientMessage(playerid, Color_Red, text);
            //     return 1;
            // }
            // SendClientMessage(playerid, Color_Red, "[��ȫ]��֤���Ѿ��ɹ����͵��������,��鿴���������֤");
            // new Query[128];
            // mysql_format(g_Sql, Query, sizeof(Query), "SELECT * FROM `users` WHERE `name` = '%e'", GetName(playerid));
            // mysql_pquery(g_Sql, Query, "OnEmailResendQuery", "ds", playerid, inputtext);
            return 1;
        }
        case 2:{
            // ��������
            if(PlayerInfo[playerid][yzbantime] != 0) {
                SendClientMessage(playerid, Color_Red, "[��ȫ]�����˺Ŵ�����ʱ��ֹ��֤״̬");
                return 1;
            }
            if(PlayerInfo[playerid][Confirmed] == 1) {
                new str[128];
                format(str, sizeof(str), "�����������䱣��,���˺������������������һ�!\n��������Ϊ%s �ݲ�֧���޸�����", PlayerInfo[playerid][Email]);
                Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "ϵͳ", str, "ȷ��", "");
                return 1;
            }
            if(PlayerInfo[playerid][Yztime]) Dialog_Show(playerid, PlayerSafeCenter, DIALOG_STYLE_LIST, "��ȫ������", MailDialogContent, "ȷ��", "ȡ��");
            else Dialog_Show(playerid, PlayerEmailSet, DIALOG_STYLE_INPUT, "��ȫ������", "{FFFFFF}������:{33CCCC}�������", "ȷ��", "ȡ��");
            // new Query[128];
            // mysql_format(g_Sql, Query, sizeof(Query), "SELECT * FROM `users` WHERE `name` = '%e'", GetName(playerid));
            // mysql_pquery(g_Sql, Query, "OnEmailSettingQuery", "d", playerid);
            return 1;
        }
        case 3:{
            if(PlayerInfo[playerid][yzbantime] != 0) {
                SendClientMessage(playerid, Color_Red, "[��ȫ]�����˺Ŵ�����ʱ��ֹ��֤״̬");
                return 1;
            }
            // ������֤��
            if(strcmp(PlayerInfo[playerid][Email], "-1") == 0 || strcmp(PlayerInfo[playerid][Email], "null", true) == 0) {
                Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "ϵͳ", "��ʱ��֧����֤/����������������", "ȷ��", "");
                return 1;
            }
            if(PlayerInfo[playerid][Confirmed] == 1) {
                Dialog_Show(playerid, PlayerPassChangeCAPTCHA, DIALOG_STYLE_LIST, "��ȫ������", "\n��ѡ�����������\n������֤��", "ȷ��", "ȡ��");
                return 1;
            }
            Dialog_Show(playerid, EmailCAPTCHA, DIALOG_STYLE_INPUT, "��ȫ������", "{FFFFFF}������:{33CCCC}��֤��", "ȷ��", "ȡ��");
            return 1;
        }
        case 4:{
            // �޸�����
            if(PlayerInfo[playerid][yzwrong] >= 3) {
                SCM(playerid, Color_White, "[ϵͳ]�벻Ҫ��ʱ�����ظ���������!");
                return 1;
            }
            if(PlayerInfo[playerid][Yztime] != 0) {
                SCM(playerid, Color_White, "[ϵͳ]�벻Ҫ��ʱ�����ظ���������!");
                return 1;
            }
            if(strcmp(PlayerInfo[playerid][Email], "-1") == 0 || strcmp(PlayerInfo[playerid][Email], "null", true) == 0) {
                SCM(playerid, Color_White, "[ϵͳ]����δ���ð�ȫ����,�������������Է����һ�");
                Dialog_Show(playerid, PlayerPassWordChange, DIALOG_STYLE_INPUT, "{FFFF00}��ȫϵͳ", "{9AFF9A}��ӭ!\n����������������������޸�!\n���μ������˺�����!\n�Ѳ���ɢ�м��������赣������й¶", "ȷ��", "ȡ��");
                return 1;
            }
            if(PlayerInfo[playerid][yzbantime] != 0) {
                SendClientMessage(playerid, Color_Red, "[��ȫ]�����˺Ŵ�����ʱ��ֹ��֤״̬");
                return 1;
            }
            PlayerInfo[playerid][ConfCode] = random(89999) + 10000; //������֤�� 10000��99999
            PlayerInfo[playerid][Yztime] = 60;

            new text[64];
            format(text, sizeof(text), "PNAME:%s#PADDRESS:%s#CONFCODE:%d", GetName(playerid), GetIP(playerid), PlayerInfo[playerid][ConfCode]);

            SendEmail(playerid, "Romantic until Death--��ȫ����", PlayerInfo[playerid][Email], "[RaceSpeedTime]��ȫ����", text, true, "confcode_RST.html");
            // Dialog_Show(playerid, PlayerSafeCenter, DIALOG_STYLE_LIST, "��ȫ������", "\n��������(��Ҫ60���)\n������֤��\n�޸�����\n�޸��û���\n", "ȷ��", "ȡ��");
            // if(responded != 200) 
            // {
            //     format(text, sizeof(text), "[����]��ȫ����������֤ϵͳ����ʧ��,�������[%d]", responded); 
            //     format(PlayerInfo[playerid][Email], MAX_EMAIL_LENGTH, "null"); //��������
            //     SendClientMessage(playerid, Color_Red, text);
            //     return 1;
            // }
            // SendClientMessage(playerid, Color_Red, "[��ȫ]��֤���Ѿ��ɹ����͵��������,��鿴���������֤");
            Dialog_Show(playerid, PlayerPassChangeCAPTCHA, DIALOG_STYLE_INPUT, "��ȫ������", "\n��ѡ�����������\n������֤��", "ȷ��", "ȡ��");
            // new query[128];
            // mysql_format(g_Sql, query, sizeof(query), "SELECT * FROM `users` WHERE `name` = '%e'", GetName(playerid));
            // mysql_pquery(g_Sql, query, "OnPlayerChangePassQuery", "d", playerid);
            return 1;
        }
        case 5:{
            // �޸��û���
            if(!PlayerInfo[playerid][Login]) return Dialog_Show(playerid, PlayerSafeCenter, DIALOG_STYLE_LIST, "��ȫ������", MailDialogContent, "ȷ��", "ȡ��");
            Dialog_Show(playerid, PlayerInfo_ChangeName, DIALOG_STYLE_INPUT, "{FFFF00}��ȫϵͳ", "��������Ҫ�޸ĳɵ��û���\n����ʹ�������������дʵ��ַ�,��ʧ������˺���ʧ���Լ�����\nUID������,�޸�һ���û�����6000���", "ȷ��", "ȡ��");
            return 1;
        }
    }
    return 1;
}
Dialog:PlayerInfo_Weather(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        AntiCommand[playerid] = 0;
        new str[16];
        format(str, sizeof(str), "/tianqi %d", strval(inputtext));
        AntiCommand[playerid] = 0;
        OnPlayerCommandText(playerid, str);
        AntiCommand[playerid] = 0;
        CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/sz");
    }
    return 1;
}
Dialog:PlayerInfo_Time(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        new str[32], temp[32], thour[128], tminute[128], hour, minute, idx;
        format(temp, 32, "%s", inputtext);
        thour = strtok(temp, idx);
        hour = strval(thour);
        tminute = strtok(temp, idx);
        minute = strval(tminute);
        if(hour < 0 || hour > 24) return SCM(playerid, Color_LightBlue, "[ʱ��] /time ʱ �� СʱΪ0~24,��Ϊ0~59");
        if(minute < 0 || minute > 59) return SCM(playerid, Color_LightBlue, "[ʱ��] /time ʱ �� СʱΪ0~24,��Ϊ0~59");
        PlayerInfo[playerid][tHour] = hour;
        PlayerInfo[playerid][tMinute] = minute;
        format(str, sizeof(str), "/time %d %d", PlayerInfo[playerid][tHour], PlayerInfo[playerid][tMinute]);
        AntiCommand[playerid] = 0;
        OnPlayerCommandText(playerid, str);
        AntiCommand[playerid] = 0;
        CallRemoteFunction("OnPlayerCommandText", "is", playerid, "/sz");
    }
    // }
    // else {
    //     SetPlayerTime(playerid, hour, minute);
    // }
    // new time = strtok(inputtext, ":");
    return 1;
}
Dialog:PlayerEmailSet(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        if(strfind(inputtext, "@", true) == -1) {
            Dialog_Show(playerid, PlayerEmailSet, DIALOG_STYLE_INPUT, "��ȫ������", "{FFFF00}������������˺Ÿ�ʽ����\n{33CCCC}��������������������", "ȷ��", "ȡ��");
            return 1;
        }
        if(strlen(inputtext) < 5 || strlen(inputtext) > MAX_EMAIL_LENGTH) {
            Dialog_Show(playerid, PlayerEmailSet, DIALOG_STYLE_INPUT, "��ȫ������", "{FFFF00}������������˺ų��ȴ���,֧��5~32λ\n��������������������", "ȷ��", "ȡ��");
            return 1;
        }
        if(IsAlreadyEmailed(inputtext)) {
            Dialog_Show(playerid, PlayerEmailSet, DIALOG_STYLE_INPUT, "��ȫ������", "������������Ѿ���ע����\n��������������������", "ȷ��", "ȡ��");
            return 1;
        }
        format(PlayerInfo[playerid][Email], MAX_EMAIL_LENGTH, "%s", inputtext); //��������
        PlayerInfo[playerid][ConfCode] = random(89999) + 10000; //������֤�� 10000��99999
        PlayerInfo[playerid][Confirmed] = 0; // ���δȷ��״̬
        PlayerInfo[playerid][Yztime] = 60;

        new text[96];
        format(text, sizeof(text), "PNAME:%s#PADDRESS:%s#CONFCODE:%d", GetName(playerid), GetIP(playerid), PlayerInfo[playerid][ConfCode]);
        // formats the string with the code standard that SAMPMailJS accepts. tag1:value1#tag2:value2

        SendEmail(playerid, "Romantic until Death--��ȫ����", PlayerInfo[playerid][Email], "[Romantic until Death]��ȫ����--��ȷ����������", text, true, "confcode_RST.html");
        Dialog_Show(playerid, PlayerSafeCenter, DIALOG_STYLE_LIST, "��ȫ������", "\n��������(��Ҫ60���)\n������֤��\n�޸�����\n�޸��û���\n", "ȷ��", "ȡ��");
        // if(responded != 200) 
        // {
        //     format(text, sizeof(text), "[����]��ȫ����������֤ϵͳ����ʧ��,�������[%d]", responded); 
        //     format(PlayerInfo[playerid][Email], MAX_EMAIL_LENGTH, "null"); //��������
        //     SendClientMessage(playerid, Color_Red, text);
        //     return 1;
        // }
        // SendClientMessage(playerid, Color_Red, "[��ȫ]��֤���Ѿ��ɹ����͵��������,��鿴���������֤");
        // new query[128];
        // mysql_format(g_Sql, query, sizeof(query), "SELECT `email` FROM `users` WHERE `email` = '%e'", inputtext);
        // mysql_pquery(g_Sql, query, "UpdateEmailQuery", "ds", playerid, inputtext);
        return 1;
    }
    Dialog_Show(playerid, PlayerSafeCenter, DIALOG_STYLE_LIST, "��ȫ������", MailDialogContent, "ȷ��", "ȡ��");
    return 1;
}
Dialog:EmailCAPTCHA(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        // new query[128];
        // mysql_format(g_Sql, query, sizeof(query), "SELECT * FROM `users` WHERE `name` = '%e'", GetName(playerid));
        // mysql_pquery(g_Sql, query, "", "ds", playerid,inputtext);
        // return true;
        // printf("%s %s", PlayerInfo[playerid][ConfCode], inputtext);
        if(pMailConfirmedCuts(playerid)) return 1;
        if(PlayerInfo[playerid][ConfCode] == strval(inputtext)) {
            new query[256];
            mysql_format(g_Sql, query, sizeof(query), "UPDATE `users` SET `yz` = 1,`email` ='%e' WHERE `name` = '%e'", PlayerInfo[playerid][Email], GetName(playerid));
            mysql_pquery(g_Sql, query);
            SendClientMessage(playerid, Color_White, "[ϵͳ]��ϲ��,������֤�ɹ�!");
            // format(PlayerInfo[playerid][Email], MAX_EMAIL_LENGTH, "-1");
            PlayerInfo[playerid][yzwrong] = 0;
            PlayerInfo[playerid][Confirmed] = 1;
            return true;
        }
        Dialog_Show(playerid, EmailCAPTCHA, DIALOG_STYLE_INPUT, "������֤ϵͳ", "��֤���������\n������������������֤������֤", "ȷ��", "ȡ��");
        PlayerInfo[playerid][yzwrong]++;
        pMailConfirmedCuts(playerid, 1);
        return true;
    }
    Dialog_Show(playerid, PlayerSafeCenter, DIALOG_STYLE_LIST, "��ȫ������", MailDialogContent, "ȷ��", "ȡ��");
    return true;
}

Dialog:PlayerPassWordChange(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        if(!strlen(inputtext)) return Dialog_Show(playerid, PlayerPassWordChange, DIALOG_STYLE_INPUT, "{FFFF00}��ȫ����", "{9AFF9A}��ӭ!\n����������������������޸�!\n���μ������˺�����!\n�Ѳ���ɢ�м��������赣������й¶", "ȷ��", "ȡ��");
        if(strlen(inputtext) < 6 || strlen(inputtext) > 16) return Dialog_Show(playerid, PlayerPassWordChange, DIALOG_STYLE_INPUT, "{FFFF00}������֤ϵͳ", "{FFFFFF}��֤�ɹ�!������6-16λ��{FFFF00}�����룡", "ȷ��", "ȡ��");
        // new query[256];
        // mysql_format(g_Sql, query, sizeof(query), "UPDATE `users` SET `yz` = 1,`email` ='%e' WHERE `name` = '%e'", PlayerInfo[playerid][Email], GetName(playerid));
        // mysql_format(g_Sql, query, sizeof(query), "UPDATE `users` SET `email` ='%e' WHERE `name` = '%e'", PlayerInfo[playerid][Email], GetName(playerid));
        // mysql_pquery(g_Sql, query);
        OnPlayerReloadRegister(playerid, inputtext);
    }
    Dialog_Show(playerid, PlayerSafeCenter, DIALOG_STYLE_LIST, "��ȫ������", MailDialogContent, "ȷ��", "ȡ��");
    return 1;
}

Dialog:PlayerPassChangeCAPTCHA(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        // new query[128];
        // mysql_format(g_Sql, query, sizeof(query), "SELECT * FROM `users` WHERE `name` = '%e'", GetName(playerid));
        // mysql_pquery(g_Sql, query, "On_CAPTCHA_PChange_Query", "ds", playerid, inputtext);
        if(pMailConfirmedCuts(playerid)) return 1;
        if(PlayerInfo[playerid][ConfCode] == strval(inputtext)) {
            Dialog_Show(playerid, PlayerPassWordChange, DIALOG_STYLE_INPUT, "{FFFF00}������֤ϵͳ", "{FFFFFF}��֤�ɹ�!������{FFFF00}�����룡", "ȷ��", "ȡ��");
            PlayerInfo[playerid][yzwrong] = 0;
            // PlayerInfo[playerid][Confirmed] = 1;
            return 1;
        }
        Dialog_Show(playerid, PlayerPassChangeCAPTCHA, DIALOG_STYLE_INPUT, "{FFFF00}������֤ϵͳ", "��֤���������\n������������������֤������֤", "ȷ��", "ȡ��");
        PlayerInfo[playerid][yzwrong]++;
        pMailConfirmedCuts(playerid, 1);
        return 1;
    }
    Dialog_Show(playerid, PlayerSafeCenter, DIALOG_STYLE_LIST, "��ȫ������", MailDialogContent, "ȷ��", "ȡ��");
    return 1;
}

// function OnEmailResendQuery(playerid, inputtext[])
// {
//     if(cache_num_rows()) {
//         new yz,string[128];
//         cache_get_value_name_int(0, "yz", yz);
//         if(yz == 1) 
//         {
//             Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "ϵͳ", "�����������䱣��,���˺������������������һ�!", "ȷ��", "");
//             return 1;
//         }
//         format(string, sizeof(string), "127.0.0.1/email.php?name=%s&email=%s", GetName(playerid), inputtext);
//         HTTP(playerid, HTTP_GET, string, "", "MyHttpResponseEX");
//         mysql_format(g_Sql, string, sizeof(string), "UPDATE `users` SET `email` = '%e' WHERE `name` = '%e'", inputtext, GetName(playerid));
//         mysql_pquery(g_Sql, string);
//         format(PlayerInfo[playerid][Email], MAX_EMAIL_LENGTH, "%s", inputtext);
//         PlayerInfo[playerid][Yztime] = 60;
//         PlayerInfo[playerid][yzwrong]++;
//         Dialog_Show(playerid, PlayerSafeCenter, DIALOG_STYLE_LIST, "��ȫ������", "\n��ѡ�����������\n���·���(��Ҫ60���)\n��������\n������֤��\n�޸�����\n�޸��û���", "ȷ��", "ȡ��");
//         SendClientMessage(playerid, Color_White, "ϵͳ:����ղ�����֤�볢�Ը�����������,����Ƶ�������ᱻ��ֹ��֤1��!");
//     }
//     // //���else��д�� ��Ϊ��Ĭ����ע���ʱ��Ų������� ���ڴ˹���֮ǰ�Ѿ�ע�����û�����ݵ� ������Ҫ����һ������ 
//     // // 2020.3.29�� ����������Ƶ�
//     // else 
//     // {
//     //     new Query2[256];
//     //     format(Query2, 256, "INSERT INTO `players` (`name`,`code`,`email`,`yz`) VALUES('%s',0,0,0)", GetName(playerid));
//     //     mysql_query(Query2);
//     //     mysql_free_result();
//     // }
//     return true;
// }
// function OnEmailSettingQuery(playerid)
// {
//     if(cache_num_rows() != 0) {
//         new yz;
//         cache_get_value_name_int(0, "yz", yz);
//         if(yz == 1) 
//         {
//             Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "ϵͳ", "�����������䱣��,���˺������������������һ�!", "ȷ��", "");
//             return true;
//         }
//     }
//     if(PlayerInfo[playerid][Yztime]) Dialog_Show(playerid, PlayerSafeCenter, DIALOG_STYLE_LIST, "��ȫ������", MailDialogContent, "ȷ��", "ȡ��");
//     else Dialog_Show(playerid, PlayerEmailSet, DIALOG_STYLE_INPUT, "��ȫ������", "{FFFFFF}������:{33CCCC}�������", "ȷ��", "ȡ��");
//     return true;
// }
// function OnPlayerChangePassQuery(playerid)
// {
//     if(cache_num_rows() != 0) {
//         new yz,string[128];
//         cache_get_value_name_int(0, "yz", yz);
//         if(yz == 1) 
//         {
//             cache_get_value_name(0, "email", PlayerInfo[playerid][Email], MAX_EMAIL_LENGTH);
//             format(string, sizeof(string), "127.0.0.1/email.php?name=%s&email=%s", GetName(playerid), PlayerInfo[playerid][Email]);
//             HTTP(playerid, HTTP_GET, string, "", "MyHttpResponseEX");

//             mysql_format(g_Sql, string, sizeof(string), "UPDATE `users` SET `email` = '%e' WHERE `name` = '%e'", PlayerInfo[playerid][Email], GetName(playerid));
//             mysql_pquery(g_Sql, string);

//             PlayerInfo[playerid][Yztime] = 60;
//             PlayerInfo[playerid][Confirmed] = 0;
//             format(PlayerInfo[playerid][Email], MAX_EMAIL_LENGTH, "%s", PlayerInfo[playerid][Email]);
//             Dialog_Show(playerid, PlayerPassChangeCAPTCHA, DIALOG_STYLE_LIST, "��ȫ������", "\n��ѡ�����������\n������֤��", "ȷ��", "ȡ��");
//             return true;
//         }
//         // Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "ϵͳ", "�㻹û�������䱣��!��������!", "ȷ��", "");
//         // 2020.2.28 ȡ������ û���ñ�������һ��֧���޸�����
//         SCM(playerid, Color_White, "[ϵͳ]����δ���ð�ȫ����,�������������Է����һ�");
//         Dialog_Show(playerid, PlayerPassWordChange, DIALOG_STYLE_INPUT, "{FFFF00}��ȫϵͳ", "{9AFF9A}��ӭ!\n����������������������޸�!\n���μ������˺�����!\n�Ѳ���ɢ�м��������赣������й¶", "ȷ��", "ȡ��");
//         return true;
//     }
//     return false;
// }
// function UpdateEmailQuery(playerid, inputtext[])
// {
//     // if(cache_num_rows()) 
//     // {
//     //     Dialog_Show(playerid, PlayerEmailSet, DIALOG_STYLE_INPUT, "��ȫ������", "{FFFF00}������������˺�{FF0000}�ѱ�ע��{FFFF00}\n{33CCCC}�������������������������֤", "ȷ��", "ȡ��");
//     //     return 1;
//     // }
//     // format(string, sizeof(string), "127.0.0.1/email.php?name=%s&email=%s", GetName(playerid), inputtext);
//     // HTTP(playerid, HTTP_GET, string, "", "MyHttpResponseEX");


//     // new string[144];
//     // mysql_format(g_Sql, string, sizeof(string), "UPDATE `users` SET `email` = '%e' WHERE `name` = '%e'", inputtext, GetName(playerid));
//     // mysql_pquery(g_Sql, string);

//     format(PlayerInfo[playerid][Email], MAX_EMAIL_LENGTH, "%s", inputtext); //��������
//     PlayerInfo[playerid][ConfCode] = random(89999) + 10000; //������֤�� 10000��99999
//     PlayerInfo[playerid][Confirmed] = 0; // ���δȷ��״̬
//     PlayerInfo[playerid][Yztime] = 60;

//     new text[64];
// 	format(text, sizeof(text), "PNAME:%s#CONFCODE:%d", GetName(playerid), PlayerInfo[playerid][ConfCode]); 
//     // formats the string with the code standard that SAMPMailJS accepts. tag1:value1#tag2:value2

// 	new responded = SendEmail("Romantic until Death--��ȫ����", PlayerInfo[playerid][Email], "[Romantic until Death]��ȫ����--��ȷ����������", text, true, "confcode_RST.html");
//     Dialog_Show(playerid, PlayerSafeCenter, DIALOG_STYLE_LIST, "��ȫ������", "\n��ѡ�����������\n���·���(��Ҫ60���)\n��������\n������֤��\n�޸�����\n�޸��û���", "ȷ��", "ȡ��");
//     if(responded != 200) 
//     {
// 	    format(text, sizeof(text), "[����]��ȫ����������֤ϵͳ����ʧ��,�������[%d]", responded); 
//         format(PlayerInfo[playerid][Email], MAX_EMAIL_LENGTH, "-1"); //��������
//         SendClientMessage(playerid, Color_Red, text);
//         return 1;
//     }
//     SendClientMessage(playerid, Color_Red, "[��ȫ]��֤���Ѿ��ɹ����͵��������,��鿴���������֤");
//     return 1;
// }

// function On_CAPTCHA_Query(playerid, inputtext[])
// {
//     if(cache_num_rows())
//     {
//         cache_get_value_name(0, "code", PlayerInfo[playerid][ConfCode]);
//         if(strcmp(PlayerInfo[playerid][ConfCode], inputtext, true) == 0) {
//             new query[256];
//             mysql_format(g_Sql, query, sizeof(query), "UPDATE `users` SET `yz` = 1,`email` ='%e' WHERE `name` = '%e'", PlayerInfo[playerid][Email], GetName(playerid));
//             mysql_pquery(g_Sql, query);
//             SendClientMessage(playerid, Color_White, "[ϵͳ]��ϲ��,������֤�ɹ�!");
//             format(PlayerInfo[playerid][Email], MAX_EMAIL_LENGTH, "-1");
//             PlayerInfo[playerid][yzwrong] = 0;
//             PlayerInfo[playerid][Confirmed] = 0;
//             return true;
//         } 
//         Dialog_Show(playerid, EmailCAPTCHA, DIALOG_STYLE_INPUT, "������֤ϵͳ", "��֤���������\n������������������֤������֤", "ȷ��", "ȡ��");
//         PlayerInfo[playerid][yzwrong]++;
//         return true;
//     }
//     return true;
// }

// function On_CAPTCHA_PChange_Query(playerid, inputtext[])
// {
//     if(cache_num_rows()) 
//     {
//         cache_get_value_name(0, "code", PlayerInfo[playerid][ConfCode], MAX_PCODE_LENGTH);
//         if(strcmp(PlayerInfo[playerid][ConfCode], inputtext, true) == 0) {
//             Dialog_Show(playerid, PlayerPassWordChange, DIALOG_STYLE_INPUT, "{FFFF00}������֤ϵͳ", "{FFFFFF}��֤�ɹ�!������{FFFF00}�����룡", "ȷ��", "ȡ��");
//             PlayerInfo[playerid][yzwrong] = 0;
//             PlayerInfo[playerid][Confirmed] = 1;
//             return 1;
//         }
//         Dialog_Show(playerid, PlayerPassChangeCAPTCHA, DIALOG_STYLE_INPUT, "{FFFF00}������֤ϵͳ", "��֤���������\n������������������֤������֤", "ȷ��", "ȡ��");
//         PlayerInfo[playerid][yzwrong]++;
//         return 1;
//     }    
//     return true;
// }

stock CheckPlayerEmailEd(playerid) {
    // ��Щ�����䱣��  
    // printf("%s", );
    if(strcmp(PlayerInfo[playerid][Email], "-1") == 0 || strcmp(PlayerInfo[playerid][Email], "null", true) == 0) {
        SCM(playerid, Color_White, "[ϵͳ]�㻹δ�������䱣��,���˺������������������һ�!ʹ��/sz���Դ򿪽���");
        return 1;
    }
    return 1;
    // new Query[128];
    // mysql_format(g_Sql, Query, sizeof(Query), "SELECT * FROM `users` WHERE `name` = '%e'", GetName(playerid));
    // mysql_pquery(g_Sql, Query, "IsPlayerEmailed", "d", playerid);
}

// function IsPlayerEmailed(playerid)
// {
//     if(cache_num_rows() != 0) {
//         new yz;
//         cache_get_value_name_int(0, "yz", yz);
//         if(yz == 1) SCM(playerid, Color_White, "[ϵͳ]�����������䱣��,���˺������������������һ�!ʹ��/sz���Դ򿪽���");
//         else SCM(playerid, Color_White, "[ϵͳ]�㻹δ�������䱣��,���˺������������������һ�!ʹ��/sz���Դ򿪽���");
//     }
//     return true;
// }

function OnPlayerDataLoaded(playerid, race_check) {
    /*	race condition check:
        player A connects -> SELECT query is fired -> this query takes very long
        while the query is still processing, player A with playerid 2 disconnects
        player B joins now with playerid 2 -> our laggy SELECT query is finally finished, but for the wrong player

        what do we do against it?
        we create a connection count for each playerid and increase it everytime the playerid connects or disconnects
        we also pass the current value of the connection count to our OnPlayerDataLoaded callback
        then we check if current connection count is the same as connection count we passed to the callback
        if yes, everything is okay, if not, we just kick the player
    */
    if(race_check != g_MysqlRaceCheck[playerid]) return Kick(playerid);
    // RegLoginTDrawCreate(playerid);
    // ���䰲׿�� �������check_android filterscript
    if(IsPlayerAndroid(playerid)) {
        SendClientMessage(playerid, Color_Yellow, "[������]��⵽���������ƶ������,����SAMP�汾Ϊ1.08�汾,���ܳ������˵Ȳ��������⡣");
        SendClientMessage(playerid, Color_Yellow, "[������]OBJ/TextDraw/GameText/�Ի����޷���ʾ����ʾ��ȫΪ��������");
        SendClientMessage(playerid, Color_White, ServerVersion);
    } else {
        for (new i = 0; i <= 7; i++) {
            TextDrawShowForPlayer(playerid, Screen[i]);
        }
    }
    // �ж�����Ƿ��ڼ���ģʽ�� �Ƿ������汾0.3.7
    if(IsPlayerCompat(playerid)) {
        new string[128];
        GetPlayerVersion(playerid, string, sizeof(string));
        format(string, sizeof(string), "[������]����SA-MP�汾Ϊ:%s, �������ѳ��Լ��ݣ������ܴ�����Ϸ���ȶ������⡣", string);
        SendClientMessage(playerid, Color_Yellow, string);
    }
    // �������֣�����ӽ�
    LoginMusicCamera(playerid);
    new string[128];
    new year, month, day, hour, minute, second;
    if(cache_num_rows() > 0) {
        // we store the password and the salt so we can compare the password the player inputs
        // and save the rest so we won't have to execute another query later
        cache_get_value(0, "Password", PlayerInfo[playerid][Password], 65);
        cache_get_value(0, "Salt", PlayerInfo[playerid][Salt], 12);

        if(!strcmp(PlayerInfo[playerid][Salt], "null", true)) {
            Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "ϵͳ", "{FFFF00}�����˺�������ԭ�������������(��������ID,�˺Ŷ�ʧ)����ȫ����Դ˽��ж���\n����ⶳ�뱾�˻������ϵ����,�����½�!", "ȷ��", "");
            DelayedKick(playerid);
            return 1;
        }

        cache_get_value_name(0, "email", PlayerInfo[playerid][Email]); //��ȡ����
        cache_get_value_name_int(0, "yz", PlayerInfo[playerid][Confirmed]); //��ȡ�Ƿ���֤
        cache_get_value_name_int(0, "Yzwrong", PlayerInfo[playerid][yzwrong]);
        cache_get_value_name_int(0, "YzBanTime", PlayerInfo[playerid][yzbantime]);
        // if(PlayerInfo[playerid][yzbantime] - gettime() < 21600000) { //������֤ʱ������⣬���һ�쳬����6����֤�ͻ��24Сʱ����֤
        if(PlayerInfo[playerid][yzbantime] != 0) {
            if(PlayerInfo[playerid][yzbantime] - gettime() < 0) { //������֤ʱ������⣬���һ�쳬����6����֤�ͻ��24Сʱ����֤
                PlayerInfo[playerid][yzwrong] = 0;
                PlayerInfo[playerid][yzbantime] = 0;
            } else {
                TimestampToDate(PlayerInfo[playerid][yzbantime], year, month, day, hour, minute, second, 8);
                format(string, sizeof(string), "[��ȫ]����֤�����������,��һ����֤����� %d-%d-%d %02d:%02d:%02d", year, month, day, hour, minute, second);
                SendClientMessage(playerid, Color_Yellow, string);
            }
        }
        new last_login_timestamp;
        cache_get_value_name_int(0, "LastLogin", last_login_timestamp);
		cache_get_value_name(0, "LastIP", PlayerInfo[playerid][LastIP], 32);
        TimestampToDate(last_login_timestamp, year, month, day, hour, minute, second, 8);
        // saves the active cache in the memory and returns an cache-id to access it for later use
        PlayerInfo[playerid][Cache_ID] = cache_save();
        // �Զ���¼ - ��ʼ
        new gLogin_IP[32];
    	GetPlayerIp(playerid, gLogin_IP, sizeof(gLogin_IP)); // ��ȡ��¼IP
    	
		printf("���ID%d�ĵ�¼IP:[%s] - �ϴ�IP:[%s]", playerid, gLogin_IP, PlayerInfo[playerid][LastIP]);
    	if(!strcmp(gLogin_IP, PlayerInfo[playerid][LastIP], false))
    	{
        	if(!IsPlayerAndroid(playerid)) {
            	for (new i = 0; i <= 7; i++) {
                	TextDrawHideForPlayer(playerid, Screen[i]);
            	}
        	}
        	for (new i = GetPlayerPoolSize(); i >= 0; i--) {
            	ShowPlayerNameTagForPlayer(playerid, i, true);
        	}
        	
        	cache_set_active(PlayerInfo[playerid][Cache_ID]);
        	new Checked = AssignPlayerData(playerid);
        	cache_delete(PlayerInfo[playerid][Cache_ID]);
        	PlayerInfo[playerid][Cache_ID] = MYSQL_INVALID_CACHE;
        	KillTimer(PlayerInfo[playerid][LoginTimer]);
        	PlayerInfo[playerid][LoginTimer] = -1;

        	if(Checked) {
            	PlayerInfo[playerid][Login] = true;
            	printf("[���]%s(%d) �ѵ�¼.", GetName(playerid), playerid);
            	new auto_string[128];
            	new ver[24];
                GetPlayerVersion(playerid, ver, sizeof(ver));
            	format(auto_string, sizeof(auto_string), "{FFFFFF}[��������ʾ]:��ӭ���%s (%d) ��ҽ����˷�����,{15BFEA}[:%s]{FFFFFF} ", GetName(playerid), playerid, ver);
            	SCMALL(Color_LightBlue, auto_string);
            	SetPlayerColor(playerid, PlayerColors[random(200)]); //�������С��ͼ��ɫ
            	// SetSpawnInfo(playerid, NO_TEAM, 0, Player[playerid][X_Pos], Player[playerid][Y_Pos], Player[playerid][Z_Pos], Player[playerid][A_Pos], 0, 0, 0, 0, 0, 0);
            	new rand = random(sizeof BirthPointInfo);
            	SetSpawnInfo(playerid, NO_TEAM, PlayerInfo[playerid][Skin], BirthPointInfo[rand][0], BirthPointInfo[rand][1], BirthPointInfo[rand][2], BirthPointInfo[rand][3], 0, 0, 0, 0, 0, 0);
            	SpawnPlayer(playerid); //����ҳ���
            	PlayerPlaySound(playerid, 1186, 0.0, 0.0, 0.0); //ֹͣ��������
            	new temp[512];
            	format(temp, sizeof(temp), "�Զ���¼�ɹ�\n����������ddos����\n���������������κξ���\n��лsamp�����ǵ����һֱ������\n��������һֱ����ȥ\n");
            	Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "Server INFO", temp, "info", "");
            	CheckPlayerEmailEd(playerid);
        	}
        	return 1;
    	}
    	GetPlayerIp(playerid, PlayerInfo[playerid][LastIP], 32); //��ȡIP
        // �Զ���¼ - ����     
        format(string, sizeof(string), "��ӭ,�����˺���ע��\n�����·����������¼\n���һ������ʱ�� %d-%d-%d %02d:%02d:%02d", year, month, day, hour, minute, second);
        if(IsPlayerAndroid(playerid)) {
            Dialog_Show(playerid, Dialog_Login, DIALOG_STYLE_INPUT, "Romantic until Death", string, "��¼", "�һ�����");
        } else {
            Dialog_Show(playerid, Dialog_Login, DIALOG_STYLE_PASSWORD, "Romantic until Death", string, "��¼", "�һ�����");
        }
        // �����ڿ�ʼ������� 30 ���ʱ���¼
        PlayerInfo[playerid][LoginTimer] = SetTimerEx_("OnLoginTimeout", 300 * 1000, 300 * 1000, 1, "d", playerid);
        return 1;
    }
    // ���ע���˵Ļ�
    if(IsPlayerAndroid(playerid)) {
        Dialog_Show(playerid, Dialog_Register, DIALOG_STYLE_INPUT, "Romantic until Death", "�����·��������������ע��!\n���μ������˺�����!\n�Ѳ���ɢ�м��������赣������й¶", "ע��", "�˳�");
    } else {
        Dialog_Show(playerid, Dialog_Register, DIALOG_STYLE_PASSWORD, "Romantic until Death", "�����·��������������ע��!\n���μ������˺�����!\n�Ѳ���ɢ�м��������赣������й¶", "ע��", "�˳�");
    }
    return 1;
}



UpdatePlayerData(playerid, reason) {
    if(PlayerInfo[playerid][Login] == false) return 0;

    // if the client crashed, it's not possible to get the player's position in OnPlayerDisconnect callback
    // so we will use the last saved position (in case of a player who registered and crashed/kicked, the position will be the default spawn point)
    // if(reason == 1)
    // {
    // 	GetPlayerPos(playerid, Player[playerid][X_Pos], Player[playerid][Y_Pos], Player[playerid][Z_Pos]);
    // 	GetPlayerFacingAngle(playerid, Player[playerid][A_Pos]);
    // }

    // new query[145];
    // mysql_format(g_SQL, query, sizeof query, "UPDATE `players` SET `x` = %f, `y` = %f, `z` = %f, `angle` = %f, `interior` = %d WHERE `id` = %d LIMIT 1", Player[playerid][X_Pos], Player[playerid][Y_Pos], Player[playerid][Z_Pos], Player[playerid][A_Pos], GetPlayerInterior(playerid), Player[playerid][ID]);
    // mysql_tquery(g_SQL, query);

    // ж���ٶȱ�
    UnLoadVelo(playerid);
    // ж�����������
    for (new i = 0; i <= 10; i++) {
        PlayerTextDrawDestroy(playerid, PlayerText:network_txtdraw[playerid][i]);
    }
    new query[512], saveinfo[512];
    strins(saveinfo, "UPDATE `users` SET `AdminLevel`=%d,`Skin`=%d,`Score`=%d,`Cash`=%d,`JailSeconds`=%d,`Yzwrong`=%d,`YzBanTime`=%d,`LastLogin`='%d',`Designation`='%s',`Tail`='%s' WHERE `Name` = '%e'", strlen(saveinfo));
    // format(msg, sizeof(msg), "", PlayerInfo[playerid][AdminLevel], PlayerInfo[playerid][Skin], PlayerInfo[playerid][Score], PlayerInfo[playerid][Cash], PlayerInfo[playerid][JailSeconds], PlayerInfo[playerid][yzwrong], PlayerInfo[playerid][yzbantime], string, GetName(playerid));
    mysql_format(g_Sql, query, sizeof(query), saveinfo, PlayerInfo[playerid][AdminLevel], PlayerInfo[playerid][Skin], \
        PlayerInfo[playerid][Score], PlayerInfo[playerid][Cash], PlayerInfo[playerid][JailSeconds], PlayerInfo[playerid][yzwrong], \
        PlayerInfo[playerid][yzbantime], gettime(), PlayerInfo[playerid][Designation], PlayerInfo[playerid][Tail], GetName(playerid));
    mysql_pquery(g_Sql, query);

    // �����������
    mysql_format(g_Sql, query, sizeof(query), "UPDATE `users` SET `tWeather`= %d,`tHour`= %d,`tMinute`= %d,`NoCrash`= %d,`AutoFlip`= %d, `LastIP` = '%s' WHERE `Name` = '%e'", \
        PlayerInfo[playerid][tWeather], PlayerInfo[playerid][tHour], PlayerInfo[playerid][tMinute], PlayerInfo[playerid][NoCrash], \
        PlayerInfo[playerid][AutoFlip], PlayerInfo[playerid][LastIP], GetName(playerid));
    mysql_pquery(g_Sql, query);
    mysql_format(g_Sql, query, sizeof query, "UPDATE `users` SET `displayObject`= %d,`speedoMeter`= %d,`AutoFix`= %d,`enableInvincible`= %d, `netStats`=%d WHERE `Name` = '%e'", \
        PlayerInfo[playerid][displayObject], PlayerInfo[playerid][speedoMeter], PlayerInfo[playerid][AutoFix], \
        PlayerInfo[playerid][enableInvincible], PlayerInfo[playerid][netStats], GetName(playerid));
    mysql_pquery(g_Sql, query);

    // ����ֵ
    mysql_format(g_Sql, query, sizeof query, "UPDATE `users` SET `exp`= %d, `pLevel` = %d where `Name` = '%e'", PlayerInfo[playerid][exp], PlayerInfo[playerid][plevel], GetName(playerid));
    mysql_pquery(g_Sql, query);

    HouseSellPlayerInitialize(playerid); //��֪�� ������PHouse���ʼ��sellto��buyit��
    if(dinfobj[playerid] == 1) {
        DestroyDynamicObject(jd[playerid]); //���پ���
        DestroyDynamicObject(wy[playerid]); //����β��
        dinfobj[playerid] = 0; //2020.1.12 �����޸�infobj�������ߺ�û�������һ����ҵ�����
    }
    if(PlayerInfo[playerid][CreateCar] == 1) {
        DestroyVehicle(PlayerInfo[playerid][BuyID]);
        PlayerInfo[playerid][CreateCar] = 0;
    }
    //TpCheck[playerid] = 0;
    if(p_ppcCar[playerid]) DestroyVehicle(p_ppcCar[playerid]);
    PlayerInfo[playerid][BuyID] = 0;
    PlayerInfo[playerid][CarLock] = 0;
    PlayerInfo[playerid][lastZpos] = 0; //��һ���Z������ ���ڷ��ؾ������쳣����
    PlayerInfo[playerid][lastVehSpeed] = 0; //��һ��ĳ��� ���ڷ��ؾ������쳣����
    Initialize_tpa(playerid);
    tdSpeedo_Disconnect(playerid);
    Race_OnPlayerDisconnect(playerid); //������ߺ�����ϵͳ����
    // PHouse_OnPlayerDisconnect(playerid); //������ߺ���ϵͳ����
    Boards_OnPlayerDisconnect(playerid); //������ߺ󹫸��ƵĴ���
    DeathMatch_OnPlayerDisconnect(playerid); //DM���ߺ�Ĵ���
    Camera_OnPlayerDisConnect(playerid); //��������ߺ�Ĵ���
    Gps_OnPlayerDisConnect(playerid); //GPS���ߺ�Ĵ���
    if(PlayerInfo[playerid][yssyjsq] != -1) KillTimer(PlayerInfo[playerid][yssyjsq]);
    // DestroyDynamic3DTextLabel(NoDieTime[playerid]); //ɾ���޵�ʱ���3D����
    // TextDrawDestroy(ReSpawningText[playerid]);
    // Delete3DTextLabel(NoDieTime[playerid]);
    for (new i = GetPlayerPoolSize(); i >= 0; i--) { //������������ߣ������ڹۿ�������ô��Ӧ�Ĺۿ�������Ӧ�ùر�tv
        if(IsPlayerConnected(i)) {
            if(PlayerInfo[i][tvid] == playerid && i != playerid) {
                // for (new a = 0; a <= 21; a++) {
                //     TextDrawHideForPlayer(i, velo[PlayerInfo[i][tvid]][a]);
                // }
                // CallRemoteFunction("ActTogglePlayerSpectating", "ii", i, 0);
                TogglePlayerSpectating(i, false);
                PlayerInfo[i][tvzt] = false;
                PlayerInfo[i][tvid] = i;
                SetPlayerVirtualWorld(i, 0);
                SendClientMessage(i, Color_Orange, "[TV]:�Է������ˣ�ȡ��TV.");
            }
        }
    }


    //�Ҿ����߱���
    //We gonna check if player exit in pickup goods mode
    //Otherwise the obj would set to pos 0,0,99999 =w=
    if(GOODS_STATUS[playerid] == true) {
        //if is yes,we reset the pos
        //SetDynamicObjectPos(GOODS[GOODS_OPRATEID[playerid]][OrderId],GOODS[GOODS_OPRATEID[playerid]][GoodX],GOODS[GOODS_OPRATEID[playerid]][GoodY],GOODS[GOODS_OPRATEID[playerid]][GoodZ]);


        //	ResetGoods(playerid,GOODS_OPRATEID[playerid]);
        GetPlayerPos(playerid, GOODS[GOODS_OPRATEID[playerid]][GoodX], GOODS[GOODS_OPRATEID[playerid]][GoodY], GOODS[GOODS_OPRATEID[playerid]][GoodZ]);
        CreateGoods(GOODS_OPRATEID[playerid]);

        SaveGoods(GOODS_OPRATEID[playerid]);
        GOODS_STATUS[playerid] = false;
        TAKEDOWN_STATUS[playerid] = false;
        printf("RESTORE GOODS [ORDERID]:%s during player exit", GOODS[GOODS_OPRATEID[playerid]][OrderId]);
    }
    new Reasons[][] = {
        "(����)",
        "(�����˳�)",
        "(Kick/Ban)"
    };
    // printf("[���]%s(%d)�뿪�˷�����,ԭ��:[%s].", GetName(playerid), playerid, Reasons[reason]);
    new string[128];
    format(string, sizeof(string), "[ϵͳ]:%s (%d) �뿪�˷����� (%s) ^^^", GetName(playerid), playerid, Reasons[reason]);
    SCMALL(Color_LightBlue, string);
    // SendDeathMessage(playerid, playerid, 201);
    SendDeathMessage(INVALID_PLAYER_ID, playerid, 201);
    return 1;
}
// ��ʼ����ұ�
stock SetupPlayerTable() {
    mysql_pquery(g_Sql, "CREATE TABLE IF NOT EXISTS `users`  (\
    `ID` int(10) UNSIGNED NOT NULL AUTO_INCREMENT,\
    `Name` varchar(24) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,\
    `Password` char(64) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,\
    `Salt` char(11) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,\
    `AdminLevel` int(11) NOT NULL DEFAULT 0,\
    `Skin` int(11) NOT NULL DEFAULT 0,\
    `Score` int(11) NOT NULL DEFAULT 0,\
    `Cash` int(11) NOT NULL DEFAULT 0,\
    `JailSeconds` int(11) NOT NULL DEFAULT 0,\
    `BanTime` int(11) NOT NULL DEFAULT 0,\
    `BanReason` int(11) NOT NULL DEFAULT 0,\
    `Yzwrong` int(11) NULL DEFAULT 0,\
    `YzBanTime` int(11) NULL DEFAULT 0,\
    `RegDate` varchar(33) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,\
    `LastLogin` varchar(33) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,\
    `Designation` varchar(19) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,\
    `Tail` varchar(33) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,\
    `tHour` int(11) NOT NULL DEFAULT 12,\
    `tMinute` int(11) NOT NULL DEFAULT 0,\
    `NoCrash` int(11) NOT NULL DEFAULT 1,\
    `AutoFlip` int(11) NOT NULL DEFAULT 1,\
    `displayObject` int(11) NOT NULL DEFAULT 1,\
    `speedoMeter` int(11) NOT NULL DEFAULT 1,\
    `AutoFix` int(11) NOT NULL DEFAULT 1,\
    `enableInvincible` int(11) NOT NULL DEFAULT 1,\
    `tWeather` int(11) NOT NULL DEFAULT 10,\
    `netStats` int(11) NOT NULL DEFAULT 1,\
    `exp` int(11) NULL DEFAULT NULL,\
    `pLevel` int(11) NULL DEFAULT 1,\
    `email` varchar(48) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,\
    `yz` varchar(2) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT '0',\
    `bantotal` int(11) NOT NULL DEFAULT 0,\
    PRIMARY KEY (`ID`) USING BTREE,\
    UNIQUE INDEX `index_email`(`email`) USING BTREE,\
    UNIQUE INDEX `index_name`(`Name`) USING BTREE\
    ) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;");
    return 1;
}



// static stock ReSetAllPlayerPass() {
//     // ��Ϊ���ݿ�Salt��ģ�������������BUG������ֻ��ʱʹ��

//     new cuts, Cache:result = mysql_query(g_Sql, "SELECT * FROM `users`");
//     cuts = cache_num_rows();
//     for (new i = 0; i < cuts; i++) {
//         new pID;
//         cache_get_value_name_int(i, "ID", pID);
//         new temp[65], pSalt[12]; // ɢ�м�������ɢ����
//         for (new j = 0; j < 11; j++) {
//             pSalt[j] = random(25) + 97;
//         }
//         SHA256_PassHash("123456", pSalt, temp, 65); //�涨65�̶�
//         new query[256];
//         mysql_format(g_Sql, query, sizeof(query), "UPDATE `users` SET `Password` = '%e',`Salt` = '%e' WHERE `ID` = %d", temp, pSalt, pID);
//         mysql_pquery(g_Sql, query);
//     }
//     cache_delete(result);
//     printf("�������");
// }


// �����ϳ��ľɰ�������֤
// function MyHttpResponseEX(index, response_code, sData[]) {
//     new buffer[128];
//     new h, m, s, y, day, d;
//     new msg[128];
//     if(response_code == 200) {
//         gettime(h, m, s);
//         getdate(y, day, d);
//         format(msg, sizeof(msg), "[��ȫ]��֤���Ѿ��ɹ����͵��������,��鿴���������֤[%d.%d.%d][%d:%d:%d]", y, day, d, h, m, s);
//         // format(msg, sizeof(msg), "�ʼ����ͳɹ�[%d.%d.%d][%d:%d:%d]", y, day, d, h, m, s);
//         SendClientMessage(index, Color_White, msg);
//         return 1;
//     } 
//     format(buffer, sizeof(buffer), "[��ȫ]�ʼ�����ʧ��,�������[%d]", response_code);
//     SendClientMessage(index, Color_White, buffer);
//     return 1;
// }

stock testmail() {
    // name:�����ڵ����ʼ��Աߵ�����(�ַ���)
    // to:����Ҫ�����ʼ��ĵ����ʼ���ַ(�ַ���)
    // subject:���ʼ�������(�ַ���)
    // isTemplate:������Ϊfalseʱ������ֻ�������ڲ����ı���������ı���������Ϊ��ʱ����������ʹ��ģ�塣(����)
    // templateName:������NodeJS�ű���ͬ�ļ����е�ģ����ļ�����(�ַ���)

    // name:The name that will appear next to your email (string)
    // to:The email address you want to send the email to (string)
    // subject:The subject of your email (string)
    // isTemplate:When set to false, it will only send the text you input in the argument text. When set to true, it will try to use a template. (boolean)
    // templateName:The file name of the template that must be in the same folder of your NodeJS script. (string)

    // ���������ڴ˴������ģ�������һЩ����������������ӱ�������ڱ�����ΪPLAYERNAME��PADDRESS��DREG��
    // ��Щ�����ڵ�λ�ã����磬������Ҫӵ����ҵ����ƣ���ҵ�IP��ע�����ڡ�Ϊ�ˣ���text�����У����ǽ���ű����ͽű���������Ԥ�����ʽ��
    // �ű��������ĸ�ʽΪtag1��value1��tag2��value2��tag3��value3��(��ֵ��)
    // ����ζ������ת����ģ�壬����ģ��tag1�������滻Ϊvalue1��tag2�������滻Ϊvalue2�ȡ�
    // ����������£�������PLAYERNAME��PADDRESS��DREG��ǩ��Ҫ�滻���ǣ�����ִ��PLAYERNAME��value��PADDRESS��value��DREG��value��PAWNʾ����
    // ���������滻��ҳ��ģ���ֵ PLAYERNAME���Զ�������ֵ�����ֵ
    // paddress������ҵ�IP��ַ
    // Dreg��������
    // CONFCODE��֤��
    // ֻ��һ���滻�����η�����


    // new string[128], pName[MAX_PLAYER_NAME], IP[32];
    // GetPlayerName(playerid, pName, sizeof pName);
    // GetPlayerIp(playerid, IP, sizeof IP);
    // format(string, sizeof string, "PLAYERNAME:%s#PADDRESS:%s#DREG:07/06/2018", pName, IP);
    // SendEmail("SAMP MailJS Teste", "omeuemail@gmail.com", "Bem Vindo ao servidor", string, true);

    // ����ֵ 200�ɹ� 400���� 403 û��Ȩ�� 404δ�ҵ� 
    // SAMPMAILJS_RESPONSE_OK 200
    // SAMPMAILJS_RESPONSE_FORBIDEN 403
    // SAMPMAILJS_RESPONSE_ERROR 400
    // SAMPMAILJS_RESPONSE_NOTFOUND 404

    // UTF8������PAWNO�������»�����,��SAMPMAILJS�õ�Ҳ��UTF8
    // SAMPMAILJS����֧��gbk����
    // ͨ��iconv-lite֧����gbk
    // SendEmail("�û���", "aipai1040859075@163.com", "����", "����", false);
    return 1;
}

// ������֤ϵͳ
// https://forum.sa-mp.com/showthread.php?t=654783
// https://forum.sa-mp.com/showthread.php?t=655060

stock pMailConfirmedCuts(const playerid, const update = 0) {
    new Query[128];
    mysql_format(g_Sql, Query, sizeof(Query), "UPDATE `users` SET `Yzwrong` = %d WHERE `Name` = '%e'", PlayerInfo[playerid][yzwrong], GetName(playerid));
    mysql_pquery(g_Sql, Query);
    if(PlayerInfo[playerid][yzwrong] >= 6) {
        if(update) {
            new t = gettime() + 86400;
            mysql_format(g_Sql, Query, sizeof(Query), "UPDATE `users` SET `YzBanTime` = %d, `Yzwrong` = 6 WHERE `Name` = '%e'", t, GetName(playerid));
            mysql_pquery(g_Sql, Query);
        }
        Dialog_Show(playerid, MessageBox, DIALOG_STYLE_MSGBOX, "ϵͳ", "����֤��������,��������������!", "ȷ��", "");
        return true;
    }
    return false;
}

Dialog:Dialog_ForgetPass(playerid, response, listitem, inputtext[]) {
    if(response) {
        if(PlayerInfo[playerid][ConfCode] == strval(inputtext)) {
            Dialog_Show(playerid, pForgetPassWordChange, DIALOG_STYLE_INPUT, "{FFFF00}������֤ϵͳ", "{FFFFFF}��֤�ɹ�!������{FFFF00}�����룡", "ȷ��", "ȡ��");
            PlayerInfo[playerid][yzwrong] = 0;
            return 1;
        }
        if(IsPlayerAndroid(playerid)) {
            Dialog_Show(playerid, Dialog_ForgetPass, DIALOG_STYLE_INPUT, "�һ�����", "��֤�����!����ϸУ���Ƿ���ȷ!�����������䷢����֤��", "ȷ��", "����");
        } else {
            Dialog_Show(playerid, Dialog_ForgetPass, DIALOG_STYLE_PASSWORD, "�һ�����", "��֤�����!����ϸУ���Ƿ���ȷ!�����������䷢����֤��", "ȷ��", "����");
        }
        PlayerInfo[playerid][yzwrong]++;
        pMailConfirmedCuts(playerid, 1);
        return 1;
    }
    if(IsPlayerAndroid(playerid)) {
        Dialog_Show(playerid, Dialog_Login, DIALOG_STYLE_INPUT, "��¼", "{00FFFF}����˺���ע��\n{00FFFF}�������������������.", "��¼", "�һ�����");
    } else {
        Dialog_Show(playerid, Dialog_Login, DIALOG_STYLE_PASSWORD, "��¼", "{00FFFF}����˺���ע��\n{00FFFF}�������������������.", "��¼", "�һ�����");
    }
    return 1;
}


Dialog:pForgetPassWordChange(playerid, response, listitem, inputtext[]) {
    if(response == 1) {
        if(!strlen(inputtext)) return Dialog_Show(playerid, PlayerPassWordChange, DIALOG_STYLE_INPUT, "{FFFF00}��ȫ����", "{9AFF9A}��ӭ!\n����������������������޸�!\n���μ������˺�����!\n�Ѳ���ɢ�м��������赣������й¶", "ȷ��", "ȡ��");
        if(strlen(inputtext) < 6 || strlen(inputtext) > 16) return Dialog_Show(playerid, PlayerPassWordChange, DIALOG_STYLE_INPUT, "{FFFF00}������֤ϵͳ", "{FFFFFF}��֤�ɹ�!������6-16λ��{FFFF00}�����룡", "ȷ��", "ȡ��");
        // new query[256];
        // mysql_format(g_Sql, query, sizeof(query), "UPDATE `users` SET `yz` = 1,`email` ='%e' WHERE `name` = '%e'", PlayerInfo[playerid][Email], GetName(playerid));
        // mysql_format(g_Sql, query, sizeof(query), "UPDATE `users` SET `email` ='%e' WHERE `name` = '%e'", PlayerInfo[playerid][Email], GetName(playerid));
        // mysql_pquery(g_Sql, query);
        OnPlayerReloadRegister(playerid, inputtext, 0);
        // 2021.2.15д��return ���������ȥ��
        return 1;
    }
    if(IsPlayerAndroid(playerid)) {
        Dialog_Show(playerid, Dialog_Login, DIALOG_STYLE_INPUT, "��¼", "{00FFFF}����˺���ע��\n{00FFFF}�������������������.", "��¼", "�һ�����");
    } else {
        Dialog_Show(playerid, Dialog_Login, DIALOG_STYLE_PASSWORD, "��¼", "{00FFFF}����˺���ע��\n{00FFFF}�������������������.", "��¼", "�һ�����");
    }

    return 1;
}



// ��ʼ������Ƿ�׿���ƶ���
stock InitPlayerAndroid(const playerid) {
    // Ĭ���ǰ�׿����
    PlayerInfo[playerid][Android] = true;
    PlayerInfo[playerid][DelayLogin] = 1;
    // ע�⣬�ú�����������fixes ����ֻ����filterscript��ʹ�ã�
    SendClientCheck(playerid, 0x48, 0, 0, 2);
    // �ӳٵ�¼����
    PlayerInfo[playerid][DelayLogin] = SetTimerEx_("DelayLoading", 0, 0, 1, "d", playerid);
    return 1;
}
// ��������Ƿ�Ϊ��׿��
stock IsPlayerAndroid(const playerid) {
    return PlayerInfo[playerid][Android];
}

// �������Ƿ�Ϊ��׿�� ע�⣬�ûص���������fixes ����ֻ����filterscript��ʹ�ã�
public OnClientCheckResponse(playerid, actionid, memaddr, retndata) {
    if(actionid == 0x48) {
        //  �����PC����ȡ��Ĭ�ϻ���Ϊ��׿��false
        PlayerInfo[playerid][Android] = false;
    }
    return 1;
}

function DelayLoading(playerid) {
    // �ӳټ��ص�¼ϵͳ���Ա�ͬ���ж�����Ƿ�Ϊ��׿ϵͳ
    new query[103];
    mysql_format(g_Sql, query, sizeof query, "SELECT * FROM `users` WHERE `Name` = '%e' LIMIT 1", GetName(playerid));
    mysql_tquery(g_Sql, query, "OnPlayerDataLoaded", "dd", playerid, g_MysqlRaceCheck[playerid]);
}

// ��ӭ�����Ϣ
stock Welcome(playerid) {
    new hours;
    gettime(hours);
    new hello[96];
    if(hours >= 6 && hours < 12) format(hello, sizeof(hello), "[������]���Ϻã�%s%d����ӭ��������֮ʱ��", GetName(playerid));
    else if(hours >= 12 && hours < 18) format(hello, sizeof(hello), "[������]����ã�%s%d����ӭ��������֮ʱ��", GetName(playerid));
    else if(hours >= 18 && hours < 24) format(hello, sizeof(hello), "[������]���Ϻã�%s%d����ӭ��������֮ʱ��", GetName(playerid));
    else format(hello, sizeof(hello), "[������]%s����ҹ�ɲ��Ǻ�ϰ�ߡ�", GetName(playerid));
    SCM(playerid, Color_White, hello);
    SCM(playerid, Color_Yellow, "[������]����ʼ���Թ���Ϊ��ּ�������ܾ�����");
}

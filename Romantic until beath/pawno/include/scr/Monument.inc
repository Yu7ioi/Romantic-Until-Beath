// 纪念碑谷 - 动态生成4个随机演员在指定坐标，读取数据库中随机用户信息并显示相关数据。
// 赋值对应皮肤，条件允许还可赋值对应装扮。

#define MAX_Monument 4
#define Act_Upt 25
enum Actor_Pos {
    Float:Actor_X, // 演员坐标X
    Float:Actor_Y, // 演员坐标Y
    Float:Actor_Z, // 演员坐标Z
    Float:Actor_R, // 演员坐标R
    Actor_Skin, // 演员皮肤
    Text3D:Actor_LabelId, //演员3D文字ID
    Actor_Label[256], // 演员3D文字
};

new Actor_SpawnPos[MAX_Monument][Actor_Pos] = { // 演员数据
    {
        -299.514648,
        1534.983642,
        75.562500,
        134.047225,
        0,
        Text3D:  - 1,
            "loading..."
    },
    {
        -301.514648,
        1534.983642,
        75.562500,
        134.047225,
        0,
        Text3D:  - 1,
        "loading..."
    },
    {
        -303.514648,
        1534.983642,
        75.562500,
        134.047225,
        0,
        Text3D:  - 1,
        "loading..."
    },
    {
        -305.514648,
        1534.983642,
        75.562500,
        134.047225,
        0,
        Text3D:  - 1,
        "loading..."
    }
};

new Actor_IdList[MAX_Monument]; // 演员ID数组
new Actor_upt_interval = 1; //演员时间计时器，用于计算多少秒后重置


enum Actor_Rand_Anim {
    Act_Animlib[18],
        Act_Animname[18]

}

new Actor_Rand_AnimLibs[][Actor_Rand_Anim] = {
    { "ATTRACTORS", "STEPSIT_LOOP" },
    { "BAR", "BARMAN_IDLE" },
    { "PED", "IDLE_CHAT" },
    { "PED", "ENDCHAT_03" },
    { "BASEBALL", "BAT_IDLE" },
    { "BD_FIRE", "BD_GF_WAVE" },
    { "BD_FIRE", "GRLFRD_KISS_03" },
    { "BD_FIRE", "M_SMKLEAN_LOOP" },
    { "BD_FIRE", "WASH_UP" },
    { "BEACH", "PARKSIT_M_LOOP" },
    { "BEACH", "PARKSIT_W_LOOP" },
    { "BEACH", "SITNWAIT_LOOP_W" },
    { "BMX", "BMX_PEDAL" },
    { "BOMBER", "BOM_PLANT" },
    { "BSKTBALL", "BBALL_IDLELOOP" },
    { "BSKTBALL", "BBALL_JUMP_SHOT" },
    { "BSKTBALL", "BBALL_REACT_MISS" },
    { "BSKTBALL", "BBALL_REACT_SCORE" },
    { "PARK", "Tai_Chi_Loop" }
};

stock Initialize_Monument() // 初始化纪念碑谷
{
    for (new i = 0; i < sizeof(Actor_IdList); i++) {
        setActorInfo(i);
    }
}

stock Destroy_Monument() // 销毁纪念碑谷
{
    for (new i = 0; i < sizeof(Actor_IdList); i++) {
        if (IsValidDynamic3DTextLabel(Actor_SpawnPos[i][Actor_LabelId])) {
            DestroyDynamic3DTextLabel(Actor_SpawnPos[i][Actor_LabelId]);
        }
        if (IsValidDynamicActor(Actor_IdList[i])) {
            DestroyDynamicActor(Actor_IdList[i]);
            Actor_IdList[i] = -1; // 重置默认值
        }
    }
}

stock setActorInfo(const index) { // 根据指定的数组中的下标 初始化演员信息
    if (!IsValidDynamicActor(Actor_IdList[index])) {
        Actor_IdList[index] = CreateDynamicActor(Actor_SpawnPos[index][Actor_Skin], Actor_SpawnPos[index][Actor_X], Actor_SpawnPos[index][Actor_Y], Actor_SpawnPos[index][Actor_Z], Actor_SpawnPos[index][Actor_R], true, 100.0, 0);
        Rand_Anim_Monument(Actor_IdList[index]);
    }
    // 动态销毁和创建label
    if (IsValidDynamic3DTextLabel(Actor_SpawnPos[index][Actor_LabelId])) {
        DestroyDynamic3DTextLabel(Actor_SpawnPos[index][Actor_LabelId]);
    }
    Actor_SpawnPos[index][Actor_LabelId] = CreateDynamic3DTextLabel(Actor_SpawnPos[index][Actor_Label], Color_White,
        Actor_SpawnPos[index][Actor_X] + 0.85, Actor_SpawnPos[index][Actor_Y], Actor_SpawnPos[index][Actor_Z], 7, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0);

}


stock seconds_Monument_Act() // 每指定秒数更新一次纪念碑谷
{
    if (++Actor_upt_interval == Act_Upt) {
        Actor_upt_interval = 0;
        new query[256]; //查询SQL
        mysql_format(g_Sql, query, sizeof query, "SELECT a.`ID`,a.`Name`,a.`Skin`,a.`Score`,a.`Cash`,a.`RegDate`,a.`LastLogin`,a.`exp`,a.`pLevel`,a.`BanTime`,COUNT(b.`uid`) AS `raceNumber` \
            FROM `users` \
            a LEFT JOIN `race_record` \
            b ON b. \
            `uid` = a. \
            `ID`\
            GROUP BY a. \
            `ID` \
            ORDER BY RAND() LIMIT % d", MAX_Monument);
        mysql_pquery(g_Sql, query, "update_Monument_Act");
    }
}

function update_Monument_Act() { // 获取随机玩家信息
    if (cache_num_rows() > 0) {
        Destroy_Monument(); // 销毁玩家信息
        for (new i = 0; i < cache_num_rows(); i++) {
            new Act_Id, Act_Name[MAX_PLAYER_NAME], Act_Skin, Act_Score, Act_Cash, Act_RegDate;
            new Act_LastLogin, Act_Exp, Act_Level, Act_BanTime, Act_BanStatus[20], Act_RaceNumber;
            new reg_year, reg_month, reg_day, reg_hour, reg_minute, reg_second;
            new login_year, login_month, login_day, login_hour, login_minute, login_second;
            cache_get_value_name_int(i, "ID", Act_Id);
            cache_get_value_name(i, "Name", Act_Name);
            cache_get_value_name_int(i, "Skin", Actor_SpawnPos[i][Actor_Skin]);
            cache_get_value_name_int(i, "Score", Act_Score);
            cache_get_value_name_int(i, "Cash", Act_Cash);
            cache_get_value_name_int(i, "RegDate", Act_RegDate);
            cache_get_value_name_int(i, "LastLogin", Act_LastLogin);
            cache_get_value_name_int(i, "exp", Act_Exp);
            cache_get_value_name_int(i, "pLevel", Act_Level);
            cache_get_value_name_int(i, "BanTime", Act_BanTime);
            cache_get_value_name_int(i, "raceNumber", Act_RaceNumber);
            TimestampToDate(Act_RegDate, reg_year, reg_month, reg_day, reg_hour, reg_minute, reg_second, 8);
            TimestampToDate(Act_LastLogin, login_year, login_month, login_day, login_hour, login_minute, login_second, 8);
            if (Act_BanTime != 0) format(Act_BanStatus, sizeof(Act_BanStatus), "{FF0000}封禁中");
            else format(Act_BanStatus, sizeof Act_BanStatus, "{00FF00}正常");
            format(Actor_SpawnPos[i][Actor_Label], 256, "%s(UID:%d)\n皮肤:%d\n时间分:%d\n现金:%d\n等级:%d(exp:%d)\n注册日期:%d-%d-%d %02d:%02d:%02d\n上次登录:%d-%d-%d %02d:%02d:%02d\n比赛次数(开发自):%d\n封禁状态:%s", Act_Name, Act_Id, Act_Skin, Act_Score, Act_Cash, Act_Level, Act_Exp, reg_year, reg_month, reg_day, reg_hour, reg_minute, reg_second, login_year, login_month, login_day, login_hour, login_minute, login_second, Act_RaceNumber, Act_BanStatus);
            setActorInfo(i); // 根据信息创建演员
        }
        // 随机动作
    }
}

stock Rand_Anim_Monument(const act_id) // 随机动作
{
    new Act_anim_index = random(sizeof(Actor_Rand_AnimLibs));
    ApplyDynamicActorAnimation(act_id, Actor_Rand_AnimLibs[Act_anim_index][Act_Animlib], Actor_Rand_AnimLibs[Act_anim_index][Act_Animname], 4.1, 1, 0, 0, 0, 0);
    // printf("act_id:%d, animlib:%s,animname:%s, rand:%d", act_id, Actor_Rand_AnimLibs[Act_anim_index][Act_Animlib], Actor_Rand_AnimLibs[Act_anim_index][Act_Animname],Act_anim_index);
}
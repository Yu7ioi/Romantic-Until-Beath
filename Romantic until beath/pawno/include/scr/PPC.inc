// IORP Script
enum ppctype {
    Float:X,
    Float:Y,
    Float:Z,
    Float:R
};

new PPC_SpawnPos[][ppctype] = {
    {
        -1790.714599, 552.736389, 234.615280, 322.741027
    },
    {
        -1808.469238,
        579.068603,
        234.617675,
        88.461364
    },
    ,
    {
        -1773.020874,
        578.919433,
        234.617996,
        103.577575
    },
    {
        -1804.983642,
        519.567504,
        234.618896,
        355.549987
    },
    {
        -1840.326049,
        578.858642,
        234.614852,
        237.597930
    },
    {
        -1820.136474,
        545.843017,
        234.614517,
        201.193878
    }
};

new p_PPC[MAX_PLAYERS], p_ppcCar[MAX_PLAYERS];

CMD:ppc(playerid, params[]) {
    new msg[128];
    if(p_PPC[playerid] == 0) {
        p_PPC[playerid] = 1;
		PPC_Veh(playerid);
        format(msg, sizeof msg, "系统 * %s(%d) 进入了碰碰车 /ppc", GetName(playerid), playerid);
    } else {
        p_PPC[playerid] = 0;
        OnPlayerSpawn(playerid);
        format(msg, sizeof msg, "系统 * %s(%d) 离开了碰碰车", GetName(playerid), playerid);
    }
    SendClientMessageToAll(white, msg)
    return 1;
}

stock running_PPC(playerid) {
    if(p_PPC[playerid]) {
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        if(z < 50 || z > 260) { //如果玩家的高度< 50那就是掉下去了  太高也不对
			SetPlayerHealth(playerid, -1);
            // dyC_Vehicle(playerid, 415, random(255), random(255));
            // dySetPlayerPos(playerid, PPC_SpawnPos[r][X], PPC_SpawnPos[r][Y], PPC_SpawnPos[r][Z], PPC_SpawnPos[r][R], 1, 2);
        }
    }
}
stock PPC_Veh(playerid) {
    new r = random(sizeof(PPC_SpawnPos));
    DestroyVehicle(p_ppcCar[playerid]);
    p_ppcCar[playerid] = CreateVehicle(415, PPC_SpawnPos[r][X], PPC_SpawnPos[r][Y], PPC_SpawnPos[r][Z], 0, -1, -1, 60);
    SetPlayerPos(playerid, PPC_SpawnPos[r][X], PPC_SpawnPos[r][Y], PPC_SpawnPos[r][Z]);
    SetVehiclePos(p_ppcCar[playerid], PPC_SpawnPos[r][X], PPC_SpawnPos[r][Y], PPC_SpawnPos[r][Z]);
    SetVehicleZAngle(p_ppcCar[playerid], PPC_SpawnPos[r][R]);
    PutPlayerInVehicle(playerid, p_ppcCar[playerid], 0)
}
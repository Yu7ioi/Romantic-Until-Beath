//Gps System By Shaheen//.........


#define     MAX_LIMITOBJ 25 //do not change
#include 	<common/GPS>

new GpsObject[MAX_PLAYERS][MAX_LIMITOBJ];
new GpsUpdateTimer[MAX_PLAYERS] = -1;
new MapNode:Nodess[MAX_PLAYERS][MAX_LIMITOBJ];
new bool:IsinGps[MAX_PLAYERS];
new MapNode:ChangeNode[MAX_PLAYERS][7];


function FindPathGps(playerid,Float:TarX,Float:TarY,Float:TarZ)
{
    new Float:x, Float:y, Float:z, MapNode:start,MapNode:target,Path:pathid;
    GetPlayerPos(playerid, x, y, z);
    GetClosestMapNodeToPoint(x, y, z, start);
    GetClosestMapNodeToPoint(TarX,TarY,TarZ, target);
    FindPath(start, target, pathid);
    new MapNode:nodeid, Float:xxx, Float:yyy, Float:zzz , Float:ANGLE;
    for (new index; index < MAX_LIMITOBJ; index++) 
	{
        GetPathNode(pathid, index, nodeid);
        if(index != 0)
        {
           GetMapNodePos(ChangeNode[playerid][6], xxx, yyy, zzz);
           GetMapNodeAngleFromPoint(nodeid,xxx, yyy, ANGLE);
        }
        GpsObject[playerid][index] = CreateDynamicObject(1318, xxx, yyy, zzz+0.7, 0, 120, ANGLE, -1, -1, playerid, STREAMER_OBJECT_SD, STREAMER_OBJECT_DD);
        // GpsObject[playerid][index] = CreatePlayerObject(playerid, 1318, xxx, yyy, zzz+0.7, 0, 120, ANGLE);
		if(index == 20)
		{
            ChangeNode[playerid][4] = nodeid;
		}
		if(index == 19)
		{
            ChangeNode[playerid][5] = nodeid;
		}
		if(index == 21)
		{
            ChangeNode[playerid][0] = nodeid;
		}
		if(index == 22)
		{
            ChangeNode[playerid][1] = nodeid;
		}
		if(index == 23)
		{
            ChangeNode[playerid][2] = nodeid;
		}
		if(index == 24)
		{
            ChangeNode[playerid][3] = nodeid;
		}
		Nodess[playerid][index] = nodeid;
		ChangeNode[playerid][6] = nodeid;
    }
    GpsUpdateTimer[playerid] = SetTimerEx_("FindPathGpsUpdate", 1200, 0, -1, "dfffd", playerid,TarX, TarY, TarZ,MAX_LIMITOBJ);
    IsinGps[playerid] = true;
    return 1;
}

function FindPathGpsUpdate(playerid,Float:TarX,Float:TarY,Float:TarZ,size)
{
	if(IsinGps[playerid])
	{
	    new Float:x,Float:y,Float:z,MapNode:Source,MapNode:Target;
	    GetPlayerPos(playerid,x,y,z);
	    GetClosestMapNodeToPoint(x, y, z, Source);
	    GetClosestMapNodeToPoint(TarX, TarY, TarZ, Target);
	    if (Source == Target)
	    {
	       KillTimer_(GpsUpdateTimer[playerid]);
		   GpsUpdateTimer[playerid] = -1;
		   destroyGpsObjects(playerid,MAX_LIMITOBJ);
		   SendClientMessage(playerid, Color_White,"[骇速导航]您已抵达目的地");
		   IsinGps[playerid] = false;
		   return 1;
	    }
	    new bool:found=false;
	    for(new i=0; i < MAX_LIMITOBJ; i++)
		{
			if (Source == ChangeNode[playerid][5] || Source == ChangeNode[playerid][3] || Source == ChangeNode[playerid][4] || Source == ChangeNode[playerid][0] ||Source == ChangeNode[playerid][1] || Source == ChangeNode[playerid][2] )
			{
                found = true;
                break;
			}
			else if(Nodess[playerid][i] == Source)
			{
				return 1;
			}
			else
			{
				found = true;
			}
		}
		if(found)
		{
		   KillTimer_(GpsUpdateTimer[playerid]);
		   GpsUpdateTimer[playerid] = -1;
		   destroyGpsObjects(playerid,MAX_LIMITOBJ);
		   FindPathGps(playerid,TarX, TarY, TarZ);
		   return 1;
		}
	}
	return 1;
}


function destroyGpsObjects(playerid,size)
{
	for(new i=0; i < MAX_LIMITOBJ; i++)
	{
    //   if(IsValidPlayerObject(playerid, GpsObject[playerid][i])) //only destroy if its valid
    	if(IsValidDynamicObject(GpsObject[playerid][i])) //only destroy if its valid
	  	{
			DestroyDynamicObject(GpsObject[playerid][i]);
        	//DestroyPlayerObject(playerid, GpsObject[playerid][i]);
      	}
	}
	return 1;
}


// CMD:gpsoff(playerid)
// {
//     destroyGpsObjects(playerid,MAX_LIMITOBJ);
//     KillTimer_(GpsUpdateTimer[playerid]);
// 	GpsUpdateTimer[playerid] = -1;
//     IsinGps[playerid] = false;
//     SendClientMessage(playerid, Color_White,"[骇速导航]已关闭本次导航");
// 	return 1;
// }


// COMMAND:findpath(playerid, params[])
// {
// 	new Float:x, Float:y, Float:z;
// 	if(sscanf(params, "p<,>fff", x, y, z))
// 	{
// 		SendClientMessage(playerid,-1,"Usage: /findpath <x,y,z>");
// 	}
// 	else
// 	{
// 		FindPathGps(playerid, x, y, z);
// 		SendClientMessage(playerid,-1,"Gps has been Turned On");
// 	}
// 	return 1;
// }

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	// 警告:在地图上被点击的区域距离玩家较远时，返回的 Z轴值为0 (无效)；

	// 未来有空的话将支持根据玩家设置地图点模式，默认为地图传送，除DM/碰碰车/赛车外可传送，或设置为导航则提供导航
	Gps_OnPlayerDisConnect(playerid, 1);
    FindPathGps(playerid, fX, fY, fZ);
	SendClientMessage(playerid, Color_White,"[骇速导航]已为您开启导航");
    return 1;
}

stock Gps_OnPlayerDisConnect(const playerid, const send = 0)
{
	if(IsinGps[playerid])
	{
		destroyGpsObjects(playerid,MAX_LIMITOBJ);
		KillTimer_(GpsUpdateTimer[playerid]);
		GpsUpdateTimer[playerid] = -1;
		IsinGps[playerid] = false;
		if(send) SendClientMessage(playerid, Color_White,"[骇速导航]已关闭本次导航");
		return 1;
	}
	return 1;
}
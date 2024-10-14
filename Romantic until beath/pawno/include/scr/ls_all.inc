// IORP Script
/*
    This server is modified from 5F free residential open source, Prace open source
    To the makers of Prace:[Ghost]Rui ushio_p [Ghost]Dylan
    Tribute to the makers of the free settlement :[ITC]dyq [ITC]fangye [ITC]Super_wlc [ITC
    Special thanks to ryddawn and technical advisor [Fire]KiVen OBJ:JoshenKM;

								The MIT License  
								
	Copyright (c) <2019-2020> YuCarl77
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
	版权(c) <2019-2020> YuCarl77

	使用该许可证的软件被授予以下权限，免费，任何人可以得到这个软件及其相关文档的一个拷贝，
	并且经营该软件不受任何限制，包括无限制的使用、复制、修改、合并、出版、发行、发放从属证书、或者出售该软件的拷贝的权利。
	同时允许获得这些软件的用户享受这些权利，使其服从下面的条件：  
	
	以上的版权通知和权限通知应该包含在所有该软件的拷贝中或者是其他该软件的真实部分中。
	
	该软件按本来的样子提供，没有任何形式的担保，不管是明确地或者暗含的，包含这些但是不受商业性质的担保的限制。
	适合一个特定的用途并且不受侵犯。作者和版权持有人在任何场合对使用该软件涉及的任何要求、损害或者其他责任都不应负责。
	不管它是正在起作用还是只是合同形式、民事侵权或是其他方式，如由它引起，在其作用范围内、与该软件有联系、该软件的使用或者有这个软件引起的其他行为。  
	=====================================   
*/


// 此inc为samp自带的脚本 整个地图三个该移除的OBJ和电梯系统
// YuCarl77适配流光插件


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// Example Filterscript for the new LS Apartments 1 Building with Elevator
// -----------------------------------------------------------------------
// Original elevator code by Zamaroht in 2010
//
// Updated by Kye in 2011
// * Added a sound effect for the elevator starting/stopping
//
// Edited by Matite in January 2015
// * Added code to remove the existing building, add the new building and
//   edited the elevator code so it works in this new building
//
// Updated to v1.02 by Matite in February 2015
// * Added code for the new car park object and edited the elevator to
//   include the car park
//
// This script creates the new LS Apartments 1 building object, removes the
// existing GTASA building object, adds the new car park object and creates
// an elevator that can be used to travel between all levels.
//
// You can un-comment the OnPlayerCommandText callback below to enable a simple
// teleport command (/lsa) that teleports you to the LS Apartments 1 building.
//
// Warning...
// This script uses a total of:
// * 27 objects = 1 for the elevator, 2 for the elevator doors, 22 for the
//   elevator floor doors, 1 for the replacement LS Apartments 1 building
//   and 1 for the car park
// * 12 3D Text Labels = 11 on the floors and 1 in the elevator
// * 1 dialog (for the elevator - dialog ID 876)
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------



// For PlaySoundForPlayersInRange()
#include "../include/gl_common.inc"

// Movement speed of the elevator
#define ELEVATOR_SPEED      (5.0)

// Movement speed of the doors
#define DOORS_SPEED         (5.0)

// Time in ms that the elevator will wait in each floor before continuing with the queue...
// be sure to give enough time for doors to open
#define ELEVATOR_WAIT_TIME  (5000)  

// Dialog ID for the LS Apartments building elevator dialog
#define LS_APARTMENTSID           (876)
#define LS_BeachSideID           (877)

// Position defines
#define Y_DOOR_CLOSED       (-1180.535917)
#define Y_DOOR_R_OPENED     Y_DOOR_CLOSED - 1.6
#define Y_DOOR_L_OPENED     Y_DOOR_CLOSED + 1.6

#define GROUND_Z_COORD      (20.879316)

#define ELEVATOR_OFFSET     (0.059523)

#define X_ELEVATOR_POS      (1181.622924)
#define Y_ELEVATOR_POS      (-1180.554687)

// Elevator state defines
#define ELEVATOR_STATE_IDLE     (0)
#define ELEVATOR_STATE_WAITING  (1)
#define ELEVATOR_STATE_MOVING   (2)

// Invalid floor define
#define INVALID_FLOOR           (-1)


// Elevator floor names for the 3D text labels
static FloorNames[11][] = {
    "停车场",
    "地下一楼",
    "一楼",
    "二楼",
    "三楼",
    "四楼",
    "五楼",
    "六楼",
    "七楼",
    "八楼",
    "九楼"
};

// Elevator floor Z heights
static Float:FloorZOffsets[11] = {
    0.0, // Car Park
    13.604544, // Ground Floor
    18.808519, // First Floor = 13.604544 + 5.203975
    24.012494, // Second Floor = 18.808519 + 5.203975
    29.216469, // Third Floor = 24.012494 + 5.203975
    34.420444, // Fourth Floor = 29.216469 + 5.203975
    39.624419, // Fifth Floor = 34.420444 + 5.203975
    44.828394, // Sixth Floor = 39.624419 + 5.203975
    50.032369, // Seventh Floor = 44.828394 + 5.203975
    55.236344, // Eighth Floor = 50.032369 + 5.203975
    60.440319 // Ninth Floor = 55.236344 + 5.203975
};

// Stores the created object numbers of the elevator, the elevator doors and
// the elevator floor doors so they can be destroyed when the filterscript
// is unloaded
new Obj_Elevator, Obj_ElevatorDoors[2], Obj_FloorDoors[11][2];

// Stores a reference to the 3D text labels used on each floor and inside the
// elevator itself so they can be detroyed when the filterscript is unloaded
new Text3D:Label_Elevator, Text3D:Label_Floors[11];

// Stores the current state of the elevator (ie ELEVATOR_STATE_IDLE,
// ELEVATOR_STATE_WAITING or ELEVATOR_STATE_MOVING)
new ElevatorState;

// Stores the current floor the elevator is on or heading to... if the value is
// ELEVATOR_STATE_IDLE or ELEVATOR_STATE_WAITING this is the current floor. If
// the value is ELEVATOR_STATE_MOVING then it is the floor it's moving to
new ElevatorFloor;

// Stores the elevator queue for each floor
new ElevatorQueue[11];

// Stores who requested the floor for the elevator queue...
// FloorRequestedBy[floor_id] = playerid;  (stores who requested which floor)
new FloorRequestedBy[11];

// Used for a timer that makes the elevator move faster after players start
// surfing the object
new ElevatorBoostTimer;

// ------------------------------------------------------------------------------
// Function Forwards
// -----------------

// Public:
forward CallElevator(playerid, floorid); // You can use INVALID_PLAYER_ID too.
forward ShowElevatorDialog(playerid);

// Private:
forward Elevator_Initialize();

forward Elevator_OpenDoors();
forward Elevator_CloseDoors();
forward Floor_OpenDoors(floorid);
forward Floor_CloseDoors(floorid);

forward Elevator_MoveToFloor(floorid);
forward Elevator_Boost(floorid); // Increases the elevator speed until it reaches 'floorid'.
forward Elevator_TurnToIdle();

forward ReadNextFloorInQueue();
forward RemoveFirstQueueFloor();
forward AddFloorToQueue(floorid);
forward IsFloorInQueue(floorid);
forward ResetElevatorQueue();

forward DidPlayerRequestElevator(playerid);

forward Float:GetElevatorZCoordForFloor(floorid);
forward Float:GetDoorsZCoordForFloor(floorid);

stock Initialize_LSApartments() { //gamemodeinit时加载
    print("\n");
    print("  |---------------------------------------------------");
    print("  |--- LS Apartments 1 Filterscript");
    print("  |--  Script v1.02");
    print("  |--  5th February 2015");
    print("  |---------------------------------------------------");

    print("  |--  LS Apartments 1 Building object created");
    CreateDynamicObject(19595, 1160.96, -1180.58, 70.4141, 0, 0, 0);
    CreateDynamicObject(19798, 1160.96, -1180.58, 20.4141, 0, 0, 0);
    print("  |--  LS Apartments 1 Car Park object created");
    ResetElevatorQueue();
    Elevator_Initialize();
    return 1;
}

stock LSApartments_OnPlayerConnect(const playerid) { //玩家加入时调用
    // Remove default GTASA building map object, LOD and awning shadows
    RemoveBuildingForPlayer(playerid, 5766, 1160.96, -1180.58, 70.4141, 250.0); // Awning shadows
    RemoveBuildingForPlayer(playerid, 5767, 1160.96, -1180.58, 70.4141, 250.0); // Building
    RemoveBuildingForPlayer(playerid, 5964, 1160.96, -1180.58, 70.4141, 250.0); // LOD
    return 1;
}

public OnDynamicObjectMoved(objectid) {
    // Create variables
    new Float:x, Float:y, Float:z;

    // Loop
    for (new i; i < sizeof(Obj_FloorDoors); i++) {
        // Check if the object that moved was one of the elevator floor doors
        if(objectid == Obj_FloorDoors[i][0]) {
            GetObjectPos(Obj_FloorDoors[i][0], x, y, z);

            // Some floor doors have shut, move the elevator to next floor in queue:
            if(y < Y_DOOR_L_OPENED - 0.5) {
                Elevator_MoveToFloor(ElevatorQueue[0]);
                RemoveFirstQueueFloor();
            }
        }
    }

    if(objectid == Obj_Elevator) // The elevator reached the specified floor.
    {
        KillTimer(ElevatorBoostTimer); // Kills the timer, in case the elevator reached the floor before boost.

        FloorRequestedBy[ElevatorFloor] = INVALID_PLAYER_ID;

        Elevator_OpenDoors();
        Floor_OpenDoors(ElevatorFloor);

        GetObjectPos(Obj_Elevator, x, y, z);
        Label_Elevator = CreateDynamic3DTextLabel("{CCCCCC}按下 '{FFFFFF}~k~~CONVERSATION_YES~{CCCCCC}'使用电梯", 0xCCCCCCAA, X_ELEVATOR_POS - 1.7, Y_ELEVATOR_POS - 1.75, z - 0.4, 4.0, 0, 1);

        ElevatorState = ELEVATOR_STATE_WAITING;
        SetTimer("Elevator_TurnToIdle", ELEVATOR_WAIT_TIME, 0);
    }

    return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) {
    if(dialogid == LS_APARTMENTSID) {
        if(!response) return 0;
        if(FloorRequestedBy[listitem] != INVALID_PLAYER_ID || IsFloorInQueue(listitem)) {
            GameTextForPlayer(playerid, "~r~The floor is already in the queue", 3500, 4);
        } else if(DidPlayerRequestElevator(playerid)) {
            GameTextForPlayer(playerid, "~r~You already requested the elevator", 3500, 4);
        } else {
            CallElevator(playerid, listitem);
        }
        return 1;
    }
    return 0;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    // Check if the player is not in a vehicle and pressed the conversation yes key (Y by default)
    if(!IsPlayerInAnyVehicle(playerid) && (newkeys & KEY_YES)) {
        // Create variables and get the players current position
        new Float:pos[3];
        GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

        // For debug
        //printf("X = %0.2f | Y = %0.2f | Z = %0.2f", pos[0], pos[1], pos[2]);

        // Check if the player is using the button inside the elevator
        if(pos[1] > (Y_ELEVATOR_POS - 1.8) && pos[1] < (Y_ELEVATOR_POS + 1.8) && pos[0] < (X_ELEVATOR_POS + 1.8) && pos[0] > (X_ELEVATOR_POS - 1.8)) {
            // The player is using the button inside the elevator
            // --------------------------------------------------

            // Show the elevator dialog to the player
            ShowElevatorDialog(playerid);
        } else {
            // Check if the player is using the button on one of the floors
            if(pos[1] < (Y_ELEVATOR_POS - 1.81) && pos[1] > (Y_ELEVATOR_POS - 3.8) && pos[0] > (X_ELEVATOR_POS - 3.8) && pos[0] < (X_ELEVATOR_POS - 1.81)) {
                // The player is most likely using an elevator floor button... check which floor
                // -----------------------------------------------------------------------------

                // Create variable with the number of floors to check (total floors minus 1)
                new i = 10;

                // Loop
                while (pos[2] < GetDoorsZCoordForFloor(i) + 3.5 && i > 0)
                    i--;

                if(i == 0 && pos[2] < GetDoorsZCoordForFloor(0) + 2.0)
                    i = -1;

                if(i <= 9) {
                    // Check if the elevator is not moving (idle or waiting)
                    if(ElevatorState != ELEVATOR_STATE_MOVING) {
                        // Check if the elevator is already on the floor it was called from
                        if(ElevatorFloor == i + 1) {
                            // Display gametext message to the player
                            GameTextForPlayer(playerid, "~n~~n~~n~~n~~n~~n~~n~~y~~h~LS Apartments 1 Elevator Is~n~~y~~h~Already On This Floor...~n~~w~Walk Inside It~n~~w~And Press '~k~~CONVERSATION_YES~'", 3500, 3);

                            // Display chat text message to the player
                            SendClientMessage(playerid, Color_Yellow, "* LS公寓1号电梯已经在这层了…走进去按下'{FFFFFF}~k~~CONVERSATION_YES~{CCCCCC}'");

                            // Exit here (return 1 so this callback is processed in other scripts)
                            return 1;
                        }
                    }

                    // Call function to call the elevator to the floor
                    CallElevator(playerid, i + 1);

                    // Display gametext message to the player
                    GameTextForPlayer(playerid, "~n~~n~~n~~n~~n~~n~~n~~n~~g~~h~LS Apartments 1 Elevator~n~~g~~h~Has Been Called...~n~~w~Please Wait", 3000, 3);

                    // Create variable for formatted message
                    new strTempString[100];

                    // Check if the elevator is moving
                    if(ElevatorState == ELEVATOR_STATE_MOVING) {
                        // Format chat text message
                        format(strTempString, sizeof(strTempString), "* LS公寓1号电梯已经呼叫…它目前正在向%s移动.", FloorNames[ElevatorFloor]);
                    } else {
                        // Check if the floor is the car park
                        if(ElevatorFloor == 0) {
                            // Format chat text message
                            format(strTempString, sizeof(strTempString), "* LS公寓1号电梯已经呼叫…it is currently at the %s.", FloorNames[ElevatorFloor]);
                        } else {
                            // Format chat text message
                            format(strTempString, sizeof(strTempString), "* LS公寓1号电梯已经呼叫…it is currently on the %s.", FloorNames[ElevatorFloor]);
                        }
                    }

                    // Display formatted chat text message to the player
                    SendClientMessage(playerid, Color_Yellow, strTempString);

                    // Exit here (return 1 so this callback is processed in other scripts)
                    return 1;
                }
            }
        }
    }

    // Exit here (return 1 so this callback is processed in other scripts)
    return 1;
}

// ------------------------ Functions ------------------------
stock Elevator_Initialize() {
    // Create the elevator and elevator door objects
    Obj_Elevator = CreateDynamicObject(18755, X_ELEVATOR_POS, Y_ELEVATOR_POS, GROUND_Z_COORD + ELEVATOR_OFFSET, 0.000000, 0.000000, 0.000000);
    Obj_ElevatorDoors[0] = CreateDynamicObject(18757, X_ELEVATOR_POS, Y_ELEVATOR_POS, GROUND_Z_COORD + ELEVATOR_OFFSET, 0.000000, 0.000000, 0.000000);
    Obj_ElevatorDoors[1] = CreateDynamicObject(18756, X_ELEVATOR_POS, Y_ELEVATOR_POS, GROUND_Z_COORD + ELEVATOR_OFFSET, 0.000000, 0.000000, 0.000000);

    // Create the 3D text label for inside the elevator
    Label_Elevator = CreateDynamic3DTextLabel("{CCCCCC}按下 '{FFFFFF}~k~~CONVERSATION_YES~{CCCCCC}' 使用电梯", 0xCCCCCCAA, X_ELEVATOR_POS - 1.7, Y_ELEVATOR_POS - 1.75, GROUND_Z_COORD - 0.4, 4.0, 0, 1);

    // Create variables
    new string[128], Float:z;

    // Loop
    for (new i; i < sizeof(Obj_FloorDoors); i++) {
        // Create elevator floor door objects
        Obj_FloorDoors[i][0] = CreateDynamicObject(18757, X_ELEVATOR_POS - 0.245, Y_ELEVATOR_POS, GetDoorsZCoordForFloor(i), 0.000000, 0.000000, 0.000000);
        Obj_FloorDoors[i][1] = CreateDynamicObject(18756, X_ELEVATOR_POS - 0.245, Y_ELEVATOR_POS, GetDoorsZCoordForFloor(i), 0.000000, 0.000000, 0.000000);

        // Format string for the floor 3D text label
        format(string, sizeof(string), "{CCCCCC}[%s]\n{CCCCCC}按下 '{FFFFFF}~k~~CONVERSATION_YES~{CCCCCC}' 呼叫", FloorNames[i]);

        // Get label Z position
        z = GetDoorsZCoordForFloor(i);

        // Create floor label
        Label_Floors[i] = CreateDynamic3DTextLabel(string, 0xCCCCCCAA, X_ELEVATOR_POS - 2.5, Y_ELEVATOR_POS - 2.5, z - 0.2, 10.5, 0, 1);
    }

    // Open the car park floor doors and the elevator doors
    Floor_OpenDoors(0);
    Elevator_OpenDoors();

    // Exit here
    return 1;
}


stock Elevator_OpenDoors() {
    // Opens the elevator's doors.

    new Float:x, Float:y, Float:z;

    GetObjectPos(Obj_ElevatorDoors[0], x, y, z);
    MoveDynamicObject(Obj_ElevatorDoors[0], x, Y_DOOR_L_OPENED, z, DOORS_SPEED);
    MoveDynamicObject(Obj_ElevatorDoors[1], x, Y_DOOR_R_OPENED, z, DOORS_SPEED);

    return 1;
}

stock Elevator_CloseDoors() {
    // Closes the elevator's doors.

    if(ElevatorState == ELEVATOR_STATE_MOVING)
        return 0;

    new Float:x, Float:y, Float:z;

    GetObjectPos(Obj_ElevatorDoors[0], x, y, z);
    MoveDynamicObject(Obj_ElevatorDoors[0], x, Y_DOOR_CLOSED, z, DOORS_SPEED);
    MoveDynamicObject(Obj_ElevatorDoors[1], x, Y_DOOR_CLOSED, z, DOORS_SPEED);

    return 1;
}

stock Floor_OpenDoors(const floorid) {
    // Opens the doors at the specified floor.

    MoveDynamicObject(Obj_FloorDoors[floorid][0], X_ELEVATOR_POS - 0.245, Y_DOOR_L_OPENED, GetDoorsZCoordForFloor(floorid), DOORS_SPEED);
    MoveDynamicObject(Obj_FloorDoors[floorid][1], X_ELEVATOR_POS - 0.245, Y_DOOR_R_OPENED, GetDoorsZCoordForFloor(floorid), DOORS_SPEED);

    PlaySoundForPlayersInRange(6401, 50.0, X_ELEVATOR_POS, Y_ELEVATOR_POS, GetDoorsZCoordForFloor(floorid) + 5.0);

    return 1;
}

stock Floor_CloseDoors(const floorid) {
    // Closes the doors at the specified floor.

    MoveDynamicObject(Obj_FloorDoors[floorid][0], X_ELEVATOR_POS - 0.245, Y_ELEVATOR_POS, GetDoorsZCoordForFloor(floorid), DOORS_SPEED);
    MoveDynamicObject(Obj_FloorDoors[floorid][1], X_ELEVATOR_POS - 0.245, Y_ELEVATOR_POS, GetDoorsZCoordForFloor(floorid), DOORS_SPEED);

    PlaySoundForPlayersInRange(6401, 50.0, X_ELEVATOR_POS, Y_ELEVATOR_POS, GetDoorsZCoordForFloor(floorid) + 5.0);

    return 1;
}

stock Elevator_MoveToFloor(const floorid) {
    // Moves the elevator to specified floor (doors are meant to be already closed).

    ElevatorState = ELEVATOR_STATE_MOVING;
    ElevatorFloor = floorid;

    // Move the elevator slowly, to give time to clients to sync the object surfing. Then, boost it up:
    MoveDynamicObject(Obj_Elevator, X_ELEVATOR_POS, Y_ELEVATOR_POS, GetElevatorZCoordForFloor(floorid), 0.25);
    MoveDynamicObject(Obj_ElevatorDoors[0], X_ELEVATOR_POS, Y_ELEVATOR_POS, GetDoorsZCoordForFloor(floorid), 0.25);
    MoveDynamicObject(Obj_ElevatorDoors[1], X_ELEVATOR_POS, Y_ELEVATOR_POS, GetDoorsZCoordForFloor(floorid), 0.25);
    DestroyDynamic3DTextLabel(Label_Elevator);

    ElevatorBoostTimer = SetTimerEx("Elevator_Boost", 2000, 0, "i", floorid);

    return 1;
}

public Elevator_Boost(floorid) {
    // Increases the elevator's speed until it reaches 'floorid'
    StopObject(Obj_Elevator);
    StopObject(Obj_ElevatorDoors[0]);
    StopObject(Obj_ElevatorDoors[1]);

    MoveDynamicObject(Obj_Elevator, X_ELEVATOR_POS, Y_ELEVATOR_POS, GetElevatorZCoordForFloor(floorid), ELEVATOR_SPEED);
    MoveDynamicObject(Obj_ElevatorDoors[0], X_ELEVATOR_POS, Y_ELEVATOR_POS, GetDoorsZCoordForFloor(floorid), ELEVATOR_SPEED);
    MoveDynamicObject(Obj_ElevatorDoors[1], X_ELEVATOR_POS, Y_ELEVATOR_POS, GetDoorsZCoordForFloor(floorid), ELEVATOR_SPEED);

    return 1;
}

public Elevator_TurnToIdle() {
    ElevatorState = ELEVATOR_STATE_IDLE;
    ReadNextFloorInQueue();

    return 1;
}

stock RemoveFirstQueueFloor() {
    // Removes the data in ElevatorQueue[0], and reorders the queue accordingly.

    for (new i; i < sizeof(ElevatorQueue) - 1; i++)
        ElevatorQueue[i] = ElevatorQueue[i + 1];

    ElevatorQueue[sizeof(ElevatorQueue) - 1] = INVALID_FLOOR;

    return 1;
}

stock AddFloorToQueue(const floorid) {
    // Adds 'floorid' at the end of the queue.

    // Scan for the first empty space:
    new slot = -1;
    for (new i; i < sizeof(ElevatorQueue); i++) {
        if(ElevatorQueue[i] == INVALID_FLOOR) {
            slot = i;
            break;
        }
    }

    if(slot != -1) {
        ElevatorQueue[slot] = floorid;

        // If needed, move the elevator.
        if(ElevatorState == ELEVATOR_STATE_IDLE)
            ReadNextFloorInQueue();

        return 1;
    }

    return 0;
}

stock ResetElevatorQueue() {
    // Resets the queue.

    for (new i; i < sizeof(ElevatorQueue); i++) {
        ElevatorQueue[i] = INVALID_FLOOR;
        FloorRequestedBy[i] = INVALID_PLAYER_ID;
    }

    return 1;
}

stock IsFloorInQueue(const floorid) {
    // Checks if the specified floor is currently part of the queue.

    for (new i; i < sizeof(ElevatorQueue); i++)
        if(ElevatorQueue[i] == floorid)
            return 1;

    return 0;
}

stock ReadNextFloorInQueue() {
    // Reads the next floor in the queue, closes doors, and goes to it.

    if(ElevatorState != ELEVATOR_STATE_IDLE || ElevatorQueue[0] == INVALID_FLOOR)
        return 0;

    Elevator_CloseDoors();
    Floor_CloseDoors(ElevatorFloor);

    return 1;
}

stock DidPlayerRequestElevator(const playerid) {
    for (new i; i < sizeof(FloorRequestedBy); i++)
        if(FloorRequestedBy[i] == playerid)
            return 1;

    return 0;
}

stock ShowElevatorDialog(const playerid) {
    new string[512];
    for (new i; i < sizeof(ElevatorQueue); i++) {
        if(FloorRequestedBy[i] != INVALID_PLAYER_ID)
            strcat(string, "{FF0000}");

        strcat(string, FloorNames[i]);
        strcat(string, "\n");
    }

    ShowPlayerDialog(playerid, LS_APARTMENTSID, DIALOG_STYLE_LIST, "LS公寓1号电梯", string, "确定", "取消");

    return 1;
}

stock CallElevator(const playerid, const floorid) {
    // Calls the elevator (also used with the elevator dialog).

    if(FloorRequestedBy[floorid] != INVALID_PLAYER_ID || IsFloorInQueue(floorid))
        return 0;

    FloorRequestedBy[floorid] = playerid;
    AddFloorToQueue(floorid);

    return 1;
}

stock Float:GetElevatorZCoordForFloor(const floorid) {
    // Return Z height value plus a small offset
    return (GROUND_Z_COORD + FloorZOffsets[floorid] + ELEVATOR_OFFSET);
}

stock Float:GetDoorsZCoordForFloor(const floorid) {
    // Return Z height value plus a small offset
    return (GROUND_Z_COORD + FloorZOffsets[floorid] + ELEVATOR_OFFSET);
}
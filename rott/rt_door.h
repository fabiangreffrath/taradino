/*
Copyright (C) 1994-1995 Apogee Software, Ltd.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
#ifndef _rt_door_public
#define _rt_door_public

#include <stdbool.h>
#include <stdint.h>

#include "rt_def.h"

//***************************************************************************
//
//   RT_DOOR.C - doors
//
//***************************************************************************

#define MAXTOUCHPLATES 64
#define MAXMASKED      300  // max masked walls
#define MAXDOORS       150  // max number of sliding doors
#define MAXPWALLS      150  // max number of pushwalls
#define DF_TIMED       0x01
#define DF_ELEVLOCKED  0x02
#define DF_MULTI       0x04
#define MAXELEVATORS   16

#define PW_DAMAGE      0x01

#define NUMELEVATORACTIONS 5

typedef enum
{ev_ras, //ready at source
 ev_rad, //ready at destination
 ev_mts, //moving to source
 ev_mtd,  //moving to destination
			 //door at elevator location open
 ev_doorclosing //door at elevator location closed
}estate;

typedef enum
{
 mw_peephole,
 mw_dogwall,
 mw_multi1,
 mw_multi2,
 mw_multi3,
 mw_singlepane,
 mw_normal1,
 mw_normal2,
 mw_normal3,
 mw_exitarch,
 mw_secretexitarch,
 mw_railing,
 mw_hiswitchon,
 mw_hiswitchoff,
 mw_platform1,
 mw_platform2,
 mw_platform3,
 mw_platform4,
 mw_platform5,
 mw_platform6,
 mw_platform7,
 mw_entrygate
}masked_walls;

typedef struct elevator
{ short sx,sy;
  short dx,dy;
  short esx,esy,edx,edy;
  short door1,door2;
  signed char state;
  short doortoopen;
  short doorclosing;
  short ticcount;
  short nextaction;
}elevator_t;


typedef struct doorstruct
{
		  thingtype   which;
		  uint8_t        tilex,tiley;
		  uint16_t        texture;
		  uint16_t        alttexture;
		  uint16_t        sidepic;
		  uint16_t        basetexture;
		  uint8_t        lock;
		  uint8_t        flags;
		  short   ticcount;
		  signed char eindex;
		  bool     vertical;
		  int         soundhandle;
		  int         position;
		  enum    {dr_open,dr_closed,dr_opening,dr_closing}       action;
} doorobj_t;

typedef struct pwallstruct
{
		  thingtype      which;
		  int       x,y;
		  int       momentumx,momentumy;
		  uint8_t      areanumber;
		  uint8_t      lock;
		  uint8_t      dir;
		  uint8_t      tilex,tiley;
		  uint8_t      num;
		  uint8_t      speed;
		  uint16_t      texture;
		  int       soundhandle;
		  enum      {pw_npushed,pw_pushing,pw_pushed,pw_moving}       action;
		  int       state;
        uint8_t      flags;
} pwallobj_t;

typedef struct tplate
{ void (*action)(intptr_t);
  void (*swapaction)(intptr_t);
  struct tplate * nextaction;
  struct tplate * prevaction;
  intptr_t whichobj;
  uint8_t tictime;
  uint8_t ticcount;
  uint8_t triggered;
  uint8_t done;
  uint8_t complete;
  uint8_t clocktype;
} touchplatetype;

#define MW_SHOOTABLE       0x01
#define MW_BLOCKING        0x02
#define MW_MULTI           0x04
#define MW_BLOCKINGCHANGES 0x08
#define MW_ABOVEPASSABLE   0x10
#define MW_NONDOGBLOCKING  0x20
#define MW_WEAPONBLOCKING  0x40
#define MW_BOTTOMPASSABLE  0x80
#define MW_MIDDLEPASSABLE  0x100
#define MW_ABP             0x200
#define MW_SWITCHON        0x400
#define MW_BOTTOMFLIPPING  0x800
#define MW_TOPFLIPPING     0x1000
#define M_ISDOOR(x,y) ((tilemap[x][y] & 0x8000) && (!(tilemap[x][y] & 0x4000)))
#define M_ISMWALL(x,y) ((tilemap[x][y] & 0x8000) && (tilemap[x][y] & 0x4000))


typedef struct mwall
{
  thingtype      which;
  uint8_t      tilex,tiley;
  signed char areanumber;
  signed short toptexture;
  signed short midtexture;
  signed short bottomtexture;
  uint16_t      flags;
  bool   vertical;
  int       sidepic;

  struct mwall *next;
  struct mwall *prev;

} maskedwallobj_t;

typedef struct animmwall
{
  uint16_t     num;
  uint8_t     count;
  signed char ticcount;
  struct animmwall *next;
  struct animmwall *prev;

} animmaskedwallobj_t;

extern elevator_t          ELEVATOR[MAXELEVATORS];
extern int                 _numelevators;
extern animmaskedwallobj_t *FIRSTANIMMASKEDWALL,*LASTANIMMASKEDWALL;
extern maskedwallobj_t     *FIRSTMASKEDWALL,*LASTMASKEDWALL;
extern uint8_t                touchindices[MAPSIZE][MAPSIZE],lasttouch;
extern touchplatetype      *touchplate[MAXTOUCHPLATES],*lastaction[MAXTOUCHPLATES];
extern uint8_t                TRIGGER[MAXTOUCHPLATES];

extern doorobj_t           *doorobjlist[MAXDOORS];
extern int                 doornum;
extern maskedwallobj_t     *maskobjlist[MAXMASKED];
extern int                 maskednum;
extern pwallobj_t          *pwallobjlist[MAXPWALLS];
extern int                 pwallnum;
																																			 // 0xffff = fully open
extern uint8_t                areaconnect[NUMAREAS][NUMAREAS];
extern bool             areabyplayer[NUMAREAS];


void ActivateAllPushWalls(void);
bool CheckTile(int,int);
void FindEmptyTile(int*,int*);
int  Number_of_Empty_Tiles_In_Area_Around(int,int);
void AddTouchplateAction(touchplatetype*,int);
void RemoveTouchplateAction(touchplatetype*,int);

void InitElevators(void);
void ProcessElevators(void);
void OperateElevatorDoor(int);


int  PlatformHeight(int,int);
void Link_To_Touchplate(uint16_t, uint16_t, void (*)(intptr_t), void (*)(intptr_t), intptr_t, int);
void TriggerStuff(void);
void ClockLink(void (*)(intptr_t),void(*)(intptr_t),intptr_t,int);
void RecursiveConnect(int);
void ConnectAreas(void);
void InitAreas(void);
void InitDoorList(void);
void SpawnDoor(int,int,int,int);
void SpawnMaskedWall (int tilex, int tiley, int which, int flags);
void OpenDoor(int);
void CloseDoor(int);
void OperateDoor (int keys, int door, bool localplayer );
void DoorOpen(int);
void DoorOpening(int);
void DoorClosing(int door);
void MoveDoors(void);
void SpawnPushWall (int tilex, int tiley, int lock, int texture, int dir, int type);
void MovePWalls(void);
void WallPushing (int pwall);
void PushWall (int pwall, int dir);
void OperatePushWall (int pwall, int dir, bool localplayer );
void ActivatePushWall (intptr_t pwall);
void ActivateMoveWall (intptr_t pwall);
int  UpdateMaskedWall (int num);

void FixDoorAreaNumbers ( void );
void FixMaskedWallAreaNumbers ( void );
void SaveMaskedWalls(uint8_t ** buf, int * size);
void LoadMaskedWalls(uint8_t * buf, int size);
void SaveDoors(uint8_t ** buf, int * size);
void SaveTouchPlates(uint8_t ** buf, int * size);
void LoadDoors(uint8_t * buf, int size);
void LoadTouchPlates(uint8_t * buf, int size);
void SavePushWalls(uint8_t ** buf, int * sz);
void LoadPushWalls(uint8_t * bufptr, int sz);

void DeactivateAnimMaskedWall(animmaskedwallobj_t* amwall);
void ActivateAnimMaskedWall(animmaskedwallobj_t* amwall);

void SpawnAnimatedMaskedWall ( int num );
void KillAnimatedMaskedWall ( animmaskedwallobj_t * temp );

void DoAnimatedMaskedWalls ( void );

void SaveElevators(uint8_t ** buffer,int *size);

void LoadElevators(uint8_t * buffer,int size);

void MakeWideDoorVisible ( int doornum );
void LinkedCloseDoor (intptr_t door);
void LinkedOpenDoor (intptr_t door);
int IsWall (int tilex, int tiley);
int IsDoor (int tilex, int tiley);
int IsMaskedWall (int tilex, int tiley);
#endif

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
#include "rt_def.h"
#include "sprites.h"
#include "states.h"
#include "rt_actor.h"
#include "develop.h"
#define SPRINGDELAY 3

/*
=============================================================================

					  LOW GUARD (includes sneaky)

=============================================================================
*/


extern void T_ReallyDead(objtype*ob);
extern void T_PlayDead(objtype*ob);
extern void T_Xylophone(objtype*ob);
extern void T_BloodFall(objtype*ob);
extern void SetShapeoffset(objtype*ob);
extern void RespawnPlayerobj(objtype*ob);
extern void T_ElevDisk(objtype*ob);
extern void T_Plead(objtype*ob);
extern void T_Blood(objtype*ob);
extern void T_Convert(objtype*ob);
extern void T_SlideDownScreen(objtype*ob);
extern void T_End(objtype*ob);
extern void T_Reset(objtype*ob);
extern void T_CollectorWander(objtype*ob);
extern void T_CollectorFindDoor(objtype*ob);
extern void T_AutoPath(objtype*);
extern void T_AutoRealign(objtype*);
extern void T_AutoShootAlign(objtype*);
extern void T_NME_SpinFire(objtype*);
extern void MissileMovement(objtype*);
extern void T_BatBlast(objtype*);
extern void T_DeadWait(objtype*);
extern void ActorMovement(objtype*);
extern void T_Spring(objtype*);
extern void T_SnakeFinale(objtype*);
extern void T_Special(objtype*);
extern void T_NME_Explode(objtype*);
extern void T_Guts(objtype*);
extern void T_Player(objtype*);
extern void T_ParticleGenerate(objtype*);
extern void T_Particle(objtype*);
extern void T_SpawnSoul(objtype*);
extern void T_NME_WindUp(objtype*);
extern void T_NME_Attack(objtype*);
extern void T_Saucer(objtype*);
extern void T_NMErocket(objtype*);
extern void T_NME_SpinAttack(objtype*);
extern void T_NME_HeadShoot(objtype*);
extern void T_CrushUp(objtype*);
extern void T_CrushDown(objtype*);
extern void T_HeinrichChase(objtype*);
extern void T_KristLeft(objtype*);
extern void T_KristRight(objtype*);
extern void T_KristCheckFire(objtype*);
extern void T_BoulderSpawn(objtype*);
extern void T_BoulderDrop(objtype*);
extern void T_BoulderMove(objtype*);
extern void T_DarkmonkChase(objtype*);
extern void T_DarkSnakeChase(objtype*);
extern void T_DarkSnakeSpawn(objtype*);
extern void A_DmonkAttack(objtype*);
extern void T_GenericMove(objtype*);
extern void T_Count(objtype*);
extern void T_Spears(objtype*);
extern void T_EsauSpears(objtype*);
extern void T_Spring(objtype*);
extern void T_4WayGunStand(objtype*);
extern void A_GunShoot(objtype*);
extern void A_4WayGunShoot(objtype*);
extern void T_Attack(objtype*);
extern void T_SnakePath(objtype*);
extern void T_SnakeFindPath(objtype*);
extern void T_DarkmonkLandAndFire(objtype*ob);
extern void T_DarkmonkCharge(objtype*ob);
extern void T_DarkmonkReact(objtype*ob);
extern void T_BossExplosions(objtype*ob);

statetype s_lowgrdstand	   = {TRUE,SPR_LOWGRD_S1,0,T_Stand,0,&s_lowgrdstand};


statetype s_lowgrdpath4    = {TRUE,SPR_LOWGRD_W41,12,T_Path,0,&s_lowgrdpath1};
statetype s_lowgrdpath3    = {TRUE,SPR_LOWGRD_W31,12,T_Path,SF_CLOSE,&s_lowgrdpath4};
statetype s_lowgrdpath2    = {TRUE,SPR_LOWGRD_W21,12,T_Path,0,&s_lowgrdpath3};
statetype s_lowgrdpath1    = {TRUE,SPR_LOWGRD_W11,12,T_Path,SF_CLOSE,&s_lowgrdpath2};

statetype s_lowgrdcollide  = {FALSE,SPR_LOWGRD_PAIN1,0,T_Collide,0,&s_lowgrdcollide};
statetype s_lowgrdcollide2 = {FALSE,SPR_LOWGRD_PAIN2,0,T_Collide,0,&s_lowgrdcollide2};


//statetype s_lowgrduse2     = {TRUE,SPR_LOWGRD_USE21,5,T_Use,0,&s_lowgrdpath1};
//statetype s_lowgrduse1     = {TRUE,SPR_LOWGRD_USE11,5,T_Use,0,&s_lowgrduse2};

statetype s_lowgrdshoot4 	= {FALSE,SPR_LOWGRD_SHOOT4,8,ActorMovement,0,&s_lowgrdchase1};
statetype s_lowgrdshoot3   = {FALSE,SPR_LOWGRD_SHOOT3,5,A_Shoot,0,&s_lowgrdshoot4};
statetype s_lowgrdshoot2   = {FALSE,SPR_LOWGRD_SHOOT2,20,ActorMovement,0,&s_lowgrdshoot3};
statetype s_lowgrdshoot1 	= {FALSE,SPR_LOWGRD_SHOOT1,6,ActorMovement,0,&s_lowgrdshoot2};

statetype s_lowgrdchase4   = {TRUE,SPR_LOWGRD_W41,12,T_Chase,0,&s_lowgrdchase1};
statetype s_lowgrdchase3   = {TRUE,SPR_LOWGRD_W31,12,T_Chase,SF_CLOSE,&s_lowgrdchase4};
statetype s_lowgrdchase2   = {TRUE,SPR_LOWGRD_W21,12,T_Chase,0,&s_lowgrdchase3};
statetype s_lowgrdchase1   = {TRUE,SPR_LOWGRD_W11,12,T_Chase,SF_CLOSE,&s_lowgrdchase2};



statetype s_lowgrddead     = {FALSE,SPR_LOWGRD_DEAD,0,T_Collide,0,&s_lowgrddead};
statetype s_lowgrddie4     = {FALSE,SPR_LOWGRD_DIE4,8,T_Collide,0,&s_lowgrddead};
statetype s_lowgrddie3     = {FALSE,SPR_LOWGRD_DIE3,8,T_Collide,0,&s_lowgrddie4};
statetype s_lowgrddie2     = {FALSE,SPR_LOWGRD_DIE2,8,T_Collide,0,&s_lowgrddie3};
statetype s_lowgrddie1		= {FALSE,SPR_LOWGRD_DIE1,15,T_Collide,0,&s_lowgrddie2};


statetype s_lowgrdcrushed2		= {FALSE,SPR_LOWGRD_DIE3,2,NULL,0,&s_lowgrddead};
statetype s_lowgrdcrushed1		= {FALSE,SPR_LOWGRD_DIE1,2,NULL,0,&s_lowgrdcrushed2};

/************** Sneaky stuff *******************************************/

statetype s_sneakydown     = {FALSE,SPR_SNEAKY_DEAD,0,T_Stand,0,&s_sneakydown};
statetype s_sneakyrise4    = {FALSE,SPR_RISE4,6,ActorMovement,0,&s_lowgrdchase1};
statetype s_sneakyrise3    = {FALSE,SPR_RISE3,6,ActorMovement,0,&s_sneakyrise4};
statetype s_sneakyrise2    = {FALSE,SPR_RISE2,6,ActorMovement,0,&s_sneakyrise3};
statetype s_sneakyrise1    = {FALSE,SPR_RISE1,6,ActorMovement,0,&s_sneakyrise2};


/*
=============================================================================

					  HIGH GUARD

=============================================================================
*/



statetype s_highgrdstand	= {TRUE,SPR_HIGHGRD_S1,0,T_Stand,0,&s_highgrdstand};


statetype s_highgrdpath4   = {TRUE,SPR_HIGHGRD_W41,12,T_Path,0,&s_highgrdpath1};
statetype s_highgrdpath3   = {TRUE,SPR_HIGHGRD_W31,12,T_Path,SF_CLOSE,&s_highgrdpath4};
statetype s_highgrdpath2   = {TRUE,SPR_HIGHGRD_W21,12,T_Path,0,&s_highgrdpath3};
statetype s_highgrdpath1   = {TRUE,SPR_HIGHGRD_W11,12,T_Path,SF_CLOSE,&s_highgrdpath2};


//statetype s_highgrdpain1 	= {2,SPR_HIGHGRD_PAIN1,10,T_Collide,0,&s_highgrdchase1};
//statetype s_highgrdpain2 	= {2,SPR_HIGHGRD_PAIN2,10,T_Collide,0,&s_highgrdchase1};


statetype s_highgrdcollide  = {FALSE,SPR_HIGHGRD_PAIN1,0,T_Collide,0,&s_highgrdcollide};
statetype s_highgrdcollide2 = {FALSE,SPR_HIGHGRD_PAIN2,0,T_Collide,0,&s_highgrdcollide2};


//statetype s_highgrduse2 	 = {TRUE,SPR_HIGHGRD_USE21,10,T_Use,0,&s_highgrdpath1};
//statetype s_highgrduse1 	 = {TRUE,SPR_HIGHGRD_USE11,10,T_Use,0,&s_highgrduse2};


statetype s_highgrdshoot4  = {FALSE,SPR_HIGHGRD_SHOOT4,3,A_Repeat,0,&s_highgrdshoot3};
statetype s_highgrdshoot3  = {FALSE,SPR_HIGHGRD_SHOOT3,5,A_Shoot,0,&s_highgrdshoot4};
statetype s_highgrdshoot2  = {FALSE,SPR_HIGHGRD_SHOOT2,6,ActorMovement,0,&s_highgrdshoot3};
statetype s_highgrdshoot1 	= {FALSE,SPR_HIGHGRD_SHOOT1,6,ActorMovement,0,&s_highgrdshoot2};


statetype s_highgrdchase4  = {TRUE,SPR_HIGHGRD_W41,12,T_Chase,0,&s_highgrdchase1};
statetype s_highgrdchase3 = {TRUE,SPR_HIGHGRD_W31,12,T_Chase,SF_CLOSE,&s_highgrdchase4};
statetype s_highgrdchase2 = {TRUE,SPR_HIGHGRD_W21,12,T_Chase,0,&s_highgrdchase3};
statetype s_highgrdchase1 = {TRUE,SPR_HIGHGRD_W11,12,T_Chase,SF_CLOSE,&s_highgrdchase2};


statetype s_highgrddead    = {FALSE,SPR_HIGHGRD_DEAD,0,T_Collide,0,&s_highgrddead};
statetype s_highgrddie5    = {FALSE,SPR_HIGHGRD_DIE5,0,T_Collide,0,&s_highgrddead};
statetype s_highgrddie4    = {FALSE,SPR_HIGHGRD_DIE4,8,T_Collide,0,&s_highgrddie5};
statetype s_highgrddie3    = {FALSE,SPR_HIGHGRD_DIE3,8,T_Collide,0,&s_highgrddie4};
statetype s_highgrddie2    = {FALSE,SPR_HIGHGRD_DIE2,8,T_Collide,0,&s_highgrddie3};
statetype s_highgrddie1    = {FALSE,SPR_HIGHGRD_DIE1,8,T_Collide,0,&s_highgrddie2};

statetype s_highgrdcrushed2		= {FALSE,SPR_HIGHGRD_DIE3,2,NULL,0,&s_highgrddead};
statetype s_highgrdcrushed1		= {FALSE,SPR_HIGHGRD_DIE1,2,NULL,0,&s_highgrdcrushed2};

/*
===========================================================================

										 STRIKEGUARD

===========================================================================
*/


statetype s_strikestand	= {TRUE,SPR_STRIKE_S1,0,T_Stand,0,&s_strikestand};


statetype s_strikepath4    = {TRUE,SPR_STRIKE_W41,12,T_Path,0,&s_strikepath1};
statetype s_strikepath3    = {TRUE,SPR_STRIKE_W31,12,T_Path,SF_CLOSE,&s_strikepath4};
statetype s_strikepath2    = {TRUE,SPR_STRIKE_W21,12,T_Path,0,&s_strikepath3};
statetype s_strikepath1    = {TRUE,SPR_STRIKE_W11,12,T_Path,SF_CLOSE,&s_strikepath2};

statetype s_strikecollide  = {FALSE,SPR_STRIKE_PAIN1,0,T_Collide,0,&s_strikecollide};
statetype s_strikecollide2 = {FALSE,SPR_STRIKE_PAIN2,0,T_Collide,0,&s_strikecollide2};

statetype s_strikeshoot4 	= {FALSE,SPR_STRIKE_SHOOT4,6,ActorMovement,0,&s_strikechase1};
statetype s_strikeshoot3 	= {FALSE,SPR_STRIKE_SHOOT3,6,ActorMovement,0,&s_strikeshoot4};
statetype s_strikeshoot2   = {FALSE,SPR_STRIKE_SHOOT2,5,A_Shoot,0,&s_strikeshoot3};
statetype s_strikeshoot1   = {FALSE,SPR_STRIKE_SHOOT1,20,ActorMovement,0,&s_strikeshoot2};


//statetype s_strikeuse2      = {TRUE,SPR_STRIKE_USE21,10,T_Use,0,&s_strikechase1};
//statetype s_strikeuse1      = {TRUE,SPR_STRIKE_USE11,10,NULL,0,&s_strikeuse2};

statetype s_strikewait      = {TRUE,SPR_STRIKE_S1,35,NULL,0,&s_strikechase1};

statetype s_strikerollright6    = {FALSE,SPR_STRIKE_LROLL6,8,T_Roll,0,&s_strikeshoot1};
statetype s_strikerollright5    = {FALSE,SPR_STRIKE_LROLL5,8,T_Roll,0,&s_strikerollright6};
statetype s_strikerollright4    = {FALSE,SPR_STRIKE_LROLL4,7,T_Reset,0,&s_strikerollright5};
statetype s_strikerollright3    = {FALSE,SPR_STRIKE_LROLL3,7,T_Roll,0,&s_strikerollright4};
statetype s_strikerollright2    = {FALSE,SPR_STRIKE_LROLL2,7,T_Roll,0,&s_strikerollright3};
statetype s_strikerollright1    = {FALSE,SPR_STRIKE_LROLL1,5,T_Roll,0,&s_strikerollright2};



statetype s_strikerollleft6    = {FALSE,SPR_STRIKE_RROLL6,8,T_Roll,0,&s_strikeshoot1};
statetype s_strikerollleft5    = {FALSE,SPR_STRIKE_RROLL5,8,T_Roll,0,&s_strikerollleft6};
statetype s_strikerollleft4    = {FALSE,SPR_STRIKE_RROLL4,7,T_Reset,0,&s_strikerollleft5};
statetype s_strikerollleft3    = {FALSE,SPR_STRIKE_RROLL3,7,T_Roll,0,&s_strikerollleft4};
statetype s_strikerollleft2    = {FALSE,SPR_STRIKE_RROLL2,7,T_Roll,0,&s_strikerollleft3};
statetype s_strikerollleft1    = {FALSE,SPR_STRIKE_RROLL1,5,T_Roll,0,&s_strikerollleft2};


statetype s_strikechase4   = {TRUE,SPR_STRIKE_W41,12,T_Chase,0,&s_strikechase1};
statetype s_strikechase3   = {TRUE,SPR_STRIKE_W31,12,T_Chase,SF_CLOSE,&s_strikechase4};
statetype s_strikechase2   = {TRUE,SPR_STRIKE_W21,12,T_Chase,0,&s_strikechase3};
statetype s_strikechase1   = {TRUE,SPR_STRIKE_W11,12,T_Chase,SF_CLOSE,&s_strikechase2};


statetype s_strikedead3    = {FALSE,SPR_STRIKE_DEAD3,0,T_Collide,0,&s_strikedead3};
statetype s_strikedead2    = {FALSE,SPR_STRIKE_DEAD2,0,T_Collide,0,&s_strikedead3};
statetype s_strikedead     = {FALSE,SPR_STRIKE_DEAD1,7,T_Collide,0,&s_strikedead2};
statetype s_strikedie4     = {FALSE,SPR_STRIKE_DIE4,7,T_Collide,0,&s_strikedead};
statetype s_strikedie3     = {FALSE,SPR_STRIKE_DIE3,7,T_Collide,0,&s_strikedie4};
statetype s_strikedie2     = {FALSE,SPR_STRIKE_DIE2,7,T_Collide,0,&s_strikedie3};
statetype s_strikedie1     = {FALSE,SPR_STRIKE_DIE1,7,T_Collide,0,&s_strikedie2};


statetype s_strikecrushed2		= {FALSE,SPR_STRIKE_DIE3,2,NULL,0,&s_strikedead};
statetype s_strikecrushed1		= {FALSE,SPR_STRIKE_DIE1,2,NULL,0,&s_strikecrushed2};

/*
============================================================================

								LIGHTNING GUARD

============================================================================
*/

statetype s_blitzstand	= {TRUE,SPR_BLITZ_S1,0,T_Stand,0,&s_blitzstand};

statetype s_blitzpath4 = {TRUE,SPR_BLITZ_W41,12,T_Path,0,&s_blitzpath1};
statetype s_blitzpath3 = {TRUE,SPR_BLITZ_W31,12,T_Path,SF_CLOSE,&s_blitzpath4};
statetype s_blitzpath2 = {TRUE,SPR_BLITZ_W21,12,T_Path,0,&s_blitzpath3};
statetype s_blitzpath1 = {TRUE,SPR_BLITZ_W11,12,T_Path,SF_CLOSE,&s_blitzpath2};



statetype s_blitzcollide  = {FALSE,SPR_BLITZ_PAIN1,0,T_Collide,0,&s_blitzcollide};
statetype s_blitzcollide2 = {FALSE,SPR_BLITZ_PAIN2,0,T_Collide,0,&s_blitzcollide2};

statetype s_blitzshoot4 = {FALSE,SPR_BLITZ_SHOOT4,10,ActorMovement,0,&s_blitzchase1};
statetype s_blitzshoot3 = {FALSE,SPR_BLITZ_SHOOT3,10,ActorMovement,0,&s_blitzshoot4};
statetype s_blitzshoot2 = {FALSE,SPR_BLITZ_SHOOT2,5,A_Shoot,0,&s_blitzshoot3};
statetype s_blitzshoot1 = {FALSE,SPR_BLITZ_SHOOT1,10,ActorMovement,0,&s_blitzshoot2};

statetype s_blitzrise4 = {FALSE,SPR_BLITZ_RISE4,8,ActorMovement,0,&s_blitzchase1};
statetype s_blitzrise3 = {FALSE,SPR_BLITZ_RISE3,8,ActorMovement,0,&s_blitzrise4};
statetype s_blitzrise2 = {FALSE,SPR_BLITZ_RISE2,8,ActorMovement,0,&s_blitzrise3};
statetype s_blitzrise1 = {FALSE,SPR_BLITZ_RISE1,8,ActorMovement,0,&s_blitzrise2};

statetype s_blitzuse    = {TRUE,SPR_BLITZ_USE,10,T_Use,0,&s_blitzpath1};


statetype s_blitzsteal2 = {TRUE,SPR_BLITZ_STEAL2,20,A_Steal,0,&s_blitzchase1};
statetype s_blitzsteal1 = {TRUE,SPR_BLITZ_STEAL1,20,ActorMovement,0,&s_blitzsteal2};

statetype s_blitzchase4    = {TRUE,SPR_BLITZ_W41,12,T_Chase,0,&s_blitzchase1};
statetype s_blitzchase3    = {TRUE,SPR_BLITZ_W31,12,T_Chase,SF_CLOSE,&s_blitzchase4};
statetype s_blitzchase2    = {TRUE,SPR_BLITZ_W21,12,T_Chase,0,&s_blitzchase3};
statetype s_blitzchase1    = {TRUE,SPR_BLITZ_W11,12,T_Chase,SF_CLOSE,&s_blitzchase2};


statetype s_blitzdead2     = {FALSE,SPR_BLITZ_DEAD2,0,T_Collide,0,&s_blitzdead2};
statetype s_blitzdead      = {FALSE,SPR_BLITZ_DEAD1,0,T_Collide,0,&s_blitzdead2};
statetype s_blitzdie4      = {FALSE,SPR_BLITZ_DIE4,7,T_Collide,0,&s_blitzdead};
statetype s_blitzdie3      = {FALSE,SPR_BLITZ_DIE3,7,T_Collide,0,&s_blitzdie4};
statetype s_blitzdie2      = {FALSE,SPR_BLITZ_DIE2,7,T_Collide,0,&s_blitzdie3};
statetype s_blitzdie1      = {FALSE,SPR_BLITZ_DIE1,7,T_Collide,0,&s_blitzdie2};


statetype s_blitzstruggledead  = {FALSE,SPR_BLITZ_RISE2,0,T_Collide,0,&s_blitzstruggledead};
statetype s_blitzstruggledie1  = {FALSE,SPR_BLITZ_RISE2,20,T_ReallyDead,0,&s_blitzstruggledead};


statetype s_blitzfakedead  = {FALSE,SPR_BLITZ_DEAD2,0,T_PlayDead,SF_FAKING,&s_blitzfakedead};
statetype s_blitzfakedie3  = {FALSE,SPR_BLITZ_DEAD1,5,ActorMovement,0,&s_blitzfakedead};
statetype s_blitzfakedie2  = {FALSE,SPR_BLITZ_DIE4,5,ActorMovement,0,&s_blitzfakedie3};
statetype s_blitzfakedie1  = {FALSE,SPR_BLITZ_DIE3,5,ActorMovement,0,&s_blitzfakedie2};


statetype s_blitzcrushed2     = {FALSE,SPR_BLITZ_DIE3,2,NULL,0,&s_blitzdead};
statetype s_blitzcrushed1		= {FALSE,SPR_BLITZ_DIE1,2,NULL,0,&s_blitzcrushed2};

statetype s_blitzplead11    = {FALSE,SPR_BLITZ_PLEAD11,0,T_Collide,0,&s_blitzplead11};
statetype s_blitzplead10    = {FALSE,SPR_BLITZ_PLEAD10,6,NULL,0,&s_blitzplead11};
statetype s_blitzplead9    = {FALSE,SPR_BLITZ_PLEAD9,6,NULL,0,&s_blitzplead10};
statetype s_blitzplead8    = {FALSE,SPR_BLITZ_PLEAD8,6,NULL,0,&s_blitzplead9};
statetype s_blitzplead7    = {FALSE,SPR_BLITZ_PLEAD7,6,NULL,0,&s_blitzplead8};




statetype s_blitzaplead4    = {FALSE,SPR_BLITZ_PLEAD4,5,T_Plead,SF_DOWN,&s_blitzplead3};
statetype s_blitzaplead5    = {FALSE,SPR_BLITZ_PLEAD5,5,T_Plead,SF_DOWN,&s_blitzaplead4};

statetype s_blitzplead6    = {FALSE,SPR_BLITZ_PLEAD6,5,T_Plead,SF_DOWN,&s_blitzaplead5};
statetype s_blitzplead5    = {FALSE,SPR_BLITZ_PLEAD5,5,T_Plead,SF_DOWN,&s_blitzplead6};
statetype s_blitzplead4    = {FALSE,SPR_BLITZ_PLEAD4,5,T_Plead,SF_DOWN,&s_blitzplead5};
statetype s_blitzplead3    = {FALSE,SPR_BLITZ_PLEAD3,5,T_Plead,SF_DOWN,&s_blitzplead4};
statetype s_blitzplead2    = {FALSE,SPR_BLITZ_PLEAD2,6,ActorMovement,0,&s_blitzplead3};
statetype s_blitzplead1    = {FALSE,SPR_BLITZ_PLEAD1,6,ActorMovement,0,&s_blitzplead2};



/*
============================================================================

								TRIAD ENFORCERS

============================================================================
*/


statetype s_enforcerstand	= {TRUE,SPR_ENFORCER_S1,0,T_Stand,0,&s_enforcerstand};


statetype s_enforcerpath4  = {TRUE,SPR_ENFORCER_W41,12,T_Path,0,&s_enforcerpath1};
statetype s_enforcerpath3  = {TRUE,SPR_ENFORCER_W31,12,T_Path,SF_CLOSE,&s_enforcerpath4};
statetype s_enforcerpath2  = {TRUE,SPR_ENFORCER_W21,12,T_Path,0,&s_enforcerpath3};
statetype s_enforcerpath1  = {TRUE,SPR_ENFORCER_W11,12,T_Path,SF_CLOSE,&s_enforcerpath2};


statetype s_enforcerchase4    = {TRUE,SPR_ENFORCER_W41,12,T_Chase,0,&s_enforcerchase1};
statetype s_enforcerchase3    = {TRUE,SPR_ENFORCER_W31,12,T_Chase,SF_CLOSE,&s_enforcerchase4};
statetype s_enforcerchase2    = {TRUE,SPR_ENFORCER_W21,12,T_Chase,0,&s_enforcerchase3};
statetype s_enforcerchase1    = {TRUE,SPR_ENFORCER_W11,12,T_Chase,SF_CLOSE,&s_enforcerchase2};

statetype s_enforcercollide  = {FALSE,SPR_ENFORCER_PAIN1,0,T_Collide,0,&s_enforcercollide};
statetype s_enforcercollide2 = {FALSE,SPR_ENFORCER_PAIN2,0,T_Collide,0,&s_enforcercollide2};


//statetype s_enforceruse2    = {TRUE,SPR_ENFORCER_USE21,10,T_Use,0,&s_enforcerchase1};
//statetype s_enforceruse1    = {TRUE,SPR_ENFORCER_USE11,10,T_Use,0,&s_enforceruse2};

statetype s_enforcershoot4 = {FALSE,SPR_ENFORCER_SHOOT4,6,A_Repeat,0,&s_enforcershoot3};
statetype s_enforcershoot3 = {FALSE,SPR_ENFORCER_SHOOT3,6,A_Shoot,0,&s_enforcershoot4};
statetype s_enforcershoot2 = {FALSE,SPR_ENFORCER_SHOOT2,6,ActorMovement,0,&s_enforcershoot3};
statetype s_enforcershoot1 = {FALSE,SPR_ENFORCER_SHOOT1,6,ActorMovement,0,&s_enforcershoot2};


statetype s_enforcerthrow8 = {FALSE,SPR_ENFORCER_THROW8,10,ActorMovement,0,&s_enforcerchase1};
statetype s_enforcerthrow7 = {FALSE,SPR_ENFORCER_THROW7,10,ActorMovement,0,&s_enforcerthrow8};
statetype s_enforcerthrow6 = {FALSE,SPR_ENFORCER_THROW6,10,ActorMovement,0,&s_enforcerthrow7};
statetype s_enforcerthrow5 = {FALSE,SPR_ENFORCER_THROW5,10,A_MissileWeapon,0,&s_enforcerthrow6};
statetype s_enforcerthrow4 = {FALSE,SPR_ENFORCER_THROW4,10,ActorMovement,0,&s_enforcerthrow5};
statetype s_enforcerthrow3 = {FALSE,SPR_ENFORCER_THROW3,10,ActorMovement,0,&s_enforcerthrow4};
statetype s_enforcerthrow2 = {FALSE,SPR_ENFORCER_THROW2,10,ActorMovement,0,&s_enforcerthrow3};
statetype s_enforcerthrow1 = {FALSE,SPR_ENFORCER_THROW1,10,ActorMovement,0,&s_enforcerthrow2};



statetype s_grenade10      = {FALSE,SPR_ENFORCER_GR10,2,T_Projectile,0,&s_grenade1};
statetype s_grenade9       = {FALSE,SPR_ENFORCER_GR9,2,T_Projectile,0,&s_grenade10};
statetype s_grenade8       = {FALSE,SPR_ENFORCER_GR8,2,T_Projectile,0,&s_grenade9};
statetype s_grenade7       = {FALSE,SPR_ENFORCER_GR7,2,T_Projectile,0,&s_grenade8};
statetype s_grenade6       = {FALSE,SPR_ENFORCER_GR6,2,T_Projectile,0,&s_grenade7};
statetype s_grenade5       = {FALSE,SPR_ENFORCER_GR5,2,T_Projectile,0,&s_grenade6};
statetype s_grenade4       = {FALSE,SPR_ENFORCER_GR4,2,T_Projectile,0,&s_grenade5};
statetype s_grenade3       = {FALSE,SPR_ENFORCER_GR3,2,T_Projectile,0,&s_grenade4};
statetype s_grenade2       = {FALSE,SPR_ENFORCER_GR2,2,T_Projectile,0,&s_grenade3};
statetype s_grenade1       = {FALSE,SPR_ENFORCER_GR1,2,T_Projectile,0,&s_grenade2};

statetype s_grenade_fall6  = {FALSE,SPR_ENFORCER_FALL6,6,T_Projectile,0,&s_grenade_fall6};
statetype s_grenade_fall5  = {FALSE,SPR_ENFORCER_FALL5,6,T_Projectile,0,&s_grenade_fall6};
statetype s_grenade_fall4  = {FALSE,SPR_ENFORCER_FALL4,6,T_Projectile,0,&s_grenade_fall5};
statetype s_grenade_fall3  = {FALSE,SPR_ENFORCER_FALL3,6,T_Projectile,0,&s_grenade_fall4};
statetype s_grenade_fall2  = {FALSE,SPR_ENFORCER_FALL1,6,T_Projectile,0,&s_grenade_fall3};
statetype s_grenade_fall1  = {FALSE,SPR_ENFORCER_FALL1,6,T_Projectile,0,&s_grenade_fall2};

statetype s_grenadehit3     = {FALSE,SPR_GRENADE_HIT3,5,NULL,0,NULL};
statetype s_grenadehit2     = {FALSE,SPR_GRENADE_HIT2,5,NULL,0,&s_grenadehit3};
statetype s_grenadehit1     = {FALSE,SPR_GRENADE_HIT1,5,NULL,0,&s_grenadehit2};


statetype s_enforcerdead      = {FALSE,SPR_ENFORCER_DEAD,0,T_Collide,0,&s_enforcerdead};
statetype s_enforcerdie4      = {FALSE,SPR_ENFORCER_DIE4,7,T_Collide,0,&s_enforcerdead};
statetype s_enforcerdie3      = {FALSE,SPR_ENFORCER_DIE3,7,T_Collide,0,&s_enforcerdie4};
statetype s_enforcerdie2      = {FALSE,SPR_ENFORCER_DIE2,7,T_Collide,0,&s_enforcerdie3};
statetype s_enforcerdie1      = {FALSE,SPR_ENFORCER_DIE1,7,T_Collide,0,&s_enforcerdie2};



statetype s_enforcercrushed2		= {FALSE,SPR_ENFORCER_DIE3,2,NULL,0,&s_enforcerdead};
statetype s_enforcercrushed1		= {FALSE,SPR_ENFORCER_DIE1,2,NULL,0,&s_enforcercrushed2};

/*
============================================================================

								 ROBOT GUARD

============================================================================
*/



statetype s_robogrdstand	= {16,SPR_ROBOGRD_S11,0,T_Stand,0,&s_robogrdstand};

statetype s_robogrdpath1 	= {16,SPR_ROBOGRD_S11,20,T_AutoPath,0,&s_robogrdpath1};

statetype s_robowait = {16,SPR_ROBOGRD_S11,0,NULL,0,&s_robowait};
statetype s_roborealign = {16,SPR_ROBOGRD_S11,0,T_AutoRealign,0,&s_roborealign};
statetype s_roboalign = {16,SPR_ROBOGRD_S11,0,T_AutoShootAlign,0,&s_roboalign};

statetype s_robogrdshoot1 	= {FALSE,SPR_ROBOGRD_S11,35,A_MissileWeapon,0,&s_roborealign};

statetype s_robogrdshuriken4 	= {FALSE,SPR_ROBOGRD_SHURIKEN4,4,T_Projectile,0,&s_robogrdshuriken1};
statetype s_robogrdshuriken3 	= {FALSE,SPR_ROBOGRD_SHURIKEN3,4,T_Projectile,0,&s_robogrdshuriken4};
statetype s_robogrdshuriken2 	= {FALSE,SPR_ROBOGRD_SHURIKEN2,4,T_Projectile,0,&s_robogrdshuriken3};
statetype s_robogrdshuriken1  = {FALSE,SPR_ROBOGRD_SHURIKEN1,4,T_Projectile,0,&s_robogrdshuriken2};

statetype s_shurikenhit3    = {FALSE,SPR_SHURIKEN_HIT3,5,NULL,0,NULL};
statetype s_shurikenhit2    = {FALSE,SPR_SHURIKEN_HIT2,5,NULL,0,&s_shurikenhit3};
statetype s_shurikenhit1    = {FALSE,SPR_SHURIKEN_HIT1,5,NULL,0,&s_shurikenhit2};

statetype s_robogrdcollide  = {16,SPR_ROBOGRD_S11,0,T_Collide,0,&s_robogrdcollide};
statetype s_robogrdcollide2 = {16,SPR_ROBOGRD_S11,0,T_Collide,0,&s_robogrdcollide2};

//statetype s_robogrdchase1 	= {16,SPR_ROBOGRD_S11,10,T_RoboChase,0,&s_robogrdchase1};

statetype s_robogrddead		= {FALSE,ROBOGRDDEAD,0,T_Collide,0,&s_robogrddead};
statetype s_robogrddie9	   = {FALSE,ROBOGRDDIE9,3,T_Collide,0,&s_robogrddead};
statetype s_robogrddie8	   = {FALSE,ROBOGRDDIE8,3,T_Collide,0,&s_robogrddie9};
statetype s_robogrddie7	   = {FALSE,ROBOGRDDIE7,3,T_Collide,0,&s_robogrddie8};
statetype s_robogrddie6	   = {FALSE,ROBOGRDDIE6,3,T_Collide,0,&s_robogrddie7};
statetype s_robogrddie5	   = {FALSE,ROBOGRDDIE5,3,T_Collide,0,&s_robogrddie6};
statetype s_robogrddie4	   = {FALSE,ROBOGRDDIE4,3,T_Collide,0,&s_robogrddie5};
statetype s_robogrddie3	   = {FALSE,ROBOGRDDIE3,3,T_Collide,0,&s_robogrddie4};
statetype s_robogrddie2	   = {FALSE,ROBOGRDDIE2,3,T_Collide,0,&s_robogrddie3};
statetype s_robogrddie1	   = {FALSE,ROBOGRDDIE1,3,T_Collide,0,&s_robogrddie2};


statetype s_bstar4 = {FALSE,SPR_BSTAR4,1,T_Projectile,0,&s_bstar1};
statetype s_bstar3 = {FALSE,SPR_BSTAR3,1,T_Projectile,0,&s_bstar4};
statetype s_bstar2 = {FALSE,SPR_BSTAR2,1,T_Projectile,0,&s_bstar3};
statetype s_bstar1 = {FALSE,SPR_BSTAR1,1,T_Projectile,0,&s_bstar2};



/*
==========================================================================
=
=                             Explosions
=
==========================================================================
*/

statetype s_altexplosion10 = {FALSE,SPR_EXPLOSION19,3,NULL,0,NULL};
statetype s_altexplosion9 = {FALSE,SPR_EXPLOSION17,3,NULL,0,&s_altexplosion10};
statetype s_altexplosion8 = {FALSE,SPR_EXPLOSION15,3,NULL,0,&s_altexplosion9};
statetype s_altexplosion7 = {FALSE,SPR_EXPLOSION13,3,NULL,0,&s_altexplosion8};
statetype s_altexplosion6 = {FALSE,SPR_EXPLOSION11,3,NULL,0,&s_altexplosion7};
statetype s_altexplosion5 = {FALSE,SPR_EXPLOSION9,3,NULL,0,&s_altexplosion6};
statetype s_altexplosion4 = {FALSE,SPR_EXPLOSION7,3,NULL,0,&s_altexplosion5};
statetype s_altexplosion3 = {FALSE,SPR_EXPLOSION5,3,NULL,0,&s_altexplosion4};
statetype s_altexplosion2 = {FALSE,SPR_EXPLOSION3,3,T_Explosion,0,&s_altexplosion3};
statetype s_altexplosion1 = {FALSE,SPR_EXPLOSION1,3,NULL,0,&s_altexplosion2};





statetype s_explosion20 = {FALSE,SPR_EXPLOSION20,2,NULL,0,NULL};
statetype s_explosion19 = {FALSE,SPR_EXPLOSION19,2,NULL,0,&s_explosion20};
statetype s_explosion18 = {FALSE,SPR_EXPLOSION18,2,NULL,0,&s_explosion19};
statetype s_explosion17 = {FALSE,SPR_EXPLOSION17,2,NULL,0,&s_explosion18};
statetype s_explosion16 = {FALSE,SPR_EXPLOSION16,2,NULL,0,&s_explosion17};
statetype s_explosion15 = {FALSE,SPR_EXPLOSION15,2,NULL,0,&s_explosion16};
statetype s_explosion14 = {FALSE,SPR_EXPLOSION14,2,NULL,0,&s_explosion15};
statetype s_explosion13 = {FALSE,SPR_EXPLOSION13,2,NULL,0,&s_explosion14};
statetype s_explosion12 = {FALSE,SPR_EXPLOSION12,2,NULL,0,&s_explosion13};
statetype s_explosion11 = {FALSE,SPR_EXPLOSION11,2,NULL,0,&s_explosion12};
statetype s_explosion10 = {FALSE,SPR_EXPLOSION10,2,NULL,0,&s_explosion11};
statetype s_explosion9 = {FALSE,SPR_EXPLOSION9,2,NULL,0,&s_explosion10};
statetype s_explosion8 = {FALSE,SPR_EXPLOSION8,2,NULL,0,&s_explosion9};
statetype s_explosion7 = {FALSE,SPR_EXPLOSION7,2,NULL,0,&s_explosion8};
statetype s_explosion6 = {FALSE,SPR_EXPLOSION6,2,NULL,0,&s_explosion7};
statetype s_explosion5 = {FALSE,SPR_EXPLOSION5,2,NULL,0,&s_explosion6};
statetype s_explosion4 = {FALSE,SPR_EXPLOSION4,2,NULL,0,&s_explosion5};
statetype s_explosion3 = {FALSE,SPR_EXPLOSION3,2,T_Explosion,0,&s_explosion4};
statetype s_explosion2 = {FALSE,SPR_EXPLOSION2,2,NULL,0,&s_explosion3};
statetype s_explosion1 = {FALSE,SPR_EXPLOSION1,2,NULL,0,&s_explosion2};



statetype s_grexplosion20 = {FALSE,SPR_GROUNDEXPL20,2,T_Special,0,NULL};
statetype s_grexplosion19 = {FALSE,SPR_GROUNDEXPL19,2,NULL,0,&s_grexplosion20};
statetype s_grexplosion18 = {FALSE,SPR_GROUNDEXPL18,2,NULL,0,&s_grexplosion19};
statetype s_grexplosion17 = {FALSE,SPR_GROUNDEXPL17,2,NULL,0,&s_grexplosion18};
statetype s_grexplosion16 = {FALSE,SPR_GROUNDEXPL16,2,NULL,0,&s_grexplosion17};
statetype s_grexplosion15 = {FALSE,SPR_GROUNDEXPL15,2,NULL,0,&s_grexplosion16};
statetype s_grexplosion14 = {FALSE,SPR_GROUNDEXPL14,2,NULL,0,&s_grexplosion15};
statetype s_grexplosion13 = {FALSE,SPR_GROUNDEXPL13,2,NULL,0,&s_grexplosion14};
statetype s_grexplosion12 = {FALSE,SPR_GROUNDEXPL12,2,NULL,0,&s_grexplosion13};
statetype s_grexplosion11 = {FALSE,SPR_GROUNDEXPL11,2,NULL,0,&s_grexplosion12};
statetype s_grexplosion10 = {FALSE,SPR_GROUNDEXPL10,2,NULL,0,&s_grexplosion11};
statetype s_grexplosion9 = {FALSE,SPR_GROUNDEXPL9,2,NULL,0,&s_grexplosion10};
statetype s_grexplosion8 = {FALSE,SPR_GROUNDEXPL8,2,NULL,0,&s_grexplosion9};
statetype s_grexplosion7 = {FALSE,SPR_GROUNDEXPL7,2,NULL,0,&s_grexplosion8};
statetype s_grexplosion6 = {FALSE,SPR_GROUNDEXPL6,2,NULL,0,&s_grexplosion7};
statetype s_grexplosion5 = {FALSE,SPR_GROUNDEXPL5,2,NULL,0,&s_grexplosion6};
statetype s_grexplosion4 = {FALSE,SPR_GROUNDEXPL4,2,NULL,0,&s_grexplosion5};
statetype s_grexplosion3 = {FALSE,SPR_GROUNDEXPL3,2,T_Explosion,0,&s_grexplosion4};
statetype s_grexplosion2 = {FALSE,SPR_GROUNDEXPL2,2,NULL,0,&s_grexplosion3};
statetype s_grexplosion1 = {FALSE,SPR_GROUNDEXPL1,2,NULL,0,&s_grexplosion2};



statetype s_staticexplosion25 = {FALSE,SPR_STATICEXPL25,2,NULL,0,NULL};
statetype s_staticexplosion24 = {FALSE,SPR_STATICEXPL24,2,NULL,0,&s_staticexplosion25};
statetype s_staticexplosion23 = {FALSE,SPR_STATICEXPL23,2,NULL,0,&s_staticexplosion24};
statetype s_staticexplosion22 = {FALSE,SPR_STATICEXPL22,2,NULL,0,&s_staticexplosion23};
statetype s_staticexplosion21 = {FALSE,SPR_STATICEXPL21,2,NULL,0,&s_staticexplosion22};
statetype s_staticexplosion20 = {FALSE,SPR_STATICEXPL20,2,NULL,0,&s_staticexplosion21};
statetype s_staticexplosion19 = {FALSE,SPR_STATICEXPL19,2,NULL,0,&s_staticexplosion20};
statetype s_staticexplosion18 = {FALSE,SPR_STATICEXPL18,2,NULL,0,&s_staticexplosion19};
statetype s_staticexplosion17 = {FALSE,SPR_STATICEXPL17,2,NULL,0,&s_staticexplosion18};
statetype s_staticexplosion16 = {FALSE,SPR_STATICEXPL16,2,NULL,0,&s_staticexplosion17};
statetype s_staticexplosion15 = {FALSE,SPR_STATICEXPL15,2,NULL,0,&s_staticexplosion16};
statetype s_staticexplosion14 = {FALSE,SPR_STATICEXPL14,2,NULL,0,&s_staticexplosion15};
statetype s_staticexplosion13 = {FALSE,SPR_STATICEXPL13,2,NULL,0,&s_staticexplosion14};
statetype s_staticexplosion12 = {FALSE,SPR_STATICEXPL12,2,NULL,0,&s_staticexplosion13};
statetype s_staticexplosion11 = {FALSE,SPR_STATICEXPL11,2,NULL,0,&s_staticexplosion12};
statetype s_staticexplosion10 = {FALSE,SPR_STATICEXPL10,2,NULL,0,&s_staticexplosion11};
statetype s_staticexplosion9 = {FALSE,SPR_STATICEXPL9,2,NULL,0,&s_staticexplosion10};
statetype s_staticexplosion8 = {FALSE,SPR_STATICEXPL8,2,NULL,0,&s_staticexplosion9};
statetype s_staticexplosion7 = {FALSE,SPR_STATICEXPL7,2,NULL,0,&s_staticexplosion8};
statetype s_staticexplosion6 = {FALSE,SPR_STATICEXPL6,2,NULL,0,&s_staticexplosion7};
statetype s_staticexplosion5 = {FALSE,SPR_STATICEXPL5,2,NULL,0,&s_staticexplosion6};
statetype s_staticexplosion4 = {FALSE,SPR_STATICEXPL4,2,NULL,0,&s_staticexplosion5};
statetype s_staticexplosion3 = {FALSE,SPR_STATICEXPL3,2,T_Explosion,0,&s_staticexplosion4};
statetype s_staticexplosion2 = {FALSE,SPR_STATICEXPL2,2,NULL,0,&s_staticexplosion3};
statetype s_staticexplosion1 = {FALSE,SPR_STATICEXPL1,2,NULL,0,&s_staticexplosion2};



statetype s_upblade16 = {FALSE,UBLADE8,1,T_Path,0,&s_upblade1};
statetype s_upblade15 = {FALSE,UBLADE7,2,T_Path,0,&s_upblade16};
statetype s_upblade14 = {FALSE,UBLADE6,1,T_Path,0,&s_upblade15};
statetype s_upblade13 = {FALSE,UBLADE5,2,T_Path,0,&s_upblade14};
statetype s_upblade12 = {FALSE,UBLADE4,1,T_Path,0,&s_upblade13};
statetype s_upblade11 = {FALSE,UBLADE3,2,T_Path,0,&s_upblade12};
statetype s_upblade10 = {FALSE,UBLADE2,1,T_Path,0,&s_upblade11};
statetype s_upblade9 = {FALSE,UBLADE9,2,T_Path,0,&s_upblade10};
statetype s_upblade8 = {FALSE,UBLADE8,1,T_Path,0,&s_upblade9};
statetype s_upblade7 = {FALSE,UBLADE7,2,T_Path,0,&s_upblade8};
statetype s_upblade6 = {FALSE,UBLADE6,1,T_Path,0,&s_upblade7};
statetype s_upblade5 = {FALSE,UBLADE5,2,T_Path,0,&s_upblade6};
statetype s_upblade4 = {FALSE,UBLADE4,1,T_Path,0,&s_upblade5};
statetype s_upblade3 = {FALSE,UBLADE3,2,T_Path,0,&s_upblade4};
statetype s_upblade2 = {FALSE,UBLADE2,1,T_Path,0,&s_upblade3};
statetype s_upblade1 = {FALSE,UBLADE1,2,T_Path,SF_SOUND,&s_upblade2};


statetype s_firejetup23 = {FALSE,FIREJETUP23,3,T_Path,SF_CRUSH,&s_firejetup1};
statetype s_firejetup22 = {FALSE,FIREJETUP22,3,T_Path,SF_CRUSH,&s_firejetup23};
statetype s_firejetup21 = {FALSE,FIREJETUP21,3,T_Path,0,&s_firejetup22};
statetype s_firejetup20 = {FALSE,FIREJETUP20,3,T_Path,0,&s_firejetup21};
statetype s_firejetup19 = {FALSE,FIREJETUP19,3,T_Path,0,&s_firejetup20};
statetype s_firejetup18 = {FALSE,FIREJETUP18,3,T_Path,0,&s_firejetup19};
statetype s_firejetup17 = {FALSE,FIREJETUP17,3,T_Path,0,&s_firejetup18};
statetype s_firejetup16 = {FALSE,FIREJETUP16,3,T_Path,SF_CRUSH,&s_firejetup17};
statetype s_firejetup15 = {FALSE,FIREJETUP15,3,T_Path,SF_CRUSH,&s_firejetup16};
statetype s_firejetup14 = {FALSE,FIREJETUP14,3,T_Path,SF_CRUSH,&s_firejetup15};
statetype s_firejetup13 = {FALSE,FIREJETUP13,3,T_Path,0,&s_firejetup14};
statetype s_firejetup12 = {FALSE,FIREJETUP12,3,T_Path,0,&s_firejetup13};
statetype s_firejetup11 = {FALSE,FIREJETUP11,3,T_Path,0,&s_firejetup12};
statetype s_firejetup10 = {FALSE,FIREJETUP10,3,T_Path,0,&s_firejetup11};
statetype s_firejetup9 = {FALSE,FIREJETUP9,3,T_Path,0,&s_firejetup10};
statetype s_firejetup8 = {FALSE,FIREJETUP8,3,T_Path,SF_CRUSH,&s_firejetup9};
statetype s_firejetup7 = {FALSE,FIREJETUP7,3,T_Path,SF_CRUSH,&s_firejetup8};
statetype s_firejetup6 = {FALSE,FIREJETUP6,3,T_Path,SF_CRUSH,&s_firejetup7};
statetype s_firejetup5 = {FALSE,FIREJETUP5,3,T_Path,0,&s_firejetup6};
statetype s_firejetup4 = {FALSE,FIREJETUP4,3,T_Path,0,&s_firejetup5};
statetype s_firejetup3 = {FALSE,FIREJETUP3,3,T_Path,0,&s_firejetup4};
statetype s_firejetup2 = {FALSE,FIREJETUP2,3,T_Path,0,&s_firejetup3};
statetype s_firejetup1 = {FALSE,FIREJETUP1,70,T_Path,SF_SOUND,&s_firejetup2};



statetype s_columndownup6 = {FALSE,CRUSHDOWN7,5,NULL,0,&s_columndowndown1};
statetype s_columndownup5 = {FALSE,CRUSHDOWN6,5,NULL,0,&s_columndownup6};
statetype s_columndownup4 = {FALSE,CRUSHDOWN5,5,NULL,SF_BLOCK,&s_columndownup5};
statetype s_columndownup3 = {FALSE,CRUSHDOWN4,5,NULL,SF_BLOCK,&s_columndownup4};
statetype s_columndownup2 = {FALSE,CRUSHDOWN3,5,NULL,SF_BLOCK,&s_columndownup3};
statetype s_columndownup1 = {FALSE,CRUSHDOWN2,5,NULL,SF_BLOCK,&s_columndownup2};


statetype s_columndowndown8 = {FALSE,CRUSHDOWN1,5,T_CrushDown,SF_CRUSH|SF_DOWN|SF_BLOCK,&s_columndownup1};
statetype s_columndowndown7 = {FALSE,CRUSHDOWN2,5,T_CrushDown,SF_CRUSH|SF_DOWN|SF_BLOCK,&s_columndowndown8};
statetype s_columndowndown6 = {FALSE,CRUSHDOWN3,5,T_CrushDown,SF_DOWN|SF_BLOCK,&s_columndowndown7};
statetype s_columndowndown5 = {FALSE,CRUSHDOWN4,5,T_CrushDown,SF_DOWN|SF_BLOCK,&s_columndowndown6};
statetype s_columndowndown4 = {FALSE,CRUSHDOWN5,5,T_CrushDown,SF_DOWN,&s_columndowndown5};
statetype s_columndowndown3 = {FALSE,CRUSHDOWN6,5,T_CrushDown,SF_DOWN,&s_columndowndown4};
statetype s_columndowndown2 = {FALSE,CRUSHDOWN7,5,T_CrushDown,SF_DOWN,&s_columndowndown3};
statetype s_columndowndown1 = {FALSE,CRUSHDOWN8,30,T_CrushDown,SF_DOWN|SF_SOUND,&s_columndowndown2};


statetype s_spearup16 = {FALSE,SPEARUP16,2,T_Spears,SF_CRUSH,&s_spearup1};
statetype s_spearup15 = {FALSE,SPEARUP15,2,T_Spears,SF_CRUSH,&s_spearup16};
statetype s_spearup14 = {FALSE,SPEARUP14,2,T_Spears,SF_CRUSH,&s_spearup15};
statetype s_spearup13 = {FALSE,SPEARUP13,2,T_Spears,SF_CRUSH,&s_spearup14};
statetype s_spearup12 = {FALSE,SPEARUP12,2,T_Spears,SF_CRUSH,&s_spearup13};
statetype s_spearup11 = {FALSE,SPEARUP11,2,T_Spears,SF_CRUSH,&s_spearup12};
statetype s_spearup10 = {FALSE,SPEARUP10,2,T_Spears,SF_CRUSH,&s_spearup11};
statetype s_spearup9 = {FALSE,SPEARUP9,2,T_Spears,SF_DOWN,&s_spearup10};

statetype s_spearup8 = {FALSE,SPEARUP8,35,T_Spears,SF_DOWN|SF_SOUND,&s_spearup9};
statetype s_spearup7 = {FALSE,SPEARUP7,2,T_Spears,SF_DOWN,&s_spearup8};
statetype s_spearup6 = {FALSE,SPEARUP6,2,T_Spears,SF_CRUSH,&s_spearup7};
statetype s_spearup5 = {FALSE,SPEARUP5,2,T_Spears,SF_CRUSH,&s_spearup6};
statetype s_spearup4 = {FALSE,SPEARUP4,2,T_Spears,SF_CRUSH,&s_spearup5};
statetype s_spearup3 = {FALSE,SPEARUP3,2,T_Spears,SF_CRUSH,&s_spearup4};
statetype s_spearup2 = {FALSE,SPEARUP2,2,T_Spears,SF_CRUSH,&s_spearup3};
statetype s_spearup1 = {FALSE,SPEARUP1,2,T_Spears,SF_CRUSH,&s_spearup2};


statetype s_dust = {FALSE,NOTHING,0,NULL,0,&s_dust};



statetype s_gas2 = {FALSE,SPR42_GRATE,5,T_Count,0,&s_gas2};
statetype s_gas1 = {FALSE,SPR42_GRATE,0,NULL,0,&s_gas1};


//================== player stuff =======================================/

statetype s_p_bazooka1 = {16,SPR_BJMISS11,3,T_Projectile,0,&s_p_bazooka1};

statetype s_p_grenade = {16,SPR_BJMISS11,3,T_Projectile,0,&s_p_grenade};

/*
statetype s_p_misssmoke4 = {FALSE,MISSSMOKE4,7,NULL,0,NULL};
statetype s_p_misssmoke3 = {FALSE,MISSSMOKE3,7,NULL,0,&s_p_misssmoke4};
statetype s_p_misssmoke2 = {FALSE,MISSSMOKE2,7,NULL,0,&s_p_misssmoke3};
statetype s_p_misssmoke1 = {FALSE,MISSSMOKE1,3,NULL,0,&s_p_misssmoke2};
*/


statetype s_basemarker8 = {FALSE,FLASH8,3,NULL,0,&s_basemarker1};
statetype s_basemarker7 = {FALSE,FLASH7,3,NULL,0,&s_basemarker8};
statetype s_basemarker6 = {FALSE,FLASH6,3,NULL,0,&s_basemarker7};
statetype s_basemarker5 = {FALSE,FLASH5,3,NULL,0,&s_basemarker6};
statetype s_basemarker4 = {FALSE,FLASH4,3,NULL,0,&s_basemarker5};
statetype s_basemarker3 = {FALSE,FLASH3,3,NULL,0,&s_basemarker4};
statetype s_basemarker2 = {FALSE,FLASH2,3,NULL,0,&s_basemarker3};
statetype s_basemarker1 = {FALSE,FLASH1,3,NULL,0,&s_basemarker2};



statetype s_flash8 = {FALSE,FLASH8,3,NULL,0,NULL};
statetype s_flash7 = {FALSE,FLASH7,3,NULL,0,&s_flash8};
statetype s_flash6 = {FALSE,FLASH6,3,NULL,0,&s_flash7};
statetype s_flash5 = {FALSE,FLASH5,3,NULL,0,&s_flash6};
statetype s_flash4 = {FALSE,FLASH4,3,NULL,0,&s_flash5};
statetype s_flash3 = {FALSE,FLASH3,3,NULL,0,&s_flash4};
statetype s_flash2 = {FALSE,FLASH2,3,NULL,0,&s_flash3};
statetype s_flash1 = {FALSE,FLASH1,3,NULL,0,&s_flash2};

statetype s_gunsmoke8 = {FALSE,GUNSMOKE8,3,NULL,0,NULL};
statetype s_gunsmoke7 = {FALSE,GUNSMOKE7,3,NULL,0,&s_gunsmoke8};
statetype s_gunsmoke6 = {FALSE,GUNSMOKE6,3,NULL,0,&s_gunsmoke7};
statetype s_gunsmoke5 = {FALSE,GUNSMOKE5,3,NULL,0,&s_gunsmoke6};
statetype s_gunsmoke4 = {FALSE,GUNSMOKE4,3,NULL,0,&s_gunsmoke5};
statetype s_gunsmoke3 = {FALSE,GUNSMOKE3,3,NULL,0,&s_gunsmoke4};
statetype s_gunsmoke2 = {FALSE,GUNSMOKE2,3,NULL,0,&s_gunsmoke3};
statetype s_gunsmoke1 = {FALSE,GUNSMOKE1,2,NULL,0,&s_gunsmoke2};

statetype s_bloodspurt8 = {FALSE,BLOODSPURT8,7,NULL,0,NULL};
statetype s_bloodspurt7 = {FALSE,BLOODSPURT7,7,NULL,0,&s_bloodspurt8};
statetype s_bloodspurt6 = {FALSE,BLOODSPURT6,7,NULL,0,&s_bloodspurt7};
statetype s_bloodspurt5 = {FALSE,BLOODSPURT5,7,NULL,0,&s_bloodspurt6};
statetype s_bloodspurt4 = {FALSE,BLOODSPURT4,7,NULL,0,&s_bloodspurt5};
statetype s_bloodspurt3 = {FALSE,BLOODSPURT3,7,NULL,0,&s_bloodspurt4};
statetype s_bloodspurt2 = {FALSE,BLOODSPURT2,7,NULL,0,&s_bloodspurt3};
statetype s_bloodspurt1 = {FALSE,BLOODSPURT1,5,NULL,0,&s_bloodspurt2};

statetype s_hitmetalwall4 = {FALSE,HITMETALWALL4,7,NULL,0,NULL};
statetype s_hitmetalwall3 = {FALSE,HITMETALWALL3,7,NULL,0,&s_hitmetalwall4};
statetype s_hitmetalwall2 = {FALSE,HITMETALWALL2,7,NULL,0,&s_hitmetalwall3};
statetype s_hitmetalwall1 = {FALSE,HITMETALWALL1,5,NULL,0,&s_hitmetalwall2};

statetype s_hitmetalactor4 = {FALSE,HITMETALACTOR4,7,NULL,0,NULL};
statetype s_hitmetalactor3 = {FALSE,HITMETALACTOR3,7,NULL,0,&s_hitmetalactor4};
statetype s_hitmetalactor2 = {FALSE,HITMETALACTOR2,7,NULL,0,&s_hitmetalactor3};
statetype s_hitmetalactor1 = {FALSE,HITMETALACTOR1,5,NULL,0,&s_hitmetalactor2};



statetype s_fireunit15 = {FALSE,FIREW15,3,T_Firethink,0,&s_fireunit1};
statetype s_fireunit14 = {FALSE,FIREW14,3,T_Firethink,0,&s_fireunit15};
statetype s_fireunit13 = {FALSE,FIREW13,3,T_Firethink,0,&s_fireunit14};
statetype s_fireunit12 = {FALSE,FIREW12,3,T_Firethink,0,&s_fireunit13};
statetype s_fireunit11 = {FALSE,FIREW11,3,T_Firethink,0,&s_fireunit12};
statetype s_fireunit10 = {FALSE,FIREW10,3,T_Firethink,0,&s_fireunit11};
statetype s_fireunit9 = {FALSE,FIREW9,3,T_Firethink,0,&s_fireunit10};
statetype s_fireunit8 = {FALSE,FIREW8,3,T_Firethink,0,&s_fireunit9};
statetype s_fireunit7 = {FALSE,FIREW7,3,T_Firethink,0,&s_fireunit8};
statetype s_fireunit6 = {FALSE,FIREW6,3,T_Firethink,0,&s_fireunit7};
statetype s_fireunit5 = {FALSE,FIREW5,3,T_Firethink,0,&s_fireunit6};
statetype s_fireunit4 = {FALSE,FIREW4,3,T_Firethink,0,&s_fireunit5};
statetype s_fireunit3 = {FALSE,FIREW3,3,T_Firethink,0,&s_fireunit4};
statetype s_fireunit2 = {FALSE,FIREW2,3,T_Firethink,0,&s_fireunit3};
statetype s_fireunit1 = {FALSE,FIREW1,3,T_Firethink,0,&s_fireunit2};



statetype s_skeleton48 = {FALSE,SKELETON48,0,T_Convert,0,&s_skeleton48};
statetype s_skeleton47 = {FALSE,SKELETON47,2,NULL,0,&s_skeleton48};
statetype s_skeleton46 = {FALSE,SKELETON46,2,NULL,0,&s_skeleton47};
statetype s_skeleton45 = {FALSE,SKELETON45,2,NULL,0,&s_skeleton46};
statetype s_skeleton44 = {FALSE,SKELETON44,2,NULL,0,&s_skeleton45};
statetype s_skeleton43 = {FALSE,SKELETON43,2,NULL,0,&s_skeleton44};
statetype s_skeleton42 = {FALSE,SKELETON42,2,NULL,0,&s_skeleton43};
statetype s_skeleton41 = {FALSE,SKELETON41,2,T_Xylophone,0,&s_skeleton42};
statetype s_skeleton40 = {FALSE,SKELETON40,2,NULL,0,&s_skeleton41};
statetype s_skeleton39 = {FALSE,SKELETON39,2,NULL,0,&s_skeleton40};
statetype s_skeleton38 = {FALSE,SKELETON38,2,NULL,0,&s_skeleton39};
statetype s_skeleton37 = {FALSE,SKELETON37,2,NULL,0,&s_skeleton38};
statetype s_skeleton36 = {FALSE,SKELETON36,2,NULL,0,&s_skeleton37};
statetype s_skeleton35 = {FALSE,SKELETON35,2,NULL,0,&s_skeleton36};
statetype s_skeleton34 = {FALSE,SKELETON34,2,NULL,0,&s_skeleton35};
statetype s_skeleton33 = {FALSE,SKELETON33,2,NULL,0,&s_skeleton34};
statetype s_skeleton32 = {FALSE,SKELETON32,2,NULL,0,&s_skeleton33};
statetype s_skeleton31 = {FALSE,SKELETON31,2,NULL,0,&s_skeleton32};
statetype s_skeleton30 = {FALSE,SKELETON30,2,NULL,0,&s_skeleton31};
statetype s_skeleton29 = {FALSE,SKELETON29,2,NULL,0,&s_skeleton30};
statetype s_skeleton28 = {FALSE,SKELETON28,2,NULL,0,&s_skeleton29};
statetype s_skeleton27 = {FALSE,SKELETON27,2,NULL,0,&s_skeleton28};
statetype s_skeleton26 = {FALSE,SKELETON26,2,NULL,0,&s_skeleton27};
statetype s_skeleton25 = {FALSE,SKELETON25,2,NULL,0,&s_skeleton26};
statetype s_skeleton24 = {FALSE,SKELETON24,2,NULL,0,&s_skeleton25};
statetype s_skeleton23 = {FALSE,SKELETON23,2,NULL,0,&s_skeleton24};
statetype s_skeleton22 = {FALSE,SKELETON22,2,NULL,0,&s_skeleton23};
statetype s_skeleton21 = {FALSE,SKELETON21,2,NULL,0,&s_skeleton22};
statetype s_skeleton20 = {FALSE,SKELETON20,2,NULL,0,&s_skeleton21};
statetype s_skeleton19 = {FALSE,SKELETON19,2,NULL,0,&s_skeleton20};
statetype s_skeleton18 = {FALSE,SKELETON18,2,NULL,0,&s_skeleton19};
statetype s_skeleton17 = {FALSE,SKELETON17,2,NULL,0,&s_skeleton18};
statetype s_skeleton16 = {FALSE,SKELETON16,2,NULL,0,&s_skeleton17};
statetype s_skeleton15 = {FALSE,SKELETON15,2,NULL,0,&s_skeleton16};
statetype s_skeleton14 = {FALSE,SKELETON14,2,NULL,0,&s_skeleton15};
statetype s_skeleton13 = {FALSE,SKELETON13,2,NULL,0,&s_skeleton14};
statetype s_skeleton12 = {FALSE,SKELETON12,2,NULL,0,&s_skeleton13};
statetype s_skeleton11 = {FALSE,SKELETON11,2,NULL,0,&s_skeleton12};
statetype s_skeleton10 = {FALSE,SKELETON10,2,NULL,0,&s_skeleton11};
statetype s_skeleton9 = {FALSE,SKELETON9,2,NULL,0,&s_skeleton10};
statetype s_skeleton8 = {FALSE,SKELETON8,2,NULL,0,&s_skeleton9};
statetype s_skeleton7 = {FALSE,SKELETON7,2,NULL,0,&s_skeleton8};
statetype s_skeleton6 = {FALSE,SKELETON6,2,NULL,0,&s_skeleton7};
statetype s_skeleton5 = {FALSE,SKELETON5,2,NULL,0,&s_skeleton6};
statetype s_skeleton4 = {FALSE,SKELETON4,2,NULL,0,&s_skeleton5};
statetype s_skeleton3 = {FALSE,SKELETON3,2,NULL,0,&s_skeleton4};
statetype s_skeleton2 = {FALSE,SKELETON2,2,NULL,0,&s_skeleton3};
statetype s_skeleton1 = {FALSE,SKELETON1,2,NULL,0,&s_skeleton2};



statetype s_spring9 = {FALSE,SPRING9,4,T_Spring,SF_DOWN,&s_spring1};
statetype s_spring8 = {FALSE,SPRING8,4,NULL,0,&s_spring9};
statetype s_spring7 = {FALSE,SPRING7,4,NULL,0,&s_spring8};
statetype s_spring6 = {FALSE,SPRING6,4,NULL,0,&s_spring7};
statetype s_spring5 = {FALSE,SPRING5,4,NULL,0,&s_spring6};
statetype s_spring4 = {FALSE,SPRING4,15,NULL,SF_UP,&s_spring5};
statetype s_spring3 = {FALSE,SPRING3,1,NULL,SF_UP,&s_spring4};
statetype s_spring2 = {FALSE,SPRING2,1,NULL,SF_UP,&s_spring3};
statetype s_spring1 = {FALSE,SPRING1,0,NULL,SF_UP,&s_spring1};


statetype s_autospring9 = {FALSE,SPRING9,4,NULL,0,&s_autospring1};
statetype s_autospring8 = {FALSE,SPRING8,4,NULL,0,&s_autospring9};
statetype s_autospring7 = {FALSE,SPRING7,4,NULL,0,&s_autospring8};
statetype s_autospring6 = {FALSE,SPRING6,4,NULL,0,&s_autospring7};
statetype s_autospring5 = {FALSE,SPRING5,4,NULL,0,&s_autospring6};
statetype s_autospring4 = {FALSE,SPRING4,15,NULL,SF_UP,&s_autospring5};
statetype s_autospring3 = {FALSE,SPRING3,1,T_Spring,SF_UP,&s_autospring4};
statetype s_autospring2 = {FALSE,SPRING2,1,T_Spring,SF_UP,&s_autospring3};
statetype s_autospring1 = {FALSE,SPRING1,70*SPRINGDELAY,NULL,0,&s_autospring2};

//====================== remote player ==============================//

statetype s_itemspawn8 = {FALSE,ITEMSPAWN8,6,NULL,0,NULL};
statetype s_itemspawn7 = {FALSE,ITEMSPAWN7,6,NULL,0,&s_itemspawn8};
statetype s_itemspawn6 = {FALSE,ITEMSPAWN6,6,NULL,0,&s_itemspawn7};
statetype s_itemspawn5 = {FALSE,ITEMSPAWN5,6,NULL,0,&s_itemspawn6};
statetype s_itemspawn4 = {FALSE,ITEMSPAWN4,6,NULL,0,&s_itemspawn5};
statetype s_itemspawn3 = {FALSE,ITEMSPAWN3,6,NULL,0,&s_itemspawn4};
statetype s_itemspawn2 = {FALSE,ITEMSPAWN2,6,NULL,0,&s_itemspawn3};
statetype s_itemspawn1 = {FALSE,ITEMSPAWN1,6,NULL,0,&s_itemspawn2};


statetype s_player = {TRUE,CASSATT_S1,0,T_Player,0,&s_player};

statetype s_pgunattack2 = {TRUE,CASSATT_SHOOT11,0,T_Attack,0,&s_pgunattack2};
statetype s_pgunattack1 = {TRUE,CASSATT_SHOOT21,5,T_Attack,0,&s_pgunattack2};

statetype s_pmissattack2 = {TRUE,CASSATTM_SHOOT11,0,T_Attack,0,&s_pmissattack2};
statetype s_pmissattack1 = {TRUE,CASSATTM_SHOOT21,5,T_Attack,0,&s_pmissattack2};

statetype s_pbatblast = {TRUE,CASSATT_SHOOT21,0,T_BatBlast,0,&s_pbatblast};




statetype s_remotemove4 = {TRUE,CASSATT_W41,5,T_Player,SF_DOWN,&s_remotemove1};
statetype s_remotemove3 = {TRUE,CASSATT_W31,5,T_Player,SF_DOWN,&s_remotemove4};
statetype s_remotemove2 = {TRUE,CASSATT_W21,5,T_Player,SF_DOWN,&s_remotemove3};
statetype s_remotemove1 = {TRUE,CASSATT_W11,5,T_Player,SF_DOWN,&s_remotemove2};

statetype s_remoteinelev = {TRUE,CASSATT_S1,700,T_Player,0,&s_remoteinelev};

statetype s_remotedead = {FALSE,CASSATT_VDEAD,0,T_Player,0,&s_remotedead};



statetype s_remotedie6 = {FALSE,CASSATT_VDIE6,3,T_Player,0,&s_remotedead};
statetype s_remotedie5 = {FALSE,CASSATT_VDIE5,3,T_Player,0,&s_remotedie6};
statetype s_remotedie4 = {FALSE,CASSATT_VDIE4,3,T_Player,0,&s_remotedie5};
statetype s_remotedie3 = {FALSE,CASSATT_VDIE3,3,T_Player,0,&s_remotedie4};
statetype s_remotedie2 = {FALSE,CASSATT_VDIE2,3,T_Player,0,&s_remotedie3};
statetype s_remotedie1 = {FALSE,CASSATT_VDIE1,3,T_Player,0,&s_remotedie2};


statetype s_voidwait = {FALSE,NOTHING,0,T_DeadWait,0,&s_voidwait};
statetype s_ashwait = {FALSE,SKELETON48,0,T_DeadWait,0,&s_ashwait};
statetype s_deadwait = {FALSE,CASSATT_VDEAD,0,T_DeadWait,0,&s_deadwait};
statetype s_gutwait = {FALSE,GUTS12,0,T_DeadWait,0,&s_gutwait};
statetype s_remoteguts12 = {FALSE,GUTS12,0,T_Player,0,&s_remoteguts12};
statetype s_remoteguts11 = {FALSE,GUTS11,3,T_Player,0,&s_remoteguts12};
statetype s_remoteguts10 = {FALSE,GUTS10,3,T_Player,0,&s_remoteguts11};
statetype s_remoteguts9 = {FALSE,GUTS9,3,T_Player,0,&s_remoteguts10};
statetype s_remoteguts8 = {FALSE,GUTS8,3,T_Player,0,&s_remoteguts9};
statetype s_remoteguts7 = {FALSE,GUTS7,3,T_Player,0,&s_remoteguts8};
statetype s_remoteguts6 = {FALSE,GUTS6,3,T_Player,0,&s_remoteguts7};
statetype s_remoteguts5 = {FALSE,GUTS5,3,T_Player,0,&s_remoteguts6};
statetype s_remoteguts4 = {FALSE,GUTS4,3,T_Player,0,&s_remoteguts5};
statetype s_remoteguts3 = {FALSE,GUTS3,3,T_Player,0,&s_remoteguts4};
statetype s_remoteguts2 = {FALSE,GUTS2,3,T_Player,0,&s_remoteguts3};
statetype s_remoteguts1 = {FALSE,GUTS1,3,T_Player,0,&s_remoteguts2};

//========================================================================//

statetype s_godfire4 = {FALSE,GODFIRE4,3,T_Projectile,0,&s_godfire1};
statetype s_godfire3 = {FALSE,GODFIRE3,3,T_Projectile,0,&s_godfire4};
statetype s_godfire2 = {FALSE,GODFIRE2,3,T_Projectile,0,&s_godfire3};
statetype s_godfire1 = {FALSE,GODFIRE1,3,T_Projectile,0,&s_godfire2};




statetype s_guts12 = {FALSE,GUTS12,0,T_Collide,SF_GUTS,&s_guts12};
statetype s_guts11 = {FALSE,GUTS11,3,T_Collide,SF_GUTS,&s_guts12};
statetype s_guts10 = {FALSE,GUTS10,3,T_Collide,SF_GUTS,&s_guts11};
statetype s_guts9 = {FALSE,GUTS9,3,T_Collide,SF_GUTS,&s_guts10};
statetype s_guts8 = {FALSE,GUTS8,3,T_Collide,SF_GUTS,&s_guts9};
statetype s_guts7 = {FALSE,GUTS7,3,T_Collide,SF_GUTS,&s_guts8};
statetype s_guts6 = {FALSE,GUTS6,3,T_Collide,SF_GUTS,&s_guts7};
statetype s_guts5 = {FALSE,GUTS5,3,T_Collide,SF_GUTS,&s_guts6};
statetype s_guts4 = {FALSE,GUTS4,3,T_Collide,SF_GUTS,&s_guts5};
statetype s_guts3 = {FALSE,GUTS3,3,T_Collide,SF_GUTS,&s_guts4};
statetype s_guts2 = {FALSE,GUTS2,3,T_Collide,SF_GUTS,&s_guts3};
statetype s_guts1 = {FALSE,GUTS1,3,T_Collide,SF_GUTS,&s_guts2};

//MED
#if (SHAREWARE == 1) || (DOPEFISH == 0)
statetype s_collectorwander8 = {FALSE,COLLECTOR15,0,T_CollectorWander,0,&s_collectorwander1};
statetype s_collectorwander7 = {FALSE,COLLECTOR13,1,T_CollectorWander,0,&s_collectorwander8};
statetype s_collectorwander6 = {FALSE,COLLECTOR11,0,T_CollectorWander,0,&s_collectorwander7};
statetype s_collectorwander5 = {FALSE,COLLECTOR9,1,T_CollectorWander,0,&s_collectorwander6};
statetype s_collectorwander4 = {FALSE,COLLECTOR7,0,T_CollectorWander,0,&s_collectorwander5};
statetype s_collectorwander3 = {FALSE,COLLECTOR5,1,T_CollectorWander,0,&s_collectorwander4};
statetype s_collectorwander2 = {FALSE,COLLECTOR3,0,T_CollectorWander,0,&s_collectorwander3};
statetype s_collectorwander1 = {FALSE,COLLECTOR1,1,T_CollectorWander,0,&s_collectorwander2};
#else

statetype s_collectorwander8 = {FALSE,DOPE8,2,T_CollectorWander,0,&s_collectorwander1};
statetype s_collectorwander7 = {FALSE,DOPE7,2,T_CollectorWander,0,&s_collectorwander8};
statetype s_collectorwander6 = {FALSE,DOPE6,2,T_CollectorWander,0,&s_collectorwander7};
statetype s_collectorwander5 = {FALSE,DOPE5,2,T_CollectorWander,0,&s_collectorwander6};
statetype s_collectorwander4 = {FALSE,DOPE4,2,T_CollectorWander,0,&s_collectorwander5};
statetype s_collectorwander3 = {FALSE,DOPE3,2,T_CollectorWander,0,&s_collectorwander4};
statetype s_collectorwander2 = {FALSE,DOPE2,2,T_CollectorWander,0,&s_collectorwander3};
statetype s_collectorwander1 = {FALSE,DOPE1,2,T_CollectorWander,0,&s_collectorwander2};
#endif


//MED
#if (SHAREWARE == 1) || (DOPEFISH == 0)
statetype s_collectorfdoor8 = {FALSE,COLLECTOR15,0,T_CollectorFindDoor,0,&s_collectorfdoor1};
statetype s_collectorfdoor7 = {FALSE,COLLECTOR13,1,T_CollectorFindDoor,0,&s_collectorfdoor8};
statetype s_collectorfdoor6 = {FALSE,COLLECTOR11,0,T_CollectorFindDoor,0,&s_collectorfdoor7};
statetype s_collectorfdoor5 = {FALSE,COLLECTOR9,1,T_CollectorFindDoor,0,&s_collectorfdoor6};
statetype s_collectorfdoor4 = {FALSE,COLLECTOR7,0,T_CollectorFindDoor,0,&s_collectorfdoor5};
statetype s_collectorfdoor3 = {FALSE,COLLECTOR5,1,T_CollectorFindDoor,0,&s_collectorfdoor4};
statetype s_collectorfdoor2 = {FALSE,COLLECTOR3,0,T_CollectorFindDoor,0,&s_collectorfdoor3};
statetype s_collectorfdoor1 = {FALSE,COLLECTOR1,1,T_CollectorFindDoor,0,&s_collectorfdoor2};
#else

statetype s_collectorfdoor8 = {FALSE,DOPE8,2,T_CollectorFindDoor,0,&s_collectorfdoor1};
statetype s_collectorfdoor7 = {FALSE,DOPE7,2,T_CollectorFindDoor,0,&s_collectorfdoor8};
statetype s_collectorfdoor6 = {FALSE,DOPE6,2,T_CollectorFindDoor,0,&s_collectorfdoor7};
statetype s_collectorfdoor5 = {FALSE,DOPE5,2,T_CollectorFindDoor,0,&s_collectorfdoor6};
statetype s_collectorfdoor4 = {FALSE,DOPE4,2,T_CollectorFindDoor,0,&s_collectorfdoor5};
statetype s_collectorfdoor3 = {FALSE,DOPE3,2,T_CollectorFindDoor,0,&s_collectorfdoor4};
statetype s_collectorfdoor2 = {FALSE,DOPE2,2,T_CollectorFindDoor,0,&s_collectorfdoor3};
statetype s_collectorfdoor1 = {FALSE,DOPE1,2,T_CollectorFindDoor,0,&s_collectorfdoor2};

#endif

statetype s_timekeeper = {FALSE,NOTHING,140,T_End,0,NULL};

statetype s_wind = {FALSE,-1,10,T_Wind,0,&s_wind};


statetype s_deadblood8 = {FALSE,NOTHING,0,T_Blood,0,&s_deadblood8};
statetype s_deadblood7 = {FALSE,DEADBLOOD7,5,NULL,0,&s_deadblood8};
statetype s_deadblood6 = {FALSE,DEADBLOOD6,5,NULL,0,&s_deadblood7};
statetype s_deadblood5 = {FALSE,DEADBLOOD5,5,NULL,0,&s_deadblood6};
statetype s_deadblood4 = {FALSE,DEADBLOOD4,5,NULL,0,&s_deadblood5};
statetype s_deadblood3 = {FALSE,DEADBLOOD3,5,NULL,0,&s_deadblood4};
statetype s_deadblood2 = {FALSE,DEADBLOOD2,5,NULL,0,&s_deadblood3};
statetype s_deadblood1 = {FALSE,DEADBLOOD1,5,NULL,0,&s_deadblood2};

/*
statetype s_rain7 = {FALSE,RAINDROP6,0,NULL,0,NULL};
statetype s_rain6 = {FALSE,RAINDROP5,3,NULL,0,&s_rain7};
statetype s_rain5 = {FALSE,RAINDROP4,0,NULL,0,&s_rain6};
statetype s_rain4 = {FALSE,RAINDROP3,3,NULL,0,&s_rain5};
statetype s_rain3 = {FALSE,RAINDROP2,0,NULL,0,&s_rain4};
statetype s_rain2 = {FALSE,RAINDROP1,0,T_RainFall,0,&s_rain2};
statetype s_rain1 = {FALSE,NOTHING,50,T_RainSpawn,0,&s_rain1};

statetype s_rainmaster = {FALSE,NOTHING,0,T_RainMaster,0,&s_rainmaster};
*/

statetype s_pathdisk = {FALSE,PLATFORM1,0,T_Path,0,&s_pathdisk};
statetype s_elevdisk = {FALSE,PLATFORM1,0,T_ElevDisk,0,&s_elevdisk};

statetype s_diskmaster = {FALSE,NOTHING,0,T_ElevDisk,0,&s_diskmaster};



statetype s_blooddrip4 = {FALSE,WALLGIB4,3,T_BloodFall,0,&s_blooddrip1};
statetype s_blooddrip3 = {FALSE,WALLGIB3,3,T_BloodFall,0,&s_blooddrip4};
statetype s_blooddrip2 = {FALSE,WALLGIB2,3,T_BloodFall,0,&s_blooddrip3};
statetype s_blooddrip1 = {FALSE,WALLGIB1,3,T_BloodFall,0,&s_blooddrip2};



//==================== Push column =====================================//


statetype s_pushcolumn1 = {FALSE,SPR_PUSHCOLUMN1,6,T_MoveColumn,0,&s_pushcolumn1};
statetype s_pushcolumn2 = {FALSE,SPR_PUSHCOLUMN1,6,T_MoveColumn,0,&s_pushcolumn2};
statetype s_pushcolumn3 = {FALSE,SPR_PUSHCOLUMN1,6,T_MoveColumn,0,&s_pushcolumn3};


//=================== Wall Fire =======================================/

statetype s_wallfireball = {FALSE,NOTHING,50,A_Wallfire,0,&s_wallfireball};

statetype s_crossfire2 = {TRUE,SPR_CROSSFIRE31,6,T_Projectile,0,&s_crossfire1};
statetype s_crossfire1 = {TRUE,SPR_CROSSFIRE11,6,T_Projectile,0,&s_crossfire2};

statetype s_crossdone5 = {FALSE,SPR_CREXP5,6,NULL,0,NULL};
statetype s_crossdone4 = {FALSE,SPR_CREXP4,6,NULL,0,&s_crossdone5};
statetype s_crossdone3 = {FALSE,SPR_CREXP3,6,NULL,0,&s_crossdone4};
statetype s_crossdone2 = {FALSE,SPR_CREXP2,6,NULL,0,&s_crossdone3};
statetype s_crossdone1 = {FALSE,SPR_CREXP1,6,NULL,0,&s_crossdone2};



//=============== gib/related states ===========================================//


statetype s_bossdeath = {FALSE,NOTHING,140,T_BossDied,0,NULL};

statetype s_megaremove     = {FALSE,NOTHING,0,NULL,0,NULL};
statetype s_megaexplosions = {FALSE,NOTHING,0,T_BossExplosions,0,&s_megaexplosions};
statetype s_superparticles = {FALSE,NOTHING,0,T_ParticleGenerate,0,&s_superparticles};


statetype s_gibsdone8 = {FALSE,PARTICLE12,2,NULL,0,NULL};
statetype s_gibsdone7 = {FALSE,PARTICLE11,2,NULL,0,&s_gibsdone8};
//MED
statetype s_gibsdone6 = {FALSE,PARTICLE10,2,NULL,0,&s_gibsdone7};
statetype s_gibsdone5 = {FALSE,PARTICLE09,2,NULL,0,&s_gibsdone6};
statetype s_gibsdone4 = {FALSE,PARTICLE08,2,NULL,0,&s_gibsdone5};
statetype s_gibsdone3 = {FALSE,PARTICLE07,2,NULL,0,&s_gibsdone4};
statetype s_gibsdone2 = {FALSE,PARTICLE06,2,NULL,0,&s_gibsdone3};
statetype s_gibsdone1 = {FALSE,PARTICLE05,2,NULL,0,&s_gibsdone2};

statetype s_gibs4 = {FALSE,PARTICLE04,2,T_Particle,0,&s_gibs1};
statetype s_gibs3 = {FALSE,PARTICLE03,2,T_Particle,0,&s_gibs4};
statetype s_gibs2 = {FALSE,PARTICLE02,2,T_Particle,0,&s_gibs3};
statetype s_gibs1 = {FALSE,PARTICLE01,2,T_Particle,0,&s_gibs2};



//statetype s_head = {FALSE,DEADHEAD,4200,NULL,0,&s_gibsdone2};

statetype s_eye3 = {FALSE,NOTHING,7,T_SlideDownScreen,SF_EYE3,&s_eye1};
statetype s_eye2 = {FALSE,NOTHING,7,T_SlideDownScreen,SF_EYE2,&s_eye3};
statetype s_eye1 = {FALSE,NOTHING,7,T_SlideDownScreen,SF_EYE1,&s_eye2};

statetype s_littlesoul = {FALSE,LITTLESOUL,0,MissileMovement,0,&s_littlesoul};
statetype s_bigsoul = {FALSE,BIGSOUL,0,MissileMovement,0,&s_bigsoul};


statetype s_vaporized8 = {FALSE,VAPORIZED8,0,T_Convert,0,NULL};
statetype s_vaporized7 = {FALSE,VAPORIZED7,3,NULL,0,&s_vaporized8};
statetype s_vaporized6 = {FALSE,VAPORIZED6,3,NULL,0,&s_vaporized7};
statetype s_vaporized5 = {FALSE,VAPORIZED5,3,NULL,0,&s_vaporized6};
statetype s_vaporized4 = {FALSE,VAPORIZED4,3,NULL,0,&s_vaporized5};
statetype s_vaporized3 = {FALSE,VAPORIZED3,3,NULL,0,&s_vaporized4};
statetype s_vaporized2 = {FALSE,VAPORIZED2,3,NULL,0,&s_vaporized3};
statetype s_vaporized1 = {FALSE,VAPORIZED1,3,NULL,0,&s_vaporized2};

statetype s_respawn8 = {FALSE,VAPORIZED1,0,SetShapeoffset,0,&s_player};
statetype s_respawn7 = {FALSE,VAPORIZED2,3,NULL,0,&s_respawn8};
statetype s_respawn6 = {FALSE,VAPORIZED3,3,NULL,0,&s_respawn7};
statetype s_respawn5 = {FALSE,VAPORIZED4,3,NULL,0,&s_respawn6};
statetype s_respawn4 = {FALSE,VAPORIZED5,3,NULL,0,&s_respawn5};
statetype s_respawn3 = {FALSE,VAPORIZED6,3,NULL,0,&s_respawn4};
statetype s_respawn2 = {FALSE,VAPORIZED7,3,NULL,0,&s_respawn3};
statetype s_respawn1 = {FALSE,VAPORIZED8,3,NULL,0,&s_respawn2};



#if (SHAREWARE == 0)


//========================= NON-SHAREWARE STATES ============================


statetype s_scottwander7 = {FALSE,SCOTHEAD7,4,T_CollectorWander,0,&s_scottwander1};
statetype s_scottwander6 = {FALSE,SCOTHEAD6,4,T_CollectorWander,0,&s_scottwander7};
statetype s_scottwander5 = {FALSE,SCOTHEAD5,4,T_CollectorWander,0,&s_scottwander6};
statetype s_scottwander4 = {FALSE,SCOTHEAD4,4,T_CollectorWander,0,&s_scottwander5};
statetype s_scottwander3 = {FALSE,SCOTHEAD3,4,T_CollectorWander,0,&s_scottwander4};
statetype s_scottwander2 = {FALSE,SCOTHEAD2,4,T_CollectorWander,0,&s_scottwander3};
statetype s_scottwander1 = {FALSE,SCOTHEAD1,4,T_CollectorWander,0,&s_scottwander2};

statetype s_scottwanderdoor7 = {FALSE,SCOTHEAD7,4,T_CollectorWander,0,&s_scottwanderdoor1};
statetype s_scottwanderdoor6 = {FALSE,SCOTHEAD6,4,T_CollectorWander,0,&s_scottwanderdoor7};
statetype s_scottwanderdoor5 = {FALSE,SCOTHEAD5,4,T_CollectorWander,0,&s_scottwanderdoor6};
statetype s_scottwanderdoor4 = {FALSE,SCOTHEAD4,4,T_CollectorWander,0,&s_scottwanderdoor5};
statetype s_scottwanderdoor3 = {FALSE,SCOTHEAD3,4,T_CollectorWander,0,&s_scottwanderdoor4};
statetype s_scottwanderdoor2 = {FALSE,SCOTHEAD2,4,T_CollectorWander,0,&s_scottwanderdoor3};
statetype s_scottwanderdoor1 = {FALSE,SCOTHEAD1,4,T_CollectorWander,0,&s_scottwanderdoor2};



/*
===========================================================================

							 OVERPATROLS (op)
																			,
===========================================================================
*/

statetype s_opstand	   = {TRUE,SPR_OP_S1,0,T_Stand,0,&s_opstand};

statetype s_oppath4 	   = {TRUE,SPR_OP_W41,10,T_Path,0,&s_oppath1};
statetype s_oppath3 	   = {TRUE,SPR_OP_W31,10,T_Path,SF_CLOSE,&s_oppath4};
statetype s_oppath2	   = {TRUE,SPR_OP_W21,10,T_Path,0,&s_oppath3};
statetype s_oppath1 	   = {TRUE,SPR_OP_W11,10,T_Path,SF_CLOSE,&s_oppath2};


statetype s_opcollide  = {FALSE,SPR_OP_PAIN1,0,T_Collide,0,&s_opcollide};
statetype s_opcollide2 = {FALSE,SPR_OP_PAIN2,0,T_Collide,0,&s_opcollide2};


//statetype s_opuse2 	   = {TRUE,SPR_OP_USE21,10,T_Use,0,&s_oppath1};
//statetype s_opuse1 	   = {TRUE,SPR_OP_USE11,10,T_Use,0,&s_opuse2};


statetype s_opshoot4 	= {FALSE,SPR_OP_SHOOT4,10,ActorMovement,0,&s_opchase1};
statetype s_opshoot3 	= {FALSE,SPR_OP_SHOOT3,10,ActorMovement,0,&s_opshoot4};
statetype s_opshoot2 	= {FALSE,SPR_OP_SHOOT2,20,A_Shoot,0,&s_opshoot3};
statetype s_opshoot1 	= {FALSE,SPR_OP_SHOOT1,6,ActorMovement,0,&s_opshoot2};



statetype s_opbolo5     = {FALSE,SPR_OP_BOLOSHOOT5,6,ActorMovement,0,&s_opchase1};
statetype s_opbolo4     = {FALSE,SPR_OP_BOLOSHOOT4,6,ActorMovement,0,&s_opbolo5};
statetype s_opbolo3     = {FALSE,SPR_OP_BOLOSHOOT3,20,A_MissileWeapon,0,&s_opbolo4};
statetype s_opbolo2     = {FALSE,SPR_OP_BOLOSHOOT2,6,ActorMovement,0,&s_opbolo3};
statetype s_opbolo1     = {FALSE,SPR_OP_BOLOSHOOT1,6,ActorMovement,0,&s_opbolo2};


statetype s_bolocast4   = {FALSE,SPR_BOLO4,6,T_Projectile,0,&s_bolocast4};
statetype s_bolocast3   = {FALSE,SPR_BOLO3,6,T_Projectile,0,&s_bolocast4};
statetype s_bolocast2   = {FALSE,SPR_BOLO2,6,T_Projectile,0,&s_bolocast3};
statetype s_bolocast1   = {FALSE,SPR_BOLO1,6,T_Projectile,0,&s_bolocast2};


statetype s_opchase4	   = {TRUE,SPR_OP_W41,6,T_Chase,0,&s_opchase1};
statetype s_opchase3	   = {TRUE,SPR_OP_W31,6,T_Chase,SF_CLOSE,&s_opchase4};
statetype s_opchase2	   = {TRUE,SPR_OP_W21,6,T_Chase,0,&s_opchase3};
statetype s_opchase1	   = {TRUE,SPR_OP_W11,6,T_Chase,SF_CLOSE,&s_opchase2};


statetype s_opdead		= {FALSE,SPR_OP_ALTDEAD,0,T_Collide,0,&s_opdead};
statetype s_opdie5		= {FALSE,SPR_OP_ALTDIE5,5,T_Collide,0,&s_opdead};
statetype s_opdie4		= {FALSE,SPR_OP_ALTDIE4,5,T_Collide,0,&s_opdie5};
statetype s_opdie3		= {FALSE,SPR_OP_ALTDIE3,5,T_Collide,0,&s_opdie4};
statetype s_opdie2		= {FALSE,SPR_OP_ALTDIE2,5,T_Collide,0,&s_opdie3};
statetype s_opdie1		= {FALSE,SPR_OP_ALTDIE1,5,T_Collide,0,&s_opdie2};


statetype s_opcrushed2		= {FALSE,SPR_OP_DIE3,2,NULL,0,&s_opdead};
statetype s_opcrushed1		= {FALSE,SPR_OP_DIE1,2,NULL,0,&s_opcrushed2};



/*
============================================================================

								DEATH MONKS

============================================================================
*/


statetype s_dmonkstand	= {TRUE,SPR_MONK_S1,0,T_Stand,0,&s_dmonkstand};

statetype s_dmonkpath4	= {TRUE,SPR_MONK_W41,10,T_Path,0,&s_dmonkpath1};
statetype s_dmonkpath3  = {TRUE,SPR_MONK_W31,10,T_Path,SF_CLOSE,&s_dmonkpath4};
statetype s_dmonkpath2	= {TRUE,SPR_MONK_W21,10,T_Path,0,&s_dmonkpath3};
statetype s_dmonkpath1  = {TRUE,SPR_MONK_W11,10,T_Path,SF_CLOSE,&s_dmonkpath2};


statetype s_dmonkcollide  = {FALSE,SPR_MONK_PAIN1,0,T_Collide,0,&s_dmonkcollide};
statetype s_dmonkcollide2 = {FALSE,SPR_MONK_PAIN2,0,T_Collide,0,&s_dmonkcollide2};


statetype s_dmonkshoot6 = {FALSE,SPR_MONK_DRAIN6,20,ActorMovement,0,&s_dmonkchase1};
statetype s_dmonkshoot5 = {FALSE,SPR_MONK_DRAIN5,20,ActorMovement,0,&s_dmonkshoot6};
statetype s_dmonkshoot4 = {FALSE,SPR_MONK_DRAIN4,20,ActorMovement,0,&s_dmonkshoot3};
statetype s_dmonkshoot3 = {FALSE,SPR_MONK_DRAIN3,20,A_Drain,0,&s_dmonkshoot4};
statetype s_dmonkshoot2 = {FALSE,SPR_MONK_DRAIN2,20,ActorMovement,0,&s_dmonkshoot3};
statetype s_dmonkshoot1 = {FALSE,SPR_MONK_DRAIN1,20,A_Drain,0,&s_dmonkshoot2};

statetype s_dmonkchase4	   = {TRUE,SPR_MONK_W41,6,T_Chase,0,&s_dmonkchase1};
statetype s_dmonkchase3 	= {TRUE,SPR_MONK_W31,6,T_Chase,SF_CLOSE,&s_dmonkchase4};
statetype s_dmonkchase2	   = {TRUE,SPR_MONK_W21,6,T_Chase,0,&s_dmonkchase3};
statetype s_dmonkchase1 	= {TRUE,SPR_MONK_W11,6,T_Chase,SF_CLOSE,&s_dmonkchase2};

statetype s_dmonkdead		= {FALSE,SPR_MONK_DEAD,0,T_Collide,0,&s_dmonkdead};
statetype s_dmonkdie4		= {FALSE,SPR_MONK_DIE4,5,T_Collide,0,&s_dmonkdead};
statetype s_dmonkdie3		= {FALSE,SPR_MONK_DIE3,5,T_Collide,0,&s_dmonkdie4};
statetype s_dmonkdie2		= {FALSE,SPR_MONK_DIE2,5,T_Collide,0,&s_dmonkdie3};
statetype s_dmonkdie1		= {FALSE,SPR_MONK_DIE1,5,T_Collide,0,&s_dmonkdie2};


statetype s_dmonkcrushed2		= {FALSE,SPR_MONK_DIE3,2,NULL,0,&s_dmonkdead};
statetype s_dmonkcrushed1		= {FALSE,SPR_MONK_DIE1,2,NULL,0,&s_dmonkcrushed2};

/*
============================================================================

								DEATH FIRE MONKS

============================================================================
*/

statetype s_firemonkstand	= {TRUE,SPR_FIREMONK_S1,0,T_Stand,0,&s_firemonkstand};

statetype s_firemonkpath4	= {TRUE,SPR_FIREMONK_W41,10,T_Path,0,&s_firemonkpath1};
statetype s_firemonkpath3  = {TRUE,SPR_FIREMONK_W31,10,T_Path,SF_CLOSE,&s_firemonkpath4};
statetype s_firemonkpath2	= {TRUE,SPR_FIREMONK_W21,10,T_Path,0,&s_firemonkpath3};
statetype s_firemonkpath1  = {TRUE,SPR_FIREMONK_W11,10,T_Path,SF_CLOSE,&s_firemonkpath2};


statetype s_firemonkcollide  = {FALSE,SPR_FIREMONK_PAIN1,0,T_Collide,0,&s_firemonkcollide};
statetype s_firemonkcollide2 = {FALSE,SPR_FIREMONK_PAIN2,0,T_Collide,0,&s_firemonkcollide2};

statetype s_firemonkcast7 = {FALSE,SPR_FIREMONK_CAST7,6,ActorMovement,0,&s_firemonkchase1};
statetype s_firemonkcast6 = {FALSE,SPR_FIREMONK_CAST6,40,A_MissileWeapon,0,&s_firemonkcast7};
statetype s_firemonkcast5 = {FALSE,SPR_FIREMONK_CAST5,6,ActorMovement,0,&s_firemonkcast6};
statetype s_firemonkcast4 = {FALSE,SPR_FIREMONK_CAST4,6,ActorMovement,0,&s_firemonkcast5};
statetype s_firemonkcast3 = {FALSE,SPR_FIREMONK_CAST3,6,ActorMovement,0,&s_firemonkcast4};
statetype s_firemonkcast2 = {FALSE,SPR_FIREMONK_CAST2,6,ActorMovement,0,&s_firemonkcast3};
statetype s_firemonkcast1 = {FALSE,SPR_FIREMONK_CAST1,6,ActorMovement,0,&s_firemonkcast2};

statetype s_monkfire4 = {FALSE,MONKFIRE4,3,T_Projectile,0,&s_monkfire1};
statetype s_monkfire3 = {FALSE,MONKFIRE3,3,T_Projectile,0,&s_monkfire4};
statetype s_monkfire2 = {FALSE,MONKFIRE2,3,T_Projectile,0,&s_monkfire3};
statetype s_monkfire1 = {FALSE,MONKFIRE1,3,T_Projectile,0,&s_monkfire2};


statetype s_fireballhit3 = {FALSE,SPR_FIREBALL_HIT3,5,NULL,0,NULL};
statetype s_fireballhit2 = {FALSE,SPR_FIREBALL_HIT2,5,NULL,0,&s_fireballhit3};
statetype s_fireballhit1 = {FALSE,SPR_FIREBALL_HIT1,5,NULL,0,&s_fireballhit2};

statetype s_firemonkchase4 	= {TRUE,SPR_FIREMONK_W41,6,T_Chase,0,&s_firemonkchase1};
statetype s_firemonkchase3 	= {TRUE,SPR_FIREMONK_W31,6,T_Chase,SF_CLOSE,&s_firemonkchase4};
statetype s_firemonkchase2	   = {TRUE,SPR_FIREMONK_W21,6,T_Chase,0,&s_firemonkchase3};
statetype s_firemonkchase1 	= {TRUE,SPR_FIREMONK_W11,6,T_Chase,SF_CLOSE,&s_firemonkchase2};


statetype s_firemonkdead7		= {FALSE,SPR_FIREMONK_DEAD7,0,T_Collide,0,&s_firemonkdead7};
statetype s_firemonkdead6	   = {FALSE,SPR_FIREMONK_DEAD6,5,T_Collide,0,&s_firemonkdead7};
statetype s_firemonkdead5	   = {FALSE,SPR_FIREMONK_DEAD5,5,T_Collide,0,&s_firemonkdead6};
statetype s_firemonkdead4	   = {FALSE,SPR_FIREMONK_DEAD4,5,T_Collide,0,&s_firemonkdead5};
statetype s_firemonkdead3	   = {FALSE,SPR_FIREMONK_DEAD3,5,T_Collide,0,&s_firemonkdead4};
statetype s_firemonkdead2	   = {FALSE,SPR_FIREMONK_DEAD2,5,T_Collide,0,&s_firemonkdead3};
statetype s_firemonkdead		= {FALSE,SPR_FIREMONK_DEAD1,5,T_Collide,0,&s_firemonkdead2};
statetype s_firemonkdie4		= {FALSE,SPR_FIREMONK_DIE3,5,T_Collide,0,&s_firemonkdead};
statetype s_firemonkdie3		= {FALSE,SPR_FIREMONK_DIE3,5,T_Collide,0,&s_firemonkdie4};
statetype s_firemonkdie2		= {FALSE,SPR_FIREMONK_DIE2,5,T_Collide,0,&s_firemonkdie3};
statetype s_firemonkdie1		= {FALSE,SPR_FIREMONK_DIE1,5,T_Collide,0,&s_firemonkdie2};


statetype s_firemonkcrushed2		= {FALSE,SPR_FIREMONK_DIE3,2,NULL,0,&s_firemonkdead};
statetype s_firemonkcrushed1		= {FALSE,SPR_FIREMONK_DIE1,2,NULL,0,&s_firemonkcrushed2};

/*===========================================================================

								  INSANE WALL DUDE

============================================================================*/


statetype s_wallstand = {16,BCRAFT01,0,T_Stand,0,&s_wallstand};

statetype s_wallpath = {16,BCRAFT01,20,T_AutoPath,0,&s_wallpath};

statetype s_wallshoot = {16,BCRAFT01,15,A_MissileWeapon,0,&s_wallshoot};

statetype s_wallalign = {16,BCRAFT01,0,T_AutoShootAlign,0,&s_wallalign};
statetype s_wallwait = {16,BCRAFT01,0,NULL,0,&s_wallwait};
statetype s_wallrestore = {16,BCRAFT01,0,T_AutoRealign,0,&s_wallrestore};



statetype s_wallcollide  = {16,BCRAFT01,0,T_Collide,0,&s_wallcollide};


/*
===========================================================================

									  ESAU GUDERIAN

===========================================================================
*/




statetype s_darianstand = {TRUE,SPR_DARIAN_S1,0,T_Stand,0,&s_darianstand};

statetype s_darianchase4	= {TRUE,SPR_DARIAN_W41,8,T_EsauChase,0,&s_darianchase1};
statetype s_darianchase3	= {TRUE,SPR_DARIAN_W31,8,T_EsauChase,SF_CLOSE,&s_darianchase4};
statetype s_darianchase2	= {TRUE,SPR_DARIAN_W21,8,T_EsauChase,0,&s_darianchase3};
statetype s_darianchase1	= {TRUE,SPR_DARIAN_W11,8,T_EsauChase,SF_CLOSE,&s_darianchase2};


statetype s_darianrise8 = {FALSE,SPR_DARIAN_SINK1,3,NULL,0,&s_darianwait};
statetype s_darianrise7 = {FALSE,SPR_DARIAN_SINK2,3,NULL,0,&s_darianrise8};
statetype s_darianrise6 = {FALSE,SPR_DARIAN_SINK3,3,NULL,0,&s_darianrise7};
statetype s_darianrise5 = {FALSE,SPR_DARIAN_SINK4,3,NULL,0,&s_darianrise6};
statetype s_darianrise4 = {FALSE,SPR_DARIAN_SINK5,3,NULL,0,&s_darianrise5};
statetype s_darianrise3 = {FALSE,SPR_DARIAN_SINK6,3,NULL,0,&s_darianrise4};
statetype s_darianrise2 = {FALSE,SPR_DARIAN_SINK7,3,NULL,0,&s_darianrise3};
statetype s_darianrise1 = {FALSE,SPR_DARIAN_SINK8,3,NULL,0,&s_darianrise2};


statetype s_dariansink9 = {FALSE,NOTHING,110,T_EsauRise,0,&s_darianrise1};
statetype s_dariansink8 = {FALSE,SPR_DARIAN_SINK8,3,NULL,0,&s_dariansink9};
statetype s_dariansink7 = {FALSE,SPR_DARIAN_SINK7,3,NULL,0,&s_dariansink8};
statetype s_dariansink6 = {FALSE,SPR_DARIAN_SINK6,3,NULL,0,&s_dariansink7};
statetype s_dariansink5 = {FALSE,SPR_DARIAN_SINK5,3,NULL,0,&s_dariansink6};
statetype s_dariansink4 = {FALSE,SPR_DARIAN_SINK4,3,NULL,0,&s_dariansink5};
statetype s_dariansink3 = {FALSE,SPR_DARIAN_SINK3,3,NULL,0,&s_dariansink4};
statetype s_dariansink2 = {FALSE,SPR_DARIAN_SINK2,3,NULL,0,&s_dariansink3};
statetype s_dariansink1 = {FALSE,SPR_DARIAN_SINK1,3,NULL,0,&s_dariansink2};


statetype s_dariancollide  = {FALSE,SPR_DARIAN_PAIN1,0,T_Collide,0,&s_dariancollide};
statetype s_dariancollide2 = {FALSE,SPR_DARIAN_PAIN2,0,T_Collide,0,&s_dariancollide2};


statetype s_darianshoot4  = {FALSE,SPR_DARIAN_SHOOT4,10,NULL,0,&s_darianchase1};
statetype s_darianshoot3  = {FALSE,SPR_DARIAN_SHOOT3,10,NULL,0,&s_darianshoot4};
statetype s_darianshoot2  = {FALSE,SPR_DARIAN_SHOOT2,20,A_MissileWeapon,0,&s_darianshoot3};
statetype s_darianshoot1  = {FALSE,SPR_DARIAN_SHOOT1,10,NULL,0,&s_darianshoot2};

statetype s_dariandefend3  = {FALSE,SPR_DARIAN_SHOOT3,10,NULL,0,&s_darianwait};
statetype s_dariandefend2  = {FALSE,SPR_DARIAN_SHOOT2,20,A_MissileWeapon,0,&s_dariandefend3};
statetype s_dariandefend1  = {FALSE,SPR_DARIAN_SHOOT1,10,NULL,0,&s_dariandefend2};


statetype s_darianuse4  = {TRUE,SPR_DARIAN_USE11,10,NULL,0,&s_darianspears};
statetype s_darianuse3  = {TRUE,SPR_DARIAN_USE21,10,T_Use,0,&s_darianuse4};
statetype s_darianuse2  = {TRUE,SPR_DARIAN_USE11,30,NULL,0,&s_darianuse3};
statetype s_darianuse1  = {TRUE,SPR_DARIAN_S1,40,NULL,0,&s_darianuse2};

statetype s_darianwait = {FALSE,SPR_DARIAN_SHOOT1,0,T_EsauWait,0,&s_darianwait};

statetype s_darianspears  = {TRUE,SPR_DARIAN_S1,280,NULL,0,&s_darianchase1};


statetype s_dspear16 = {FALSE,SPEARDOWN7,2,T_Spears,SF_DOWN,NULL};
statetype s_dspear15 = {FALSE,SPEARDOWN6,2,T_Spears,SF_CRUSH,&s_dspear16};
statetype s_dspear14 = {FALSE,SPEARDOWN5,2,T_Spears,SF_CRUSH,&s_dspear15};
statetype s_dspear13 = {FALSE,SPEARDOWN4,2,T_Spears,SF_CRUSH,&s_dspear14};
statetype s_dspear12 = {FALSE,SPEARDOWN3,2,T_Spears,SF_CRUSH,&s_dspear13};
statetype s_dspear11 = {FALSE,SPEARDOWN2,2,T_Spears,SF_CRUSH,&s_dspear12};
statetype s_dspear10 = {FALSE,SPEARDOWN1,2,T_Spears,SF_CRUSH,&s_dspear11};
statetype s_dspear9 = {FALSE,SPEARDOWN16,2,T_Spears,SF_CRUSH,&s_dspear10};
statetype s_dspear8 = {FALSE,SPEARDOWN15,2,T_Spears,SF_CRUSH,&s_dspear9};
statetype s_dspear7 = {FALSE,SPEARDOWN14,2,T_Spears,SF_CRUSH,&s_dspear8};
statetype s_dspear6 = {FALSE,SPEARDOWN13,2,T_Spears,SF_CRUSH,&s_dspear7};
statetype s_dspear5 = {FALSE,SPEARDOWN12,2,T_Spears,SF_CRUSH,&s_dspear6};
statetype s_dspear4 = {FALSE,SPEARDOWN11,2,T_Spears,SF_CRUSH,&s_dspear5};
statetype s_dspear3 = {FALSE,SPEARDOWN10,2,T_Spears,SF_CRUSH,&s_dspear4};
statetype s_dspear2 = {FALSE,SPEARDOWN9,2,T_Spears,SF_DOWN,&s_dspear3};
statetype s_dspear1 = {FALSE,SPEARDOWN8,2,T_Spears,SF_DOWN,&s_dspear2};


statetype s_dariandead2 	= {FALSE,SPR_DARIAN_DEAD,0,T_BossDied,0,&s_dariandead2};
statetype s_dariandead1 	= {FALSE,SPR_DARIAN_DEAD,140,NULL,0,&s_dariandead2};
statetype s_dariandead 	= {FALSE,SPR_DARIAN_DEAD,0,NULL,0,&s_dariandead1};

statetype s_dariandie10 = {FALSE,SPR_DARIAN_DIE10,5,NULL,0,&s_dariandead};
statetype s_dariandie9 	= {FALSE,SPR_DARIAN_DIE9,5,NULL,0,&s_dariandie10};
statetype s_dariandie8 	= {FALSE,SPR_DARIAN_DIE8,5,NULL,0,&s_dariandie9};
statetype s_dariandie7 	= {FALSE,SPR_DARIAN_DIE7,5,NULL,0,&s_dariandie8};
statetype s_dariandie6 	= {FALSE,SPR_DARIAN_DIE6,5,T_Guts,0,&s_dariandie7};
statetype s_dariandie5 	= {FALSE,SPR_DARIAN_DIE5,5,NULL,0,&s_dariandie6};
statetype s_dariandie4 	= {FALSE,SPR_DARIAN_DIE4,5,NULL,0,&s_dariandie5};
statetype s_dariandie3 	= {FALSE,SPR_DARIAN_DIE3,5,NULL,0,&s_dariandie4};
statetype s_dariandie2 	= {FALSE,SPR_DARIAN_DIE2,5,NULL,0,&s_dariandie3};
statetype s_dariandie1 	= {FALSE,SPR_DARIAN_DIE1,70,T_Collide,0,&s_dariandie2};



/*
===========================================================================

									HEINRICH KRIST

===========================================================================
*/


statetype s_heinrichstand	= {TRUE,SPR_KRIST_S1,0,T_Stand,0,&s_heinrichstand};

statetype s_heinrichchase 	= {TRUE,SPR_KRIST_FOR1,10,T_HeinrichChase,0,&s_heinrichchase};


statetype s_kristleft = {TRUE,SPR_KRIST_LEFT1,10,T_KristLeft,0,&s_kristleft};

statetype s_kristright = {TRUE,SPR_KRIST_RIGHT1,10,T_KristRight,0,&s_kristright};

statetype s_heinrichshoot11  = {FALSE,SPR_KRIST_SHOOT11,8,NULL,0,&s_heinrichchase};
statetype s_heinrichshoot10  = {FALSE,SPR_KRIST_SHOOT10,8,NULL,0,&s_heinrichshoot11};
statetype s_heinrichshoot9  = {FALSE,SPR_KRIST_SHOOT9,8,A_HeinrichShoot,0,&s_heinrichshoot10};
statetype s_heinrichshoot8  = {FALSE,SPR_KRIST_SHOOT8,8,NULL,0,&s_heinrichshoot9};
statetype s_heinrichshoot7  = {FALSE,SPR_KRIST_SHOOT7,8,T_KristCheckFire,0,&s_heinrichshoot8};
statetype s_heinrichshoot6  = {FALSE,SPR_KRIST_SHOOT6,8,NULL,0,&s_heinrichshoot7};
statetype s_heinrichshoot5  = {FALSE,SPR_KRIST_SHOOT5,8,NULL,0,&s_heinrichshoot6};
statetype s_heinrichshoot4  = {FALSE,SPR_KRIST_SHOOT4,8,A_HeinrichShoot,0,&s_heinrichshoot5};
statetype s_heinrichshoot3  = {FALSE,SPR_KRIST_SHOOT3,8,NULL,0,&s_heinrichshoot4};
statetype s_heinrichshoot2  = {FALSE,SPR_KRIST_SHOOT2,8,NULL,0,&s_heinrichshoot3};
statetype s_heinrichshoot1  = {FALSE,SPR_KRIST_SHOOT1,8,T_KristCheckFire,0,&s_heinrichshoot2};

statetype s_missile1       = {16,SPR_BJMISS11,6,T_Projectile,0,&s_missile1};


statetype s_missilehit3    = {FALSE,SPR_MISSILEHIT3,6,NULL,0,NULL};
statetype s_missilehit2    = {FALSE,SPR_MISSILEHIT2,6,NULL,0,&s_missilehit3};
statetype s_missilehit1    = {FALSE,SPR_MISSILEHIT1,6,NULL,0,&s_missilehit2};

statetype s_mine4          = {FALSE,SPR_MINE4,3,T_Projectile,0,&s_mine1};
statetype s_mine3          = {FALSE,SPR_MINE3,3,T_Projectile,0,&s_mine4};
statetype s_mine2          = {FALSE,SPR_MINE2,3,T_Projectile,0,&s_mine3};
statetype s_mine1          = {FALSE,SPR_MINE1,3,T_Projectile,0,&s_mine2};

statetype s_heinrichdefend = {TRUE,SPR_KRIST_MINERIGHT1,35,T_Heinrich_Defend,0,&s_heinrichdefend};


statetype s_heinrichooc    = {TRUE,SPR_KRIST_DEAD1,0,T_Heinrich_Out_of_Control,0,&s_heinrichooc};

statetype s_heinrichdead 	= {FALSE,SPR_KRIST_DEAD1,35,T_Collide,0,&s_heinrichooc};

statetype s_heinrichdie2 	= {FALSE,SPR_KRIST_DIE2,35,T_Collide,0,&s_heinrichdead};
statetype s_heinrichdie1 	= {FALSE,SPR_KRIST_DIE1,35,T_Collide,0,&s_heinrichdie2};

statetype s_heindead2 = {FALSE,-1,0,T_BossDied,0,NULL};
statetype s_heindead1 = {FALSE, -1, 140,NULL,0,&s_heindead2};
statetype s_heinexp13 = {FALSE,SPR_EXPLOSION13,2,NULL,0,&s_heindead1};
statetype s_heinexp12 = {FALSE,SPR_EXPLOSION12,2,NULL,0,&s_heinexp13};
statetype s_heinexp11 = {FALSE,SPR_EXPLOSION11,2,NULL,0,&s_heinexp12};
statetype s_heinexp10 = {FALSE,SPR_EXPLOSION10,2,NULL,0,&s_heinexp11};
statetype s_heinexp9 = {FALSE,SPR_EXPLOSION9,2,NULL,0,&s_heinexp10};
statetype s_heinexp8 = {FALSE,SPR_EXPLOSION8,2,NULL,0,&s_heinexp9};
statetype s_heinexp7 = {FALSE,SPR_EXPLOSION7,2,NULL,0,&s_heinexp8};
statetype s_heinexp6 = {FALSE,SPR_EXPLOSION6,2,NULL,0,&s_heinexp7};
statetype s_heinexp5 = {FALSE,SPR_EXPLOSION5,2,NULL,0,&s_heinexp6};
statetype s_heinexp4 = {FALSE,SPR_EXPLOSION4,2,NULL,0,&s_heinexp5};
statetype s_heinexp3 = {FALSE,SPR_EXPLOSION3,2,T_Explosion,0,&s_heinexp4};
statetype s_heinexp2 = {FALSE,SPR_EXPLOSION2,2,NULL,0,&s_heinexp3};
statetype s_heinexp1 = {FALSE,SPR_EXPLOSION1,0,NULL,0,&s_heinexp2};


/*
===========================================================================

								  DARK MONK (TOM)

===========================================================================
*/


statetype s_darkmonkstand	= {TRUE,TOMS1,0,T_Stand,0,&s_darkmonkstand};

statetype s_darkmonkland	= {TRUE,TOMFLY11,6,NULL,0,&s_darkmonkstand};


statetype s_darkmonkchase2	= {TRUE,TOMFLY21,10,T_DarkmonkChase,0,&s_darkmonkchase2};
statetype s_darkmonkchase1	= {TRUE,TOMFLY11,3,T_DarkmonkChase,0,&s_darkmonkchase2};

statetype s_dmlandandfire = {TRUE,TOMFLY11,0,T_DarkmonkLandAndFire,0,&s_dmlandandfire};

statetype s_darkmonkcover3	= {FALSE,TAWAKEN1,1,NULL,0,&s_darkmonkawaken1};
statetype s_darkmonkcover2	= {FALSE,TAWAKEN2,1,NULL,0,&s_darkmonkcover3};
statetype s_darkmonkcover1	= {FALSE,TAWAKEN3,1,NULL,0,&s_darkmonkcover2};


statetype s_darkmonkawaken5	= {FALSE,TAWAKEN5,3,NULL,0,&s_darkmonkchase1};
statetype s_darkmonkawaken4	= {FALSE,TAWAKEN4,3,NULL,0,&s_darkmonkawaken5};
statetype s_darkmonkawaken3	= {FALSE,TAWAKEN3,3,NULL,0,&s_darkmonkawaken4};
statetype s_darkmonkawaken2	= {FALSE,TAWAKEN2,3,NULL,0,&s_darkmonkawaken3};
statetype s_darkmonkawaken1 	= {FALSE,TAWAKEN1,3,NULL,0,&s_darkmonkawaken2};


statetype s_darkmonklightning11	= {FALSE,TOMLG11,3,NULL,0,&s_darkmonkchase1};
statetype s_darkmonklightning10	= {FALSE,TOMLG10,3,NULL,0,&s_darkmonklightning11};
statetype s_darkmonklightning9	= {FALSE,TOMLG9,70,A_DmonkAttack,0,&s_darkmonklightning10};
statetype s_darkmonklightning8	= {FALSE,TOMLG8,3,NULL,0,&s_darkmonklightning9};
statetype s_darkmonklightning7 	= {FALSE,TOMLG7,3,NULL,0,&s_darkmonklightning8};
statetype s_darkmonklightning6	= {FALSE,TOMLG6,3,NULL,0,&s_darkmonklightning7};
statetype s_darkmonklightning5	= {FALSE,TOMLG5,3,NULL,0,&s_darkmonklightning6};
statetype s_darkmonklightning4	= {FALSE,TOMLG4,3,NULL,0,&s_darkmonklightning5};
statetype s_darkmonklightning3	= {FALSE,TOMLG3,3,NULL,0,&s_darkmonklightning4};
statetype s_darkmonklightning2 	= {FALSE,TOMLG2,3,NULL,0,&s_darkmonklightning3};
statetype s_darkmonklightning1 	= {FALSE,TOMLG1,3,NULL,0,&s_darkmonklightning2};

statetype s_darkmonkfspark6	= {FALSE,TOMFS6,3,NULL,0,&s_darkmonkchase1};
statetype s_darkmonkfspark5	= {FALSE,TOMFS5,70,A_DmonkAttack,0,&s_darkmonkfspark6};
statetype s_darkmonkfspark4	= {FALSE,TOMFS4,3,NULL,0,&s_darkmonkfspark5};
statetype s_darkmonkfspark3	= {FALSE,TOMFS3,3,NULL,0,&s_darkmonkfspark4};
statetype s_darkmonkfspark2 	= {FALSE,TOMFS2,3,NULL,0,&s_darkmonkfspark3};
statetype s_darkmonkfspark1 	= {FALSE,TOMFS1,3,NULL,0,&s_darkmonkfspark2};


statetype s_darkmonkbreathe8	= {FALSE,TOMBR8,3,NULL,0,&s_darkmonkchase1};
statetype s_darkmonkbreathe7 	= {FALSE,TOMBR7,3,NULL,0,&s_darkmonkbreathe8};
statetype s_darkmonkbreathe6	= {FALSE,TOMBR6,70,A_DmonkAttack,0,&s_darkmonkbreathe7};
statetype s_darkmonkbreathe5	= {FALSE,TOMBR5,3,NULL,0,&s_darkmonkbreathe6};
statetype s_darkmonkbreathe4	= {FALSE,TOMBR4,3,NULL,0,&s_darkmonkbreathe5};
statetype s_darkmonkbreathe3	= {FALSE,TOMBR3,3,NULL,0,&s_darkmonkbreathe4};
statetype s_darkmonkbreathe2 	= {FALSE,TOMBR2,3,NULL,0,&s_darkmonkbreathe3};
statetype s_darkmonkbreathe1 	= {FALSE,TOMBR1,70,NULL,0,&s_darkmonkbreathe2};

statetype s_darkmonksummon3	= {FALSE,TOMBR1,3,NULL,0,&s_darkmonkchase1};
statetype s_darkmonksummon2 	= {FALSE,TOMBR3,3,NULL,0,&s_darkmonksummon3};
statetype s_darkmonksummon1 	= {FALSE,TOMBR2,3,NULL,0,&s_darkmonksummon2};

statetype s_snakepath = {TRUE,TOMHEAD1,0,T_SnakePath,0,&s_snakepath};
statetype s_snakefindpath = {TRUE,TOMHEAD1,0,T_SnakeFindPath,0,&s_snakefindpath};

statetype s_darkmonkhead	= {TRUE,TOMHEAD1,0,T_DarkSnakeChase,0,&s_darkmonkhead};
statetype s_darkmonksnakelink	= {TRUE,TOMHEAD1,0,T_GenericMove,0,&s_darkmonksnakelink};


statetype s_darkmonkhspawn = {FALSE,NOTHING,78,T_DarkSnakeSpawn,0,NULL};
statetype s_darkmonkfastspawn = {FALSE,NOTHING,35,T_DarkSnakeSpawn,0,NULL};

statetype s_darkmonkheaddead	= {FALSE,THDIE2,0,NULL,0,&s_darkmonkheaddead};
statetype s_darkmonkheaddie1	= {FALSE,THDIE1,1400,T_SnakeFinale,0,&s_snakefireworks1};

statetype s_snakefireworks2 = {FALSE,THDIE2,10,T_SnakeFinale,0,&s_snakefireworks1};
statetype s_snakefireworks1 = {FALSE,THDIE1,10,T_SnakeFinale,0,&s_snakefireworks2};

statetype s_darkmonkhball9	= {FALSE,THBALL9,5,NULL,0,&s_darkmonkchase1};
statetype s_darkmonkhball8	= {FALSE,THBALL8,5,NULL,0,&s_darkmonkhball9};
statetype s_darkmonkhball7 = {FALSE,THBALL7,5,A_DmonkAttack,0,&s_darkmonkhball8};
statetype s_darkmonkhball6	= {FALSE,THBALL6,5,NULL,0,&s_darkmonkhball7};
statetype s_darkmonkhball5	= {FALSE,THBALL5,5,NULL,0,&s_darkmonkhball6};
statetype s_darkmonkhball4	= {FALSE,THBALL4,5,NULL,0,&s_darkmonkhball5};
statetype s_darkmonkhball3	= {FALSE,THBALL3,5,NULL,0,&s_darkmonkhball4};
statetype s_darkmonkhball2 = {FALSE,THBALL2,5,NULL,0,&s_darkmonkhball3};
statetype s_darkmonkhball1 = {FALSE,THBALL1,70,NULL,0,&s_darkmonkhball2};

statetype s_darkmonkabsorb9	= {FALSE,THBALL9,3,NULL,0,&s_darkmonksphere1};
statetype s_darkmonkabsorb8	= {FALSE,THBALL8,3,NULL,0,&s_darkmonkabsorb9};
statetype s_darkmonkabsorb7 = {FALSE,THBALL7,3,NULL,0,&s_darkmonkabsorb8};
statetype s_darkmonkabsorb6	= {FALSE,THBALL6,3,NULL,0,&s_darkmonkabsorb7};
statetype s_darkmonkabsorb5	= {FALSE,THBALL5,3,NULL,0,&s_darkmonkabsorb6};
statetype s_darkmonkabsorb4	= {FALSE,THBALL4,3,NULL,0,&s_darkmonkabsorb5};
statetype s_darkmonkabsorb3	= {FALSE,THBALL3,3,NULL,0,&s_darkmonkabsorb4};
statetype s_darkmonkabsorb2 = {FALSE,THBALL2,3,NULL,0,&s_darkmonkabsorb3};
statetype s_darkmonkabsorb1 = {FALSE,THBALL1,3,NULL,0,&s_darkmonkabsorb2};


statetype s_darkmonksphere10 = {FALSE,TSPHERE10,4,NULL,0,&s_darkmonkchase1};
statetype s_darkmonksphere9 = {FALSE,TSPHERE9,4,NULL,0,&s_darkmonksphere10};
statetype s_darkmonksphere8 = {FALSE,TSPHERE8,4,A_DmonkAttack,0,&s_darkmonksphere9};
statetype s_darkmonksphere7 = {FALSE,TSPHERE7,4,NULL,0,&s_darkmonksphere8};
statetype s_darkmonksphere6 = {FALSE,TSPHERE6,4,NULL,0,&s_darkmonksphere7};
statetype s_darkmonksphere5 = {FALSE,TSPHERE5,4,NULL,0,&s_darkmonksphere6};
statetype s_darkmonksphere4 = {FALSE,TSPHERE4,4,NULL,0,&s_darkmonksphere5};
statetype s_darkmonksphere3 = {FALSE,TSPHERE3,4,NULL,0,&s_darkmonksphere4};
statetype s_darkmonksphere2 = {FALSE,TSPHERE2,4,NULL,0,&s_darkmonksphere3};
statetype s_darkmonksphere1 = {FALSE,TSPHERE1,4,NULL,0,&s_darkmonksphere2};

statetype s_dmgreenthing10 = {FALSE,TSPHERE10,4,NULL,0,&s_darkmonkchase1};
statetype s_dmgreenthing9 = {FALSE,TSPHERE9,4,NULL,0,&s_dmgreenthing10};
statetype s_dmgreenthing8 = {FALSE,TSPHERE8,4,A_DmonkAttack,0,&s_dmgreenthing9};
statetype s_dmgreenthing7 = {FALSE,TSPHERE7,4,NULL,0,&s_dmgreenthing8};
statetype s_dmgreenthing6 = {FALSE,TSPHERE6,4,NULL,0,&s_dmgreenthing7};
statetype s_dmgreenthing5 = {FALSE,TSPHERE5,4,NULL,0,&s_dmgreenthing6};
statetype s_dmgreenthing4 = {FALSE,TSPHERE4,4,NULL,0,&s_dmgreenthing5};
statetype s_dmgreenthing3 = {FALSE,TSPHERE3,4,NULL,0,&s_dmgreenthing4};
statetype s_dmgreenthing2 = {FALSE,TSPHERE2,4,NULL,0,&s_dmgreenthing3};
statetype s_dmgreenthing1 = {FALSE,TSPHERE1,4,NULL,0,&s_dmgreenthing2};


statetype s_energysphere4 = {FALSE,TOMSPHERE4,4,T_Projectile,0,&s_energysphere1};
statetype s_energysphere3 = {FALSE,TOMSPHERE3,4,T_Projectile,0,&s_energysphere4};
statetype s_energysphere2 = {FALSE,TOMSPHERE2,4,T_Projectile,0,&s_energysphere3};
statetype s_energysphere1 = {FALSE,TOMSPHERE1,4,T_Projectile,0,&s_energysphere2};


statetype s_lightning = {TRUE,TOMLIGHTNING1,0,T_Projectile,0,&s_lightning};


statetype s_handball2 = {FALSE,TOMHANDBALL2,3,T_Projectile,0,&s_handball1};
statetype s_handball1 = {FALSE,TOMHANDBALL1,3,T_Projectile,0,&s_handball2};

statetype s_faceball2 = {FALSE,TOMFACEBALL2,3,T_Projectile,0,&s_faceball1};
statetype s_faceball1 = {FALSE,TOMFACEBALL1,3,T_Projectile,0,&s_faceball2};

statetype s_floorspark4 = {FALSE,TOMFLOORSPARK4,3,T_Projectile,0,&s_floorspark1};
statetype s_floorspark3 = {FALSE,TOMFLOORSPARK3,3,T_Projectile,0,&s_floorspark4};
statetype s_floorspark2 = {FALSE,TOMFLOORSPARK2,3,T_Projectile,0,&s_floorspark3};
statetype s_floorspark1 = {FALSE,TOMFLOORSPARK1,3,T_Projectile,0,&s_floorspark2};



statetype s_darkmonkreact = {TRUE,TOMFLY11,0,T_DarkmonkReact,0,&s_darkmonkreact};

statetype s_darkmonkbball9	= {FALSE,TBBALL9,4,NULL,0,&s_darkmonkchase1};
statetype s_darkmonkbball8	= {FALSE,TBBALL8,4,NULL,0,&s_darkmonkbball9};
statetype s_darkmonkbball7 = {FALSE,TBBALL7,4,A_DmonkAttack,0,&s_darkmonkbball8};
statetype s_darkmonkbball6	= {FALSE,TBBALL6,4,NULL,0,&s_darkmonkbball7};
statetype s_darkmonkbball5	= {FALSE,TBBALL5,4,NULL,0,&s_darkmonkbball6};
statetype s_darkmonkbball4	= {FALSE,TBBALL4,4,NULL,0,&s_darkmonkbball5};
statetype s_darkmonkbball3	= {FALSE,TBBALL3,4,NULL,0,&s_darkmonkbball4};
statetype s_darkmonkbball2 = {FALSE,TBBALL2,4,NULL,0,&s_darkmonkbball3};
statetype s_darkmonkbball1 = {FALSE,TBBALL1,70,NULL,0,&s_darkmonkbball2};


statetype s_darkmonkcharge10 = {FALSE,TSCAREB3,140,T_DarkmonkCharge,0,&s_darkmonkchase1};
statetype s_darkmonkcharge9	= {FALSE,TAWAKEN5,3,NULL,0,&s_darkmonkcharge10};
statetype s_darkmonkcharge8	= {FALSE,TAWAKEN4,3,NULL,0,&s_darkmonkcharge9};
statetype s_darkmonkcharge7	= {FALSE,TAWAKEN3,3,NULL,0,&s_darkmonkcharge8};
statetype s_darkmonkcharge6 	= {FALSE,TAWAKEN2,3,NULL,0,&s_darkmonkcharge7};
statetype s_darkmonkcharge5	= {FALSE,TAWAKEN1,3,NULL,0,&s_darkmonkcharge6};
statetype s_darkmonkcharge4	= {FALSE,TAWAKEN2,3,NULL,0,&s_darkmonkcharge5};
statetype s_darkmonkcharge3	= {FALSE,TAWAKEN3,3,NULL,0,&s_darkmonkcharge4};
statetype s_darkmonkcharge2	= {FALSE,TAWAKEN4,3,NULL,0,&s_darkmonkcharge3};
statetype s_darkmonkcharge1 	= {FALSE,TAWAKEN5,3,NULL,0,&s_darkmonkcharge2};


statetype s_darkmonkscare5	= {FALSE,TSCAREB5,3,NULL,0,&s_darkmonkcharge1};
statetype s_darkmonkscare4	= {FALSE,TSCAREB4,3,NULL,0,&s_darkmonkscare5};
statetype s_darkmonkscare3	= {FALSE,TSCAREB3,3,NULL,0,&s_darkmonkscare4};
statetype s_darkmonkscare2 = {FALSE,TSCAREB2,3,NULL,0,&s_darkmonkscare3};
statetype s_darkmonkscare1 = {FALSE,TSCAREB1,3,NULL,0,&s_darkmonkscare2};

statetype s_darkmonkdead = {FALSE,TOMDIE8,0,T_Collide,0,&s_darkmonkdead};
statetype s_darkmonkdie7 = {FALSE,TOMDIE7,3,T_Collide,0,&s_darkmonkdead};
statetype s_darkmonkdie6 = {FALSE,TOMDIE6,3,T_Collide,0,&s_darkmonkdie7};
statetype s_darkmonkdie5 = {FALSE,TOMDIE5,3,T_Collide,0,&s_darkmonkdie6};
statetype s_darkmonkdie4 = {FALSE,TOMDIE4,3,T_Collide,0,&s_darkmonkdie5};
statetype s_darkmonkdie3 = {FALSE,TOMDIE3,3,T_Collide,0,&s_darkmonkdie4};
statetype s_darkmonkdie2 = {FALSE,TOMDIE2,3,T_Collide,0,&s_darkmonkdie3};
statetype s_darkmonkdie1 = {FALSE,TOMDIE1,3,T_Collide,0,&s_darkmonkdie2};



statetype s_darkmonkredhead = {TRUE,TOMRH1,50,T_DarkSnakeChase,0,&s_darkmonkhead};
statetype s_darkmonkredlink = {TRUE,TOMRH1,50,T_GenericMove,0,&s_darkmonksnakelink};

statetype s_redheadhit = {FALSE,THDIE1,35,T_DarkSnakeChase,0,&s_darkmonkhead};
statetype s_redlinkhit = {FALSE,THDIE1,35,T_GenericMove,0,&s_darkmonksnakelink};


statetype s_spithit4 = {FALSE,SPITHIT4,3,NULL,0,NULL};
statetype s_spithit3 = {FALSE,SPITHIT3,3,NULL,0,&s_spithit4};
statetype s_spithit2 = {FALSE,SPITHIT2,3,NULL,0,&s_spithit3};
statetype s_spithit1 = {FALSE,SPITHIT1,3,NULL,0,&s_spithit2};

statetype s_spit4 = {FALSE,TOMSPIT4,3,T_Projectile,0,&s_spit1};
statetype s_spit3 = {FALSE,TOMSPIT3,3,T_Projectile,0,&s_spit4};
statetype s_spit2 = {FALSE,TOMSPIT2,3,T_Projectile,0,&s_spit3};
statetype s_spit1 = {FALSE,TOMSPIT1,3,T_Projectile,0,&s_spit2};

statetype s_snakefire2 = {TRUE,TOMHEAD1,1,T_DarkSnakeChase,SF_DOWN|SF_UP,&s_darkmonkhead};
statetype s_snakefire1 = {FALSE,TPREPARE,30,T_DarkSnakeChase,SF_DOWN,&s_snakefire2};

statetype s_dexplosion22 = {FALSE,-1,0,T_BossDied,0,NULL};
statetype s_dexplosion21 = {FALSE, -1, 240,NULL,0,&s_dexplosion22};
statetype s_dexplosion20 = {FALSE,SPR_EXPLOSION20,2,NULL,0,&s_dexplosion21};
statetype s_dexplosion19 = {FALSE,SPR_EXPLOSION19,2,NULL,0,&s_dexplosion20};
statetype s_dexplosion18 = {FALSE,SPR_EXPLOSION18,2,NULL,0,&s_dexplosion19};
statetype s_dexplosion17 = {FALSE,SPR_EXPLOSION17,2,NULL,0,&s_dexplosion18};
statetype s_dexplosion16 = {FALSE,SPR_EXPLOSION16,2,NULL,0,&s_dexplosion17};
statetype s_dexplosion15 = {FALSE,SPR_EXPLOSION15,2,NULL,0,&s_dexplosion16};
statetype s_dexplosion14 = {FALSE,SPR_EXPLOSION14,2,NULL,0,&s_dexplosion15};
statetype s_dexplosion13 = {FALSE,SPR_EXPLOSION13,2,NULL,0,&s_dexplosion14};
statetype s_dexplosion12 = {FALSE,SPR_EXPLOSION12,2,NULL,0,&s_dexplosion13};
statetype s_dexplosion11 = {FALSE,SPR_EXPLOSION11,2,NULL,0,&s_dexplosion12};
statetype s_dexplosion10 = {FALSE,SPR_EXPLOSION10,2,NULL,0,&s_dexplosion11};
statetype s_dexplosion9 = {FALSE,SPR_EXPLOSION9,2,NULL,0,&s_dexplosion10};
statetype s_dexplosion8 = {FALSE,SPR_EXPLOSION8,2,NULL,0,&s_dexplosion9};
statetype s_dexplosion7 = {FALSE,SPR_EXPLOSION7,2,NULL,0,&s_dexplosion8};
statetype s_dexplosion6 = {FALSE,SPR_EXPLOSION6,2,NULL,0,&s_dexplosion7};
statetype s_dexplosion5 = {FALSE,SPR_EXPLOSION5,2,NULL,0,&s_dexplosion6};
statetype s_dexplosion4 = {FALSE,SPR_EXPLOSION4,2,NULL,0,&s_dexplosion5};
statetype s_dexplosion3 = {FALSE,SPR_EXPLOSION3,2,T_Explosion,0,&s_dexplosion4};
statetype s_dexplosion2 = {FALSE,SPR_EXPLOSION2,2,NULL,0,&s_dexplosion3};
statetype s_dexplosion1 = {FALSE,SPR_EXPLOSION1,2,NULL,0,&s_dexplosion2};


/*
===========================================================================

									OROBOT

===========================================================================
*/

statetype s_NMEdeathbuildup = {16,NMEBODY1_01,210,T_NME_Explode,0,NULL};
statetype s_NMEheadexplosion = {16,NMEHEAD1_01,140,T_Special,0,&s_grexplosion1};

statetype s_NMEstand = {16,NMEBODY1_01,0,T_Stand,0,&s_NMEstand};
statetype s_NMEhead1 = {16,NMEHEAD1_01,0,NULL,0,&s_NMEhead1};
statetype s_NMEhead2 = {16,NMEHEAD2_01,0,NULL,0,&s_NMEhead2};
statetype s_NMEchase = {16,NMEBODY1_01,20,T_OrobotChase,0,&s_NMEchase};
statetype s_NMEwheels1 = {16,NMEWHEEL1_01,0,NULL,0,&s_NMEwheels1};
statetype s_NMEwheels2 = {16,NMEWHEEL2_01,0,NULL,0,&s_NMEwheels2};
statetype s_NMEwheels3 = {16,NMEWHEEL3_01,0,NULL,0,&s_NMEwheels3};
statetype s_NMEwheels4 = {16,NMEWHEEL4_01,0,NULL,0,&s_NMEwheels4};
statetype s_NMEwheels5 = {16,NMEWHEEL5_01,0,NULL,0,&s_NMEwheels5};

statetype s_shootinghead = {16,NMEHEAD1_01,140,T_NME_HeadShoot,0,&s_shootinghead};

statetype s_NMEspinattack = {16,NMEBODY1_01,70,T_NME_SpinAttack,0,&s_NMEchase};
statetype s_NMEwheelspin = {16,NMEWHEEL1_01,70,NULL,0,&s_NMEwheels2};

statetype s_NMEminiball4 = {FALSE,NMEMINIBALL_04,4,T_Projectile,0,&s_NMEminiball1};
statetype s_NMEminiball3 = {FALSE,NMEMINIBALL_03,4,T_Projectile,0,&s_NMEminiball4};
statetype s_NMEminiball2 = {FALSE,NMEMINIBALL_02,4,T_Projectile,0,&s_NMEminiball3};
statetype s_NMEminiball1 = {FALSE,NMEMINIBALL_01,4,T_Projectile,0,&s_NMEminiball2};

statetype s_NMEsaucer4 = {FALSE,NMESAUCER_04,6,T_Saucer,0,&s_NMEsaucer1};
statetype s_NMEsaucer3 = {FALSE,NMESAUCER_03,6,T_Saucer,0,&s_NMEsaucer4};
statetype s_NMEsaucer2 = {FALSE,NMESAUCER_02,6,T_Saucer,0,&s_NMEsaucer3};
statetype s_NMEsaucer1 = {FALSE,NMESAUCER_01,6,T_Saucer,SF_SOUND,&s_NMEsaucer2};

statetype s_NMEdie = {16,NMEBODY1_01,0,T_Collide,0,&s_NMEdie};


statetype s_NMEspinfire = {16,NMEBODY1_01,0,T_NME_SpinFire,0,&s_NMEspinfire};
statetype s_NMEattack = {16,NMEBODY1_01,0,T_NME_Attack,0,&s_NMEattack};
statetype s_NMEhead1rl = {16,NMEROCKET_01,0,NULL,0,&s_NMEhead1rl};
statetype s_NMEhead2rl = {16,NMEROCKET2_01,0,NULL,0,&s_NMEhead2rl};

statetype s_NMEwindup = {16,NMEBODY1_01,0,T_NME_WindUp,0,&s_NMEwindup};

statetype s_NMEwheels120 = {16,NMEWHEEL1_01,0,NULL,0,&s_NMEwheels120};

statetype s_NMEwrotleft3 = {16,NMEWHEEL4_01,10,NULL,0,&s_NMEwrotleft3};
statetype s_NMEwrotleft2 = {16,NMEWHEEL3_01,10,NULL,0,&s_NMEwrotleft3};
statetype s_NMEwrotleft1 = {16,NMEWHEEL2_01,10,NULL,0,&s_NMEwrotleft2};

statetype s_NMEwrotright3 = {16,NMEWHEEL4_01,10,NULL,0,&s_NMEwrotleft3};
statetype s_NMEwrotright2 = {16,NMEWHEEL5_01,10,NULL,0,&s_NMEwrotleft3};
statetype s_NMEwrotright1 = {16,NMEWHEEL2_01,10,NULL,0,&s_NMEwrotleft2};

statetype  s_oshuriken4 = {FALSE,SPR_OSHUR4,1,T_Projectile,0,&s_oshuriken1};
statetype  s_oshuriken3 = {FALSE,SPR_OSHUR3,1,T_Projectile,0,&s_oshuriken4};
statetype  s_oshuriken2 = {FALSE,SPR_OSHUR2,1,T_Projectile,0,&s_oshuriken3};
statetype  s_oshuriken1 = {FALSE,SPR_OSHUR1,1,T_Projectile,0,&s_oshuriken2};

statetype  s_oshurikenhit3 = {FALSE,SPR_OSHURHIT3,3,NULL,0,NULL};
statetype  s_oshurikenhit2 = {FALSE,SPR_OSHURHIT2,3,NULL,0,&s_oshurikenhit3};
statetype  s_oshurikenhit1 = {FALSE,SPR_OSHURHIT1,3,NULL,0,&s_oshurikenhit2};

statetype s_speardown16 = {FALSE,SPEARDOWN16,2,T_Spears,SF_CRUSH,&s_speardown1};
statetype s_speardown15 = {FALSE,SPEARDOWN15,2,T_Spears,SF_CRUSH,&s_speardown16};
statetype s_speardown14 = {FALSE,SPEARDOWN14,2,T_Spears,SF_CRUSH,&s_speardown15};
statetype s_speardown13 = {FALSE,SPEARDOWN13,2,T_Spears,SF_CRUSH,&s_speardown14};
statetype s_speardown12 = {FALSE,SPEARDOWN12,2,T_Spears,SF_CRUSH,&s_speardown13};
statetype s_speardown11 = {FALSE,SPEARDOWN11,2,T_Spears,SF_CRUSH,&s_speardown12};
statetype s_speardown10 = {FALSE,SPEARDOWN10,2,T_Spears,SF_CRUSH,&s_speardown11};
statetype s_speardown9 = {FALSE,SPEARDOWN9,2,T_Spears,SF_DOWN,&s_speardown10};

statetype s_speardown8 = {FALSE,SPEARDOWN8,35,T_Spears,SF_DOWN|SF_SOUND,&s_speardown9};
statetype s_speardown7 = {FALSE,SPEARDOWN7,2,T_Spears,SF_DOWN,&s_speardown8};
statetype s_speardown6 = {FALSE,SPEARDOWN6,2,T_Spears,SF_CRUSH,&s_speardown7};
statetype s_speardown5 = {FALSE,SPEARDOWN5,2,T_Spears,SF_CRUSH,&s_speardown6};
statetype s_speardown4 = {FALSE,SPEARDOWN4,2,T_Spears,SF_CRUSH,&s_speardown5};
statetype s_speardown3 = {FALSE,SPEARDOWN3,2,T_Spears,SF_CRUSH,&s_speardown4};
statetype s_speardown2 = {FALSE,SPEARDOWN2,2,T_Spears,SF_CRUSH,&s_speardown3};
statetype s_speardown1 = {FALSE,SPEARDOWN1,2,T_Spears,SF_CRUSH,&s_speardown2};



statetype s_downblade16 = {FALSE,DBLADE8,1,T_Path,0,&s_downblade1};
statetype s_downblade15 = {FALSE,DBLADE7,2,T_Path,0,&s_downblade16};
statetype s_downblade14 = {FALSE,DBLADE6,1,T_Path,0,&s_downblade15};
statetype s_downblade13 = {FALSE,DBLADE5,2,T_Path,0,&s_downblade14};
statetype s_downblade12 = {FALSE,DBLADE4,1,T_Path,0,&s_downblade13};
statetype s_downblade11 = {FALSE,DBLADE3,2,T_Path,0,&s_downblade12};
statetype s_downblade10 = {FALSE,DBLADE2,1,T_Path,0,&s_downblade11};
statetype s_downblade9 = {FALSE,DBLADE9,2,T_Path,0,&s_downblade10};
statetype s_downblade8 = {FALSE,DBLADE8,1,T_Path,0,&s_downblade9};
statetype s_downblade7 = {FALSE,DBLADE7,2,T_Path,0,&s_downblade8};
statetype s_downblade6 = {FALSE,DBLADE6,1,T_Path,0,&s_downblade7};
statetype s_downblade5 = {FALSE,DBLADE5,2,T_Path,0,&s_downblade6};
statetype s_downblade4 = {FALSE,DBLADE4,1,T_Path,0,&s_downblade5};
statetype s_downblade3 = {FALSE,DBLADE3,2,T_Path,0,&s_downblade4};
statetype s_downblade2 = {FALSE,DBLADE2,1,T_Path,0,&s_downblade3};
statetype s_downblade1 = {FALSE,DBLADE1,2,T_Path,SF_SOUND,&s_downblade2};


statetype s_firejetdown23 = {FALSE,FIREJETDOWN23,3,T_Path,SF_CRUSH,&s_firejetdown1};
statetype s_firejetdown22 = {FALSE,FIREJETDOWN22,3,T_Path,SF_CRUSH,&s_firejetdown23};
statetype s_firejetdown21 = {FALSE,FIREJETDOWN21,3,T_Path,0,&s_firejetdown22};
statetype s_firejetdown20 = {FALSE,FIREJETDOWN20,3,T_Path,0,&s_firejetdown21};
statetype s_firejetdown19 = {FALSE,FIREJETDOWN19,3,T_Path,0,&s_firejetdown20};
statetype s_firejetdown18 = {FALSE,FIREJETDOWN18,3,T_Path,0,&s_firejetdown19};
statetype s_firejetdown17 = {FALSE,FIREJETDOWN17,3,T_Path,0,&s_firejetdown18};
statetype s_firejetdown16 = {FALSE,FIREJETDOWN16,3,T_Path,SF_CRUSH,&s_firejetdown17};
statetype s_firejetdown15 = {FALSE,FIREJETDOWN15,3,T_Path,SF_CRUSH,&s_firejetdown16};
statetype s_firejetdown14 = {FALSE,FIREJETDOWN14,3,T_Path,SF_CRUSH,&s_firejetdown15};
statetype s_firejetdown13 = {FALSE,FIREJETDOWN13,3,T_Path,0,&s_firejetdown14};
statetype s_firejetdown12 = {FALSE,FIREJETDOWN12,3,T_Path,0,&s_firejetdown13};
statetype s_firejetdown11 = {FALSE,FIREJETDOWN11,3,T_Path,0,&s_firejetdown12};
statetype s_firejetdown10 = {FALSE,FIREJETDOWN10,3,T_Path,0,&s_firejetdown11};
statetype s_firejetdown9 = {FALSE,FIREJETDOWN9,3,T_Path,0,&s_firejetdown10};
statetype s_firejetdown8 = {FALSE,FIREJETDOWN8,3,T_Path,SF_CRUSH,&s_firejetdown9};
statetype s_firejetdown7 = {FALSE,FIREJETDOWN7,3,T_Path,SF_CRUSH,&s_firejetdown8};
statetype s_firejetdown6 = {FALSE,FIREJETDOWN6,3,T_Path,SF_CRUSH,&s_firejetdown7};
statetype s_firejetdown5 = {FALSE,FIREJETDOWN5,3,T_Path,0,&s_firejetdown6};
statetype s_firejetdown4 = {FALSE,FIREJETDOWN4,3,T_Path,0,&s_firejetdown5};
statetype s_firejetdown3 = {FALSE,FIREJETDOWN3,3,T_Path,0,&s_firejetdown4};
statetype s_firejetdown2 = {FALSE,FIREJETDOWN2,3,T_Path,0,&s_firejetdown3};
statetype s_firejetdown1 = {FALSE,FIREJETDOWN1,70,T_Path,SF_SOUND,&s_firejetdown2};



statetype s_columnupdown6 = {FALSE,CRUSHUP7,5,T_CrushUp,SF_DOWN,&s_columnupup1};
statetype s_columnupdown5 = {FALSE,CRUSHUP6,2,T_CrushUp,SF_DOWN,&s_columnupdown6};
statetype s_columnupdown4 = {FALSE,CRUSHUP5,5,T_CrushUp,SF_DOWN|SF_BLOCK,&s_columnupdown5};
statetype s_columnupdown3 = {FALSE,CRUSHUP4,5,T_CrushUp,SF_DOWN|SF_BLOCK,&s_columnupdown4};
statetype s_columnupdown2 = {FALSE,CRUSHUP3,5,T_CrushUp,SF_DOWN|SF_BLOCK,&s_columnupdown3};
statetype s_columnupdown1 = {FALSE,CRUSHUP2,2,T_CrushUp,SF_DOWN|SF_BLOCK,&s_columnupdown2};


statetype s_columnupup8 = {FALSE,CRUSHUP1,5,T_CrushUp,SF_CRUSH|SF_BLOCK,&s_columnupdown1};
statetype s_columnupup7 = {FALSE,CRUSHUP2,2,T_CrushUp,SF_CRUSH|SF_UP|SF_BLOCK,&s_columnupup8};
statetype s_columnupup6 = {FALSE,CRUSHUP3,5,T_CrushUp,SF_UP|SF_BLOCK,&s_columnupup7};
statetype s_columnupup5 = {FALSE,CRUSHUP4,5,T_CrushUp,SF_UP|SF_BLOCK,&s_columnupup6};
statetype s_columnupup4 = {FALSE,CRUSHUP5,5,T_CrushUp,SF_UP,&s_columnupup5};
statetype s_columnupup3 = {FALSE,CRUSHUP6,2,T_CrushUp,SF_UP,&s_columnupup4};
statetype s_columnupup2 = {FALSE,CRUSHUP7,5,T_CrushUp,SF_UP,&s_columnupup3};
statetype s_columnupup1 = {FALSE,CRUSHUP8,30,T_CrushUp,SF_SOUND,&s_columnupup2};



statetype s_spinupblade16 = {FALSE,SPINUBLADE_16,2,T_Path,SF_DOWN,&s_spinupblade1};
statetype s_spinupblade15 = {FALSE,SPINUBLADE_15,2,T_Path,0,&s_spinupblade16};
statetype s_spinupblade14 = {FALSE,SPINUBLADE_14,2,T_Path,0,&s_spinupblade15};
statetype s_spinupblade13 = {FALSE,SPINUBLADE_13,2,T_Path,0,&s_spinupblade14};
statetype s_spinupblade12 = {FALSE,SPINUBLADE_12,2,T_Path,0,&s_spinupblade13};
statetype s_spinupblade11 = {FALSE,SPINUBLADE_11,2,T_Path,0,&s_spinupblade12};
statetype s_spinupblade10 = {FALSE,SPINUBLADE_10,2,T_Path,0,&s_spinupblade11};
statetype s_spinupblade9 = {FALSE,SPINUBLADE_09,2,T_Path,0,&s_spinupblade10};

statetype s_spinupblade8 = {FALSE,SPINUBLADE_08,2,T_Path,0,&s_spinupblade9};
statetype s_spinupblade7 = {FALSE,SPINUBLADE_07,2,T_Path,0,&s_spinupblade8};
statetype s_spinupblade6 = {FALSE,SPINUBLADE_06,2,T_Path,0,&s_spinupblade7};
statetype s_spinupblade5 = {FALSE,SPINUBLADE_05,2,T_Path,0,&s_spinupblade6};
statetype s_spinupblade4 = {FALSE,SPINUBLADE_04,2,T_Path,0,&s_spinupblade5};
statetype s_spinupblade3 = {FALSE,SPINUBLADE_03,2,T_Path,0,&s_spinupblade4};
statetype s_spinupblade2 = {FALSE,SPINUBLADE_02,2,T_Path,0,&s_spinupblade3};
statetype s_spinupblade1 = {FALSE,SPINUBLADE_01,35,T_Path,SF_UP|SF_SOUND,&s_spinupblade2};


statetype s_spindownblade16 = {FALSE,SPINDBLADE_16,2,T_Path,SF_DOWN,&s_spindownblade1};
statetype s_spindownblade15 = {FALSE,SPINDBLADE_15,2,T_Path,0,&s_spindownblade16};
statetype s_spindownblade14 = {FALSE,SPINDBLADE_14,2,T_Path,0,&s_spindownblade15};
statetype s_spindownblade13 = {FALSE,SPINDBLADE_13,2,T_Path,0,&s_spindownblade14};
statetype s_spindownblade12 = {FALSE,SPINDBLADE_12,2,T_Path,0,&s_spindownblade13};
statetype s_spindownblade11 = {FALSE,SPINDBLADE_11,2,T_Path,0,&s_spindownblade12};
statetype s_spindownblade10 = {FALSE,SPINDBLADE_10,2,T_Path,0,&s_spindownblade11};
statetype s_spindownblade9 = {FALSE,SPINDBLADE_09,2,T_Path,0,&s_spindownblade10};

statetype s_spindownblade8 = {FALSE,SPINDBLADE_08,2,T_Path,0,&s_spindownblade9};
statetype s_spindownblade7 = {FALSE,SPINDBLADE_07,2,T_Path,0,&s_spindownblade8};
statetype s_spindownblade6 = {FALSE,SPINDBLADE_06,2,T_Path,0,&s_spindownblade7};
statetype s_spindownblade5 = {FALSE,SPINDBLADE_05,2,T_Path,0,&s_spindownblade6};
statetype s_spindownblade4 = {FALSE,SPINDBLADE_04,2,T_Path,0,&s_spindownblade5};
statetype s_spindownblade3 = {FALSE,SPINDBLADE_03,2,T_Path,0,&s_spindownblade4};
statetype s_spindownblade2 = {FALSE,SPINDBLADE_02,2,T_Path,0,&s_spindownblade3};
statetype s_spindownblade1 = {FALSE,SPINDBLADE_01,35,T_Path,SF_UP|SF_SOUND,&s_spindownblade2};


statetype s_bouldersink9 = {FALSE,BSINK9,2,NULL,0,NULL};
statetype s_bouldersink8 = {FALSE,BSINK8,2,NULL,0,&s_bouldersink9};
statetype s_bouldersink7 = {FALSE,BSINK7,2,NULL,0,&s_bouldersink8};
statetype s_bouldersink6 = {FALSE,BSINK6,2,NULL,0,&s_bouldersink7};
statetype s_bouldersink5 = {FALSE,BSINK5,2,NULL,0,&s_bouldersink6};
statetype s_bouldersink4 = {FALSE,BSINK4,2,NULL,0,&s_bouldersink5};
statetype s_bouldersink3 = {FALSE,BSINK3,2,NULL,0,&s_bouldersink4};
statetype s_bouldersink2 = {FALSE,BSINK2,2,NULL,0,&s_bouldersink3};
statetype s_bouldersink1 = {FALSE,BSINK1,2,NULL,0,&s_bouldersink2};


statetype s_boulderroll8 = {FALSE,BOULDER41,3,T_BoulderMove,0,&s_boulderroll1};
statetype s_boulderroll7 = {FALSE,BOULDER31,3,T_BoulderMove,0,&s_boulderroll8};
statetype s_boulderroll6 = {FALSE,BOULDER21,3,T_BoulderMove,0,&s_boulderroll7};
statetype s_boulderroll5 = {FALSE,BOULDER11,3,T_BoulderMove,0,&s_boulderroll6};
statetype s_boulderroll4 = {FALSE,BOULDER41,3,T_BoulderMove,0,&s_boulderroll5};
statetype s_boulderroll3 = {FALSE,BOULDER31,3,T_BoulderMove,0,&s_boulderroll4};
statetype s_boulderroll2 = {FALSE,BOULDER21,3,T_BoulderMove,0,&s_boulderroll3};
statetype s_boulderroll1 = {FALSE,BOULDER11,3,T_BoulderMove,SF_SOUND,&s_boulderroll2};




statetype s_boulderdrop12 = {FALSE,BOULDER11,0,T_BoulderDrop,0,&s_boulderdrop12};
statetype s_boulderdrop11 = {FALSE,BDROP11,1,T_BoulderDrop,0,&s_boulderdrop12};
statetype s_boulderdrop10 = {FALSE,BDROP10,1,T_BoulderDrop,0,&s_boulderdrop11};
statetype s_boulderdrop9 = {FALSE,BDROP9,2,T_BoulderDrop,0,&s_boulderdrop10};
statetype s_boulderdrop8 = {FALSE,BDROP8,1,T_BoulderDrop,0,&s_boulderdrop9};
statetype s_boulderdrop7 = {FALSE,BDROP7,1,T_BoulderDrop,0,&s_boulderdrop8};
statetype s_boulderdrop6 = {FALSE,BDROP6,2,T_BoulderDrop,0,&s_boulderdrop7};
statetype s_boulderdrop5 = {FALSE,BDROP5,3,T_BoulderDrop,0,&s_boulderdrop6};
statetype s_boulderdrop4 = {FALSE,BDROP4,4,T_BoulderDrop,0,&s_boulderdrop5};
statetype s_boulderdrop3 = {FALSE,BDROP3,5,T_BoulderDrop,SF_SOUND,&s_boulderdrop4};
statetype s_boulderdrop2 = {FALSE,BDROP2,6,NULL,0,&s_boulderdrop3};
statetype s_boulderdrop1 = {FALSE,BDROP1,6,NULL,0,&s_boulderdrop2};

statetype s_boulderspawn = {FALSE,NOTHING,70,T_BoulderSpawn,0,&s_boulderspawn};



/*==========================================================================

									GUN STUFF

============================================================================*/

statetype s_gunfire2 = {TRUE,GUNRISE51,5,A_GunShoot,0,&s_gunfire1};
statetype s_gunfire1 = {TRUE,GUNFIRE1,5,A_GunShoot,0,&s_gunfire2};

statetype s_gunstand = {TRUE,GUNRISE11,0,T_GunStand,0,&s_gunstand};

statetype s_gunraise4 = {TRUE,GUNRISE51,2,NULL,0,&s_gunfire1};
statetype s_gunraise3 = {TRUE,GUNRISE41,2,NULL,0,&s_gunraise4};
statetype s_gunraise2 = {TRUE,GUNRISE31,2,NULL,0,&s_gunraise3};
statetype s_gunraise1 = {TRUE,GUNRISE21,2,NULL,0,&s_gunraise2};

statetype s_gunlower3 = {TRUE,GUNRISE21,2,NULL,0,&s_gunstand};
statetype s_gunlower2 = {TRUE,GUNRISE31,2,NULL,0,&s_gunlower3};
statetype s_gunlower1 = {TRUE,GUNRISE41,2,NULL,0,&s_gunlower2};


statetype s_gundead = {FALSE,GUNDEAD2,0,T_Collide,0,&s_gundead};
statetype s_gundie1 = {FALSE,GUNDEAD1,5,NULL,0,&s_gundead};



//======================================================================//

statetype s_4waygunfire1 = {TRUE,FOURWAYFIRE01,5,A_4WayGunShoot,0,&s_4waygunfire2};
statetype s_4waygunfire2 = {TRUE,FOURWAY01,5,NULL,0,&s_4waygunfire1};

statetype s_4waygun = {TRUE,FOURWAY01,0,T_4WayGunStand,0,&s_4waygun};




statetype s_kessphere8 = {FALSE,KESSPHERE8,2,T_Projectile,0,&s_kessphere1};
statetype s_kessphere7 = {FALSE,KESSPHERE7,2,T_Projectile,0,&s_kessphere8};
statetype s_kessphere6 = {FALSE,KESSPHERE6,2,T_Projectile,0,&s_kessphere7};
statetype s_kessphere5 = {FALSE,KESSPHERE5,2,T_Projectile,0,&s_kessphere6};
statetype s_kessphere4 = {FALSE,KESSPHERE4,2,T_Projectile,0,&s_kessphere5};
statetype s_kessphere3 = {FALSE,KESSPHERE3,2,T_Projectile,0,&s_kessphere4};
statetype s_kessphere2 = {FALSE,KESSPHERE2,2,T_Projectile,0,&s_kessphere3};
statetype s_kessphere1 = {FALSE,KESSPHERE1,2,T_Projectile,0,&s_kessphere2};


statetype s_slop4 = {FALSE,TOMSPIT4,3,T_Particle,0,&s_slop1};
statetype s_slop3 = {FALSE,TOMSPIT3,3,T_Particle,0,&s_slop4};
statetype s_slop2 = {FALSE,TOMSPIT2,3,T_Particle,0,&s_slop3};
statetype s_slop1 = {FALSE,TOMSPIT1,3,T_Particle,0,&s_slop2};


statetype s_batblast4 = {FALSE,BATBLAST4,3,T_Projectile,SF_BAT,&s_batblast1};
statetype s_batblast3 = {FALSE,BATBLAST3,3,T_Projectile,SF_BAT,&s_batblast4};
statetype s_batblast2 = {FALSE,BATBLAST2,3,T_Projectile,SF_BAT,&s_batblast3};
statetype s_batblast1 = {FALSE,BATBLAST1,3,T_Projectile,SF_BAT,&s_batblast2};


statetype s_serialdog4 = {TRUE,SERIALDOG_W41,5,T_Player,SF_DOGSTATE,&s_serialdog};
statetype s_serialdog3 = {TRUE,SERIALDOG_W31,5,T_Player,SF_DOGSTATE,&s_serialdog4};
statetype s_serialdog2 = {TRUE,SERIALDOG_W21,5,T_Player,SF_DOGSTATE,&s_serialdog3};
statetype s_serialdog = {TRUE,SERIALDOG_W11,5,T_Player,SF_DOGSTATE,&s_serialdog2};
statetype s_serialdogattack = {TRUE,SERIALDOG_ATTACK1,0,T_Attack,SF_DOGSTATE,&s_serialdogattack };


#endif


#include "rt_table.h"

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
#ifndef _states_public
#define _states_public

#include <stdint.h>

#include "develop.h"

#if (SHAREWARE == 0)
#define MAXSTATES 1300+17+8+32+32
#else
#define MAXSTATES 660+17+8+32+32
#endif

#define SF_CLOSE   0x01
#define SF_CRUSH   0x02
#define SF_UP      0x04
#define SF_DOWN    0x08
#define SF_SOUND   0x10
#define SF_BLOCK   0x20
#define SF_EYE1    0
#define SF_EYE2    1
#define SF_EYE3    2
#define SF_DOGSTATE 0x40
#define SF_BAT      0x80
#define SF_FAKING   0x80
#define SF_DEAD     0x80

typedef struct  statestruct
{
		  uint8_t            rotate;
		  int16_t           shapenum;  // a shapenum of -1 means get from ob->temp1
		  int16_t           tictime;
		  void            (*think) ();
		  int8_t     condition;
		  struct  statestruct     *next;
} statetype;

extern   statetype * statetable[MAXSTATES];

extern   statetype s_lowgrdstand;
extern   statetype s_lowgrdpath4;
extern   statetype s_lowgrdpath3;
extern   statetype s_lowgrdpath2;
extern   statetype s_lowgrdpath1;
extern   statetype s_lowgrdcollide;
extern   statetype s_lowgrdcollide2;
//extern   statetype s_lowgrduse1;
extern   statetype s_lowgrdshoot1;
extern   statetype s_lowgrdshoot2;
extern   statetype s_lowgrdshoot3;
extern   statetype s_lowgrdshoot4;
extern   statetype s_lowgrdchase1;
extern   statetype s_lowgrdchase2;
extern   statetype s_lowgrdchase3;
extern   statetype s_lowgrdchase4;
extern   statetype s_lowgrddie1;
extern   statetype s_lowgrddie2;
extern   statetype s_lowgrddie3;
extern   statetype s_lowgrddie4;
extern   statetype s_lowgrddead;
extern   statetype s_lowgrdcrushed1;
extern   statetype s_lowgrdcrushed2;
extern   statetype s_sneakydown;
extern   statetype s_sneakyrise1;
extern   statetype s_sneakyrise2;
extern   statetype s_sneakyrise3;
extern   statetype s_sneakyrise4;


extern   statetype s_highgrdstand;
extern   statetype s_highgrdpath1;
extern   statetype s_highgrdpath2;
extern   statetype s_highgrdpath3;
extern   statetype s_highgrdpath4;
extern   statetype s_highgrdcollide;
extern   statetype s_highgrdcollide2;
//extern   statetype s_highgrduse1;
extern   statetype s_highgrdshoot1;
extern   statetype s_highgrdshoot2;
extern   statetype s_highgrdshoot3;
extern   statetype s_highgrdshoot4;
extern   statetype s_highgrdchase1;
extern   statetype s_highgrdchase2;
extern   statetype s_highgrdchase3;
extern   statetype s_highgrdchase4;
extern   statetype s_highgrddie1;
extern   statetype s_highgrddie2;
extern   statetype s_highgrddie3;
extern   statetype s_highgrddie4;
extern   statetype s_highgrddie5;
extern   statetype s_highgrddead;
extern   statetype s_highgrdcrushed1;
extern   statetype s_highgrdcrushed2;


extern   statetype s_strikestand;
extern   statetype s_strikepath1;
extern   statetype s_strikepath2;
extern   statetype s_strikepath3;
extern   statetype s_strikepath4;
extern   statetype s_strikecollide;
extern   statetype s_strikecollide2;
extern   statetype s_strikeshoot1;
extern   statetype s_strikeshoot2;
extern   statetype s_strikeshoot3;
extern   statetype s_strikeshoot4;
//extern   statetype s_strikeuse1;
extern   statetype s_strikerollright1;
extern   statetype s_strikerollright2;
extern   statetype s_strikerollright3;
extern   statetype s_strikerollright4;
extern   statetype s_strikerollright5;
extern   statetype s_strikerollright6;
extern   statetype s_strikerollleft1;
extern   statetype s_strikerollleft2;
extern   statetype s_strikerollleft3;
extern   statetype s_strikerollleft4;
extern   statetype s_strikerollleft5;
extern   statetype s_strikerollleft6;
extern   statetype s_strikechase1;
extern   statetype s_strikechase2;
extern   statetype s_strikechase3;
extern   statetype s_strikechase4;
extern   statetype s_strikedie1;
extern   statetype s_strikedie2;
extern   statetype s_strikedie3;
extern   statetype s_strikedie4;
extern   statetype s_strikedead;
extern   statetype s_strikedead1;
extern   statetype s_strikedead2;
extern   statetype s_strikedead3;
extern   statetype s_strikewait;
extern   statetype s_strikecrushed1;
extern   statetype s_strikecrushed2;


extern   statetype s_blitzstand;
extern   statetype s_blitzpath1;
extern   statetype s_blitzpath2;
extern   statetype s_blitzpath3;
extern   statetype s_blitzpath4;
extern   statetype s_blitzcollide;
extern   statetype s_blitzcollide2;
extern   statetype s_blitzshoot1;
extern   statetype s_blitzshoot2;
extern   statetype s_blitzshoot3;
extern   statetype s_blitzshoot4;
extern   statetype s_blitzuse;
extern   statetype s_blitzsteal1;
extern   statetype s_blitzsteal2;
extern   statetype s_blitzchase1;
extern   statetype s_blitzchase2;
extern   statetype s_blitzchase3;
extern   statetype s_blitzchase4;
extern   statetype s_blitzfakedie1;
extern   statetype s_blitzfakedie2;
extern   statetype s_blitzfakedie3;
extern   statetype s_blitzdie1;
extern   statetype s_blitzdie2;
extern   statetype s_blitzdie3;
extern   statetype s_blitzdie4;
extern   statetype s_blitzfakedead;
extern   statetype s_blitzdead;
extern   statetype s_blitzdead1;
extern   statetype s_blitzdead2;
extern   statetype s_blitzplead1;
extern   statetype s_blitzplead2;
extern   statetype s_blitzplead3;
extern   statetype s_blitzplead4;
extern   statetype s_blitzplead5;
extern   statetype s_blitzplead6;
extern   statetype s_blitzplead7;
extern   statetype s_blitzplead8;
extern   statetype s_blitzplead9;
extern   statetype s_blitzplead10;
extern   statetype s_blitzplead11;
extern   statetype s_blitzaplead5;
extern   statetype s_blitzaplead4;
extern   statetype s_blitzcrushed1;
extern   statetype s_blitzcrushed2;
extern   statetype s_blitzfakedie1;
extern   statetype s_blitzrise1;
extern   statetype s_blitzrise2;
extern   statetype s_blitzrise3;
extern   statetype s_blitzrise4;
extern   statetype s_blitzstruggledie1;
extern   statetype s_blitzstruggledead;

extern   statetype s_enforcerstand;
extern   statetype s_enforcerpath1;
extern   statetype s_enforcerpath2;
extern   statetype s_enforcerpath3;
extern   statetype s_enforcerpath4;
extern   statetype s_enforcercollide;
extern   statetype s_enforcercollide2;
//extern   statetype s_enforceruse1;
extern   statetype s_enforcershoot1;
extern   statetype s_enforcershoot2;
extern   statetype s_enforcershoot3;
extern   statetype s_enforcershoot4;
extern   statetype s_enforcerthrow1;
extern   statetype s_enforcerthrow2;
extern   statetype s_enforcerthrow3;
extern   statetype s_enforcerthrow4;
extern   statetype s_enforcerthrow5;
extern   statetype s_enforcerthrow6;
extern   statetype s_enforcerthrow7;
extern   statetype s_enforcerthrow8;
extern   statetype s_grenade1;
extern   statetype s_grenade2;
extern   statetype s_grenade3;
extern   statetype s_grenade4;
extern   statetype s_grenade5;
extern   statetype s_grenade6;
extern   statetype s_grenade7;
extern   statetype s_grenade8;
extern   statetype s_grenade9;
extern   statetype s_grenade10;
extern   statetype s_grenadehit1;
extern   statetype s_grenadehit2;
extern   statetype s_grenadehit3;
extern   statetype s_enforcerchase1;
extern   statetype s_enforcerchase2;
extern   statetype s_enforcerchase3;
extern   statetype s_enforcerchase4;
extern   statetype s_enforcerdead;
extern   statetype s_enforcerdie1;
extern   statetype s_enforcerdie2;
extern   statetype s_enforcerdie3;
extern   statetype s_enforcerdie4;
extern   statetype s_grenade_fall1;
extern   statetype s_grenade_fall2;
extern   statetype s_grenade_fall3;
extern   statetype s_grenade_fall4;
extern   statetype s_grenade_fall5;
extern   statetype s_grenade_fall6;
extern   statetype s_enforcercrushed1;
extern   statetype s_enforcercrushed2;


extern   statetype s_robogrdstand;
extern   statetype s_robogrdpath1;
extern   statetype s_robogrdshoot1;
extern   statetype s_robogrdshuriken1;
extern   statetype s_robogrdshuriken2;
extern   statetype s_robogrdshuriken3;
extern   statetype s_robogrdshuriken4;
extern   statetype s_shurikenhit1;
extern   statetype s_shurikenhit2;
extern   statetype s_shurikenhit3;
extern   statetype s_robogrdchase1;
extern   statetype s_robogrddead;
extern   statetype s_robogrddie1;
extern   statetype s_robogrddie2;
extern   statetype s_robogrddie3;
extern   statetype s_robogrddie4;
extern   statetype s_robogrddie5;
extern   statetype s_robogrddie6;
extern   statetype s_robogrddie7;
extern   statetype s_robogrddie8;
extern   statetype s_robogrddie9;
extern   statetype s_robogrdcollide;
extern   statetype s_robogrdcollide2;
extern   statetype s_robogrdcrushed1;
extern   statetype s_roboalign;
extern   statetype s_robowait;
extern   statetype s_roborealign;





extern statetype s_altexplosion10;
extern statetype s_altexplosion9 ;
extern statetype s_altexplosion8 ;
extern statetype s_altexplosion7 ;
extern statetype s_altexplosion6 ;
extern statetype s_altexplosion5 ;
extern statetype s_altexplosion4 ;
extern statetype s_altexplosion3  ;
extern statetype s_altexplosion2  ;
extern statetype s_altexplosion1  ;



extern   statetype s_explosion1;
extern   statetype s_explosion2;
extern   statetype s_explosion3;
extern   statetype s_explosion4;
extern   statetype s_explosion5;
extern   statetype s_explosion6;
extern   statetype s_explosion7;
extern   statetype s_explosion8;
extern   statetype s_explosion9;
extern   statetype s_explosion10;
extern   statetype s_explosion11;
extern   statetype s_explosion12;
extern   statetype s_explosion13;
extern   statetype s_explosion14;
extern   statetype s_explosion15;
extern   statetype s_explosion16;
extern   statetype s_explosion17;
extern   statetype s_explosion18;
extern   statetype s_explosion19;
extern   statetype s_explosion20;

extern   statetype s_grexplosion1;
extern   statetype s_grexplosion2;
extern   statetype s_grexplosion3;
extern   statetype s_grexplosion4;
extern   statetype s_grexplosion5;
extern   statetype s_grexplosion6;
extern   statetype s_grexplosion7;
extern   statetype s_grexplosion8;
extern   statetype s_grexplosion9;
extern   statetype s_grexplosion10;
extern   statetype s_grexplosion11;
extern   statetype s_grexplosion12;
extern   statetype s_grexplosion13;
extern   statetype s_grexplosion14;
extern   statetype s_grexplosion15;
extern   statetype s_grexplosion16;
extern   statetype s_grexplosion17;
extern   statetype s_grexplosion18;
extern   statetype s_grexplosion19;
extern   statetype s_grexplosion20;



extern   statetype s_staticexplosion1;
extern   statetype s_staticexplosion2;
extern   statetype s_staticexplosion3;
extern   statetype s_staticexplosion4;
extern   statetype s_staticexplosion5;
extern   statetype s_staticexplosion6;
extern   statetype s_staticexplosion7;
extern   statetype s_staticexplosion8;
extern   statetype s_staticexplosion9;
extern   statetype s_staticexplosion10;
extern   statetype s_staticexplosion11;
extern   statetype s_staticexplosion12;
extern   statetype s_staticexplosion13;
extern   statetype s_staticexplosion14;
extern   statetype s_staticexplosion15;
extern   statetype s_staticexplosion16;
extern   statetype s_staticexplosion17;
extern   statetype s_staticexplosion18;
extern   statetype s_staticexplosion19;
extern   statetype s_staticexplosion20;
extern   statetype s_staticexplosion21;
extern   statetype s_staticexplosion22;
extern   statetype s_staticexplosion23;
extern   statetype s_staticexplosion24;
extern   statetype s_staticexplosion25;

extern   statetype s_upblade1;
extern   statetype s_upblade2;
extern   statetype s_upblade3;
extern   statetype s_upblade4;
extern   statetype s_upblade5;
extern   statetype s_upblade6;
extern   statetype s_upblade7;
extern   statetype s_upblade8;
extern   statetype s_upblade9;
extern   statetype s_upblade10;
extern   statetype s_upblade11;
extern   statetype s_upblade12;
extern   statetype s_upblade13;
extern   statetype s_upblade14;
extern   statetype s_upblade15;
extern   statetype s_upblade16;

extern   statetype s_firejetup1;
extern   statetype s_firejetup2;
extern   statetype s_firejetup3;
extern   statetype s_firejetup4;
extern   statetype s_firejetup5;
extern   statetype s_firejetup6;
extern   statetype s_firejetup7;
extern   statetype s_firejetup8;
extern   statetype s_firejetup9;
extern   statetype s_firejetup10;
extern   statetype s_firejetup11;
extern   statetype s_firejetup12;
extern   statetype s_firejetup13;
extern   statetype s_firejetup14;
extern   statetype s_firejetup15;
extern   statetype s_firejetup16;
extern   statetype s_firejetup17;
extern   statetype s_firejetup18;
extern   statetype s_firejetup19;
extern   statetype s_firejetup20;
extern   statetype s_firejetup21;
extern   statetype s_firejetup22;
extern   statetype s_firejetup23;

extern   statetype s_columndownup1;
extern   statetype s_columndownup2;
extern   statetype s_columndownup3;
extern   statetype s_columndownup4;
extern   statetype s_columndownup5;
extern   statetype s_columndownup6;

extern   statetype s_columndowndown1;
extern   statetype s_columndowndown2;
extern   statetype s_columndowndown3;
extern   statetype s_columndowndown4;
extern   statetype s_columndowndown5;
extern   statetype s_columndowndown6;
extern   statetype s_columndowndown7;
extern   statetype s_columndowndown8;


extern   statetype s_spearup1;
extern   statetype s_spearup2;
extern   statetype s_spearup3;
extern   statetype s_spearup4;
extern   statetype s_spearup5;
extern   statetype s_spearup6;
extern   statetype s_spearup7;
extern   statetype s_spearup8;
extern   statetype s_spearup9;
extern   statetype s_spearup10;
extern   statetype s_spearup11;
extern   statetype s_spearup12;
extern   statetype s_spearup13;
extern   statetype s_spearup14;
extern   statetype s_spearup15;
extern   statetype s_spearup16;
extern   statetype s_pushcolumn1;
extern   statetype s_pushcolumn2;
extern   statetype s_pushcolumn3;

extern   statetype s_wallfireball;
extern   statetype s_crossfire1;
extern   statetype s_crossfire2;
extern   statetype s_crossdone1;
extern   statetype s_crossdone2;
extern   statetype s_crossdone3;
extern   statetype s_crossdone4;
extern   statetype s_crossdone5;




extern   statetype s_fireunit1;
extern   statetype s_fireunit2;
extern   statetype s_fireunit3;
extern   statetype s_fireunit4;
extern   statetype s_fireunit5;
extern   statetype s_fireunit6;
extern   statetype s_fireunit7;
extern   statetype s_fireunit8;
extern   statetype s_fireunit9;
extern   statetype s_fireunit10;
extern   statetype s_fireunit11;
extern   statetype s_fireunit12;
extern   statetype s_fireunit13;
extern   statetype s_fireunit14;
extern   statetype s_fireunit15;
extern   statetype s_firespan1;

extern   statetype s_p_bazooka1;
extern   statetype s_p_bazooka2;

extern   statetype s_p_grenade;
extern   statetype s_p_gfall1;
extern   statetype s_p_gfall2;
extern   statetype s_p_gfall3;
extern   statetype s_p_gfall4;

extern   statetype s_gunsmoke1;
extern   statetype s_gunsmoke2;
extern   statetype s_gunsmoke3;
extern   statetype s_gunsmoke4;
extern   statetype s_gunsmoke5;
extern   statetype s_gunsmoke6;
extern   statetype s_gunsmoke7;
extern   statetype s_gunsmoke8;
extern   statetype s_bloodspurt1;
extern   statetype s_bloodspurt2;
extern   statetype s_bloodspurt3;
extern   statetype s_bloodspurt4;
extern   statetype s_bloodspurt5;
extern   statetype s_bloodspurt6;
extern   statetype s_bloodspurt7;
extern   statetype s_bloodspurt8;
extern   statetype s_hitmetalwall1;
extern   statetype s_hitmetalwall2;
extern   statetype s_hitmetalwall3;
extern   statetype s_hitmetalwall4;
extern   statetype s_hitmetalactor1;
extern   statetype s_hitmetalactor2;
extern   statetype s_hitmetalactor3;
extern   statetype s_hitmetalactor4;






extern   statetype s_dust;



extern   statetype s_skeleton1;
extern   statetype s_skeleton2;
extern   statetype s_skeleton3;
extern   statetype s_skeleton4;
extern   statetype s_skeleton5;
extern   statetype s_skeleton6;
extern   statetype s_skeleton7;
extern   statetype s_skeleton8;
extern   statetype s_skeleton9;
extern   statetype s_skeleton10;
extern   statetype s_skeleton11;
extern   statetype s_skeleton12;
extern   statetype s_skeleton13;
extern   statetype s_skeleton14;
extern   statetype s_skeleton15;
extern   statetype s_skeleton16;
extern   statetype s_skeleton17;
extern   statetype s_skeleton18;
extern   statetype s_skeleton19;
extern   statetype s_skeleton20;
extern   statetype s_skeleton21;
extern   statetype s_skeleton22;
extern   statetype s_skeleton23;
extern   statetype s_skeleton24;
extern   statetype s_skeleton25;
extern   statetype s_skeleton26;
extern   statetype s_skeleton27;
extern   statetype s_skeleton28;
extern   statetype s_skeleton29;
extern   statetype s_skeleton30;
extern   statetype s_skeleton31;
extern   statetype s_skeleton32;
extern   statetype s_skeleton33;
extern   statetype s_skeleton34;
extern   statetype s_skeleton35;
extern   statetype s_skeleton36;
extern   statetype s_skeleton37;
extern   statetype s_skeleton38;
extern   statetype s_skeleton39;
extern   statetype s_skeleton40;
extern   statetype s_skeleton41;
extern   statetype s_skeleton42;
extern   statetype s_skeleton43;
extern   statetype s_skeleton44;
extern   statetype s_skeleton45;
extern   statetype s_skeleton46;
extern   statetype s_skeleton47;


extern   statetype s_gas2;
extern   statetype s_gas1;


extern   statetype s_spring1;
extern   statetype s_spring2;
extern   statetype s_spring3;
extern   statetype s_spring4;
extern   statetype s_spring5;
extern   statetype s_spring6;
extern   statetype s_spring7;
extern   statetype s_spring8;
extern   statetype s_spring9;




extern   statetype s_player;
extern   statetype s_free;

extern   statetype s_pgunattack1;
extern   statetype s_pmissattack1;
extern   statetype s_pgunattack2;
extern   statetype s_pmissattack2;
extern   statetype s_remoteinelev;
extern   statetype s_remotemove1;
extern   statetype s_remotemove2;
extern   statetype s_remotemove3;
extern   statetype s_remotemove4;


extern   statetype s_godfire1;
extern   statetype s_godfire2;
extern   statetype s_godfire3;
extern   statetype s_godfire4;



extern   statetype s_remotedead;
extern   statetype s_remotedie1;
extern   statetype s_remotedie2;
extern   statetype s_remotedie3;
extern   statetype s_remotedie4;
extern   statetype s_remotedie5;


extern   statetype s_guts1;
extern   statetype s_guts2;
extern   statetype s_guts3;
extern   statetype s_guts4;
extern   statetype s_guts5;
extern   statetype s_guts6;
extern   statetype s_guts7;
extern   statetype s_guts8;
extern   statetype s_guts9;
extern   statetype s_guts10;
extern   statetype s_guts11;
extern   statetype s_guts12;


extern   statetype s_bossdeath;
extern   statetype s_megaexplosions;

extern   statetype s_superparticles;
extern   statetype s_gibs1;
extern   statetype s_gibs2;
extern   statetype s_gibs3;
extern   statetype s_gibs4;
extern   statetype s_gibsdone1;
extern   statetype s_gibsdone2;
extern   statetype s_gibsdone3;
extern   statetype s_gibsdone4;
extern   statetype s_gibsdone5;
extern   statetype s_gibsdone6;
extern   statetype s_gibsdone7;
extern   statetype s_gibsdone8;
extern   statetype s_bigsoul;
extern   statetype s_littlesoul;
extern   statetype s_vaporized1;
extern   statetype s_vaporized2;
extern   statetype s_vaporized3;
extern   statetype s_vaporized4;
extern   statetype s_vaporized5;
extern   statetype s_vaporized6;
extern   statetype s_vaporized7;
extern   statetype s_autospring1;
extern   statetype s_autospring2;
extern   statetype s_autospring3;
extern   statetype s_autospring4;
extern   statetype s_autospring5;
extern   statetype s_autospring6;
extern   statetype s_autospring7;
extern   statetype s_autospring8;
extern   statetype s_autospring9;
extern   statetype s_pbatblast;



extern   statetype s_collectorwander1;
extern   statetype s_collectorwander2;
extern   statetype s_collectorwander3;
extern   statetype s_collectorwander4;
extern   statetype s_collectorwander5;
extern   statetype s_collectorwander6;
extern   statetype s_collectorwander7;
extern   statetype s_collectorwander8;
extern   statetype s_collectorfdoor1;
extern   statetype s_collectorfdoor2;
extern   statetype s_collectorfdoor3;
extern   statetype s_collectorfdoor4;
extern   statetype s_collectorfdoor5;
extern   statetype s_collectorfdoor6;
extern   statetype s_collectorfdoor7;
extern   statetype s_collectorfdoor8;
extern   statetype s_tag;
extern   statetype s_timekeeper;
extern   statetype s_skeleton48;
extern   statetype s_skeleton24;

extern   statetype s_wind;
extern   statetype s_remoteguts1;
extern   statetype s_remoteguts2;
extern   statetype s_remoteguts3;
extern   statetype s_remoteguts4;
extern   statetype s_remoteguts5;
extern   statetype s_remoteguts6;
extern   statetype s_remoteguts7;
extern   statetype s_remoteguts8;
extern   statetype s_remoteguts9;
extern   statetype s_remoteguts10;
extern   statetype s_remoteguts11;
extern   statetype s_voidwait;
extern   statetype s_ashwait;
extern   statetype s_deadwait;
extern   statetype s_gutwait;
extern   statetype s_vaporized8;
extern   statetype s_remoteguts12;
extern   statetype s_eye1;
extern   statetype s_eye2;
extern   statetype s_eye3;
extern   statetype s_itemspawn1;
extern   statetype s_itemspawn2;
extern   statetype s_itemspawn3;
extern   statetype s_itemspawn4;
extern   statetype s_itemspawn5;
extern   statetype s_itemspawn6;
extern   statetype s_itemspawn7;
extern   statetype s_itemspawn8;
extern   statetype s_deadblood1;
extern   statetype s_deadblood2;
extern   statetype s_deadblood3;
extern   statetype s_deadblood4;
extern   statetype s_deadblood5;
extern   statetype s_deadblood6;
extern   statetype s_deadblood7;
extern   statetype s_deadblood8;


extern   statetype s_flash1;
extern   statetype s_flash2;
extern   statetype s_flash3;
extern   statetype s_flash4;
extern   statetype s_flash5;
extern   statetype s_flash6;
extern   statetype s_flash7;
extern   statetype s_flash8;

extern   statetype s_elevdisk;
extern   statetype s_pathdisk;
extern   statetype s_megaremove;

extern   statetype s_respawn1;
extern   statetype s_respawn2;
extern   statetype s_respawn3;
extern   statetype s_respawn4;
extern   statetype s_respawn5;
extern   statetype s_respawn6;
extern   statetype s_respawn7;
extern   statetype s_respawn8;
extern   statetype s_basemarker1;
extern   statetype s_basemarker2;
extern   statetype s_basemarker3;
extern   statetype s_basemarker4;
extern   statetype s_basemarker5;
extern   statetype s_basemarker6;
extern   statetype s_basemarker7;
extern   statetype s_basemarker8;

extern   statetype s_blooddrip1;
extern   statetype s_blooddrip2;
extern   statetype s_blooddrip3;
extern   statetype s_blooddrip4;

extern   statetype s_diskmaster;
extern   statetype s_bstar1;
extern   statetype s_bstar2;
extern   statetype s_bstar3;
extern   statetype s_bstar4;


#if (SHAREWARE == 0)

extern   statetype s_scottwander1;
extern   statetype s_scottwander2;
extern   statetype s_scottwander3;
extern   statetype s_scottwander4;
extern   statetype s_scottwander5;
extern   statetype s_scottwander6;
extern   statetype s_scottwander7;
extern   statetype s_scottwanderdoor1;
extern   statetype s_scottwanderdoor2;
extern   statetype s_scottwanderdoor3;
extern   statetype s_scottwanderdoor4;
extern   statetype s_scottwanderdoor5;
extern   statetype s_scottwanderdoor6;
extern   statetype s_scottwanderdoor7;

extern   statetype s_opstand;
extern   statetype s_oppath1;
extern   statetype s_oppath2;
extern   statetype s_oppath3;
extern   statetype s_oppath4;
extern   statetype s_opcollide;
extern   statetype s_opcollide2;
extern   statetype s_opgiveup;
//extern   statetype s_opuse1;
extern   statetype s_opshoot1;
extern   statetype s_opshoot2;
extern   statetype s_opshoot3;
extern   statetype s_opshoot4;
extern   statetype s_opbolo1;
extern   statetype s_opbolo2;
extern   statetype s_opbolo3;
extern   statetype s_opbolo4;
extern   statetype s_opbolo5;
extern   statetype s_bolocast1;
extern   statetype s_bolocast2;
extern   statetype s_bolocast3;
extern   statetype s_bolocast4;
extern   statetype s_opchase1;
extern   statetype s_opchase2;
extern   statetype s_opchase3;
extern   statetype s_opchase4;
extern   statetype s_opdead;
extern   statetype s_opdie1;
extern   statetype s_opdie2;
extern   statetype s_opdie3;
extern   statetype s_opdie4;
extern   statetype s_opdie5;
extern   statetype s_opcrushed1;
extern   statetype s_opcrushed2;


extern   statetype s_dmonkstand;
extern   statetype s_dmonkpath1;
extern   statetype s_dmonkpath2;
extern   statetype s_dmonkpath3;
extern   statetype s_dmonkpath4;
extern   statetype s_dmonkshoot1;
extern   statetype s_dmonkshoot2;
extern   statetype s_dmonkshoot3;
extern   statetype s_dmonkshoot4;
extern   statetype s_dmonkshoot5;
extern   statetype s_dmonkshoot6;
extern   statetype s_dmonkchase1;
extern   statetype s_dmonkchase2;
extern   statetype s_dmonkchase3;
extern   statetype s_dmonkchase4;
extern   statetype s_dmonkdead;
extern   statetype s_dmonkdie1;
extern   statetype s_dmonkdie2;
extern   statetype s_dmonkdie3;
extern   statetype s_dmonkdie4;
extern   statetype s_dmonkcollide;
extern   statetype s_dmonkcollide2;
extern   statetype s_dmonkcrushed1;
extern   statetype s_dmonkcrushed2;
extern   statetype s_dmonkshoot5;
extern   statetype s_dmonkshoot3;
extern   statetype s_dmonkshoot4;


extern   statetype s_firemonkstand;
extern   statetype s_firemonkpath1;
extern   statetype s_firemonkpath2;
extern   statetype s_firemonkpath3;
extern   statetype s_firemonkpath4;
extern   statetype s_firemonkcast1;
extern   statetype s_firemonkcast2;
extern   statetype s_firemonkcast3;
extern   statetype s_firemonkcast4;
extern   statetype s_firemonkcast5;
extern   statetype s_firemonkcast6;
extern   statetype s_firemonkcast7;
extern   statetype s_monkfire1;
extern   statetype s_monkfire2;
extern   statetype s_monkfire3;
extern   statetype s_monkfire4;
extern   statetype s_fireballhit1;
extern   statetype s_fireballhit2;
extern   statetype s_fireballhit3;
extern   statetype s_firemonkchase1;
extern   statetype s_firemonkchase2;
extern   statetype s_firemonkchase3;
extern   statetype s_firemonkchase4;
extern   statetype s_firemonkdead;
extern   statetype s_firemonkdead1;
extern   statetype s_firemonkdead2;
extern   statetype s_firemonkdead3;
extern   statetype s_firemonkdead4;
extern   statetype s_firemonkdead5;
extern   statetype s_firemonkdead6;
extern   statetype s_firemonkdead7;
extern   statetype s_firemonkdie1;
extern   statetype s_firemonkdie2;
extern   statetype s_firemonkdie3;
extern   statetype s_firemonkdie4;
extern   statetype s_firemonkcollide;
extern   statetype s_firemonkcollide2;
extern   statetype s_firemonkcrushed1;
extern   statetype s_firemonkcrushed2;


extern   statetype s_wallstand;
extern   statetype s_wallpath;
extern   statetype s_wallshoot;
extern   statetype s_wallcollide;
extern   statetype s_wallalign;
extern   statetype s_wallwait;
extern   statetype s_wallrestore;


extern   statetype s_darianstand;
extern   statetype s_darianchase1;
extern   statetype s_darianchase2;
extern   statetype s_darianchase3;
extern   statetype s_darianchase4;
extern   statetype s_darianuse;
extern   statetype s_darianshoot1;
extern   statetype s_darianshoot2;
extern   statetype s_darianshoot3;
extern   statetype s_darianshoot4;
extern   statetype s_dariancollide;
extern   statetype s_dariancollide2;
extern   statetype s_dariandead;
extern   statetype s_dariandead1;
extern   statetype s_dariandead2;
extern   statetype s_dariandie1;
extern   statetype s_dariandie2;
extern   statetype s_dariandie3;
extern   statetype s_dariandie4;
extern   statetype s_dariandie5;
extern   statetype s_dariandie6;
extern   statetype s_dariandie7;
extern   statetype s_dariandie8;
extern   statetype s_dariandie9;
extern   statetype s_dariandie10;
extern   statetype s_darianspears;
extern   statetype s_darianuse1;
extern   statetype s_darianuse2;
extern   statetype s_darianuse3;
extern   statetype s_darianuse4;
extern   statetype s_dariansink1;
extern   statetype s_dariansink2;
extern   statetype s_dariansink3;
extern   statetype s_dariansink4;
extern   statetype s_dariansink5;
extern   statetype s_dariansink6;
extern   statetype s_dariansink7;
extern   statetype s_dariansink8;
extern   statetype s_dariansink9;
extern   statetype s_darianrise1;
extern   statetype s_darianrise2;
extern   statetype s_darianrise3;
extern   statetype s_darianrise4;
extern   statetype s_darianrise5;
extern   statetype s_darianrise6;
extern   statetype s_darianrise7;
extern   statetype s_darianrise8;
extern   statetype s_darianwait;
extern   statetype s_dariandefend1;
extern   statetype s_dariandefend2;
extern   statetype s_dariandefend3;


extern   statetype s_heinrichstand;
extern   statetype s_heinrichshoot1;
extern   statetype s_heinrichshoot2;
extern   statetype s_heinrichshoot3;
extern   statetype s_heinrichshoot4;
extern   statetype s_heinrichshoot5;
extern   statetype s_heinrichshoot6;
extern   statetype s_heinrichshoot7;
extern   statetype s_heinrichshoot8;
extern   statetype s_heinrichshoot9;
extern   statetype s_heinrichshoot10;
extern   statetype s_heinrichshoot11;
extern   statetype s_heinrichooc;
extern   statetype s_heinrichchase;
extern   statetype s_heindead;
extern   statetype s_heindead1;
extern   statetype s_heindead2;
extern   statetype s_heinexp1;
extern   statetype s_heinexp2;
extern   statetype s_heinexp3;
extern   statetype s_heinexp4;
extern   statetype s_heinexp5;
extern   statetype s_heinexp6;
extern   statetype s_heinexp7;
extern   statetype s_heinexp8;
extern   statetype s_heinexp9;
extern   statetype s_heinexp10;
extern   statetype s_heinexp11;
extern   statetype s_heinexp12;
extern   statetype s_heinexp13;
extern   statetype s_kristleft;
extern   statetype s_kristright;


extern   statetype s_missile1;
extern   statetype s_missilehit1;
extern   statetype s_missilehit2;
extern   statetype s_missilehit3;
extern   statetype s_mine1;
extern   statetype s_mine2;
extern   statetype s_mine3;
extern   statetype s_mine4;
extern   statetype s_heinrichchase1;
extern   statetype s_heinrichuse;
extern   statetype s_heinrichmine;
extern   statetype s_heinrichdie1;
extern   statetype s_heinrichdie2;
extern   statetype s_heinrichdead;
extern   statetype s_heinrichdead1;
extern   statetype s_heinrichdead2;
extern   statetype s_heinrichdefend;

extern   statetype s_dexplosion22;
extern   statetype s_dexplosion21;
extern   statetype s_dexplosion20;
extern   statetype s_dexplosion19;
extern   statetype s_dexplosion18;
extern   statetype s_dexplosion17;
extern   statetype s_dexplosion16;
extern   statetype s_dexplosion15;
extern   statetype s_dexplosion14;
extern   statetype s_dexplosion13;
extern   statetype s_dexplosion12;
extern   statetype s_dexplosion11;
extern   statetype s_dexplosion10;
extern   statetype s_dexplosion9;
extern   statetype s_dexplosion8;
extern   statetype s_dexplosion7;
extern   statetype s_dexplosion6;
extern   statetype s_dexplosion5;
extern   statetype s_dexplosion4;
extern   statetype s_dexplosion3;
extern   statetype s_dexplosion2;
extern   statetype s_dexplosion1;
extern   statetype s_dspear1;
extern   statetype s_dspear2;
extern   statetype s_dspear3;
extern   statetype s_dspear4;
extern   statetype s_dspear5;
extern   statetype s_dspear6;
extern   statetype s_dspear7;
extern   statetype s_dspear8;
extern   statetype s_dspear9;
extern   statetype s_dspear10;
extern   statetype s_dspear11;
extern   statetype s_dspear12;
extern   statetype s_dspear13;
extern   statetype s_dspear14;
extern   statetype s_dspear15;
extern   statetype s_dspear16;



extern   statetype s_NMEchase;
extern   statetype s_NMEdie;
extern   statetype s_NMEhead1;
extern   statetype s_NMEhead2;
extern   statetype s_NMEwheels1;
extern   statetype s_NMEwheels2;
extern   statetype s_NMEwheels3;
extern   statetype s_NMEwheels4;
extern   statetype s_NMEwheels5;
extern   statetype s_NMEwindup;
extern   statetype s_NMEwheels120;

extern   statetype s_NMEwrotleft3;
extern   statetype s_NMEwrotleft2;
extern   statetype s_NMEwrotleft1;

extern   statetype s_NMEwrotright3;
extern   statetype s_NMEwrotright2;
extern   statetype s_NMEwrotright1;
extern   statetype s_NMEminiball1;
extern   statetype s_NMEminiball2;
extern   statetype s_NMEminiball3;
extern   statetype s_NMEminiball4;
extern   statetype s_NMEattack;
extern   statetype s_NMEsaucer1;
extern   statetype s_NMEsaucer2;
extern   statetype s_NMEsaucer3;
extern   statetype s_NMEsaucer4;
extern   statetype s_NMEhead1rl;
extern   statetype s_NMEhead2rl;
extern   statetype s_NMEspinattack;
extern   statetype s_NMEwheelspin;
extern   statetype s_NMEcollide;
extern   statetype s_NMEdeathbuildup;
extern   statetype s_NMEheadexplosion;
extern   statetype s_NMEstand;
extern   statetype s_NMEspinfire;
extern   statetype s_shootinghead;
extern   statetype s_oshuriken1;
extern   statetype s_oshuriken2;
extern   statetype s_oshuriken3;
extern   statetype s_oshuriken4;
extern   statetype s_oshurikenhit1;
extern   statetype s_oshurikenhit2;
extern   statetype s_oshurikenhit3;




extern   statetype s_darkmonkstand;
extern   statetype s_darkmonkcharge1;
extern   statetype s_darkmonkcharge2;
extern   statetype s_darkmonkcharge3;
extern   statetype s_darkmonkcharge4;
extern   statetype s_darkmonkcharge5;
extern   statetype s_darkmonkcharge6;
extern   statetype s_darkmonkcharge7;
extern   statetype s_darkmonkcharge8;
extern   statetype s_darkmonkcharge9;
extern   statetype s_darkmonkcharge10;
extern   statetype s_darkmonkreact;
extern   statetype s_darkmonkland;
extern   statetype s_darkmonkchase1;
extern   statetype s_darkmonkchase2;
extern   statetype s_darkmonkchase3;
extern   statetype s_darkmonkcover1;
extern   statetype s_darkmonkcover2;
extern   statetype s_darkmonkcover3;
extern   statetype s_darkmonkawaken1;
extern   statetype s_darkmonkawaken2;
extern   statetype s_darkmonkawaken3;
extern   statetype s_darkmonkawaken4;
extern   statetype s_darkmonkawaken5;
extern   statetype s_darkmonklightning1;
extern   statetype s_darkmonklightning2;
extern   statetype s_darkmonklightning3;
extern   statetype s_darkmonklightning4;
extern   statetype s_darkmonklightning5;
extern   statetype s_darkmonklightning6;
extern   statetype s_darkmonklightning7;
extern   statetype s_darkmonklightning8;
extern   statetype s_darkmonklightning9;
extern   statetype s_darkmonklightning10;
extern   statetype s_darkmonklightning11;
extern   statetype s_darkmonkfspark1;
extern   statetype s_darkmonkfspark2;
extern   statetype s_darkmonkfspark3;
extern   statetype s_darkmonkfspark4;
extern   statetype s_darkmonkfspark5;
extern   statetype s_darkmonkfspark6;
extern   statetype s_darkmonkbreathe1;
extern   statetype s_darkmonkbreathe2;
extern   statetype s_darkmonkbreathe3;
extern   statetype s_darkmonkbreathe4;
extern   statetype s_darkmonkbreathe5;
extern   statetype s_darkmonkbreathe6;
extern   statetype s_darkmonkbreathe7;
extern   statetype s_darkmonkbreathe8;
extern   statetype s_darkmonksummon1;
extern   statetype s_darkmonksummon2;
extern   statetype s_darkmonksummon3;
extern   statetype s_darkmonkhead;
extern   statetype s_darkmonkhspawn;
extern   statetype s_darkmonksneer1;
extern   statetype s_darkmonkheadhappy;
extern   statetype s_darkmonkheaddead;
extern   statetype s_darkmonkheaddie1;
extern   statetype s_darkmonkhball1;
extern   statetype s_darkmonkhball2;
extern   statetype s_darkmonkhball3;
extern   statetype s_darkmonkhball4;
extern   statetype s_darkmonkhball5;
extern   statetype s_darkmonkhball6;
extern   statetype s_darkmonkhball7;
extern   statetype s_darkmonkhball8;
extern   statetype s_darkmonkhball9;
extern   statetype s_darkmonksphere1;
extern   statetype s_darkmonksphere2;
extern   statetype s_darkmonksphere3;
extern   statetype s_darkmonksphere4;
extern   statetype s_darkmonksphere5;
extern   statetype s_darkmonksphere6;
extern   statetype s_darkmonksphere7;
extern   statetype s_darkmonksphere8;
extern   statetype s_darkmonksphere9;
extern   statetype s_darkmonksphere10;
extern   statetype s_darkmonkscare1;
extern   statetype s_darkmonkscare2;
extern   statetype s_darkmonkscare3;
extern   statetype s_darkmonkscare4;
extern   statetype s_darkmonkscare5;
extern   statetype s_darkmonkdead;
extern   statetype s_darkmonkdie1;
extern   statetype s_darkmonkdie2;
extern   statetype s_darkmonkdie3;
extern   statetype s_darkmonkdie4;
extern   statetype s_darkmonkdie5;
extern   statetype s_darkmonkdie6;
extern   statetype s_darkmonkdie7;
extern   statetype s_darkmonkredhead;
extern   statetype s_redheadhit;
extern   statetype s_darkmonksnakelink;
extern   statetype s_darkmonkredlink;
extern   statetype s_redlinkhit;
extern   statetype s_energysphere1;
extern   statetype s_energysphere2;
extern   statetype s_energysphere3;
extern   statetype s_energysphere4;
extern   statetype s_lightning;
extern   statetype s_handball2;
extern   statetype s_handball1;
extern   statetype s_faceball2;
extern   statetype s_faceball1;
extern   statetype s_floorspark1;
extern   statetype s_floorspark2;
extern   statetype s_floorspark3;
extern   statetype s_floorspark4;
extern   statetype s_dmlandandfire;

extern   statetype s_darkmonkhball7;
extern   statetype s_darkmonkbball1;
extern   statetype s_darkmonkbball2;
extern   statetype s_darkmonkbball3;
extern   statetype s_darkmonkbball4;
extern   statetype s_darkmonkbball5;
extern   statetype s_darkmonkbball6;
extern   statetype s_darkmonkbball7;
extern   statetype s_darkmonkbball8;
extern   statetype s_darkmonkbball9;
extern   statetype s_darkmonklightning9;
extern   statetype s_darkmonkfspark5;
extern   statetype s_darkmonkbreathe6;
extern   statetype s_darkmonkabsorb1;
extern   statetype s_darkmonkabsorb2;
extern   statetype s_darkmonkabsorb3;
extern   statetype s_darkmonkabsorb4;
extern   statetype s_darkmonkabsorb5;
extern   statetype s_darkmonkabsorb6;
extern   statetype s_darkmonkabsorb7;
extern   statetype s_darkmonkabsorb8;
extern   statetype s_darkmonkabsorb9;
extern   statetype s_dmgreenthing1;
extern   statetype s_dmgreenthing2;
extern   statetype s_dmgreenthing3;
extern   statetype s_dmgreenthing4;
extern   statetype s_dmgreenthing5;
extern   statetype s_dmgreenthing6;
extern   statetype s_dmgreenthing7;
extern   statetype s_dmgreenthing8;
extern   statetype s_dmgreenthing9;
extern   statetype s_dmgreenthing10;
extern   statetype s_darkmonkfastspawn;
extern   statetype s_spit1;
extern   statetype s_spit2;
extern   statetype s_spit3;
extern   statetype s_spit4;
extern   statetype s_spithit1;
extern   statetype s_spithit2;
extern   statetype s_spithit3;
extern   statetype s_spithit4;
extern   statetype s_snakefire1;
extern   statetype s_snakefire2;
extern   statetype s_snakepath;
extern   statetype s_snakefindpath;
extern   statetype s_snakefireworks1;
extern   statetype s_snakefireworks2;


extern   statetype s_speardown1;
extern   statetype s_speardown2;
extern   statetype s_speardown3;
extern   statetype s_speardown4;
extern   statetype s_speardown5;
extern   statetype s_speardown6;
extern   statetype s_speardown7;
extern   statetype s_speardown8;
extern   statetype s_speardown9;
extern   statetype s_speardown10;
extern   statetype s_speardown11;
extern   statetype s_speardown12;
extern   statetype s_speardown13;
extern   statetype s_speardown14;
extern   statetype s_speardown15;
extern   statetype s_speardown16;


extern   statetype s_downblade1;
extern   statetype s_downblade2;
extern   statetype s_downblade3;
extern   statetype s_downblade4;
extern   statetype s_downblade5;
extern   statetype s_downblade6;
extern   statetype s_downblade7;
extern   statetype s_downblade8;
extern   statetype s_downblade9;
extern   statetype s_downblade10;
extern   statetype s_downblade11;
extern   statetype s_downblade12;
extern   statetype s_downblade13;
extern   statetype s_downblade14;
extern   statetype s_downblade15;
extern   statetype s_downblade16;


extern   statetype s_firejetdown1;
extern   statetype s_firejetdown2;
extern   statetype s_firejetdown3;
extern   statetype s_firejetdown4;
extern   statetype s_firejetdown5;
extern   statetype s_firejetdown6;
extern   statetype s_firejetdown7;
extern   statetype s_firejetdown8;
extern   statetype s_firejetdown9;
extern   statetype s_firejetdown10;
extern   statetype s_firejetdown11;
extern   statetype s_firejetdown12;
extern   statetype s_firejetdown13;
extern   statetype s_firejetdown14;
extern   statetype s_firejetdown15;
extern   statetype s_firejetdown16;
extern   statetype s_firejetdown17;
extern   statetype s_firejetdown18;
extern   statetype s_firejetdown19;
extern   statetype s_firejetdown20;
extern   statetype s_firejetdown21;
extern   statetype s_firejetdown22;
extern   statetype s_firejetdown23;

extern   statetype s_columnupup1;
extern   statetype s_columnupup2;
extern   statetype s_columnupup3;
extern   statetype s_columnupup4;
extern   statetype s_columnupup5;
extern   statetype s_columnupup6;
extern   statetype s_columnupup7;
extern   statetype s_columnupup8;
extern   statetype s_columnupdown1;
extern   statetype s_columnupdown2;
extern   statetype s_columnupdown3;
extern   statetype s_columnupdown4;
extern   statetype s_columnupdown5;
extern   statetype s_columnupdown6;

extern   statetype s_spinupblade1;
extern   statetype s_spinupblade2;
extern   statetype s_spinupblade3;
extern   statetype s_spinupblade4;
extern   statetype s_spinupblade5;
extern   statetype s_spinupblade6;
extern   statetype s_spinupblade7;
extern   statetype s_spinupblade8;
extern   statetype s_spinupblade9;
extern   statetype s_spinupblade10;
extern   statetype s_spinupblade11;
extern   statetype s_spinupblade12;
extern   statetype s_spinupblade13;
extern   statetype s_spinupblade14;
extern   statetype s_spinupblade15;
extern   statetype s_spinupblade16;
extern   statetype s_spindownblade1;
extern   statetype s_spindownblade2;
extern   statetype s_spindownblade3;
extern   statetype s_spindownblade4;
extern   statetype s_spindownblade5;
extern   statetype s_spindownblade6;
extern   statetype s_spindownblade7;
extern   statetype s_spindownblade8;
extern   statetype s_spindownblade9;
extern   statetype s_spindownblade10;
extern   statetype s_spindownblade11;
extern   statetype s_spindownblade12;
extern   statetype s_spindownblade13;
extern   statetype s_spindownblade14;
extern   statetype s_spindownblade15;
extern   statetype s_spindownblade16;

extern   statetype s_boulderdrop1;
extern   statetype s_boulderdrop2;
extern   statetype s_boulderdrop3;
extern   statetype s_boulderdrop4;
extern   statetype s_boulderdrop5;
extern   statetype s_boulderdrop6;
extern   statetype s_boulderdrop7;
extern   statetype s_boulderdrop8;
extern   statetype s_boulderdrop9;
extern   statetype s_boulderdrop10;
extern   statetype s_boulderdrop11;
extern   statetype s_boulderdrop12;
extern   statetype s_boulderspawn;
extern   statetype s_bouldersink1;
extern   statetype s_bouldersink2;
extern   statetype s_bouldersink3;
extern   statetype s_bouldersink4;
extern   statetype s_bouldersink5;
extern   statetype s_bouldersink6;
extern   statetype s_bouldersink7;
extern   statetype s_bouldersink8;
extern   statetype s_bouldersink9;
extern   statetype s_boulderroll1;
extern   statetype s_boulderroll2;
extern   statetype s_boulderroll3;
extern   statetype s_boulderroll4;
extern   statetype s_boulderroll5;
extern   statetype s_boulderroll6;
extern   statetype s_boulderroll7;
extern   statetype s_boulderroll8;

extern   statetype s_gunfire1;
extern   statetype s_gunfire2;
extern   statetype s_gundead;
extern   statetype s_gundie1;
extern   statetype s_gunstand;
extern   statetype s_gunraise1;
extern   statetype s_gunraise2;
extern   statetype s_gunraise3;
extern   statetype s_gunraise4;
extern   statetype s_gunlower1;
extern   statetype s_gunlower2;
extern   statetype s_gunlower3;
extern   statetype s_4waygunfire1;
extern   statetype s_4waygunfire2;
extern   statetype s_4waygun;


extern   statetype s_kessphere1;
extern   statetype s_kessphere2;
extern   statetype s_kessphere3;
extern   statetype s_kessphere4;
extern   statetype s_kessphere5;
extern   statetype s_kessphere6;
extern   statetype s_kessphere7;
extern   statetype s_kessphere8;
extern   statetype s_batblast1;
extern   statetype s_batblast2;
extern   statetype s_batblast3;
extern   statetype s_batblast4;
extern   statetype s_slop1;
extern   statetype s_slop2;
extern   statetype s_slop3;
extern   statetype s_slop4;



extern   statetype s_serialdog4;
extern   statetype s_serialdog3;
extern   statetype s_serialdog2;
extern   statetype s_serialdog;
extern   statetype s_serialdogattack;
extern   statetype s_doguse;
extern   statetype s_doglick;
extern   statetype s_dogwait;



#endif






#endif

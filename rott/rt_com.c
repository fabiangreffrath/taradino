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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include "rt_def.h"
#include "_rt_com.h"
#include "rt_com.h"
#include "rt_util.h"
#include "rt_in.h"
#include "rt_crc.h"
#include "rt_playr.h"
#include "isr.h"
#include "rt_msg.h"
#include "rottnet.h"
#include "rt_main.h"
#include "rt_net.h"
#include "rt_draw.h"
//#include "rt_ser.h"

// GLOBAL VARIABLES

// Same as in real mode
rottcom_t   *	rottcom;
int badpacket;
int consoleplayer;
uint8_t ROTTpacket[MAXCOMBUFFERSIZE];
int controlsynctime;

// LOCAL VARIABLES

static int    ComStarted=FALSE;
static int    transittimes[MAXPLAYERS];

void SyncTime( int client );
void SetTransitTime( int client, int time );

#ifndef _WIN32

static void ReadUDPPacket()
{
	rottcom->remotenode = -1;
}

static void WriteUDPPacket()
{
}

#endif

/*
===============
=
= InitROTTNET
=
===============
*/

void InitROTTNET (void)
{
#ifndef _WIN32
	int netarg;
#endif

	if (ComStarted==TRUE)
		return;
	ComStarted=TRUE;

#ifndef _WIN32
	/*
	server-specific options:
	-net: enables netplay
	-server: run as rott server (default port 34858)
	-port: select a non-default port for server
	-host: select a non-default ip address to bind to
	-standalone: run as standalone server
	-remoteridicule: enable remote ridicule
	-players: number of players to expect

	client-specific options:
	-master: request to have control
	-net: specifies the host to connect to
	-port: select a non-default port to connect to
	*/
	
        rottcom = (rottcom_t *) malloc (sizeof(rottcom_t));
        memset(rottcom, 0, sizeof(rottcom_t));
        
        rottcom->ticstep = 1;
        rottcom->gametype = 1;
        rottcom->remotenode = -1;
        
        if (CheckParm("server")) {
        	if (CheckParm("standalone")) {
        		rottcom->consoleplayer = 0;
        	} else {
        		rottcom->consoleplayer = 1;
        	}
        	
        	if (CheckParm("remoteridicule")) {
        		rottcom->remoteridicule = 1;
        	} else {
        		rottcom->remoteridicule = 0;
        	}
        	
        	netarg = CheckParm("players");
        	if (netarg && netarg < _argc-1) {
        		rottcom->numplayers = atoi(_argv[netarg+1]);
        	} else {
        		rottcom->numplayers = 2;
        	}
        	
        	rottcom->client = 0;
        } else {
        	rottcom->client = 1;
        	
        	/* consoleplayer will be initialized after connecting */
        	/* numplayers will be initialized after connecting */
        	/* remoteridicule will be initialized after connecting */
        }
        
        /* client-server negotiation protocol, as inspired by ipxsetup.c */
        /*
          Best case:
          client sends a HereIAm packet.
          server replies with a YouAre packet, and broadcasts an Info packet
            to the rest, indicating that the new player has joined.
          client replies with an IAm packet
          until all players have joined, the server broadcasts an Info packet
            to the rest, indicating that another player is needed.
          once server has enough players, it broadcasts an AllDone packet.
          
          In detail:
          Client state: HereIAm (initial connection)
          Client sends HereIAm packet, waits for YouAre from server.
          At timeout, Client resends HereIam
          When client receives YouAre, client switches to IAm state
          
          Client state: IAm
          Client sends IAm packet, waits for IAmAck from server.
          At timeout, Client resends IAm
          When client receives IAmAck, client switches to WaitForDone state.
          
          Client state: WaitForDone
          Client waits for AllDone packet.
          When client receives AllDone, it sends an AllDoneAck.
         */
        
#endif

   remoteridicule = FALSE;
   remoteridicule = rottcom->remoteridicule;
   if (rottcom->ticstep != 1)
      remoteridicule = FALSE;
   if (remoteridicule == TRUE)
      {
      if (!quiet)
         printf("ROTTNET: LIVE Remote Ridicule Enabled\n");
      }

   if (!quiet)
      {
      printf("ROTTNET: consoleplayer=%ld\n",(long)rottcom->consoleplayer);
      }
}

/*
================
=
= ReadPacket
=
================
*/

bool8_t ReadPacket (void)
{
   uint16_t   crc;
   uint16_t   sentcrc;

   // Set command (Get Packet)
	rottcom->command=CMD_GET;

   badpacket = 0;

   // Check to see if a packet is ready

#ifndef _WIN32
	ReadUDPPacket();
#endif

   // Is it ready?

   if (rottcom->remotenode!=-1)
      {
      // calculate crc on packet
      crc=CalculateCRC ((uint8_t *)&rottcom->data[0], rottcom->datalength-sizeof(uint16_t));

      // get crc inside packet
      sentcrc=*((uint16_t *)(&rottcom->data[rottcom->datalength-sizeof(uint16_t)]));

      // are the crcs the same?
      if (crc!=sentcrc)
         {
         badpacket=1;
         SoftError("BADPKT at %d\n",GetTicCount());
         }
      if (networkgame==FALSE)
         {
         rottcom->remotenode=server;
         }
      else
         {
         if ((IsServer==TRUE) && (rottcom->remotenode>0))
            rottcom->remotenode--;
         }
      memcpy(&ROTTpacket[0], &rottcom->data[0], rottcom->datalength);

//      SoftError( "ReadPacket: time=%ld size=%ld src=%ld type=%d\n",GetTicCount(), rottcom->datalength,rottcom->remotenode,rottcom->data[0]);

      return TRUE;
      }
   else // Not ready yet....
      return FALSE;
}


/*
=============
=
= WritePacket
=
=============
*/

void WritePacket (void * buffer, int len, int destination)
{
   uint16_t      crc;

    if (!rottcom)
        return;

   // set send command
	rottcom->command=CMD_SEND;

   // set destination
   rottcom->remotenode=destination;

   if (len>(int)(MAXCOMBUFFERSIZE-sizeof(uint16_t)))
      {
      Error("WritePacket: Overflowed buffer\n");
      }

   // copy local buffer into realmode buffer
   memcpy((uint8_t *)&(rottcom->data[0]),(uint8_t *)buffer,len);

   // calculate CRC
   crc=CalculateCRC (buffer, len);

   // put CRC into realmode buffer packet
   *((uint16_t *)&rottcom->data[len])=crc;

   // set size of realmode packet including crc
   rottcom->datalength=len+sizeof(uint16_t);

   if (*((uint8_t *)buffer)==0)
       Error("Packet type = 0\n");

   if (networkgame==TRUE)
      {
      if (IsServer==TRUE)
         rottcom->remotenode++; // server fix-up
      }

//   SoftError( "WritePacket: time=%ld size=%ld src=%ld type=%d\n",GetTicCount(),rottcom->datalength,rottcom->remotenode,rottcom->data[0]);
   // Send It !
#ifndef _WIN32
	WriteUDPPacket();
#endif
}




/*
=============
=
= ValidSyncPacket
=
=============
*/
bool8_t ValidSyncPacket ( synctype * sync )
{
   if (ReadPacket() && (badpacket==0))
      {
      if (((syncpackettype *)&(ROTTpacket[0]))->type==COM_SYNC)
         {
         memcpy(&(sync->pkt),&(ROTTpacket[0]),sizeof(sync->pkt));
         return TRUE;
         }
      }
   return FALSE;
}

/*
=============
=
= SendSyncPacket
=
=============
*/
void SendSyncPacket ( synctype * sync, int dest)
{
   sync->pkt.type=COM_SYNC;
   sync->sendtime=GetTicCount();
   WritePacket( &(sync->pkt.type) , sizeof(syncpackettype) , dest );
}


/*
=============
=
= SlavePhaseHandler
=
=============
*/

bool8_t SlavePhaseHandler( synctype * sync )
{
   bool8_t done;

   done=FALSE;

   switch (sync->pkt.phase)
      {
      case SYNC_PHASE1:
         break;
      case SYNC_PHASE2:
         ISR_SetTime(sync->pkt.clocktime);
         break;
      case SYNC_PHASE3:
         sync->pkt.clocktime=GetTicCount();
         break;
      case SYNC_PHASE4:
         ISR_SetTime(GetTicCount()-sync->pkt.delta);
         sync->pkt.clocktime=GetTicCount();
         break;
      case SYNC_PHASE5:
         ISR_SetTime(GetTicCount()-sync->pkt.delta);
         sync->sendtime=sync->pkt.clocktime;
         done=TRUE;
         break;
      }
   return done;
}



/*
=============
=
= MasterPhaseHandler
=
=============
*/

bool8_t MasterPhaseHandler( synctype * sync )
{
   bool8_t done;

   done=FALSE;

   switch (sync->pkt.phase)
      {
      case SYNC_PHASE1:
         sync->pkt.phase=SYNC_PHASE2;
         sync->pkt.clocktime=GetTicCount()+(sync->deltatime>>1);
         break;
      case SYNC_PHASE2:
         sync->pkt.phase=SYNC_PHASE3;
         break;
      case SYNC_PHASE3:
         sync->pkt.delta=sync->pkt.clocktime-GetTicCount()+(sync->deltatime>>1);
         sync->pkt.phase=SYNC_PHASE4;
         break;
      case SYNC_PHASE4:
         sync->pkt.phase=SYNC_PHASE5;
         sync->pkt.delta=sync->pkt.clocktime-GetTicCount()+(sync->deltatime>>1);
         sync->sendtime=GetTicCount()+SYNCTIME;
         sync->pkt.clocktime=sync->sendtime;
         done=TRUE;
         break;
      }
   return done;
}


/*
=============
=
= ComSetTime
=
=============
*/

void ComSetTime ( void )
{
   int i;
   syncpackettype * syncpacket;
   bool8_t done=FALSE;

   syncpacket=(syncpackettype *)SafeMalloc(sizeof(syncpackettype));


   // Sync clocks

   if (networkgame==TRUE)
      {
      if (IsServer==TRUE)
         {
         for (i=0;i<numplayers;i++)
            {
            if (PlayerInGame(i)==FALSE)
               continue;
            if (standalone==TRUE)
               SyncTime(i);
            else if (i!=consoleplayer)
               SyncTime(i);
            if (standalone==TRUE)
               printf("ComSetTime: player#%ld\n",(long)i);
            }
         }
      else
         {
         SyncTime(0);
         }
      }
   else // Modem 2-player game
      {
      if (consoleplayer==0)
         SyncTime(server);
      else
         SyncTime(server);
      }

   if ( ( (networkgame==TRUE) && (IsServer==TRUE) ) ||
        ( (networkgame==FALSE) && (consoleplayer==0) )
      ) // Master/Server
      {
      int nump;
      int time;

      syncpacket->type=COM_START;
      syncpacket->clocktime=GetTicCount();
      controlsynctime=syncpacket->clocktime;
      if (networkgame==TRUE)
         nump=numplayers;
      else
         nump=1;

      time = GetTicCount();

      for (i=0;i<nump;i++)
         {
         WritePacket( &(syncpacket->type) , sizeof(syncpackettype) , i );
         }

      while (GetTicCount()<time+(VBLCOUNTER/4)) ;

      for (i=0;i<nump;i++)
         {
         WritePacket( &(syncpacket->type) , sizeof(syncpackettype) , i );
         }

      if (standalone==TRUE)
         printf("ComSetTime: Start packets sent\n");
      }
   else // Slave/Client
      {
      while (done==FALSE)
         {
         AbortCheck("ComSetTime aborted as client");

         if (ReadPacket() && (badpacket==0))
            {
            memcpy(syncpacket,&(ROTTpacket[0]),sizeof(syncpackettype));
            if (syncpacket->type==COM_START)
               {
               controlsynctime=syncpacket->clocktime;
               done=TRUE;
               }
            }
         }
      }
   if (standalone==FALSE)
      {
      AddMessage("All players synched.",MSG_SYSTEM);
      ThreeDRefresh();
      }
   SafeFree(syncpacket);

//
// flush out any extras
//
   while (GetTicCount()<controlsynctime+VBLCOUNTER)
      {
      ReadPacket ();
      }
}

/*
=============
=
= InitialMasterSync
=
=============
*/
void InitialMasterSync ( synctype * sync, int client )
{
   bool8_t done=FALSE;
   int i;

   if (networkgame==TRUE)
      {
      for (i=0;i<numplayers;i++)
         {
         if (i<=client)
            continue;
         sync->pkt.type=COM_SYNC;
         sync->pkt.phase=SYNC_MEMO;
         sync->pkt.clocktime=client;
         SendSyncPacket(sync,i);
         }
      }

   // Initialize send time so as soon as we enter the loop, we send

   sync->sendtime=GetTicCount()-SYNCTIME;

   while (done==FALSE)
      {
      sync->pkt.phase=SYNC_PHASE0;

      AbortCheck("Initial sync aborted as master");
	   if ((sync->sendtime+SYNCTIME) <= GetTicCount())
         SendSyncPacket(sync,client);
      if (ValidSyncPacket(sync)==TRUE)
         {
         if (sync->pkt.phase==SYNC_PHASE0)
            {
            int time=GetTicCount();

            while (time+SYNCTIME>GetTicCount())
               {
               ReadPacket();
               }
            time=GetTicCount();
            while (time+SYNCTIME>GetTicCount()) {}
            done=TRUE;
            }
         }
      }
}

/*
=============
=
= InitialSlaveSync
=
=============
*/
void InitialSlaveSync ( synctype * sync )
{
   bool8_t done=FALSE;

   while (done==FALSE)
      {
      AbortCheck("Initial sync aborted as slave");
      if (ValidSyncPacket(sync)==TRUE)
         {
         if (sync->pkt.phase==SYNC_MEMO)
            {
            char str[50]="Server is synchronizing player ";
            char str2[10];

            strcat(str,itoa(sync->pkt.clocktime+1,str2,10));
            AddMessage(str,MSG_SYSTEM);
            ThreeDRefresh();
            }
         if (sync->pkt.phase==SYNC_PHASE0)
            {
            int time=GetTicCount();

            SendSyncPacket(sync,server);
            while (time+SYNCTIME>GetTicCount())
               {
               ReadPacket();
               }
            done=TRUE;
            }
         }
      }
   AddMessage("Server is synchronizing your system",MSG_SYSTEM);
   ThreeDRefresh();
}


/*
=============
=
= SyncTime
=
=============
*/

void SyncTime( int client )
{
   int dtime[NUMSYNCPHASES];
   bool8_t done;
   int i;
   synctype * sync;

   sync=(synctype *)SafeMalloc(sizeof(synctype));

   if ( ((networkgame==TRUE) && (IsServer==TRUE)) ||
         ((networkgame==FALSE) && (consoleplayer==0)) )
      {
      // Master

      InitialMasterSync ( sync, client );

      done=FALSE;

      // Initial setup for Master
      // Initialize send time so as soon as we enter the loop, we send

      sync->pkt.phase=SYNC_PHASE1;
      sync->sendtime=GetTicCount()-SYNCTIME;

      while (done==FALSE)
         {
         // Master

         AbortCheck("SyncTime aborted as master");

		   if ((sync->sendtime+SYNCTIME) <= GetTicCount())
            SendSyncPacket(sync,client);

         while (ValidSyncPacket(sync)==TRUE)
            {

            // find average delta

            sync->deltatime=0;

            // calculate last delta

            dtime[sync->pkt.phase]=GetTicCount()-sync->sendtime;

            for (i=0;i<=sync->pkt.phase;i++)
               sync->deltatime+=dtime[i];
            if (i!=0)
               sync->deltatime/=i;
            else
               Error("SyncTime: this should not happen\n");

            done = MasterPhaseHandler( sync );

            SendSyncPacket(sync,client);

            }
         }
      }
   else
      {
      // Slave

      InitialSlaveSync ( sync );

      done=FALSE;

      while (done==FALSE)
         {
         // Slave

         AbortCheck("SyncTime aborted as slave");

         while (ValidSyncPacket(sync)==TRUE)
            {
            done = SlavePhaseHandler( sync );

            if (done==FALSE)
               SendSyncPacket(sync,server);

            }
         }
      }

   while (sync->sendtime > GetTicCount())
      {
      while (ReadPacket()) {}
      }
   while ((sync->sendtime+SYNCTIME) > GetTicCount())
      {
      }

   if ( ((networkgame==TRUE) && (IsServer==TRUE)) ||
         ((networkgame==FALSE) && (consoleplayer==0)) )
      SetTransitTime( client, (sync->deltatime>>1));

   SafeFree(sync);
}

/*
=============
=
= SetTransitTime
=
=============
*/

void SetTransitTime( int client, int time )
{
   transittimes[client]=time;
}

/*
=============
=
= GetTransitTime
=
=============
*/

int GetTransitTime( int client )
{
   return transittimes[client];
}


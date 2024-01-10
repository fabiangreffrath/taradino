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
#ifndef SPLIB_H
#define SPLIB_H

/* ======================================================================= *
 * Please Read "SpReadme.doc" for usage                                    *
 * ======================================================================= */

/* ======================================================================= *
 * Compiler & Memory Mode Wrappers.                                        *
 * ----------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------
// Constants

#define TSR_SERIAL_LENGTH		14
#define TSR_MAX_LENGTH			80

//---------------------------------------------------------------------------

typedef struct {
	char           copyright[TSR_MAX_LENGTH];	// driver copyright
	int16_t          major;							// driver version number
	int16_t				minor;
	int16_t				count;							// # of available devices
} SpwDrvOpenPacket;



typedef struct {
	char           copyright[TSR_MAX_LENGTH];	// device copyright
	char           serial[TSR_SERIAL_LENGTH];	// device serial number
} SpwDevOpenPacket;



typedef struct {
	long				timestamp;						// time of event
	uint16_t	period;							// period since last MOVEMENT
	uint16_t button;							// button pressed mask
	int16_t          tx;								// Translation X
	int16_t          ty;								//					Y
	int16_t          tz;								//					Z
	int16_t          rx;								// Rotation X
	int16_t          ry;								//				Y
	int16_t          rz;								//				Z
} SpwForcePacket;



typedef struct {
	long           timestamp;						// time of event
	uint16_t period;							// period since last BUTTON
	uint16_t button;							// button pressed mask
} SpwButtonPacket;



typedef struct {
	unsigned long data;								// MUST be TSRCMD_DATA
} SpwCommandPacket;

#define TSRCMD_DATA	0xFF0000FF

//---------------------------------------------------------------------------

typedef union {
	char              padding[128];		/* Extra room for future expansion */

	SpwCommandPacket	command;

	SpwDrvOpenPacket	drvOpen;
	SpwDevOpenPacket	devOpen;

	SpwForcePacket		force;
	SpwButtonPacket	button;
} SpwPacket;



// TSR Interrupt Functions
#define TSR_DRIVER_CLOSE		 		0x0000
#define TSR_DRIVER_OPEN					0x8001
#define TSR_DEVICE_CLOSE				0x0002
#define TSR_DEVICE_OPEN					0x8003

#define TSR_DEVICE_DISABLE				0x0010
#define TSR_DEVICE_ENABLE				0x0011

#define TSR_DEVICE_GETFORCE			0x8020
#define TSR_DEVICE_GETBUTTONS			0x8021

/* ======================================================================= *
 * Function Prototypes                                                     *
 * ======================================================================= */

int16_t SpwOpenDriver(SpwPacket *packet);
int16_t SpwCloseDriver(void);

int16_t SpwOpenDevice(int16_t device, SpwPacket *packet);
int16_t SpwCloseDevice(int16_t device);

int16_t SpwEnableDevice(int16_t device);
int16_t SpwDisableDevice(int16_t device);

int16_t SpwGetForce(int16_t device, SpwPacket *packet);
int16_t SpwGetButton(int16_t device, SpwPacket *packet);

/* ======================================================================= *
 * Convience functions                                                     *
 * ======================================================================= */

/* ----------------------------------------------------------------------- *
 * The SpwSimple... functions are just convienence wrappers for the above  *
 * functions.                                                              *
 * ----------------------------------------------------------------------- */

#ifndef SPWSTRUCTS
#define SPWSTRUCTS

enum SpwDeviceType {
	SPW_AVENGER=1,
};



enum SpwEventType {
	SPW_NO_EVENT=0,
   SPW_BUTTON_HELD=1,
   SPW_BUTTON_DOWN=2,
   SPW_BUTTON_UP=4,
   SPW_MOTION=8
};



/* ----------------------------------------------------------------------- *
 * Data struct for handling library calls                                  *
 * ----------------------------------------------------------------------- */

typedef struct {
	int16_t new;
   int16_t cur;
   int16_t old;
} SpwButtonRec;



typedef struct {
	int16_t 		 tx;			/* Current Translation vector */
   int16_t 		 ty;
   int16_t 		 tz;
   int16_t 		 rx;			/* Current Rotation vector    */
   int16_t 		 ry;
   int16_t        rz;
   SpwButtonRec buttons;   /* Current Button Record      */
   int16_t			 newData;   /* An SpEventType mask of newData, 0 if none */
} SpwRawData;

#endif



int16_t SpwSimpleGet(int16_t devNum, SpwRawData *splayer);
int16_t SpwSimpleOpen(int16_t devNum);
int16_t SpwSimpleClose(int16_t devNum);

/* ======================================================================= *
 * Compiler & Memory Mode Wrappers.                                        *
 * ======================================================================= *
 */

#ifdef __cplusplus
};
#endif

/* ======================================================================= */
#endif


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

#include <unordered_map>

#include "SDL.h"
#include "keyb.h"

#include "rt_scancodes.h"

static std::unordered_map<int, unsigned int> scancodes;

void InitScancodes (void)
{
/*
  all keys are now mapped to the wolf3d-style names,
  except where no such name is available.
 */
    scancodes.clear();

    scancodes[SDLK_UNKNOWN]         = sc_None;
    scancodes[SDLK_ESCAPE]          = sc_Escape;
    scancodes[SDLK_1]               = sc_1;
    scancodes[SDLK_2]               = sc_2;
    scancodes[SDLK_3]               = sc_3;
    scancodes[SDLK_4]               = sc_4;
    scancodes[SDLK_5]               = sc_5;
    scancodes[SDLK_6]               = sc_6;
    scancodes[SDLK_7]               = sc_7;
    scancodes[SDLK_8]               = sc_8;
    scancodes[SDLK_9]               = sc_9;
    scancodes[SDLK_0]               = sc_0;
    
    //scancodes[SDLK_EQUALS]          = 0x4E;
    scancodes[SDLK_EQUALS]          = sc_Equals;
    
    scancodes[SDLK_BACKSPACE]       = sc_BackSpace;
    scancodes[SDLK_TAB]             = sc_Tab;
    scancodes[SDLK_q]               = sc_Q;
    scancodes[SDLK_w]               = sc_W;
    scancodes[SDLK_e]               = sc_E;
    scancodes[SDLK_r]               = sc_R;
    scancodes[SDLK_t]               = sc_T;
    scancodes[SDLK_y]               = sc_Y;
    scancodes[SDLK_u]               = sc_U;
    scancodes[SDLK_i]               = sc_I;
    scancodes[SDLK_o]               = sc_O;
    scancodes[SDLK_p]               = sc_P;
    scancodes[SDLK_LEFTBRACKET]     = sc_OpenBracket;
    scancodes[SDLK_RIGHTBRACKET]    = sc_CloseBracket;
    scancodes[SDLK_RETURN]          = sc_Return;
    scancodes[SDLK_LCTRL]           = sc_Control;
    scancodes[SDLK_a]               = sc_A;
    scancodes[SDLK_s]               = sc_S;
    scancodes[SDLK_d]               = sc_D;
    scancodes[SDLK_f]               = sc_F;
    scancodes[SDLK_g]               = sc_G;
    scancodes[SDLK_h]               = sc_H;
    scancodes[SDLK_j]               = sc_J;
    scancodes[SDLK_k]               = sc_K;
    scancodes[SDLK_l]               = sc_L;
    scancodes[SDLK_SEMICOLON]       = 0x27;
    scancodes[SDLK_QUOTE]           = 0x28;
    scancodes[SDLK_BACKQUOTE]       = 0x29;
    
    /* left shift, but ROTT maps it to right shift in isr.c */
    scancodes[SDLK_LSHIFT]          = sc_RShift; /* sc_LShift */
    
    scancodes[SDLK_BACKSLASH]       = 0x2B;
    scancodes[SDLK_z]               = sc_Z;
    scancodes[SDLK_x]               = sc_X;
    scancodes[SDLK_c]               = sc_C;
    scancodes[SDLK_v]               = sc_V;
    scancodes[SDLK_b]               = sc_B;
    scancodes[SDLK_n]               = sc_N;
    scancodes[SDLK_m]               = sc_M;
    scancodes[SDLK_COMMA]           = sc_Comma;
    scancodes[SDLK_PERIOD]          = sc_Period;
    scancodes[SDLK_SLASH]           = 0x35;
    scancodes[SDLK_RSHIFT]          = sc_RShift;
    scancodes[SDLK_KP_DIVIDE]       = 0x35;
    
    /* 0x37 is printscreen */
    //scancodes[SDLK_KP_MULTIPLY]     = 0x37;
    
    scancodes[SDLK_LALT]            = sc_Alt;
    scancodes[SDLK_RALT]            = sc_Alt;
    scancodes[SDLK_MODE]            = sc_Alt;
    scancodes[SDLK_RCTRL]           = sc_Control;
    scancodes[SDLK_SPACE]           = sc_Space;
    scancodes[SDLK_CAPSLOCK]        = sc_CapsLock;
    scancodes[SDLK_F1]              = sc_F1;
    scancodes[SDLK_F2]              = sc_F2;
    scancodes[SDLK_F3]              = sc_F3;
    scancodes[SDLK_F4]              = sc_F4;
    scancodes[SDLK_F5]              = sc_F5;
    scancodes[SDLK_F6]              = sc_F6;
    scancodes[SDLK_F7]              = sc_F7;
    scancodes[SDLK_F8]              = sc_F8;
    scancodes[SDLK_F9]              = sc_F9;
    scancodes[SDLK_F10]             = sc_F10;
    scancodes[SDLK_F11]             = sc_F11;
    scancodes[SDLK_F12]             = sc_F12;
    scancodes[SDLK_NUMLOCKCLEAR]    = 0x45;
    scancodes[SDLK_SCROLLLOCK]      = 0x46;
    
    //scancodes[SDLK_MINUS]           = 0x4A;
    scancodes[SDLK_MINUS]           = sc_Minus;
    
    scancodes[SDLK_KP_7]            = sc_Home;
    scancodes[SDLK_KP_8]            = sc_UpArrow;
    scancodes[SDLK_KP_9]            = sc_PgUp;
    scancodes[SDLK_HOME]            = sc_Home;
    scancodes[SDLK_UP]              = sc_UpArrow;
    scancodes[SDLK_PAGEUP]          = sc_PgUp;
    // Make this a normal minus, for viewport changing
    //scancodes[SDLK_KP_MINUS]        = 0xE04A;
    scancodes[SDLK_KP_MINUS]        = sc_Minus;
    scancodes[SDLK_KP_4]            = sc_LeftArrow;
    scancodes[SDLK_KP_5]            = 0x4C;
    scancodes[SDLK_KP_6]            = sc_RightArrow;
    scancodes[SDLK_LEFT]            = sc_LeftArrow;
    scancodes[SDLK_RIGHT]           = sc_RightArrow;
    
    //scancodes[SDLK_KP_PLUS]         = 0x4E;
    scancodes[SDLK_KP_PLUS]         = sc_Plus;
    
    scancodes[SDLK_KP_1]            = sc_End;
    scancodes[SDLK_KP_2]            = sc_DownArrow;
    scancodes[SDLK_KP_3]            = sc_PgDn;
    scancodes[SDLK_END]             = sc_End;
    scancodes[SDLK_DOWN]            = sc_DownArrow;
    scancodes[SDLK_PAGEDOWN]        = sc_PgDn;
    scancodes[SDLK_DELETE]          = sc_Delete;
    scancodes[SDLK_KP_0]            = sc_Insert;
    scancodes[SDLK_INSERT]          = sc_Insert;
    scancodes[SDLK_KP_ENTER]        = sc_Return;
}

unsigned int GetScancode(int i)
{
    return scancodes[i];
}

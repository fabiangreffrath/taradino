#-----------------------------------------------------------------------------#
# ROTT makefile.
#-----------------------------------------------------------------------------#


#-----------------------------------------------------------------------------#
# If this makefile fails to detect Cygwin correctly, or you want to force
#  the build process's behaviour, set it to "true" or "false" (w/o quotes).
#-----------------------------------------------------------------------------#
#cygwin := true
#cygwin := false
cygwin := autodetect

# you only need to set these for Cygwin at the moment.
SDL_INC_DIR = /cygdrive/c/SDL/include
SDL_LIB_DIR = /cygdrive/c/SDL/lib


# Don't touch anything below this line unless you know what you're doing.

ifeq ($(strip $(cygwin)),autodetect)
  ifneq ($(strip $(shell gcc -v 2>&1 |grep "cygwin")),)
    cygwin := true
  else
    cygwin := false
  endif
endif


ifeq ($(strip $(cygwin)),true)
  ifeq ($(strip $(SDL_INC_DIR)),please_set_me_cygwin_users)
    $(error Cygwin users need to set the SDL_INC_DIR envr var.)
  else
    SDL_CFLAGS := -I$(SDL_INC_DIR)
  endif

  ifeq ($(strip $(SDL_LIB_DIR)),please_set_me_cygwin_users)
    $(error Cygwin users need to set the SDL_LIB_DIR envr var.)
  else
    SDL_LDFLAGS := -L$(SDL_LIB_DIR) -lSDL
  endif
else
  SDL_CFLAGS := $(shell sdl-config --cflags)
  SDL_LDFLAGS := $(shell sdl-config --libs)
  EXTRACFLAGS += -DUSE_EXECINFO=1
endif


CC = gcc
CFLAGS += -g -W -Wall -Wno-unused
ALL_CFLAGS = $(SDL_CFLAGS) -DUSE_SDL=1 -DPLATFORM_UNIX=1 $(CFLAGS) $(EXTRACFLAGS)
LDFLAGS += -Wl,-E
LDLIBS = $(LDFLAGS) $(SDL_LDFLAGS) -lSDL -lSDL_mixer $(EXTRALDFLAGS)

%.o: %.c
	$(CC) $(ALL_CFLAGS) -c $< -o $@

all: rott

audiolib/audiolib.a:
	$(MAKE) -C audiolib CC="$(CC)" CFLAGS="$(ALL_CFLAGS)" LDLIBS="$(LDLIBS)"

rott: 	\
	cin_actr.o \
	cin_efct.o \
	cin_evnt.o \
	cin_glob.o \
	cin_main.o \
	cin_util.o \
	dosutil.o \
	engine.o \
	isr.o \
	modexlib.o \
	rt_actor.o \
	rt_battl.o \
	rt_build.o \
	rt_cfg.o \
	rt_crc.o \
	rt_com.o \
	rt_debug.o \
	rt_dmand.o \
	rt_door.o \
	rt_draw.o \
	rt_floor.o \
	rt_game.o \
	rt_in.o \
	rt_main.o \
	rt_map.o \
	rt_menu.o \
	rt_msg.o \
	rt_net.o \
	rt_playr.o \
	rt_rand.o \
	rt_scale.o \
	rt_sound.o \
	rt_spbal.o \
	rt_sqrt.o \
	rt_stat.o \
	rt_state.o \
	rt_str.o \
	rt_swift.o \
	rt_ted.o \
	rt_util.o \
	rt_view.o \
	rt_vid.o \
	rt_err.o \
	scriplib.o \
	w_wad.o \
	watcom.o \
	z_zone.o \
	byteordr.o \
	dukemusc.o \
	audiolib/audiolib.a \
	winrott.o
	$(CC) $^ $(LDLIBS) -o $@

clean:
	$(MAKE) -C audiolib clean
	rm -rf rott rott.exe *.o

distclean: clean
	$(MAKE) -C audiolib distclean
	rm -rf *~

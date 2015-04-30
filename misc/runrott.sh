#!/bin/sh
# quick hack rott.rot generation in sh v0.1 by Steven Fuller
# inspired by Delphi Dude's RUNROTT:
# http://www.delphidude.com/personal/utilities/games/

CONFIG=$HOME/.rott/darkwar/rott.rot
EXE=rott

if [ $# != 1 ]; then
	echo "usage: "$0" <RTL or RTC filename>"
	exit 1
fi

if [ ! -r $1 ]; then
	echo "error: "$1" does not exist"
	exit 1
fi

cat > $CONFIG <<!END!
; Rise of the Triad Game Information File
; (c) 1997
PHONENUMBER          ~
REMOTESOUNDFILE      ~
GAMELEVELFILE        $1
COMMBATLEVELFILE     ~
!END!

exec $EXE

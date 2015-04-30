rott.xpm: rott.png
	convert $^ $@

rott.png: rott.svg
	inkscape --export-png=$@ $^

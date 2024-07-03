taradino.xpm: taradino.png
	convert $^ $@

taradino.png: taradino.svg
	inkscape --export-png=$@ $^

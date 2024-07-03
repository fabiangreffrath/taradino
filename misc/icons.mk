taradino.xpm: taradino.png
	convert $^ $@

taradino.png: taradino.svg
	inkscape --export-png=$@ $^

taradino.ico: taradino.svg
	convert -background transparent -define 'icon:auto-resize=16,24,32,64' $^ $@

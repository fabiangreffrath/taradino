FLVRS=huntbgin darkwar rottcd rottsite
ROTTS=$(addprefix rott-,$(FLVRS))
SRCDIR=rott

all: $(ROTTS)

# Shareware Version
rott-huntbgin:
	$(MAKE) -C $(SRCDIR) tidy
	$(MAKE) -C $(SRCDIR) ROTT=$@
	mv $(SRCDIR)/$@ .

# Commercial Version
rott-darkwar:
	$(MAKE) -C $(SRCDIR) tidy
	$(MAKE) -C $(SRCDIR) ROTT=$@ SHAREWARE=0
	mv $(SRCDIR)/$@ .

# CD Version
rott-rottcd:
	$(MAKE) -C $(SRCDIR) tidy
	$(MAKE) -C $(SRCDIR) ROTT=$@ SHAREWARE=0 SUPERROTT=1
	mv $(SRCDIR)/$@ .

# Site License CD Version
rott-rottsite:
	$(MAKE) -C $(SRCDIR) tidy
	$(MAKE) -C $(SRCDIR) ROTT=$@ SHAREWARE=0 SITELICENSE=1
	mv $(SRCDIR)/$@ .

clean:
	$(MAKE) -C $(SRCDIR) $@
	$(RM) $(ROTTS) $(addsuffix .exe,$(ROTTS))

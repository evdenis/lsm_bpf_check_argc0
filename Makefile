TOPTARGETS := all install clean

SUBDIRS := src share test

DESTDIR := /usr/local
export DESTDIR

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) --no-print-directory -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPTARGETS) $(SUBDIRS)

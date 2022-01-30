SUBDIRS = ApplicationA/src ApplicationB/src

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: $(SUBDIRS)

SUBDIRS = ApplicationA/src ApplicationB/src

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: $(SUBDIRS)

clean:
	rm -f ApplicationA/bin/*.bin
	rm -f ApplicationB/bin/*.bin
	rm -f ApplicationA/src/obj/*.o *~ core ApplicationA/inc/*~
	rm -f ApplicationB/src/obj/*.o *~ core ApplicationB/inc/*~

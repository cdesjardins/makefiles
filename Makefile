SUBDIRS = sub
.PHONY: $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean: $(addprefix clean_,$(SUBDIRS))

$(addprefix clean_,$(SUBDIRS)):
	$(MAKE) -C $(subst clean_,,$@) clean

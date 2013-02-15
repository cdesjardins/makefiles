SUBDIRS = exe shared static
.PHONY: $(SUBDIRS)


all: dist/deps $(SUBDIRS)

include dist/deps

$(SUBDIRS):
	@$(MAKE) -C $@

realclean: $(addprefix realclean_,$(SUBDIRS))
	@$(RM) -rf dist

clean: $(addprefix clean_,$(SUBDIRS))
	@$(RM) dist/deps

$(addprefix clean_,$(SUBDIRS)):
	@$(MAKE) -C $(subst clean_,,$@) clean

$(addprefix realclean_,$(SUBDIRS)):
	@$(MAKE) -C $(subst realclean_,,$@) realclean

dist/deps:
	@mkdir -p dist
	@for i in $(SUBDIRS); do ($(MAKE) -C $$i $(MFLAGS) projectdeps); done


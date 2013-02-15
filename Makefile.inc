CC          = $(CROSS_COMPILE)gcc
CXX         = $(CROSS_COMPILE)g++
AR          = $(CROSS_COMPILE)ar
SDIR        = src
CPP_SRCS    = $(wildcard $(SDIR)/*.cpp)
C_SRCS      = $(wildcard $(SDIR)/*.c)
OBJECTS	    = $(addprefix $(ODIR)/,$(notdir $(CPP_SRCS:.cpp=.o) $(C_SRCS:.c=.o)))
LIBS       += $(PRJLIBS)
LIBRARIES  += $(patsubst %,-l%,$(LIBS)) $(patsubst %,-L%,$(LIBDIRS))
GENFLAGS   += $(patsubst %,-I%,$(INCLUDEDIRS)) -Wall
CFLAGS     += $(GENFLAGS)
CXXFLAGS   += $(GENFLAGS)
DEPS        = $(patsubst %,%,$(OBJECTS:.o=.d))
MAKEDEPEND  = $(SILENT)$(CC) -MF"$(@:.o=.d)" -MG -MM -MP -MT"$(@:.d=.o)" "$<" $(CFLAGS) 
MKODIR      = -@mkdir -p $(ODIR)
MKDISTDIR   = -@mkdir -p $(DISTDIR)
DEBUG      ?= 1

# use DEBUG=1 or DEBUG=0 to enable/disable debug builds, default is DEBUG=1.

ifneq ($(DEBUG), 0)
GENFLAGS   += -g -DDEBUG
RDIR		= debug
else
GENFLAGS   += -O2 -DNDEBUG
RDIR		= release
endif

ODIR		= obj/$(RDIR)
DISTDIR		= ../dist/$(RDIR)

ifeq ("$(OUTEXE)", "")
OUTPUT		= $(DISTDIR)/$(OUTLIB)
else
OUTPUT		= $(DISTDIR)/$(OUTEXE)
endif

# use either V=1 or VERBOSE=1 to enable verbose mode

ifdef V
VERBOSE=1
endif

ifndef VERBOSE
SILENT=@
NICEDEPOUTPUT=@echo "  Depends  $@"
NICEBLDOUTPUT=@echo "  Building $@"
NICECLNOUTPUT=@echo "  Cleaning $(ODIR)"
endif

all: $(DEPS) $(OUTPUT)

projectdeps:
	$(NICEDEPOUTPUT)
	$(MKEDIR)
	echo "$(notdir $(CURDIR)): $(PRJLIBS)" >> ../dist/deps

$(ODIR)/%.d: $(SDIR)/%.*
	$(NICEDEPOUTPUT)
	$(MKODIR)
	$(MAKEDEPEND)

$(ODIR)/%.o: $(SDIR)/%.c
	$(NICEBLDOUTPUT)
	$(MKODIR)
	$(SILENT)$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(NICEBLDOUTPUT)
	$(MKODIR)
	$(SILENT)$(CXX) -c -o $@ $< $(CXXFLAGS)

$(DISTDIR)/%.a: $(OBJECTS)
	$(NICEBLDOUTPUT)
	$(MKDISTDIR)
	$(SILENT)$(AR) rcs $@ $^

$(DISTDIR)/%.so: $(OBJECTS)
	$(NICEBLDOUTPUT)
	$(MKDISTDIR)
	$(SILENT)$(CXX) -shared -fPIC -o $@ $^ $(LIBRARIES) $(ARCHIVES)

$(DISTDIR)/$(OUTEXE): $(OBJECTS) $(ARCHIVES)
	$(NICEBLDOUTPUT)
	$(MKDISTDIR)
	$(SILENT)$(CXX) -o $@ $^ $(CFLAGS) $(LIBRARIES) $(ARCHIVES)

.PHONY: clean

clean:
	$(NICECLNOUTPUT)
	$(SILENT)$(RM) $(ODIR)/*
	$(SILENT)$(RM) $(DISTDIR)/*

realclean:
	$(NICECLNOUTPUT)
	$(SILENT)$(RM) -rf obj

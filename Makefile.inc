CC          = $(CROSS_COMPILE)g++
AR          = $(CROSS_COMPILE)ar
SDIR        = src
CPP_SRCS    = $(wildcard $(SDIR)/*.cpp)
C_SRCS      = $(wildcard $(SDIR)/*.c)
OBJECTS	    = $(addprefix $(ODIR)/,$(notdir $(CPP_SRCS:.cpp=.o) $(C_SRCS:.c=.o)))
LIBRARIES  += $(patsubst %,-l%,$(LIBS)) $(patsubst %,-L%,$(LIBDIRS))
CFLAGS     += $(patsubst %,-I%,$(INCLUDEDIRS)) -Wall
DEPS        = $(patsubst %,%,$(OBJECTS:.o=.d))
MAKEDEPEND  = $(SILENT)$(CC) -MF"$(@:.o=.d)" -MG -MM -MP -MT"$(@:.d=.o)" "$<" $(CFLAGS) 
MKODIR      = -@mkdir -p $(ODIR)
DEBUG      ?= 1

# use DEBUG=1 or DEBUG=0 to enable/disable debug builds, default is DEBUG=1.

ifneq ($(DEBUG), 0)
CFLAGS     += -g -DDEBUG
ODIR        = obj/debug
else
CFLAGS     += -O2 -DNDEBUG
ODIR        = obj/release
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

all: $(DEPS) $(OUTEXE) $(OUTLIB)

$(ODIR)/%.d: $(SDIR)/%.*
	$(NICEDEPOUTPUT)
	$(MKODIR)
	$(MAKEDEPEND)

$(ODIR)/%.o: $(SDIR)/%.*
	$(NICEBLDOUTPUT)
	$(MKODIR)
	$(SILENT)$(CC) -c -o $@ $< $(CFLAGS)

$(OUTLIB): $(OBJECTS)
	$(NICEBLDOUTPUT)
	$(MKODIR)
	$(SILENT)$(AR) rcs $@ $^

$(OUTEXE): $(OBJECTS) $(ARCHIVES)
	$(NICEBLDOUTPUT)
	$(MKODIR)
	$(SILENT)$(CC) -o $@ $^ $(CFLAGS) $(LIBRARIES) $(ARCHIVES)

.PHONY: clean

clean:
	$(NICECLNOUTPUT)
	$(SILENT)$(RM) $(ODIR)/*

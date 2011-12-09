TOP := $(dir $(lastword $(MAKEFILE_LIST)))

CLEAN_FILES := 

# First rule
ALL := 
all:

# copy command
# CP := ln -s -f
CP := cp


VL_PATH   :=/home/pashkoff/vlfeat-0.9.13/
VL_BIN    :=$(VL_PATH)bin/glnxa64/
CIMG_PATH :=/home/pashkoff/d/code/CImg-1.4.9/


ARCH := linux

CONF := debug
#CONF := release

TARGET := $(ARCH)-$(CONF)
OBJDIR := obj/$(TARGET)
BINDIR := bin

CFLAGS := -I$(TOP) -Wall -I$(VL_PATH) -I$(CIMG_PATH)
CXXFLAGS := $(CFLAGS) -std=c++0x
LDFLAGS := -L$(BINDIR) -L$(VL_BIN) -lvl -lX11 -ljpeg -lboost_system -lboost_filesystem -lboost_program_options

ARCH_linux_CFLAGS := -pthread
ARCH_linux_LDFLAGS := -lrt -lpthread

		
TARGET_linux-debug_CFLAGS := -ggdb -DDEBUG -O0

TARGET_linux-release_CFLAGS := -ggdb -DNDEBUG -O2


CFLAGS += $(ARCH_$(ARCH)_CFLAGS) $(TARGET_$(TARGET)_CFLAGS)
CXXFLAGS += $(ARCH_$(ARCH)_CFLAGS) $(TARGET_$(TARGET)_CFLAGS)
LDFLAGS += $(ARCH_$(ARCH)_LDFLAGS)

ARCH_linux_CC = $(CC)
ARCH_linux_CXX = $(CXX)

CC := $(ARCH_$(ARCH)_CC)
CXX := $(ARCH_$(ARCH)_CXX)

export LD_RUN_PATH=.:$(VL_BIN)

VL_SO :=$(BINDIR)/libvl.so

### List of included makefiles
include HIKMTree/Makefile
include Image/Makefile
include isifter/Makefile
include ivf_creator/Makefile
include ivfile/Makefile
include iwords/Makefile
include query_maker/Makefile
include Sift/Makefile
include tree_creator/Makefile
include Util/Makefile


$(VL_SO): $(VL_BIN)libvl.so
	$(CP) $^ $@

$(BINDIR): 
	mkdir -p $@
	
$(OBJDIR):
	mkdir -p $@
	
clean: 
	-$(RM) -r $(OBJDIR)
	# -$(RM) -r $(BINDIR)
	-$(RM) $(CLEAN_FILES)
	
all: $(ALL)
	@echo $(ALL)

.PHONY: all clean

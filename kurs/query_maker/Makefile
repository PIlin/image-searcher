# $Id: mf 406 2011-09-20 13:09:01Z dlobashevskiy $

LOCAL_TOP := $(dir $(lastword $(MAKEFILE_LIST)))

OUT_NAME := query_maker
FNAME := $(OUT_NAME)

SRC_DIR := $(LOCAL_TOP)
SRC :=  main.cpp 
LIBS := Util Image Sift HIKMTree ivfile
STD_LIBS := 

LOCAL_LDFLAGS := 
LOCAL_CXXFLAGS := -I$(LOCAL_TOP)include

include build-exec.mk

$(OUT_NAME): $(VL_SO)

ALL += $(OUT_NAME)
.PHONY: $(OUT_NAME)

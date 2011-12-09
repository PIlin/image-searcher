LOCAL_TOP := $(dir $(lastword $(MAKEFILE_LIST)))

OUT_NAME := Image
FNAME := lib$(OUT_NAME).a

SRC_DIR := $(LOCAL_TOP)
SRC := Image.cpp Image_pimpl.cpp


LIBS := 
STD_LIBS := 

LOCAL_LDFLAGS := 
LOCAL_CXXFLAGS := -I$(LOCAL_TOP)include

include build-static.mk

ALL += $(OUT_NAME)
.PHONY: $(OUT_NAME)

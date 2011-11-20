include basefiles.mk

BASE_DIR := ./base

all:


SIFT_BASEFILE_FUNC = $(addprefix $(BASE_DIR)/,$(addsuffix .sift,$(subst :,,$(subst /,_,$(1)))))

# BASEFILES := d:/Downloads/caltech_101/kangaroo/image_0001.jpg

# ALL_SIFT_FILES := $(abspath ./base/d_Downloads_caltech_101_kangaroo_image_0001.jpg.sift)
ALL_SIFT_FILES := $(abspath $(call SIFT_BASEFILE_FUNC,$(BASEFILES)) )

TREE_FILE := $(abspath $(BASE_DIR)/hikm.tree)

define SIFT_FILE_template
$(abspath $(call SIFT_BASEFILE_FUNC,$(1)) ) : $(1) | $(BASE_DIR)
	isifter $$< $$@
endef
$(foreach file,$(BASEFILES),$(eval $(call SIFT_FILE_template,$(file))))


# $(abspath ./base/d_Downloads_caltech_101_kangaroo_image_0001.jpg.sift): d:/Downloads/caltech_101/kangaroo/image_0001.jpg
	# isifter.exe $< $@

$(TREE_FILE): $(ALL_SIFT_FILES) | $(BASE_DIR)
	tree_creator $@ $^


all: $(TREE_FILE)






$(BASE_DIR): 
	mkdir -p $@


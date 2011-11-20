include basefiles.mk

BASE_DIR := ./cache

nothing:


SIFT_BASEFILE_FUNC = $(addprefix $(BASE_DIR)/,$(addsuffix .sift,$(subst :,,$(subst /,_,$(1)))))

# BASEFILES := d:/Downloads/caltech_101/kangaroo/image_0001.jpg

# ALL_SIFT_FILES := $(abspath ./base/d_Downloads_caltech_101_kangaroo_image_0001.jpg.sift)
ALL_SIFT_FILES := $(abspath $(call SIFT_BASEFILE_FUNC,$(BASEFILES)) )

ALL_WORD_FILES := $(patsubst %.sift,%.word,$(ALL_SIFT_FILES))

TREE_FILE := $(abspath $(BASE_DIR)/hikm.tree)

IVF_FILE := $(abspath $(BASE_DIR)/ivf.file)

define SIFT_FILE_template
$(abspath $(call SIFT_BASEFILE_FUNC,$(1)) ) : $(1) | $(BASE_DIR)
	isifter $$< $$@
endef
$(foreach file,$(BASEFILES),$(eval $(call SIFT_FILE_template,$(file))))


# $(abspath ./base/d_Downloads_caltech_101_kangaroo_image_0001.jpg.sift): d:/Downloads/caltech_101/kangaroo/image_0001.jpg
	# isifter.exe $< $@

$(TREE_FILE): $(ALL_SIFT_FILES) | $(BASE_DIR)
	tree_creator $@ $^


%.word : $(TREE_FILE) %.sift | $(BASE_DIR)
	iwords $^ $@

$(IVF_FILE): $(TREE_FILE) $(ALL_WORD_FILES) | $(BASE_DIR)
	ivf_creator $@ $^
	
# all: $(TREE_FILE)
# base: $(ALL_WORD_FILES)
base: $(IVF_FILE)



clean:
	rm -rf $(BASE_DIR)


$(BASE_DIR): 
	mkdir -p $@

.PHONY: nothing base clean query
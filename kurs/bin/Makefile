BASEFILES_LIST := basefiles.mk


BASE_DIR := ./cache
FILELIST_FILE := $(abspath $(BASE_DIR)/file.list)
TREE_FILE := $(abspath $(BASE_DIR)/hikm.tree)
IVF_FILE := $(abspath $(BASE_DIR)/ivf.file)

include $(BASEFILES_LIST)

nothing:

SIFT_BASEFILE_FUNC = $(addprefix $(BASE_DIR)/,$(addsuffix .sift,$(subst :,,$(subst /,_,$(1)))))

ALL_SIFT_FILES := $(abspath $(call SIFT_BASEFILE_FUNC,$(BASEFILES)) )
ALL_WORD_FILES := $(patsubst %.sift,%.word,$(ALL_SIFT_FILES))


define SIFT_FILE_template
$(abspath $(call SIFT_BASEFILE_FUNC,$(1)) ) : $(1) | $(BASE_DIR)
	isifter $$< $$@
endef
$(foreach file,$(BASEFILES),$(eval $(call SIFT_FILE_template,$(file))))

$(TREE_FILE): $(ALL_SIFT_FILES) $(BASEFILES_LIST) | $(BASE_DIR)
	tree_creator $@ $(ALL_SIFT_FILES)


%.word : $(TREE_FILE) %.sift | $(BASE_DIR)
	iwords $^ $@

$(IVF_FILE): $(TREE_FILE) $(ALL_WORD_FILES) $(BASEFILES_LIST) | $(BASE_DIR)
	ivf_creator $@ $(TREE_FILE) $(ALL_WORD_FILES)
	
$(FILELIST_FILE): $(BASEFILES_LIST) | $(BASE_DIR)
	echo $(BASEFILES) | tr ' ' '\n' > $@
	
# all: $(TREE_FILE)
# base: $(ALL_WORD_FILES)
base: $(IVF_FILE) $(FILELIST_FILE)



clean:
	rm -rf $(BASE_DIR)


query:
	query_maker -i d:\Downloads\caltech_101\kangaroo\image_0001.jpg -t cache\hikm.tree -v cache\ivf.file -o --
	
$(BASE_DIR): 
	mkdir -p $@

.PHONY: nothing base clean query
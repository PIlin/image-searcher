BASEFILES_LIST := basefiles.mk
QUERY_IN_FILE := query.in
QUERY_OUT_FILE := query.out
RES_OUT_FILE := res.htm

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


query: $(QUERY_IN_FILE)
	query_maker -i $(shell cat $(QUERY_IN_FILE)) -t $(TREE_FILE) -v $(IVF_FILE) -o  $(QUERY_OUT_FILE)
	python query.py $(FILELIST_FILE) $(QUERY_IN_FILE) $(QUERY_OUT_FILE) $(RES_OUT_FILE)
	
$(BASE_DIR): 
	mkdir -p $@

.PHONY: nothing base clean query
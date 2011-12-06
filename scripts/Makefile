include xargs.mk

BASEFILES_LIST := basefiles.mk
QUERY_IN_FILE := query.in
QUERY_OUT_FILE := query.out
RES_OUT_FILE := res.htm

BASE_DIR := ./cache
FILELIST_FILE := $(abspath $(BASE_DIR)/file.list)
SIFTLIST_FILE := $(abspath $(BASE_DIR)/sift.list)
WORDLIST_FILE := $(abspath $(BASE_DIR)/word.list)
TREE_FILE := $(abspath $(BASE_DIR)/hikm.tree)
IVF_FILE := $(abspath $(BASE_DIR)/ivf.file)

BIN_DIR      := ../kurs/bin
ISIFTER      := $(BIN_DIR)/isifter
TREE_CREATOR := $(BIN_DIR)/tree_creator
IWORDS       := $(BIN_DIR)/iwords
IVF_CREATOR  := $(BIN_DIR)/ivf_creator
QUERY_MAKER  := $(BIN_DIR)/query_maker

include $(BASEFILES_LIST)

nothing:

SIFT_BASEFILE_FUNC = $(addprefix $(BASE_DIR)/,$(addsuffix .sift,$(subst :,,$(subst /,_,$(1)))))

ALL_SIFT_FILES := $(abspath $(call SIFT_BASEFILE_FUNC,$(BASEFILES)) )
ALL_WORD_FILES := $(patsubst %.sift,%.word,$(ALL_SIFT_FILES))


define SIFT_FILE_template
$(abspath $(call SIFT_BASEFILE_FUNC,$(1)) ) : $(1) | $(BASE_DIR)
	$(ISIFTER) $$< $$@
endef
$(foreach file,$(BASEFILES),$(eval $(call SIFT_FILE_template,$(file))))

$(TREE_FILE): $(ALL_SIFT_FILES) $(SIFTLIST_FILE) $(BASEFILES_LIST) | $(BASE_DIR)
	$(TREE_CREATOR) -o $@ -l $(SIFTLIST_FILE)


%.word : $(TREE_FILE) %.sift | $(BASE_DIR)
	$(IWORDS) $^ $@

$(IVF_FILE): $(TREE_FILE) $(ALL_WORD_FILES) $(WORDLIST_FILE) $(BASEFILES_LIST) | $(BASE_DIR)
	$(IVF_CREATOR) -o $@ -t $(TREE_FILE) -l $(WORDLIST_FILE)
	
$(FILELIST_FILE): $(BASEFILES_LIST) | $(BASE_DIR)
	$(call write-to-file,$@,$(BASEFILES))
	
$(SIFTLIST_FILE): $(BASEFILES_LIST) | $(BASE_DIR)
	$(call write-to-file,$@,$(ALL_SIFT_FILES))

$(WORDLIST_FILE): $(BASEFILES_LIST) | $(BASE_DIR)
	$(call write-to-file,$@,$(ALL_WORD_FILES))

# all: $(TREE_FILE)
# base: $(ALL_WORD_FILES)
base: $(FILELIST_FILE) $(TREE_FILE) $(IVF_FILE)
#base: $(FILELIST_FILE) $(SIFTLIST_FILE) $(WORDLIST_FILE)



clean:
	rm -rf $(BASE_DIR)


query: $(QUERY_IN_FILE)
	$(QUERY_MAKER) -i $(shell cat $(QUERY_IN_FILE)) -t $(TREE_FILE) -v $(IVF_FILE) -o  $(QUERY_OUT_FILE)
	query2html.py $(FILELIST_FILE) $(QUERY_IN_FILE) $(QUERY_OUT_FILE) $(RES_OUT_FILE)
	
$(BASE_DIR): 
	mkdir -p $@

.PHONY: nothing base clean query
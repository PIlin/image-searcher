include common.mk

$(FNAME_FULL_OBJ): $(OBJS_FULL) | $(OBJS_DIR) 
	ar cr $@ $^

$(FNAME_FULL_BIN): $(FNAME_FULL_OBJ) | $(BINDIR)
	$(CP) $< $@
	
$(OUT_NAME):  $(FNAME_FULL_BIN)
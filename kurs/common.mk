LOCAL_CXXFLAGS += $(CXXFLAGS)
LOCAL_CFLAGS += $(CFLAGS)

LOCAL_LDFLAGS += $(addprefix -l, $(LIBS))
LOCAL_LDFLAGS += $(addprefix -l, $(STD_LIBS))
LOCAL_LDFLAGS += $(LDFLAGS)

OBJS := $(SRC:.cpp=.o)
OBJS := $(OBJS:.c=.o)

OBJS_DIR := $(OBJDIR)/$(OUT_NAME)
OBJS_FULL := $(addprefix $(OBJS_DIR)/, $(OBJS))
DEPS_FULL :=  $(patsubst %.o,%.d,$(OBJS_FULL))

FNAME_FULL_OBJ := $(addprefix $(OBJS_DIR)/, $(FNAME))
FNAME_FULL_BIN := $(addprefix $(BINDIR)/, $(FNAME))

CLEAN_FILES += $(FNAME_FULL_BIN)

LIBS_DEPEND := $(patsubst %,$(BINDIR)/lib%.a,$(LIBS))

$(OBJS_DIR) :
	mkdir -p $@

$(OBJS_DIR)/%.o: PRIV_CXXFLAGS := $(LOCAL_CXXFLAGS)	
$(OBJS_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJS_DIR) 
	$(CXX) -c $< $(PRIV_CXXFLAGS) -MMD -o $@

$(OBJS_DIR)/%.o: PRIV_CFLAGS := $(LOCAL_CFLAGS)	
$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJS_DIR) 
	$(CC) -c $< $(PRIV_CFLAGS) -MMD -o $@
	
-include $(DEPS_FULL)

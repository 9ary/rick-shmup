.PHONY: all run clean cflags .FORCE

all: $(EXE)

include $(wildcard */rules.mk)

# Object dependency files
-include $(OBJS:%.o=%.d)

$(OUT)/%.o: %.c | $(BUILT_HEADERS)
	@echo "  CC      $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c | $(BUILT_HEADERS)
	@echo "  CC      $@"
	@$(CC) $(CFLAGS) -c $< -o $@

$(EXE): $(OBJS)
	@mkdir -p $(dir $@)
	@echo "  CCLD    $@"
	@+$(CC) $(LDFLAGS) $^ -o $@

run: all
	@echo "  $(EXE)"
	@./$(EXE)

clean:
	@echo "  RM      $(OUT)"
	@rm -rf $(OUT)

# YouCompleteMe configuration
cflags:
	@echo -n $(CFLAGS)

.FORCE:

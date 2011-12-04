# Macro for invoking a command on groups of 1000 words at a time
# (analogous to xargs(1)).  The macro invokes itself recursively
# until the list of words is depleted.
#
# Usage: $(call xargs,COMMAND,LIST)
#
# COMMAND should be a shell command to which the words will be
# appended as arguments in groups of 200.
define xargs
$(1) $(wordlist 1,200,$(2))
$(if $(word 201,$(2)),$(call xargs,$(1),$(wordlist 201,$(words $(2)),$(2))))
endef

# Macro for writing the contents of a word list to a file, one word
# per line.  This macro will work even for very long lists of words.
#
# Usage: $(call write-to-file,FILENAME,LIST)
define write-to-file
@: >$(1)
$(call xargs,@printf "%s\n" >>$(1),$(2))
endef
	
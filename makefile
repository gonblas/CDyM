# Directories
DIR = Practicas/Repaso
UTILS_DIR = utils/

# Source files
SOURCES = $(DIR)/b.c

ARGS = 123

# Arguments (you can modify these values for each iteration, allowing for multiple arguments together)
ARGS_LIST =

# Executable file
EXE = results

# Compiler
CC = gcc

# Compilation and linking flags
CFLAGS =
LDFLAGS =
LIBS = 

# Object files
OBJECTS = $(SOURCES:%.c=%.o)

# Main rule: compile, run, and clean
all: $(EXE) run clean

# Generate the executable
$(EXE): $(OBJECTS)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $(EXE) $(LIBS)

# Rule to compile .c files to .o files
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

# Run the program
run: $(EXE)
	@./$(EXE) $(ARGS)

# Clean up
clean:
	@-rm -f $(EXE) $(OBJECTS)

# Loop command to run the program with different argument values
loop: $(EXE)  # Ensures that the executable is compiled before running the loop
	@for args in $(ARGS_LIST); do \
		echo -e "\n################## Running with arguments $$args ##################\n"; \
		./$(EXE) $$args; \
	done; \
	$(MAKE) -s clean  # Deletes the executables and object files in silent mode

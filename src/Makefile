CC = clang
CFLAGS = -g -fPIC
INCLUDES = -I../crates/ -I../crates/dorton_utils/ -I../crates/app/ -I../crates/internal/ -I${GLFW}/include -I${cglm}/include 
LFLAGS = -L../bin -lcrates -Wl,-rpath,../bin
DEFINES = -DDIMPORT -D_DEBUG

ASSEMBLY = dorton 
BINDIR = ../bin/

TARGET = $(BINDIR)$(ASSEMBLY)
SRC = $(shell find . -name '*.c')
OBJ = $(SRC:%.c=$(BINDIR)%.o)

$(BINDIR)%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

$(TARGET): $(OBJ)
	echo "Building src"
	$(CC) $(OBJ) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LFLAGS) -o $@ 

.PHONY: all clean rebuild run vrun

all: $(TARGET)

clean: 
	rm -f $(OBJ) $(TARGET)

rebuild:
	make clean
	make

run:
	make
	$(TARGET)

vrun:
	make
	valgrind --leak-check=full --gen-suppressions=all --log-file=valgrind.log --suppressions=valgrind.supp $(TARGET)


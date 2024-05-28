CC = clang
CFLAGS = -g -fPIC
INCLUDES = -I../crates/app/ -I../crates/internal/ -I$(VULKAN_SDK)/include -I$(GLFW)/include
LFLAGS = -L../bin/ -lcrates -Wl,-rpath,$(shell pwd)/../bin/
DEFINES = -D_DEBUG -DDIMPORT

ASSEMBLY = dorton 
BINDIR = ../bin/

TARGET = $(BINDIR)$(ASSEMBLY)
SRC = $(shell find . -name '*.c')
OBJ = $(patsubst %.c,$(BINDIR)%.o,$(SRC))
BIN_OBJ = $(addprefix $(BINDIR),$(notdir $(OBJ)))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	echo "Building src"
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o $@ $^ $(LFLAGS)

$(BINDIR)%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $(BINDIR)$(notdir $@)

clean: 
	rm -f $(BIN_OBJ) $(TARGET)

rebuild:
	make clean
	make

run: $(TARGET)
	make clean
	make
	$(TARGET)

vrun:
	make clean
	make
	valgrind --leak-check=full --gen-suppressions=all --log-file=valgrind.log --suppressions=valgrind.supp $(TARGET)


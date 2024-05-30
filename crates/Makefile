CC = clang
CFLAGS = -g -fPIC
INCLUDES = -Iinternal -I$(VULKAN_SDK)/include -I$(GLFW)/include
LFLAGS = -shared -lvulkan -lxcb -lX11 -lX11-xcb -lxkbcommon -L$(VULKAN_SDK)/lib -L/usr/X11R6/lib -L%GLFW%/lib -lglfw
DEFINES = -DDEXPORT

ASSEMBLY = crates
BINDIR = ../bin/

TARGET = $(BINDIR)lib$(ASSEMBLY).so
SRC = $(shell find . -name '*.c')
OBJ = $(patsubst %.c,$(BINDIR)%.o,$(SRC))
BIN_OBJ = $(addprefix $(BINDIR),$(notdir $(OBJ)))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	echo "Building crates"
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o $@ $(BIN_OBJ) $(LFLAGS)

$(BINDIR)%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $(BINDIR)$(notdir $@)

clean: 
	rm -f $(BIN_OBJ) $(TARGET)

rebuild:
	make clean
	make

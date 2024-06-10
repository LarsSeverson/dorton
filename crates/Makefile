CC = clang
CFLAGS = -g -fPIC
INCLUDES = -I. -Iinternal -Idorton_utils -Ilogger -I$(VULKAN_SDK)/include -I$(GLFW)/include -I${cglm}/include
LFLAGS = -shared -lvulkan -lglfw -lcglm -lm -lxcb -lX11 -lX11-xcb -lxkbcommon -L/usr/X11R6/lib -L$(VULKAN_SDK)/lib -L${GLFW}/lib -L${cglm}/lib 
DEFINES = -DDEXPORT -D_DEBUG 

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

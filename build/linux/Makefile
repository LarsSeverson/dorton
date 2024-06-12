CC = clang
CFLAGS = -g -fPIC
INCLUDES = -I. -Iinternal -Idorton_utils -Ilogger -I$(VULKAN_SDK)/include -I$(GLFW)/include -I${cglm}/include 
LFLAGS = -shared $(LIBVULKAN) $(LIBGLFW) $(LIBCGLM) -lm -lxcb -lX11 -lX11-xcb -lxkbcommon -L/usr/X11R6/lib
DEFINES = -DDEXPORT -D_DEBUG 

ASSEMBLY = crates
BINDIR = ../bin/

LIBVULKAN = -L${VULKAN_SDK}/lib -Wl,-rpath,${VULKAN_SDK}/lib -lvulkan
LIBGLFW = -L${GLFW}/lib -Wl,-rpath,${GLFW}/lib -lglfw
LIBCGLM = -L$(cglm)/lib -Wl,-rpath,${cglm}/lib -lcglm

TARGET = $(BINDIR)lib$(ASSEMBLY).so
SRC = $(shell find . -name '*.c')
OBJ = $(SRC:%.c=$(BINDIR)%.o)

$(BINDIR)%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

$(TARGET): $(OBJ)
	echo "Building crates"
	$(CC) $(OBJ) $(CFLAGS) $(DEFINES) $(INCLUDES) $(LFLAGS) -o $@ 

.PHONY: all

all: $(TARGET)

clean: 
	rm -f $(OBJ) $(TARGET)

rebuild:
	make clean
	make

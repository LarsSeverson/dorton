CC = clang
CFLAGS = -shared -g -Wvarargs -Wall -Werror
INCLUDES = -I. -Iinternal -Idorton_utils -Ilogger -I$(VULKAN_SDK)\include -I$(GLFW)\include -I${cglm}\include 
LFLAGS = -luser32 $(LIBVULKAN) $(LIBGLFW) $(LIBCGLM) -lm -lshell32 -lgdi32
DEFINES = -DDEXPORT -D_DEBUG 

ASSEMBLY = crates
BINDIR = ../bin/

LIBVULKAN = -L${VULKAN_SDK}\lib -Wl,-rpath,${VULKAN_SDK}\lib -lvulkan-1
LIBGLFW = -L${GLFW}\lib -Wl,-rpath,${GLFW}\lib -lglfw3_mt -lopengl32 
LIBCGLM = -L$(cglm)\lib -Wl,-rpath,${cglm}\lib -lcglm

RWILDCARD = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call RWILDCARD, $d/,$2))

TARGET = $(BINDIR)lib$(ASSEMBLY).so
SRC = $(call RWILDCARD,$(ASSEMBLY)/,*.c)
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

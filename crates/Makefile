CC = clang
CFLAGS = -g -shared -fdeclspec -fPIC
INCLUDES = -Iinternal -I$(VULKAN_SDK)/include
LFLAGS = -lvulkan -lxcb -lX11 -lX11-xcb -lxkbcommon -L$(VULKAN_SDK)/lib -L/usr/X11R6/lib
DEFINES = -D_DEBUG -DDEXPORT

ASSEMBLY = crates

TARGET = ../bin/lib$(ASSEMBLY).so
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(INCLUDES) $(LFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean: 
	rm -f $(OBJ) $(TARGET)

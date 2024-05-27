CC = clang
CFLAGS = -g -fdeclspec -fPIC
INCLUDES = -I../crates/app/ -I../crates/internal/ -I$(VULKAN_SDK)/include
LFLAGS = -L../bin/ -lcrates
DEFINES = -D_DEBUG -DDIMPORT

ASSEMBLY = dorton 

TARGET = ../bin/$(ASSEMBLY)
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


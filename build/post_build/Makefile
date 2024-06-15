BIN_DIR = ../../bin
ASSETS_DIR = ../../assets

SHADERS_DIR = $(ASSETS_DIR)/shaders
SHADERS_COMP = $(VULKAN_SDK)/bin/glslc
SHADERS_OUT = $(BIN_DIR)/assets/shaders

SRC = $(SHADERS_DIR)/.something.vert.glsl
TARGET = $(SHADERS_OUT)/.something.vert.spv

$(TARGET): $(SRC)
	-echo "Compiling shaders."
	-mkdir -p $(SHADERS_DIR)
	-mkdir -p $(SHADERS_OUT)
	-$(SHADERS_COMP) -fshader-stage=vert $< -o $@ && echo "Shader compiled." \
	(echo "Error compiling shader. Error: $$?"; exit 1)

all: $(TARGET)

clean:
	-echo "Cleaning shaders."
	-rm -f $(TARGET)

.PHONY: all clean
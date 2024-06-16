TARGET_EXTENSION=

ifeq ($(OS),Windows_NT)
	TARGET_EXTENSION=windows
else
	TARGET_EXTENSION=linux
endif

.PHONY: all
all: shaders crates src

.PHONY: crates
crates:
	$(MAKE) -C build/crates -f Makefile.$(TARGET_EXTENSION)

.PHONY: src
src:
	$(MAKE) -C build/src -f Makefile.$(TARGET_EXTENSION)

.PHONY: shaders
shaders:
	$(MAKE) -C build/shaders -f Makefile.$(TARGET_EXTENSION)

.PHONY: clean
clean:
	$(MAKE) -C build/shaders clean -f Makefile.$(TARGET_EXTENSION)
	$(MAKE) -C build/crates clean -f Makefile.$(TARGET_EXTENSION)
	$(MAKE) -C build/src clean -f Makefile.$(TARGET_EXTENSION)

.PHONY: rebuild
rebuild:
	$(MAKE) -C build/shaders rebuild -f Makefile.$(TARGET_EXTENSION)
	$(MAKE) -C build/crates rebuild -f Makefile.$(TARGET_EXTENSION)
	$(MAKE) -C build/src rebuild -f Makefile.$(TARGET_EXTENSION)

.PHONY: run 
run: shaders crates
	$(MAKE) -C build/src run -f Makefile.$(TARGET_EXTENSION)

ifeq ($(TARGET_EXTENSION),linux)
.PHONY: vrun
vrun: shaders crates
	$(MAKE) -C build/src vrun -f Makefile.linux
endif
.PHONY: all crates src

all: 
	$(MAKE) build 

crates:
	$(MAKE) -C crates

src:
	$(MAKE) -C src

clean:
	$(MAKE) -C crates clean
	$(MAKE) -C src clean

build:
	$(MAKE) -C crates rebuild
	$(MAKE) -C src rebuild

run:
	$(MAKE) -C crates rebuild
	$(MAKE) -C src run

vrun:
	$(MAKE) -C crates rebuild
	$(MAKE) -C src vrun
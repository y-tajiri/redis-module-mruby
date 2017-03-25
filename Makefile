CFLAGS = -Ivendor/mruby/include

all: build libmruby.a mruby.o
	ld -o mruby.so mruby.o -bundle -undefined dynamic_lookup -lc vendor/mruby/build/host/lib/libmruby.a -lm

mruby.o:
	gcc -dynamic -fno-common -std=gnu99 -mmacosx-version-min=10.6 -c -Ivendor/mruby/include -o mruby.o mruby.c

libmruby.a: vendor/mruby
	cd vendor/mruby && ${MAKE}

vendor/mruby:
	mkdir -p vendor
	git clone https://github.com/mruby/mruby.git vendor/mruby
	cd vendor/mruby && git reset --hard && git clean -fdx

build:
	mkdir -p build

clean:
	rm -rf vendor
	rm -rf build

.PHONY: build

build:
	rm -rf build/lib.macosx-10.7-intel-2.7/libplasma.so
	python setup.py build
	llvm-gcc-4.2 -Wl,-F. -bundle -undefined dynamic_lookup -Wl,-F. -arch i386 build/temp.macosx-10.7-intel-2.7/plasma.o -o build/lib.macosx-10.7-intel-2.7/libplasma.so /opt/oblong/g-speak-32-2/lib/libPlasma.dylib
.PHONY: setup build all

all:	build

setup:
	ln -sf build/lib.macosx-10.7-intel-2.7/libplasma.so .
	lipo /usr/bin/python2.7 -extract i386 -output ./python2.7
	lipo /usr/bin/python -extract i386 -output ./python

build:
	$(RM) build/lib.macosx-10.7-intel-2.7/libplasma.so
	python setup.py build
	llvm-gcc-4.2 -Wl,-F. -bundle -undefined dynamic_lookup -Wl,-F. -arch i386 build/temp.macosx-10.7-intel-2.7/plasma.o -o build/lib.macosx-10.7-intel-2.7/libplasma.so /opt/oblong/g-speak-32-2/lib/libPlasma.dylib

clean:
	python setup.py clean
	$(RM) -r build
	$(RM) python python2.7
	$(RM) libplasma.so

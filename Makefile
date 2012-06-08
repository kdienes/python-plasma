.PHONY: setup build all

all:	build

setup:
	ln -sf build/lib.macosx-10.7-i386-2.7/libplasma.so .
	lipo /usr/bin/python2.7 -extract i386 -output ./python2.7
	lipo /usr/bin/python -extract i386 -output ./python

build:
	ARCHFLAGS="-arch i386" python setup.py build

clean:
	python setup.py clean
	$(RM) -r build
	$(RM) python python2.7
	$(RM) libplasma.so

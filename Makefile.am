.PHONY: build

all: build

build:
	rm -rf build
	rm -f *.so
	G_SPEAK_HOME=$(G_SPEAK_HOME) ARCHFLAGS="-arch i386" python setup.py build
	ln -s `find . -name '*.so'` libplasma.so

clean:
	G_SPEAK_HOME=$(G_SPEAK_HOME) python setup.py clean
	rm -rf build

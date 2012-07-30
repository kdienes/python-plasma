.PHONY: build

all: build

build:
	rm -rf build
	G_SPEAK_HOME=$(G_SPEAK_HOME) ARCHFLAGS="-arch i386" python setup.py build

clean:
	G_SPEAK_HOME=$(G_SPEAK_HOME) python setup.py clean
	rm -rf build

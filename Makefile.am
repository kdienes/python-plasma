.PHONY: build

all: build

build:
	rm -rf build
	ARCHFLAGS="-arch i386" python setup.py build

clean:
	python setup.py clean
	rm -rf build

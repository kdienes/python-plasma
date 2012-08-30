.PHONY: build

all: build

build:
	rm -rf build
	ARCHFLAGS="-arch i386" python setup.py build

install:
	rm -rf build
	python setup.py install --prefix $(prefix)

install-debian:
	rm -rf build
	python setup.py install --prefix $(prefix) --install-layout=deb

clean:
	python setup.py clean
	rm -rf build

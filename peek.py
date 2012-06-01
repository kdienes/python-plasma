import libplasma

hose = libplasma.hose ('tile-server');

while True:
    print hose.fetch (-1)

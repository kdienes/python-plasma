import libplasma

hose = libplasma.hose ('3dconnexion-events');
while True:
    print hose.fetch ()

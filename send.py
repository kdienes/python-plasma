import libplasma

hose = libplasma.hose ('3dconnexion-events');
print hose.deposit (['3DConnexionEvent'],
                    {'Buttons': 1L, 'Axes': [0L, 0L, 0L, 0L, 0L, 0L], 'Command': 2L}
                    )

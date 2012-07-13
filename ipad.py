import math
import time
import libplasma
import socket

class iPadConnection:
    def __init__ (self):
        self.hose = libplasma.hose ('map_pool')

    def set (self, x, y, scale):
        self.hose.deposit (['map-set-position'], { 'center' : [ x, y, 0.0 ], 'scale' : scale * 4000.0, 'source' : 'g-speak' })


class ViewClient:

    def __init__ (self):
        self.hose = libplasma.hose ('map-view')
        
    def wait (self):
        r = self.hose.fetch (-1)
        if not r:
            return r
        descrips, ingests = r
        if not 'current-view' in descrips:
            return
        return ingests

c = ViewClient ()
i = iPadConnection ()

while True:
    time.sleep (0.01)
    d = c.wait ()
    i.set (d['lng'], d['lat'], d['altitude'])

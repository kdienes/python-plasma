import time
import libplasma
import socket

class GoogleEarthConnection:

    def __init__ (self, host, port):
        self._s = socket.socket (socket.AF_INET, socket.SOCK_DGRAM, 0)
        self._s.connect ((host, port))

    def send_packet (self, counter = 0, lat = -33.54, long = 150.59, altitude = 50000, heading = 0.0, tilt = 0.0, roll = 0.0, start = 0, end = 0, planet = ''):
        s = '%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%s' % (counter, lat, long, altitude, heading, tilt, roll, start, end, planet)
        print s
        self._s.send (s)

    def close (self):
        self._s.close ()

class ViewClient:

    def __init__ (self):
        self.hose = libplasma.hose ('map-view');
        
    def wait (self):
        r = self.hose.fetch (-1)
        if not r:
            return r
        descrips, ingests = r
        if not 'current-view' in descrips:
            return
        return ingests

e = GoogleEarthConnection ('172.16.6.121', 49490)
c = ViewClient ()

i = time.time ()

lng = -71.091652
lat = 42.358609

e.send_packet (counter = i, lat = lat, long = lng, altitude = 20000.0)

raise ValueError

while True:
    d = c.wait ()
    i += 1
    e.send_packet (counter = i, lat = d['lat'], long = d['lng'], altitude = d['altitude'] * 2000, heading = d['rotation'] * 360 / (2 * 3.14), tilt = 0, roll = 0)

e.close ()

# counter, lat, lon, alt, heading, tilt, roll, time start, time end, planet_name ("sky", "mars", "moon", empty "" is Earth)

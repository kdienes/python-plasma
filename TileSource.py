import WebMercator

import math
import string

class TileSource:

    def __init__ (self, url, format, min, max):

        self._url = url
        self._format = format
        self._min = min
        self._max = max

    def __repr__ (self):
        
        return 'TileSource ("%s", "%s", %d, %d)' % (self._url, self._format, self._min, self._max)

    def select_lod (self, scale):

        EarthMajorAxis = 6378137.0
        flod = math.log (2 * math.pi * EarthMajorAxis / scale, 2) - math.log (256.0, 2);
        flod -= 1.0;
        flod += 1.6;
        if (flod < 0):
            flod = 0;
        return int (flod);

    def request_str (self, c):

        s = self._url
        s = string.replace (s, '%{lod}', str (c.lod))
        s = string.replace (s, '%{x}', str (c.x))
        s = string.replace (s, '%{y}', str (c.y))
        s = string.replace (s, '%{tilekey}', c.tilekey ())
        return s

    def TileIDForCoordinate (self, c, lod):
        c = c.at_lod (lod)
        return c

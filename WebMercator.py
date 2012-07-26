import math

class WebMercatorCoordinate:

    def __init__ (self, x, y, lod):
        self.x = x
        self.y = y
        self.lod = lod

    def __str__ (self):
        return "[%d %s @%d]" % (self.x, self.y, self.lod)

    def __repr__ (self):
        return "WebMercatorCoordinate (%d, %d, %d)" % (self.x, self.y, self.lod)

    def at_lod (self, nlod):
        if (nlod > self.lod):
	    return WebMercatorCoordinate (self.x << (nlod - self.lod), self.y << (nlod - self.lod), nlod)
        else:
            return WebMercatorCoordinate (self.x >> (self.lod - nlod), self.y >> (self.lod - nlod), nlod)

    def tilekey (self):
	lod, x, y = self.lod, self.x, self.y
        s = ''
        for n in range (lod, 0, -1):
            mask = 1 << (n - 1)
            if (y & mask):
                if (x & mask):
                    s = s + '3'
                else:
                    s = s + '2'
            else:
                if (x & mask):
                    s = s + '1'
                else:
                    s = s + '0'
        return s

    def geodetic (self):
        sx = self.x / float (1 << self.lod)
        sy = self.y / float (1 << self.lod)


def FromLatLng (lat, lng, lod = None):
    if (lod is None): lod = 31
    x = int ((lng + 180.0) / 360.0 * math.pow (2.0, lod));
    y = int ((1.0 - math.log (math.tan (lat * math.pi / 180.0) + 1.0 / math.cos (lat * math.pi / 180.0)) / math.pi) / 2.0 * math.pow (2.0, lod))
    return WebMercatorCoordinate (x, y, lod)


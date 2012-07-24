import WebMercator
import string

def spiral (N, M):
    x, y = 0, 0   
    dx, dy = 0, -1

    for dumb in xrange (N*M):
        if abs(x) == abs(y) and [dx,dy] != [1,0] or x>0 and y == 1-x:  
            dx, dy = -dy, dx            # corner, change direction

        if abs(x)>N/2 or abs(y)>M/2:    # non-square
            dx, dy = -dy, dx            # change direction
            x, y = -y+dx, x+dy          # jump

        yield x, y
        x, y = x+dx, y+dy

class TileSource:

    def __init__ (self, url, format, min, max):
        self._url = url
        self._format = format
        self._min = min
        self._max = max

    def request_str (self, c):
        s = self._url
        s = string.replace (s, '%{lod}', str (c.lod))
        s = string.replace (s, '%{x}', str (c.x))
        s = string.replace (s, '%{y}', str (c.y))
        s = string.replace (s, '%{tilekey}', c.tilekey ())
        return s

    def tiles_for (self, center, lod):

        base = center.at_lod (lod)
        c = base

        while True:
            yield c
            if c.lod == self._min:
                break
            c = c.at_lod (c.lod - 1)
            
        for x, y in spiral (100, 100):

            c = WebMercator.WebMercatorCoordinate (base.x + x, base.y + y, base.lod)
            c = c.at_lod (lod)

            while True:
                yield c
                if (c.x % 2 != 0) or (c.y % 2 != 0):
                    break
                c = c.at_lod (c.lod - 1)

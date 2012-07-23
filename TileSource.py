import string

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
        while True:
            yield center
            center.x += 1
            return

import WebMercator

class TileManager:
    
    def __init__ (self):
        pass

    def TileIDsFor (self, s, region, rlod):

        if s._min > s._max:
            return
        
        tiles = []

        maxlod = min (rlod, s._max)
        minlod = max (0, s._min)

        if (maxlod < minlod):
            maxlod = minlod

        for lod in range (maxlod, minlod - 1, -1):
            
            t1 = s.TileIDForCoordinate (region['ul'], lod)
            t2 = s.TileIDForCoordinate (region['ll'], lod)
            t3 = s.TileIDForCoordinate (region['ur'], lod)
            t4 = s.TileIDForCoordinate (region['lr'], lod)

            min_x = min (t1.x, t2.x, t3.x, t4.x)
            min_y = min (t1.y, t2.y, t3.y, t4.y)
            max_x = max (t1.x, t2.x, t3.x, t4.x)
            max_y = max (t1.y, t2.y, t3.y, t4.y)
            c_x = (min_x + max_x) / 2.0
            c_y = (min_y + max_y) / 2.0

            for x in range (min_x, max_x + 1, 1):
                for y in range (min_y, max_y + 1, 1):
                    
                    dsq = (x - c_x) * (x - c_x) + (y - c_y) * (y - c_y)
                    priority = 1000000 * (32 - lod) - dsq

                    tiles.append ((WebMercator.WebMercatorCoordinate (x, y, lod), priority))

        tiles.sort (key = lambda x: x[1])
        tiles.reverse ()
        for x in tiles:
            yield s, x[0]

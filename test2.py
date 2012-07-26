import WebMercator
import TileSource
import TileManager

bingSource = TileSource.TileSource ("http://ecn.t0.tiles.virtualearth.net/tiles/a%{tilekey}.jpeg?g=926&mkt=en-US&shading=hill&stl=H", "jpeg", 1, 22)

manager = TileManager.TileManager ()

ingests = {'center': (46.50847859433766, -73.72838764451444), 'altitude': 230.67484205852693, 'ur': (48.0843844312566, -71.29084168933332), 'ul': (47.73789411348229, -76.57039445824921), 'lr': (45.25061370884338, -70.88638074696064), 'rotation': -0.09760147240012884, 'll': (44.88552211524416, -76.16593351587653)}
region = {
    'll' : WebMercator.FromLatLng (ingests['ll'][0], ingests['ll'][1]),
    'ul' : WebMercator.FromLatLng (ingests['ul'][0], ingests['ul'][1]),
    'lr' : WebMercator.FromLatLng (ingests['lr'][0], ingests['lr'][1]),
    'ur' : WebMercator.FromLatLng (ingests['ur'][0], ingests['ur'][1]),
    'center' : WebMercator.FromLatLng (ingests['center'][0], ingests['center'][1])
    }

lod = bingSource.select_lod (ingests['altitude'])

for x in manager.TileIDsFor (bingSource, region, lod):
    print x

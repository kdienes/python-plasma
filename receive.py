import tornado.ioloop
import tornado.httpclient

import urllib2
import libplasma

import os
import os.path
import string
import math

import WebMercator
import TileSource
import TileManager

if False:
    faaSource = TileSource.TileSource ("http://services.arcgisonline.com/ArcGIS/rest/services/World_Topo_Map/MapServer/tile/%{lod}/%{y}/%{x}.jpeg", "jpeg", 0, 19)
    roadsSource = TileSource.TileSource ("http://a3.acetate.geoiq.com/tiles/acetate-roads/%{lod}/%{x}/%{y}.png", "png", 0, 23)
    esriSource = TileSource.TileSource ("http://services.arcgisonline.com/ArcGIS/rest/services/World_Topo_Map/MapServer/tile/%{lod}/%{y}/%{x}.jpeg", "jpeg", 0, 19)
    bingSource = TileSource.TileSource ("http://ecn.t0.tiles.virtualearth.net/tiles/a%{tilekey}.jpeg?g=926&mkt=en-US&shading=hill&stl=H", "jpeg", 1, 22)

    sources = [ faaSource, roadsSource, esriSource, bingSource ]

class NullIterator ():
    def __init__ (self):
        pass
    def next (self):
        raise StopIteration

class mapserver:

    def __init__ (self):

        self._center = None

        self.requests = {}
        self.npending = 0
        self.maxpending = 4
        self.loop = tornado.ioloop.IOLoop.instance ()
        self.client = tornado.httpclient.AsyncHTTPClient ()
        self.view = libplasma.hose ('tcp://localhost/nga-server')
        self.manager = TileManager.TileManager ()

        self._sourceList = []
        self._altitude = None
        self.update_iterator ()

        self.scheduler = tornado.ioloop.PeriodicCallback (self.handle_view, 20, io_loop = self.loop)
        self.scheduler.start ()

        self.rscheduler = tornado.ioloop.PeriodicCallback (self.make_requests, 10, io_loop = self.loop)
        self.rscheduler.start ()

    def check_filetype (self, r):

        if (r.request.filetype == 'jpeg'):
            return (r.body[0:2] == '\xff\xd8')
        elif (r.request.filetype == 'png'):
            return (r.body[0:8] == '\x89\x50\x4e\x47\x0d\x0a\x1a\x0a')
        else:
            raise ValueError, 'unknown filetype'

    def handle_request (self, r):

        if r.error:
            print "Error ",r.error
            response = None

        elif not self.check_filetype (r):
            response = None

        else:

            if not os.path.exists (os.path.dirname (r.request.cache)):
                os.makedirs (os.path.dirname (r.request.cache))
            f = file (r.request.cache, 'w')
            f.write (r.body)
            f.close ()

            response = (['tile-loaded'], r.request.id)
            
        del self.requests[r.request.url]
        self.npending -= 1

        if response:
            print response
            self.view.deposit (response[0], response[1])


    def cache_for (self, s, c):
    
        filepart = '%02d/%03d/%03d/%03d/%03d/%03d/%03d.%s' % \
        (c.lod,
         int (c.x / 1000000), int (c.x / 1000) % 1000, int (c.x) % 1000,
         int (c.y / 1000000), int (c.y / 1000) % 1000, int (c.y) % 1000,  
         s._format)

        cdir = s._url.replace ('/', '|')

        return '/Library/Caches/oblong/tile-cache/%s/%s' % (cdir, filepart)
        
    def make_requests (self):

        while True:

            if (len (self.requests) >= self.maxpending):
                return

            try:
                s, c = self._iter.next ()
            except StopIteration:
                return

            url = s.request_str (c)
            if (self.requests.has_key (url)):
                continue

            cache = self.cache_for (s, c)
            if os.path.isfile (cache):
                continue

            r = tornado.httpclient.HTTPRequest (url = url)
            r.id = [ s._id, c.lod, c.x, c.y ]
            r.url = url
            r.cache = cache
            r.filetype = s._format
            self.requests[r.url] = r
            self.client.fetch (r, self.handle_request)

    def update_iterator (self):

        if (len (self._sourceList) == 0) or (self._altitude is None):
            self._iter = NullIterator ()
            return

        s = self._sourceList[0]
        source = TileSource.TileSource (s[0], s[1], s[2], s[3], s[4])
        lod = source.select_lod (self._altitude)
        self._iter = self.manager.TileIDsFor (source, self._region, lod)

    def handle_view (self):

        nview = None
        nlayers = None

        while True:

            r = self.view.fetch (0)
            if not r:
                break
            descrips, ingests = r
            
            if descrips[0] == 'current-view':
                nview = ingests
            elif descrips[0] == 'current-layers':
                nlayers = ingests
            elif descrips[0] == 'tile-loaded':
                pass
            elif descrips[0] == 'synchronize':
                pass
            else:
                raise ValueError, r

        if nlayers is not None:
            self._sourceList = nlayers

        if nview is not None:
            self._altitude = nview['altitude']
            self._region = {
                'll' : WebMercator.FromLatLng (nview['ll'][0], nview['ll'][1]),
                'ul' : WebMercator.FromLatLng (nview['ul'][0], nview['ul'][1]),
                'lr' : WebMercator.FromLatLng (nview['lr'][0], nview['lr'][1]),
                'ur' : WebMercator.FromLatLng (nview['ur'][0], nview['ur'][1]),
                'center' : WebMercator.FromLatLng (nview['center'][0], nview['center'][1])
                }

        if nview or nlayers:
            self.update_iterator ()


server = mapserver ()
server.view.deposit (['synchronize'])
server.loop.start ()

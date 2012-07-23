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

esriSource = TileSource.TileSource ("http://services.arcgisonline.com/ArcGIS/rest/services/World_Topo_Map/MapServer/tile/%{lod}/%{y}/%{x}.jpeg", "jpeg", 0, 19)
bingSource = TileSource.TileSource ("http://ecn.t0.tiles.virtualearth.net/tiles/a%{tilekey}.jpeg?g=926&mkt=en-US&shading=hill&stl=H", "jpeg", 1, 22)

class mapserver:

    def __init__ (self):

        self._center = None

        self.requests = {}
        self.npending = 0
        self.maxpending = 4
        self.loop = tornado.ioloop.IOLoop.instance ()
        self.client = tornado.httpclient.AsyncHTTPClient ()
        self.hose = libplasma.hose ('tile-server')
        self.view = libplasma.hose ('map-view')

        self.scheduler = tornado.ioloop.PeriodicCallback (self.handle_view, 20, io_loop = self.loop)
        self.scheduler.start ()

        self.rscheduler = tornado.ioloop.PeriodicCallback (self.make_requests, 100, io_loop = self.loop)
        self.rscheduler.start ()

    def check_filetype (self, r):

        if (r.request.filetype == 'jpeg'):
            return (r.body[0:2] == '\xff\xd8')
        elif (r.request.filetype == 'png'):
            return (r.body[0:8] == '\x89\x50\x4e\x47\x0d\x0a\x1a\x0a')
        else:
            raise ValueError, 'unknown filetype'

    def handle_request (self, r):

        if r.error or not self.check_filetype (r):

            response = None

        else:
            try:
                os.mkdir (os.path.dirname (r.request.cache))
            except:
                pass
            f = file (r.request.cache, 'w')
            f.write (r.body)
            f.close ()

            response = (['tile-response'], { 'id' : r.request.id })
            
        del self.requests[r.request.url]
        self.npending -= 1

        if response:
            print response
            # self.hose.deposit (response[0], response[1])


    def cache_for (self, s, c):
        
        cdir = s._url.replace ('/', '|')
        return '../cache/%s/%d-%d-%d.%s' % (cdir, c.lod, c.x, c.y, s._format)
        
    def make_requests (self):

        if not self._center:
            return

        s = bingSource

        for c in s.tiles_for (self._center, self._lod):

            c = c.at_lod (self._lod)

            if (len (self.requests) >= self.maxpending):
                return

            url = s.request_str (c)
            if (self.requests.has_key (url)):
                continue

            cache = self.cache_for (s, c)
            if os.path.isfile (cache):
                continue

            r = tornado.httpclient.HTTPRequest (url = url)
            r.id = id
            r.url = url
            r.cache = cache
            r.filetype = s._format
            self.requests[r.url] = r
            self.client.fetch (r, self.handle_request)

            print r.url

    def select_lod (self, scale):

        EarthMajorAxis = 6378137.0
        flod = math.log (2 * math.pi * EarthMajorAxis / scale, 2) - math.log (256.0, 2);
        flod -= 1.0;
        flod += 1.6;
        if (flod < 0):
            flod = 0;
        return int (flod);

    def handle_view (self):
        while True:
            r = self.view.fetch (0)
            if not r:
                return
            descrips, ingests = r
            self._lod = self.select_lod (ingests['altitude'])
            self._center = WebMercator.FromLatLng (ingests['lat'], ingests['lng'])

server = mapserver ()
server.loop.start ()

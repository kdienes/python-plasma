import tornado.ioloop
import tornado.httpclient

import urllib2
import libplasma

import os
import os.path

class mapserver:

    def __init__ (self):

        self.requests = []
        self.npending = 0
        self.maxpending = 4
        self.loop = tornado.ioloop.IOLoop.instance ()
        self.client = tornado.httpclient.AsyncHTTPClient ()
        self.hose = libplasma.hose ('tile-server');

        self.scheduler = tornado.ioloop.PeriodicCallback (self.handle_pool, 100, io_loop = self.loop)
        self.scheduler.start ()

        self.rscheduler = tornado.ioloop.PeriodicCallback (self.make_requests, 100, io_loop = self.loop)
        self.rscheduler.start ()

    def handle_request (self, r):

        self.npending -= 1
        if r.error:
            print "Error:", r.error
        else:
            try:
                os.mkdir (os.path.dirname (r.request.cache))
            except:
                pass
            f = file (r.request.cache, 'w')
            f.write (r.body)
            f.close ()
            print [['tile-response'], { 'id' : r.request.id }]
            self.hose.deposit (['tile-response'], { 'id' : r.request.id })

    def handle_protein (self, p):

        descrips, ingests = p
        if not 'tile-request' in descrips:
            return

        print p
        r = tornado.httpclient.HTTPRequest (url = ingests['url'])
        r.cache = ingests['cache']
        r.id = ingests['id']
        self.requests.insert (0, r)

    def handle_pool (self):

        while True:
            r = self.hose.fetch (0)
            if not r:
                return
            self.handle_protein (r)

    def make_requests (self):

        while self.npending < self.maxpending:
            if (len (self.requests) == 0):
                return
            self.npending += 1
            r = self.requests.pop (0)
            self.client.fetch (r, self.handle_request)

server = mapserver ()
server.loop.start ()

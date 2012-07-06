import tornado.ioloop
import tornado.httpclient

import urllib2
import libplasma

import os
import os.path
import string

class mapserver:

    def __init__ (self):

        self.requests = {}
        self.npending = 0
        self.maxpending = 4
        self.loop = tornado.ioloop.IOLoop.instance ()
        self.client = tornado.httpclient.AsyncHTTPClient ()
        self.hose = libplasma.hose ('tile-server');

        self.scheduler = tornado.ioloop.PeriodicCallback (self.handle_pool, 100, io_loop = self.loop)
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
            self.hose.deposit (response[0], response[1])

    def tilekey (self, id):
        s = ''
        priority, lod, x, y = id
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

    def request_str (self, url, id):
        s = url
        s = string.replace (s, '%{lod}', str (int (id[1])))
        s = string.replace (s, '%{x}', str (int (id[2])))
        s = string.replace (s, '%{y}', str (int (id[3])))
        s = string.replace (s, '%{tilekey}', self.tilekey (id))
        return s

    def handle_protein (self, p):

        descrips, ingests = p
        if not 'tiles-request' in descrips:
            return

        for source in ingests:
            for id in source['tiles']:

                if (len (self.requests) >= self.maxpending):
                    return

                url = self.request_str (source['url'], id)
                if (self.requests.has_key (url)):
                    continue

                cdir = source['url'].replace ('/', '|')
                cache = '../cache/%s/%d-%d-%d.%s' % (cdir, id[1], id[2], id[3], source['filetype'])

                if os.path.isfile (cache):
                    continue

                r = tornado.httpclient.HTTPRequest (url = url)
                r.id = id
                r.url = url
                r.cache = cache
                r.filetype = source['filetype']
                self.requests[r.url] = r
                self.client.fetch (r, self.handle_request)

                print r.url

    def handle_pool (self):

        while True:
            r = self.hose.fetch (0)
            if not r:
                return
            self.handle_protein (r)

    def make_requests (self):
        pass

server = mapserver ()
server.loop.start ()

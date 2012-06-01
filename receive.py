import tornado.ioloop
import tornado.httpclient

import urllib2
import libplasma

loop = tornado.ioloop.IOLoop.instance ()
client = tornado.httpclient.AsyncHTTPClient ()
hose = libplasma.hose ('tile-server');

def handle_request (response):

    if response.error:
        print "Error:", response.error
    else:
        f = file (response.request.cache, 'w')
        f.write (response.body)
        f.close ()
        print [['tile-response'], { 'id' : response.request.id }]
        hose.deposit (['tile-response'], { 'id' : response.request.id })

def handle_protein (p):

    descrips, ingests = p
    if not 'tile-request' in descrips:
        return

    print p
    r = tornado.httpclient.HTTPRequest (url = ingests['url'])
    r.cache = ingests['cache']
    r.id = ingests['id']

    client.fetch (r, handle_request)

def handle_pool ():

    while True:
        r = hose.fetch (0)
        if not r:
            return
        handle_protein (r)

scheduler = tornado.ioloop.PeriodicCallback (handle_pool, 100, io_loop = loop)
scheduler.start ()
loop.start ()



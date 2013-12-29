#!/usr/bin/env python

import os
import os.path
import sys
import distutils.core

G_SPEAK_DIR = '/opt/oblong/g-speak3.11'

if sys.platform != "win32":
    compiler = [ '-Wall -Werror -fpermissive -Wno-strict-prototypes' ]

libplasma = distutils.core.Extension \
('libplasma',
 [ 'plasma.c' ],
 include_dirs = [ os.path.join (G_SPEAK_DIR, 'include') ],
 library_dirs = [ os.path.join (G_SPEAK_DIR, 'lib') ],
 runtime_library_dirs = [ os.path.join (G_SPEAK_DIR, 'lib') ],
 libraries = [ 'Plasma' ],
 extra_compile_args = [ '-Wall', '-g', '-O0', '-I../..']
 )

distutils.core.setup \
(name = 'plasma',
 version = '0.0.1',
 maintainer = 'Klee Dienes',
 maintainer_email = 'klee.dienes@hadronindustries.com',
 description = 'Utilities for interfacing to g-speak libPlasma.',
 packages = [ 'plasma' ],
 url = 'http://www.hadronindustries.com',
 ext_modules = [ libplasma ],
 )

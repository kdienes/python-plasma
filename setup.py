#!/usr/bin/env python

import os
import os.path
import sys
import distutils.core

sys.path.insert (0, os.path.sep.join (os.path.realpath (__file__).split (os.path.sep)[0:-2] + ['python-hadron']))
print sys.path

G_SPEAK_DIR = '/opt/oblong/g-speak3.9'

if sys.platform != "win32":
    compiler = [ '-Wall -Werror -fpermissive -Wno-strict-prototypes' ]

libplasma = distutils.core.Extension \
('libplasma',
 [ 'plasma.c' ],
 include_dirs = [ os.path.join (G_SPEAK_DIR, 'include') ],
 library_dirs = [ os.path.join (G_SPEAK_DIR, 'lib') ],
 runtime_library_dirs = [ os.path.join (G_SPEAK_DIR, 'lib') ],
 libraries = [ 'Plasma' ],
 extra_compile_args = [ '-Wall', '-g', '-O0']
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

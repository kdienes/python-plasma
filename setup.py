#!/usr/bin/env python

import os
import os.path
import sys
import distutils.core

if os.name == 'posix':
    CXX_libraries = [ 'stdc++', 'm' ]
else:
    CXX_libraries = []

if sys.platform != "win32":
    compiler = [ '-Wall -Werror -fpermissive -Wno-strict-prototypes' ]

G_SPEAK_HOME = os.getenv ('G_SPEAK_HOME')

plasma = distutils.core.Extension \
("libplasma",
 [ "plasma.c" ],
 include_dirs = [ os.path.join (G_SPEAK_HOME, 'include') ],
 library_dirs= [ os.path.join (G_SPEAK_HOME, 'lib') ],
 libraries = [ "Plasma" ],
 extra_compile_args = [ '-Wall', '-g', '-O0' ]
 )

distutils.core.setup \
(name = "libPlasma module",
 version = "0.1",
 maintainer = "Klee Dienes",
 maintainer_email = "klee.dienes@hadronindustries.com",
 description = "Utilities for interfacing to g-speak libPlasma.",
 packages = ['CXX'],
 package_dir = {'CXX': '.'},
 url = "http://www.oblong.com",
 ext_modules = [ plasma ],
 )

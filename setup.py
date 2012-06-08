#!/usr/bin/env python

import os
import sys
import distutils.core

if os.name == 'posix':
    CXX_libraries = [ 'stdc++', 'm' ]
else:
    CXX_libraries = []

if sys.platform != "win32":
    compiler = [ '-Wall -Werror -fpermissive -Wno-strict-prototypes' ]

support_dir = os.path.normpath 
(os.path.join
 (sys.prefix, 'share', 'python%d.%d' % (sys.version_info[0], sys.version_info[1]), 'CXX'))

plasma = distutils.core.Extension \
("libplasma",
 [ "plasma.c" ],
 include_dirs = [ '/opt/oblong/g-speak-32-2/include' ],
 library_dirs= [ '/opt/oblong/g-speak-32-2/lib' ],
 libraries = [ "Plasma" ],
 extra_compile_args = [ '-Wall', '-g', '-O0', '-arch', 'i386' ]
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

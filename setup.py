#!/usr/bin/env python

# Copyright (c) 2012-2014 Hadron Industries, Inc.

# This is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
   
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
   
# You should have received a copy of the GNU Lesser General Public
# License along with this program.  If not, see
# <http://www.gnu.org/licenses/>.

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

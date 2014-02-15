#!/usr/bin/env python

# Copyright (c) 2012-2014 Hadron Industries, Inc.

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

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

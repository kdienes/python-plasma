import libplasma

import os
import os.path

f = libplasma.slaw ([
("53493251.tif"),
("03475442.tif"),
("04518946.tif"),
("58884183.tif"),
("05797421.tif"),
("46532678.tif"),
("19051500.tif"),
("65117025.tif"),
("99905449.tif"),
("65638946.tif"),
("40202299.tif"),
("18464358.tif"),
("61407638.tif")
])

print f.to_str ()

import libplasma

import os
import os.path

icons = {
    "aircraft-civilian" : "Civilian Aircraft",
    "aircraft-military" : "Military Aircraft",
    "cattle" : "Cattle",
    "church-catholic" : "Catholic Church",
    "church-goc" : "Orthodox Church",
    "church-protestant" : "Protestant Church",
    "golfcourse" : "Golf Course",
    "holysite-christian" : "Christian Holy Site",
    "holysite-islam" : "Islamic Holy Site",
    "holysite-jewish" : "Jewish Holy Site",
    "moonbase" : "Moon Base",
    "mosque-islamic" : "Mosque",
    "parkingstructure" : "Parking Structure",
    "raceway-auto" : "Auto Raceway",
    "raceway-equestrian" : "Equestrian Raceway",
    "school-primary" : "Primary School",
    "school-university" : "University",
    "stadium-baseball" : "Baseball Stadium",
    "stadium-football" : "Football Stadium",
    "stadium-soccer" : "Soccer Stadium",
    "station-police" : "Police Station",
    "station-radio" : "Radio Station",
    "station-tv" : "TV Station",
    "temple-jewish" : "Jewish Temple",
    "vehicle-lighttruck" : "Light Truck",
    "vehicle-18wheeler" : "18-Wheeler",
    "vehicle-auto" : "Automobile",
    "vehicle-bus" : "Bus"
    }

sources = [
    { "53493251.tif" : ('aircraft-civilian', 'aircraft-military', 'cattle', 'church-catholic'),
      "03475442.tif" : ('church-goc', 'church-protestant', 'golfcourse', 'holysite-christian'),
      "04518946.tif" : ('holysite-islam', 'holysite-jewish', 'moonbase', 'mosque-islamic'),
      "58884183.tif" : ('parkingstructure', 'raceway-auto', 'raceway-equestrian', 'school-primary'),
      "05797421.tif" : ('school-university', 'stadium-baseball', 'stadium-football', 'stadium-soccer'),
      "46532678.tif" : ('station-police', 'station-radio', 'station-tv', 'temple-jewish'),
      "19051500.tif" : ('vehicle-lighttruck', 'vehicle-18wheeler', 'vehicle-auto' 'vehicle-bus'),
      "65117025.tif" : ('parkingstructure', 'raceway-auto', 'raceway-equestrian', 'school-primary'),
      "99905449.tif" : ('school-university', 'stadium-baseball', 'stadium-football', 'stadium-soccer'),
      "65638946.tif" : ('holysite-islam', 'holysite-jewish', 'moonbase', 'mosque-islamic'),
      "40202299.tif" : ('school-university', 'stadium-baseball', 'stadium-football', 'stadium-soccer'),
      "18464358.tif" : ('station-police', 'station-radio', 'station-tv', 'temple-jewish'),
      "61407638.tif" : ('vehicle-lighttruck', 'vehicle-18wheeler', 'vehicle-auto' 'vehicle-bus')
      }
    ]

f = libplasma.slaw ({ 'icons' : icons, 'sources' : sources })

print f.to_str ()

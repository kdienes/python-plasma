import libplasma

import os
import os.path

icons = {
    "aircraft-civilian" : "Civilian Aircraft",
    "aircraft-military" : "Military Aircraft",
    "boat" : "Boat"
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
    "vehicle-bus" : "Bus",
    }

sources = {
	  "53493251.tif" : ('aircraft-military' , 'aircraft-civilian' , 'boat' , 'vehicle-18wheeler' , 'vehicle-auto' , 'vehicle-bus' , 'vehicle-lighttruck'),
	  "03475442.tif" : ('school-university' , 'school-primary' , 'station-police'),
	  "04518946.tif" : ('vehicle-auto' , 'boat'),
	  "58884183.tif" : ('vehicle-18wheeler' , 'vehicle-bus'),
	  "05797421.tif" : ('golfcourse' , 'raceway-equestrian' , 'raceway-auto' , 'stadium-soccer' , 'stadium-baseball' , 'stadium-football' , 'station-tv' , 'station-radio'),
	  "46532678.tif" : ('golfcourse' , 'raceway-equestrian' , 'raceway-auto' , 'stadium-soccer' , 'stadium-baseball' , 'stadium-football'),
	  "19051500.tif" : ('golfcourse' , 'raceway-equestrian' , 'raceway-auto' , 'stadium-soccer' , 'stadium-baseball' , 'stadium-football'),
	  "65117025.tif" : ('golfcourse'),
	  "99905449.tif" : ('cattle'),
	  "65638946.tif" : ('boat'),
	  "40202299.tif" : ('vehicle-lighttruck'),
	  "18464358.tif" : ('vehicle-bus'),
	  "61407638.tif" : ('church-protestant' , 'church-catholic' , 'church-goc' , 'mosque-islamic' , 'temple-jewish' , 'parkingstructure')
	  "rome.tif" : ('vehicle-bus' , 'church-protestant' , 'church-goc' , 'church-catholic' , 'holysite-christian' , 'temple-jewish' , 'holysite-jewish' , 'mosque-islamic' ,'holysite-islam'),
	  }

f = libplasma.slaw ({ 'icons' : icons, 'sources' : sources })

print f.to_str ()

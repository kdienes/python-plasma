#include <Python.h>
#include <structmember.h>

#include <libPlasma/c/pool.h>
#include <libPlasma/c/slaw.h>
#include <libPlasma/c/protein.h>

#include "defs.h"

#include "slawconvert.c"
#include "slaw.c"
#include "hose.c"

static PyMethodDef PlasmaMethods[] =
  {
    { NULL, NULL, 0, NULL }
  };
 
PyMODINIT_FUNC initlibplasma (void)
{
  PyObject *m;

  HoseObjectType.tp_new = PyType_GenericNew;
  if (PyType_Ready (&HoseObjectType) < 0)
    return;

  SlawObjectType.tp_new = PyType_GenericNew;
  if (PyType_Ready (&SlawObjectType) < 0)
    return;

  m = Py_InitModule3 ("libplasma", PlasmaMethods, "libplasma doc");
  if (m == NULL)
    return;

  Py_INCREF (&HoseObjectType);
  Py_INCREF (&SlawObjectType);
  
  PyModule_AddObject (m, "hose", (PyObject *) &HoseObjectType);
  PyModule_AddObject (m, "slaw", (PyObject *) &SlawObjectType);
}

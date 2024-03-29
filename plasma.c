/* Copyright (c) 2012-2014 Hadron Industries, Inc. */

/* This is free software: you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.
   
   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with this program.  If not, see
   <http://www.gnu.org/licenses/>. */

#include <Python.h>
#include <structmember.h>

#include <libPlasma/c/pool.h>

#include <libPlasma/c/slaw.h>
#include <libPlasma/c/protein.h>

#include "defs.h"

#include "slawtopython.c"
#include "pythontoslaw.c"
// #include "slaw.c"
#include "hose.c"

static PyObject *PoolDispose (HoseObject *self, PyObject *args)
{
  char *name = NULL;
  if (! PyArg_ParseTuple (args, "s", &name)) {
    return NULL;
  }

  ob_retort oret = pool_dispose (name);
  PYTHON_OBCHECK (oret);

  Py_INCREF (Py_None);
  return Py_None;
}

static PyMethodDef PlasmaMethods[] =
  {
    { NULL, NULL, 0, NULL }
  };

static PyMethodDef PoolMethods[] =
  {
    { "dispose", (PyCFunction) PoolDispose, METH_VARARGS,
      "Destroy a pool utterly." },
    { NULL, NULL, 0, NULL }
  };
 
struct plasma_exception {
  ob_retort code;
  char *libname;
  char *name;
  PyObject *exception;
};

#define PLASMA_EXCEPTION_ENTRY(code){ code, "libplasma."#code, #code, NULL }

struct plasma_exception exceptions[] = { 
  PLASMA_EXCEPTION_ENTRY(POOL_NO_SUCH_POOL),
  PLASMA_EXCEPTION_ENTRY(POOL_NO_SUCH_PROTEIN),
  { 0, NULL, NULL, NULL }
};

PyMODINIT_FUNC initlibplasma (void)
{
  PyObject *m, *mpool;

  HoseObjectType.tp_new = PyType_GenericNew;
  if (PyType_Ready (&HoseObjectType) < 0)
    return;

  PoolGangObjectType.tp_new = PyType_GenericNew;
  if (PyType_Ready (&PoolGangObjectType) < 0)
    return;

#if 0
  SlawObjectType.tp_new = PyType_GenericNew;
  if (PyType_Ready (&SlawObjectType) < 0)
    return;
#endif

  m = Py_InitModule3 ("libplasma", PlasmaMethods, "libplasma doc");
  if (m == NULL)
    return;

  mpool = Py_InitModule3 ("libplasma.pool", PoolMethods, "libplasma pool doc");
  if (m == NULL)
    return;

  Py_INCREF (&HoseObjectType);
  Py_INCREF (&PoolGangObjectType);
  // Py_INCREF (&SlawObjectType);
  Py_INCREF (mpool);
  
  PyModule_AddObject (m, "hose", (PyObject *) &HoseObjectType);
  PyModule_AddObject (m, "gang", (PyObject *) &PoolGangObjectType);
  // PyModule_AddObject (m, "slaw", (PyObject *) &SlawObjectType);
  PyModule_AddObject (m, "pool", (PyObject *) mpool);

  struct plasma_exception *e = exceptions;
  while (e->code != 0) {
    e->exception = PyErr_NewException (e->libname, PyExc_RuntimeError, NULL);
    Py_INCREF (e->exception);
    PyModule_AddObject (m, e->name, e->exception);
    e++;
  }
}

void PlasmaError (ob_retort oret)
{
  struct plasma_exception *e = exceptions;
  while (e->code != 0) {
    if (e->code == oret) {
      PyErr_SetString (e->exception, ob_error_string (oret));
      break;
    }
    e++;
  }
  if (e->code == 0) {
    PyErr_SetString (PyExc_RuntimeError, ob_error_string (oret));
  }
}

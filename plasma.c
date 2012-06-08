#include <Python.h>
#include <structmember.h>

#include <libPlasma/c/pool.h>
#include <libPlasma/c/slaw.h>
#include <libPlasma/c/protein.h>

#define PYTHON_OBCHECK(oret) \
  if (oret != OB_OK) { \
    PyErr_SetString (PyExc_RuntimeError, ob_error_string (oret)); \
    return NULL; \
  }

static void UNKNOWN_TYPE (bslaw s)
{
  fprintf (stderr, "unknown type\n");
  slaw_spew_overview (s, stderr, "");
  fprintf (stderr, "\n");
  fflush (stderr);
  abort ();
}

static PyObject *SlawToPython (bslaw s)
{
  PyObject *ret = NULL;

  if (slaw_is_map (s)) {

    int64 count = slaw_map_count (s);
    int64 i = 0;
    ret = PyDict_New ();

    for (i = 0; i < count; i++) {
      bslaw x = slaw_list_emit_nth (s, i);
      assert (slaw_is_cons (x));
      PyObject *k = SlawToPython (slaw_cons_emit_car (x));
      PyObject *v = SlawToPython (slaw_cons_emit_cdr (x));
      PyDict_SetItem (ret, k, v);
    }

  } else if (slaw_is_protein (s)) {

    ret = PyList_New (2);
    assert (slaw_is_nil (slaw_cons_emit_car (s)));
    PyList_SET_ITEM (ret, 0, SlawToPython (protein_descrips (s)));
    PyList_SET_ITEM (ret, 1, SlawToPython (protein_ingests (s)));

  } else if (slaw_is_list (s)) {

    int64 count = slaw_list_count (s);
    int64 i = 0;
    assert (count < 1024);
    ret = PyList_New ((int) count);

    for (i = 0; i < count; i++) {
      PyList_SET_ITEM (ret, i, SlawToPython (slaw_list_emit_nth (s, i)));
    }

  } else if (slaw_is_cons (s)) {

    int64 count = slaw_list_count (s);
    assert (count < 1024);
    ret = PyTuple_New ((int) count);

    UNKNOWN_TYPE (s);

  } else if (slaw_is_string (s)) {

    ret = PyString_FromString (slaw_string_emit (s));

  } else if (slaw_is_numeric (s)) {

    if (slaw_is_int32 (s)) {
      int32 v = *(slaw_int32_emit (s));
      ret = PyLong_FromLong (v);
    } else if (slaw_is_unt32 (s)) {
      unt32 v = *(slaw_unt32_emit (s));
      ret = PyLong_FromUnsignedLong (v);
    } else if (slaw_is_int16 (s)) {
      int16 v = *(slaw_int16_emit (s));
      ret = PyLong_FromLong (v);
    } else if (slaw_is_unt16 (s)) {
      unt16 v = *(slaw_unt16_emit (s));
      ret = PyLong_FromUnsignedLong (v);
    }  else {
      UNKNOWN_TYPE (s);
    }
 
  } else if (slaw_is_boolean (s)) {
    
    ret = PyBool_FromLong (* (slaw_boolean_emit (s)));
 
  } else if (slaw_is_nil (s)) {

    ret = Py_None;
 
  } else {

    printf ("*** UNKNOWN TYPE\n");
    slaw_spew_overview (s, stdout, "");
    printf ("\n");
    printf ("*** END UNKNOWN TYPE\n");
    fflush (stdout);
    ret = Py_None;
  }

  Py_INCREF (ret);
  return ret;
}

typedef struct {
  PyObject_HEAD
  pool_hose hose;
} HoseObject;

static int HoseInit (HoseObject *self, PyObject *args, PyObject *keywords)
{
  pool_hose h;
  ob_retort ret;
  
  PyObject *poolid;
  static char *kwlist[] = { "pool", NULL };

  if (! PyArg_ParseTupleAndKeywords (args, keywords, "O", kwlist, &poolid))
    return -1;

  char *poolname = PyString_AsString (poolid);

  ret = pool_participate (poolname, &h, NULL);
  if (ret != OB_OK) {
    PyErr_SetString (PyExc_RuntimeError, ob_error_string (ret));
    return -1;
  }

  self->hose = h;
  return 0;
}

static PyObject *HoseFetch (HoseObject *self, PyObject *args)
{
  pool_timestamp stamp;
  int64 index;
  protein p;
  long timeout;

  if (! PyArg_ParseTuple (args, "l", &timeout)) {
    return NULL;
  }

  for (;;) {
    ob_retort ret;
    ret = pool_await_next (self->hose, timeout, &p, &stamp, &index);
    if (ret == POOL_AWAIT_TIMEDOUT) {
      Py_INCREF (Py_None);
      return Py_None;
    }
    return SlawToPython (p);
  }
}

static PyObject *PythonToSlaw (PyObject *p, slaw *pslaw)
{
  PyObject *pret = NULL;
  slaw s;

  if (PyLong_Check (p)) {

    s = slaw_int32 (PyLong_AsLong (p));

  } else if (PyString_Check (p)) {

    s = slaw_string (PyString_AsString (p));

  } else if (PyDict_Check (p)) {

    PyObject *key, *value;
    Py_ssize_t pos = 0;
    
    slabu *su = slabu_new ();

    for (;;) {
      int ret = PyDict_Next (p, &pos, &key, &value);
      if (ret == 0) { break; }
      
      slaw car, cdr;

      pret = PythonToSlaw (key, &car);
      if (pret == NULL) { return pret; }

      pret = PythonToSlaw (value, &cdr);
      if (pret == NULL) { return pret; }

      int64 rr = slabu_list_add (su, slaw_cons (car, cdr));
      if (rr < 0) { PYTHON_OBCHECK (rr) };
    }
    
    s = slaw_map (su);


  } else if (PyTuple_Check (p)) {

    slabu *su = slabu_new ();
    int i = 0;

    for (i = 0; i < PyTuple_Size (p); i++) {

      PyObject *pp = PyTuple_GetItem (p, i);
      slaw ns;

      pret = PythonToSlaw (pp, &ns);
      if (pret == NULL) { return pret; }

      int64 rr = slabu_list_add (su, ns);
      if (rr < 0) { PYTHON_OBCHECK (rr) };
    }
    
    s = slaw_list (su);

  } else if (PyList_Check (p)) {

    slabu *su = slabu_new ();
    int i = 0;

    for (i = 0; i < PyList_Size (p); i++) {

      PyObject *pp = PyList_GetItem (p, i);
      slaw ns;

      pret = PythonToSlaw (pp, &ns);
      if (pret == NULL) { return pret; }

      int64 rr = slabu_list_add (su, ns);
      if (rr < 0) { PYTHON_OBCHECK (rr) };
    }
    
    s = slaw_list (su);

  } else {
    PYTHON_OBCHECK (OB_INVALID_ARGUMENT);
  }
  
  *pslaw = s;
  return Py_None;
}

static PyObject *HoseDeposit (HoseObject *self, PyObject *args)
{
  ob_retort oret;
  protein p;
  int64 idx;
  pool_timestamp timestamp;
  PyObject *ret = NULL;
  PyObject *descrips = NULL;
  PyObject *ingests = NULL;

  if (! PyArg_ParseTuple (args, "OO", &descrips, &ingests)) {
    return NULL;
  }
  
  slaw dslaw, islaw;
  ret = PythonToSlaw (descrips, &dslaw);
  if (ret == NULL) { return ret; }
  ret = PythonToSlaw (ingests, &islaw);
  if (ret == NULL) { return ret; }

  p = protein_from (dslaw, islaw);
  
  oret = pool_deposit_ex (self->hose, p, &idx, &timestamp);
  PYTHON_OBCHECK (oret);

  ret = PyTuple_New (2);
  PyTuple_SET_ITEM (ret, 0, PyLong_FromLongLong (idx));
  PyTuple_SET_ITEM (ret, 1, PyFloat_FromDouble (timestamp));

  Py_INCREF (ret);
  return ret;
}

static PyMemberDef HoseMembers[] = {
  { NULL }
};

static PyMethodDef HoseMethods[] = {
  { "fetch", (PyCFunction) HoseFetch, METH_VARARGS,
    "Fetch a protein from the pool." },
  { "deposit", (PyCFunction) HoseDeposit, METH_VARARGS,
    "Fetch a protein from the pool." },
  { NULL }
};

static char HoseDoc[] = "Hose documentation.";

static PyTypeObject HoseObjectType = {
  	PyObject_HEAD_INIT (NULL)
	0,				/* ob_size           */
	"libplasma.hose",		/* tp_name           */
	sizeof (HoseObject),		/* tp_basicsize      */
	0,				/* tp_itemsize       */
	0,				/* tp_dealloc        */
	0,				/* tp_print          */
	0,				/* tp_getattr        */
	0,				/* tp_setattr        */
	0,				/* tp_compare        */
	0,				/* tp_repr           */
	0,				/* tp_as_number      */
	0,				/* tp_as_sequence    */
	0,				/* tp_as_mapping     */
	0,				/* tp_hash           */
	0,				/* tp_call           */
	0,				/* tp_str            */
	0,				/* tp_getattro       */
	0,				/* tp_setattro       */
	0,				/* tp_as_buffer      */
	Py_TPFLAGS_DEFAULT,		/* tp_flags          */
	HoseDoc,			/* tp_doc            */
	0,				/* tp_traverse       */
	0,				/* tp_clear          */
	0,				/* tp_richcompare    */
	0,				/* tp_weaklistoffset */
	0,				/* tp_iter           */
	0,				/* tp_iternext       */
	HoseMethods,	     		/* tp_methods        */
	HoseMembers,			/* tp_members        */
	0,				/* tp_getset         */
	0,				/* tp_base           */
	0,				/* tp_dict           */
	0,				/* tp_descr_get      */
	0,				/* tp_descr_set      */
	0,				/* tp_dictoffset     */
	(initproc) HoseInit,		/* tp_init           */
};

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

  m = Py_InitModule3 ("libplasma", PlasmaMethods, "libplasma doc");
  if (m == NULL)
    return;

  Py_INCREF (&HoseObjectType);
  PyModule_AddObject (m, "hose", (PyObject *) &HoseObjectType);
}

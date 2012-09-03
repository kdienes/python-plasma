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
    PYTHON_OBCHECK (ret);

    PyObject *pret = SlawToPython (p);
    if (pret == NULL) { return pret; }

    PyObject *hret = PyTuple_New (3);
    PyTuple_SET_ITEM (hret, 0, pret);
    PyTuple_SET_ITEM (hret, 1, PyFloat_FromDouble (stamp));
    PyTuple_SET_ITEM (hret, 2, PyLong_FromLong (index));
    return hret;
  }
}

static PyObject *HoseRunout (HoseObject *self, PyObject *args)
{
  if (! PyArg_ParseTuple (args, "")) {
    return NULL;
  }

  ob_retort ret;
  ret = pool_runout (self->hose);
  PYTHON_OBCHECK (ret);
  
  Py_INCREF (Py_None);
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
  { "runout", (PyCFunction) HoseRunout, METH_VARARGS,
    "Set the pool hose's index to that following the last available protein." },
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

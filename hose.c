typedef struct {
  PyObject_HEAD
  pool_hose hose;
} HoseObject;

static int HoseInit (HoseObject *self, PyObject *args, PyObject *keywords)
{
  pool_hose h;
  ob_retort ret;
  
  char *poolname = NULL;
  char *pooltype = "mmap";
  int creatingly = 0;
  PyObject *options = NULL;
  slaw optslaw = slaw_nil ();
  static char *kwlist[] = { "pool", "type", "create", "options", NULL };

  if (! PyArg_ParseTupleAndKeywords (args, keywords, "s|siO", kwlist, &poolname, &pooltype, &creatingly, &options))
    return -1;

  if (options != NULL) {
    PyObject *ret = PythonToSlaw (options, &optslaw);
    if (ret == NULL) {
      return -1;
    }
  } else {
    optslaw = slaw_map_empty ();
  }

  if (creatingly) {
    ret = pool_participate_creatingly (poolname, pooltype, &h, optslaw);
  } else {
    ret = pool_participate (poolname, &h, optslaw);
  }

  slaw_free (optslaw);

  if (! ((ret == OB_OK) || (creatingly && (ret == POOL_CREATED)))) {
    PlasmaError (ret);
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
    protein_free (p);

    if (pret == NULL) { return pret; }

    PyObject *hret = PyTuple_New (3);
    PyTuple_SET_ITEM (hret, 0, pret);
    PyTuple_SET_ITEM (hret, 1, PyLong_FromLong (index));
    PyTuple_SET_ITEM (hret, 2, PyFloat_FromDouble (stamp));
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

static PyObject *HoseToLast (HoseObject *self, PyObject *args)
{
  if (! PyArg_ParseTuple (args, "")) {
    return NULL;
  }

  ob_retort ret;
  ret = pool_tolast (self->hose);
  PYTHON_OBCHECK (ret);
  
  Py_INCREF (Py_None);
  return Py_None;
}

static PyObject *HoseIndex (HoseObject *self, PyObject *args)
{
  if (! PyArg_ParseTuple (args, "")) {
    return NULL;
  }

  ob_retort ret;
  int64 index;
  ret = pool_index (self->hose, &index);
  PYTHON_OBCHECK (ret);
  
  return PyLong_FromLong (index);
}

static PyObject *HoseNewestIndex (HoseObject *self, PyObject *args)
{
  if (! PyArg_ParseTuple (args, "")) {
    return NULL;
  }

  ob_retort ret;
  int64 index;
  ret = pool_newest_index (self->hose, &index);
  PYTHON_OBCHECK (ret);
  
  return PyLong_FromLong (index);
}

static PyObject *HoseSeekTo (HoseObject *self, PyObject *args)
{
  int64 index;

  if (! PyArg_ParseTuple (args, "l", &index)) {
    return NULL;
  }

  ob_retort ret;
  ret = pool_seekto (self->hose, index);
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
  else { Py_DecRef (ret); }
  ret = PythonToSlaw (ingests, &islaw);
  if (ret == NULL) { slaw_free (dslaw); return ret; }
  else { Py_DecRef (ret); }

  p = protein_from_ff (dslaw, islaw);
  
  oret = pool_deposit_ex (self->hose, p, &idx, &timestamp);
  protein_free (p);
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
  { "tolast", (PyCFunction) HoseToLast, METH_VARARGS,
    "Set the pool hose's index to the last available protein." },
  { "index", (PyCFunction) HoseIndex, METH_VARARGS,
    "Determines the current position (index) of the specified pool hose." },
  { "newestIndex", (PyCFunction) HoseNewestIndex, METH_VARARGS,
    "Get the index of the newest protein in this pool.  Returns  POOL_NO_SUCH_PROTEIN if no proteins are in the pool." },
  { "seekTo", (PyCFunction) HoseSeekTo, METH_VARARGS,
    "Set the pool hose's index to the given value." },
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

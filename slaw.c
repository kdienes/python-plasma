#include <libPlasma/c/slaw-io.h>

typedef struct {
  PyObject_HEAD
  slaw s;
} SlawObject;

static int SlawInit (SlawObject *self, PyObject *args, PyObject *keywords)
{
  PyObject *obj = NULL;
  PyObject *pret = NULL;

  if (! PyArg_ParseTuple (args, "O", &obj)) {
    return -1;
  }
  
  pret = PythonToSlaw (obj, &self->s);
  if (pret == NULL) { return -1; }

  return 0;
}

static PyObject *SlawToPythonFunc (SlawObject *self)
{
  return SlawToPython (self->s);
}

static PyObject *SlawToStringFunc (SlawObject *self)
{
  ob_retort oret;
  slaw s;

  oret = slaw_to_string (self->s, &s);
  PYTHON_OBCHECK (oret);

  return SlawToPython (s);
}

static PyMemberDef SlawMembers[] = {
  { NULL }
};

static PyMethodDef SlawMethods[] = {
  { "to_python", (PyCFunction) SlawToPythonFunc, METH_NOARGS,
    "Convert to a raw python object." },
  { "to_str", (PyCFunction) SlawToStringFunc, METH_NOARGS,
    "Convert to a string." },
  { NULL }
};

static char SlawDoc[] = "Slaw documentation.";

static PyTypeObject SlawObjectType = {
  	PyObject_HEAD_INIT (NULL)
	0,				/* ob_size           */
	"libplasma.slaw",		/* tp_name           */
	sizeof (SlawObject),		/* tp_basicsize      */
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
	SlawDoc,			/* tp_doc            */
	0,				/* tp_traverse       */
	0,				/* tp_clear          */
	0,				/* tp_richcompare    */
	0,				/* tp_weaklistoffset */
	0,				/* tp_iter           */
	0,				/* tp_iternext       */
	SlawMethods,	     		/* tp_methods        */
	SlawMembers,			/* tp_members        */
	0,				/* tp_getset         */
	0,				/* tp_base           */
	0,				/* tp_dict           */
	0,				/* tp_descr_get      */
	0,				/* tp_descr_set      */
	0,				/* tp_dictoffset     */
	(initproc) SlawInit,		/* tp_init           */
};

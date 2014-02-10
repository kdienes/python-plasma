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

static PyObject *PythonToSlaw (PyObject *p, slaw *pslaw)
{
  PyObject *pret = NULL;
  slaw s;

  if (p == Py_None) {
    
    s = slaw_nil ();

  } else if (PyInt_Check (p)) {

    s = slaw_int32 (PyLong_AsLong (p));

  } else if (PyLong_Check (p)) {

    s = slaw_int32 (PyInt_AsLong (p));

  } else if (PyFloat_Check (p)) {

    s = slaw_float64 (PyFloat_AsDouble (p));

  } else if (PyString_Check (p)) {

    s = slaw_string (PyString_AsString (p));

  } else if (PyUnicode_Check (p)) {

    PyObject *np = PyUnicode_AsUTF8String (p);
    if (np == NULL) { return np; }

    s = slaw_string (PyString_AsString (np));
    Py_DecRef (np);

  } else if (PyDict_Check (p)) {

    PyObject *key, *value;
    Py_ssize_t pos = 0;
    
    slabu *su = slabu_new ();

    for (;;) {
      int ret = PyDict_Next (p, &pos, &key, &value);
      if (ret == 0) { break; }
      
      slaw car, cdr;

      pret = PythonToSlaw (key, &car);
      if (pret == NULL) { slabu_free (su); return pret; }
      else { Py_DecRef (pret); }

      pret = PythonToSlaw (value, &cdr);
      if (pret == NULL) { slabu_free (su); return pret; }
      else { Py_DecRef (pret); }

      int64 rr = slabu_list_add_x (su, slaw_cons_ff (car, cdr));
      if (rr < 0) { slabu_free (su); PYTHON_OBCHECK (rr) };
    }
    
    s = slaw_map_f (su);


  } else if (PyTuple_Check (p)) {

    slabu *su = slabu_new ();
    int i = 0;

    for (i = 0; i < PyTuple_Size (p); i++) {

      PyObject *pp;
      slaw ns;

      pp = PyTuple_GetItem (p, i);
      /* We just got the size of the list, above. */
      assert (pp != NULL);

      pret = PythonToSlaw (pp, &ns);
      if (pret == NULL) { slabu_free (su); return pret; }
      else { Py_DecRef (pret); }

      int64 rr = slabu_list_add_x (su, ns);
      if (rr < 0) { slabu_free (su); PYTHON_OBCHECK (rr) };
    }
    
    s = slaw_list_f (su);

  } else if (PyList_Check (p)) {

    slabu *su = slabu_new ();
    int i = 0;

    for (i = 0; i < PyList_Size (p); i++) {

      PyObject *pp;
      slaw ns;

      pp = PyList_GetItem (p, i);
      /* We just got the size of the list, above. */
      assert (pp != NULL);

      pret = PythonToSlaw (pp, &ns);
      if (pret == NULL) { slabu_free (su); return pret; }
      else { Py_DecRef (pret); }

      int64 rr = slabu_list_add_x (su, ns);
      if (rr < 0) { slabu_free (su); PYTHON_OBCHECK (rr) };
    }
    
    s = slaw_list_f (su);

  } else {
    PYTHON_OBCHECK (OB_INVALID_ARGUMENT);
  }
  
  *pslaw = s;
  Py_IncRef (Py_None);
  return Py_None;
}

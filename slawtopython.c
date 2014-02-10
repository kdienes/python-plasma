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

static PyObject *SlawToPython (bslaw s)
{
  PyObject *ret = NULL;

  if (s == NULL) {

    ret = Py_None;
    
  } else if (slaw_is_map (s)) {

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

    ret = PyTuple_New (2);
    PyTuple_SET_ITEM (ret, 0, SlawToPython (slaw_cons_emit_car (s)));
    PyTuple_SET_ITEM (ret, 1, SlawToPython (slaw_cons_emit_cdr (s)));

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
    } else if (slaw_is_float32 (s)) {
      float32 v = *(slaw_float32_emit (s));
      ret = PyFloat_FromDouble (v);
    } else if (slaw_is_float64 (s)) {
      float64 v = *(slaw_float64_emit (s));
      ret = PyFloat_FromDouble (v);
    }  else {
      UNKNOWN_TYPE (s);
    }
 
  } else if (slaw_is_boolean (s)) {
    
    ret = PyBool_FromLong (* (slaw_boolean_emit (s)));
 
  } else if (slaw_is_nil (s)) {

    ret = Py_None;
    Py_INCREF (ret);
 
  } else {

    /* This should raise a SystemError instead of spamming the console
       and aborting.  This is nontrivial though as we'd then have to
       check for errors in all the returns from SlawToPython, which in
       turn would make refcounting challenging.  Maybe easier to
       switch to something like cython? */
    /* https://redmine.hadronindustries.com/redmine/issues/153 */

    UNKNOWN_TYPE (s);
  }

  return ret;
}

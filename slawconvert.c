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

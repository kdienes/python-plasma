void PlasmaError (ob_retort oret);

#define PYTHON_OBCHECK(oret) \
  if (oret != OB_OK) { \
    PlasmaError (oret); \
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

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

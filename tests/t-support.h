/* t-support.h - Helper routines for regression tests.
   Copyright (C) 2011 g10 Code GmbH

   This file is part of NPTH.
 
   NPTH is free software; you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2.1 of
   the License, or (at your option) any later version.
   
   NPTH is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <locale.h>

#include <npth.h>

#ifndef DIM
#define DIM(v)		     (sizeof(v)/sizeof((v)[0]))
#endif

#define fail_if_err(err)					\
  do								\
    {								\
      if (err)							\
        {							\
          fprintf (stderr, "%s:%d: %s\n",			\
                   __FILE__, __LINE__, strerror(err));		\
          exit (1);						\
        }							\
    }								\
  while (0)

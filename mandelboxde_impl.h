/*
   This file is part of the Mandelbox program developed for the course
    CS/SE  Distributed Computer Systems taught by N. Nedialkov in the
    Winter of 2015-2016 at McMaster University.

    Copyright (C) 2015-2016 T. Gwosdz and N. Nedialkov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "color.h"
#include "mandelbox.h"

#ifdef _OPENACC
#include <accelmath.h>
#include <cmath>
#else
#include <math.h>
#endif

#define SQR(x) ((x)*(x))

#define COMPONENT_FOLD(x) { (x) = fabs(x) <= 1? (x) : copysign(2,(x))-(x); }

//double MandelBoxDE(const vec3 &p0, const MandelBoxParams &params, double c1, double c2)
#define MANDEL_BOX_DE(d, p0, params, c1, c2) \
{ \
  vec3 p = p0; \
  double rMin2   = SQR(params.rMin); \
  double rFixed2 = SQR(params.rFixed); \
  double escape  = SQR(params.escape_time); \
  double dfactor = 1; \
  double r2      =-1; \
  const double rFixed2rMin2 = rFixed2/rMin2; \
  int i = 0; \
  while (i< params.num_iter && r2 < escape) { \
    COMPONENT_FOLD(p.x) \
    COMPONENT_FOLD(p.y) \
    COMPONENT_FOLD(p.z) \
    DOT(r2, p, p); \
    if (r2<rMin2) { \
      VEC_SCL(p,p,rFixed2rMin2); \
      dfactor *= (rFixed2rMin2); \
    } else if( r2<rFixed2 ) { \
      const double t = (rFixed2/r2); \
      VEC_SCL(p,p,t); \
      dfactor *= t; \
    } \
    dfactor = dfactor*fabs(params.scale)+1.0; \
    VEC_SCL(p,p,params.scale); \
    VEC_ADD(p,p,p0); \
    i++; \
  } \
  double m; \
  MAGNITUDE(m, p); \
  d = (m - c1) / dfactor - c2; \
}



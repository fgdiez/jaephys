/******************************************************************************
 *
 * Jaephys - Just An Extendable PHYsics Simulator
 *
 * Copyright (C) 2001  Federico G. Diez <fedegdiez@hotmail.com>
 *   
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *****************************************************************************/


#ifndef JAEPHYS_CONST_ENUM_TYPEDEF_H
#define JAEPHYS_CONST_ENUM_TYPEDEF_H

#include <stdlib.h>
#include <ostream.h>
#include <assert.h>

#include <string.h>

namespace jaephys
{
  typedef double Matrix3x3[3][3];
  typedef double Matrix3x1[3];

  typedef void Function(void);

  // for hash_map and other containers
  struct equal_char{
    bool operator()(const char* s1, const char* s2)
    {
   		return strcmp(s1,s2) == 0;
    }
  };


  const double ERROR_MARGIN = 1e-4;
  const double PI   = 3.142592654;
  const double PI_2 = 1.570796327;
  const double PI_4 = 0.785398163;
  const int RGB_MAX= 255;
  const double INC_CAMARA_POS=10;
  const double INC_CAMARA_TURN=0.04;

  enum GraphicMode 
  { GM640x480x256
  , GM640x480x64K
  , GM640x480x16M
  , GM800x600x256
  , GM800x600x64K
  , GM800x600x16M
  };

  enum NumberOfColors
  { COLOR_256
  , COLOR_64K
  , COLOR_16M
  };
  enum RelativePosition
  { EXTERNAL
  , THIS_CONTAINED
  , THIS_CONTAINER
  };
  
  enum CollitionKind
  { SPHERICAL
  , POLYGONAL
  };

  enum Logical
  { YES
  , NO
  , AMBIGUOUS
  };

  inline ostream & operator<<(ostream & os, RelativePosition pr)
  {
    switch(pr)
    {
      case EXTERNAL: os << "EXTERNAL"; break;
      case THIS_CONTAINED: os << "THIS_CONTAINED"; break;
      case THIS_CONTAINER: os << "THIS_CONTAINER"; break;
      default: assert(0);
    } 
    return os;
  }
} // namespace jaephys

#endif // JAEPHYS_CONST_ENUM_TYPEDEF_H

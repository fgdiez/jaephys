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

#ifndef JAEPHYS_AUXILIARY_HPP
#define JAEPHYS_AUXILIARY_HPP

#include <string>

class ostream;

namespace jaephys
{
  const double ERROR_MARGIN = 1e-4;
  const double PI   = 3.142592654;
  const double PI_DIV_2 = 1.570796327;
  const double PI_DIV_4 = 0.785398163;
  const int RGB_MAX= 255;
  const double CAMERA_TRANSLATION_INCR=10;
  const double CAMERA_ROTATION_INCR=0.04;
  const double pixelsPerMeter= 100;

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
  
  enum Logical
  { NO
  , YES
  , AMBIGUOUS
  };

  typedef double Matrix3x3[3][3];
  typedef double Matrix3x1[3];

  typedef void Function(void);

  // structure with comparation function
  struct equal_char{
    bool operator()( const char* s1, const char* s2 );
	};

	void str2Double( const std::string & orig, double & dest );

  template <class T>
  inline void interchange(T & a, T & b)
  {
    T aux=a;
    a= b;
    b= aux;
  }

  std::ostream & operator<<( std::ostream & os, RelativePosition rp );

} // namespace jaephys

#endif // JAEPHYS_AUXILIARY_HPP

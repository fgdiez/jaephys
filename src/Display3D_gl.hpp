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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *****************************************************************************/

#ifndef JAEPHYS_DISPLAY3D_GL_HPP
#define JAEPHYS_DISPLAY3D_GL_HPP

#include "Display3D.hpp"

namespace jaephys
{
  class Display3D_gl : virtual public Display3D
  {
  public:
    Display3D_gl( unsigned int width, unsigned int height);
    virtual ~Display3D_gl();

    void perspective( double visonAngle, double proximity, double depth );

    void drawLine( const Coord3D & p0, const Coord3D& p1 ) const;

    void drawTexturedPolygon( Polygon & polig) const;
    void useTexture(unsigned int id);
    void disableTexture();
    unsigned int iniTexture(char *textureFile);

    void drawFlatColorPolygon( Polygon & polig ) const;
    void drawWiredPolygon( Polygon & polig) const;
    void useColor(unsigned int color);
    void useColor(int r, int g, int b);

    void clean() const;
    void flush() const;

  protected:
    static void rescale(int ancho, int alto);
    // atributos
    static double visionAngle_;
    static double depth_;
    static double focalDistance_;

  private:
    // not implemented
		//
    Display3D_gl();
    Display3D_gl(const Display3D_gl &);             
    Display3D_gl & operator=(const Display3D_gl &); 
      
  }; // class Display3D_gl

} // namespace jaephys

#endif // JAEPHYS_DISPLAY3D_GL_HPP

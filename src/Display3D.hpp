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

#ifndef JAEPHYS_DISPLAY3D_HPP
#define JAEPHYS_DISPLAY3D_HPP

#include "Coord3D.hpp"

namespace jaephys
{
	class Polygon;

  enum EnumDisplay3D
  { DISPLAY3D_USE2D
  , DISPLAY3D_GL
  };
  enum EnumAxis
  { AXIS_X
  , AXIS_Y
  , AXIS_Z
  };
  
  class Display3D
  {
  public:
      
    Display3D();
    virtual ~Display3D();

		static Display3D & create( EnumDisplay3D type, 
															 int width, int height );
		static Display3D & getCurrent();

    virtual void perspective( double visionAngle, 
															double proximity, double depth )=0;

    virtual void drawLine( const Coord3D & p1, const Coord3D & p2) const=0;

    virtual void useTexture(unsigned int ){};
    virtual void disableTexture(){};
    virtual unsigned int iniTexture(char *){return 0;};

    virtual void drawWiredPolygon( Polygon & polyg) const=0;
    virtual void drawFlatColorPolygon( Polygon & polyg) const=0;
    virtual void drawTexturedPolygon( Polygon & polyg) const=0;
    virtual void useColor(unsigned int color)= 0;
    virtual void useColor(int r, int g, int b)= 0;

    virtual void clean() const = 0;
    virtual void flush() const =0;

    const Coord3D & getCentre() const;
    int width() const{ return width_;}
    int height() const{ return height_;}
    
    void rotateView( EnumAxis axis, double angle);
    void translateView( EnumAxis axis, double increment);
    void flushViewMatrix();
    
    Coord3D viewPosition_;
    Coord3D viewXdir_;
    Coord3D viewYdir_;
    Coord3D viewZdir_;
  protected:
    void applyView( const Coord3D & p, Coord3D& pTransformed ) const;
    void rotate2Axes( double angle, Coord3D & v1, Coord3D& v2 );

		static Display3D* current_;
    Coord3D centre_; 
    Matrix3x3 mView_;
    int width_;
    int height_;

  private:  
    // not implemented
    //
    Display3D(const Display3D &);
    Display3D & operator=(const Display3D &);
      
  };
}

#endif // JAEPHYS_DISPLAY3D_HPP

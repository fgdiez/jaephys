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


#ifndef JAEPHYS_SHAPE_HPP
#define JAEPHYS_SHAPE_HPP

#include "Coord3D.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace jaephys
{
	class Shape
	{
	public:
	  Shape();
    virtual ~Shape();
    
	  virtual void translate( const Coord3D & vector)=0;
	  virtual void rotate( const Coord3D & w )=0;

	  const Coord3D & centre() const { return centre_;}
    const unsigned int & id() const { return id_;}
    
	protected:
	  Coord3D centre_;
	  unsigned int id_;
    static unsigned int idCounter_;
    	  
	private:
	  Shape(const Shape &);
	  Shape & operator==( const Shape &);
	};
}

#endif // JAEPHYS_SHAPE_HPP

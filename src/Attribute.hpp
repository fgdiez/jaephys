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


#ifndef JAEPHYS_ATTRIBUTE_HPP
#define JAEPHYS_ATTRIBUTE_HPP

#include "debugging.h"

#include <string>
#include <vector>
#include <typeinfo>

namespace jaephys
{
  /////////////////////////////////////////////////////////////////////////////
  class Attribute
  {
  public:
    Attribute(const char* concept): concept_( concept ){}
    virtual ~Attribute(){}

    const std::string & concept()
    { return concept_;}

  protected:
    std::string concept_;
  };


  /////////////////////////////////////////////////////////////////////////////
	// Template for the generation of concrete Attribute classes
  template <class T>
  class TAttribute : virtual public Attribute
  {
  public:
    TAttribute( T * attrib, const char* concept)
			: Attribute( concept ),
				attrib_( attrib )
		{ }

    T* attrib_;
        
  private:
    TAttribute();
    TAttribute( const TAttribute&);
    TAttribute& operator=(const TAttribute&);
  };

  /////////////////////////////////////////////////////////////////////////////
	// Template for the generation of concrete Attribute classes
  template <class Base, class T>
  class BTAttribute : virtual public Attribute
  {
  public:
    BTAttribute( const T & attrib, const char* concept)
			: Attribute( concept )
    {
      attrib_= new T(attrib);
    }

    Base* attrib_;
        
  private:
    BTAttribute();
    BTAttribute( const BTAttribute&);
    BTAttribute& operator=(const BTAttribute&);
  };

} // jaephys

#endif // JAEPHYS_ATTRIBUTE_HPP

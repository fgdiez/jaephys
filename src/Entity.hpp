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


#ifndef JAEPHYS_ENTITY_HPP
#define JAEPHYS_ENTITY_HPP

#include "Algorithm.hpp"
#include "Attribute.hpp"
#include "debugging.h"

#include <string>
#include <vector>

namespace jaephys
{
  class Entity
  {
  public:
		Entity();
    Entity( const std::vector<Attribute* >& vAttrib, 
						Algorithm* visualization, 
						AnimationAlgorithm* animation );

    ~Entity();
    
		void addAttribute( Attribute* attrib );
		void setAppearence( Algorithm* appearence );
		void setAnimation( AnimationAlgorithm* animation );

    void visualize() const;
    void animate();
    void animationFactor(const double factor);    

		template <class T>
		T* findAttribute( const char* concept);

		template <class Base, class T>
		T* findAttribute( const char* concept);

  protected:

    std::vector<Attribute* > vAttrib_;
    Algorithm* appearence_;
    AnimationAlgorithm* animation_;    

  private:
    Entity(const Entity &);
    Entity & operator=(const Entity &);
  };

  /////////////////////////////////////////////////////////////////////////////
  // inline
  /////////////////////////////////////////////////////////////////////////////
  template <class T> inline 
  T* Entity::findAttribute( const char* concept)
  {
    Attribute* attr;
    for(unsigned int i=0; i< vAttrib_.size(); ++i)
    {
      attr = vAttrib_[i];
      if(attr->concept() == concept)
      {
        TAttribute<T>* target = dynamic_cast< TAttribute<T>* >(attr);

        if( !target)
        {
          TRACEC( JAEPHYS_ENT, 1, "Type="<< typeid(target->attrib_).name()<< 
                                  " not compatible with concept=" << concept );
          return 0;
        }
        return target->attrib_;
      }
    }
    TRACEC(JAEPHYS_ENT, 1, "Concept not found " << concept);
    return 0;
  }

		template <class Base, class T> inline
		T* Entity::findAttribute( const char* concept)
    {
      Base* base = findAttribute<Base>(concept);
      T* target= 0;
      
      if(base != 0){
        target = dynamic_cast< T* >(base);
        TRACEC(JAEPHYS_ENT,1,"Unable to dynamic_cast "<< typeid(base).name()<<
                             " to "<<typeid(target).name() );
      }
      return target;
    }
}

#endif //JAEPHYS_ENTITY_HPP


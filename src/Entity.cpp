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


#include "Entity.hpp"
#include <iostream>

using namespace jaephys;

Entity::Entity() 
	: appearence_(0), 
		animation_(0)
{
}

Entity::Entity( const std::vector<Attribute* > & vAttrib, 
								Algorithm*                  appearence, 
								AnimationAlgorithm*         animation )

	: vAttrib_(vAttrib), 
		appearence_(appearence), 
		animation_(animation)
{
  TRACEC(JAEPHYS_ENT,1,"enter");
}

Entity::~Entity()
{
  TRACEC(JAEPHYS_ENT,1,"enter");
  GUARANTEE(appearence_ != 0, "algoritmo inexistentity");
  delete appearence_;

  GUARANTEE(animation_ != 0, "algoritmo inexistentity");
  delete animation_;

  for(unsigned int i=0; i< vAttrib_.size(); ++i)
    delete vAttrib_[i]; 
}

void Entity::addAttribute( Attribute* attrib )
{
	vAttrib_.push_back(attrib);
}

void Entity::setAppearence( Algorithm* appearence )
{
	appearence_= appearence;
}

void Entity::setAnimation( AnimationAlgorithm* animation )
{
	animation_= animation;
}
    
void Entity::visualize() const
{
  TRACEC(JAEPHYS_ENT,2,"enter");
  GUARANTEE(appearence_ != 0, "algoritmo inexistentity");
  appearence_->run();
}

void Entity::animate()
{
  TRACEC(JAEPHYS_ENT,2,"enter");
  animation_->run();
}

void Entity::animationFactor(const double factor)
{
  TRACEC(JAEPHYS_ENT,2,"enter");
  animation_->setFactor(factor);
}    

Attribute* Entity::getAttribute( const char* concept )
{
  Attribute* attr;
  
  for( unsigned int i=0; i< vAttrib_.size(); ++i )
  {
    attr = vAttrib_[i];

    if( attr->concept() == concept )
    {
      return attr;
    }
  }
  TRACEC(JAEPHYS_ENT, 1, "Concept not found " << concept);

  return 0;
}

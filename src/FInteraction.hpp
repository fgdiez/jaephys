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

#ifndef JAEPHYS_FINTERACTION_HPP
#define JAEPHYS_FINTERACTION_HPP

#include "auxiliary.hpp"

#include <map>
#include <vector>

namespace jaephys
{
	class ParseTree;
	class Interaction;
	class Entity;

	typedef std::map<std::string, Entity*> Map_idEntity;

  class FInteraction
  {
   public:
     void createElement( ParseTree      & tree, 
												 const Map_idEntity   & mapEntity, 
												 std::vector<Interaction*> & vInter );

   protected:
		void createCollisions( ParseTree      & tree, 
													 std::vector<Interaction*> & vInter );

		void createGravitations( ParseTree            & tree, 
														 std::vector<Interaction*> & vInter );

		void getEntities( const std::string & ids, 
											std::vector<Entity *> & vEntity );

		void createCollisionCombinations( std::vector<Entity*> & vEntity, 
																			std::vector<Interaction*> & vInter,
																			std::string & type,
																			RelativePosition relPos,
																			unsigned int index=0 );

		void createCollisionCombinations( Entity* entity, 
																			std::vector<Entity*>& vEntity, 
																			std::vector<Interaction*>& vInter, 
																			std::string & type, 
																			RelativePosition relPos );
		void createGravitationCombinacions( std::vector<Entity*>      & vEntity,
																				std::vector<Interaction*> & vInter,
																				unsigned int           index=0 );

		void createCollisionInteraction( const std::string & type, 
																		 Entity & entity1, Entity & entity2, 
																		 const RelativePosition relativePos1, 
																		 Interaction* & interaction );
		void createGravitationInteraction( Entity & entity1, 
																			 Entity & entity2, 
																			 Interaction* & interaction );

		Map_idEntity mapIdEntity_;

  };
}

#endif //JAEPHYS_FINTERACTION_HPP

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

#ifndef JAEPHYS_DEMIURGE_HPP
#define JAEPHYS_DEMIURGE_HPP

#include "auxiliary.hpp"

#include <map>
#include <vector>

namespace jaephys
{
	class Entity;
	class Interaction;
  //class Display2D;
  class Display3D;

  class Demiurge
  {
  public:
    //////////////////////////////////////////////////////////////////////////
    // Simulation configuration
    // 
    static void setSimulation( const std::vector<Entity* >      & vEntity, 
															 const std::vector<Interaction* > & vInter );
    static void setSimulation (const char* filename);

    //////////////////////////////////////////////////////////////////////////
    // System animation
    // 
    static void beginSimulation();
    static void endSimulation();
    static void pauseSimulation();
    static void resumeSimulation();

  protected:

    static std::vector<Entity* > vEntity_;
    static std::vector<Interaction* > vInter_;

    static bool pause_;  // simulation pause control
    static int fps_;     // frames per second 
    static int cpf_;     // clock tics per frame period
    static int ipf_;     // iterations per frame period over the sequence of
                         // animation and interaction algorithms
    static int ips_;     // iterations per second over the sequence of animaion
                         // and interaction algorithms

		// system animation
    static void animateWorld();
    static void animateEntities();
    static void evaluateInteractions();
    static void visualizeEntities();
    static void setAnimationFactor( const double factor );
    static bool displayUsesGLUT();

		// user interaction
    static void defineDefaultKeys();

  }; // Demiurge

} // namespace jaephys

#endif // JAEPHYS_DEMIURGE_HPP

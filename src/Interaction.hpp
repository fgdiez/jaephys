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

#ifndef JAEPHYS_INTERACTION_HPP
#define JAEPHYS_INTERACTION_HPP

#include "Algorithm.hpp"

#include <vector>

namespace jaephys
{
  class Interaction
  {
  public:
    Interaction( const std::vector<Algorithm* > & vAlgor);
    ~Interaction();

    void evaluate();
    
  protected:
    std::vector<Algorithm* > vAlgor_;

  private:
    Interaction();
    Interaction(const Interaction & );
    Interaction & operator=(const Interaction &);

  }; // Interaction

} // jaephys

#endif // JAEPHYS_INTERACTION_HPP

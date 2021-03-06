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


#ifndef JAEPHYS_ALGORITHM_HPP
#define JAEPHYS_ALGORITHM_HPP

namespace jaephys
{
  class Algorithm
  {
	public:
    Algorithm();
    virtual ~Algorithm();
    
    virtual bool run()=0;

  private:
    Algorithm( const Algorithm &);
    Algorithm & operator=(const Algorithm &);
  };

  class AnimationAlgorithm
	{
  public:
    AnimationAlgorithm();
    virtual ~AnimationAlgorithm();
    
    virtual bool run()=0;
    virtual void setFactor(const double factor)=0;
    
  private:
    AnimationAlgorithm( const AnimationAlgorithm &);
    AnimationAlgorithm & operator=(const AnimationAlgorithm &);
  };

}

#endif //JAEPHYS_ALGORITHM_HPP

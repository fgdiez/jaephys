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

#ifndef JAEPHYS_KEYBOARD_HPP
#define JAEPHYS_KEYBOARD_HPP

#include "auxiliary.hpp"
#include <map>

namespace jaephys
{
  ////////////////////////////////////////////////////////////////////////////
  class Keyboard
  {
  public:
    static void registerCallback(int key, Function*);
    static void doCallback(unsigned char tecla, int x=0, int y=0);
    static void getKey();
    static unsigned char key_;
    
  protected:
   static std::map<int, Function*> map_;
  };

  void exit_callback();
  void pause_callback();

  void rotate_callback();
  void translate_callback();
}

#endif //JAEPHYS_KEYBOARD_HPP

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

#include "Keyboard.hpp"
#include "Demiurge.hpp"
#include "Display3D.hpp"
#include "debugging.h"
//#include <vga.h>

using namespace jaephys;

std::map<int, Function*> Keyboard::map_;
unsigned char Keyboard::key_='\0';

//////////////////////////////////////////////////////////////////////////////
void Keyboard::registerCallback(int key, Function* callback)
{
  map_[key] = callback;
}

//////////////////////////////////////////////////////////////////////////////
void Keyboard::getKey()
{
  //int key = vga_getkey();
  int key = 'k';

  if(key != 0)
  {
    Keyboard::doCallback(key);
  }
}

//////////////////////////////////////////////////////////////////////////////
void Keyboard::doCallback(unsigned char key, int , int )
{
  key_=key;
  Function* callback =0;
  callback = map_[key];
  if(callback != 0)
    (*callback)();
}

///////////////////////////////////////////////////////////////////////////////
// CallBacks
///////////////////////////////////////////////////////////////////////////////
void jaephys::pause_callback()
{
  static bool pause=false;

  if(pause)
  {
    Demiurge::resumeSimulation();
  }
  else
  {
    Demiurge::pauseSimulation();
  }
  pause = !pause;
}

//////////////////////////////////////////////////////////////////////////////
void jaephys::exit_callback()
{ 
  Demiurge::endSimulation();
}

//////////////////////////////////////////////////////////////////////////////
void jaephys::translate_callback()
{
  Display3D& display3D = Display3D::getCurrent();

  switch(Keyboard::key_){
  case 'a':
    display3D.translateView(AXIS_X, -CAMERA_TRANSLATION_INCR);
    break;
  case 'd':
    display3D.translateView(AXIS_X, CAMERA_TRANSLATION_INCR);
    break;
  case 's':
    display3D.translateView(AXIS_Y, -CAMERA_TRANSLATION_INCR);
    break;
  case 'w':
    display3D.translateView(AXIS_Y, CAMERA_TRANSLATION_INCR);
    break;
  case 'q':
    display3D.translateView(AXIS_Z, -CAMERA_TRANSLATION_INCR);
    break;
  case 'e':
    display3D.translateView(AXIS_Z, CAMERA_TRANSLATION_INCR);
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
void jaephys::rotate_callback()
{
  Display3D & display3D = Display3D::getCurrent();

  switch(Keyboard::key_){
  case 't':
    display3D.rotateView(AXIS_X,CAMERA_ROTATION_INCR);
    break;
  case 'g':
    display3D.rotateView(AXIS_X,-CAMERA_ROTATION_INCR);
    break;
  case 'h':
    display3D.rotateView(AXIS_Y,-CAMERA_ROTATION_INCR);
    break;
  case 'f':
    display3D.rotateView(AXIS_Y, CAMERA_ROTATION_INCR);
    break;
  case 'r':
    display3D.rotateView(AXIS_Z, CAMERA_ROTATION_INCR);
    break;
  case 'y':
    display3D.rotateView(AXIS_Z, -CAMERA_ROTATION_INCR);
    break;
  default:
    GUARANTEE(0, "Key not defined");
  }
  display3D.flushViewMatrix();
}

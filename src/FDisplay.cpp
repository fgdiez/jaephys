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

#include "FDisplay.hpp"

#include "ParseTree.hpp"
#include "Demiurge.hpp"

//#include "Display2D.h"
#include "Display3D.hpp"

#include "debugging.h"

using namespace jaephys;

void FDisplay::createElement(ParseTree& tree)
{
  if(tree.value_ == "3D")
  {
    createDisplay3D(tree);
  }
	/*
  else if(tree.value_ == "2D")
  {
    createDisplay2D(tree);
  }
	*/
	else{
		GUARANTEE(false, "Type of Display="<< tree.value_<<" unexpected");
	}
}

void FDisplay::createDisplay3D(ParseTree& tree)
{
  EnumDisplay3D type=DISPLAY3D_GL;
  int height=480, width=640;
  
  tree.iniIteration();
  ParseTree* child= tree.nextChild();
  while(child != 0)
  {
    if(child->token_ == "type")
    {
	    if(child->value_ == "GL")
	    {
	      type = DISPLAY3D_GL;
	    }
			/*
	    else if(child->value_ == "uses2D")
	    {
	      type = DISPLAY3D_USES2D;
	    }
			*/
	    else
	    {
	      GUARANTEE(false,"Display3D type=" << child->value_ << " unexpected");
	    }
	  }
	  else if(child->token_ == "width")
	  {
	    std::stringstream ss;
	    ss << child->value_;
	    ss >> width;
	  }
	  else if(child->token_ == "height")
	  {
	    std::stringstream ss;
	    ss << child->value_;
	    ss >> height;
	  }
	  else
	  {
	    GUARANTEE(false,"token=" << child->token_ << " unexpected");
	  }
	  child = tree.nextChild();
  }
  switch(type)
  {
	case DISPLAY3D_GL:{
     Display3D& display= Display3D::create(type, width, height);
     display.translateView(AXIS_Z, 200);
     break;
	}
		/*
			case DISPLAY3D_USA2D:
			Demiurgo::createDisplay3D_usa2D();
			break;
		*/
	default:
		GUARANTEE(false,"Type no contemplado");
  }
  Display3D::getCurrent().perspective(40,10,10000);
}

/*
void FDisplay::createDisplay2D(ParseTree& tree)
{
  EnumDisplay2D type=V2D_GL;
  int height=480, width=640;
  
  tree.iniIteration();
  ParseTree* child= tree.nextChild();
  while(child != 0)
  {
    if(child->token_ == "type")
    {
	    if(child->value_ == "svga")
	    {
	      type = V2D_SVGA;
	    }
	    else if(child->value_ == "gl")
	    {
	      type = V2D_GL;
	    }
	    else
	    {
	      GUARANTEE(false,"Type de display=" << child->value_ << " incorrector");
	    }
	  }
	  else if(child->token_ == "width")
	  {
	    strstream ss;
	    ss << child->value_;
	    ss >> width;
	  }
	  else if(child->token_ == "height")
	  {
	    strstream ss;
	    ss << child->value_;
	    ss >> height;
	  }
	  else
	  {
	    GUARANTEE(false,"token=" << child->token_ << " no previsto");
	  }
	  child = tree.nextChild();
  }
  switch(type)
  {
   case V2D_GL:
     Demiurgo::createDisplay2D_x(type,width,height);
     break;
   case V2D_SVGA:
   {
     ModoGrafico modo;
     bool modoValido= Display2D::daModo(width,height,16, modo);
     GUARANTEE(modoValido,"Modo grafico invalido");
     Demiurgo::createDisplay2D_consola(type,modo);
     break;
   }
   default:
     GUARANTEE(false,"Type de display incorrecto");
  }
}
*/

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

#include "Demiurge.hpp"

#include "Entity.hpp"
#include "Interaction.hpp"
#include "FDisplay.hpp"
#include "FEntity.hpp"
#include "FInteraction.hpp"
#include "Display3D_gl.hpp"
#include "Keyboard.hpp"
#include "Parser.hpp"
#include "ParseTree.hpp"

#include "debugging.h"
//#include "cronos.h"

#include <GL/glut.h>

using namespace jaephys;

unsigned int JAEPHYS_FPS
 = (getenv("JAEPHYS_FPS") != NULL) ? atoi(getenv("JAEPHYS_FPS")) : 40;

unsigned int JAEPHYS_IPF
 = (getenv("JAEPHYS_IPF") != NULL) ? atoi(getenv("JAEPHYS_IPF")) :  3;

std::vector<Entity* > Demiurge::vEntity_;
std::vector<Interaction* > Demiurge::vInter_;
bool Demiurge::pause_= false;
int Demiurge::fps_; // frames per second
int Demiurge::ipf_; // iterations per fram
int Demiurge::ips_; // iterations per second
int Demiurge::cpf_; // tics per frame

///////////////////////////////////////////////////////////////////////////////
void Demiurge::setSimulation
 ( const std::vector<Entity* > & vEntity
 , const std::vector<Interaction* >& vInter
 )
{
  vEntity_ = vEntity;
  vInter_ = vInter;
}

///////////////////////////////////////////////////////////////////////////////
void Demiurge::setSimulation( const char* filename )
{
  Parser parser;
  ParseTree* tree= 0;
  parser.createParseTree(filename, tree);
  GUARANTEE(tree !=0,"No se ha creado el tree parse");
  tree->print();
  
  tree->iniIteration();
  ParseTree* p= tree->nextChild();
  GUARANTEE(p !=0,"Tree badly build");
  GUARANTEE(p->token_ == "scene\\", "lacks scene token");

  FEntity fEntity;         //factory of entities
  FInteraction fInter; // factory of interactions
  std::map<std::string, Entity*>  mapEntity;

  p->iniIteration();
  for( ParseTree* child= p->nextChild()
     ; child != 0
     ; child= p->nextChild()
     )
  {
    if(child->token_ == "entity\\")
    {
      Entity* entity=0;
      std::string idEntity= child->value_;
      fEntity.createElement(*child,entity);
      vEntity_.push_back(entity);
      mapEntity[idEntity.c_str()] = entity;
    }
    else if(child->token_ == "interaction\\")
    {
      fInter.createElement(*child, mapEntity, vInter_);
    }
    else if(child->token_ == "display\\")
    {
      FDisplay displayFactory;
      displayFactory.createElement(*child);
    }
    else if(child->token_ == "!\\")
    {
      // ho hacemos nada: es un comentario
    }
    else
    {
      GUARANTEE(false, "token=" << child->token_ << " unexpected");
    }
  }
  GUARANTEE(vEntity_.size() > 0, "No entity defined");
}

///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION: activates the simulation
// PRECONDITIONS: 
//  - vEntity_ must not be empty.
//  - display3D_ must be initialized
// 
void Demiurge::beginSimulation()
{
  //CRONO_INI;
  //CRONO_FUNC_INI;

  GUARANTEE(vEntity_.size() > 0, "No hay entitys en el mundo");

  defineDefaultKeys();

  ipf_ = JAEPHYS_IPF;
  fps_ = JAEPHYS_FPS;
  ips_ = ipf_*fps_;
  cpf_ = static_cast<int> ( static_cast<double>(CLOCKS_PER_SEC) / fps_ );
  
  setAnimationFactor( static_cast<double>(1) / ips_ );
  
  if( displayUsesGLUT())
  {
    TRACEC(JAEPHYS_DEM,1,"Display usa GLUT");
		glutDisplayFunc(visualizeEntities);
		glutIdleFunc(animateWorld);
		glutKeyboardFunc(Keyboard::doCallback);
		glutMainLoop();
  }
  else
  {
    TRACEC(JAEPHYS_DEM,1,"Display No usa GLUT");
    while(1)
		{
      animateWorld();
			Keyboard::getKey();
		}
	}
  //CRONO_FUNC_FIN;
}

///////////////////////////////////////////////////////////////////////////////
bool Demiurge::displayUsesGLUT()
{
	//  bool usesDisplay2D_gl = (dynamic_cast<Display2D_gl*>(display2D) != 0);
  bool usesDisplay3D_gl 
		= dynamic_cast<Display3D_gl*>(&Display3D::getCurrent()) != 0;

  return ( //usesDisplay2D_gl || 
					 usesDisplay3D_gl);
}

void Demiurge::pauseSimulation()
{
  pause_ = true;
}

void Demiurge::resumeSimulation()
{
  pause_ = false;
}

void Demiurge::endSimulation()
{
  for(unsigned int i=0; i< vEntity_.size(); ++i)
  {
    delete vEntity_[i];
  }

  for(unsigned int i=0; i< vInter_.size(); ++i)
  {
    delete vInter_[i];
  }
	/*
  if(display2D_)
  {
    delete display2D;
  }
	*/
  //CRONO_FIN;
  exit(0);
}

///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION: anima el sistema durante el tiempo de un frame. Realiza tantas
//  iterationes de los algoritmos de animación e interacción como indique
//  el atributo itsPorFrame_ siempre y cuando no exceda del plazo disponible,
//  el cual viene dado por el atributo cpf_. Si despues de haber
//  hecho las iterationes indicadas queda tiempo, visualiza la escena, en
//  caso contrario, se salta el frame.
//   
void Demiurge::animateWorld()
{ 
  //CRONO_FUNC_INI;

  static clock_t frameDeadline=clock();
  frameDeadline += cpf_;

  // vemos si el sistema esta pausedo. Si es asi, nos limitamos a visualizar
  // los entitys sin animarlos
  if(pause_)
  {
    visualizeEntities();

    // esperamos a que finalice el periodo del frame
    while(clock() < frameDeadline);
    return;
  }

  bool withinDeadline=true;
  for(int i=1; i<= ipf_;++i)
  {
    animateEntities();
    evaluateInteractions();
    if(clock() > frameDeadline)
    {
      withinDeadline=false;
      break;
    }
  }

  if(withinDeadline)
  {
    visualizeEntities();
  }

  // esperamos a que termine el periodo del frame actual
  while(clock() < frameDeadline);
  //CRONO_FUNC_FIN;
}


//////////////////////////////////////////////////////////////////////////////
// DESCRIPTION notifica a todos los entitys del sistema cual es el factor por el
//   cual deben de multiplicar sus velocidades para adecuarse al rendimiento
//   de la maquina, con objeto de que este no afecte a la velocidad visual de 
//   animacion.
//   
void Demiurge::setAnimationFactor(const double factor)
{
  TRACEC(JAEPHYS_DEM,2,"factor="<< factor);
  unsigned int itFin= vEntity_.size();
  for(unsigned int i=0; i< itFin; ++i)
    (vEntity_[i])->animationFactor(factor);
}

//////////////////////////////////////////////////////////////////////////////
// DESCRIPTION: ejecuta una vez el algoritmo de animacion de cada uno de los entitys
void Demiurge::animateEntities()
{
  //CRONO_FUNC_INI;
  unsigned int itFin= vEntity_.size();
  for(unsigned int i=0; i< itFin; ++i)
    (vEntity_[i])->animate();
 
  //CRONO_FUNC_FIN;
}

//////////////////////////////////////////////////////////////////////////////
// DESCRIPTION: evalua las interactiones del sistema
void Demiurge::evaluateInteractions()
{
  //CRONO_FUNC_INI;
  unsigned int itFin= vInter_.size();
  for(unsigned int i=0; i< itFin; ++i)
    (vInter_[i])->evaluate();
  //CRONO_FUNC_FIN;
}
//////////////////////////////////////////////////////////////////////////////
// DESCRIPTION: renderiza en el display cada uno de los entitys del sistema
void Demiurge::visualizeEntities()
{
  //CRONO_FUNC_INI;

  Display3D& display = Display3D::getCurrent();
	display.clean();

  unsigned int itFin= vEntity_.size();
  for(unsigned int i=0; i< itFin; ++i)
    (vEntity_[i])->visualize();

  display.flush();

  //CRONO_FUNC_FIN;
}


//////////////////////////////////////////////////////////////////////////////
void Demiurge::defineDefaultKeys()
{
  Keyboard::registerCallback('p', pause_callback);
  Keyboard::registerCallback('k', exit_callback);

  Keyboard::registerCallback('q', translate_callback);
  Keyboard::registerCallback('w', translate_callback);
  Keyboard::registerCallback('e', translate_callback);
  Keyboard::registerCallback('a', translate_callback);
  Keyboard::registerCallback('s', translate_callback);
  Keyboard::registerCallback('d', translate_callback);


  Keyboard::registerCallback('r', rotate_callback);
  Keyboard::registerCallback('t', rotate_callback);
  Keyboard::registerCallback('y', rotate_callback);
  Keyboard::registerCallback('f', rotate_callback);
  Keyboard::registerCallback('g', rotate_callback);
  Keyboard::registerCallback('h', rotate_callback);
}

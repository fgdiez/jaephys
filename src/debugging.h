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


#ifndef JAEPHYS_DEPURACION_H
#define JAEPHYS_DEPURACION_H

#include <iostream>
#include <stdlib.h>

static int TRACE_LEVEL 
 = (getenv("TRACE_LEVEL") != NULL) ? atoi(getenv("TRACE_LEVEL")): 0;


static int JAEPHYS_PARSE
 = (getenv("JAEPHYS_PARSE") != NULL) ? atoi(getenv("JAEPHYS_PARSE")): 0;
static int JAEPHYS_DEM
 = (getenv("JAEPHYS_DEM") != NULL) ? atoi(getenv("JAEPHYS_DEM")): 0;

static int JAEPHYS_ENT
 = (getenv("JAEPHYS_ENT") != NULL) ? atoi(getenv("JAEPHYS_ENT")): 0;
static int JAEPHYS_SHAPE
 = (getenv("JAEPHYS_SHAPE") != NULL) ? atoi(getenv("JAEPHYS_SHAPE")): 0;

static int JAEPHYS_DISP
 = (getenv("JAEPHYS_VISOR") != NULL) ? atoi(getenv("JAEPHYS_VISOR")): 0;
static int JAEPHYS_VISZ
 = (getenv("JAEPHYS_VISZ") != NULL) ? atoi(getenv("JAEPHYS_VISZ")): 0;

static int JAEPHYS_ANIM
 = (getenv("JAEPHYS_ANIM") != NULL) ? atoi(getenv("JAEPHYS_ANIM")): 0;
static int JAEPHYS_INTER
 = (getenv("JAEPHYS_INTER") != NULL) ? atoi(getenv("JAEPHYS_INTER")): 0;
static int JAEPHYS_COLLID
 = (getenv("JAEPHYS_COLLID") != NULL) ? atoi(getenv("JAEPHYS_COLLID")): 0;
static int JAEPHYS_GEO
 = (getenv("JAEPHYS_GEO") != NULL) ? atoi(getenv("JAEPHYS_GEO")): 0;
static int JAEPHYS_INERTIA
 = (getenv("JAEPHYS_INERTIA") != NULL) ? atoi(getenv("JAEPHYS_INERTIA")): 0;
static int JAEPHYS_GRAV
 = (getenv("JAEPHYS_GRAV") != NULL) ? atoi(getenv("JAEPHYS_GRAV")): 0;


static int JAEPHYS_TEST
 = (getenv("JAEPHYS_TEST") != NULL) ? atoi(getenv("JAEPHYS_TEST")): 0;

// Macros
//
#ifndef RELEASE
// Trace
#define TRACE(level, message) \
  if (level <= TRACE_LEVEL)     \
    std::cout << __FILE__ << "::" << __FUNCTION__ << "(" << __LINE__ << ") " \
         << message << std::endl;

// Trace clasificada
#define TRACEC(clasificacion, level, message) \
  if(level <= clasificacion)     \
    std::cout << __FILE__ << "::" << __FUNCTION__ << "(" << __LINE__ << ") " \
         << message << std::endl;

// Trace si se cumple la condition
#define TRACECOND(clasificacion, condition, message) \
  if((clasificacion > 0) && (condition))                       \
    std::cout << __FILE__ << "::" << __FUNCTION__ << "(" << __LINE__ << ") " \
         << message << std::endl;

#define TEST(condition, message) \
  if(!( condition))                      \
  { std::cout << "Abortando en " \
         << __FILE__ << "(" << __LINE__ << "):" << message << std::endl; \
    while(1); \
  }
#else
// Omitimos todo codigo de trace y comprobacion
// 
  #define TRACE(level, message) 
  #define TRACEC(clasificacion,level, message) 
  #define TRACECOND(clasificacion,condition, message) 
  #define TEST(condition, message)
#endif

#define GUARANTEE(condition, message) \
  if(!( condition))                      \
  { std::cout << "Aborted in " \
         << __FILE__ << "(" << __LINE__ << "):" << message << std::endl; \
    while(1); \
  }

#endif // JAEPHYS_DEPURACION_H

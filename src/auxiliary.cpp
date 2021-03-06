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

#include "auxiliary.hpp"

#include <string.h>

#include <iostream>
#include <sstream>

#include <assert.h>

using namespace jaephys;

bool equal_char::operator()( const char* s1, const char* s2)
{
	return strcmp( s1, s2 ) == 0;
}

void jaephys::str2Double( const std::string & orig, double & dest )
{
	std::stringstream aux;
	aux << orig;
	aux >> dest;
}


std::ostream & jaephys::operator<<( std::ostream & os, RelativePosition rp)
{
	switch(rp) {
		case EXTERNAL: os << "EXTERNAL"; break;
		case THIS_CONTAINED: os << "THIS_CONTAINED"; break;
		case THIS_CONTAINER: os << "THIS_CONTAINER"; break;
		default: assert(0);
	} 
	return os;
}

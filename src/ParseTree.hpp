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


#ifndef JAEPHYS_PARSETREE_HPP
#define JAEPHYS_PARSETREE_HPP

#include <vector>
#include <string>

namespace jaephys
{
	class ParseTree
	{
	public:
	  // constructors & copy operator
    ParseTree();
	  ParseTree( const std::string & token, const std::string & value );
    ParseTree(const ParseTree& other);
    ParseTree& operator=(const ParseTree& other);

    // iteration
	  void iniIteration();
	  ParseTree* nextChild();

    // modification
	  ParseTree& createChild(const std::string& token, const std::string& value);

    // observation
    void print(const std::string& margin= "");

    // attributes 
	  std::string token_;
	  std::string value_;
	  
	protected:
	  // attributes
	  unsigned int index_;
	  std::vector<ParseTree> children_;
	};

}

#endif //JAEPHYS_PARSETREE_HPP

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

#ifndef JAEPHYS_PARSER_HPP
#define JAEPHYS_PARSER_HPP

#include <string>

namespace jaephys
{
	class ParseTree;

  enum EnumToken
  { TOKEN_INI_RECURSION
  , TOKEN_END_RECURSION
  , TOKEN_SINGLE
  , TOKEN_UNPAIRED
  };

	//////////////////////////////////////////////////////////////////////////////
	class Parser
	{
	public:
	  void createParseTree(const char* filename, ParseTree* & tree);
	protected:
	  bool nextPair_tokenValue(std::string& token, std::string& value);
	  bool processChild(std::string& parentToken, ParseTree& parentTree);
    EnumToken getTokenType(std::string& parentToken, std::string& token);

    std::string filename_;
    unsigned int index_; // indice de parseo del fichero a parsear
	};

} // namespace

#endif // JAEPHYS_PARSER_HPP

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

#include "Parser.hpp"
#include "ParseTree.hpp"

#include <fstream>
#include <strstream>

#include "debugging.h"

using namespace jaephys;

void Parser::createParseTree(const char* filename, ParseTree*& tree)
{
  index_ = 0;
  // abrimos el file
  std::ifstream fich(filename);
  GUARANTEE(fich.is_open(), "El file " << filename << " no se pudo abrir");

  std::strstream ss;
  ss  << fich.rdbuf();

  filename_ = ss.str();
  
  tree= new ParseTree(std::string("root"), std::string(""));
  std::string token("");
  processChild(token, *tree);
}

///////////////////////////////////////////////////////////////////////////////
bool Parser::processChild(std::string& parentToken, ParseTree& parentTree)
{
  std::string token, value;
  if(! nextPair_tokenValue(token,value))
    return false;
    
  EnumToken type = getTokenType(parentToken, token);
  switch(type)
  {
    case TOKEN_INI_RECURSION:
    {
      TRACEC(JAEPHYS_PARSE,2,"TOKEN_INI_RECURSION");
      ParseTree& tree = parentTree.createChild(token, value);

      bool remainGrandChildren=true;
      while(remainGrandChildren)
      {
        remainGrandChildren = processChild(token,tree);
      }
      return true;
      break;
    }
    case TOKEN_SINGLE:
    {
      TRACEC(JAEPHYS_PARSE,2,"TOKEN_SINGLE");
      parentTree.createChild(token, value);
      return true;
      break;
    }
    case TOKEN_END_RECURSION:
      TRACEC(JAEPHYS_PARSE,2,"TOKEN_END_RECURSION");
      return false;
      break;
    case TOKEN_UNPAIRED:
      TRACEC(JAEPHYS_PARSE,2,"TOKEN_UNPAIRED");
      GUARANTEE(false,"token=" << token << " unpaired");
      break;
  }  
  return false;  
}

bool Parser::nextPair_tokenValue(std::string& token, std::string& value)
{
  index_ = filename_.find_first_of('<',index_);

  unsigned int blockEnd= filename_.find_first_of('>',index_);
  if(blockEnd == std::string::npos)
  {
    GUARANTEE(false, "Block not closed");
  }

  // begin of token name
  unsigned int ini = filename_.find_first_not_of("< \t\n", index_);
  if(ini == blockEnd)
  {
    TRACEC(JAEPHYS_PARSE,1,"Empty block");
    return false;
  }

  // end of token name
  index_ = filename_.find_first_of("> \t\n", ini);
  int end= index_ - ini;
  token = std::string(filename_,ini,end); // get token name

  if( index_ < blockEnd)
  {
		// skip spaces
    index_ = filename_.find_first_not_of(" \t\n", index_);
  }
  
  if(index_ == blockEnd)
  {
    // no value
    value = "";
  }
  else
  {
    // get value
    ini = index_;
    index_ = blockEnd;
    end= index_ - ini;
    value = std::string(filename_,ini,end);
  }
  TRACEC(JAEPHYS_PARSE,1,"token=" << token << std::endl << "value="<< value);
  return true;
}

EnumToken Parser::getTokenType(std::string & parentToken, std::string & token)
{

  if(token[0] == '\\')
  {
    int end= parentToken.size();
    int ini= end -1;
    parentToken.replace(ini,end,"");

    token.replace(0,1,"");
    TRACEC(JAEPHYS_PARSE,3,"parent=" << parentToken);
    TRACEC(JAEPHYS_PARSE,3,"token=" << token);
    
    if(token == parentToken)
    {
      return TOKEN_END_RECURSION;
    }
    else
    {
      return TOKEN_UNPAIRED;
    }
  }
  else if(token[token.size() -1] == '\\')
  {
    return TOKEN_INI_RECURSION;
  }
  else
  {
    return TOKEN_SINGLE;
  }
}

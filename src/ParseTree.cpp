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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *****************************************************************************/


#include "ParseTree.hpp"

using namespace jaephys;

ParseTree::ParseTree(const string& token, const string& value)
: token_(token)
, value_(value)
, index_(0)
{
}
  
ParseTree::ParseTree()
: token_("")
, value_("")
, index_(0)
{
}

ParseTree::ParseTree(const ParseTree& other)
: index_(0)
{
  token_ = other.token_;
  value_ = other.value_;
  sons_ = other.sons_;
}

ParseTree& ParseTree::operator=(const ParseTree& other)
{
  index_ = 0;
  token_ = other.token_;
  value_ = other.value_;
  sons_ = other.sons_;
  return *this;
}
  
void ParseTree::iniIteration()
{
  index_ = 0;
}

ParseTree* ParseTree::nextSon()
{
  if(sons_.size() <= index_)
    return 0;
      
  ParseTree* tree= &(sons_[index_]);
  ++index_;
  return tree;
}

ParseTree& ParseTree::createSon(const string& token, const string& value)
{
  sons_.push_back(ParseTree(token,value));
  return sons_[sons_.size() -1];
}

void ParseTree::print(const string& margin)
{
  cout << margin << token_ << "\t" << value_ << endl;
  iniIteration();
  string son_margin = margin + "\t";

  for( ParseTree* a = nextSon()
     ; a != 0
     ; a = nextSon()
     )
  {
    a->print(son_margin);
  }
  
}

#ifndef JAEPHYS_FENTITY_H
#define JAEPHYS_FENTITY_H

#include "auxiliary.hpp"
#include <vector>
#include <string>
#include <map>

namespace jaephys
{
	class ParseTree;
	class Entity;
	class Attribute;
	class AnimationAlgorithm;
	class Algorithm;
	class PolygonalShape;
  class Coord3D;

  class FEntity
  {
   public:
     void createElement( ParseTree & tree, Entity*& entity );

   protected:
     void createAnimation (ParseTree & tree, Entity & entity );
     void createAppearence(ParseTree & tree, Entity & entity );
     void createAttribute (ParseTree & tree, Entity & entity );

     // metodos fabrica de attribute
     void createPolygonalShape( ParseTree  & tree, 
																const std::string& concepto, 
																Attribute* & attrib );
     void createCoord3d( ParseTree  & tree,
												 const std::string& concepto, 
												 Attribute* & attrib );
 
     void createDouble( ParseTree  & tree,
												const std::string& concepto, 
												Attribute* & attrib );

		 std::map<std::string, Attribute*> mapAttrib_;
     std::vector<Attribute*> vAttrib_;
		int aux;
  };
}

#endif 

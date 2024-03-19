#pragma once

//cPhysicsObjectTypes

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <map>
#include <string>


class cPhysicsObjectTypes
{
public:
	enum eShapeType
	{
		BOX,		// Params: "half-extents": x,y,z 

		SPHERE,		// Params: "radius": x

		CAPSULE,
		MESH,
		// ...etc.

		UNKNOWN
	};

	eShapeType shapeType = UNKNOWN;

	// Orientation of the object
	glm::mat4 matModel = glm::mat4(1.0f);

	std::map< std::string /*param name*/, glm::vec4 /*value*/ > map_ParamToValue;

};

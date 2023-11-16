#ifndef _cContact_HG_
#define _cContact_HG_

// Stores contact information for a SINGLE
//   object in the world. 
#include <glm/glm.hpp>

class cContact
{
public:
	cContact() : penetrationDepth(0.0f), objectID(0) { }
	// From Ian Millington's book suggestion
	// Contact point (xyz) in world space
	// Contact normal (xyz) in world space (i.e. The direction of the contact)
	// Depth of penetration
	glm::vec3 contactXYZ;
	glm::vec3 normal;
	float penetrationDepth;
	// 
	unsigned int objectID;
};

#endif

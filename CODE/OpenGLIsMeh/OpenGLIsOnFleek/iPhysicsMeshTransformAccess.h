#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

// Used by the Physics system: 
//	Read and Write access to position, orientation, and scale

// Interface class:
// - class
// - virtual destructor
// - ONLY methods
// - ALL methods are vitual 
// - All are PURE vitual ("= 0" at end)

class iPhysicsMeshTransformAccess
{
public:
	virtual ~iPhysicsMeshTransformAccess() {};

	virtual glm::vec3 getDrawPosition(void) = 0;
	virtual glm::vec3 getDrawOrientation(void) = 0;

	// Used by Phsyics (and anything else) that needs to update the DRAW position
	virtual void setDrawPosition(const glm::vec3& newPosition) = 0;
	virtual void setDrawOrientation(const glm::vec3& newOrientation) = 0;
	virtual void setDrawOrientation(const glm::quat& newOrientation) = 0;
};

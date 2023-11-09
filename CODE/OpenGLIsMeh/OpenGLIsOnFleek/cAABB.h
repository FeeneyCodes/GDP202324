#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <math.h>
#include <map>

// These would also store triangle info, 
//	either the actual vertices of the triangle, or the index to the triangle...

class cAABB
{
public:
	glm::vec3 minXYZ;
	glm::vec3 extentXYZ;	// AKA the "lengths" or whatever

	// I'll place the index calculation here 
	// It's based on the lowest point on the AABB
	unsigned int getLocationIndex(void)
	{
		// Note that the casting to int will "round down" by removing the fractions
		unsigned int theX = (unsigned int) floor(this->minXYZ.x / extentXYZ.x);
		unsigned int theY = (unsigned int) floor(this->minXYZ.y / extentXYZ.y);
		unsigned int theZ = (unsigned int) floor(this->minXYZ.z / extentXYZ.z);

		// Shift the numbers into the columns we want:
		// xxx,yyy,zzz
		// For 32 bits, we'll multiply by 1000

		unsigned int theIndex = (theX * 1000 * 1000) +
		                        (theY * 1000) +
		                        (theZ);

		// If you are using negative numbers, you can use the 1st 'digit" as a sign:
		// Instead of xxx,yyy,zzz, you get
		//            +xx,+yy,+zz (where "+" is the "sign"
		// Let's say 0 is positive and 1 is negative
		if (this->minXYZ.x < 0)
		{
			theIndex += 100'000'000;	// Place a "1" at that location +__,___,___
		}
		if (this->minXYZ.y < 0)
		{
			theIndex += 100'000;	// Place a "1" at that location ___,+___,___
		}
		if (this->minXYZ.z < 0)
		{
			theIndex += 100;	// Place a "1" at that location ___,___,+___
		}
		return theIndex;
	}

	// Maybe we want "child" AABBs (like for an oct-tree)
	std::map< unsigned int /*index*/, cAABB* > vecChild_pAABBs;
};


#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <math.h>
#include <map>
#include <string>

// These would also store triangle info, 
//	either the actual vertices of the triangle, or the index to the triangle...

class cAABB
{
public:
	unsigned int uniqueID;
	glm::vec3 minXYZ;
	glm::vec3 maxXYZ;	// AKA the "lengths" or whatever
	glm::vec3 getCentreXYZ(void);
	glm::vec3 getExtentsXYZ(void)
	{
		// TODO: Calculate the extent
		return maxXYZ - minXYZ;	// Does this work?? 
	}

	// I'll place the index calculation here 
	// It's based on the lowest point on the AABB
	unsigned int getLocationIndex_OG(void)
	{
		// Note that the casting to int will "round down" by removing the fractions
		unsigned int theX = (unsigned int) floor(this->minXYZ.x / getExtentsXYZ().x);
		unsigned int theY = (unsigned int) floor(this->minXYZ.y / getExtentsXYZ().y);
		unsigned int theZ = (unsigned int) floor(this->minXYZ.z / getExtentsXYZ().z);

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

	// Calls the static function
	unsigned int getLocationIndex(void)
	{
		return cAABB::static_getLocationIndex(this->minXYZ, this->getExtentsXYZ());
	}

	// Maybe we want "child" AABBs (like for an oct-tree)
	std::map< unsigned int /*index*/, cAABB* > vecChild_pAABBs;


	static unsigned int static_getLocationIndex(glm::vec3 minXYZ, glm::vec3 extentXYZ)
	{
		// Note that the casting to int will "round down" by removing the fractions
		unsigned int theX = (unsigned int)floor(minXYZ.x / extentXYZ.x);
		unsigned int theY = (unsigned int)floor(minXYZ.y / extentXYZ.y);
		unsigned int theZ = (unsigned int)floor(minXYZ.z / extentXYZ.z);

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
		if (minXYZ.x < 0)
		{
			theIndex += 100'000'000;	// Place a "1" at that location +__,___,___
		}
		if (minXYZ.y < 0)
		{
			theIndex += 100'000;	// Place a "1" at that location ___,+___,___
		}
		if (minXYZ.z < 0)
		{
			theIndex += 100;	// Place a "1" at that location ___,___,+___
		}
		return theIndex;
	}//static unsigned int getLocationIndex(

//	std::string& operator<<(const cAABB& other)
//	{
//		retrn l
//	}
};




//class Circle;		// Centre, radius
//class Square;
//
//Circle A, B;
//Circle C = A + B;
//
//Circle oprerator + (const Circle & A, const Circle & B);
//
//vec3 a;
//float b;
//vec3 c = a * b;


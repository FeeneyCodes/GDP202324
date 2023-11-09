#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <map>

class cAABB
{
	static const unsigned int NUM_BOXES = 10;		// Per axis
	// 10,000 * 24 = 240,000 
	cAABB* theAABBs[NUM_BOXES][NUM_BOXES][NUM_BOXES];

	std::vector< std::vector< std::vector<cAABB> > > vecAABBs;
	//vecAABBs[3][2][4]

	std::map<unsigned int /*indexXYZ*/, cAABB> map1DAABBs;

//	cAABB theAABBWeWant = vec1DAABBs[27, 030, 005];
//	vector<cTri> vecTriangles;
};


class cAABB
{
public:
	// Do this??
	glm::vec3 centreXYZ;
	float length, width, height;
	float length_X, width_Y, height_Z;
	// Fill in the rest with methods
	glm::vec3 getMaxXYZ(void);

	// Or this??
	glm::vec3 minXYZ;
	glm::vec3 maxXYZ;

	// Or this?
	glm::vec3 centreXYX;
	float size;		// If it's a cube

	// Or this
	glm::vec3 centreXYZ;
	glm::vec3 halfLengthsXYZ;	
	// DON'T Make them ALL variables
//	glm::vec3 minXYZ;
//	glm::vec3 maxXYZ;
//	glm::vec3 extents;
	glm::vec3 getMinXYZ(void);
	glm::vec3 getMaxXYZ(void);
	glm::vec3 getExtentsXYZ(void);

	void Update(void);



	// Or this?
	glm::vec3 minXYZ;	//27, 30, 5
	glm::vec3 extents;			// 10 units

	unsigned int getLocationIndex(void)
	{
		unsigned int theX = (unsigned int)this->minXYZ.x;
		unsigned int theY = (unsigned int)this->minXYZ.y;
		unsigned int theZ = (unsigned int)this->minXYZ.z;

		// 32 bits    8 bits per digit
		// Unsigned char: 0 to 255
		// 32 bits ---> 4 billion
		// 4,000,000,000	


		unsigned int theIndex =
			(theX * 1000 * 1000) +
			(theY * 1000) +
			(theZ);


		// 27, 30, 5
		// 27,000,000
		//     30,000
		//          5
		// 27,030,005

		// Or use 64.
		unsigned long long theIndex =
			(theX * 1000'000 * 1000'000) +
			(theY * 1000'000) +
			(theZ);

	}

	// Fill in whatever is missing with methods
	inline glm::vec3 getMaxXYZ(void)
	{
		glm::vec3 min;
//		min.x = minXYX.x - extents.x;
//		min.y = minXYX.y - extents.y;
//		min.z = minXYX.z - extents.z;
		return min;
	}



	bool isItInside(glm::vec3 testPoint)
	{
		// Instead of seeing if it's INSIDE, 
		// test if it's OUTSIDE...
		if (testPoint.x < minXYZ.x)					{	return false; }
		if (testPoint.x >= (minXYZ.x + extents.x))	{	return false; }

		if (testPoint.x < minXYZ.x)					{	return false; }
		if (testPoint.x >= (minXYZ.x + extents.x))	{	return false; }

		if (testPoint.x < minXYZ.y)					{	return false; }
		if (testPoint.x >= (minXYZ.y + extents.y))	{	return false; }

		if (testPoint.x < minXYZ.y)					{	return false; }
		if (testPoint.x >= (minXYZ.y + extents.y))	{	return false; }

		if (testPoint.x < minXYZ.z)					{	return false; }
		if (testPoint.x >= (minXYZ.z + extents.z))	{	return false; }

		if (testPoint.x < minXYZ.z)					{	return false; }
		if (testPoint.x >= (minXYZ.z + extents.z))	{	return false; }

		// We're in!
		return true;	
	}

	bool doWeOverlap(cAABB &theOtherAABB)
	{

	}
};


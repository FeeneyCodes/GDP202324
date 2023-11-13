//cVAOManager_UV_helper
#include "cVAOManager.h"
#include <glm/gtc/constants.hpp>		// for PI

void cVAOManager::CalculateExtents(sModelDrawInfo& drawInfo)
{
		// Calcaulte the "extents" of the model 
		// "bounding box"

		// Assume vertex #0 is both the smallest and largest values
	drawInfo.maxValues.x = drawInfo.pVertices[0].x;
	drawInfo.maxValues.y = drawInfo.pVertices[0].y;
	drawInfo.maxValues.z = drawInfo.pVertices[0].z;

	drawInfo.minValues.x = drawInfo.pVertices[0].x;
	drawInfo.minValues.y = drawInfo.pVertices[0].y;
	drawInfo.minValues.z = drawInfo.pVertices[0].z;

	for (unsigned int index = 0; index != drawInfo.numberOfVertices; index++)
	{
		sVert_n_rgba_uv CurrentVertex = drawInfo.pVertices[index];

		if (CurrentVertex.x > drawInfo.maxValues.x) { drawInfo.maxValues.x = CurrentVertex.x; }
		if (CurrentVertex.y > drawInfo.maxValues.y) { drawInfo.maxValues.y = CurrentVertex.y; }
		if (CurrentVertex.z > drawInfo.maxValues.z) { drawInfo.maxValues.z = CurrentVertex.z; }

		if (CurrentVertex.x < drawInfo.minValues.x) { drawInfo.minValues.x = CurrentVertex.x; }
		if (CurrentVertex.y < drawInfo.minValues.y) { drawInfo.minValues.y = CurrentVertex.y; }
		if (CurrentVertex.z < drawInfo.minValues.z) { drawInfo.minValues.z = CurrentVertex.z; }

	}//for (unsigned int index = 0

	// What's the extent (delta or difference) between these axes
	drawInfo.maxExtents.x = drawInfo.maxValues.x - drawInfo.minValues.x;
	drawInfo.maxExtents.y = drawInfo.maxValues.y - drawInfo.minValues.y;
	drawInfo.maxExtents.z = drawInfo.maxValues.z - drawInfo.minValues.z;

	// Get the largest side;
	drawInfo.maxExtent = drawInfo.maxExtents.x;
	if (drawInfo.maxExtents.y > drawInfo.maxExtents.y)
	{
		drawInfo.maxExtent = drawInfo.maxExtent;
	}
	if (drawInfo.maxExtents.z > drawInfo.maxExtent)
	{
		drawInfo.maxExtent = drawInfo.maxExtents.z;
	}
	return;
}


// Generates texture coordinates based on a shperical projection from the 
// origin (0,0,0) location of the model. 
// NOTE: Will overwrite any existing texture coordinates 
// Inspired by: http://www.mvps.org/directx/articles/spheremap.htm
// 
//	basedOnNormals = true --> uses normals for calculation, so good for reflections
//	basedOnNormals = false --> uses model coordinates, so more 'shrink wrapped'
//  fast -> approximate or not (not uses sin() )
//  scale -> normally 1.0, but can be the limits of the texture coord
//
// The 'bias' is what coordinates are used for the generation. 
// For example, if POSITIVE_X is for u and POSITIVE_Y is for v, then:
//	u coords: based on -x to +x
//  v coords: based on -y to +y
//
void cVAOManager::GenTextureCoordsSpherical(sModelDrawInfo &modelDrawInfo, enumTEXCOORDBIAS uBias, enumTEXCOORDBIAS vBias, bool basedOnNormals, float scale, bool fast)
{
	// determine the 'remaining' coordinate...
	bool xUsed = false;
	bool yUsed = false;
	bool zUsed = false;
	if (uBias == POSITIVE_X || vBias == POSITIVE_X)	xUsed = true;
	if (uBias == POSITIVE_Y || vBias == POSITIVE_Y)	yUsed = true;
	if (uBias == POSITIVE_Z || vBias == POSITIVE_Z)	yUsed = true;

	for ( unsigned int index = 0; index != modelDrawInfo.numberOfVertices; index++ )
	{
		sVert_n_rgba_uv* pCurrentVertex = &(modelDrawInfo.pVertices[index]);

		glm::vec3 xyz;

		if (basedOnNormals)
		{
			if (uBias == POSITIVE_X)		xyz.x = pCurrentVertex->nx;
			else if (uBias == POSITIVE_Y)	xyz.x = pCurrentVertex->ny;
			else if (uBias == POSITIVE_Z)	xyz.x = pCurrentVertex->nz;

			if (vBias == POSITIVE_X)		xyz.y = pCurrentVertex->nx;
			else if (vBias == POSITIVE_Y)	xyz.y = pCurrentVertex->ny;
			else if (vBias == POSITIVE_Z)	xyz.y = pCurrentVertex->nz;

			// Fill in the remaining coordinate...
			if (!xUsed)	xyz.z = pCurrentVertex->nx;
			if (!yUsed)	xyz.z = pCurrentVertex->ny;
			if (!zUsed)	xyz.z = pCurrentVertex->nz;
		}
		else
		{
			if (uBias == POSITIVE_X)		xyz.x = pCurrentVertex->x;
			else if (uBias == POSITIVE_Y)	xyz.x = pCurrentVertex->y;
			else if (uBias == POSITIVE_Z)	xyz.x = pCurrentVertex->z;

			if (vBias == POSITIVE_X)		xyz.y = pCurrentVertex->x;
			else if (vBias == POSITIVE_Y)	xyz.y = pCurrentVertex->y;
			else if (vBias == POSITIVE_Z)	xyz.y = pCurrentVertex->z;

			// Fill in the remaining coordinate...
			if (!xUsed)	xyz.z = pCurrentVertex->x;
			if (!yUsed)	xyz.z = pCurrentVertex->y;
			if (!zUsed)	xyz.z = pCurrentVertex->z;
		}

		xyz = glm::normalize(xyz);

		if (fast)
		{
			pCurrentVertex->u1 = ((xyz.x / 2.0f) + 0.5f) * scale;
			pCurrentVertex->v1 = ((xyz.y / 2.0f) + 0.5f) * scale;
		}
		else
		{
			pCurrentVertex->u1 = ((asin(xyz.x) / glm::pi<float>() ) + 0.5f) * scale;
			pCurrentVertex->v1 = ((asin(xyz.y) / glm::pi<float>() ) + 0.5f) * scale;
		}
		pCurrentVertex->u2 = pCurrentVertex->u1;
		pCurrentVertex->v2 = pCurrentVertex->v1;
	}
	return;
}



void cVAOManager::GenTextureCoordsLinear(sModelDrawInfo &modelDrawInfo, enumTEXCOORDBIAS uBias, float scale)
{
	// Check to see if there's an invalid uBias
	switch (uBias)
	{
	case enumTEXCOORDBIAS::PLANAR_XY:
	case enumTEXCOORDBIAS::PLANAR_XZ:
	case enumTEXCOORDBIAS::PLANAR_YZ:
	case enumTEXCOORDBIAS::PLANAR_ON_WIDEST_AXES:
		// It's OK...
		break;
	default:
		// Likely passed an invalid enum
		return;
		break;
	};

	this->CalculateExtents(modelDrawInfo);

	// If it's PLANAR_ON_WIDEST_AXES, then determine the right option:
	if (uBias == enumTEXCOORDBIAS::PLANAR_ON_WIDEST_AXES)
	{
		// Assume it's XZ. I mean, why not? 
		uBias = enumTEXCOORDBIAS::PLANAR_XZ;

		// Is is actually XY (so deltaZ is the smallest?)
		if ((modelDrawInfo.maxExtents.x < modelDrawInfo.maxExtents.z) &&
			(modelDrawInfo.maxExtents.y < modelDrawInfo.maxExtents.z))
		{
			uBias = enumTEXCOORDBIAS::PLANAR_XY;
		}
		// Or is it YZ? (so deltaX is the smallest?)
		else if ((modelDrawInfo.maxExtents.y < modelDrawInfo.maxExtents.x) &&
				 (modelDrawInfo.maxExtents.z < modelDrawInfo.maxExtents.x))
		{
			uBias = enumTEXCOORDBIAS::PLANAR_YZ;
		}
	}//if(uBias==PLANAR_ON_WIDEST_AXES...

	for (unsigned int vertIndex = 0; vertIndex != modelDrawInfo.numberOfVertices; vertIndex++)
	{
		sVert_n_rgba_uv* pCurrentVert = &(modelDrawInfo.pVertices[vertIndex]);

		float u = 0.0f;
		float v = 0.0f;

		switch (uBias)
		{
		case enumTEXCOORDBIAS::PLANAR_XY:
			u = (pCurrentVert->x - modelDrawInfo.minValues.x) / (modelDrawInfo.maxExtents.x / scale);
			v = (pCurrentVert->y - modelDrawInfo.minValues.y) / (modelDrawInfo.maxExtents.y / scale);
			break;
		case enumTEXCOORDBIAS::PLANAR_XZ:
			u = (pCurrentVert->x - modelDrawInfo.minValues.x) / (modelDrawInfo.maxExtents.x / scale);
			v = (pCurrentVert->z - modelDrawInfo.minValues.z) / (modelDrawInfo.maxExtents.z / scale);
			break;
		case enumTEXCOORDBIAS::PLANAR_YZ:
			u = (pCurrentVert->y - modelDrawInfo.minValues.y) / (modelDrawInfo.maxExtents.y / scale);
			v = (pCurrentVert->z - modelDrawInfo.minValues.z) / (modelDrawInfo.maxExtents.z / scale);
			break;
		};

		pCurrentVert->u1 = u;
		pCurrentVert->v1 = v;

	}//for(unsigned int vertIndex...

	return;
}


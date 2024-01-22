#include "cSoftBodyVerlet.h"

#if _DEBUG 
#include <iostream>
#endif

cSoftBodyVerlet::cSoftBodyVerlet()
{

}

cSoftBodyVerlet::~cSoftBodyVerlet()
{

}

// This is for loading the original model
bool cSoftBodyVerlet::CreateSoftBody(sModelDrawInfo ModelInfo)
{
	// Copy the model info
	this->m_ModelVertexInfo = ModelInfo;
	// Copy the vertices and indices as well
	// The cModelDrawInfo has pointers to the originally loaded vertices.
	sVertex* pVerticesLocalCopy = new sVertex[ModelInfo.numberOfVertices];
	for (unsigned int index = 0; index != ModelInfo.numberOfVertices; index++)
	{
		pVerticesLocalCopy[index].x = ModelInfo.pVertices[index].x;
		pVerticesLocalCopy[index].y = ModelInfo.pVertices[index].y;
		pVerticesLocalCopy[index].z = ModelInfo.pVertices[index].z;
		pVerticesLocalCopy[index].w = ModelInfo.pVertices[index].w;

		pVerticesLocalCopy[index].r = ModelInfo.pVertices[index].r;
		pVerticesLocalCopy[index].g = ModelInfo.pVertices[index].g;
		pVerticesLocalCopy[index].b = ModelInfo.pVertices[index].b;
		pVerticesLocalCopy[index].a = ModelInfo.pVertices[index].a;

		pVerticesLocalCopy[index].nx = ModelInfo.pVertices[index].nx;
		pVerticesLocalCopy[index].ny = ModelInfo.pVertices[index].ny;
		pVerticesLocalCopy[index].nz = ModelInfo.pVertices[index].nz;
		pVerticesLocalCopy[index].nw = ModelInfo.pVertices[index].nw;

		pVerticesLocalCopy[index].u = ModelInfo.pVertices[index].u;
		pVerticesLocalCopy[index].v = ModelInfo.pVertices[index].v;
	}//for (unsigned int index

	unsigned int* pIndicesLocalCopy = new unsigned int[ModelInfo.numberOfIndices];
	for (unsigned int index = 0; index != ModelInfo.numberOfIndices; index++)
	{
		pIndicesLocalCopy[index] = ModelInfo.pIndices[index];
	}//for (unsigned int index

	this->m_ModelVertexInfo.pVertices = pVerticesLocalCopy;
	this->m_ModelVertexInfo.pIndices = pIndicesLocalCopy;

	// From this point on, we are referencing the copy of the original vertices

	// Go through the model info and add the particles from the vertex locations
	this->vec_pParticles.reserve(this->m_ModelVertexInfo.numberOfVertices);

	for (unsigned int index = 0; index != this->m_ModelVertexInfo.numberOfVertices; index++)
	{
		sParticle* pParticle = new sParticle();
		pParticle->position.x = this->m_ModelVertexInfo.pVertices[index].x;
		pParticle->position.y = this->m_ModelVertexInfo.pVertices[index].y;
		pParticle->position.z = this->m_ModelVertexInfo.pVertices[index].z;

		pParticle->old_position = pParticle->position;

		// Save the pointer address.
		// Note the "&" (address of)
		pParticle->pModelVertex = &(this->m_ModelVertexInfo.pVertices[index]);

		this->vec_pParticles.push_back(pParticle);
	}//for ( unsigned int index =...

	// Constraints
	// Note: we are stepping through the array by 3s (3 vertices per triangle)
	for ( unsigned int index = 0; index < this->m_ModelVertexInfo.numberOfIndices; index += 3 )
	{
		// Example: ply file has “3 733 1026 736” for this triangle.
		// pIndices would have:
		// 
		//	 733	this->m_ModelVertexInfo.pIndices[ index + 0 ]
		//  1026	this->m_ModelVertexInfo.pIndices[ index + 1 ]
		//   736	this->m_ModelVertexInfo.pIndices[ index + 2 ]
		//
		sParticle* pParticle1 = this->vec_pParticles[ this->m_ModelVertexInfo.pIndices[index /* +0 */] ];
		sParticle* pParticle2 = this->vec_pParticles[ this->m_ModelVertexInfo.pIndices[index + 1] ];
		sParticle* pParticle3 = this->vec_pParticles[ this->m_ModelVertexInfo.pIndices[index + 2] ];

		sConstraint* pEdge1 = new sConstraint();
		pEdge1->pParticleA = pParticle1;
		pEdge1->pParticleB = pParticle2;
		pEdge1->restLength = this->calcDistanceBetween(pEdge1->pParticleA, pEdge1->pParticleB);

		sConstraint* pEdge2 = new sConstraint();
		pEdge2->pParticleA = pParticle2;
		pEdge2->pParticleB = pParticle3;
		pEdge2->restLength = this->calcDistanceBetween(pEdge2->pParticleA, pEdge2->pParticleB);

		sConstraint* pEdge3 = new sConstraint();
		pEdge3->pParticleA = pParticle3;
		pEdge3->pParticleB = pParticle1;
		pEdge3->restLength = this->calcDistanceBetween(pEdge3->pParticleA, pEdge3->pParticleB);

		this->vec_pConstraints.push_back(pEdge1);
		this->vec_pConstraints.push_back(pEdge2);
		this->vec_pConstraints.push_back(pEdge3);
	}//for ( unsigned int index

	return true;
}

float cSoftBodyVerlet::calcDistanceBetween(sParticle* pPartA, sParticle* pPartB)
{
	return glm::distance(pPartA->position, pPartB->position);
}



void cSoftBodyVerlet::VerletUpdate(double deltaTime)
{
	if (deltaTime > MAX_DELTATIME)
	{
		deltaTime = MAX_DELTATIME;
	}


	for (sParticle* pCurrentParticle : vec_pParticles )
	{
		glm::vec3 current_pos = pCurrentParticle->position;
		glm::vec3 old_pos = pCurrentParticle->old_position;

// From Michael's ancient (2016) code:
//		CVector& x = this->m_vec_x[index];
//		CVector temp = x;
//		CVector& oldx = this->m_vec_oldx[index];
//
//		CVector& a = this->m_vec_a[index];
//		x += x - oldx + (a * fTimeStep * fTimeStep);
//
//		oldx = temp;


		// Might be 2x???
		glm::vec3 new_pos = current_pos + (current_pos - old_pos) + (this->acceleration * (float)(deltaTime * deltaTime) );

		pCurrentParticle->old_position = current_pos;
		pCurrentParticle->position = new_pos;

		// Check if there is a LARGE different between old and new positions
#if _DEBUG 
		float distDiff = glm::distance(pCurrentParticle->position, pCurrentParticle->old_position);
		if (distDiff > 0.1f )
		{
			std::cout << "LARGE DISTANCE DETECTED!" << std::endl;
		}
#endif

		this->cleanZeros(pCurrentParticle->position);
		this->cleanZeros(pCurrentParticle->old_position);
	}

	return;
}

void cSoftBodyVerlet::SatisfyConstraints(void)
{
//	sParticle* pParticleA = NULL;
//	sParticle* pParticleB = NULL;
//	float restLength = 0.0f;


	// This is ONE pass of the constraint resolution
	for (sConstraint* pCurConstraint : this->vec_pConstraints )
	{
		// Decide that the direction is from A to B
		// Calculate direction from normalized length

		// A --------> B
		// 
		// A is 'start' and B is 'end'
		// 
		// Going from A to B is going in the positive (+ve) direction

		glm::vec3 vecAB = pCurConstraint->pParticleB->position - pCurConstraint->pParticleA->position;

// DANGER!: This does NOT return the length of the vector!!
// 		    It returns how many items are in the variable, like how many floats or whatever
//		float vecABLength = (float)vecAB.length();

		float vecABlength = glm::length(vecAB);

		glm::vec3 vecABdirection = glm::normalize(vecAB);
		// The centre point between point A and B
//		glm::vec3 vecABcentre = pCurConstraint->pParticleA->position + (vecAB / 2.0f);

		// Calculate difference between current length and rest length
		float lengthDiff = pCurConstraint->restLength - vecABlength;

#if _DEBUG 
		if (lengthDiff > 0.1f)
		{
			std::cout << "LARGE DISTANCE DETECTED!" << std::endl;
		}
#endif

		// 
		float lengthDiffRatio = lengthDiff / vecABlength;
			
		// Making this non-one, will change how quickly the objects move together q
		// For example, making this < 1.0 will make it "bouncier"
		float tightnessFactor = 1.0f;

		// Adjust the points based on this half length error
		pCurConstraint->pParticleA->position += vecABlength * 0.5f * lengthDiffRatio * tightnessFactor;
		pCurConstraint->pParticleB->position -= vecABlength * 0.5f * lengthDiffRatio * tightnessFactor;

		this->cleanZeros(pCurConstraint->pParticleA->position);
		this->cleanZeros(pCurConstraint->pParticleB->position);

	}//for (sConstraint* pCurConstraint...

	return;
}

void cSoftBodyVerlet::cleanZeros(glm::vec3& value)
{
	// 1.192092896e–07F 
	const float minFloat = 1.192092896e-07f;
	if ((value.x < minFloat) && (value.x > -minFloat))
	{
		value.x = 0.0f;
	}
	if ((value.y < minFloat) && (value.y > -minFloat))
	{
		value.y = 0.0f;
	}
	if ((value.z < minFloat) && (value.z > -minFloat))
	{
		value.z = 0.0f;
	}
}

// Michael's original 2016 code from the original Hitman article on gamasutra:
// from the satisfyConstraints() method
//for (int itCount = 0; itCount != 10; itCount++)
////for ( int itCount = 0; itCount != this->numIterations; itCount++ )
//{
//	int numConstraints = this->m_vecConstraints.size();
//	for (int index = 0; index != numConstraints; index++)
//	{
//
//		// Apply constraints...
//		if (index > static_cast<int>(this->m_vecConstraints.size()))
//		{
//			int x = 0;
//		}
//		CConstraint& c = this->m_vecConstraints[index];
//		// 
//		unsigned int indexA = c.particleA;
//		unsigned int indexB = c.particleB;
//
//		if (indexA > this->m_numParticles)
//		{
//			int stop = 0;
//		}
//		if (indexB > this->m_numParticles)
//		{
//			int stop = 0;
//		}
//
//		CVector& x1 = this->m_vec_x[indexA];
//		CVector& x2 = this->m_vec_x[indexB];
//		CVector delta = x2 - x1;
//		float deltaMag = delta.Magnitude();
//		float deltalength = sqrt(deltaMag * deltaMag);
//
//		// Speed up by placing this on separate lines... why? who knows?
//		// (Actually, I think it's because of some wonkly bug in the CVector3f...)
//		//float diff=( deltalength - c.restlength ) / deltalength;
//
//		float diffA = (deltalength - c.restlength);
//		float diff = diffA / deltalength;
//
//		// Making this non-one, will change how quickly the objects move together
//		// For example, making this < 1.0 will make it "bouncier"
//		float tightnessFactor = 1.0f;
//
//		x1 += delta * 0.5 * diff * tightnessFactor;
//		x2 -= delta * 0.5 * diff * tightnessFactor;
//
//		x1.CleanZero();
//		x2.CleanZero();
//	}
//}// for ( int itCount = 0; itCount != this->numIterations; 
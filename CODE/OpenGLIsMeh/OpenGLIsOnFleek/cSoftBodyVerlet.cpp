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
bool cSoftBodyVerlet::CreateSoftBody(sModelDrawInfo ModelInfo, glm::mat4 matInitalTransform /*=glm::mat4(1.0f)*/ )
{
	// Copy the model info
	this->m_ModelVertexInfo = ModelInfo;
	// Copy the vertices and indices as well
	// The cModelDrawInfo has pointers to the originally loaded vertices.
	sVertex* pVerticesLocalCopy = new sVertex[ModelInfo.numberOfVertices];
	for (unsigned int index = 0; index != ModelInfo.numberOfVertices; index++)
	{
		// TODO: If we are taking these vertices, apply some soft boy tranform, 
		//	then updating the original VAO buffers (which we ARE going to do),
		//	then we have to be aware that this will CHANGE the original VAO.
		// In other words, if we have TWO bouncy soft bunnies, we will
		//	have to load TWO meshes into the VAO...because the soft body
		//	physics will update each one. 
		glm::vec4 theVertex = glm::vec4(ModelInfo.pVertices[index].x,
										ModelInfo.pVertices[index].y,
										ModelInfo.pVertices[index].z, 1.0f);
		// Apply transformation
		theVertex = matInitalTransform * theVertex;

		pVerticesLocalCopy[index].x = theVertex.x;
		pVerticesLocalCopy[index].y = theVertex.y;
		pVerticesLocalCopy[index].z = theVertex.z;
		pVerticesLocalCopy[index].w = ModelInfo.pVertices[index].w;

//		pVerticesLocalCopy[index].x = ModelInfo.pVertices[index].x;
//		pVerticesLocalCopy[index].y = ModelInfo.pVertices[index].y;
//		pVerticesLocalCopy[index].z = ModelInfo.pVertices[index].z;
//		pVerticesLocalCopy[index].w = ModelInfo.pVertices[index].w;

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

		// This is the actual Verlet integration step (notice there isn't a velocity)
		pCurrentParticle->position += (current_pos - old_pos) + (this->acceleration * (float)(deltaTime * deltaTime) );

		pCurrentParticle->old_position = current_pos;

		// Check if there is a LARGE different between old and new positions


		this->cleanZeros(pCurrentParticle->position);
		this->cleanZeros(pCurrentParticle->old_position);
	}

	return;
}

void cSoftBodyVerlet::ApplyCollision(double deltaTime)
{
	// HACK: Stop any particles that go below the "ground"
//	for (sParticle* pCurrentParticle : vec_pParticles)
//	{
//		if ( pCurrentParticle->position.y < 0.0f )
//		{
//			pCurrentParticle->position.y = 0.0f;
//		}
//	}

//	this->vec_pParticles[5'000]->position = glm::vec3(0.0f, 30.0f, 0.0f);

	// Collide with a sphere at 20 units above the origin
	//	with a radius of 5 units.
	// Check to see if this particle is inside this sphere...
	for (sParticle* pCurrentParticle : vec_pParticles)
	{
		glm::vec3 sphereCentre = glm::vec3(0.0f, 20.0f, 24.0f);
		float sphereRadius = 15.0f;

		float distanceToSphere = glm::distance(pCurrentParticle->position,
											   sphereCentre);
		if (distanceToSphere < sphereRadius )
		{
			// it's 'inside' the sphere
			// Shift or slide the point along the ray from the centre of the sphere
			glm::vec3 particleToCentreRay = pCurrentParticle->position - sphereCentre;
			// Normalize to get the direction
			particleToCentreRay = glm::normalize(particleToCentreRay);
			// 
			pCurrentParticle->position = (particleToCentreRay * sphereRadius) + sphereCentre;

		}
	}//for (sParticle* pCurrentParticle



	return;
}


void cSoftBodyVerlet::SatisfyConstraints(void)
{
	const unsigned int NUM_ITERATIONS = 5	;
	
	for ( unsigned int iteration = 0; iteration != NUM_ITERATIONS; iteration++ )
	{
		// This is ONE pass of the constraint resolution
		for (sConstraint* pCurConstraint : this->vec_pConstraints )
		{
			if ( pCurConstraint->bIsActive )
			{


				cSoftBodyVerlet::sParticle* pX1 = pCurConstraint->pParticleA;
				cSoftBodyVerlet::sParticle* pX2 = pCurConstraint->pParticleB;
		
				glm::vec3 delta = pX2->position - pX1->position;

				float deltaLength = glm::length(delta);


				float diff = (deltaLength - pCurConstraint->restLength) / deltaLength;

				// If we were having this 'tear' or break apart, 
				//	you could check some maximum length and if it's 'too long'
				//	then the constraint 'breaks'
				// Handle this by:
				// - Setting a bool (where it doesn't check the constraint any more)
				// - Remove the constraint (but removing from a vector is sketchy...)

//				if ( diff > 0.1f )
//				{
//					pCurConstraint->bIsActive = false;
//				}

				// Making this non-one, will change how quickly the objects move together
				// For example, making this < 1.0 will make it "bouncier"
				float tightnessFactor = 1.0f;

				pX1->position += delta * 0.5f * diff * tightnessFactor;
				pX2->position -= delta * 0.5f * diff * tightnessFactor;

				this->cleanZeros(pX1->position);
				this->cleanZeros(pX2->position);
			}//if (pCurConstraint->bIsActive)

		}//for (sConstraint* pCurConstraint...
	}//for ( unsigned int iteration

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



void cSoftBodyVerlet::CreateRandomBracing(unsigned int numberOfBraces,
										  float minDistanceBetweenVertices)
{
	for ( unsigned int count = 0; count != numberOfBraces; count++ )
	{
		// Assume the distance is OK
		bool bKeepLookingForParticles = false;

		do 
		{
			// Assume the distance is OK
			bKeepLookingForParticles = false;

			// Pick two random vertices
			// NOTE: Here, rand() might not be great because there's usually
			//	onlly about 32,000 possible integer values...
			// Meaning that if your are chosing from something LARGER than
			//	around 32,000, you'll miss a bunch of values. 
			// HOWEVER, you could also multiply rand() by itself
			unsigned int particleIndex1 = rand() % this->vec_pParticles.size();
			unsigned int particleIndex2 = rand() % this->vec_pParticles.size();

			sParticle* pParticle1 = this->vec_pParticles[particleIndex1];
			sParticle* pParticle2 = this->vec_pParticles[particleIndex2];
			float distBetween = this->calcDistanceBetween(pParticle1, pParticle2);

			// Distance OK?
			if ( distBetween < minDistanceBetweenVertices )
			{
				// No
				bKeepLookingForParticles = true;
			}
			else
			{
				// Distance is OK, so make a constraint
				sConstraint* pBracingConstraint = new sConstraint();
				pBracingConstraint->pParticleA = pParticle1;
				pBracingConstraint->pParticleB = pParticle2;
				pBracingConstraint->restLength = this->calcDistanceBetween(pBracingConstraint->pParticleA, pBracingConstraint->pParticleB);
			
				this->vec_pConstraints.push_back(pBracingConstraint);
			}

		} while (bKeepLookingForParticles);

	}

	return;
}

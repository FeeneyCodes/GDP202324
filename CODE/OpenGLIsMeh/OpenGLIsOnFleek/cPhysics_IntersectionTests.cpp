#include "cPhysics.h"

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 

bool cPhysics::m_Sphere_Sphere_IntersectionTest(sPhsyicsProperties* pSphereA, sPhsyicsProperties* pShpereB)
{
	// TODO: Insert amazing code here 
	return false;	
}

bool cPhysics::m_Sphere_Plane_IntersectionTest(sPhsyicsProperties* pSphere, sPhsyicsProperties* pPlane)
{
	// TODO: Insert amazing code here 
	return false;
}

bool cPhysics::m_Sphere_Triangle_IntersectionTest(sPhsyicsProperties* pSphere, sPhsyicsProperties* pTriangle)
{
	// TODO: Insert amazing code here 
	return false;
}

bool cPhysics::m_Sphere_AABB_IntersectionTest(sPhsyicsProperties* pSphere, sPhsyicsProperties* pAABB)
{
	// TODO: Insert amazing code here 
	return false;
}

bool cPhysics::m_Sphere_Capsule_IntersectionTest(sPhsyicsProperties* pSphere, sPhsyicsProperties* pCapsule)
{
	// TODO: Insert amazing code here 
	return false;
}

bool cPhysics::m_Sphere_TriMeshIndirect_IntersectionTest(sPhsyicsProperties* pSphere_General, sPhsyicsProperties* pTriMesh_General)
{

	// Do we have a mesh manager? 
	if ( ! this->m_pMeshManager )
	{
		return false;
	}

	// Does the physics object have a mesh object associated? 
	if (!pTriMesh_General->pTheAssociatedMesh)
	{
		return false;
	}

	// Get the info about this shape, specifically
	sPhsyicsProperties::sMeshOfTriangles_Indirect* pTriangleMesh = 
						(sPhsyicsProperties::sMeshOfTriangles_Indirect*)(pTriMesh_General->pShape);

	sPhsyicsProperties::sSphere* pSphere = (sPhsyicsProperties::sSphere*)(pSphere_General->pShape);

	sModelDrawInfo theMeshDrawInfo;

	// Find the raw mesh information from the VAO manager
	if ( ! this->m_pMeshManager->FindDrawInfoByModelName(pTriangleMesh->meshName, theMeshDrawInfo) )
	{
		// Didn't find it
		return false;
	}
	

//	glm::vec3 closestPointToTriangle = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	float closestDistanceSoFar = FLT_MAX;
	glm::vec3 closestTriangleVertices[3] = { glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f) };
	glm::vec3 closestContactPoint = glm::vec3(0.0f);
	unsigned int indexOfClosestTriangle = INT_MAX;


	// We now have the mesh object location and the detailed mesh information 
					// Which triangle is closest to this sphere (right now)
	for (unsigned int index = 0; index != theMeshDrawInfo.numberOfIndices; index += 3)
	{
		glm::vec3 verts[3];

		// This is TERRIBLE for cache misses
		verts[0].x = theMeshDrawInfo.pVertices[theMeshDrawInfo.pIndices[index]].x;
		verts[0].y = theMeshDrawInfo.pVertices[theMeshDrawInfo.pIndices[index]].y;
		verts[0].z = theMeshDrawInfo.pVertices[theMeshDrawInfo.pIndices[index]].z;

		verts[1].x = theMeshDrawInfo.pVertices[theMeshDrawInfo.pIndices[index + 1]].x;
		verts[1].y = theMeshDrawInfo.pVertices[theMeshDrawInfo.pIndices[index + 1]].y;
		verts[1].z = theMeshDrawInfo.pVertices[theMeshDrawInfo.pIndices[index + 1]].z;

		verts[2].x = theMeshDrawInfo.pVertices[theMeshDrawInfo.pIndices[index + 2]].x;
		verts[2].y = theMeshDrawInfo.pVertices[theMeshDrawInfo.pIndices[index + 2]].y;
		verts[2].z = theMeshDrawInfo.pVertices[theMeshDrawInfo.pIndices[index + 2]].z;

		// Transform this object in world space using the same technique we did to render it
		// 
		// Here's the key line: 	
		// 
		//		vertexWorldPos = matModel * vec4( vPos.xyz, 1.0f);
		// 
		// THIS BLOCK OF CODE IS FROM DrawObject()

		glm::mat4 matModel = glm::mat4(1.0f);

		// Translation
		glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
												glm::vec3(pTriMesh_General->position.x,
														  pTriMesh_General->position.y,
														  pTriMesh_General->position.z));

		   // Rotation matrix generation
//		glm::mat4 matRotateX = glm::rotate(glm::mat4(1.0f),
//										   pTriMesh_General->orientation.x,
//										   glm::vec3(1.0f, 0.0, 0.0f));
//
//
//		glm::mat4 matRotateY = glm::rotate(glm::mat4(1.0f),
//										   pTriMesh_General->orientation.y,
//										   glm::vec3(0.0f, 1.0, 0.0f));
//
//		glm::mat4 matRotateZ = glm::rotate(glm::mat4(1.0f),
//										   pTriMesh_General->orientation.z,
//										   glm::vec3(0.0f, 0.0, 1.0f));
		glm::mat4 matRotation = glm::mat4(pTriMesh_General->get_qOrientation());

		// Scaling matrix
//		glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
//										glm::vec3(pTriMesh->scale,
//												  pTheGround->scale,
//												  pTheGround->scale));
		--------------------------------------------------------------

		// Combine all these transformation
		matModel = matModel * matTranslate;

//		matModel = matModel * matRotateX;
//		matModel = matModel * matRotateY;
//		matModel = matModel * matRotateZ;

		matModel = matModel * matRotation;

//		matModel = matModel * matScale;


		// vertexWorldPos = matModel * vec4(vPos.xyz, 1.0f);

		glm::vec4 vertsWorld[3];
		vertsWorld[0] = (matModel * glm::vec4(verts[0], 1.0f));
		vertsWorld[1] = (matModel * glm::vec4(verts[1], 1.0f));
		vertsWorld[2] = (matModel * glm::vec4(verts[2], 1.0f));

		// And make sure you multiply the normal by the inverse transpose
		// OR recalculate it right here! 

		// ******************************************************

		glm::vec3 thisTriangleClosestPoint = this->m_ClosestPtPointTriangle(pSphere_General->position,
																	vertsWorld[0], vertsWorld[1], vertsWorld[2]);

		// Is this the closest so far
		float distanceToThisTriangle = glm::distance(thisTriangleClosestPoint, pSphere_General->position);

		if (distanceToThisTriangle < closestDistanceSoFar)
		{
			// this one is closer
			closestDistanceSoFar = distanceToThisTriangle;
			// Make note of the triangle index
			indexOfClosestTriangle = index;
			// 
			closestTriangleVertices[0] = vertsWorld[0];
			closestTriangleVertices[1] = vertsWorld[1];
			closestTriangleVertices[2] = vertsWorld[2];

			closestContactPoint = thisTriangleClosestPoint;
		}


	} //for ( unsigned int index...

	//    ____  _     _                  _     _ _     _        _                   _     ___ 
	//   |  _ \(_) __| | __      _____  | |__ (_) |_  | |_ _ __(_) __ _ _ __   __ _| | __|__ \
	//   | | | | |/ _` | \ \ /\ / / _ \ | '_ \| | __| | __| '__| |/ _` | '_ \ / _` | |/ _ \/ /
	//   | |_| | | (_| |  \ V  V /  __/ | | | | | |_  | |_| |  | | (_| | | | | (_| | |  __/_| 
	//   |____/|_|\__,_|   \_/\_/ \___| |_| |_|_|\__|  \__|_|  |_|\__,_|_| |_|\__, |_|\___(_) 
	//                                                                        |___/           

	if (closestDistanceSoFar < pSphere->radius)
	{
		// Hit it!
		// Take the normal of that triangle and bounce the sphere along it

		// How do we calculate the new direction after the "bounce"? 
		// 
		// Answer: it's based on the REFLECTION vector around the normal.
		// The sphere is travelling along its VELOCITY vector (at this moment)
		//	and will "bounce off" along an angle reflected by the normal

		// The object HAS PENETRATED the triangle
		// Instead of using the CURRENT location, 
		//	calculate everything based on the LAST location of the object. 
		// i.e. JUST BEFORE the object collided.
		
// Add whatever information we might need later to do something for the response.


		// Calculate the current "direction" vector 
		// We're using the velocity
		glm::vec3 sphereDirection = pSphere_General->velocity;
		// Normalize... 
		sphereDirection = glm::normalize(sphereDirection);

		// Calcualte the current normal from the TRANSFORMED vertices
		glm::vec3 edgeA = closestTriangleVertices[1] - closestTriangleVertices[0];
		glm::vec3 edgeB = closestTriangleVertices[2] - closestTriangleVertices[0];

		glm::vec3 triNormal = glm::normalize(glm::cross(edgeA, edgeB));

		// Calculate the reflection vector from the normal	
		// https://registry.khronos.org/OpenGL-Refpages/gl4/html/reflect.xhtml
		// 1st parameter is the "incident" vector
		// 2nd parameter is the "normal" vector
		glm::vec3 reflectionVec = glm::reflect(sphereDirection, triNormal);

		// Update the  velocity based on this reflection vector
		float sphereSpeed = glm::length(pSphere_General->velocity);
		glm::vec3 newVelocity = reflectionVec * sphereSpeed;

		pSphere_General->velocity = newVelocity;


		// We add this "collision event" to the list or queue of collisions
		sCollisionEvent theCollision;
		theCollision.pObjectA = pSphere_General;
		// 
		theCollision.contactPoint = closestContactPoint;
		theCollision.reflectionNormal = reflectionVec;
//		theCollision.velocityAtCollision = reflectionVec;
		
		// TODO: We'll have a problem later: what deletes this?
		sPhsyicsProperties* pTriangleWeHit = new sPhsyicsProperties();

		pTriangleWeHit->setShape(new sPhsyicsProperties::sTriangle(closestTriangleVertices[0],
																   closestTriangleVertices[1],
																   closestTriangleVertices[2]));
		theCollision.pObjectB = pTriangleWeHit;
		
		this->m_vecCollisionsThisFrame.push_back(theCollision);

// Or we 
//		pSphere_General->velocity = glm::vec3(0.0f);

		return true;
	}

	// Didn't hit
	return false;
}

bool cPhysics::m_Sphere_TriMeshLocal_IntersectionTest(sPhsyicsProperties* pSphere, sPhsyicsProperties* pTriMesh)
{
	// TODO: Insert amazing code here 
	return false;
}






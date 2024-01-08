struct sSphere
{
	float posXYZ[3];
	float radius;
};

struct sAABB
{
	// Store min and max extent
	float minExtent_or_Corner[3];
	float maxExtent_or_Corner[3];
};

bool Are_Sphere_AABB_Overlaping(sSphere, sAABB )
{
	// TODO: Amazing code here
	if ( /*did they overlap?*/ true )
	{
		return true;
	}

	return false;

}

sSphere SphereThatColides;

// Since this is recursive, we need to do two things:
// - What infomration are we returning (besides true or false)
bool MovingSphere_AABB_Overlap(sSphere theSphere, float start[3], float end[3], sAABB theAABB, 
							   sSphere &SphereThatColides, int callCount )
{
	callCount++;
	// How deep do you want this to go?
	// 5 is 2^5 or 32x the framerate that we originally had 2000 HZ
	// 6 --> 4000 Hz
	// 8 --> 8000 Hz
	if ( callCount >= 5 )  
	{
		return false;
	}

	// 1. calculate the large sphere from start and end location
	sSphere largeSphere;
	largeSphere.posXYZ = (end - start)/2.0f;
	largeSphere.radius = (end - largeSphere.posXYZ) + theSphere.radius;

	// 2. See if that sphere overlaps the AABB
	if (Are_Sphere_AABB_Overlaping(largeSphere, theAABB) )
	{
		// We ARE coliding
		// Calculate 2 smaller spheres and call MovingSphere_AABB_Overlap recursively()
		if ( MovingSphere_AABB_Overlap(theSphere, ? , ? , theAABB) )
		{
		}
		else
		{
			if ( MovingSphere_AABB_Overlap(theSphere, ? , ? , theAABB) )
			{
			}
		}
	}

	// Nope 
	return false;
}
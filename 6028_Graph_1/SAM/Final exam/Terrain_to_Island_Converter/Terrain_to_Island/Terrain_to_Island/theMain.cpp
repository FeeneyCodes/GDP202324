#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "glm/glm.hpp"
#include "glm/vec3.hpp"


bool ReadToToken( std::string token, std::ifstream &theFile )
{
	std::string curToken;
	while ( theFile >> curToken )
	{
		if ( curToken == token )
		{
			return true;
		}
	};
	return false;
}

float getMax( float A, float B )
{
	if ( A > B ) 
	{
		return A;
	}
	return B;
}

float getMin( float A, float B )
{
	if ( A < B )
	{
		return A;
	}
	return B;
}

struct sSphere
{
	glm::vec3 c;
	float r;
};
typedef glm::vec3 Point;
typedef glm::vec3 Vector;

bool IntersectRaySphere(Point p, Vector d, sSphere s, float &t, Point &q);

struct sLimits
{
	glm::vec3 minXYZ;
	glm::vec3 maxXYZ;
	glm::vec3 extentXYZ;
	float maxExtent;
};

void CalcExtents( std::vector<glm::vec3> &vecVerts, sLimits &limits );


int main(int argc, char* argv[])
{
	std::cout << "Notes:" << std::endl;
	std::cout << "-> The assumption is Y is height, so in MeshLab, " << std::endl;
	std::cout << "   rotate by -90 degrees along the x axis." << std::endl;
	std::cout << "-> The ply model should NOT have normals, so" << std::endl;
	std::cout << "   you will have to regenerated them in MeshLab." << std::endl;
	std::cout << std::endl;

	if ( (argc < 2) || (argc > 4 ) )
	{
		std::cout << "Usage: program.exe source.ply dest.ply waterHeightRatio" << std::endl;
		std::cout << "(waterHeightRatio is from 0.0 to 1.0 in relation to the height)" << std::endl;
		std::cout << std::endl;
		return -1;
	}

	float waterHeightRatio = 0.5f;		// Default
	std::string outputFileName = "output.ply";

	if ( argc == 2 )
	{
		std::cout << "Usage: program.exe source.ply dest.ply waterHeightRatio" << std::endl;
		std::cout << "(waterHeightRatio is from 0.0 to 1.0 in relation to the height)" << std::endl;
		std::cout << std::endl;
		std::cout << "Assuming output file is: output.ply" << std::endl;
		std::cout << "Assuming waterHeightRatio = 0.5" << std::endl;
	}
	else if ( argc == 3 ) 
	{
		std::cout << "Usage: program.exe source.ply dest.ply waterHeightRatio" << std::endl;
		std::cout << "(waterHeightRatio is from 0.0 to 1.0 in relation to the height)" << std::endl;
		std::cout << std::endl;
		std::cout << "Assuming waterHeightRatio = 0.5" << std::endl;
		outputFileName.clear();
		outputFileName.append( argv[2] );
	}
	else if ( argc == 4 )  
	{
		outputFileName.clear();
		outputFileName.append( argv[2] );
		waterHeightRatio = (float)atof( argv[3] );
	}


	std::cout << "Input file: " << argv[1] << std::endl;
	std::cout << "Output file: " << outputFileName << std::endl;
	std::cout << "waterHeightRatio: " << waterHeightRatio << std::endl;
	std::cout << std::endl;

	std::ifstream inFile(argv[1]);
	if ( ! inFile.is_open() )
	{
		std::cout << "Can't open file: " << argv[1] << std::endl;
		return -1;
	}

	ReadToToken( "vertex", inFile );
	unsigned int numVerts;
	inFile >> numVerts;

	ReadToToken( "face", inFile );
	unsigned int numFaces;
	inFile >> numFaces;

	ReadToToken( "end_header", inFile );

	std::vector<glm::vec3> vecVerts;
	std::vector<glm::ivec3> vecTris;

	std::cout << "Loading file...";

	for ( unsigned int index = 0; index != numVerts; index++ )
	{
		glm::vec3 tempV;
		inFile 
			>> tempV.x >> tempV.y >> tempV.z;
		vecVerts.push_back( tempV );
	}

	for ( unsigned int index = 0; index != numFaces; index++ )
	{
		glm::ivec3 tempTri;
		int garbage;
		inFile >> garbage >> tempTri.x >> tempTri.y >> tempTri.z;
		vecTris.push_back(tempTri);
	}

	inFile.close();


	std::cout << "OK" << std::endl;


	// Find extents
	sLimits origLimits;
	CalcExtents( vecVerts, origLimits );


	// Stage 1: Pick the water height (which is the y axis)
	float waterHeightRelative = waterHeightRatio * origLimits.extentXYZ.y;
	float waterHeightAbsolute = waterHeightRelative + origLimits.minXYZ.y;
	
	std::cout << "Lowest height = " << origLimits.minXYZ.y << std::endl;
	std::cout << "Highest height = " << origLimits.maxXYZ.y << std::endl;
	std::cout << "Water heigth:" << std::endl;
	std::cout << "\tRelative: " << waterHeightRelative << std::endl;
	std::cout << "\tAbsolute: " << waterHeightAbsolute << std::endl;

	// Shift the model to the centre...
	glm::vec3 shiftToCentreXYZ;
	shiftToCentreXYZ.x = -((origLimits.extentXYZ.x / 2.0f) + origLimits.minXYZ.x);
	shiftToCentreXYZ.y = -waterHeightAbsolute;
	shiftToCentreXYZ.z = -((origLimits.extentXYZ.z / 2.0f) + origLimits.minXYZ.z);

	std::cout << std::endl;
	std::cout << "Shifting the model to the origin:" << std::endl;
	std::cout << "\t" << shiftToCentreXYZ.x << ", " << shiftToCentreXYZ.y << ", " << shiftToCentreXYZ.z << std::endl;
	std::cout << std::endl;
	for ( unsigned int index = 0; index != numVerts; index++ )
	{
		vecVerts[index] += shiftToCentreXYZ;
	}

	// Reajust extents...
	// Assume 1st vertex is both max and min (for now)
	sLimits adjLimits;
	CalcExtents( vecVerts, adjLimits );

	// Flattening any terrain below water level...
	std::cout << "Flattening anything below sealevel... ";
	unsigned int numVertsFlattened = 0;
	for ( unsigned int index = 0; index != numVerts; index++ )
	{
		if ( vecVerts[index].y < 0.0f )
		{
			vecVerts[index].y = 0.0f;
			numVertsFlattened++;
		}
	}
	std::cout << numVertsFlattened << " vertices set to y=0" << std::endl;


	// Step 2: Determine if height is outside of sphere
	sSphere theSphere;
	theSphere.c.x = (adjLimits.extentXYZ.x / 2.0f) + adjLimits.minXYZ.x;
	theSphere.c.y = -(adjLimits.maxExtent / 2.0f);
	theSphere.c.z = (adjLimits.extentXYZ.z / 2.0f) + adjLimits.minXYZ.z;

	theSphere.r = adjLimits.maxExtent * 0.70f;

	std::cout << "Rounding terrain... ";
	unsigned int numVertsScaled = 0;
	for ( unsigned int index = 0; index != numVerts; index++ )
	{
		if ( vecVerts[index].y > 0.0f )
		{
			// // Intersects ray r = p + td, |d| = 1, with sphere s and, if intersecting,
			// returns t value of intersection and intersection point q

			glm::vec3 rayStart = vecVerts[index];
			rayStart.y = origLimits.maxExtent;
			glm::vec3 rayDirection = glm::vec3(0.0f, -1.0f, 0.0f);
			float t = origLimits.maxExtent; 
			glm::vec3 intersectionPoint;

			if ( IntersectRaySphere( rayStart, rayDirection, theSphere, t, intersectionPoint ) )
			{
				// Scale height based on curve of sphere
				//float quaterMaxExtent = maxExtent/4.0f;

				//float ratio = (intersectionPoint.y / quaterMaxExtent);
				float ratio = intersectionPoint.y / origLimits.extentXYZ.y;

				vecVerts[index].y *= ratio;
//				vecVerts[index].y = intersectionPoint.y;
			}
			else
			{
				vecVerts[index].y = 0.0f;
			}
		}

		// Check if height is LT 0.0f
		if ( vecVerts[index].y < 0.0f )
		{
			vecVerts[index].y = 0.0f;
		}
	}
	std::cout << "done." << std::endl;


	sLimits resLimits;
	CalcExtents( vecVerts, resLimits );
	float ratioAdjust = origLimits.extentXYZ.y / resLimits.extentXYZ.y;
	for ( unsigned int index = 0; index != numVerts; index++ )
	{
		vecVerts[index].y *= ratioAdjust;
	}


	std::cout << "Outputting to file " << outputFileName << "...";
	std::ofstream outFile( outputFileName.c_str() );
	if ( ! outFile.is_open() )
	{
		std::cout << "Can't open file for writing." << std::endl;
		return -1;
	}

	outFile << "ply" << std::endl;
	outFile << "format ascii 1.0" << std::endl;
	outFile << "comment VCGLIB generated" << std::endl;
	outFile << "element vertex "<< numVerts << std::endl;
	outFile << "property float x" << std::endl;
	outFile << "property float y" << std::endl;
	outFile << "property float z" << std::endl;
	outFile << "element face " << numFaces << std::endl;
	outFile << "property list uchar int vertex_indices" << std::endl;
	outFile << "end_header" << std::endl;

	for ( unsigned int index = 0; index != numVerts; index++ )
	{
		outFile 
			<< vecVerts[index].x << " " 
			<< vecVerts[index].y << " " 
			<< vecVerts[index].z << std::endl;
	}

	for ( unsigned int index = 0; index != numFaces; index++ )
	{
		outFile 
			<< "3 " 
			<< vecTris[index].x << " "
			<< vecTris[index].y << " "
			<< vecTris[index].z << std::endl;
	}

	outFile.close();

	return 0;
}

// Intersects ray r = p + td, |d| = 1, with sphere s and, if intersecting,
// returns t value of intersection and intersection point q
typedef glm::vec3 Point;
typedef glm::vec3 Vector;

bool IntersectRaySphere(Point p, Vector d, sSphere s, float &t, Point &q)
{
	Vector m = p - s.c;

    float b = glm::dot(m, d);
	float c = glm::dot(m, m) - s.r * s.r;
    // Exit if r’s origin outside s (c > 0)and r pointing away from s (b > 0)
    if (c > 0.0f && b > 0.0f) 
	{
		return false;
	}
    float discr = b*b - c;
    // A negative discriminant corresponds to ray missing sphere
    if (discr < 0.0f) 
	{
		return false;
	}
    // Ray now found to intersect sphere, compute smallest t value of intersection
    t = -b - sqrt(discr);
    // If t is negative, ray started inside sphere so clamp t to zero
    if (t < 0.0f)
	{
		t = 0.0f;
	}
	else
	{
		t = t;
	}
    q = p + t * d;
    return true;
}

void CalcExtents( std::vector<glm::vec3> &vecVerts, sLimits &limits )
{
	// max and min are 1st vertes
	limits.minXYZ = limits.maxXYZ = vecVerts[0];

	for ( unsigned int index = 0; index != vecVerts.size(); index++ )
	{
		limits.minXYZ.x = getMin( vecVerts[index].x, limits.minXYZ.x );
		limits.minXYZ.y = getMin( vecVerts[index].y, limits.minXYZ.y );
		limits.minXYZ.z = getMin( vecVerts[index].z, limits.minXYZ.z );

		limits.maxXYZ.x = getMax( vecVerts[index].x, limits.maxXYZ.x );
		limits.maxXYZ.y = getMax( vecVerts[index].y, limits.maxXYZ.y );
		limits.maxXYZ.z = getMax( vecVerts[index].z, limits.maxXYZ.z );
	}

	limits.extentXYZ.x = limits.maxXYZ.x - limits.minXYZ.x;
	limits.extentXYZ.y = limits.maxXYZ.y - limits.minXYZ.y;		// height 
	limits.extentXYZ.z = limits.maxXYZ.z - limits.minXYZ.z;

	limits.maxExtent = limits.extentXYZ.x;
	limits.maxExtent = getMax( limits.extentXYZ.y, limits.maxExtent );
	limits.maxExtent = getMax( limits.extentXYZ.z, limits.maxExtent );

	return;
}


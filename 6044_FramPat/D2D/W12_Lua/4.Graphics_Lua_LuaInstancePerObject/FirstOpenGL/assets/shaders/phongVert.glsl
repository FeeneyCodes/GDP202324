#version 400

// VAO layout (vertex attribute layout)
in vec3 vColour;		
in vec3 vPosition;	
in vec3 vNormal;
in vec3 vTexSTU;		// ADDED

uniform mat4 matModel;	// "per object" 
uniform mat4 matView;	// "per scene" or "per frame"
uniform mat4 matProj;	// "Projection" (only if size or aspect changes)

uniform vec4 objectColour;
uniform int isWireframe;		

const int NUMBEROFLIGHTS = 1;

struct sLightDesc
{
	vec4 position;		// In the world
	vec4 diffuse;		// 1,1,1 (white colour light)
	vec4 attenuation;	// x = const, y = linear, z = quad, w = specular ratio
	vec4 specular;		// xyz = colour of the highlight, w is specular power
	                    // "shininess"  0.0, realy 1.0 to large, like 1000+
	vec4 direction;		// For directional and spot (ignored by others)
	vec4 typeAndParams;	// To be used soon. Specify type and attributes of type
						// x = type: 0 = point, 1 = spot, 2 = directional, 3 =hemispherical 
						// y = for Spot: cos cutoff start (start of penumbra, edge of bright spot)
						// z = for Spot: cos cutoff end (end of penumbra, beyond this it's dark)
						
};

uniform sLightDesc theLights[NUMBEROFLIGHTS];

// For spec:
uniform vec3 eyeLocation;

out vec4 diffuseColour;	
out vec3 vertMV;
out vec3 vertWorld;
out vec3 vertNorm;
//out vec3 viewVector;
out vec3 lightVector[NUMBEROFLIGHTS];		// For Phong
out vec3 lightHalfVector[NUMBEROFLIGHTS];	// For Blinn-Phong
out vec3 text_STU;	// ADDED, texture coordinates (also called UV)

// VERTEX shader
void main()
{
	mat4 matMV = matView * matModel;	// model-view matrix
	mat4 matMVP = matProj * matView * matModel;		// m = p * v * m;
	
	vertMV = vec3(matMV * vec4(vPosition.xyz, 1.0f)).xyz;
	
	// The "normal" matrix. The normal without scaling or tranlation
	// (So this vector is extending from the origin, in the direction 
	//  of the vertex normal)
	// Since the vertex normal only needs rotation, we only 
	//  need to use a 3x3 matrix (the "top-left" portion of the 4x4 matrix)
	mat4 matNormal = inverse( transpose(matModel) );
//	mat3 matNomral = mat3(matModel);
	vertNorm = mat3(matNormal) * normalize(vNormal.xyz);
	
	// Position of the vertex in the world
	// (i.e. without View or Projection matrix
	vertWorld = vec3(matModel * vec4(vPosition.xyz, 1.0f));
		
	// To calcualte the specular, we determine the rays 
	//  cast from the vertex (in "world" space) to the
	//  light position. 
	// In the fragment shader, we also calculate the 
	//  "eye vector", which is the ray from the vertex 
	//  in "world" space to the eye (or camera). 
	// Since both rays are cast from the vertex, this 
	//  essentially makes the vertex position 0,0,0
	//  (from the perspective of of the vertex),
	//  which is the same place the vertNormal is cast
	//  from (because the matrix we use there, the 
	//  "normal matrix" doesn't include translation or 
	//  scaling (i.e. it stays at the origin, too)
		
	// "Light Vector", or the ray from the vertex
	//  in "world" space and the light (also in "world" space)
	// Note that this is NOT normalized here, as we need 
	//  to calculate the distance to the vertex in the fragment shader
	for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
	{
		lightVector[index].xyz = theLights[index].position.xyz - vertWorld.xyz;
	}
	

	// Model+View+Projection matrix. 
	// i.e. the location of the pixel in screen space
	gl_Position = matMVP * vec4(vPosition.xyz, 1.0f);
	
	
	if ( isWireframe == 1 )  
	{
		diffuseColour = objectColour;
	}
	else  
	{	
		diffuseColour = vec4(vColour, 1.0f);
	}
	
	// Copy the texture coordinates to the fragment shader
	text_STU = vTexSTU;
	
}
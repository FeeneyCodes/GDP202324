#version 420
// Geometry shader
layout(triangles) 			in;
layout(triangle_strip)		out;
layout(max_vertices = 9)	out;

// From the vertex shader
in vec4 gColour[];				// Indexed starting at 0
in vec4 gVertexWorldPos[];	
in vec4 gVertexWorldNormal[];
in vec2 gTextureCoords[];		

// Going to the fragment shader
out vec4 fColour;
out vec4 fVertexWorldPos;	
out vec4 fVertexWorldNormal;
out vec2 fTextureCoords;	

uniform bool bIsSkyBox;

void main()
{
	// Calculate the normal from this triangle
	vec3 triEdgeA = gVertexWorldPos[2].xyz - gVertexWorldPos[0].xyz;
	vec3 triEdgeB = gVertexWorldPos[1].xyz - gVertexWorldPos[0].xyz;
	
	vec3 newNormal = normalize( cross(triEdgeB, triEdgeA) );	


	fColour = gColour[0];				
	fVertexWorldPos = gVertexWorldPos[0];	
	if ( bIsSkyBox )
	{
		// DON'T regenerate the normals
		fVertexWorldNormal = gVertexWorldNormal[0];
	}
	else
	{
		fVertexWorldNormal = vec4(newNormal, 1.0f);	// gVertexWorldNormal[0];
	}
	fTextureCoords = gTextureCoords[0];		
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	fColour = gColour[1];				
	fVertexWorldPos = gVertexWorldPos[1];	
	if ( bIsSkyBox )
	{
		// DON'T regenerate the normals
		fVertexWorldNormal = gVertexWorldNormal[1];
	}
	else
	{
		fVertexWorldNormal = vec4(newNormal, 1.0f);	// gVertexWorldNormal[1];
	}
	fTextureCoords = gTextureCoords[1];		
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	fColour = gColour[2];				
	fVertexWorldPos = gVertexWorldPos[2];	
	if ( bIsSkyBox )
	{
		// DON'T regenerate the normals
		fVertexWorldNormal = gVertexWorldNormal[2];
	}
	else
	{
		fVertexWorldNormal = vec4(newNormal, 1.0f);	// gVertexWorldNormal[2];
	}
	fTextureCoords = gTextureCoords[2];		
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();


}
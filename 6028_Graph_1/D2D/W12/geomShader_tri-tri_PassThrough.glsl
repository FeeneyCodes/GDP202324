#version 420
// Geometry shader
layout(triangles) 			in;
layout(triangle_strip)		out;
layout(max_vertices = 3)	out;

// Coming in from the vertex shader
in vec4 gColour[];		// colour;			
in vec4 gNormal[];
in vec4 gVertWorldLocation[];	// using only matModel
in vec4 gUVx2[];
in vec4 gTangent[];
in vec4 gBinormal[];

// Going out to the fragment shader
out vec4 fColour;		// colour;			
out vec4 fNormal;
out vec4 fVertWorldLocation;	// using only matModel
out vec4 fUVx2;
out vec4 fTangent;
out vec4 fBinormal;

void main()
{

	gl_Position = gl_in[0].gl_Position;
	fColour = gColour[0];		// colour;			
	fNormal = gNormal[0];
	fVertWorldLocation = gVertWorldLocation[0];	// using only matModel
	fUVx2 = gUVx2[0];
	fTangent = gTangent[0];
	fBinormal = gBinormal[0];
	EmitVertex();
	
	gl_Position = gl_in[1].gl_Position;
	fColour = gColour[1];		// colour;			
	fNormal = gNormal[1];
	fVertWorldLocation = gVertWorldLocation[1];	// using only matModel
	fUVx2 = gUVx2[1];
	fTangent = gTangent[1];
	fBinormal = gBinormal[1];
	EmitVertex();
	
	gl_Position = gl_in[2].gl_Position;
	fColour = gColour[2];		// colour;			
	fNormal = gNormal[2];
	fVertWorldLocation = gVertWorldLocation[2];	// using only matModel
	fUVx2 = gUVx2[2];
	fTangent = gTangent[2];
	fBinormal = gBinormal[2];
	EmitVertex();
	
	EndPrimitive();
}
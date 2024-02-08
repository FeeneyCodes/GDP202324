// Vertex shader
#version 420

//uniform mat4 MVP;
//uniform mat4 matView;
uniform mat4 matView[6];
uniform mat4 matProjection;
uniform mat4 matModel;			// Later...
uniform mat4 matModel_IT;		// Inverse transpose of the model matrix

//uniform vec3 modelScale;
//uniform vec3 modelOffset;

in vec4 vPos[6];		
in vec4 vCol;
in vec4 vNormal;	
in vec2 vTextureCoords;		// NOTE this is a vec2 not vec4

//out vec4 fColour;
//out vec4 fVertexWorldPos;	
//out vec4 fVertexWorldNormal;
//out vec2 fTextureCoords;		// ADDED

out vec4 gColour;
out vec4 gVertexWorldPos;	
out vec4 gVertexWorldNormal;
out vec2 gTextureCoords;		// ADDED

uniform bool bUseHeightMap;
uniform sampler2D heightMapSampler;		// Texture unit 20
uniform float heightScale;
uniform vec2 UVOffset;

void main()
{
	vec4 vertexModelPosition = vPos;	

	vec2 UVFinal = vTextureCoords.st;
	
	if ( bUseHeightMap )
	{
		// Greyscale (black and white) heightmap image
		// NvF5e_height_map.bmp
//		vec3 heightRGB = texture( heightMapSampler, vTextureCoords.st ).rgb;
//		float height = (heightRGB.r + heightRGB.g + heightRGB.b) / 3.0f;
		UVFinal += UVOffset.yx;
		float height = texture( heightMapSampler, UVFinal.st ).r;
		
		// Adjust the height of the y axis from this "colour"
//		const float SCALEADJUST = 25.0f;
//		vertexModelPosition.y += ( height * SCALEADJUST );
		vertexModelPosition.y += ( height * heightScale );
	}
	
	
//	gl_Position = MVP * vec4(finalPos, 1.0);
//	gl_Position = MVP * vertModelPosition;
	mat4 matMVP = matProjection * matView * matModel;
	gl_Position = matMVP * vec4(vertexModelPosition.xyz, 1.0);
	
		
	// Rotate the normal by the inverse transpose of the model matrix
	// (so that it only is impacted by the rotation, not translation or scale)
	gVertexWorldNormal = matModel_IT * vec4(vNormal.xyz, 1.0f);
	gVertexWorldNormal.xyz = normalize(gVertexWorldNormal.xyz);
	gVertexWorldNormal.w = 1.0f;
	
	gVertexWorldPos = matModel * vec4( vertexModelPosition.xyz, 1.0f);
	
	gColour = vCol;
	
	// Copy the UV coordinates unchanged (to the fragment shader)
//	textureCoords = vTextureCoords;
	gTextureCoords = UVFinal;
}

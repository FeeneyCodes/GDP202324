// Fragment shader
#version 420

in vec4 fColour;
in vec4 fVertexWorldPos;			// vertex in "world space"
in vec4 fVertexWorldNormal;	
in vec2 fTextureCoords;			// u is x or red, v is y or green

// These are listed in order, starting from zero
out vec4 outputColour;			// GL_COLOR_ATTACHMENT0      // To the frame buffer (aka screen)

out vec4 cubeMapOutput_Left;
out vec4 cubeMapOutput_Right;
...

//out vec4 finalLightColour_and_Brightness;
//out vec4 vertexNormal;
//out vec4 motionSOmeting_Spec_SOmething;
//out vec4 vertexColour;
//
//out vec4 vertexWorldPositionXYZ_;
//out vec4 vertexWorldNormal;
//out vec4 vertexSpecular_and_Power;
//out vec4 vertexFinalColourRGB;


//uniform vec3 directionalLightColour;
// rgb are the rgb of the light colour
//uniform vec4 directionalLight_Direction_power;
// xyz is the normalized direction, w = power (between 0 and 1)

// If true, then passes the colour without calculating lighting
uniform bool bDoNotLight;		// Really a float (0.0 or not zero)

uniform vec4 eyeLocation;

uniform bool bUseDebugColour;	// if this is true, then use debugColourRGBA for the colour
uniform vec4 debugColourRGBA;		

// If FALSE, we use the texture colour as the vertex colour
// (NOT the one from the model file)
uniform bool bUseVertexColours;		// If true, then DOESN'T use texture colours

// Usually, you would pass the alpha transparency as the 4th colour value, 
// 	but for clarity, we'll pass it separately...
uniform float transparencyAlpha;

uniform sampler2D texture_00;			// 2D meaning x,y or s,t or u,v
uniform sampler2D texture_01;
uniform sampler2D texture_02;
uniform sampler2D texture_03;
uniform sampler2D texture_04;			
uniform sampler2D texture_05;
uniform sampler2D texture_06;
uniform sampler2D texture_07;

//
uniform bool bUseHeightMap;
uniform sampler2D heightMapSampler;		// Texture unit 20
uniform sampler2D discardSampler;		// Texture unit 21

// Skybox, cubemap, etc.
uniform bool bIsSkyBox;
uniform samplerCube skyBoxTexture;

// For the discard example
uniform bool bUseDiscardMaskTexture;
uniform sampler2D maskSamplerTexture01;

// From the FBO
uniform bool bIsOffScreenTextureQuad;
uniform sampler2D textureOffScreen;

//... and so on
//uniform float textureMixRatio[8];
uniform vec4 textureMixRatio_0_3;		// 1, 0, 0, 0 
uniform vec4 textureMixRatio_4_7;

//uniform samplerCube skyBox;

struct sLight
{
	vec4 position;			
	vec4 diffuse;	// Colour of the light (used for diffuse)
	vec4 specular;	// rgb = highlight colour, w = power
	vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 direction;	// Spot, directional lights
	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	                // 0 = pointlight
					// 1 = spot light
					// 2 = directional light
	vec4 param2;	// x = 0 for off, 1 for on
};

const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;

const int NUMBEROFLIGHTS = 10;
uniform sLight theLights[NUMBEROFLIGHTS];  	// 70 uniforms
//... is really:
//uniform vec4 theLights[0].position;
//uniform vec4 theLights[1].position;
//uniform vec4 theLights[2].position;
// etc...



vec4 calculateLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular );


void main()
{
//	gl_FragColor = vec4(color, 1.0);

//	if ( bUseHeightMap )
//	{
//		outputColour.rgba = vec4(1.0f, 0.0f, 0.0f, 1.0f);
//		return;
//	}

	// Discard the water
//	if ( bUseHeightMap )
//	{
//		// Range of 0 to 1
//		float height = texture( heightMapSampler, textureCoords.st ).r;
//		
//		if ( height <= 0.005f )
//		{
//			discard;
//		}
//	}

	// Offscreen quad
	if ( bIsOffScreenTextureQuad )
	{
//		outputColour.rgb = vec3(1.0f, 0.0f, 0.0f);
		vec3 theColour = texture( textureOffScreen, fTextureCoords.st ).rgb;
		
		// I can do cool here
		
		outputColour.rgb = theColour;
						   
		outputColour.a = 1.0f;
		return;

//		outputColour.rgb = theColour;
		
//		vec2 textCoordsScreen = vec2( gl_FragCoord.x / 1920, gl_FragCoord.y  / 1080 );
//		vec3 theColour = texture( textureOffScreen, textCoordsScreen.st ).rgb;

// 'night vision' 
//		float brightness = ( theColour.r + theColour.g + theColour.b ) / 3.0f;
//		outputColour.rgb = vec3( 0.0f, brightness, 0.0f );

//		.....
//		..x..
//		.....

		theColour.r = texture( textureOffScreen, 
		                       vec2( fTextureCoords.s + 0.01f, 
							         fTextureCoords.t + 0.01f ) ).r;
		
		theColour.b = texture( textureOffScreen, 
		                       vec2( fTextureCoords.s - 0.01f, 
							         fTextureCoords.t + 0.05f ) ).b;		
							   
		theColour.g = texture( textureOffScreen, 
		                       vec2( fTextureCoords.s - 0.015f, 
							         fTextureCoords.t - 0.06f  ) ).g;	
									 
		
		

		outputColour.rgb = theColour;
						   
		outputColour.a = 1.0f;
		return;
	}


	if ( bUseDiscardMaskTexture )
	{
		float maskValue = texture( maskSamplerTexture01, fTextureCoords.st ).r;
		// If "black" then discard
		if ( maskValue < 0.1f )
		{
			discard;
//			outputColour.rgba = vec4( 1.0f, 0.0f, 0.0f, 1.0f );
//			return;
		}	
	}
	
	if ( bIsSkyBox )
	{
		//uniform samplerCube skyBoxTexture;
		vec4 skyBoxSampleColour = texture( skyBoxTexture, fVertexWorldNormal.xyz ).rgba;
		outputColour.rgb = skyBoxSampleColour.rgb;
		outputColour.a = 1.0f;
		return;
	}
	
	// Reflect
	// Reflects based on the eye position
	// genType reflect( genType IncidentAngle, 	genType Nnormal);
//	vec3 eyeVector = normalize(eyeLocation.xyz - vertexWorldPos.xyz);
//	vec3 reflectAngle = reflect( eyeVector, vertexWorldNormal.xyz);
//	vec3 reflectAngle = refract( eyeVector, vertexWorldNormal.xyz, 	1.333);
//	
//	vec4 skyBoxSampleColour = texture( skyBoxTexture, reflectAngle.xyz ).rgba;
	
//	outputColour.rgb = skyBoxSampleColour.rgb;
//	outputColour.a = 1.0f;
//	return;
	

	vec4 textureColour = 
			  texture( texture_00, fTextureCoords.st ).rgba * textureMixRatio_0_3.x 	
			+ texture( texture_01, fTextureCoords.st ).rgba * textureMixRatio_0_3.y
			+ texture( texture_02, fTextureCoords.st ).rgba * textureMixRatio_0_3.z
			+ texture( texture_03, fTextureCoords.st ).rgba * textureMixRatio_0_3.w;

	// Make the 'vertex colour' the texture colour we sampled...
	vec4 vertexRGBA = textureColour;	
	
	// ...unless we want to use the vertex colours from the model
	if (bUseVertexColours)
	{
		// Use model vertex colour and NOT the texture colour
		vertexRGBA = fColour;
	}
	
	if ( bUseDebugColour )
	{	
		vertexRGBA = debugColourRGBA;
	}
	
	if ( bDoNotLight )
	{
		outputColour = vertexRGBA;
		return;
	}
	
	// *************************************
	// Hard code the specular (for now)
	vec4 vertexSpecular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	// xyzw or rgba or stuw
	// RGB is the specular highglight colour (usually white or the colour of the light)
	// 4th value is the specular POWER (STARTS at 1, and goes to 1000000s)
	
	vec4 vertexColourLit = calculateLightContrib( vertexRGBA.rgb, fVertexWorldNormal.xyz, 
	                                              fVertexWorldPos.xyz, vertexSpecular );
	// *************************************
			
	outputColour.rgb = vertexColourLit.rgb;
	
	// Real gamma correction is a curve, but we'll Rock-n-Roll it here
	outputColour.rgb *= 1.35f;
	
	// This is where the alpha transparency happens
//	outputColour.a = 1.0f;
	outputColour.a = transparencyAlpha;
	
	
	// DEBUG HACK
//	outputColour.rgb *= 0.0001f;
//	outputColour.rgb += vec3( textureCoords.st, 0.0f);
}


vec4 calculateLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular )
{
	vec3 norm = normalize(vertexNormal);
	
	vec4 finalObjectColour = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	
	for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
	{	
		// ********************************************************
		// is light "on"
		if ( theLights[index].param2.x == 0.0f )
		{	// it's off
			continue;
		}
		
		// Cast to an int (note with c'tor)
		int intLightType = int(theLights[index].param1.x);
		
		// We will do the directional light here... 
		// (BEFORE the attenuation, since sunlight has no attenuation, really)
		if ( intLightType == DIRECTIONAL_LIGHT_TYPE )		// = 2
		{
			// This is supposed to simulate sunlight. 
			// SO: 
			// -- There's ONLY direction, no position
			// -- Almost always, there's only 1 of these in a scene
			// Cheapest light to calculate. 

			vec3 lightContrib = theLights[index].diffuse.rgb;
			
			// Get the dot product of the light and normalize
			float dotProduct = dot( -theLights[index].direction.xyz,  
									   normalize(norm.xyz) );	// -1 to 1

			dotProduct = max( 0.0f, dotProduct );		// 0 to 1
		
			lightContrib *= dotProduct;		
			
			finalObjectColour.rgb += (vertexMaterialColour.rgb * theLights[index].diffuse.rgb * lightContrib); 
									 //+ (materialSpecular.rgb * lightSpecularContrib.rgb);
			// NOTE: There isn't any attenuation, like with sunlight.
			// (This is part of the reason directional lights are fast to calculate)


			return finalObjectColour;		
		}
		
		// Assume it's a point light 
		// intLightType = 0
		
		// Contribution for this light
		vec3 vLightToVertex = theLights[index].position.xyz - vertexWorldPos.xyz;
		float distanceToLight = length(vLightToVertex);	
		vec3 lightVector = normalize(vLightToVertex);
		float dotProduct = dot(lightVector, vertexNormal.xyz);	 
		
		dotProduct = max( 0.0f, dotProduct );	
		
		vec3 lightDiffuseContrib = dotProduct * theLights[index].diffuse.rgb;
			

		// Specular 
		vec3 lightSpecularContrib = vec3(0.0f);
			
		vec3 reflectVector = reflect( -lightVector, normalize(norm.xyz) );

		// Get eye or view vector
		// The location of the vertex in the world to your eye
		vec3 eyeVector = normalize(eyeLocation.xyz - vertexWorldPos.xyz);

		// To simplify, we are NOT using the light specular value, just the object’s.
		float objectSpecularPower = vertexSpecular.w; 
		
//		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
//			                   * vertexSpecular.rgb;	//* theLights[lightIndex].Specular.rgb
		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
			                   * theLights[index].specular.rgb;
							   
		// Attenuation
		float attenuation = 1.0f / 
				( theLights[index].atten.x + 										
				  theLights[index].atten.y * distanceToLight +						
				  theLights[index].atten.z * distanceToLight*distanceToLight );  	
				  
		// total light contribution is Diffuse + Specular
		lightDiffuseContrib *= attenuation;
		lightSpecularContrib *= attenuation;
		
		
		// But is it a spot light
		if ( intLightType == SPOT_LIGHT_TYPE )		// = 1
		{	
		

			// Yes, it's a spotlight
			// Calcualate light vector (light to vertex, in world)
			vec3 vertexToLight = vertexWorldPos.xyz - theLights[index].position.xyz;

			vertexToLight = normalize(vertexToLight);

			float currentLightRayAngle
					= dot( vertexToLight.xyz, theLights[index].direction.xyz );
					
			currentLightRayAngle = max(0.0f, currentLightRayAngle);

			//vec4 param1;	
			// x = lightType, y = inner angle, z = outer angle, w = TBD

			// Is this inside the cone? 
			float outerConeAngleCos = cos(radians(theLights[index].param1.z));
			float innerConeAngleCos = cos(radians(theLights[index].param1.y));
							
			// Is it completely outside of the spot?
			if ( currentLightRayAngle < outerConeAngleCos )
			{
				// Nope. so it's in the dark
				lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
				lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
			}
			else if ( currentLightRayAngle < innerConeAngleCos )
			{
				// Angle is between the inner and outer cone
				// (this is called the penumbra of the spot light, by the way)
				// 
				// This blends the brightness from full brightness, near the inner cone
				//	to black, near the outter cone
				float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) / 
									  (innerConeAngleCos - outerConeAngleCos);
									  
				lightDiffuseContrib *= penumbraRatio;
				lightSpecularContrib *= penumbraRatio;
			}
						
		}// if ( intLightType == 1 )
		
		
					
		finalObjectColour.rgb += (vertexMaterialColour.rgb * lightDiffuseContrib.rgb)
								  + (vertexSpecular.rgb  * lightSpecularContrib.rgb );

	}//for(intindex=0...
	
	finalObjectColour.a = 1.0f;
	
	return finalObjectColour;
}


//	// For now, just trust Michael
//	// Very basic directional shader
//	vec3 lightContrib = directionalLightColour * directionalLight_Direction_power.w;
//	// 
//	// Get the dot product of the light and normalize
//	float dotProduct = dot( -directionalLight_Direction_power.xyz,  
//							vertexWorldNormal.xyz );	
//	// Clamp this to a positive number
//	dotProduct = max( 0.0f, dotProduct );		// 0 to 1		
//	
//	lightContrib *= dotProduct;		
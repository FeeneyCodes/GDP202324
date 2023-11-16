#include <glad/glad.h>		
#include <GLFW/glfw3.h>		
#include <iostream>

#include "GlobalStuff.h"

//#include "linmath.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stdio.h>
#include <sstream>		// "String Stream", used for setting the windows title

#include <vector>

#include "vert_XYZ_RGB.h"


#include "Utilities.h"

#include <limits.h>



void PhysicsStep(float deltaTime);
void CollisionStep(float deltaTime);

bool LoadObjectsIntoScene( std::vector< cGameObject* > &vec_pGOs );

// Function that Lua can call
int MakeTheCowsBigger(lua_State* L)
{
	//float scaleAdjust = 3.0f;
	float scaleAdjust = (float)lua_tonumber( L, 1 ); 


	for (std::vector< cGameObject* >::iterator itGO = ::g_vec_pGOs.begin();
		 itGO != ::g_vec_pGOs.end(); itGO++)
	{
		if ((*itGO)->meshName == "cow_N.ply")
		{
			// It's a cow!
			(*itGO)->scale *= scaleAdjust;
		}

	}
	
	//	          4.1234567891235 e+17
	long long p = 412345678912345678;  // Bigger than 4 billion

	// Try and push a 64 bit number to Lua
	lua_pushnumber(L, p);

	// How many things got returned on the stack
	return 1;
}



static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


void DrawSkyBox(void)
{
	// Draw skybox
	// Place the skybox at the same location as the camera 'eye'
	::g_pSkyBoxObject->x = ::g_CameraX;
	::g_pSkyBoxObject->y = ::g_CameraY;
	::g_pSkyBoxObject->z = ::g_CameraZ;
	// There are a number of other things you could do with this, for example:
	// Turn off the depth buffer, and the writing to the depth buffer, 
	//  then draw the sky box FIRST. Then turn on the depth buffer and 
	//  continue to draw the rest of the scene. 
	// This case is more straight-forward, in that we simply make the sky box
	//  an appropriate size for the sky (so big) and be careful not to 
	//  draw anything "behind" it.

	glUniform1i( UniformLoc_bIsCubeMapObject, TRUE );

	// Bind the appriprate texture unit... 
	GLuint texNumberSky = ::g_pTextureManager->getTextureIDFromName("SkyMap");
	// We'll pick a higher value texture unit. 
	// There are at least 80 of them, so we'll pick the last one...
	// (Note the GL_TEXTUREX values only go up to 31, though)
	const unsigned int TEXTURE_UNIT_80_MAGIC_NUMBER = 80;
	glActiveTexture( TEXTURE_UNIT_80_MAGIC_NUMBER + GL_TEXTURE0 );		// GL_TEXTURE0 = 33984

//	glBindTexture( GL_TE, texNumberSky );
	glBindTexture( GL_TEXTURE_2D, texNumberSky );

	glUniform1i( UniformLoc_myCubemapSkyBoxSampler, texNumberSky );

	glDisable( GL_DEPTH );			// turn off depth test (i.e. just write)
	glDepthMask( GL_FALSE );		// no writing to z or depth buffer
	DrawObject( ::g_pSkyBoxObject, glm::mat4x4(1.0f), false );
	glDepthMask( GL_TRUE );
	glEnable( GL_DEPTH );

	glUniform1i( UniformLoc_bIsCubeMapObject, FALSE );

	return;
}





std::vector< cParticleEmitter::cParticle > g_vecParticlesToRender;

int main(void)
{
    GLFWwindow* window;
	// Added an "index_buffer" variable here
    //GLuint vertex_buffer, index_buffer, vertex_shader, fragment_shader, program;
	//GLuint vertex_shader, fragment_shader; //, program;
//    GLint mvp_location;//, vpos_location, vcol_location;

	//GLint locID_matModel = -1;		// 
	//GLint locID_matView = -1;
	//GLint locID_matProj = -1;

	GLint locID_lightPosition = -1;
	GLint locID_lightDiffuse = -1;
	GLint locID_lightAttenuation = -1;
	GLint locID_lightDirection = -1;
	GLint locID_lightTypeAndParams = -1;



    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(1200, 800, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);


	std::cout << glGetString(GL_VENDOR) << " " 
		<< glGetString(GL_RENDERER) << ", " 
		<< glGetString(GL_VERSION) << std::endl;
	std::cout << "Shader language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

//	std::cout << glGetString(GL_EXTENSIONS) << std::endl;

	::g_pTheShaderManager = new cShaderManager();

	cShaderManager::cShader verShader;	
	cShaderManager::cShader fragShader;

	verShader.fileName = "simpleVert.glsl";
	fragShader.fileName = "simpleFrag.glsl";
	//verShader.fileName = "phongVert.glsl";
	//fragShader.fileName = "phongFrag.glsl";
	::g_pTheShaderManager->setBasePath( "assets//shaders//" );
	if ( ! ::g_pTheShaderManager->createProgramFromFile( "simple", verShader, fragShader ) )
	{	// Oh no!
		std::cout << ::g_pTheShaderManager->getLastError() << std::endl;
		std::cout.flush();
		// TODO: Shut it all down... 
		return -1;
	}



	// Shader loading happening before vertex buffer array
 //   vertex_shader = glCreateShader(GL_VERTEX_SHADER);

 ////  char* vertex_shader_text = "wewherlkherlkh";

 //   glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
 //   glCompileShader(vertex_shader); //???? 

 //   fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
 //   glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
 //   glCompileShader(fragment_shader);

 //   program = glCreateProgram();
 //   glAttachShader(program, vertex_shader);
 //   glAttachShader(program, fragment_shader);
 //   glLinkProgram(program);

	// Entire loading the model and placing ito the VAOs 
	// is now in the cMeshTypeManager
	::g_pTheMeshTypeManager = new cMeshTypeManager();


	// Note: this returns a bool, which you might want to check...
	// (We won't right now, though)
	GLuint shadProgID = ::g_pTheShaderManager->getIDFromFriendlyName("simple");
	::g_pTheMeshTypeManager->SetBaseFilePath( "assets//models//" );

	::g_pTheMeshTypeManager->ScaleEverythingToBoundingBoxOnLoad( true, 10.0f );

		// Objects we can use for imposters
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "1x1_2Tri_Quad_1_Sided_XY_Plane.ply" );
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "1x1_2Tri_Quad_2_Sided_XY_Plane.ply" );


	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "bun_zipper_res3_XYZ_N.ply");
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "su47_XYZ_N.ply");					// "su47_XYZ.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "dolphin_XYZ_N.ply");				// "dolphin.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Sphere_N.ply");					// "Sphere.ply");
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Ground_XYZ_N_Rot_Y_is_up.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Sphere_InvertedNormals.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"

	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Isoshphere.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Isoshphere_xyz_InvertedNormals.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "cow_N.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "tie_withNormals.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "bun_zipper_N.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "bun_zipper_res2_N.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "bun_zipper_res4_N.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Cube_1x1x1_N.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Cube2.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Utah_Teapot.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"

	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Seafloor2_N.ply" );

//	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "dragon_vrip_res2_N.ply" );

	

//	struct vertXYRGB
//{
//    float x, y (z);				// 4 + 4 
//    float r, g, b;			// 4 + 4 + 4  = 20 bytes
//};

	// Create a particle emitter
	::g_pParticles = new cParticleEmitter();

	cParticleEmitter::sParticleDesc emitDescript;
	// Change stuff in the description, perhaps?
	emitDescript.lifetimeMin = 2.0f;
	emitDescript.lifetimeMax = 4.0f;
	emitDescript.particlesCreatedPerUpdate = 20;
	// You would change this "force" to be in the direction
	// you wanted to travel (or whatever)
	emitDescript.ongoingForce = glm::vec3( 0.0f, 0.5f, 0.0f );

	emitDescript.initVelMin = glm::vec3( -0.2f,  0.0f,  -0.2f);
	emitDescript.initVelMax = glm::vec3(  0.2f,   1.0f,  0.2f);


	::g_pParticles->allocateParticles( 2000, emitDescript /*, true*/);



	::g_pTextureManager = new cBasicTextureManager();

	::g_pTextureManager->SetBasePath("assets/textures");
	if ( ! ::g_pTextureManager->Create2DTextureFromBMPFile( "brickTexture_square_1024pixels.bmp", true ) )
	{
		std::cout << "WARNING: Didn't load texture" << std::endl;
	}
	else
	{
		std::cout << "Texture loaded OK" << std::endl;
	}

	if ( ! ::g_pTextureManager->Create2DTextureFromBMPFile( "trudeau_transparency_20140611.bmp", true ) )
	{
		std::cout << "WARNING: Didn't load smoketex.bmp texture" << std::endl;
	}
	else
	{
		std::cout << "Texture loaded OK" << std::endl;
	}

	::g_pTextureManager->Create2DTextureFromBMPFile( "FenceActual.bmp", true );
	::g_pTextureManager->Create2DTextureFromBMPFile( "FenceAlphaMask.bmp", true );


	// Clear any gl errors
	GLenum ignoreerorr = glGetError();

	::g_pTextureManager->SetBasePath( "assets/textures/skybox");
	GLenum glErrorEnum = 0;
	std::string errorString, errorDetails;
	if ( ! ::g_pTextureManager->CreateNewCubeTextureFromBMPFiles( "SkyMap", 
														   "SpaceBox_right1_posX.bmp", 
														   "SpaceBox_left2_negX.bmp", 
														   "SpaceBox_top3_posY.bmp", 
														   "SpaceBox_bottom4_negY.bmp", 
														   "SpaceBox_front5_posZ.bmp", 
														   "SpaceBox_back6_negZ.bmp", 
														   true, glErrorEnum,
														   errorString, 
														   errorDetails ) )
	{
		std::cout << "Trouble loading skybox" << std::endl;
		std::cout << "Error: " << errorString << std::endl;
		std::cout << "Details: " << errorDetails << std::endl;
	}
														   

	// Shader loading and building was here...

//    mvp_location = glGetUniformLocation(shadProgID, "MVP");

	// glGetActiveUniforms... 
	locID_matModel = glGetUniformLocation(shadProgID, "matModel");
	locID_matView = glGetUniformLocation(shadProgID, "matView");
	locID_matProj = glGetUniformLocation(shadProgID, "matProj");


	locID_lightPosition = glGetUniformLocation(shadProgID, "theLights[0].position");
	locID_lightDiffuse = glGetUniformLocation(shadProgID, "theLights[0].diffuse");
	locID_lightAttenuation = glGetUniformLocation(shadProgID, "theLights[0].attenuation");// = 0.0f;		// Can set to 1.0 to "turn off" a light

	locID_lightDirection = glGetUniformLocation(shadProgID, "theLights[0].direction");
	locID_lightTypeAndParams = glGetUniformLocation(shadProgID, "theLights[0].typeAndParams");

	UniformLoc_textureMod = glGetUniformLocation(shadProgID, "textureMod");;
	UniformLoc_matTexMod = glGetUniformLocation(shadProgID, "matTexMod");;


	if ( ! LoadObjectsIntoScene( ::g_vec_pGOs ) )
	{
		std::cout << "WARNING: Could not load all models into the scene." << std::endl;
	}


	//GLuint UniformLoc_ID_objectColour = 0;
	//GLuint UniformLoc_ID_isWireframe = 0;

	UniformLoc_ID_objectColour = glGetUniformLocation( shadProgID, "objectColour" );
	UniformLoc_ID_isWireframe = glGetUniformLocation( shadProgID, "isWireframe" );
	UniformLoc_ID_bUseDebugDiffuseNoLighting = glGetUniformLocation( shadProgID, "bUseDebugDiffuseNoLighting" );

	UniformLoc_alpha = glGetUniformLocation( shadProgID, "alpha" );

	UniformLoc_texSamp2D_00 = glGetUniformLocation( shadProgID, "texSamp2D_00" );
	UniformLoc_texSamp2D_01 = glGetUniformLocation( shadProgID, "texSamp2D_01" );
	UniformLoc_texSamp2D_02 = glGetUniformLocation( shadProgID, "texSamp2D_02" );
	UniformLoc_texSamp2D_03 = glGetUniformLocation( shadProgID, "texSamp2D_03" );

	UniformLoc_bUseTextureAsDiffuse = glGetUniformLocation( shadProgID, "bUseTextureAsDiffuse" );
	UniformLoc_bUseLighting = glGetUniformLocation( shadProgID, "bUseLighting" );

	UniformLoc_bIsImposterBlackIsAlpha  = glGetUniformLocation( shadProgID, "bIsImposterBlackIsAlpha" );
	
	UniformLoc_bIsCubeMapObject = glGetUniformLocation( shadProgID, "bIsCubeMapObject" );
	UniformLoc_myCubemapSkyBoxSampler = glGetUniformLocation( shadProgID, "myCubemapSkyBoxSampler" );


	// Note that GLFW would have had to create a "depth buffer" also
	// (which it does, apparently...)
	glEnable(GL_DEPTH_TEST);


	// Create the Lua script handler
	::p_LuaScripts = new cLuaBrain();
	// Point to all the game objects
	// (We are passing a pointer to the vector of game object pointers... yeah)
	::p_LuaScripts->SetObjectVector( &(::g_vec_pGOs) );

	// A test script
	std::stringstream ssMoveTo;
//	ssMoveTo << "x = 1\n";
//	ssMoveTo << "print(x)\n" << std::endl;

	ssMoveTo << "r, x, y, z, vx, vy, vz = getObjectState(";
	ssMoveTo << ::g_pObjectToRotate->getID();
	ssMoveTo << ") \n";
	// New function
//	ssMoveTo << "JustinTrudeauIsOurPM(\"Justin\", 47, 3.14159) \n";
	//
//	ssMoveTo << "if ( r == true ) then \n";
//	ssMoveTo << "    print( \"xyz=\", x, \",\", y, \", \", z )  \n";
//	ssMoveTo << "end \n";

	ssMoveTo << " x = x - 0.01  \n";
	ssMoveTo << " y = y + 0.01  \n";
	ssMoveTo << "setObjectState(";
	ssMoveTo << ::g_pObjectToRotate->getID();
	ssMoveTo << ", x, y, z, Vx, Vy, Vz ) \n";

	std::cout << ssMoveTo.str() << std::endl;

	::p_LuaScripts->LoadScript( "MoveToOBJ17", ssMoveTo.str() );
	::p_LuaScripts->LoadScript( "MoveToOBJ18", ssMoveTo.str() );

	::p_LuaScripts->RunThis(
		"a, b = shakeHandsWithALeglessLizard('Michael', 3.14, 17)\n " \
		"print(a)\n" \
		"print(b)"	
	);

	double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
//        mat4x4 m, p, mvp;
		glm::mat4x4 matProjection;		// Was just "p"

		glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);

		// 1st one clears the "colour" buffer (i.e. the colour of the pixels)
		// 2nd one clears the "depth" or "Z" buffer 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// The "view" matrix is (usually) only updated once per "scene"
		// The "projection" matrix only needs to be updated when the window size 
		//    changes, but here we are updating it every "scene"
		// (a "scene" is when ALL the game objects are drawn)

		glm::mat4x4 matView(1.0f);	// "View" (or camera) matrix

//		mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		matProjection = glm::perspective( 0.6f, ratio, 0.01f, 10000.0f );

		//v = glm::lookAt( glm::vec3( 0.0f, 0.0f, -10.0f), // Eye
		matView = glm::lookAt( 
			             glm::vec3( ::g_CameraX, ::g_CameraY, ::g_CameraZ ), // Eye
						 glm::vec3( ::g_TargetX, ::g_TargetY, ::g_TargetZ ), // At (target)
						 glm::vec3( 0.0f, 1.0f, 0.0f ) );	// Up


		DrawSkyBox();


//		const float TIMESTEPCONST = 0.01f;	// 10ms

		static const float MAXDELTATIME = 0.1f;	// 100 ms
		float deltaTime = static_cast<float>( glfwGetTime() - lastTime );
		lastTime = glfwGetTime();
		if ( deltaTime > MAXDELTATIME )
		{
			deltaTime = MAXDELTATIME;
		}

		// Physics update
		PhysicsStep( deltaTime );		// 10 ms
		// Collision detection step 
		CollisionStep(deltaTime);

		// Update the lua script manager
		::p_LuaScripts->Update( deltaTime );

		// Place the emitter at the light #0 location...
		::g_pParticles->position = lightPos;
		// Inidcate where the eye is (for the imposters)
		::g_pParticles->particleSettings.imposterEyeLocation.x = ::g_CameraX;
		::g_pParticles->particleSettings.imposterEyeLocation.y = ::g_CameraY;
		::g_pParticles->particleSettings.imposterEyeLocation.z = ::g_CameraZ;
		::g_pParticles->Update( deltaTime );


		// update the bunny rotation
//		::g_vec_pGOs[0]->pre_Rot_X += 0.01f;
//		::g_vec_pGOs[0]->post_Rot_X += 0.01f;

		// Only needs to be set if window is created and-or 
		// is resized (you could put this in the window re-size
		// callback in GLFW or freeGLUT)
		glUniformMatrix4fv(locID_matProj, 1, GL_FALSE, 
				    (const GLfloat*) glm::value_ptr(matProjection) );

		// This is set once at the start of the "scene" draw.
		glUniformMatrix4fv(locID_matView, 1, GL_FALSE, 
				    (const GLfloat*) glm::value_ptr(matView) );

		glUniform4f( locID_lightPosition, lightPos.x, lightPos.y, lightPos.z, 1.0f );
		glUniform4f( locID_lightDiffuse, 1.0, 1.0f, 1.0f, 1.0f );
		glUniform4f( locID_lightAttenuation, ::lightConstAtten, ::lightLinearAtten, ::lightQuadAtten, 1.0f );


//		DrawSkyBox();

		// Order the object by distance from the camera
		// Left for the students to do, for the time being...

		// Sort objects by distance from camera
		std::vector< cGameObject* > vecGO_Sorted;
		vecGO_Sorted.reserve( ::g_vec_pGOs.size() );
		// Copy to vector
		for ( std::vector<cGameObject*>::iterator itGO = ::g_vec_pGOs.begin(); itGO != ::g_vec_pGOs.end(); itGO++ )
		{
			vecGO_Sorted.push_back( *itGO );
		}

		// Do one pass of bubble sort, becuase the objects are likely to be in 
		// almost the correct order from frame to frame		
		glm::vec3 eye(::g_CameraX, ::g_CameraY, ::g_CameraZ);
		for ( int index = 0; index != vecGO_Sorted.size() - 1; index++ )
		{
			cGameObject* p1 = vecGO_Sorted[index];
			cGameObject* p2 = vecGO_Sorted[index+1];

			// Use glm distance 
			// (note, you can use a "squared distance" function
			//  that does not do the square root, as it's faster)
			glm::vec3 p1Pos(p1->x, p1->y, p1->z);
			glm::vec3 p2Pos(p2->x, p2->y, p2->z);

			if ( glm::distance( eye, p1Pos ) < glm::distance( eye, p2Pos ) )
			{	// p1 is closer than p2, but 
				// we are drawing from back to front, so switch them
				vecGO_Sorted[index] = p2;
				vecGO_Sorted[index+1] = p1;
			}

		}//for ( int index = 0

		// Copy order back to main vectro
		for ( int index = 0; index != vecGO_Sorted.size(); index++ )
		{
			::g_vec_pGOs[index] = vecGO_Sorted[index];
		}



// Start of Draw Scene
//		for ( int index = 0; index != ::g_vec_pGOs.size(); index++)
		for ( int index = 0; index != vecGO_Sorted.size(); index++)
		{
			//cGameObject* pCurGO = ::g_vec_pGOs[index];
			cGameObject* pCurGO = vecGO_Sorted[index];

			// Set identity matrix for all "parent" or "root" objects
			glm::mat4x4 matModel = glm::mat4x4(1.0f);

			// Draw an object.... 
			DrawObject( pCurGO, matModel );

		}// for ( int index = 0;.... (bottom of "render scene" loop)
// End of Draw Scene

//		// Draw the particles...
//		int numParticles = ::g_pParticles->getParticlesCopy( ::g_vecParticlesToRender );
//		for ( int index = 0; index != numParticles; index++ )
//		{
//			DrawQuad( true, 
//			          ::g_vecParticlesToRender[index].position, 
//					  ::g_vecParticlesToRender[index].rotation,
//					  "",					/* texture name */
//					  1.0f,					/* scale */
//					  glm::vec3( 1.0f, 1.0f, 0.0f ), false /*useDebugColours*/, 
//					  false /*isWireframe*/);
//		}


//		DrawSkyBox();


		// *********************************
		DrawDebugLightSpheres();
		// *********************************

	//// Adjust the transparency of the objects
	//	for ( std::vector<cGameObject*>::iterator itpGO = g_vec_pGOs.begin();
	//			itpGO != g_vec_pGOs.end(); itpGO++ )
	//	{
	//		(*itpGO)->alpha += 0.01f;
	//		// To high alpha?
	//		if ( (*itpGO)->alpha >= 1.0f )
	//		{	// Yes, so make it 0.0f
	//			(*itpGO)->alpha = 0.0f;
	//		}
	//	}



		// Show or "present" what we drew...
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

	// Bye bye...
	delete ::g_pTheMeshTypeManager;

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

float distanceBetweenSpheres( cGameObject* pBallA, cGameObject* pBallB )
{
	float deltaX = pBallA->x - pBallB->x;
	float deltaY = pBallA->y - pBallB->y;
	float deltaZ = pBallA->z - pBallB->z;

	return sqrt( deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);

}

float distanceBetweenPoints( glm::vec3 P1, glm::vec3 P2 )
{
	float deltaX = P1.x - P2.x;
	float deltaY = P1.y - P2.y;
	float deltaZ = P1.z - P2.z;

	return sqrt( deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);

}

// Narrow phase 
bool testSphereSphereCollision( cGameObject* pBallA, cGameObject* pBallB )
{
	float totalRadius = pBallA->radius + pBallB->radius;
	if ( distanceBetweenSpheres(pBallA, pBallB) <= totalRadius )
	{
		return true;
	}
	return false;
}

// Search by ID, then returns pointer to object
cGameObject* findObjectByID( unsigned int ID )
{
	for ( int index = 0; index != g_vec_pGOs.size(); index++ )
	{
		if ( ::g_vec_pGOs[index]->getID() == ID )
		{	// Found it! 
			return ::g_vec_pGOs[index];
		}
	}
	// Didn't find it
	return 0;
}

void CollisionStep(float deltaTime)
{
	// The detection phase:
	// 1. For each ball, check for collision with other balls
	//    --> if collision, add to "contact points" from perspective of "outer" ball
	// 2. For each ball, check for collision with triangles (environment)
	//    --> if collision, add to "contact points" from perspective of ball
	//
	// 3. Go throught the contact points, calculating response (sphere-sphere or sphere-triangle)
	// 4. Sort responses by object, calculate average resulting response
	//    --> update object


	// Clear the contact list
	g_vec_Contacts.clear();

	// Brute force narrow phase detection

	// 1. For each ball, check for collision with other balls
	for ( int indexOut = 0; indexOut != ::g_vec_pGOs.size(); indexOut++)
	{
		cGameObject* pOutBall = ::g_vec_pGOs[indexOut];	

		for ( int indexIn = 0; indexIn != ::g_vec_pGOs.size(); indexIn++)
		{
			cGameObject* pInBall = ::g_vec_pGOs[indexIn];	
	
			// Is this the same object
			if ( pOutBall != pInBall )
			{	// No, so we're good to check

				pOutBall->solid_B = pOutBall->solid_G = pOutBall->solid_R = 1.0f;
				pInBall->solid_B = pOutBall->solid_G = pOutBall->solid_R = 1.0f;

				if ( testSphereSphereCollision(pOutBall, pInBall) )
				{
					// TODO: Calculate cool math stuff... 
					// Add to a vector of contact points...
					cContact curContact;
					curContact.objectID = pOutBall->getID();

					::g_vec_Contacts.push_back( curContact );

				}//if ( testSphereSphereCollision(pOutBall, pInBall) )
			}
		}//for ( int indexIn = 0; 
	}//for ( int indexOut 
	// Colour all the balls white
	for ( int index = 0; index != ::g_vec_pGOs.size(); index++ )
	{
		cGameObject* pCurGO = ::g_vec_pGOs[index];
		// Maybe check for zero...?
		pCurGO->solid_R = 1.0f;
		pCurGO->solid_G = 1.0f;
		pCurGO->solid_B = 1.0f;
	}


	// 2. For each ball, check for collision with triangles (environment)
	for ( int indexOut = 0; indexOut != ::g_vec_pGOs.size(); indexOut++)
	{
		cGameObject* pOutBall = ::g_vec_pGOs[indexOut];	

		// You'll need to decide how you are handling the triangles...
		//for ( int indexIn = 0; indexIn != ::g_vec_Triangles.size(); indexIn++)
		//{
		//	cGameObject* pInBall = ::g_vec_pGOs[indexIn];	


		//}

	}


//			glm::reflect( velocity, normalOfTheTriangle );

	// Calculate reponse....
	if ( ! ::g_vec_Contacts.empty() )
	{
		int sexyBreakPoint = 0;

		for ( int index = 0; index != ::g_vec_Contacts.size(); index++ )
		{
			// Change colour of any balls that are touching...
			unsigned int ID = ::g_vec_Contacts[index].objectID;
			cGameObject* pCurGO = findObjectByID( ID );
			// Maybe check for zero...?
			pCurGO->solid_R = 1.0f;
			pCurGO->solid_G = 0.0f;
			pCurGO->solid_B = 0.0f;
		}
	}

	// This very trivial check will be replaced by something more fancy
	for ( int index = 0; index != ::g_vec_pGOs.size(); index++)
	{
		cGameObject* pCurGO = ::g_vec_pGOs[index];	

		if ( pCurGO->bIsUpdatedByPhysics )
		{
			float groundSurface = 0.0f;
			// Did the thingy go through the ground??? 
			if ( pCurGO->y <= ( ( groundSurface + pCurGO->radius) ) )
			{
				// It's hit the ground
				// Reverse the velocity. 
				pCurGO->Vy = -pCurGO->Vy;
				// Non-elastic collision (loses some energy)
				pCurGO->Vy = 0.0f;
			}
		}//if ( pCurShip->bIsUpdatedByPhysics )
	}
	return;
}
//
void PhysicsStep(float deltaTime)		// 24 Hz  30 Hz  60 Hz  100 Hz 60Hz  50,000 fps
{
	//glm::vec3 gravityForce(0.0f, -9.81f, 0.0f );
	glm::vec3 gravityForce(0.0f, 0.0f, 0.0f );


	// Rotational "speed" using SLERP of quaternion
	for ( int index = 0; index != ::g_vec_pGOs.size(); index++)
	{
		cGameObject* pGO = ::g_vec_pGOs[index];

		//pGO->post_Rot_X += pGO->rotSpeed.x * deltaTime;
		//pGO->post_Rot_Y += pGO->rotSpeed.y * deltaTime;
		//pGO->post_Rot_Z += pGO->rotSpeed.z * deltaTime;

		// Represents the change over 1 second.
		glm::quat qRotChangeZeroSeconds;		// No rotational change
		glm::quat qRotChange( pGO->rotSpeed );

		glm::quat qRotChangeThisFrame = glm::slerp( qRotChangeZeroSeconds, 
												    qRotChange, 
												    deltaTime );

		pGO->qOrientaion *= qRotChangeThisFrame;

	}


	// Euler... 
	for ( int index = 0; index != ::g_vec_pGOs.size(); index++)
	{
		// f = m*a
		// Velocity = 1 m per second
		// x = x + velocity*time
		// v = v + accel*time

		// each second 
		cGameObject* pCurGO = ::g_vec_pGOs[index];

		if ( pCurGO->bIsUpdatedByPhysics )
		{
			// Update velocity based on acceleration

			// This: 
			//		pCurGO->Vx = pCurGO->Vx + ( pCurGO->Ax * deltaTime ) ;
			//
			// Would typically be expressed as this:
			//		pCurGO->Vx += pCurGO->Ax * deltaTime;
			//
			// With the "+=" operator

			pCurGO->Vx += (pCurGO->Ax + gravityForce.x) * deltaTime;					
			pCurGO->Vy += (pCurGO->Ay + gravityForce.y) * deltaTime;
			pCurGO->Vz += (pCurGO->Az + gravityForce.z) * deltaTime;

			pCurGO->lastX = pCurGO->x;
			pCurGO->lastY = pCurGO->y;
			pCurGO->lastZ = pCurGO->z;

			// Update position based on velocity
			pCurGO->x += pCurGO->Vx * deltaTime; 
			pCurGO->y += pCurGO->Vy * deltaTime; 
			pCurGO->z += pCurGO->Vz * deltaTime;
		}//if ( pCurShip->bIsUpdatedByPhysics )
	}

	return;
}
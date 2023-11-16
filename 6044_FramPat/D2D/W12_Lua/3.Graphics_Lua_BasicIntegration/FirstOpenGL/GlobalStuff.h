#ifndef _GlobalStuff_HG_
#define _GlobalStuff_HG_

#include <glad/glad.h>		
#include <GLFW/glfw3.h>	

#include <vector>
#include "cGameObject.h"
#include "cMeshTypeManager.h"	
#include "cShaderManager.h"
#include "cContact.h"	// Object collision object
#include "cParticleEmitter.h"
#include "Texture/cBasicTextureManager.h"	// ADDED

#include "cLuaBrain.h"

extern void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// We should place this into a class...
extern float g_CameraX;
extern float g_CameraY;
extern float g_CameraZ;	
extern float g_CameraMovementSpeed;

extern float g_TargetX;
extern float g_TargetY;
extern float g_TargetZ;


extern glm::vec3 lightPos;		
extern float lightConstAtten;
extern float lightLinearAtten;
extern float lightQuadAtten;
extern glm::vec3 lightDirection;
extern float lightType;
extern float lightSpotCosCutoffStart;
extern float lightSpotCosCutoffEnd;

extern bool bDrawLightDebugSpheres;
extern bool bDrawLightDebugLocation;

extern GLint locID_matModel;		// 
extern GLint locID_matView;
extern GLint locID_matProj;

extern GLint UniformLoc_ID_objectColour;
extern GLint UniformLoc_ID_isWireframe;
extern GLint UniformLoc_ID_bUseDebugDiffuseNoLighting;

extern GLint UniformLoc_alpha;

extern GLint UniformLoc_texSamp2D_00;
extern GLint UniformLoc_texSamp2D_01;
extern GLint UniformLoc_texSamp2D_02;
extern GLint UniformLoc_texSamp2D_03;
extern GLint UniformLoc_bUseTextureAsDiffuse;
extern GLint UniformLoc_bUseLighting;

extern GLint UniformLoc_bIsImposterBlackIsAlpha;

// This was added to demonstrate controlling an object rotation.
// The "better" way would be to search for the object in the
//  vector of objects, and manipulate it. 
// But, if you only have one "playe" object, this might be OK, too...
extern cGameObject* g_pObjectToRotate;


// The skybox object isn't added to the regular vector of 
//  objects. Instead, it is called separately. 
// You could also add it to the "rendered objects" and 
//  add some kind of indication that it's a skybox...
extern cGameObject* g_pSkyBoxObject;
extern GLint UniformLoc_bIsCubeMapObject;
extern GLint UniformLoc_myCubemapSkyBoxSampler;


extern std::vector< cGameObject* > g_vec_pGOs;

extern std::vector< cContact > g_vec_Contacts;

extern cMeshTypeManager* g_pTheMeshTypeManager;

extern cShaderManager* g_pTheShaderManager;

extern cParticleEmitter* g_pParticles;

extern cBasicTextureManager* g_pTextureManager;

void DrawDebugLightSpheres(void);
void DrawDebugBall( glm::vec3 position, 
				    glm::vec3 colour, float scale );
void DrawQuad( bool bTwoSided, glm::vec3 position, glm::vec3 rotation, std::string texture, float scale,
			   glm::vec3 debugColour, bool bUseDebugColour, 
			   bool bIsWireFrame );

void DrawObject( cGameObject* pCurGO, 
				 glm::mat4x4 matModel,		// NOT passing by reference 
				 bool bUseLighting = true );

cGameObject* findObjectByID( unsigned int ID );

// For "good enough for rock-n-roll" texture changes
extern glm::vec4 g_textureMod;
extern glm::mat4 g_matTexMod;
extern GLint UniformLoc_textureMod;
extern GLint UniformLoc_matTexMod;


extern cLuaBrain* p_LuaScripts;

#endif

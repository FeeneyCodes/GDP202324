#include "GlobalStuff.h"

// We should place this into a class...
float g_CameraX = 6.9f;
float g_CameraY = 6.5f;
float g_CameraZ = 28.7f;	
float g_CameraMovementSpeed = 0.1f;

float g_TargetX = 0.0f;
float g_TargetY = 0.0f;
float g_TargetZ = 0.0f;


glm::vec3 lightPos(5.4f, 5.0f, 19.4f);		// I just picked these values after moving the light around until it looked "nice"
float lightConstAtten = 0.0f;
float lightLinearAtten = 0.0477747f;
float lightQuadAtten = 0.000683441f;
bool bDrawLightDebugSpheres = false;
bool bDrawLightDebugLocation = true;

GLint locID_matModel = -1;		// 
GLint locID_matView = -1;
GLint locID_matProj = -1;

GLint UniformLoc_ID_objectColour = -1;
GLint UniformLoc_ID_isWireframe = -1;
GLint UniformLoc_ID_bUseDebugDiffuseNoLighting = -1;

GLint UniformLoc_alpha = -1;

GLint UniformLoc_texSamp2D_00 = -1;
GLint UniformLoc_texSamp2D_01 = -1;
GLint UniformLoc_texSamp2D_02 = -1;
GLint UniformLoc_texSamp2D_03 = -1;

GLint UniformLoc_bUseTextureAsDiffuse = -1;
GLint UniformLoc_bUseLighting = -1;

GLint UniformLoc_bIsImposterBlackIsAlpha = -1;


std::vector< cGameObject* > g_vec_pGOs;
std::vector< cContact > g_vec_Contacts;

cMeshTypeManager* g_pTheMeshTypeManager = 0;

cShaderManager* g_pTheShaderManager = 0;

cParticleEmitter* g_pParticles = 0;

cBasicTextureManager* g_pTextureManager = 0;


// This was only added as a temorary thing...
cGameObject* g_pObjectToRotate = 0;


// The skybox object isn't added to the regular vector of 
//  objects. Instead, it is called separately. 
// You could also add it to the "rendered objects" and 
//  add some kind of indication that it's a skybox...
cGameObject* g_pSkyBoxObject = 0;
GLint UniformLoc_bIsCubeMapObject = -1;
GLint UniformLoc_myCubemapSkyBoxSampler = -1;

// For "good enough for rock-n-roll" texture changes
glm::vec4 g_textureMod;
glm::mat4 g_matTexMod;
GLint UniformLoc_textureMod = -1;
GLint UniformLoc_matTexMod = -1;


cLuaBrain* p_LuaScripts = nullptr;
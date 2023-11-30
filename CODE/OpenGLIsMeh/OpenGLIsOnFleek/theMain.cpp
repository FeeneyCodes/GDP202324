//https://www.glfw.org/docs/latest/quick.html
//http://graphics.stanford.edu/data/3Dscanrep/

#include "OpenGLCommon.h"
//#include <glad/glad.h>
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>
#include "cGlobal.h"

#include <iostream>
#include <fstream>      // C++ file IO (secret: it's a wraper for the c IO)
#include <sstream>      // like a string builder
#include <vector>       // Aka a "smart array"


//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "Basic Shader Manager/cShaderManager.h"
#include "cVAOManager/cVAOManager.h"

#include "GLWF_CallBacks.h" // keyboard and mouse input

#include "cMesh.h"
//#include "sPhsyicsProperties.h"
#include "cPhysics.h"

#include "cLightManager.h"
#include "cLightHelper.h"

#include "TextureManager/cBasicTextureManager.h"

#include "cHiResTimer.h"

#include "cCommand_MoveTo.h"

#include "LuaBrain/cLuaBrain.h"

glm::vec3 g_cameraEye = glm::vec3(0.0, 70.0, 181.0f);
glm::vec3 g_cameraTarget = glm::vec3(0.0f, 5.0f, 0.0f);
glm::vec3 g_upVector = glm::vec3(0.0f, 1.0f, 0.0f);



cVAOManager* g_pMeshManager = NULL;

cBasicTextureManager* g_pTextureManager = NULL;

cMesh* g_pDebugSphereMesh = NULL;
// Used by g_DrawDebugSphere()
GLuint g_DebugSphereMesh_shaderProgramID = 0;

std::vector<double> g_vecLastFrameTimes;



// Smart array of cMesh object
//std::vector<cMesh> g_vecMeshesToDraw;
//    ____       _       _                  _          __  __           _               
//   |  _ \ ___ (_)_ __ | |_ ___ _ __ ___  | |_ ___   |  \/  | ___  ___| |__   ___  ___ 
//   | |_) / _ \| | '_ \| __/ _ \ '__/ __| | __/ _ \  | |\/| |/ _ \/ __| '_ \ / _ \/ __|
//   |  __/ (_) | | | | | ||  __/ |  \__ \ | || (_) | | |  | |  __/\__ \ | | |  __/\__ \
//   |_|   \___/|_|_| |_|\__\___|_|  |___/  \__\___/  |_|  |_|\___||___/_| |_|\___||___/
//                            
// This is the list of meshes                                                           
std::vector< cMesh* > g_vec_pMeshesToDraw;
// This is the list of physical properties 
// This has a pointer to the matching mesh
//std::vector< sPhsyicsProperties* > g_vec_pPhysicalProps;
cPhysics* g_pPhysics = NULL;


// Returns NULL if not found
cMesh* g_pFindMeshByFriendlyName(std::string friendlyNameToFind);

cLightManager* g_pTheLights = NULL;

std::vector<cCommand_MoveTo> g_vecAnimationCommands;

//std::vector<sPhsyicsProperties*> g_vecThingsThePhysicsThingPaysAtte;
// 
int g_selectedMesh = 0;
int g_selectedLight = 0;

// Function signature
bool SaveVectorSceneToFile(std::string saveFileName);

bool LoadModels(void);

//void DoPhysicUpdate(double deltaTime);

void DrawObject(cMesh* pCurrentMesh, glm::mat4 matModel, GLuint shaderProgramID);

cMesh* g_pFindMeshByFriendlyName(std::string friendlyNameToFind);

void DrawLightDebugSpheres(glm::mat4 matProjection, glm::mat4 matView,
                           GLuint shaderProgramID);

float getRandomFloat(float a, float b);

void LoadTheRobotronModels(GLuint shaderProgram);

// HACK:
float g_HeightAdjust = 10.0f;
glm::vec2 g_UVOffset = glm::vec2(0.0f, 0.0f);

// This is in the global space as an example, 
// mainly so that we can run scripts when we press keys
cLuaBrain g_LuaBrain;

// Silly Lua script binding example
void ChangeTaylorSwiftTexture(std::string newTexture)
{
//    pGroundMesh->friendlyName = "Ground";
//    pGroundMesh->textureName[0] = "TaylorSwift_Eras_Poster.bmp";

    cMesh* pGround = g_pFindMeshByFriendlyName("Ground");
    if ( pGround )
    {
//        pGround->textureName[0] = "TaylorSwift_Eras_Poster.bmp";
        pGround->textureName[0] = newTexture;   // "SpidermanUV_square.bmp";
    }
    return;
}

// Here the same sort of thing, but that Lua can call
int lua_ChangeTaylorSwiftTexture(lua_State* L)
{
//    ChangeTaylorSwiftTexture("SpidermanUV_square.bmp");

    // Look up whatever was passed on the stack
    // https://www.lua.org/pil/24.2.2.html:
    // "The first element in the stack (that is, the element that was pushed first) 
    //    has index 1, the next one has index 2, and so on."

    const char* bitMapName = lua_tostring(L, 1);
    std::string sBitMapName(bitMapName);

    ChangeTaylorSwiftTexture(sBitMapName);

    return 0;
}

// Here the same sort of thing, but that Lua can call
int lua_AddSerialMoveObjectCommand(lua_State* L)
{
    // We have to decide what we are typing into Lua.
    // It's 100% up to us
    // 
    // How about:
    // AddMoveCommand('bathtub', Dx, Dy, Dz, timeInSeconds)
    //
    // The first string is the friendly name
    // The Dx, etc. is "Destination" X,Y,Z



    std::string MeshFriendlyName(lua_tostring(L, 1));       // 'bathtub'
    cMesh* pBathTub = g_pFindMeshByFriendlyName(MeshFriendlyName);

    // https://pgl.yoyo.org/luai/i/lua_Number
    // All Lua numbers are stored as doubles
    // typedef double lua_Number;

    glm::vec3 destinationXYZ;
    destinationXYZ.x = (float)lua_tonumber(L, 2);     // Dx (destination X)
    destinationXYZ.y = (float)lua_tonumber(L, 3);     // Dy (destination Y)
    destinationXYZ.z = (float)lua_tonumber(L, 4);     // Dz (destination Z)
    float timeInSeconds = (float)lua_tonumber(L, 5);   // timeInSeconds

//    cCommand_MoveTo moveBathTub(pBathTub,
//                                pBathTub->getDrawPosition(),
//                                glm::vec3(50.0f, 0.0f, 0.0f),
//                                10.0f);

    cCommand_MoveTo moveBathTub(pBathTub,
                                pBathTub->getDrawPosition(),
                                destinationXYZ,
                                timeInSeconds);
 
    ::g_vecAnimationCommands.push_back(moveBathTub);


    return 0;
}

// 
int lua_GetMeshPositionByFriendlyName(lua_State* L)
{
    // Example Lua script might be:
    // 
    // bIsValid, x, y, z = GetMeshPositionByFriendlyName('bathtub');
    // 
    // 1st parameter indicates if we found this object or not
    // 
    // Returns the x, y, z location

    std::string MeshFriendlyName(lua_tostring(L, 1));       // 'bathtub'
    cMesh* pBathTub = g_pFindMeshByFriendlyName(MeshFriendlyName);

    if (pBathTub)
    {
        // 1st parameter: indicate if this object was found
        lua_pushboolean(L, true);
        lua_pushnumber( L, pBathTub->getDrawPosition().x);
        lua_pushnumber( L, pBathTub->getDrawPosition().y );
        lua_pushnumber( L, pBathTub->getDrawPosition().z );
        // Tell Lua how many things got pushed onto the stack
        return 4;
    }
    
    // Didn't find it
    lua_pushboolean(L, false);

    // We returned 1 value, a "false"
    return 1;
}

int lua_SetMeshPositionByFriendlyName(lua_State* L)
{
    // Example Lua script might be
    // SetMeshPositionByFriendlyName('bathtub', x, y, z )

    std::string MeshFriendlyName(lua_tostring(L, 1));       // 'bathtub'
    cMesh* pBathTub = g_pFindMeshByFriendlyName(MeshFriendlyName);

    glm::vec3 newLocationXYZ;
    newLocationXYZ.x = (float)lua_tonumber(L, 2);     // Dx (destination X)
    newLocationXYZ.y = (float)lua_tonumber(L, 3);     // Dy (destination Y)
    newLocationXYZ.z = (float)lua_tonumber(L, 4);     // Dz (destination Z)

    pBathTub->drawPosition = newLocationXYZ;

    return 0;
}



static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void TestSort(void);

int main(void)
{
    TestSort();

    cMesh bob;

    std::cout << "About to blow you mind with OpenGL!" << std::endl;

    GLFWwindow* window;
//    GLuint vertex_buffer; //, vertex_shader, fragment_shader;//v , program;
//    GLint mvp_location;// , vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);



//    cHiResTimer* p_HRTimer = new cHiResTimer();
    // Get average of last 30 frames
//    cHiResTimer* p_HRTimer = new cHiResTimer(32);
    cHiResTimer* p_HRTimer = new cHiResTimer(60);


    cShaderManager* pShaderThing = new cShaderManager();
    pShaderThing->setBasePath("assets/shaders");

    cShaderManager::cShader vertexShader;
    vertexShader.fileName = "vertexShader01.glsl";

    // Add a geometry shader
    cShaderManager::cShader geometryShader;
    geometryShader.fileName = "geometryPassThrough.glsl";


    cShaderManager::cShader fragmentShader;
    fragmentShader.fileName = "fragmentShader01.glsl";

//    if ( ! pShaderThing->createProgramFromFile("shader01", vertexShader, fragmentShader ) )
    if ( ! pShaderThing->createProgramFromFile("shader01", vertexShader, geometryShader, fragmentShader ) )
    {
        std::cout << "Error: Couldn't compile or link:" << std::endl;
        std::cout << pShaderThing->getLastError();
        return -1;
    }


//
    GLuint shaderProgramID = pShaderThing->getIDFromFriendlyName("shader01");

    // Set the debug shader ID we're going to use
    ::g_DebugSphereMesh_shaderProgramID = shaderProgramID;

    ::g_pMeshManager = new cVAOManager();

    ::g_pMeshManager->setBasePath("assets/models");

    //sModelDrawInfo bunnyDrawingInfo;
    //pMeshManager->LoadModelIntoVAO("bun_zipper_res2_xyz_n_rgba.ply",
    //                               bunnyDrawingInfo, shaderProgramID);
    //std::cout << "Loaded: " << bunnyDrawingInfo.numberOfVertices << " vertices" << std::endl;

    sModelDrawInfo bathtubDrawingInfo;
    ::g_pMeshManager->LoadModelIntoVAO("bathtub_xyz_n_rgba_uv.ply",
                                   bathtubDrawingInfo, shaderProgramID);
    std::cout << "Loaded: " << bathtubDrawingInfo.numberOfVertices << " vertices" << std::endl;

    sModelDrawInfo terrainDrawingInfo;
//    ::g_pMeshManager->LoadModelIntoVAO("Terrain_xyz_n_rgba.ply",
    ::g_pMeshManager->LoadModelIntoVAO("Terrain_xyz_n_rgba_uv.ply",
                                   terrainDrawingInfo, shaderProgramID);
    std::cout << "Loaded: " << terrainDrawingInfo.numberOfVertices << " vertices" << std::endl;

    ::g_pMeshManager->LoadModelIntoVAO("Big_Flat_Mesh_256x256_00_132K_xyz_n_rgba_uv.ply",
                                   terrainDrawingInfo, shaderProgramID);
    std::cout << "Loaded: " << terrainDrawingInfo.numberOfVertices << " vertices" << std::endl;

    ::g_pMeshManager->LoadModelIntoVAO("Big_Flat_Mesh_256x256_12_5_xyz_n_rgba_uv.ply",
                                   terrainDrawingInfo, shaderProgramID);
    std::cout << "Loaded: " << terrainDrawingInfo.numberOfVertices << " vertices" << std::endl;

    ::g_pMeshManager->LoadModelIntoVAO("Big_Flat_Mesh_256x256_07_1K_xyz_n_rgba_uv.ply",
                                   terrainDrawingInfo, shaderProgramID);
    std::cout << "Loaded: " << terrainDrawingInfo.numberOfVertices << " vertices" << std::endl;


//    sModelDrawInfo HilbertRampDrawingInfo;
//    ::g_pMeshManager->LoadModelIntoVAO("HilbertRamp_stl (rotated).ply",
//                                       HilbertRampDrawingInfo, shaderProgramID);
//    std::cout << "Loaded: " << HilbertRampDrawingInfo.numberOfVertices << " vertices" << std::endl;
//
//    sModelDrawInfo gridDrawingInfo;
//    ::g_pMeshManager->LoadModelIntoVAO("Flat_Grid_100x100.ply",
//                                   gridDrawingInfo, shaderProgramID);
//    std::cout << "Loaded: " << gridDrawingInfo.numberOfVertices << " vertices" << std::endl;

    sModelDrawInfo sphereDrawingInfo;
//    ::g_pMeshManager->LoadModelIntoVAO("Sphere_1_unit_Radius.ply",
    ::g_pMeshManager->LoadModelIntoVAO("Sphere_1_unit_Radius_xyz_n_rgba_uv.ply",
                                   sphereDrawingInfo, shaderProgramID);
    std::cout << "Loaded: " << sphereDrawingInfo.numberOfVertices << " vertices" << std::endl;

//    sModelDrawInfo Flat_1x1_planeDrawingInfo;
//    ::g_pMeshManager->LoadModelIntoVAO("Flat_1x1_plane.ply",
//                                       Flat_1x1_planeDrawingInfo, shaderProgramID);
//    std::cout << "Loaded: " << Flat_1x1_planeDrawingInfo.numberOfVertices << " vertices" << std::endl;

    // Spiderman
    sModelDrawInfo spiderMan;

    ::g_pMeshManager->LoadModelIntoVAO("legospiderman_body_xyz_n_rgba_uv.ply", spiderMan, shaderProgramID);
    ::g_pMeshManager->LoadModelIntoVAO("legospiderman_head_xyz_n_rgba_uv.ply", spiderMan, shaderProgramID);
    ::g_pMeshManager->LoadModelIntoVAO("legospiderman_Hips_xyz_n_rgba_uv.ply", spiderMan, shaderProgramID);

    ::g_pMeshManager->LoadModelIntoVAO("legospiderman_Left_arm_xyz_n_rgba_uv.ply", spiderMan, shaderProgramID);
    ::g_pMeshManager->LoadModelIntoVAO("legospiderman_Left_hand_xyz_n_rgba_uv.ply", spiderMan, shaderProgramID);

    ::g_pMeshManager->LoadModelIntoVAO("legospiderman_Right_arm_xyz_n_rgba_uv.ply", spiderMan, shaderProgramID);
    ::g_pMeshManager->LoadModelIntoVAO("legospiderman_Right_hand_xyz_n_rgba_uv.ply", spiderMan, shaderProgramID);

    ::g_pMeshManager->LoadModelIntoVAO("legospiderman_Left_leg_xyz_n_rgba_uv.ply", spiderMan, shaderProgramID);
    ::g_pMeshManager->LoadModelIntoVAO("legospiderman_Right_leg_xyz_n_rgba_uv.ply", spiderMan, shaderProgramID);

        
        
    // ... and so on

    ::g_pTextureManager = new cBasicTextureManager();

    ::g_pTextureManager->SetBasePath("assets/textures"); 
    if ( ::g_pTextureManager->Create2DTextureFromBMPFile("TaylorSwift_Eras_Poster.bmp", true) )
    {
        std::cout << "Loaded the Taylor Swift texture" << std::endl;
    }
    else
    {
        std::cout << "ERROR: no Taylor Swift!!" << std::endl;
    }
    //
    ::g_pTextureManager->Create2DTextureFromBMPFile("Blank_UV_Text_Texture.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("SpidermanUV_square.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("Water_Texture_01.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("taylor-swift-jimmy-fallon.bmp", true);
    // Load the HUGE height map
    ::g_pTextureManager->Create2DTextureFromBMPFile("NvF5e_height_map.bmp", true);
    
    // Using this for discard transparency mask
    ::g_pTextureManager->Create2DTextureFromBMPFile("FAKE_Stencil_Texture_612x612.bmp", true);

    // Load a cube map
    ::g_pTextureManager->SetBasePath("assets/textures/CubeMaps");
    std::string errors;
    ::g_pTextureManager->CreateCubeTextureFromBMPFiles("space",
                                                       "SpaceBox_right1_posX.bmp",
                                                       "SpaceBox_left2_negX.bmp",
                                                       "SpaceBox_top3_posY.bmp",
                                                       "SpaceBox_bottom4_negY.bmp",
                                                       "SpaceBox_front5_posZ.bmp",
                                                       "SpaceBox_back6_negZ.bmp",
                                                       true,
                                                       errors);

    ::g_pTextureManager->CreateCubeTextureFromBMPFiles("SunnyDay",
                                                       "TropicalSunnyDayLeft2048.bmp",
                                                       "TropicalSunnyDayRight2048.bmp",
                                                       "TropicalSunnyDayUp2048.bmp",
                                                       "TropicalSunnyDayDown2048.bmp",
                                                       "TropicalSunnyDayFront2048.bmp",
                                                       "TropicalSunnyDayBack2048.bmp",
                                                       true,
                                                       errors);


    // This handles the phsyics objects
    ::g_pPhysics = new cPhysics();

    ::g_pPhysics->setVAOManager(::g_pMeshManager);

    // 
    LoadModels();

//    LoadTheRobotronModels(shaderProgramID);


    ::g_pTheLights = new cLightManager();
    // 
    ::g_pTheLights->SetUniformLocations(shaderProgramID);

    ::g_pTheLights->theLights[0].param2.x = 1.0f;   // Turn on
//    ::g_pTheLights->theLights[0].param1.x = 0.0f;   // 0 = point light
    ::g_pTheLights->theLights[0].param1.x = 1.0f;   // 1 = spot light
    // With spots, set direction (relative)
    ::g_pTheLights->theLights[0].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    //	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
    ::g_pTheLights->theLights[0].param1.y = 15.0f;
    ::g_pTheLights->theLights[0].param1.z = 25.0f;


    ::g_pTheLights->theLights[0].position.x = -46.0f;
    ::g_pTheLights->theLights[0].position.y = 23.0f;
    ::g_pTheLights->theLights[0].position.z = -26.0f;



    // How "bright" the lights is
    // Really the opposite of brightness.
    //  how dim do you want this
    ::g_pTheLights->theLights[0].atten.x = 0.0f;        // Constant attenuation
    ::g_pTheLights->theLights[0].atten.y = 0.01f;        // Linear attenuation
    ::g_pTheLights->theLights[0].atten.z = 0.01f;        // Quadratic attenuation

    // Light #1 is a directional light 
    ::g_pTheLights->theLights[1].param2.x = 1.0f;   // Turn on
    ::g_pTheLights->theLights[1].param1.x = 2.0f;   // Directional

    // Direction with respect of the light.
    ::g_pTheLights->theLights[1].direction = glm::normalize(glm::vec4(-0.5f, -1.0f, 0.0f, 1.0f));
//    float lightBrightness = 0.7f;
    ::g_pTheLights->theLights[1].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pTheLights->theLights[1].specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);


//    glm::vec3 cameraEye = glm::vec3(10.0, 5.0, -15.0f);
    float yaxisRotation = 0.0f;

    double lastTime = glfwGetTime();

// *************************************************************************************
    // These are the commands we are going to process
//    std::vector<cCommand_MoveTo> vecAnimationCommands;
//
//    cMesh* pBathTub = g_pFindMeshByFriendlyName("bathtub");
//
//    cCommand_MoveTo moveBathTub(pBathTub,
//                                pBathTub->getDrawPosition(),
//                                glm::vec3(50.0f, 0.0f, 0.0f),
//                                10.0f);
//    ::g_vecAnimationCommands.push_back(moveBathTub);
//
//
//    cCommand_MoveTo moveBathTub2(pBathTub,
//                                pBathTub->getDrawPosition(),
//                                glm::vec3(-30.0f, 10.0f, 16.0f),
//                                5.0f);
//    ::g_vecAnimationCommands.push_back(moveBathTub2);
//
// *************************************************************************************

    while (!glfwWindowShouldClose(window))
    {

        // Switch the "main" shader
//        shaderProgramID = pShaderThing->getIDFromFriendlyName("shader01");
//        glUseProgram(shaderProgramID);

        float ratio;
        int width, height;

        glUseProgram(shaderProgramID);

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        // While drawing a pixel, see if the pixel that's already there is closer or not?
        glEnable(GL_DEPTH_TEST);
        // (Usually) the default - does NOT draw "back facing" triangles
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);


// *****************************************************************

        ::g_pTheLights->UpdateUniformValues(shaderProgramID);


// *****************************************************************
        //uniform vec4 eyeLocation;
        GLint eyeLocation_UL = glGetUniformLocation(shaderProgramID, "eyeLocation");
        glUniform4f(eyeLocation_UL,
                    ::g_cameraEye.x, ::g_cameraEye.y, ::g_cameraEye.z, 1.0f);



//       //mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glm::mat4 matProjection = glm::perspective(0.6f,
                                                   ratio,
                                                   0.1f,        // Near (as big)
                                                   10'000.0f);    // Far (as small)

        glm::mat4 matView = glm::lookAt(::g_cameraEye,
                                        ::g_cameraTarget,
                                        ::g_upVector);

        GLint matProjection_UL = glGetUniformLocation(shaderProgramID, "matProjection");
        glUniformMatrix4fv(matProjection_UL, 1, GL_FALSE, glm::value_ptr(matProjection));

        GLint matView_UL = glGetUniformLocation(shaderProgramID, "matView");
        glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(matView));


        // *********************************************************************
        // Draw all the objects
        for ( unsigned int index = 0; index != ::g_vec_pMeshesToDraw.size(); index++ )
        {
            cMesh* pCurrentMesh = ::g_vec_pMeshesToDraw[index];

            if (pCurrentMesh->bIsVisible)
            {

                glm::mat4 matModel = glm::mat4(1.0f);   // Identity matrix

                DrawObject(pCurrentMesh, matModel, shaderProgramID);
            }//if (pCurrentMesh->bIsVisible)

        }//for ( unsigned int index
        // *********************************************************************

// This is very old OpenGL "Immediate Mode"
// Don't use this for your usual stuff, but it's very handy 
//   for debug stuff (where you're not worried about speed)
// So I guess you CAN mix old and new stuff
//        glBegin(GL_QUADS);
//            glColor3f(1.0f, 1.0f, 1.0f);
//            glVertex3i(-10, 10.0, 0.0);
//            glVertex3i(+10, 10.0, 0.0);
//            glVertex3i(+10, 10.0, -10.0);
//            glVertex3i(-10, 10.0, -10.0);
//        glEnd();


        // Draw the skybox
        {
            // HACK: I'm making this here, but hey...
            cMesh theSkyBox;
            theSkyBox.meshName = "Sphere_1_unit_Radius_xyz_n_rgba_uv.ply";
            theSkyBox.setUniformDrawScale(10.0f);

            theSkyBox.setUniformDrawScale(5'000.0f);
            theSkyBox.setDrawPosition(::g_cameraEye);
//            theSkyBox.bIsWireframe = true;

            // Depth test
//            glDisable(GL_DEPTH_TEST);       // Writes no matter what
            // Write to depth buffer (depth mask)
//            glDepthMask(GL_FALSE);          // Won't write to the depth buffer
            
            // uniform bool bIsSkyBox;
            GLint bIsSkyBox_UL = glGetUniformLocation(shaderProgramID, "bIsSkyBox");
            glUniform1f(bIsSkyBox_UL, (GLfloat) GL_TRUE);

            // The normals for this sphere are facing "out" but we are inside the sphere
            glCullFace(GL_FRONT);

            DrawObject(&theSkyBox, glm::mat4(1.0f), shaderProgramID);

            glUniform1f(bIsSkyBox_UL, (GLfloat)GL_FALSE);

            // Put the culling back to "normal" (back facing are not drawn)
            glCullFace(GL_BACK);
        }




 
//        // Time per frame (more or less)
//        double currentTime = glfwGetTime();
//        double deltaTime = currentTime - lastTime;
//        // We should add this to a list of times, and get the average frame time
//        const double LARGEST_DELTA_TIME = 1.0f / 30.0f; // 30FPS  (16 ms)  24 FPS
//        if ( deltaTime > LARGEST_DELTA_TIME )
//        {
//            deltaTime = LARGEST_DELTA_TIME;
//        }
//        std::cout << deltaTime << std::endl;
//        lastTime = currentTime;

        double deltaTime = p_HRTimer->getFrameTime();
//        std::cout << deltaTime << std::endl;


// ***********************************************************************
        if ( ! ::g_vecAnimationCommands.empty() )
        {
            ::g_vecAnimationCommands[0].Update(deltaTime);

            // Done? 
            if (::g_vecAnimationCommands[0].isDone() )
            {
                // Erase the item from vector
                // Is "slow" in that it has to copy the vector again
                ::g_vecAnimationCommands.erase(::g_vecAnimationCommands.begin());
            }
        }//if (!vecAnimationCommands.empty())
// ***********************************************************************


        DrawLightDebugSpheres(matProjection, matView, shaderProgramID);


        // Point the spotlight at the bathtub
        cMesh* pBathTub = g_pFindMeshByFriendlyName("bathtub");
        if ( pBathTub )
        {
            glm::vec3 bathTubToLightRay = pBathTub->drawPosition - glm::vec3(::g_pTheLights->theLights[0].position);

            bathTubToLightRay = glm::normalize(bathTubToLightRay);

            ::g_pTheLights->theLights[0].direction = glm::vec4(bathTubToLightRay, 1.0f);
        }

        // HACK:
        cMesh* pSpidey = g_pFindMeshByFriendlyName("SpiderManBody");
//        pSpidey->drawOrientation.y += 0.001f;
        if ( pSpidey )
        {
            pSpidey->adjustRoationAngleFromEuler(glm::vec3(0.0f, 0.0f, 0.001f));
        }

//        cMesh* pTayTayGround = g_pFindMeshByFriendlyName("Ground");
//        pTayTayGround->textureRatios[1] += 0.0001f;
//        pTayTayGround->textureRatios[0] = 1.0f - pTayTayGround->textureRatios[1];



//        pSpidey->drawPosition.x += 0.01f;


//        // HACK: See where the sphere is on the surface of the "ground"
//        cMesh* pBouncingSphere = g_pFindMeshByFriendlyName("Sphere");
//        if ( pBouncingSphere )
//        {
//            cMesh* pGround = g_pFindMeshByFriendlyName("Ground");
//            // Place this sphere right above the "ground"
//            
//            cMesh* pDebugSphere = g_pFindMeshByFriendlyName("DEBUG_SPHERE");
//
//            sTransformInfo bouncingSphereTrans = pBouncingSphere->getTransformInfo();
//            // Place it where it intersects the ground
//            bouncingSphereTrans.position.y = pGround->getTransformInfo().position.y;
//
//            pDebugSphere->setTransformInfo(bouncingSphereTrans);
//            //
//            pDebugSphere->bIsVisible = true;
//            pDebugSphere->bUseDebugColours = true;
//            pDebugSphere->wholeObjectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//
//            DrawObject(pDebugSphere, glm::mat4(1.0f), shaderProgramID);
//
//            pDebugSphere->bIsVisible = false;
//        }

        // 
//        DoPhysicUpdate(deltaTime);

//        ::g_pPhysics->Update(deltaTime);


        glfwSwapBuffers(window);
        glfwPollEvents();

        // Update the title screen
        std::stringstream ssTitle;
        ssTitle << "Camera (x,y,z): "
            << ::g_cameraEye.x << ", "
            << ::g_cameraEye.y << ", "
            << ::g_cameraEye.z << ") "
            << "Light[" << ::g_selectedLight << "]: "
            << ::g_pTheLights->theLights[::g_selectedLight].position.x << ", "
            << ::g_pTheLights->theLights[::g_selectedLight].position.y << ", "
            << ::g_pTheLights->theLights[::g_selectedLight].position.z << "  "
            << "const:" << ::g_pTheLights->theLights[::g_selectedLight].atten.x << " "
            << "linear:" << ::g_pTheLights->theLights[::g_selectedLight].atten.y << " "
            << "quad:" << ::g_pTheLights->theLights[::g_selectedLight].atten.z << " "
            << "inner: " << ::g_pTheLights->theLights[::g_selectedLight].param1.y << " "
            << "outer: " << ::g_pTheLights->theLights[::g_selectedLight].param1.z << " ";

//        glfwSetWindowTitle(window, "HEY!");

        std::string theTitle = ssTitle.str();

        glfwSetWindowTitle(window, theTitle.c_str() );


    }

    // Delete everything


    glfwDestroyWindow(window);




    glfwTerminate();
    exit(EXIT_SUCCESS);
}

// Returns NULL if not found
cMesh* g_pFindMeshByFriendlyName(std::string friendlyNameToFind)
{
    for ( unsigned int index = 0; index != ::g_vec_pMeshesToDraw.size(); index++ )
    {
        if ( ::g_vec_pMeshesToDraw[index]->friendlyName == friendlyNameToFind )
        {
            // Found it
            return ::g_vec_pMeshesToDraw[index];
        }
    }
    // Didn't find it
    return NULL;
}


void DrawLightDebugSpheres(glm::mat4 matProjection, glm::mat4 matView,
                           GLuint shaderProgramID)
{
    if ( ! ::g_drawDebugLightSpheres )
    {
        return;
    }

    // Draw concentric spheres to indicate light position and attenuation

    // Small white sphere where the light is
    ::g_DrawDebugSphere(::g_pTheLights->theLights[g_selectedLight].position,
                        0.5f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    cLightHelper lightHelper;

    // vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
    float constantAtten = ::g_pTheLights->theLights[g_selectedLight].atten.x;
    float linearAtten = ::g_pTheLights->theLights[g_selectedLight].atten.y;
    float quadAtten = ::g_pTheLights->theLights[g_selectedLight].atten.z;


    // Draw a red sphere at 75% brightness
    {
        float distAt75Percent = lightHelper.calcApproxDistFromAtten(0.75f, 0.01f, 100000.0f,
                                                                    constantAtten, linearAtten, quadAtten, 50);

        ::g_DrawDebugSphere(::g_pTheLights->theLights[g_selectedLight].position,
                            distAt75Percent, 
                            glm::vec4(0.5f, 0.0f, 0.0f, 1.0f));
    }


    // Draw a green sphere at 50% brightness
    {
        float distAt50Percent = lightHelper.calcApproxDistFromAtten(0.50f, 0.01f, 100000.0f,
                                                                    constantAtten, linearAtten, quadAtten, 50);

        ::g_DrawDebugSphere(::g_pTheLights->theLights[g_selectedLight].position,
                            distAt50Percent,
                            glm::vec4(0.0f, 0.5f, 0.0f, 1.0f));
    }

    // Draw a yellow? sphere at 25% brightness
    {
        float distAt25Percent = lightHelper.calcApproxDistFromAtten(0.25f, 0.01f, 100000.0f,
                                                                    constantAtten, linearAtten, quadAtten, 50);

        ::g_DrawDebugSphere(::g_pTheLights->theLights[g_selectedLight].position,
                            distAt25Percent,
                            glm::vec4(0.50f, 0.5f, 0.0f, 1.0f));
    }

    // Draw a blue sphere at 1% brightness
    {
        float distAt_5Percent = lightHelper.calcApproxDistFromAtten(0.01f, 0.01f, 100000.0f,
                                                                    constantAtten, linearAtten, quadAtten, 50);

        ::g_DrawDebugSphere(::g_pTheLights->theLights[g_selectedLight].position,
                            distAt_5Percent,
                            glm::vec4(0.0f, 0.0f, 0.5f, 1.0f));
    }


    return;
}

void SetUpTextures(cMesh* pCurrentMesh, GLuint shaderProgramID)
{
//    GLuint Texture00 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[0]);
//    if (Texture00 == 0)
//    {
//        Texture00 = ::g_pTextureManager->getTextureIDFromName("Blank_UV_Text_Texture.bmp");
//    }
//
//    // We are just going to pick texture unit 5 (for no reason, just as an example)
//    //    glActiveTexture(GL_TEXTURE5);       // #5 TEXTURE UNIT
//    glActiveTexture(GL_TEXTURE0 + 5);       // #5 TEXTURE UNIT
//    glBindTexture(GL_TEXTURE_2D, Texture00);
//
//    //uniform sampler2D texture_00;
//    GLint texture_00_UL = glGetUniformLocation(shaderProgramID, "texture_00");
//    glUniform1i(texture_00_UL, 5);     // <- 5, an integer, because it's "Texture Unit #5"
//    // ***************************************************************

//    uniform sampler2D texture_00;			// 2D meaning x,y or s,t or u,v
//    uniform sampler2D texture_01;
//    uniform sampler2D texture_02;
//    uniform sampler2D texture_03;
//    uniform sampler2D texture_04;			// 2D meaning x,y or s,t or u,v
//    uniform sampler2D texture_05;
//    uniform sampler2D texture_06;
//    uniform sampler2D texture_07;
//    //... and so on
//    //uniform float textureMixRatio[8];
//    uniform vec4 textureMixRatio_0_3;
//    uniform vec4 textureMixRatio_4_7;


    {
        GLint textureUnitNumber = 0;
        GLuint Texture00 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[textureUnitNumber]);
        glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
        glBindTexture(GL_TEXTURE_2D, Texture00);
        GLint texture_00_UL = glGetUniformLocation(shaderProgramID, "texture_00");
        glUniform1i(texture_00_UL, textureUnitNumber);
    }

    {
        GLint textureUnitNumber = 1;
        GLuint Texture01 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[textureUnitNumber]);
        glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
        glBindTexture(GL_TEXTURE_2D, Texture01);
        GLint texture_01_UL = glGetUniformLocation(shaderProgramID, "texture_01");
        glUniform1i(texture_01_UL, textureUnitNumber);
    }

    {
        GLint textureUnitNumber = 2;
        GLuint Texture02 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[textureUnitNumber]);
        glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
        glBindTexture(GL_TEXTURE_2D, Texture02);
        GLint texture_02_UL = glGetUniformLocation(shaderProgramID, "texture_02");
        glUniform1i(texture_02_UL, textureUnitNumber);
    }

    {
        GLint textureUnitNumber = 3;
        GLuint Texture03 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[textureUnitNumber]);
        glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
        glBindTexture(GL_TEXTURE_2D, Texture03);
        GLint texture_03_UL = glGetUniformLocation(shaderProgramID, "texture_03");
        glUniform1i(texture_03_UL, textureUnitNumber);
    }    
    // and so on to however many texture you are using

//    uniform vec4 textureMixRatio_0_3;
//    uniform vec4 textureMixRatio_4_7;

    GLint textureMixRatio_0_3_UL = glGetUniformLocation(shaderProgramID, "textureMixRatio_0_3");
//    GLint textureMixRatio_4_7_UL = glGetUniformLocation(shaderProgramID, "textureMixRatio_4_7");

    glUniform4f(textureMixRatio_0_3_UL,
                pCurrentMesh->textureRatios[0],
                pCurrentMesh->textureRatios[1],
                pCurrentMesh->textureRatios[2],
                pCurrentMesh->textureRatios[3]);
//    glUniform4f(textureMixRatio_4_7_UL,
//                pCurrentMesh->textureRatios[4],
//                pCurrentMesh->textureRatios[5],
//                pCurrentMesh->textureRatios[6],
//                pCurrentMesh->textureRatios[7]);


    // Also set up the height map and discard texture

    {
        // uniform sampler2D heightMapSampler;		// Texture unit 20
        GLint textureUnitNumber = 20;
        GLuint Texture20 = ::g_pTextureManager->getTextureIDFromName("NvF5e_height_map.bmp");
        glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
        glBindTexture(GL_TEXTURE_2D, Texture20);
        GLint texture_20_UL = glGetUniformLocation(shaderProgramID, "heightMapSampler");
        glUniform1i(texture_20_UL, textureUnitNumber);
    }    



    // Set up a skybox
    {
        // uniform samplerCube skyBoxTexture;		// Texture unit 30
        GLint textureUnit30 = 30;
        GLuint skyBoxID = ::g_pTextureManager->getTextureIDFromName("SunnyDay");
        glActiveTexture(GL_TEXTURE0 + textureUnit30);
        // NOTE: Binding is NOT to GL_TEXTURE_2D
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxID);
        GLint skyBoxSampler_UL = glGetUniformLocation(shaderProgramID, "skyBoxTexture");
        glUniform1i(skyBoxSampler_UL, textureUnit30);
    }

    
    return;
}




void DrawObject(cMesh* pCurrentMesh, glm::mat4 matModelParent, GLuint shaderProgramID)
{

    //         mat4x4_identity(m);
    glm::mat4 matModel = matModelParent;




    // Translation
    glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
                                            glm::vec3(pCurrentMesh->drawPosition.x,
                                                      pCurrentMesh->drawPosition.y,
                                                      pCurrentMesh->drawPosition.z));


       // Rotation matrix generation
//    glm::mat4 matRotateX = glm::rotate(glm::mat4(1.0f),
//                                       pCurrentMesh->drawOrientation.x, // (float)glfwGetTime(),
//                                       glm::vec3(1.0f, 0.0, 0.0f));
//
//
//    glm::mat4 matRotateY = glm::rotate(glm::mat4(1.0f),
//                                       pCurrentMesh->drawOrientation.y, // (float)glfwGetTime(),
//                                       glm::vec3(0.0f, 1.0, 0.0f));
//
//    glm::mat4 matRotateZ = glm::rotate(glm::mat4(1.0f),
//                                       pCurrentMesh->drawOrientation.z, // (float)glfwGetTime(),
//                                       glm::vec3(0.0f, 0.0, 1.0f));

    // Now we are all bougie, using quaternions
    glm::mat4 matRotation = glm::mat4( pCurrentMesh->get_qOrientation() );


    // Scaling matrix
    glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
                                    glm::vec3(pCurrentMesh->drawScale.x,
                                              pCurrentMesh->drawScale.y,
                                              pCurrentMesh->drawScale.z));
    //--------------------------------------------------------------

    // Combine all these transformation
    matModel = matModel * matTranslate;         // Done last

//    matModel = matModel * matRotateX;
//    matModel = matModel * matRotateY;
//    matModel = matModel * matRotateZ;
    //
    matModel = matModel * matRotation;


    matModel = matModel * matScale;             // Mathematically done 1st

//        m = m * rotateZ;
//        m = m * rotateY;
//        m = m * rotateZ;



   //mat4x4_mul(mvp, p, m);
//    glm::mat4 mvp = matProjection * matView * matModel;

//    GLint mvp_location = glGetUniformLocation(shaderProgramID, "MVP");
//    //glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
//    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

    GLint matModel_UL = glGetUniformLocation(shaderProgramID, "matModel");
    glUniformMatrix4fv(matModel_UL, 1, GL_FALSE, glm::value_ptr(matModel));


    // Also calculate and pass the "inverse transpose" for the model matrix
    glm::mat4 matModel_InverseTranspose = glm::inverse(glm::transpose(matModel));

    // uniform mat4 matModel_IT;
    GLint matModel_IT_UL = glGetUniformLocation(shaderProgramID, "matModel_IT");
    glUniformMatrix4fv(matModel_IT_UL, 1, GL_FALSE, glm::value_ptr(matModel_InverseTranspose));


    if (pCurrentMesh->bIsWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

//        glPointSize(10.0f);


        // uniform bool bDoNotLight;
    GLint bDoNotLight_UL = glGetUniformLocation(shaderProgramID, "bDoNotLight");

    if (pCurrentMesh->bDoNotLight)
    {
        // Set uniform to true
        glUniform1f(bDoNotLight_UL, (GLfloat)GL_TRUE);
    }
    else
    {
        // Set uniform to false;
        glUniform1f(bDoNotLight_UL, (GLfloat)GL_FALSE);
    }

        //uniform bool bUseDebugColour;	
    GLint bUseDebugColour_UL = glGetUniformLocation(shaderProgramID, "bUseDebugColour");
    if (pCurrentMesh->bUseDebugColours)
    {
        glUniform1f(bUseDebugColour_UL, (GLfloat)GL_TRUE);
        //uniform vec4 debugColourRGBA;
        GLint debugColourRGBA_UL = glGetUniformLocation(shaderProgramID, "debugColourRGBA");
        glUniform4f(debugColourRGBA_UL,
                    pCurrentMesh->wholeObjectDebugColourRGBA.r,
                    pCurrentMesh->wholeObjectDebugColourRGBA.g,
                    pCurrentMesh->wholeObjectDebugColourRGBA.b,
                    pCurrentMesh->wholeObjectDebugColourRGBA.a);
    }
    else
    {
        glUniform1f(bUseDebugColour_UL, (GLfloat)GL_FALSE);
    }



    // FOR NOW, hardcode the texture settings
    
    // uniform bool bUseVertexColours;
    GLint bUseVertexColours_UL = glGetUniformLocation(shaderProgramID, "bUseVertexColours");
    glUniform1f(bUseVertexColours_UL, (GLfloat)GL_FALSE);



    SetUpTextures(pCurrentMesh, shaderProgramID);

// *********************************************************************
    // Is this using the heigth map?
    // HACK:
    GLint bUseHeightMap_UL = glGetUniformLocation(shaderProgramID, "bUseHeightMap");
    // uniform bool bUseHeightMap;
    if ( pCurrentMesh->friendlyName == "Ground" )
    {
        glUniform1f(bUseHeightMap_UL, (GLfloat)GL_TRUE);

        //uniform float heightScale;
        GLint heightScale_UL = glGetUniformLocation(shaderProgramID, "heightScale");
        glUniform1f(heightScale_UL, ::g_HeightAdjust);
        
        //uniform vec2 UVOffset;
        GLint UVOffset_UL = glGetUniformLocation(shaderProgramID, "UVOffset");
        glUniform2f(UVOffset_UL, ::g_UVOffset.x, ::g_UVOffset.y);


    }
    else
    {
        glUniform1f(bUseHeightMap_UL, (GLfloat)GL_FALSE);
    }
// *********************************************************************


// *********************************************************************
    //  Discard texture example
    //    uniform bool bUseDiscardMaskTexture;
    //    uniform sampler2D maskSamplerTexture01;
    {
        GLint bUseDiscardMaskTexture_UL = glGetUniformLocation(shaderProgramID, "bUseDiscardMaskTexture");

        // uniform bool bUseHeightMap;
        if ( pCurrentMesh->friendlyName == "Ground" )
        {
            glUniform1f(bUseDiscardMaskTexture_UL, (GLfloat)GL_TRUE);

            //uniform sampler2D maskSamplerTexture01; 	// Texture unit 25
            GLint textureUnitNumber = 25;
            GLuint stencilMaskID = ::g_pTextureManager->getTextureIDFromName("FAKE_Stencil_Texture_612x612.bmp");
            glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
            glBindTexture(GL_TEXTURE_2D, stencilMaskID);

            GLint bUseDiscardMaskTexture_UL = glGetUniformLocation(shaderProgramID, "maskSamplerTexture01");
            glUniform1i(bUseDiscardMaskTexture_UL, textureUnitNumber);

        }
        else
        {
            glUniform1f(bUseDiscardMaskTexture_UL, (GLfloat)GL_FALSE);
        }
    }
// *********************************************************************

    sModelDrawInfo modelInfo;
    if (::g_pMeshManager->FindDrawInfoByModelName(pCurrentMesh->meshName, modelInfo))
    {
        // Found it!!!

        glBindVertexArray(modelInfo.VAO_ID); 		//  enable VAO (and everything else)
        glDrawElements(GL_TRIANGLES,
                       modelInfo.numberOfIndices,
                       GL_UNSIGNED_INT,
                       0);
        glBindVertexArray(0); 			            // disable VAO (and everything else)

    }

    // Are there any child meshes on this mesh
    // std::vector<cMesh*> vec_pChildMeshes;

    glm::mat4 matRemoveScaling = glm::scale(glm::mat4(1.0f),
                                            glm::vec3( 
                                                1.0f / pCurrentMesh->drawScale.x,
                                                1.0f / pCurrentMesh->drawScale.y,
                                                1.0f / pCurrentMesh->drawScale.z));

    matModel = matModel * matRemoveScaling;

   for ( cMesh* pChild : pCurrentMesh->vec_pChildMeshes )
   {

       // Notice we are passing the "parent" (already transformed) matrix
       // NOT an identiy matrix

       // if you are using scaling, you can "undo" the scaling
       // i.e. add the opposite of the scale the parent had



       DrawObject(pChild, matModel, shaderProgramID);

   }//for ( cMesh* pChild 



    return;
}


void g_DrawDebugSphere(glm::vec3 position, float scale, glm::vec4 colourRGBA)
{
    // Save the debug sphere state
    bool OLD_isVisible = ::g_pDebugSphereMesh->bIsVisible;
    glm::vec3 OLD_position = ::g_pDebugSphereMesh->drawPosition;
    glm::vec3 OLD_scale = ::g_pDebugSphereMesh->drawScale;
    glm::vec4 OLD_colours = ::g_pDebugSphereMesh->wholeObjectDebugColourRGBA;

    ::g_pDebugSphereMesh->bIsVisible = true;
    ::g_pDebugSphereMesh->drawPosition = position;
    ::g_pDebugSphereMesh->setUniformDrawScale(scale);
    ::g_pDebugSphereMesh->bDoNotLight = true;
    ::g_pDebugSphereMesh->bUseDebugColours = true;
    ::g_pDebugSphereMesh->wholeObjectDebugColourRGBA = colourRGBA;

   
    DrawObject(::g_pDebugSphereMesh, glm::mat4(1.0f), ::g_DebugSphereMesh_shaderProgramID);

    ::g_pDebugSphereMesh->bIsVisible = OLD_isVisible;
    ::g_pDebugSphereMesh->drawPosition = OLD_position;
    ::g_pDebugSphereMesh->drawScale = OLD_scale;
    ::g_pDebugSphereMesh->wholeObjectDebugColourRGBA = OLD_colours;

    return;
}

// https://stackoverflow.com/questions/5289613/generate-random-float-between-two-floats
float getRandomFloat(float a, float b) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}


void LoadTheRobotronModels(GLuint shaderProgram)
{

    // Load the Robotron: 2084 models

    std::string oldBasePath = ::g_pMeshManager->getBasePath();

    ::g_pMeshManager->setBasePath("Robotron_2084");

    std::vector<std::string> vecRobotronModels;
    vecRobotronModels.push_back("player1_xyz_rgba_n.ply");
    vecRobotronModels.push_back("player2_xyz_rgba_n.ply");
    vecRobotronModels.push_back("player3_xyz_rgba_n.ply");
    vecRobotronModels.push_back("player4_xyz_rgba_n.ply");
    vecRobotronModels.push_back("player5_xyz_rgba_n.ply");
    vecRobotronModels.push_back("player6_xyz_rgba_n.ply");
    vecRobotronModels.push_back("player7_xyz_rgba_n.ply");

    vecRobotronModels.push_back("mommy1_xyz_rgba_n.ply");
    vecRobotronModels.push_back("mommy2_xyz_rgba_n.ply");
    vecRobotronModels.push_back("mommy3_xyz_rgba_n.ply");
    //vecRobotronModels.push_back("mommy4.ply");
    //vecRobotronModels.push_back("mommy5.ply");

    vecRobotronModels.push_back("hulk1_xyz_rgba_n.ply");
    vecRobotronModels.push_back("hulk2_xyz_rgba_n.ply");
    vecRobotronModels.push_back("hulk3_xyz_rgba_n.ply");

    vecRobotronModels.push_back("daddy1_xyz_rgba_n.ply");
    vecRobotronModels.push_back("daddy2_xyz_rgba_n.ply");
    //vecRobotronModels.push_back("daddy3.ply");
    //vecRobotronModels.push_back("daddy4.ply");
    //vecRobotronModels.push_back("daddy5.ply");

    vecRobotronModels.push_back("grunt1_xyz_rgba_n.ply");
    vecRobotronModels.push_back("grunt2_xyz_rgba_n.ply");
    vecRobotronModels.push_back("grunt3_xyz_rgba_n.ply");

    //vecRobotronModels.push_back("enforcer1.ply");
    //vecRobotronModels.push_back("enforcer2.ply");
    //vecRobotronModels.push_back("enforcer3.ply");
    //vecRobotronModels.push_back("enforcer4.ply");
    vecRobotronModels.push_back("enforcer5_xyz_rgba_n.ply");
    vecRobotronModels.push_back("enforcer6_xyz_rgba_n.ply");

    vecRobotronModels.push_back("brain1_xyz_rgba_n.ply");
    vecRobotronModels.push_back("brain2_xyz_rgba_n.ply");
    //vecRobotronModels.push_back("brain3.ply");
    //vecRobotronModels.push_back("brain4.ply");
    //vecRobotronModels.push_back("brain5.ply");

    vecRobotronModels.push_back("tank1_xyz_rgba_n.ply");
    vecRobotronModels.push_back("tank2_xyz_rgba_n.ply");
    vecRobotronModels.push_back("tank3_xyz_rgba_n.ply");
    vecRobotronModels.push_back("tank4_xyz_rgba_n.ply");
 //   vecRobotronModels.push_back("tank5_xyz_rgba_n.ply");

    for (std::string modelName : vecRobotronModels)
    {
        ::g_pMeshManager->LoadModelIntoVAO(modelName, shaderProgram);
    }

    // Place them everywhere
    const float DRAW_LIMIT = 500.0f;
    unsigned int numberOfModels = (unsigned int)vecRobotronModels.size();
    for (unsigned int count = 0; count != 100; count++)
    {
        cMesh* pTempMesh = new cMesh();
        pTempMesh->drawPosition.x = getRandomFloat(-DRAW_LIMIT, DRAW_LIMIT);
        pTempMesh->drawPosition.y = getRandomFloat(0.0f, DRAW_LIMIT);
        pTempMesh->drawPosition.z = getRandomFloat(-DRAW_LIMIT, DRAW_LIMIT);
        pTempMesh->bDoNotLight = true;

        pTempMesh->meshName = vecRobotronModels[rand() % numberOfModels];

        ::g_vec_pMeshesToDraw.push_back(pTempMesh);
    }



    ::g_pMeshManager->setBasePath(oldBasePath);

    return;
}

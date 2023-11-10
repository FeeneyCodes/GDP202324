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





static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


int main(void)
{

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


    cShaderManager* pShaderThing = new cShaderManager();
    pShaderThing->setBasePath("assets/shaders");

    cShaderManager::cShader vertexShader;
    vertexShader.fileName = "vertexShader01.glsl";

    cShaderManager::cShader fragmentShader;
    fragmentShader.fileName = "fragmentShader01.glsl";

    if ( ! pShaderThing->createProgramFromFile("shader01", vertexShader, fragmentShader ) )
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
    ::g_pTextureManager->Create2DTextureFromBMPFile("Water_Texture_01.bmp", true);
    ::g_pTextureManager->Create2DTextureFromBMPFile("Blank_UV_Text_Texture.bmp", true);


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


 
        // Time per frame (more or less)
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        // We should add this to a list of times, and get the average frame time
        const double LARGEST_DELTA_TIME = 1.0f / 30.0f; // 30FPS  (16 ms)
        if ( deltaTime > LARGEST_DELTA_TIME )
        {
            deltaTime = LARGEST_DELTA_TIME;
        }
//        std::cout << deltaTime << std::endl;
        lastTime = currentTime;


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
    // ***************************************************************
    //    GLuint TaylorSwiftTextureNumber = ::g_pTextureManager->getTextureIDFromName("TaylorSwift_Eras_Poster.bmp");
    GLuint Texture00 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[0]);
    if (Texture00 == 0)
    {
        Texture00 = ::g_pTextureManager->getTextureIDFromName("Blank_UV_Text_Texture.bmp");
    }

    // We are just going to pick texture unit 5 (for no reason, just as an example)
    //    glActiveTexture(GL_TEXTURE5);       // #5 TEXTURE UNIT
    glActiveTexture(GL_TEXTURE0 + 5);       // #5 TEXTURE UNIT
    glBindTexture(GL_TEXTURE_2D, Texture00);

    //uniform sampler2D texture_00;
    GLint texture_00_UL = glGetUniformLocation(shaderProgramID, "texture_00");
    glUniform1i(texture_00_UL, 5);     // <- 5, an integer, because it's "Texture Unit #5"
    // ***************************************************************

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

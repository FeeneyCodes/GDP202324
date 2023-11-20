#include "cMesh.h"
//#include "sPhsyicsProperties.h"
#include "cVAOManager/cVAOManager.h"
#include "cPhysics.h"
#include <vector>
#include "LuaBrain/cLuaBrain.h"

extern std::vector< cMesh* > g_vec_pMeshesToDraw;
//extern std::vector< sPhsyicsProperties* > g_vec_pPhysicalProps;
extern cPhysics* g_pPhysics;    //

extern cMesh* g_pDebugSphereMesh;

extern cVAOManager* g_pMeshManager;

// From main.cpp
extern cLuaBrain g_LuaBrain;
// Silly function binding example
void ChangeTaylorSwiftTexture(void);


float getRandomFloat(float a, float b);

void LoadLuaScripts(void)
{
//    g_LuaBrain.


    return;
}


bool LoadModels(void)
{
    LoadLuaScripts();

    ::g_pDebugSphereMesh = new cMesh();
    ::g_pDebugSphereMesh->meshName = "Sphere_1_unit_Radius.ply";
    ::g_pDebugSphereMesh->bIsWireframe = true;
    ::g_pDebugSphereMesh->bDoNotLight = true;
    ::g_pDebugSphereMesh->setUniformDrawScale(1.0f);
    ::g_pDebugSphereMesh->bIsVisible = false;
    ::g_pDebugSphereMesh->friendlyName = "DEBUG_SPHERE";
    // Note: we are NOT adding this to the vector of meshes


    cMesh* pBathtub = new cMesh();
    pBathtub->meshName = "bathtub_xyz_n_rgba_uv.ply";
    pBathtub->friendlyName = "bathtub";
    pBathtub->setUniformDrawScale(10.0f);
//    pBathtub->setRotationFromEuler(glm::vec3(45.0f, 0.0f, 0.0f));
    pBathtub->drawPosition = glm::vec3(0.0f, -30.0f, 0.0f);
    // 
    pBathtub->textureName[0] = "Water_Texture_01.bmp";
    pBathtub->textureRatios[0] = 1.0f;
    ::g_vec_pMeshesToDraw.push_back(pBathtub);



    cMesh* pGroundMesh = new cMesh();
//    pGroundMesh->meshName = "Terrain_xyz_n_rgba_uv.ply";
//    pGroundMesh->meshName = "Big_Flat_Mesh_256x256_00_132K_xyz_n_rgba_uv.ply";    
//    pGroundMesh->meshName = "Big_Flat_Mesh_256x256_07_1K_xyz_n_rgba_uv.ply";    
    pGroundMesh->meshName = "Big_Flat_Mesh_256x256_12_5_xyz_n_rgba_uv.ply";    
    pGroundMesh->drawPosition.y = -50.0f;
    pGroundMesh->drawPosition.z = -50.0f;
    pGroundMesh->friendlyName = "Ground";

    //pGroundMesh->bIsWireframe = true;
    //pGroundMesh->bDoNotLight = true;
    //pGroundMesh->wholeObjectDebugColourRGBA = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    //pGroundMesh->bUseDebugColours = true;
    //
//    pGroundMesh->textureName[0] = "NvF5e_height_map.bmp";
//    pGroundMesh->textureName[0] = "Blank_UV_Text_Texture.bmp";
    pGroundMesh->textureName[0] = "TaylorSwift_Eras_Poster.bmp";
    pGroundMesh->textureRatios[0] = 1.0f;

//    pGroundMesh->textureName[1] = "taylor-swift-jimmy-fallon.bmp";
//    pGroundMesh->textureRatios[1] = 0.0f;

    ::g_vec_pMeshesToDraw.push_back(pGroundMesh);




    const float MAX_SPHERE_LOCATION = 30.0f;
    const float MAX_VELOCITY = 1.0f;

    // Make a bunch of spheres...
    const unsigned int NUMBER_OF_SPHERES = 1;
    for (unsigned int count = 0; count != NUMBER_OF_SPHERES; count++)
    {
        cMesh* pSphereMesh = new cMesh();
//        pSphereMesh->meshName = "Sphere_1_unit_Radius.ply";
        pSphereMesh->meshName = "Sphere_1_unit_Radius_xyz_n_rgba_uv.ply";

        pSphereMesh->friendlyName = "Sphere";

        pSphereMesh->setUniformDrawScale(5.0f);

        // Add drawing mesh to the things to draw
        ::g_vec_pMeshesToDraw.push_back(pSphereMesh);

        // Now the physical object to match this
        sPhsyicsProperties* pSpherePhysProps = new sPhsyicsProperties();        // HEAP

        pSpherePhysProps->velocity.y = getRandomFloat(0.0f, MAX_VELOCITY);
        pSpherePhysProps->velocity.x = getRandomFloat(-MAX_VELOCITY, MAX_VELOCITY);
        pSpherePhysProps->velocity.z = getRandomFloat(-MAX_VELOCITY, MAX_VELOCITY);

        pSpherePhysProps->acceleration.y = (-9.81f / 5.0f);

        pSpherePhysProps->position.x = -10.0f;                      // getRandomFloat(-MAX_SPHERE_LOCATION, MAX_SPHERE_LOCATION);
        pSpherePhysProps->position.z = -10.0f;                        // getRandomFloat(-MAX_SPHERE_LOCATION, MAX_SPHERE_LOCATION);
        pSpherePhysProps->position.y = MAX_SPHERE_LOCATION; //  getRandomFloat(10.0f, MAX_SPHERE_LOCATION + 20.0f);

        // Mass of 10 somethings? kg?
        pSpherePhysProps->inverse_mass = 1.0f / 10.0f;


        pSpherePhysProps->shapeType = sPhsyicsProperties::SPHERE;

        // The rendered graphical object that moves with this physics object
        pSpherePhysProps->pTheAssociatedMesh = pSphereMesh;

//        sPhsyicsProperties::sSphere* pTemp = new sPhsyicsProperties::sSphere(1.0f);
//        pSpherePhysProps->setShape(pTemp);

        pSpherePhysProps->setShape( new sPhsyicsProperties::sSphere(5.0f) );
        ::g_pPhysics->AddShape(pSpherePhysProps);


    }//for ( unsigned int count...





//    float spiderScale = 10.0f;
//
//    cMesh* pSpiderMan_body = new cMesh();
//    pSpiderMan_body->setUniformDrawScale(spiderScale);
//    pSpiderMan_body->friendlyName = "SpiderManBody";
//    pSpiderMan_body->meshName = "legospiderman_body_xyz_n_rgba.ply";
////    pSpiderMan_body->drawOrientation.x = glm::radians(-90.0f);
////    pSpiderMan_body->drawOrientation.z = glm::radians(-90.0f);
//
//    pSpiderMan_body->adjustRoationAngleFromEuler( glm::vec3(-90.0f, 0.0f, 0.0f) );
////    pSpiderMan_body->adjustRoationAngleFromEuler( glm::vec3(0.0f, 0.0f, -90.0f) );
//
//
//
//    ::g_vec_pMeshesToDraw.push_back(pSpiderMan_body);
//
//    // Now add these as child meshes
//    cMesh* pSpiderMan_head = new cMesh();
//    pSpiderMan_head->setUniformDrawScale(spiderScale);
//    pSpiderMan_head->meshName = "legospiderman_head_xyz_n_rgba.ply";
//    pSpiderMan_body->vec_pChildMeshes.push_back(pSpiderMan_head);
////    ::g_vec_pMeshesToDraw.push_back(pSpiderManHead);
//
//    cMesh* pSpiderMan_Hips = new cMesh();
//    pSpiderMan_Hips->setUniformDrawScale(spiderScale);
//    pSpiderMan_Hips->meshName = "legospiderman_Hips_xyz_n_rgba.ply";
//    pSpiderMan_body->vec_pChildMeshes.push_back(pSpiderMan_Hips);
//
//    cMesh* pSpiderMan_Left_arm = new cMesh();
//    pSpiderMan_Left_arm->meshName = "legospiderman_Left_arm_xyz_n_rgba.ply";
//    pSpiderMan_Left_arm->setUniformDrawScale(spiderScale);
//    pSpiderMan_body->vec_pChildMeshes.push_back(pSpiderMan_Left_arm);
////    ::g_vec_pMeshesToDraw.push_back(pSpiderManLeft_arm);
//
//    cMesh* pSpiderMan_Left_hand = new cMesh();
//    pSpiderMan_Left_hand->meshName = "legospiderman_Left_hand_xyz_n_rgba.ply";
//    pSpiderMan_Left_hand->setUniformDrawScale(spiderScale);
//    pSpiderMan_body->vec_pChildMeshes.push_back(pSpiderMan_Left_hand);
//
//    cMesh* pSpiderMan_Right_arm = new cMesh();
//    pSpiderMan_Right_arm->meshName = "legospiderman_Right_arm_xyz_n_rgba.ply";
//    pSpiderMan_Right_arm->setUniformDrawScale(spiderScale);
//    pSpiderMan_body->vec_pChildMeshes.push_back(pSpiderMan_Right_arm);
//
//    cMesh* pSpiderMan_Right_hand = new cMesh();
//    pSpiderMan_Right_hand->meshName = "legospiderman_Right_hand_xyz_n_rgba.ply";
//    pSpiderMan_Right_hand->setUniformDrawScale(spiderScale);
//    pSpiderMan_body->vec_pChildMeshes.push_back(pSpiderMan_Right_hand);
//
//    cMesh* pSpiderMan_Left_leg = new cMesh();
//    pSpiderMan_Left_leg->setUniformDrawScale(spiderScale);
//    pSpiderMan_Left_leg->meshName = "legospiderman_Left_leg_xyz_n_rgba.ply";
//    pSpiderMan_body->vec_pChildMeshes.push_back(pSpiderMan_Left_leg);
////    ::g_vec_pMeshesToDraw.push_back(pSpiderManLeft_leg);
//
//    cMesh* pSpiderMan_Right_leg = new cMesh();
//    pSpiderMan_Right_leg->setUniformDrawScale(spiderScale);
//    pSpiderMan_Right_leg->meshName = "legospiderman_Right_leg_xyz_n_rgba.ply";
//    pSpiderMan_body->vec_pChildMeshes.push_back(pSpiderMan_Right_leg);
//
//
//
//
////    cMesh* pSpiderManHead = new cMesh();
////    pSpiderManHead->setUniformDrawScale(spiderScale);
////    pSpiderManHead->meshName = "legospiderman_head_xyz_n_rgba.ply";
////    ::g_vec_pMeshesToDraw.push_back(pSpiderManHead);
////
////    cMesh* pSpiderManLeft_arm = new cMesh();
////    pSpiderManLeft_arm->meshName = "legospiderman_Left_arm_xyz_n_rgba.ply";
////    ::g_vec_pMeshesToDraw.push_back(pSpiderManLeft_arm);
////
////    cMesh* pSpiderManLeft_leg = new cMesh();
////    pSpiderManLeft_leg->setUniformDrawScale(spiderScale);
////    pSpiderManLeft_leg->meshName = "legospiderman_Left_leg_xyz_n_rgba.ply";
////    ::g_vec_pMeshesToDraw.push_back(pSpiderManLeft_leg);













    return true;
}

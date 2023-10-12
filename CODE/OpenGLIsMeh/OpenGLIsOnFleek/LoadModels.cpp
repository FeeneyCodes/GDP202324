#include "cMesh.h"
//#include "sPhsyicsProperties.h"
#include "cPhysics.h"
#include <vector>

extern std::vector< cMesh* > g_vec_pMeshesToDraw;
//extern std::vector< sPhsyicsProperties* > g_vec_pPhysicalProps;
extern cPhysics* g_pPhysics;    //

extern cMesh* g_pDebugSphereMesh;

float getRandomFloat(float a, float b);

bool LoadModels(void)
{


    ::g_pDebugSphereMesh = new cMesh();
    ::g_pDebugSphereMesh->meshName = "Sphere_1_unit_Radius.ply";
    ::g_pDebugSphereMesh->bIsWireframe = true;
    ::g_pDebugSphereMesh->bDoNotLight = true;
    ::g_pDebugSphereMesh->setUniformDrawScale(1.0f);
    ::g_pDebugSphereMesh->bIsVisible = false;
    ::g_pDebugSphereMesh->friendlyName = "DEBUG_SPHERE";
    // Note: we are NOT adding this to the vector of meshes


        // bun_zipper_res2_xyz_n_rgba.ply
    // bathtub_xyz_n_rgba.ply

//    // Add some models to the "scene"
//    cMesh bunny1;   
//    bunny1.meshName = "bun_zipper_res2_xyz_n_rgba.ply";
//    bunny1.position = glm::vec3(-1.0f, 0.0f, 0.0f);
//    bunny1.scale = 10.0f;
//    bunny1.orientation.x = glm::radians(45.0f);
//
//    cMesh bunny2;
//    bunny2.meshName = "bun_zipper_res2_xyz_n_rgba.ply";
//    bunny2.position = glm::vec3(1.0f, 0.0f, 0.0f);
//    bunny2.scale = 7.5f;
//    bunny2.orientation.y = glm::radians(135.0f);
//
//    cMesh bathtub;
//    bathtub.meshName = "bathtub_xyz_n_rgba.ply";
//    //bunny2.position = glm::vec3(1.0f, 0.0f, 0.0f);
//    bathtub.scale = 0.25f;
//
////    // Smart array of cMesh object
////    std::vector<cMesh> vecMeshesToDraw;
//    g_vecMeshesToDraw.push_back(bunny1);
//    g_vecMeshesToDraw.push_back(bunny2);
//    g_vecMeshesToDraw.push_back(bathtub);
//    g_vecMeshesToDraw.push_back(terrain);

//    cMesh* pTerrain = new cMesh();
//    pTerrain->meshName = "Terrain_xyz_n_rgba.ply";
//    //bunny2.position = glm::vec3(1.0f, 0.0f, 0.0f);
//    pTerrain->scale = 1.0f;
//    pTerrain->drawPosition.y = -25.0f;
//    ::g_vec_pMeshesToDraw.push_back(pTerrain);


//    cMesh* pGridGroundMesh = new cMesh();
//    pGridGroundMesh->meshName = "Flat_Grid_100x100.ply";
    //pGridGroundMesh->bIsWireframe = true;
    //pGridGroundMesh->bDoNotLight = true;
    // note this does NOT have a physProps, so is ignored by the physics update loop
//    pGridGroundMesh->drawPosition.y = 0.0f;   //  0,-10,0
//    pGridGroundMesh->friendlyName = "Ground";
//    ::g_vec_pMeshesToDraw.push_back(pGridGroundMesh);

    cMesh* pGroundMesh = new cMesh();
//    pFlat_1x1_planeMesh->meshName = "Flat_1x1_plane.ply";
    pGroundMesh->meshName = "Terrain_xyz_n_rgba.ply";
//    pGroundMesh->meshName = "HilbertRamp_stl (rotated).ply";
    pGroundMesh->friendlyName = "Ground";
    ::g_vec_pMeshesToDraw.push_back(pGroundMesh);

    // Add matching physics object
    sPhsyicsProperties* pGroundMeshShape = new sPhsyicsProperties();
    pGroundMeshShape->shapeType = sPhsyicsProperties::MESH_OF_TRIANGLES_INDIRECT;

//    pGroundMeshShape->setShape( new sPhsyicsProperties::sMeshOfTriangles_Indirect("HilbertRamp_stl (rotated).ply") );
    pGroundMeshShape->setShape( new sPhsyicsProperties::sMeshOfTriangles_Indirect(pGroundMesh->meshName) );



    // Tie this phsyics object to the associated mesh
    pGroundMeshShape->pTheAssociatedMesh = pGroundMesh;
    // If it's infinite, the physics intrator ignores it
    pGroundMeshShape->inverse_mass = 0.0f;  // Infinite, so won't move

//    pGroundMeshShape->acceleration.y = (-9.81f / 5.0f);

//    pGroundMeshShape->position.x = -10.0f;
    pGroundMeshShape->position.y = -40.0f;
    pGroundMeshShape->orientation.z = glm::radians(-45.0f);
    pGroundMeshShape->friendlyName = "Ground";
    ::g_pPhysics->AddShape(pGroundMeshShape);
   
    //pFlat_1x1_planeMesh->bIsWireframe = true;
    //pFlat_1x1_planeMesh->bDoNotLight = true;
    // note this does NOT have a physProps, so is ignored by the physics update loop
//    sTransformInfo transformPlane = pFlat_1x1_planeMesh->getTransformInfo();
//    transformPlane.position.y = -40.0f;   //  0,-10,0
//    transformPlane.position.x = -10.0f;   //  0,-10,0
//    pFlat_1x1_planeMesh->setTransformInfo(transformPlane);
//    pFlat_1x1_planeMesh->drawPosition.x = 10.0f;
//    pFlat_1x1_planeMesh->orientation.z = glm::radians(+12.0f);
    // Also add the physics items
//   sPhsyicsProperties* pFlat_1x1_plane = new sPhsyicsProperties();
//   pFlat_1x1_plane->pTheAssociatedMesh = pFlat_1x1_planeMesh;
//   ::g_vec_pPhysicalProps.push_back(pFlat_1x1_plane);

    cMesh* pFlat_1x1_planeMesh_DEBUG = new cMesh();
    pFlat_1x1_planeMesh_DEBUG->meshName = pGroundMesh->meshName;
    pFlat_1x1_planeMesh_DEBUG->bIsWireframe = true;
    pFlat_1x1_planeMesh_DEBUG->bDoNotLight = true;
    pFlat_1x1_planeMesh_DEBUG->bUseDebugColours = true;
    pFlat_1x1_planeMesh_DEBUG->wholeObjectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    pFlat_1x1_planeMesh_DEBUG->drawPosition = pGroundMeshShape->position;
    pFlat_1x1_planeMesh_DEBUG->drawOrientation = pGroundMeshShape->orientation;
    pFlat_1x1_planeMesh_DEBUG->setUniformDrawScale(1.001f);
    ::g_vec_pMeshesToDraw.push_back(pFlat_1x1_planeMesh_DEBUG);


    const float MAX_SPHERE_LOCATION = 30.0f;
    const float MAX_VELOCITY = 1.0f;

    // Make a bunch of spheres...
    const unsigned int NUMBER_OF_SPHERES = 1;
    for (unsigned int count = 0; count != NUMBER_OF_SPHERES; count++)
    {
        cMesh* pSphereMesh = new cMesh();
        pSphereMesh->meshName = "Sphere_1_unit_Radius.ply";

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


    return true;
}

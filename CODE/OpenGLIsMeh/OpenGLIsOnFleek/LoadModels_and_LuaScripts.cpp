#include "cMesh.h"
//#include "sPhsyicsProperties.h"
#include "cVAOManager/cVAOManager.h"
#include "cPhysics.h"
#include <vector>
#include "LuaBrain/cLuaBrain.h"
#include "cParticleSystem.h"
#include <sstream>
#include <iostream>
#include <fstream>

extern std::vector< cMesh* > g_vec_pMeshesToDraw;
//extern std::vector< sPhsyicsProperties* > g_vec_pPhysicalProps;
extern cPhysics* g_pPhysics;    //

extern cMesh* g_pDebugSphereMesh;

extern cVAOManager* g_pMeshManager;

// From main.cpp
extern cLuaBrain g_LuaBrain;
// Silly function binding example
void ChangeTaylorSwiftTexture(void);

// Our one and only emitter (for now, maybe)
extern cParticleSystem g_anEmitter;

extern cMesh* g_pParticleMeshModel;

extern cMesh* g_pOffscreenTextureQuad;



float getRandomFloat(float a, float b);

void LoadLuaScripts(void)
{
//    g_LuaBrain.


    return;
}


bool LoadModels(void)
{
    LoadLuaScripts();

#pragma region mining_complex
//    cMesh* pAsteroid_industrial = new cMesh();
//    pAsteroid_industrial->meshName = "asteroid_industrial_xyz_n_rgba_uv.ply";
//    ::g_vec_pMeshesToDraw.push_back(pAsteroid_industrial);
//
//    cMesh* pAsteroid_main = new cMesh();
//    pAsteroid_main->meshName = "asteroid_main_xyz_n_rgba_uv.ply";
//    ::g_vec_pMeshesToDraw.push_back(pAsteroid_main);
//
//    cMesh* pAsteroid_other = new cMesh();
//    pAsteroid_other->meshName = "asteroid_other_xyz_n_rgba_uv.ply";
//    ::g_vec_pMeshesToDraw.push_back(pAsteroid_other);
//
//    cMesh* pStuff_main_asteroid = new cMesh();
//    pStuff_main_asteroid->meshName = "stuff_main_asteroid_xyz_n_rgba_uv.ply";
//    ::g_vec_pMeshesToDraw.push_back(pStuff_main_asteroid);
//
//    for (cMesh* pCurMesh : ::g_vec_pMeshesToDraw)
//    {
//        pCurMesh->bIsVisible = true;
////        pCurMesh->bIsWireframe = true;
////        pCurMesh->bUseDebugColours = true;
////        pCurMesh->wholeObjectDebugColourRGBA = glm::vec4(1.0f);
//        pCurMesh->textureName[0] = "Yellow.bmp";
//        pCurMesh->textureRatios[0] = 1.0f;
//    }
#pragma endregion

    // Load the test dungeon
#pragma region Load_test_dungeon

    std::ifstream dungeonFile("assets/models/The Dark Labyrinth of Himundua the Lunatic 01 (cleaned).tsv");
    if ( dungeonFile.is_open() )
    {
        std::cout << "Generating dungeon...";

        // Scan to 2nd DUNGEON_BEGIN
        std::string token;
        while ( dungeonFile >> token )
        {
            if (token == "DUNGEON_BEGIN") break;
        }
        while ( dungeonFile >> token )
        {
            if (token == "DUNGEON_BEGIN") break;
        }

        std::vector< std::string > vecDungeon;
        while ( dungeonFile >> token )
        {
            if ( token == "DUNGEON_END" )
            {
                break;
            }
            vecDungeon.push_back(token);
//            std::cout << token << std::endl;
        }

        unsigned int numRows = (unsigned int)vecDungeon.size();
        unsigned int numCols = (unsigned int)vecDungeon[0].size();

        std::cout << "Dungeon is " << numRows + 1 << " (columns) x " << numCols + 1 << " (rows)" << std::endl;

        float cellSize = 1.0f;
        float xOffset = (-cellSize * numCols) / 2.0f;
        float yOffset = (-cellSize * numRows) / 2.0f;

        for ( unsigned int rowIndex = 0; rowIndex < numRows; rowIndex++ )
        {
            for ( unsigned int colIndex = 0; colIndex < numCols; colIndex++ )
            {
                char cell = (char)vecDungeon[rowIndex][colIndex];

                cMesh* pCell = new cMesh();

                pCell->drawPosition.x = xOffset + (colIndex * cellSize);
                pCell->drawPosition.y = yOffset + (rowIndex * cellSize);
                pCell->drawPosition.z = -50.0f;

                pCell->setUniformDrawScale(cellSize);

                pCell->bIsVisible = true;
                pCell->bUseDebugColours = true;

                switch ( cell )
                {
                case '.':   // It's a floor, Jim
                    pCell->meshName = "Flat_1x1_units.ply";
                    pCell->drawPosition.z -= (cellSize / 2.0f);
                    pCell->setDrawOrientation(glm::vec3(glm::radians(90.0f), 0.0f, 0.0f));
                    pCell->wholeObjectDebugColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
                    ::g_vec_pMeshesToDraw.push_back(pCell);
                    break;
                case 'X':
                    pCell->meshName = "Cube_1x1x1_xyz_n_rgba.ply";
                    pCell->wholeObjectDebugColourRGBA = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
                    ::g_vec_pMeshesToDraw.push_back(pCell);
                    break;  // It's a wall, Jim
                default:
                    // WTF, Jim?
                    std::cout << "Error: unexpected cell character at row: " << rowIndex << ", column: " << colIndex << std::endl;
                    delete pCell;
                }
            }//for ( unsigned int colIndex
        }//for ( unsigned int rowIndex
    }//if ( dungeonFile.is_open() )


#pragma endregion

    ::g_pDebugSphereMesh = new cMesh();
//    ::g_pDebugSphereMesh->meshName = "Sphere_1_unit_Radius.ply";
    ::g_pDebugSphereMesh->meshName = "Sphere_1_unit_Radius_xyz_n_rgba_uv.ply";
    ::g_pDebugSphereMesh->bIsWireframe = true;
    ::g_pDebugSphereMesh->bDoNotLight = true;
    ::g_pDebugSphereMesh->setUniformDrawScale(1.0f);
    ::g_pDebugSphereMesh->bIsVisible = false;
    ::g_pDebugSphereMesh->friendlyName = "DEBUG_SPHERE";
    // Note: we are NOT adding this to the vector of meshes


    cMesh* pBathtub = new cMesh();
//    pBathtub->meshName = "bathtub_xyz_n_rgba_uv.ply";
//    pBathtub->meshName = "bathtub_xyz_n_rgba_uv_x3_size.ply";
    pBathtub->meshName = "bathtub_xyz_n_rgba_uv_x3_size_Offset_in_Y.ply";
    pBathtub->friendlyName = "bathtub";
//    pBathtub->setUniformDrawScale(3.0f);
//    pBathtub->setRotationFromEuler(glm::vec3(45.0f, 0.0f, 0.0f));
//    pBathtub->drawPosition = glm::vec3(0.0f, -30.0f, 0.0f);
    // 
    pBathtub->textureName[0] = "Water_SpidermanUV_square.bmp";
    pBathtub->textureRatios[0] = 1.0f;

    pBathtub->drawPosition.x = 60.0f;

//    pBathtub->bIsVisible = false;

    ::g_vec_pMeshesToDraw.push_back(pBathtub);



    cMesh* pGroundMesh = new cMesh();
//    pGroundMesh->meshName = "Terrain_xyz_n_rgba_uv.ply";
//    pGroundMesh->meshName = "Big_Flat_Mesh_256x256_00_132K_xyz_n_rgba_uv.ply";    
//    pGroundMesh->meshName = "Big_Flat_Mesh_256x256_07_1K_xyz_n_rgba_uv.ply";    
    pGroundMesh->meshName = "Big_Flat_Mesh_256x256_12_5_xyz_n_rgba_uv.ply";    
    //pGroundMesh->drawPosition.y = -50.0f;
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
//    pGroundMesh->textureName[0] = "stickers-explosion-texture.bmp";
    pGroundMesh->textureRatios[0] = 1.0f;

//    pGroundMesh->textureName[1] = "taylor-swift-jimmy-fallon.bmp";
//    pGroundMesh->textureRatios[1] = 0.0f;

    ::g_vec_pMeshesToDraw.push_back(pGroundMesh);


    cMesh* pZeroYMesh = new cMesh();
    pZeroYMesh->meshName = "Big_Flat_Mesh_256x256_00_132K_xyz_n_rgba_uv.ply";
    pZeroYMesh->friendlyName = "pZeroYMesh";
    pZeroYMesh->drawPosition.z = -50.0f;
    pZeroYMesh->bIsWireframe = true;
    pZeroYMesh->bDoNotLight = true;
    pZeroYMesh->wholeObjectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    pZeroYMesh->bUseDebugColours = true;
    ::g_vec_pMeshesToDraw.push_back(pZeroYMesh);




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


    ::g_pParticleMeshModel = new cMesh();
    ::g_pParticleMeshModel->setUniformDrawScale(1.0f);
//    ::g_pParticleMeshModel->meshName = "legospiderman_head_xyz_n_rgba_uv.ply";
//    ::g_pParticleMeshModel->meshName = "legospiderman_head_xyz_n_rgba_uv_at_Origin.ply";
    ::g_pParticleMeshModel->meshName = "Quad_2_sided_aligned_on_XY_plane.ply";
//    ::g_pParticleMeshModel->textureName[0] = "SpidermanUV_square.bmp";
    ::g_pParticleMeshModel->textureName[0] = "stickers-explosion-texture.bmp";
    ::g_pParticleMeshModel->textureRatios[0] = 1.0f;
    //
    ::g_pParticleMeshModel->bDoNotLight = true;



    cParticleSystem::sEmitterInfo ballEmitter;
    ballEmitter.emitterPosition = glm::vec3(0.0f, 5.0f, 0.0f);
    // A little bit to the sides (x & z)
    // 5.0 to 10.0 veclocity 'up' (in y)
    ballEmitter.initVelocityMin = glm::vec3(-3.0f, 10.0f, -3.0f);
    ballEmitter.initVelocityMax = glm::vec3( 3.0f, 15.0f, 3.0f);

    // Have them tumble (change orientation) a little bit
//    ballEmitter.orientationChangeMinRadians = glm::vec3(-0.1f, -0.1f, -0.1f);
//    ballEmitter.orientationChangeMaxRadians = glm::vec3( 0.1f,  0.1f,  0.1f);
    ballEmitter.orientationChangeMinRadians = glm::vec3( 0.0f, 0.0f, -0.1f);
    ballEmitter.orientationChangeMaxRadians = glm::vec3( 0.0f, 0.0f,  0.1f);

    ballEmitter.uniformScaleChangeMin = 0.001f;
    ballEmitter.uniformScaleChangeMax = 0.003f;

    // A little bit of 'gravity'
    ballEmitter.constantForce = glm::vec3(0.0f, -4.0f, 0.0f);
    // 
    ballEmitter.minLifetime = 10.0f;
    ballEmitter.maxLifetime = 15.0f;
    ballEmitter.minNumParticlesPerUpdate = 1;
    ballEmitter.maxNumParticlesPerUpdate = 3;
    //
    ballEmitter.maxNumParticles = 20'000;

    ::g_anEmitter.InitializeParticles(ballEmitter);
    ::g_anEmitter.Enable();
//    ::g_anEmitter.Show_MakeVisible();


    // What's on the FBO??
    ::g_pOffscreenTextureQuad = new cMesh();
    ::g_pOffscreenTextureQuad->meshName = "Quad_2_sided_aligned_on_XY_plane.ply";
    ::g_pOffscreenTextureQuad->setUniformDrawScale(100.0f);
    ::g_pOffscreenTextureQuad->bDoNotLight = true;
    ::g_pOffscreenTextureQuad->bIsVisible = true;
    ::g_pOffscreenTextureQuad->drawPosition.x = +10.0f;

    ::g_pOffscreenTextureQuad->textureName[0] = "stickers-explosion-texture.bmp";
    ::g_pOffscreenTextureQuad->textureRatios[0] = 1.0f;



    cMesh* pTieFighter = new cMesh();
    pTieFighter->meshName = "Tie-Fighter_cleaned.ply";
    pTieFighter->textureName[0] = "stickers-explosion-texture.bmp";
    pTieFighter->textureRatios[0] = 1.0f;
    pTieFighter->drawPosition.y = 30.0f;
    pTieFighter->drawPosition.x = 30.0f;
    pTieFighter->friendlyName = "TieFighter";
    ::g_vec_pMeshesToDraw.push_back(pTieFighter);


    cMesh* wheel = new cMesh();
    wheel->meshName = "Wheel_15.ply";
    wheel->textureName[0] = "stickers-explosion-texture.bmp";
    wheel->textureRatios[0] = 1.0f;
    wheel->friendlyName = "Wheel";
//    ::g_vec_pMeshesToDraw.push_back(wheel);




    // Draw a ton of bunnies
    float boxSize = 100.0f;
    float boxStep = 10.0f;

    std::stringstream ssName;

    int bunnyCount = 0;

//    for ( float x = -boxSize; x <= boxSize; x += boxStep )
//    {
//        for ( float y = -boxSize; y <= boxSize; y += boxStep )
//        {
//            for ( float z = -(2.0f * boxSize); z <= 0; z += boxStep )
//            {
//                cMesh* pBunny = new cMesh();
////                pBunny->meshName = "bun_zipper_xyz_n_rgba_uv.ply";
////                pBunny->meshName = "bun_zipper_res4_xyz_n_rgba_uv.ply";
//                pBunny->textureName[0] = "stickers-explosion-texture.bmp";
//                pBunny->textureRatios[0] = 1.0f;
//                pBunny->setUniformDrawScale(30.0f);
//
//                pBunny->drawPosition.x = x;
//                pBunny->drawPosition.y = y;
//                pBunny->drawPosition.z = z;
//
//
//                //bun_zipper_xyz_n_rgba_uv.ply
//                //bun_zipper_res2_xyz_n_rgba_uv.ply
//                //bun_zipper_res3_xyz_n_rgba_uv.ply
//                //bun_zipper_res4_xyz_n_rgba_uv.ply
//
//                // Add some LOD info 
//                pBunny->vecLODs.push_back(cMesh::sLODInfo("bun_zipper_res4_xyz_n_rgba_uv.ply", FLT_MAX));
//                pBunny->vecLODs.push_back(cMesh::sLODInfo("bun_zipper_res3_xyz_n_rgba_uv.ply", 200.0f));
//                pBunny->vecLODs.push_back(cMesh::sLODInfo("bun_zipper_res2_xyz_n_rgba_uv.ply", 75.0f));
//                pBunny->vecLODs.push_back(cMesh::sLODInfo("bun_zipper_xyz_n_rgba_uv.ply", 25.0f));
//
//
//                std::stringstream ssName;
//                ssName << "Bunny_" << (int)x << "_" << (int)y << "_" << (int)z;
//                pBunny->friendlyName = ssName.str();
//
//                g_vec_pMeshesToDraw.push_back(pBunny);
//
//                bunnyCount++;
//            }
//        }
//
//    }

    std::cout << "Bunny count = " << bunnyCount << std::endl;


    return true;
}

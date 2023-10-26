
#include "cAssimpHelper.h"


// We include the ACTUAL header here, since the compiler needs
//  to know how large the class really is in order to create one. 
#include "cFileLoader_Imp.h"

//#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
//#include <assimp/postprocess.h>     // Post processing flags

AH::cFileLoader::cFileLoader()
{
    // Create the implementation class...
    this->m_pImp = new cFileLoader_Imp();
}

AH::cFileLoader::~cFileLoader()
{
    // Delete the implementation class
    delete this->m_pImp;

}

bool AH::cFileLoader::Load3DModelFile(cString filename)
{
    sPostProcessFlags postProcessOptionsDefaults;
    return this->m_pImp->Load3DModelFile(filename, postProcessOptionsDefaults);

}


bool AH::cFileLoader::Load3DModelFile(cString filename, sPostProcessFlags postProcessOptions)
{
    return this->m_pImp->Load3DModelFile(filename, postProcessOptions);
}

void AH::cFileLoader::SetBasePath(cString basepath_no_end_slash)
{
    this->m_pImp->SetBasePath(basepath_no_end_slash);
    return;
}

std::string AH::cFileLoader::getLastError(bool bAndClearErrors /*=true*/)
{
    return this->m_pImp->getLastError().c_str();
}

//AH::cScene::cScene()
//{
//}
//
//AH::cScene::~cScene()
//{
//    for (cMesh* pCurrentMesh : vecMeshes )
//    {
//        delete pCurrentMesh;
//    }
//}

//
void AH::cScene::sSceneFlags::DecodeSceneFlags(unsigned int flags)
{
    this->incomplete = (flags & AI_SCENE_FLAGS_INCOMPLETE);                 // 0x1
    this->validated = (flags & AI_SCENE_FLAGS_VALIDATED);                   // 0x2
    this->validation_warning = (flags & AI_SCENE_FLAGS_VALIDATION_WARNING); // 0x4
    this->non_verbose_format = (flags & AI_SCENE_FLAGS_NON_VERBOSE_FORMAT); // 0x8
    this->terrain = (flags & AI_SCENE_FLAGS_TERRAIN);                       // 0x10
    this->allow_shared = (flags & AI_SCENE_FLAGS_ALLOW_SHARED);             // 0x20
    return;
};






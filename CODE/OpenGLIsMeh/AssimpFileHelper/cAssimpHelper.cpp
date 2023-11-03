
#include "cAssimpHelper.h"


// We include the ACTUAL header here, since the compiler needs
//  to know how large the class really is in order to create one. 
#include "cFileLoader_Imp.h"

//#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
//#include <assimp/postprocess.h>     // Post processing flags




AH::cScene* AH::cFileLoader::pGetScenePointer(std::string filename)
{
    return this->m_pImp->pGetScenePointer(filename);
}

bool AH::cFileLoader::cloneScene(std::string filename, AH::cScene& theScene)
{
    return this->m_pImp->cloneScene(filename, theScene);
}


AH::cScene::cScene()
{

}

AH::cScene::~cScene()
{

}





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

bool AH::cFileLoader::Load3DModelFile(std::string filename)
{
    sPostProcessFlags postProcessOptionsDefaults;
    return this->m_pImp->Load3DModelFile(filename, postProcessOptionsDefaults);

}


bool AH::cFileLoader::Load3DModelFile(std::string filename, sPostProcessFlags postProcessOptions)
{
    return this->m_pImp->Load3DModelFile(filename, postProcessOptions);
}

void AH::cFileLoader::SetBasePath(std::string basepath_no_end_slash)
{
    this->m_pImp->SetBasePath(basepath_no_end_slash);
    return;
}

std::string AH::cFileLoader::getLastError(bool bAndClearErrors /*=true*/)
{
    return this->m_pImp->getLastError();
}


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





// *************************************************************************************

std::string AH::cMesh::getPrimitiveTypeString(void)
{
    switch (this->primitive_type)
    {
    case AH::cMesh::POINT:     // aiPrimitiveType_POINT = 0x1,
        return "POINT";
        break;
    case AH::cMesh::LINE:       // aiPrimitiveType_LINE = 0x2,
        return "LINE";
        break;
    case AH::cMesh::TRIANGLE:   // aiPrimitiveType_TRIANGLE = 0x4,
        return "TRIANGLE";			
        break;
    case AH::cMesh::POLYGON:    // aiPrimitiveType_POLYGON = 0x8,
        return "POLYGON";
        break;
    case AH::cMesh::NGON:       // aiPrimitiveType_NGONEncodingFlag = 0x10,
        return "NGON";
        break;
    case AH::cMesh::UNKNOWN_OR_UNDEFINED:// 0?
    default:
        return "UNKNOWN_OR_UNDEFINED";
        break;
    }
    // We'll never get here
    return "ERROR: Primitive type not defined (this shouldn't happen).";
}

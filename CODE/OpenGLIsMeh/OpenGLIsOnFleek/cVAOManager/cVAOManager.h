#ifndef _cVAOManager_HG_
#define _cVAOManager_HG_

// Will load the models and place them 
// into the vertex and index buffers to be drawn

#include <string>
#include <map>

#include "sModelDrawInfo.h"

class cVAOManager
{
public:

	bool LoadModelIntoVAO(std::string fileName, 
						  sModelDrawInfo &drawInfo, 
						  unsigned int shaderProgramID,
						  bool bIsDynamicBuffer = false);

	// Simpler one that doesn't need the draw info passed
	bool LoadModelIntoVAO(std::string fileName, 
						  unsigned int shaderProgramID);

	// We don't want to return an int, likely
	bool FindDrawInfoByModelName(std::string filename,
								 sModelDrawInfo &drawInfo);

	std::string getLastError(bool bAndClear = true);

	void setBasePath(std::string basePathWithoutSlash);
	std::string getBasePath(void);

	// Here you have to be a little careful about what you pass. 
	// It will use the vertex and element (triangle) buffer IDs 
	//	to update - i.e. OVERWRITE - the buffers with whatever
	//	you've got in the two pointers. 
	// You can get this information by calling FindDrawInfoByModelName()
	//	and using the drawInfo that is returned. Note the sDrawInfo is a
	//	COPY of the structure in the map, but the pointers to the 
	//	vertex and element buffers are the ONE AND ONLY COPIES that
	//	exist on the CPU side. So if you overwrite them, they're gone.
	bool UpdateVAOBuffers(std::string fileName,
						  sModelDrawInfo& updatedDrawInfo,
						  unsigned int shaderProgramID);


private:

	bool m_LoadTheFile_Ply_XYZ_N_RGBA(std::string theFileName, sModelDrawInfo& drawInfo);

	bool m_LoadTheFile_Ply_XYZ_N_RGBA_UV(std::string theFileName, sModelDrawInfo& drawInfo);

	std::map< std::string /*model name*/,
		      sModelDrawInfo /* info needed to draw*/ >
		m_map_ModelName_to_VAOID;

	std::string m_basePathWithoutSlash;

};

#endif	// _cVAOManager_HG_

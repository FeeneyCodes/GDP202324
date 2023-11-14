#include "Ply_File_Loader/CPlyFile5nt.h"
#include "Ply_File_Loader/CStringHelper.h"

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

int main(int argc, char* argv[])
{
	// Get the list of files.
	// Ignore the 1st command line param
	std::vector<std::string> vecFileList;
	for (int index = 1; index != argc; index++)
	{
		// F:\z_FS\3111_3D\2023\Local_SAM\CPXX (Bonus)\PLY\SM_Env_Basement_Ceiling_01.ply
		// C:\z_FS\3111_3D\2023\Local_SAM\CPXX (Bonus)\PLY\SM_Env_Basement_Ceiling_01.ply
		// 
		// Is this a ply file?
		std::string curParam(argv[index]);
//		std::cout << curParam << std::endl;
		std::string extension = curParam.substr(curParam.length() - 3, 3);
		std::transform(extension.begin(), extension.end(), extension.begin(), ::toupper);
//		std::cout << extension << std::endl;
		if (extension == "PLY")
		{
//			std::cout << "It's a ply file" << std::endl;
			std::string fileNoExtension = curParam.substr(0, curParam.length() - 4);
			vecFileList.push_back(fileNoExtension);
		}
	}

	if (vecFileList.empty())
	{
		std::cout << "I didn't see any ply files." << std::endl;
		return -1;
	}


	for (std::string modelName : vecFileList)
	{
		CPlyFile5nt* pPlyLoader = new CPlyFile5nt();

		std::wstring wFileToLoad = CStringHelper::ASCIIToUnicodeQnD(modelName);
		std::wstring wFileToSave = wFileToLoad;

		wFileToSave.append(L"_UV.ply");

		wFileToLoad.append(L".ply");

		std::wstring wErrors;
		if (!pPlyLoader->OpenPLYFile2(wFileToLoad, wErrors))
		{
			std::wcout << "Error: Can't load >" << wFileToLoad << "< because: " << wErrors << std::endl;
			// 
			continue;
		}

		std::wcout << "Loaded " << wFileToLoad << "< : ";
		std::wcout << pPlyLoader->GetNumberOfVerticies() << " vertices, " << pPlyLoader->GetNumberOfElements() << " elements" << std::endl;

		pPlyLoader->FixOutOfBoundsHogwartsUVs();

		pPlyLoader->SetUniformColour(CVector3f(1.0f, 1.0f, 1.0f));


		CPlyFile5nt::sSaveFileParams fileSaveParams;
		fileSaveParams.vecComments.push_back(L"Hogwarts_UV_fixer generated");
		fileSaveParams.vecComments.push_back(L"TextureFile Stone10.jpg");
		fileSaveParams.bIncludeNormals = true;
		fileSaveParams.bIncludeUVs = true;
		fileSaveParams.bInculdeRGBA = true;
		pPlyLoader->SavePlyFileASCII(wFileToSave, true, fileSaveParams);


	}//for (std::string modelName : vecFileList)

	system("pause");

	return 0;
}

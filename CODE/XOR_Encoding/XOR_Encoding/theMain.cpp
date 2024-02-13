#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


int main()
{
	std::ifstream inFile; ("Source_Text.txt");


	// Reads the text as binary
	inFile.open("Source_Text.txt", std::ios_base::binary);
//	inFile.open("XOR_Text.txt", std::ios_base::binary);

	// Scan to the end to get the size
	inFile.seekg(0, std::ios::end);
	std::ios::pos_type fileSize = inFile.tellg();
	// Return to start
	inFile.seekg(0, std::ios::beg);

	char* pRawData = new char[static_cast<unsigned int>(fileSize)];

	inFile.read(pRawData, fileSize);
	inFile.close();


	for ( unsigned int index = 0; index != fileSize; index++ )
	{
		char curChar = pRawData[index];

		// Here's the exclusive or part (XOR):
		curChar = curChar ^ 42;
//		curChar = curChar ^ 86;

		pRawData[index] = curChar;
	}

		// Reads the text as binary
	std::ofstream outFile;
	outFile.open("XOR_Text.txt", std::ios_base::binary);
//	outFile.open("Output_Text.txt", std::ios_base::binary);

	outFile.write(pRawData, fileSize);
	outFile.close();




	return 0;
}




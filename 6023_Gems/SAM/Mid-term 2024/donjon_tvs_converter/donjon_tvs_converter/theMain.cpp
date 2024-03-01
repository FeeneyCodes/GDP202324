#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <sstream>


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: donjon_tsv_converter tsvFile.tsv" << std::endl;
		std::cout << "(Or drop a tsv file onto the exe)" << std::endl;
		return -1;
	}

	std::string infileName(argv[1]);

	std::ifstream inFile;
	inFile.open(infileName.c_str(), std::ios_base::binary);
	if ( ! inFile.is_open() )
	{
		std::cout << "Error: Couldn't open " << infileName << std::endl;
		return -1;
	}

	inFile.seekg(0, std::ios::end);
	std::ios::pos_type fileSize = inFile.tellg();
	inFile.seekg(0, std::ios::beg);

	char* pRawFileData = new char[static_cast<unsigned int>(fileSize)];
	
	inFile.read( pRawFileData, fileSize);
	inFile.close();

	std::cout << "Read " << fileSize << " characters OK" << std::endl;

	std::vector< std::vector< std::string > > vec2DMaze;

	// See how big this map is
	unsigned int columns = 0;
	unsigned int rows = 0;

	// Read to the first LF
	for ( unsigned int index = 0; index != fileSize; index++ )
	{
		char curChar = pRawFileData[index];
		if ( curChar != '\n' )
		{
			columns++;
		}
		else
		{
			break;
		}
	}

	// Count the LFs
	for ( unsigned int index = 0; index != fileSize; index++ )
	{
		char curChar = pRawFileData[index];
		if ( curChar == '\n' )
		{
			rows++;
		}
	}

	unsigned int index = 0;
	bool bError = false;
	std::vector< std::string > curRow;
	do 
	{
		char curChar = pRawFileData[index];

		std::string curToken;

		bool bTokenRead = false;
		bool bEndOfLine = false;
		do 
		{
			curChar = pRawFileData[index];
			// End of line is a line-feed
			if ( curChar == '\n')
			{
				// End of line
				bEndOfLine = true;
				bTokenRead = true;
				vec2DMaze.push_back(curRow);
				curRow.clear();

			}
			else if ( curChar == '\t' )
			{
				bTokenRead = true;
				// Is this a valid tab?
				if ( curToken == "" )
				{	
					// Yes
					curRow.push_back(".");
				}
				else
				{
					// This is the tab after a multi-character token
					curRow.push_back(curToken);
					curRow.push_back(".");
				}
			}
			else
			{
				curToken += curChar;
			}

			index++;
		
		} while ( ! bTokenRead );
	
	
	}while ( index < fileSize);

	if ( bError )
	{
		std::cout << "Error: Read error" << std::endl;
		return -1;
	}

	// Delete the top row
	vec2DMaze.erase(vec2DMaze.begin());


	// The tab characters have all been replaced with a period "."
	// But below, I'm describing them as tabs, anyway
	// (that's what the original file had)

	// Delete any tab after a non-tab token
	// Either the item is just a tab: it's a wall or whatever.
	// Or it's some text + a tab (floor, door, etc.)
//	std::vector< std::vector< std::string > > vec2DMaze;
	for ( unsigned int row = 0; row != vec2DMaze.size(); row++ )
	{
		// Note that there will always be a tab AFTER a non-tab token
		std::vector< std::string > cleanRow;

		// Ignore the 1st token from the row (it's a tab)
		// (i.e. start at 1, not 0)
		for ( unsigned int col = 1; col != vec2DMaze[row].size(); col++ )
		{
			//// This token a tab?
			std::string curToken = vec2DMaze[row][col];
			if (curToken != "." )
			{
				cleanRow.push_back(curToken);
				// Advance past the tab
				col++;
			}
			else
			{
				cleanRow.push_back(".");
			}
		}
		vec2DMaze[row].clear();
		vec2DMaze[row] = cleanRow;
	}


	// Stores the frequncey of each token
	std::map< std::string, unsigned int > mapTokenCount;
	for ( unsigned int row = 0; row != vec2DMaze.size(); row++ )
	{
		for ( unsigned int col = 0; col != vec2DMaze[row].size(); col++ )
		{
			//// This token a tab?
			std::string curToken = vec2DMaze[row][col];

			std::map< std::string, unsigned int >::iterator itTokenCount = mapTokenCount.find(curToken);
			if ( itTokenCount == mapTokenCount.end() )
			{
				// New token
				mapTokenCount[curToken] = 1;
			}
			else
			{
				itTokenCount->second++;
			}
		}
	}

	// Add a border around the entire maze
	// 1st, add a "." to the left and right of each row
	for ( unsigned int row = 0; row != vec2DMaze.size(); row++ )
	{
		// Note that there will always be a tab AFTER a non-tab token
		vec2DMaze[row].insert( vec2DMaze[row].begin(), ".");
		vec2DMaze[row].push_back(".");
	}

	// Add a top and bottom boundary (of "." characters)
	std::vector< std::string > vecPaddingRow;
	for ( unsigned int count = 0; count != vec2DMaze[0].size(); count++ )
	{
		vecPaddingRow.push_back(".");
	}

	vec2DMaze.insert( vec2DMaze.begin(), vecPaddingRow );
	vec2DMaze.push_back( vecPaddingRow );


	std::stringstream ssOutput;

	ssOutput 
		<< "Dungeon map is " 
		<< vec2DMaze.size() + 1 << " rows by "
		<< vec2DMaze[0].size() + 1 << " columns in size." << std::endl
		<< std::endl;

	ssOutput 
		<< "It contains the following tokens:" << std::endl
		<< " (token)      Character     occurances" << std::endl
		<< "---------------------------------------" << std::endl;
	unsigned int tokenIndex = 0;
	const unsigned int ASCII_CHAR_OFFSET = 'a';
	std::map< std::string, char > mapLongToShortTokenLookup;
	for ( std::map< std::string, unsigned int >::iterator itTokenCount = mapTokenCount.begin(); 
		  itTokenCount != mapTokenCount.end(); itTokenCount++, tokenIndex++ )
	{
		ssOutput 
			<< itTokenCount->first << "\t";
		if ( itTokenCount->first == "." || itTokenCount->first == "F" )
		{
			ssOutput << itTokenCount->first;
		}
		else
		{
			// Print a single character instead of the multi character token
			ssOutput << char( tokenIndex + ASCII_CHAR_OFFSET );
			mapLongToShortTokenLookup[itTokenCount->first] = char( tokenIndex + ASCII_CHAR_OFFSET );
		}
		ssOutput << "\t" << itTokenCount->second;
		ssOutput << std::endl;
	}

	// Add the "." and "F" tokens
	mapLongToShortTokenLookup["."] = '.';
	mapLongToShortTokenLookup["F"] = 'F';
	
	ssOutput 
		<< std::endl;

	ssOutput << "Here's the cleaned up map:" << std::endl;
	ssOutput << std::endl;

	ssOutput << "DUNGEON_BEGIN" << std::endl;

	for ( unsigned int row = 0; row != vec2DMaze.size(); row++ )
	{
		for ( unsigned int col = 0; col != vec2DMaze[row].size(); col++ )
		{
			std::string curToken = vec2DMaze[row][col];
			// Look up the single character token
			ssOutput << mapLongToShortTokenLookup[curToken];
		}
		ssOutput << std::endl;
	}

	ssOutput << "DUNGEON_END" << std::endl;


	ssOutput << std::endl;
	ssOutput << "Here's dungeon with just X and . characters:" << std::endl;
	ssOutput << "(Like the maze generator you used before)" << std::endl;
	ssOutput << "(Where 'X' is a wall and '.' is a corridor)" << std::endl;
	ssOutput << std::endl;

	ssOutput << "DUNGEON_BEGIN" << std::endl;

	for ( unsigned int row = 0; row != vec2DMaze.size(); row++ )
	{
		for ( unsigned int col = 0; col != vec2DMaze[row].size(); col++ )
		{
			std::string curToken = vec2DMaze[row][col];

			if ( curToken == "." )
			{
				ssOutput << "X";
			}
			else
			{
				ssOutput << ".";
			}
		}
		ssOutput << std::endl;
	}

	ssOutput << "DUNGEON_END" << std::endl;




	std::cout << ssOutput.str();

	
	std::string outFileName = infileName.substr(0, infileName.find(".tsv") );
	outFileName += " (cleaned).tsv";

	std::ofstream outFile(outFileName);
	outFile << ssOutput.str();
	outFile.close();


	return 0;
}
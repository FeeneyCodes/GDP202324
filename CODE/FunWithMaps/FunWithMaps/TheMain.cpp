#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

int main()
{
	std::vector<std::string> vecNames;

	vecNames.push_back("Michael");		
	vecNames.push_back("Mary");			// <--- Iterator points to here
	vecNames.push_back("Mike");
	vecNames.push_back("Ali");
	vecNames.push_back("Robin");
	// "end()"

	for ( unsigned int index = 0; index != vecNames.size(); index++ )
	{
		std::cout << vecNames[index] << std::endl;
	}

	std::vector<std::string>::iterator itNames;
	itNames = vecNames.begin();

	for ( ; itNames != vecNames.end(); itNames++ )
	{
		std::cout << *itNames << std::endl;
	}

	// Sort assumes that "<" makes sense

	std::sort(vecNames.begin(), vecNames.end());

	for (unsigned int index = 0; index != vecNames.size(); index++)
	{
		std::cout << vecNames[index] << std::endl;
	}

	return 0;
}
#include "sqlite/sqlite3.h"
#include <iostream>


// callback for SQLite
static int callback(void* NotUsed, int argc, char** argv, char** azColName) 
{
	int i;
	for (i = 0; i < argc; i++) 
	{
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int main()
{
	// Make a database
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;

	sqlite3_initialize();

	rc = sqlite3_open("myDatabase.dat", &db);
	if (rc) 
	{
		std::cout << "Didn't open database" << std::endl;
		return -1;
	}

	std::string command;
	do 
	{
		std::cout << "SQL command?:";
		std::cin >> command;

		if ( command != "EXIT" )
		{
			rc = sqlite3_exec(db, command.c_str(), callback, 0, &zErrMsg);
			if (rc != SQLITE_OK) 
			{
				std::cout << "SQL error: " << zErrMsg << std::endl;
				sqlite3_free(zErrMsg);
			}
		}//if ( command != "EXIT" )

	} while (command != "EXIT");

	sqlite3_close(db);

	return 0;
}

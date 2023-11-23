#pragma once

#include <vector>
#include <string>

#include "iCommand.h"

class cCommandGroup
{
public:
	cCommandGroup();

	std::string friendlyName;
	unsigned int UniqueID;

	void AddSerialCommand(iCommand* pTheCommand);
	void AddParallelCommand(iCommand* pTheCommand);

	bool Update(double deltaTime);
	bool isDone(void);

private:
	std::vector< iCommand* > m_vecSerialCommands;
	// Points to the next command we're going to run
	std::vector< iCommand* >::iterator m_itNextSerialCommand;
	bool m_UpdateSerial(double deltaTime);
	bool m_isDoneSerial(void);



	std::vector< iCommand* > m_vecParallelCommands;
	bool m_UpdateParallel(double deltaTime);
	bool m_isDoneParallel(void);

	std::vector< cCommandGroup* > m_vecCommandGroups;
	
};

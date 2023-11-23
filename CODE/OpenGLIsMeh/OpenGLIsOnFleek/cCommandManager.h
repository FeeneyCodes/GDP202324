#pragma once

#include <vector>
#include "cCommandGroup.h"

class cCommandManager
{
public:
	cCommandManager();

	void AddSerialCommandGroup(cCommandGroup* pNewCommand);
	void ExecuteCommandGroup(cCommandGroup* pImmediateCommand);

	void AddForeverCommand(cCommandGroup* pNewCommand);
	void DeleteForeverCommand(cCommandGroup* pNewCommand);

	// ONLY does serial command group.
	// The parallel is handled by the Command Group
	void Update(double deltaTime);

private:
	// Serial commands. Run until isDone(), then remove or ignore
	std::vector< cCommandGroup* > m_vecCommandGroups;

	std::vector< cCommandGroup* > m_vecForeverCommands;

};

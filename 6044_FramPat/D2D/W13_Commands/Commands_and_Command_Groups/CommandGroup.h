#pragma once

#include <vector>

class CCommand;

class CCommandGroup {
public:
	void addSerial(CCommand * pCommand);
	void addParallel(CCommand * pCommand);
	void update();
	bool isDone();
	void addCommandGroup(const CCommandGroup & commandGroup);

private:
	std::vector<CCommand*> mSerialCommands;
	std::vector<CCommand*> mParallelCommands;
	std::vector<CCommandGroup> mCommandGroups;
};
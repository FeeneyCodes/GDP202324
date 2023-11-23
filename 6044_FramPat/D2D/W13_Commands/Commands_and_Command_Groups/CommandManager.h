#pragma once

#include <vector>

class CCommand;
class CCommandGroup;

class CCommandManager {
public:
	CCommandManager();

	void addCommandGroup(CCommandGroup * commandGroup);
	void beginCommandGroup();
	void addCommand(bool serial, CCommand * command);
	void endCommandGroup();
	void update();

private:
	CCommandGroup * mCommandGroup;
	std::vector<CCommandGroup*> mCommandGroups;
};
#include "global.h"

void CCommandGroup::addSerial(CCommand * pCommand) {
	mSerialCommands.push_back(pCommand);
}

void CCommandGroup::addParallel(CCommand * pCommand) {
	mParallelCommands.push_back(pCommand);
}

void CCommandGroup::update() {
	// Serial Commands
	if (!mSerialCommands.empty()) {
		CCommand * pCurrentCommand = *this->mSerialCommands.begin();
		pCurrentCommand->execute();

		if (pCurrentCommand->isDone()) {
			mSerialCommands.erase(mSerialCommands.begin());
			delete pCurrentCommand;
		}
	}

	// Parallel Commands
	bool hasParallelCommands = false;
	for (std::vector<CCommand*>::iterator itCMD = mParallelCommands.begin(); itCMD != mParallelCommands.end(); ++itCMD) {
		CCommand * pCurrentCommand = *itCMD;
		if (!pCurrentCommand->isDone()) {
			hasParallelCommands = true;
			pCurrentCommand->execute();
		}
	}

	if (!hasParallelCommands) {
		for (std::vector<CCommand*>::iterator itCMD = mParallelCommands.begin(); itCMD != mParallelCommands.end(); ++itCMD) {
			CCommand * pCurrentCommand = *itCMD;
			delete pCurrentCommand;
		}
		mParallelCommands.clear();
	}

	// Serial Command Groups
	if (!mCommandGroups.empty()) {
		mCommandGroups.begin()->update();

		if (mCommandGroups.begin()->isDone()) {
			mCommandGroups.erase(mCommandGroups.begin());
		}
	}
}

bool CCommandGroup::isDone() {
	bool isDone = true;

	if (!mCommandGroups.empty() || !mSerialCommands.empty() || !mParallelCommands.empty()) {
		isDone = false;
	}

	return isDone;
}

void CCommandGroup::addCommandGroup(const CCommandGroup & commandGroup) {
	mCommandGroups.push_back(commandGroup);
}
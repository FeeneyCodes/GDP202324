#include "global.h"

CCommandManager::CCommandManager() {
	mCommandGroup = new CCommandGroup();
}

void CCommandManager::addCommandGroup(CCommandGroup * commandGroup) {
	mCommandGroups.push_back(commandGroup);
}

void CCommandManager::beginCommandGroup() {
	mCommandGroup = new CCommandGroup();
}

void CCommandManager::addCommand(bool serial, CCommand * command) {
	if (serial) {
		mCommandGroup->addSerial(command);
	}
	else {
		mCommandGroup->addParallel(command);
	}
}

void CCommandManager::endCommandGroup() {
	mCommandGroups.push_back(mCommandGroup);
}

void CCommandManager::update() {
	if (!mCommandGroups.empty()) {
		(*mCommandGroups.begin())->update();

		if ((*mCommandGroups.begin())->isDone()) {
			mCommandGroups.erase(mCommandGroups.begin());
		}
	}
}
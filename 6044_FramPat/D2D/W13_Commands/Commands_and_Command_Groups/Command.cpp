#include "Command.h"

void CCommand::setModel(CModel * pModel) {
	mModel = pModel;
}

void CCommand::initialize() {

}

bool CCommand::execute() {
	return true;
}

bool CCommand::isDone() {
	return true;
}
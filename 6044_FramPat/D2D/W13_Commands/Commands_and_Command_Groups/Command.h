#pragma once

class CModel;

class CCommand {
public:
	CCommand() : mModel(nullptr) {};
	virtual ~CCommand() {};

	virtual void setModel(CModel * pModel);

	virtual void initialize();
	virtual bool execute();
	virtual bool isDone();

protected:
	CModel * mModel;
};
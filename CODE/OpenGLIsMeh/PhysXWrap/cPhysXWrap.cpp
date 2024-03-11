// PhysXWrap.cpp : Defines the functions for the static library.
//

#include "cPhysXWrap.h"

#include "cPhysXWrap_Imp.h"

cPhysXWrap::cPhysXWrap()
{
	this->m_pImp = new cPhysXWrap_Imp();

}

cPhysXWrap::~cPhysXWrap()
{
	delete this->m_pImp;
}


void cPhysXWrap::initPhysics(bool interactive, std::string PVD_Host_Address /*= "127.0.0.1"*/)
{
	this->m_pImp->initPhysics(interactive, PVD_Host_Address);


	return;
}

void cPhysXWrap::update(void)
{
	this->m_pImp->update();
	return;
}

void cPhysXWrap::cleanupPhysics(bool interactive)
{
	this->m_pImp->cleanupPhysics(interactive);

	return;
}



void exampleFunction(bool blah)
{
	// TODO: Here's an example function in a library
	return;
}
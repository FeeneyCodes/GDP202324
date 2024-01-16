#pragma once

#include "cPerson.h"

class cNode
{
public:
	cPerson thePerson;	// The data you really want
	cNode* pNextNode = NULL;
	cNode* pPriorNode = NULL;
};

class cLinkedList
{
public:
	cLinkedList();
	~cLinkedList();

	void insertAtCurrent(cPerson newPerson);
	cPerson getAtCurrent(void);

	bool moveNext(void);
	bool movePrior(void);

	void updateAtCurrent(cPerson updatedPerson);

// We might want these, too
	unsigned int getSize(void);

	void insertAtStart(cPerson newPerson);
	void insertAtEnd(cPerson newPerson);
	void moveToHead(void);
	void moveToTail(void);

private:
	cNode* pHead;
	cNode* pTail;

	void m_FindHeadNode(void);
	void m_FindTailNode(void);

	cNode* pCurrent;	// Where we are "pointing" to
	// If we insert, we insert here
	// If we read info, we read the data here, etc.


};
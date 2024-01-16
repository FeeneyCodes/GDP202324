#include "cLinkedList.h"

cLinkedList::cLinkedList()
{
	// At the start, there's nothing in this list
	this->pHead = NULL;
	// 
	this->pCurrent = NULL;
}

cLinkedList::~cLinkedList()
{

}


void cLinkedList::insertAtCurrent(cPerson newPerson)
{
	// Is there anything there, yet?
	if (this->pHead == NULL )
	{
		// No, the list is empty
		cNode* pNewNode = new cNode();
		pNewNode->thePerson = newPerson;
		//
		this->pHead = pNewNode;
		this->pCurrent = this->pHead;
		return;
	}

	// It's NOT the 1st node to be addded
	cNode* pNewNode = new cNode();
	pNewNode->thePerson = newPerson;
	
	// Update the pointers to this node
	pNewNode->pNextNode = this->pCurrent;
	pNewNode->pPriorNode = this->pCurrent->pPriorNode;

	// Update the current node
	this->pCurrent->pPriorNode = pNewNode;
	// Don't need to update the "next" node

	// Update current note to point to this new person
	this->pCurrent = pNewNode;



	return;
}


bool cLinkedList::moveNext(void)
{
	// Can I move "next"?
	if ( this->pCurrent->pNextNode == NULL )
	{
		return false;
	}
	// Yes, I can
	this->pCurrent = this->pCurrent->pNextNode;
	return true;
}

bool cLinkedList::movePrior(void)
{
	// Can I move "back"?
	if (this->pCurrent->pPriorNode == NULL)
	{
		return false;
	}
	// Yes, I can
	this->pCurrent = this->pCurrent->pPriorNode;
	return true;
}


cPerson cLinkedList::getAtCurrent(void)
{
	return this->pCurrent->thePerson;
}


// HACK: Good enough for rock-n-roll
// Scan from the current node until we find a node with a prior set to NULL
void cLinkedList::m_FindHeadNode(void)
{
	cNode* pPossibleHead = this->pCurrent;

	while (pPossibleHead->pPriorNode != NULL)
	{
		// Point to the "next" nodes
		pPossibleHead = pPossibleHead->pPriorNode;
	}
	// Found it
	this->pHead = pPossibleHead;

	return;
}


// HACK: Good enough for rock-n-roll
// Scan from the current node until we find a node with a next set to NULL
void cLinkedList::m_FindTailNode(void)
{
	cNode* pPossibleTail = this->pCurrent;

	while ( pPossibleTail->pNextNode != NULL )
	{
		// Point to the "next" nodes
		pPossibleTail = pPossibleTail->pNextNode;
	}
	// Found it
	this->pTail = pPossibleTail;
	return;
}


void cLinkedList::moveToHead(void)
{
	this->m_FindHeadNode();
	this->pCurrent = this->pHead;
	return;
}

void cLinkedList::moveToTail(void)
{
	this->m_FindTailNode();
	this->pCurrent = this->pTail;
	return;
}
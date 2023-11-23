#include "cCommandGroup.h"

cCommandGroup::cCommandGroup()
{
	//std::vector< iCommand*>::iterator m_itNextCommand;
	this->m_itNextSerialCommand = this->m_vecSerialCommands.begin();

}

bool cCommandGroup::isDone(void)
{
	if ( ! this->m_isDoneParallel() )
	{
		return false;
	}
	if ( ! this->m_isDoneSerial() ) 
	{
		return false;
	}
	// Both ARE done
	return true;
}



void cCommandGroup::AddSerialCommand(iCommand* pTheCommand)
{
	this->m_vecSerialCommands.push_back(pTheCommand);
	return;
}



bool cCommandGroup::m_UpdateSerial(double deltaTime)
{
	// Are there any more commands? 
	if (this->m_itNextSerialCommand != this->m_vecSerialCommands.end() )
	{
		// get pointer to this command by dereferncing the iterator
		
		iCommand* pCurrentCommand = *this->m_itNextSerialCommand;

		// Yes, there's a current command 
		pCurrentCommand->Update(deltaTime);

		// Done? 
		if ( pCurrentCommand->isDone() )
		{
			// Maybe delete it? (who "owns" this command)
			// (I'd think that the command group would "own" this 
			//  command - be "responsible for" - so would call delete
			pCurrentCommand->PostEnd();
			delete pCurrentCommand;
			// Move to next command
			this->m_itNextSerialCommand++;
			// Call PreStart()???
		}
	}//if (this->m_itNextSerialCommand

	// How do I know if all the commands are done
	// Is the iterator at the end of the vector? 
	if (this->m_itNextSerialCommand == this->m_vecSerialCommands.end())
	{
		return true;
	}

	// We're not done. there are more commands to do
	return false;
}

bool cCommandGroup::m_isDoneSerial(void)
{
	if (this->m_itNextSerialCommand == this->m_vecSerialCommands.end())
	{
		return true;
	}
	return false;
}








void cCommandGroup::AddParallelCommand(iCommand* pTheCommand)
{
	this->m_vecParallelCommands.push_back(pTheCommand);
	return;
}

bool cCommandGroup::m_UpdateParallel(double deltaTime)
{
	// Assume they are all done
	bool bAllDone = true;

	for ( std::vector< iCommand* >::iterator itCurCommand = this->m_vecParallelCommands.begin();
		  itCurCommand != this->m_vecParallelCommands.end(); 
		  itCurCommand++ )
	{

		iCommand* pThisCommand = *itCurCommand;

		// Is this done? 
		if ( ! pThisCommand->isDone() )
		{
			pThisCommand->Update(deltaTime);
			// This isn't done
			bAllDone = false;
		}
	}//for ( std::vector< iCommand* >::iterator


	return bAllDone;
}

bool cCommandGroup::m_isDoneParallel(void)
{
	// Assume they are all done
	bool bAllDone = true;

	for (std::vector< iCommand* >::iterator itCurCommand = this->m_vecParallelCommands.begin();
		 itCurCommand != this->m_vecParallelCommands.end();
		 itCurCommand++)
	{
		iCommand* pThisCommand = *itCurCommand;

		// Is this done? 
		if (!pThisCommand->isDone())
		{
			// This isn't done
			bAllDone = false;
		}
	}//for (std::vector< iCommand* >::iterator

	return bAllDone;
}









bool cCommandGroup::Update(double deltaTime)
{
	this->m_UpdateSerial(deltaTime);
	this->m_UpdateParallel(deltaTime);

	return this->isDone();
}
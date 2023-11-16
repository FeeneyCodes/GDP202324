#include "cLuaBrain.h"

#include <iostream>

#include "cGameObject.h"

#include <sstream>
#include <map>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

// Global function. 
class iCommand
{
public:
	virtual void DoThis(void) = 0;
};
class cMoveCommand : public iCommand
{
public:
	cMoveCommand(glm::vec3 position) 
	{ 
		std::cout << "MoveCommand created" << std::endl; 
		std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;
	}
	virtual void DoThis(void)
	{
		std::cout << "Hey";
	}
private:
	cMoveCommand() {};
};

//void CreateCommand(std::string commandType)
// But the signature of the Lua integration MUST be this signature
int CreateCommand(lua_State* L)
{
	// Grab the top thing from the stack:
	std::string commandType = lua_tostring(L, 1);
	glm::vec3 destination;
	destination.x = (float)lua_tonumber(L, 2);
	destination.y = (float)lua_tonumber(L, 3);
	destination.z = (float)lua_tonumber(L, 4);
	
	// There could be an instance of the factory class set here
	iCommand* pCommand = NULL; 
	if (commandType == "MoveCommand")
	{
		pCommand = new cMoveCommand(destination);
	}
	
	delete pCommand;

	// Am returning nothing, so don't push anything, and return 0

	return 0;
}

cLuaBrain::cLuaBrain()
{
	this->m_p_vecGOs = nullptr;

	// Create new Lua state.
	// NOTE: this is common to ALL script in this case
	this->m_pLuaState = luaL_newstate();

	luaL_openlibs(this->m_pLuaState);					/* Lua 5.3.3 */


	lua_pushcfunction( this->m_pLuaState, cLuaBrain::l_UpdateObject );
	lua_setglobal( this->m_pLuaState, "setObjectState" );

	lua_pushcfunction( this->m_pLuaState, cLuaBrain::l_GetObjectState );
	lua_setglobal( this->m_pLuaState, "getObjectState" );

	// Add another binding to the CreateCommand() function
	lua_pushcfunction( this->m_pLuaState, CreateCommand );
	lua_setglobal( this->m_pLuaState, "CreateCommand" );


	return;
}

cLuaBrain::~cLuaBrain()
{
	lua_close(this->m_pLuaState);
	return;
}


// Saves (and overwrites) any script
// scriptName is just so we can delete them later
void cLuaBrain::LoadScript( std::string scriptName, 
					        std::string scriptSource )
{
	this->m_mapScripts[scriptName] = scriptSource;
	return;
}


void cLuaBrain::DeleteScript( std::string scriptName )
{
	this->m_mapScripts.erase(scriptName);
	return;
}

// Passes a pointer to the game object vector
void cLuaBrain::SetObjectVector( std::vector< cGameObject* >* p_vecGOs )
{
	this->m_p_vecGOs = p_vecGOs;
	return;
}

// Call all the active scripts that are loaded
void cLuaBrain::Update(float deltaTime)
{
	// Here, you could pass a delta time and save it into the Lua state as a 
	// variable. On the 1st call, the variable will be created and loaded, 
	// but after that, it would just be updated. 

	// Uses the RunScriptImmediately() method

	std::stringstream ssDeltatime;
	ssDeltatime << "deltaTime = " << deltaTime;
	this->RunScriptImmediately(ssDeltatime.str().c_str());

	// At this point, there is a variable called "deltaTime" that's in Lua.



//	std::cout << "cLuaBrain::Update() called" << std::endl;
	for( std::map< std::string /*name*/, std::string /*source*/>::iterator itScript = 
		 this->m_mapScripts.begin(); itScript != this->m_mapScripts.end(); itScript++ )
	{
		// Pass the script into Lua and update
//		int error = luaL_loadbuffer(L, buffer, strlen(buffer), "line");

		std::string curLuaScript = itScript->second;

		int error = luaL_loadstring( this->m_pLuaState, 
									 curLuaScript.c_str() );

		if ( error != 0 /*no error*/)	
		{
			std::cout << "-------------------------------------------------------" << std::endl;
			std::cout << "Error running Lua script: ";
			std::cout << itScript->first << std::endl;
			std::cout << this->m_decodeLuaErrorToString(error) << std::endl;
			std::cout << "-------------------------------------------------------" << std::endl;
			continue;
		}

		// execute funtion in "protected mode", where problems are 
		//  caught and placed on the stack for investigation
		error = lua_pcall(this->m_pLuaState,	/* lua state */
						  0,	/* nargs: number of arguments pushed onto the lua stack */
						  0,	/* nresults: number of results that should be on stack at end*/
						  0);	/* errfunc: location, in stack, of error function. 
								   if 0, results are on top of stack. */
		if ( error != 0 /*no error*/)	
		{
			std::cout << "Lua: There was an error..." << std::endl;
			std::cout << this->m_decodeLuaErrorToString(error) << std::endl;

			std::string luaError;
			// Get error information from top of stack (-1 is top)
			luaError.append( lua_tostring(this->m_pLuaState, -1) );

			// Make error message a little more clear
			std::cout << "-------------------------------------------------------" << std::endl;
			std::cout << "Error running Lua script: ";
			std::cout << itScript->first << std::endl;
			std::cout << luaError << std::endl;
			std::cout << "-------------------------------------------------------" << std::endl;
			// We passed zero (0) as errfunc, so error is on stack)
			lua_pop(this->m_pLuaState, 1);  /* pop error message from the stack */

			continue;
		}

	}

	return;
}

// Runs a script, but doesn't save it (originally used to set the ObjectID)
void cLuaBrain::RunScriptImmediately(std::string script)
{

	int error = luaL_loadstring( this->m_pLuaState, 
								 script.c_str() );

	if ( error != 0 /*no error*/)	
	{
		std::cout << "-------------------------------------------------------" << std::endl;
		std::cout << "Error running Lua script: ";
		std::cout << this->m_decodeLuaErrorToString(error) << std::endl;
		std::cout << "-------------------------------------------------------" << std::endl;
		return;
	}

	// execute funtion in "protected mode", where problems are 
	//  caught and placed on the stack for investigation
	error = lua_pcall(this->m_pLuaState,	/* lua state */
						0,	/* nargs: number of arguments pushed onto the lua stack */
						0,	/* nresults: number of results that should be on stack at end*/
						0);	/* errfunc: location, in stack, of error function. 
								if 0, results are on top of stack. */
	if ( error != 0 /*no error*/)	
	{
		std::cout << "Lua: There was an error..." << std::endl;
		std::cout << this->m_decodeLuaErrorToString(error) << std::endl;

		std::string luaError;
		// Get error information from top of stack (-1 is top)
		luaError.append( lua_tostring(this->m_pLuaState, -1) );

		// Make error message a little more clear
		std::cout << "-------------------------------------------------------" << std::endl;
		std::cout << "Error running Lua script: ";
		std::cout << luaError << std::endl;
		std::cout << "-------------------------------------------------------" << std::endl;
		// We passed zero (0) as errfunc, so error is on stack)
		lua_pop(this->m_pLuaState, 1);  /* pop error message from the stack */
	}

	return;
}


// Called by Lua
// Passes object ID, new velocity, etc.
// Returns valid (true or false)
// Passes: 
// - position (xyz)
// - velocity (xyz)
// called "setObjectState" in lua
/*static*/ int cLuaBrain::l_UpdateObject( lua_State *L )
{
	int objectID = (int)lua_tonumber(L, 1);	/* get argument */
	
	// Exist? 
	cGameObject* pGO = cLuaBrain::m_findObjectByID(objectID);

	if ( pGO == nullptr )
	{	// No, it's invalid
		lua_pushboolean( L,  false );
		// I pushed 1 thing on stack, so return 1;
		return 1;	
	}

	// Object ID is valid
	// Get the values that lua pushed and update object
	pGO->x = (float)lua_tonumber(L, 2);	/* get argument */
	pGO->y = (float)lua_tonumber(L, 3);	/* get argument */
	pGO->z = (float)lua_tonumber(L, 4);	/* get argument */
	pGO->Vx = (float)lua_tonumber(L, 5);	/* get argument */
	pGO->Vy = (float)lua_tonumber(L, 6);	/* get argument */
	pGO->Vz = (float)lua_tonumber(L, 7);	/* get argument */

	lua_pushboolean( L, true );	// index is OK
	
	return 1;		// There were 7 things on the stack

}

// Passes object ID
// Returns valid (true or false)
// - position (xyz)
// - velocity (xyz)
// called "getObjectState" in lua
/*static*/ int cLuaBrain::l_GetObjectState( lua_State *L )
{
	int objectID = (int)lua_tonumber(L, 1);	/* get argument */
	
	// Exist? 
	cGameObject* pGO = cLuaBrain::m_findObjectByID(objectID);

	if ( pGO == nullptr )
	{	// No, it's invalid
		lua_pushboolean( L,  false );
		// I pushed 1 thing on stack, so return 1;
		return 1;	
	}

	// Object ID is valid
	lua_pushboolean( L, true );	// index is OK
	lua_pushnumber( L, pGO->x );		
	lua_pushnumber( L, pGO->y );		
	lua_pushnumber( L, pGO->z );		
	lua_pushnumber( L, pGO->Vx );		
	lua_pushnumber( L, pGO->Vy );		
	lua_pushnumber( L, pGO->Vz );		
	
	return 7;		// There were 7 things on the stack
}

/*static*/ 
std::vector< cGameObject* >* cLuaBrain::m_p_vecGOs;


// returns nullptr if not found
/*static*/ cGameObject* cLuaBrain::m_findObjectByID( int ID )
{
	for ( std::vector<cGameObject*>::iterator itGO = cLuaBrain::m_p_vecGOs->begin(); 
		  itGO != cLuaBrain::m_p_vecGOs->end(); itGO++ )
	{
		if ( (*itGO)->getID() == ID )
		{	// Found it!
			return (*itGO);
		}
	}//for ( std::vector<cGameObject*>::iterator itGO...
	// Didn't find it
	return nullptr;
}


std::string cLuaBrain::m_decodeLuaErrorToString( int error )
{
	switch ( error )
	{
	case 0:
		return "Lua: no error";
		break;
	case LUA_ERRSYNTAX:
		return "Lua: syntax error"; 
		break;
	case LUA_ERRMEM:
		return "Lua: memory allocation error";
		break;
	case LUA_ERRRUN:
		return "Lua: Runtime error";
		break;
	case LUA_ERRERR:
		return "Lua: Error while running the error handler function";
		break;
	}//switch ( error )

	// Who knows what this error is?
	return "Lua: UNKNOWN error";
}

#ifndef _cLuaBrain_HG_
#define _cLuaBrain_HG_

extern "C" {
#include <Lua5.4.6/lua.h>
#include <Lua5.4.6/lauxlib.h>
#include <Lua5.4.6/lualib.h>
}

#include <string>
#include <vector>
//#include "cGameObject.h"
#include <map>

// Forward declaration for cyclical reference
class cGameObject;

class cLuaBrain
{
public:
	// Init Lua and set callback functions
	cLuaBrain();
	~cLuaBrain();
	void LoadScript( std::string scriptName, 
					 std::string scriptSource );
	void DeleteScript( std::string scriptName );
	// Passes a pointer to the game object vector
//	void SetObjectVector( std::vector< cGameObject* >* p_vecGOs );
	// Call all the active scripts that are loaded
	void Update(float deltaTime);

	// Runs a script, but doesn't save it (originally used to set the ObjectID)
	void RunScriptImmediately(std::string script);
	// Called by Lua
	// Passes object ID, new velocity, etc.
	// Returns valid (true or false)
	// Passes: 
	// - position (xyz)
	// - velocity (xyz)
	// called "setObjectState" in lua
//	static int l_UpdateObject( lua_State *L );
	// Passes object ID
	// Returns valid (true or false)
	// - position (xyz)
	// - velocity (xyz)
	// called "getObjectState" in lua
//	static int l_GetObjectState( lua_State *L );

	static int l_ChangeTaylorSwiftTexture(lua_State* L);

private:
	std::map< std::string /*scriptName*/, 
			  std::string /*scriptSource*/ > m_mapScripts;

//	static std::vector< cGameObject* >* m_p_vecGOs;
	// returns nullptr if not found
//	static cGameObject* m_findObjectByID( int ID );

	lua_State* m_pLuaState;

	std::string m_decodeLuaErrorToString( int error );
};

#endif

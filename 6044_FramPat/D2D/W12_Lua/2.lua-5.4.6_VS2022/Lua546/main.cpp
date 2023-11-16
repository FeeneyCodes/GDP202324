// Simple Lua as an embedded interpreter...
// Taken from: https://www.lua.org/pil/24.1.html

#include <iostream>
#include <string>

    
extern "C" {
#include <Lua5.4.6/lua.h>
#include <Lua5.4.6/lauxlib.h>
#include <Lua5.4.6/lualib.h>
}

// A function that Lua can call:


lua_State* luaStart(void)
{
    lua_State* L = luaL_newstate();		/* opens Lua 5.3.4 */
	if ( L == NULL )
	{	// Out of memory
		return false;
	}
	// No way to check if this is working or not
	luaL_openlibs(L);					/* Lua 5.3.4 */

	return L;
}

void luaShutDown(lua_State* &L)
{
	// Note: if you shut down the thing with ctrl-c, then
	//	you will likely get an exception here, which 
	//	can be ignored. Keep in mind that you won't be 
	//	doing this kind of shutdown, normally.
	lua_close(L);
	return;
}

std::string decodeLuaErrorToString( int error )
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


int main(void)		//
{
	lua_State* L = luaStart();
	if ( L == NULL )
	{
		std::cout << "All is lost, forever LOST!! (because Lua didn't start)" << std::endl;
		return -1;
	}

	int error = 0;
	char buffer[256] = {0};

	std::cout << "Lua interpreter has started. crtl-C or crtl-Z+enter to exit." << std::endl;
	std::cout << "------------------------------------------------------------" << std::endl;

	// Set a global lua variable and value
	// Which we could use as an ID value for the object
	// This way we don't have to pass it every time.

	//std::string objectIDString = "ObjectID = 15";
	//luaL_loadbuffer(L, objectIDString.c_str(), objectIDString.length(), "line");
	//error = lua_pcall(L, 0,	0, 0);	
	//if ( error != 0 /*no error*/)	
	//{
	//	std::cout << "error";
	//}


	bool bKeepReadingLuaCode = true;
	while ( bKeepReadingLuaCode )
	{
		std::cin.getline( buffer, 256 );
		if ( ! std::cin.good() )
		{	// std::istream::failbit, eofbit, or badbit is set
			// i.e. we did a ctrl-Z, crtl-C, or something else
			bKeepReadingLuaCode = false;
			continue;
		}

		error = luaL_loadbuffer(L, buffer, strlen(buffer), "line");
		if ( error != 0 /*no error*/)	
		{
			std::cout << "Lua: There was an error..." << std::endl;
			std::cout << decodeLuaErrorToString(error) << std::endl;
			continue;
		}

		// execute funtion in "protected mode", where problems are 
		//  caught and placed on the stack for investigation
		error = lua_pcall(L,	/* lua state */
						  0,	/* nargs: number of arguments pushed onto the lua stack */
						  0,	/* nresults: number of results that should be on stack at end*/
						  0);	/* errfunc: location, in stack, of error function. 
								   if 0, results are on top of stack. */
		if ( error != 0 /*no error*/)	
		{
			std::cout << "Lua: There was an error..." << std::endl;
			std::cout << decodeLuaErrorToString(error) << std::endl;

			std::string luaError;
			// Get error information from top of stack (-1 is top)
			luaError.append( lua_tostring(L, -1) );
			std::cout << luaError << std::endl;
			// We passed zero (0) as errfunc, so error is on stack)
			lua_pop(L, 1);  /* pop error message from the stack */

			continue;
		}

    }//while ( bKeepReadingLuaCode )

//	lua_close(L);
	luaShutDown(L);

	std::cout << "------------------------------------------------------------" << std::endl;
	std::cout << "Lua is shut down. Have a nice day." << std::endl;

	return 0;
}


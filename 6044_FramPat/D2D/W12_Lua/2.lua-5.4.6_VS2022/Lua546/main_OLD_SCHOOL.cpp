// Simple Lua as an embedded interpreter...
// Taken from: https://www.lua.org/pil/24.1.html

#include <stdio.h>
#include <string.h>
    
extern "C" {
#include <Lua5.4.6/lua.h>
#include <Lua5.4.6/lauxlib.h>
#include <Lua5.4.6/lualib.h>
}

int main_OLD_SCHOOL (void)			// _OLD_SCHOOL
{
    char buff[256];
    int error;
    //lua_State *L = lua_open();		/* opens Lua */
    lua_State *L = luaL_newstate();		/* opens Lua 5.3.3 */
	luaL_openlibs(L);					/* Lua 5.3.3 */
	// These ones are for pre-5.3.3...
    //luaopen_base(L);             /* opens the basic library */
    //luaopen_table(L);            /* opens the table library */
    //luaopen_io(L);               /* opens the I/O library */
    //luaopen_string(L);           /* opens the string lib. */
    //luaopen_math(L);             /* opens the math lib. */
    
    while (fgets(buff, sizeof(buff), stdin) != NULL) 
	{
		error = luaL_loadbuffer(L, buff, strlen(buff), "line") ||
				lua_pcall(L, 0, 0, 0);
		if (error) 
		{
			fprintf(stderr, "%s", lua_tostring(L, -1));
			lua_pop(L, 1);  /* pop error message from the stack */
		}
    }
    
    lua_close(L);
    return 0;
}
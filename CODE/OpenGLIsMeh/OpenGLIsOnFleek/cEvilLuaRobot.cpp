#include "cEvilLuaRobot.h"


// Because this function is in this file, it's only "seen" by the Lua brain
//
// This would be bound to the lua brain with :
// 	lua_pushcfunction( this->m_pLuaState, LuaCallBack_KillAllBunnies);
//	lua_setglobal(this->m_pLuaState, "FindTheNearestHuman");

// This is hidden, but also doesn't "know" about any specific EvilLuaRobot
int LuaCallBack_FindTheNearestHuman(lua_State* L)
{



	return 0;
}
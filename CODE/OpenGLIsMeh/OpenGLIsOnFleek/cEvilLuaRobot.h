#pragma once

#include "cMesh.h"

#include "LuaBrain/cLuaBrain.h"

class cEvilLuaRobot : public cMesh
{
public:

	// this is specific to this class

	// This is visible to anything that uses cEvilLuaBrain
	static int LuaCallBack_FindTheMostDangerousHuman(lua_State* L);

	cLuaBrain myBrain;
};

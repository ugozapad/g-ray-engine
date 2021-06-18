#ifndef LUASCRIPTMANAGER_H
#define LUASCRIPTMANAGER_H

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>

class LuaScriptManager
{
public:
	void Init();
	void Shutdown();

	void DoFile(const std::string& filename);
	void DoFile(const std::string& pathname, const std::string& filename);

	lua_State* GetState() { return m_luaState; }

private:
	lua_State* m_luaState;
};

extern LuaScriptManager g_luaScriptManager;

#endif // !LUASCRIPTMANAGER_H

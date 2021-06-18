#ifndef LUASCRIPTMANAGER_H
#define LUASCRIPTMANAGER_H

#include "LuaPlusInternal.h"

#ifdef luaplus_assert
#undef luaplus_assert
#endif // luaplus_assert

#define luaplus_assert ASSERT

#include "LuaStackObject.h"
#include "LuaObject.h"
#include "LuaState.h"
#include "LuaTableIterator.h"
#include "LuaObject.inl"
#include "LuaStateOutFile.h"
#include "LuaStateOutString.h"
#include "LuaHelper.h"
#include "LuaAutoBlock.h"
#include "LuaStackTableIterator.h"
#include "LuaCall.h"
#include "LuaFunction.h"
#include "LuaPlusCD.h"

class LuaScriptManager
{
public:
	void Init();
	void Shutdown();

	void DoFile(const std::string& filename);
	void DoFile(const std::string& pathname, const std::string& filename);

	LuaPlus::LuaState& GetState() { return *m_luaState; }

private:
	LuaPlus::LuaState* m_luaState;
};

extern LuaScriptManager g_luaScriptManager;

#endif // !LUASCRIPTMANAGER_H

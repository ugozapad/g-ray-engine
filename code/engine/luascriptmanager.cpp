#include "stdafx.h"
#include "luascriptmanager.h"

using namespace LuaPlus;

namespace engine_lua_help
{
	static void include(const char* pathname, const char* filename)
	{
		g_luaScriptManager.DoFile(pathname, filename);
	}
}

class EngineWrapper
{
public:
	int msg(LuaState* state)
	{
		LuaStack args(state);
		Msg("%s", args[1].GetString());
		return 0;
	}


};

static EngineWrapper s_engineWrapper;
LuaScriptManager g_luaScriptManager;

void LuaScriptManager::Init()
{
	m_luaState = LuaState::Create();

	LuaObject globalObjects = m_luaState->GetGlobals();

	// Register include function.
	globalObjects.RegisterDirect("include", &engine_lua_help::include);

	// Create engine wrapper table and register callbacks.
	LuaObject engineWrapper = globalObjects.CreateTable("engine");
	engineWrapper.Register("msg", s_engineWrapper, &EngineWrapper::msg);
}

void LuaScriptManager::Shutdown()
{
	LuaState::Destroy(m_luaState);
}

#include "filesystem/filesystem.h"

void LuaScriptManager::DoFile(const std::string& filename)
{
	std::unique_ptr<DataStream> stream = IFileSystem::Instance()->OpenReadStream("scripts", filename);
	std::string filebuffer = ReadStreamToString(stream);
	stream.release();

	m_luaState->DoString(&filebuffer[0]);
}

void LuaScriptManager::DoFile(const std::string& pathname, const std::string& filename)
{
	std::unique_ptr<DataStream> stream = IFileSystem::Instance()->OpenReadStream(pathname, filename);
	std::string filebuffer = ReadStreamToString(stream);
	stream.release();

	m_luaState->DoString(&filebuffer[0]);
}


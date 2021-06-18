#include "stdafx.h"
#include "luascriptmanager.h"

class EngineWrapper
{
public:
	static void register_script(lua_State* L);
public:
	static void print(const char* msg);
};

void EngineWrapper::print(const char* msg)
{
	Msg("%s", msg);
}

void EngineWrapper::register_script(lua_State* L)
{
	using namespace luabind;

	module(L)
	[
		class_<EngineWrapper>("engine")
			.def("print", &EngineWrapper::print)
	];
}

LuaScriptManager g_luaScriptManager;

void include(const char* filename)
{
	g_luaScriptManager.DoFile(filename);
}

void RegisterSomeLuaShit(lua_State* L)
{
	using namespace luabind;

	module(L)
	[
		def("include", include)
	];
}

void LuaScriptManager::Init()
{
	m_luaState = lua_open();
	luabind::open(m_luaState);

	RegisterSomeLuaShit(m_luaState);
	EngineWrapper::register_script(m_luaState);
}

void LuaScriptManager::Shutdown()
{
}

#include "filesystem/filesystem.h"

void LuaScriptManager::DoFile(const std::string& filename)
{
	std::unique_ptr<DataStream> stream = IFileSystem::GetFileSystem()->OpenReadStream("scripts", filename);
	std::string filebuffer = ReadStreamToString(stream);
	stream.release();

	luaL_dostring(m_luaState, &filebuffer[0]);
}

void LuaScriptManager::DoFile(const std::string& pathname, const std::string& filename)
{
	std::unique_ptr<DataStream> stream = IFileSystem::GetFileSystem()->OpenReadStream(pathname, filename);
	std::string filebuffer = ReadStreamToString(stream);
	stream.release();

	luaL_dostring(m_luaState, &filebuffer[0]);
}


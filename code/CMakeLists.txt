set (CMAKE_CXX_STANDARD 11)

if (NOT MSVC)
	set (CMAKE_CXX_STANDARD 17)
endif()

file(GLOB ENGINE_SRC 
	"${CMAKE_CURRENT_SOURCE_DIR}/engine/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/engine/*.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/engine/*.h")

source_group("engine" FILES ${ENGINE_SRC})

file(GLOB FILESYSTEM_SRC 
	"${CMAKE_CURRENT_SOURCE_DIR}/filesystem/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/filesystem/*.h")

source_group("filesystem" FILES ${FILESYSTEM_SRC})

file(GLOB RENDER_SRC 
	"${CMAKE_CURRENT_SOURCE_DIR}/render/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/render/*.h")

source_group("render" FILES ${RENDER_SRC})

file(GLOB RENDER_GL_SRC 
	"${CMAKE_CURRENT_SOURCE_DIR}/render/gl/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/render/gl/*.h")

source_group("render\\gl" FILES ${RENDER_GL_SRC})

file(GLOB RENDER_IMGUI_SRC 
	"${CMAKE_CURRENT_SOURCE_DIR}/render/imgui/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/render/imgui/*.h")

source_group("render\\imgui" FILES ${RENDER_IMGUI_SRC})

file(GLOB GAME_SRC 
	"${CMAKE_CURRENT_SOURCE_DIR}/game/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/game/*.h")

source_group("game" FILES ${GAME_SRC})

file(GLOB GAME_PHYS_SRC 
	"${CMAKE_CURRENT_SOURCE_DIR}/game/phys/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/game/phys/*.h")

source_group("game\\phys" FILES ${GAME_PHYS_SRC})

file(GLOB EDITOR_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/editor/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/editor/*.h")

source_group("editor" FILES ${EDITOR_SRC})

add_executable(engine WIN32 
	"CMakeLists.txt"
	"stdafx.h"
	"stdafx.cpp"
	${ENGINE_SRC}
	${FILESYSTEM_SRC}
	${RENDER_SRC}
	${RENDER_GL_SRC}
	${RENDER_IMGUI_SRC}
	${GAME_SRC}
	${GAME_PHYS_SRC}
	${EDITOR_SRC}
)

target_include_directories(engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(engine PUBLIC "${CMAKE_SOURCE_DIR}/externals/glm")
target_include_directories(engine PUBLIC "${CMAKE_SOURCE_DIR}/externals/stb")
target_include_directories(engine PUBLIC "${CMAKE_SOURCE_DIR}/externals/include")
target_include_directories(engine PUBLIC "${CMAKE_SOURCE_DIR}/externals/lua/src")
target_include_directories(engine PUBLIC "${CMAKE_SOURCE_DIR}/externals/luabind")
target_include_directories(engine PUBLIC "${CMAKE_SOURCE_DIR}/externals/boost")
target_link_libraries(engine glad glfw yaml-cpp imgui luabind)

if(WIN32)
	target_link_libraries(engine Imm32 Dwmapi)
endif()

target_precompile_headers(engine PRIVATE "stdafx.h")

set_target_properties(engine PROPERTIES 
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

if (CMAKE_SIZEOF_VOID_P EQUAL 4)
	target_link_directories(engine PRIVATE "${CMAKE_SOURCE_DIR}/externals/libs/x32")
else()
	target_link_directories(engine PRIVATE "${CMAKE_SOURCE_DIR}/externals/libs/x64")
endif()


if (MSVC)
	target_link_libraries(engine debug "assimp-vc142-mtd.lib")
	target_link_libraries(engine optimized "assimp-vc142-mt.lib")
else()
	target_link_libraries(engine "libassimp.dll.a")
endif()

#if (MSVC)
#   set_target_properties(engine PROPERTIES COMPILE_FLAGS "/Yustdafx.h")
#   set_source_files_properties(stdafx.cpp PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
#endif(MSVC)
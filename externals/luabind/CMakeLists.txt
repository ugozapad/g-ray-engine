cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(luabind)

option(LUABIND_SHARED "Compile luabind as shared library" ON)

list(APPEND LUABIND_SRC
	src/class.cpp
	src/class_info.cpp
	src/class_registry.cpp
	src/class_rep.cpp
	src/create_class.cpp
	src/error.cpp
	src/exception_handler.cpp
	src/function.cpp
	src/inheritance.cpp
	src/link_compatibility.cpp
	src/object_rep.cpp
	src/open.cpp
	src/pcall.cpp
	src/scope.cpp
	src/stack_content_by_name.cpp
	src/weak_ref.cpp
	src/wrapper_base.cpp
)

if (LUABIND_SHARED)
	add_library(luabind SHARED ${LUABIND_SRC})
	target_compile_definitions(luabind PUBLIC  LUABIND_DYNAMIC_LINK
									   PRIVATE LUABIND_BUILDING)
else()
	add_library(luabind STATIC ${LUABIND_SRC})
endif()

target_include_directories(luabind PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(luabind PUBLIC "${CMAKE_SOURCE_DIR}/externals/boost")
target_link_libraries(luabind lua)
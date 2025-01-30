cmake_minimum_required(VERSION 3.15)

project(LuaBuild)

set(_LUA_VERSION 5.4.6)

set(_LUA_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lua")

set(_LUA_SOURCE_DIR "${_LUA_ROOT_DIR}")
set(_LUA_LIB_SRC
    "${_LUA_SOURCE_DIR}/lapi.c"
    "${_LUA_SOURCE_DIR}/lcode.c"
    "${_LUA_SOURCE_DIR}/lctype.c"
    "${_LUA_SOURCE_DIR}/ldebug.c"
    "${_LUA_SOURCE_DIR}/ldo.c"
    "${_LUA_SOURCE_DIR}/ldump.c"
    "${_LUA_SOURCE_DIR}/lfunc.c"
    "${_LUA_SOURCE_DIR}/lgc.c"
    "${_LUA_SOURCE_DIR}/llex.c"
    "${_LUA_SOURCE_DIR}/lmem.c"
    "${_LUA_SOURCE_DIR}/lobject.c"
    "${_LUA_SOURCE_DIR}/lopcodes.c"
    "${_LUA_SOURCE_DIR}/lparser.c"
    "${_LUA_SOURCE_DIR}/lstate.c"
    "${_LUA_SOURCE_DIR}/lstring.c"
    "${_LUA_SOURCE_DIR}/ltable.c"
    "${_LUA_SOURCE_DIR}/ltm.c"
    "${_LUA_SOURCE_DIR}/lundump.c"
    "${_LUA_SOURCE_DIR}/lvm.c"
    "${_LUA_SOURCE_DIR}/lzio.c"
    "${_LUA_SOURCE_DIR}/lauxlib.c"
    "${_LUA_SOURCE_DIR}/lbaselib.c"
    "${_LUA_SOURCE_DIR}/lcorolib.c"
    "${_LUA_SOURCE_DIR}/ldblib.c"
    "${_LUA_SOURCE_DIR}/liolib.c"
    "${_LUA_SOURCE_DIR}/lmathlib.c"
    "${_LUA_SOURCE_DIR}/loadlib.c"
    "${_LUA_SOURCE_DIR}/loslib.c"
    "${_LUA_SOURCE_DIR}/lstrlib.c"
    "${_LUA_SOURCE_DIR}/ltablib.c"
    "${_LUA_SOURCE_DIR}/lutf8lib.c"
    "${_LUA_SOURCE_DIR}/linit.c"
)
# Exclude Lua standalone interpreter and compiler if they exist in the source
list(FILTER _LUA_LIB_SRC EXCLUDE REGEX "lua.c$")
list(FILTER _LUA_LIB_SRC EXCLUDE REGEX "luac.c$")

set(_LUA_COMPILER_SRC ${_LUA_SOURCE_DIR}/luac.c)
set(_LUA_INTERPRETER_SRC ${_LUA_SOURCE_DIR}/lua.c)


# Include Lua header directory
include_directories(${_LUA_SOURCE_DIR})

# Create a static library for Lua
add_library(lua-static STATIC ${_LUA_LIB_SRC})
# Set the library output name to lua
set_target_properties(lua-static PROPERTIES OUTPUT_NAME lua)

target_include_directories(lua-static PUBLIC ${_LUA_SOURCE_DIR})

# Add an alias to use a namespace
add_library(lua::lua-static ALIAS lua-static)

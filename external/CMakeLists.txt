# Don't compile using shared libs
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# TIGR
# https://github.com/erkkah/tigr
include(TigrBuild.cmake)
target_link_libraries(main tigr::tigr-static)

# PugiXML
# https://github.com/zeux/pugixml
add_subdirectory(pugixml)
target_link_libraries(main pugixml-static)

# Lua
# https://github.com/lua/lua
include(LuaBuild.cmake)
target_link_libraries(main lua::lua-static)

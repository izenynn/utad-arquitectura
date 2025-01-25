# Don't compile using shared libs
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# TIGR
# https://github.com/erkkah/tigr
include(TigrBuild.cmake)
target_link_libraries(main tigr::tigr-static)

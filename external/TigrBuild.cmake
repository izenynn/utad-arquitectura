cmake_minimum_required(VERSION 3.15)

project(TigrBuild)

set(_TIGR_VERSION 3.1.0)

set(_TIGR_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tigr")

set(_TIGR_SOURCE_DIR "${_TIGR_ROOT_DIR}")
set(_TIGR_LIB_SRC
        "${_TIGR_SOURCE_DIR}/tigr.c"
)

# Include Tigr header directory
include_directories(${_TIGR_SOURCE_DIR})

# Create a static library for Tigr
add_library(tigr-static STATIC ${_TIGR_LIB_SRC})
# Set the library output name to tigr
set_target_properties(tigr-static PROPERTIES OUTPUT_NAME tigr)

target_include_directories(tigr-static PUBLIC ${_TIGR_SOURCE_DIR})

# Platform-specific libraries
if(WIN32)
    target_link_libraries(tigr-static PUBLIC opengl32 gdi32)
elseif(APPLE)
    target_link_libraries(tigr-static PUBLIC "-framework OpenGL" "-framework Cocoa")
elseif(UNIX)
    target_link_libraries(tigr-static PUBLIC GLU GL X11)
endif()

# Add an alias to use a namespace
add_library(tigr::tigr-static ALIAS tigr-static)
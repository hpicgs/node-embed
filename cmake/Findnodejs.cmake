
# NODEJS_FOUND - system has the node.js library
# NODEJS_INCLUDE_DIR - the node.js include directory
# NODEJS_LIBRARY - the node.js library name
if(NODEJS_NODE_INCLUDE_DIR AND NODEJS_V8_INCLUDE_DIR AND NODEJS_UV_INCLUDE_DIR AND NODEJS_LIBRARY)
set(NODEJS_FIND_QUIETLY TRUE)
endif()
find_path(NODEJS_NODE_INCLUDE_DIR node.h PATHS
"${CMAKE_SOURCE_DIR}/externals/node/src"
 NO_DEFAULT_PATH
)
find_path(NODEJS_V8_INCLUDE_DIR v8.h PATHS
"${CMAKE_SOURCE_DIR}/externals/node/deps/v8/include"
 NO_DEFAULT_PATH
)
find_path(NODEJS_UV_INCLUDE_DIR uv.h PATHS
"${CMAKE_SOURCE_DIR}/externals/node/deps/uv/include"
 NO_DEFAULT_PATH
)
find_library(NODEJS_LIBRARY NAMES node node.lib libnode.so.61 libnode.61.dylib
PATHS
    "${CMAKE_SOURCE_DIR}/externals/node/Release"                # Windows
    "${CMAKE_SOURCE_DIR}/externals/node/out/Release"            # Mac
    "${CMAKE_SOURCE_DIR}/externals/node/out/Release/lib"        # Linux 1
    "${CMAKE_SOURCE_DIR}/externals/node/out/Release/lib.target" # Linux 2
NO_DEFAULT_PATH
)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Node DEFAULT_MSG NODEJS_NODE_INCLUDE_DIR NODEJS_V8_INCLUDE_DIR NODEJS_UV_INCLUDE_DIR NODEJS_LIBRARY)
mark_as_advanced(NODEJS_NODE_INCLUDE_DIR NODEJS_V8_INCLUDE_DIR NODEJS_UV_INCLUDE_DIR NODEJS_LIBRARY)


# NODEJS_FOUND - system has the node.js library
# NODEJS_INCLUDE_DIR - the node.js include directory
# NODEJS_LIBRARY - the node.js library name
if(NODEJS_INCLUDE_DIR AND NODEJS_DEV_INCLUDE_DIR AND NODEJS_LIBRARY)
set(NODEJS_FIND_QUIETLY TRUE)
endif()
find_path(NODEJS_INCLUDE_DIR node.h PATHS
"${CMAKE_SOURCE_DIR}/externals/node-v9.0.0/include/node"
 NO_DEFAULT_PATH
)
find_path(NODEJS_DEV_INCLUDE_DIR node_lib.h PATHS
"${CMAKE_SOURCE_DIR}/externals/node-v9.0.0/src"
 NO_DEFAULT_PATH
)
find_library(NODEJS_LIBRARY NAMES node node.lib libnode.so.59 libnode.59.dylib
PATHS
    "${CMAKE_SOURCE_DIR}/externals/node-v9.0.0/Release"
    "${CMAKE_SOURCE_DIR}/externals/node-v9.0.0/out/Release/lib"
    "${CMAKE_SOURCE_DIR}/externals/node-v9.0.0/out/Release/lib.target"
NO_DEFAULT_PATH
)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Node DEFAULT_MSG NODEJS_INCLUDE_DIR NODEJS_LIBRARY)
mark_as_advanced(NODEJS_INCLUDE_DIR NODEJS_LIBRARY)

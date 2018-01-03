
# NODEJS_FOUND - system has the node.js library
# NODEJS_INCLUDE_DIR - the node.js include directory
# NODEJS_LIBRARY - the node.js library name
if(NODEJS_INCLUDE_DIR AND NODEJS_LIBRARY)
set(NODEJS_FIND_QUIETLY TRUE)
endif()
find_path(NODEJS_INCLUDE_DIR node.h PATHS
"${CMAKE_SOURCE_DIR}/externals/node-v9.0.0/include/node"
 NO_DEFAULT_PATH
)
find_library(NODEJS_LIBRARY NAMES node libnode.lib
PATHS
    "${CMAKE_SOURCE_DIR}/externals/node-v9.0.0/Release"
NO_DEFAULT_PATH
)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Node DEFAULT_MSG NODEJS_INCLUDE_DIR NODEJS_LIBRARY)
mark_as_advanced(NODEJS_INCLUDE_DIR NODEJS_LIBRARY)

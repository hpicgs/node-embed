
#
# Configuration for all sub-projects
#

# Generate version-header
configure_file(version.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME}/${META_PROJECT_NAME}-version.h)


#
# Sub-projects
#

# Libraries
set(IDE_FOLDER "")

# Applications
set(IDE_FOLDER "Applications")
add_subdirectory(node-qt-rss)
add_subdirectory(node-lib-qt-rss)
add_subdirectory(node-lib-cli)

# Examples
set(IDE_FOLDER "Examples")

# Tests
# if(OPTION_BUILD_TESTS)
#    set(IDE_FOLDER "Tests")
#    add_subdirectory(tests)
# endif()


#
# Deployment
#

# Deploy generated headers
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME} DESTINATION include COMPONENT dev)

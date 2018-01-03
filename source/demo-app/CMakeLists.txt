#
# External dependencies
#

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Quick Concurrent REQUIRED)


#
# Executable name and options
#

# Target name
set(target node-embed)

# Exit here if required dependencies are not met
message(STATUS "Application ${target}")


#
# Sources
#

set(headers
    RssFeed.h
)

set(sources
    main.cpp
    qml.qrc
    RssFeed.cpp
)


#
# Create executable
#

# Build executable
add_executable(${target}
    MACOSX_BUNDLE
    ${sources}
)

# Create namespaced alias
add_executable(${META_PROJECT_NAME}::${target} ALIAS ${target})


#
# Project options
#

set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
)


#
# Include directories
#

target_include_directories(${target}
    PRIVATE
    ${DEFAULT_INCLUDE_DIRECTORIES}
    ${PROJECT_SOURCE_DIR}/source/include
    ${PROJECT_SOURCE_DIR}/externals/node-v9.0.0/include/node
)


#
# Libraries
#

add_library( nodejs-lib SHARED IMPORTED)

set_target_properties( nodejs-lib
                      PROPERTIES IMPORTED_LOCATION
                      ${PROJECT_SOURCE_DIR}/externals/libnode.so.59 )

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LIBRARIES}
    Qt5::Core
    Qt5::Quick
    Qt5::Concurrent
    nodejs-lib
)

#
# Compile definitions
#

target_compile_definitions(${target}
    PRIVATE
    ${DEFAULT_COMPILE_DEFINITIONS}
)


#
# Compile options
#

target_compile_options(${target}
    PRIVATE
    ${DEFAULT_COMPILE_OPTIONS}
)


#
# Linker options
#

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LINKER_OPTIONS}
)


#
# Target Health
#

perform_health_checks(
    ${target}
    ${sources}
)


#
# Deployment
#

# Executable
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN} COMPONENT runtime
    BUNDLE  DESTINATION ${INSTALL_BIN} COMPONENT runtime
)

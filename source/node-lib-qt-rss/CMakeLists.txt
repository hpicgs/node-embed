#
# External dependencies
#

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Quick Concurrent REQUIRED)
find_package(nodejs REQUIRED)
find_package(cpplocate REQUIRED)


#
# Executable name and options
#

# Target name
set(target node-lib-qt-rss)

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
    MACOSX_BUNDLE_INFO_PLIST CustomMacOSXBundleInfo.plist.in
)


#
# Include directories
#

target_include_directories(${target}
    PRIVATE
    ${DEFAULT_INCLUDE_DIRECTORIES}
    ${PROJECT_SOURCE_DIR}/source/include
    ${NODEJS_NODE_INCLUDE_DIR}
    ${NODEJS_V8_INCLUDE_DIR}
    ${NODEJS_UV_INCLUDE_DIR}
)


#
# Libraries
#

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LIBRARIES}
    Qt5::Core
    Qt5::Quick
    Qt5::Concurrent
    ${NODEJS_LIBRARY}
    cpplocate::cpplocate
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

if(WIN32)
    # Copy node.dll
    add_custom_command(TARGET ${target} POST_BUILD # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        ${CMAKE_SOURCE_DIR}/externals/node/Release/node.dll      # <--this is in-file
        $<TARGET_FILE_DIR:${target}>)                 # <--this is out-file path

    # Copy Qt dependencies
    find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS ${Qt5_DIR}/../../../ ENV QTDIR PATH_SUFFIXES bin)
    add_custom_command(TARGET ${target} POST_BUILD
    COMMAND ${WINDEPLOYQT_EXECUTABLE} --no-patchqt --qmldir ${CMAKE_CURRENT_SOURCE_DIR} $<TARGET_FILE:${target}>)
endif()

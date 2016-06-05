
#
# External dependencies
#

# find_package(THIRDPARTY REQUIRED)


# Enable automoc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(AUTOMOC_MOC_OPTIONS PROPERTIES FOLDER CMakeAutomocTargets)
set_property(GLOBAL PROPERTY AUTOMOC_FOLDER CMakeAutomocTargets)

# ENABLE CMP0020: Automatically link Qt executables to qtmain target on Windows.
cmake_policy(SET CMP0020 NEW)

#
# Library name and options
#
if (${CMAKE_BUILD_TYPE} AND ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        add_definitions(-DRESOURCES_ROOT="${CMAKE_CURRENT_SOURCE_DIR}/resources")
endif()

# Target name
set(target hyphawebmanager)

# Exit here if required dependencies are not met
message(STATUS "Exe ${target}")


# Set API export file and macro
string(TOUPPER ${target} target_upper)
set(feature_file "include/${target}/${target}_features.h")
set(export_file  "include/${target}/${target}_api.h")
set(export_macro "${target_upper}_API")


#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/")
set(source_path  "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(resource_path  "${CMAKE_CURRENT_SOURCE_DIR}/resources")


file(GLOB headers
"${include_path}/*.h"
"${include_path}/api/*.h"
)

file(GLOB sources
"${source_path}/*.cpp"
"${source_path}/api/*.cpp"
)

file(GLOB cpsp_sources
    "${source_path}/*.cpsp"
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$"
    ${header_group} ${headers})
source_group_by_path(${source_path}  "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$"
    ${source_group} ${sources})

foreach(CPSP_FILE ${cpsp_sources})
        get_filename_component(CPSP_FILE_BASE ${CPSP_FILE} NAME_WE)
        add_custom_command(
                OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${CPSP_FILE_BASE}.h ${CMAKE_CURRENT_BINARY_DIR}/${CPSP_FILE_BASE}.cpp
                COMMAND cpspc
                ARGS --output-dir="${CMAKE_CURRENT_BINARY_DIR}" ${CPSP_FILE}
                DEPENDS ${CPSP_FILE}
                COMMENT "Compile Servlet page ${CPSP_FILE}"
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

        set (CPSP_CXX ${CPSP_CXX} ${CMAKE_CURRENT_BINARY_DIR}/${CPSP_FILE_BASE}.cpp)
endforeach(CPSP_FILE)

#
# Create executable
#

# Build executable
add_executable(${target}
    MACOSX_BUNDLE
    ${sources}
    ${headers}
    ${CPSP_CXX}
    ${cpsp_sources}
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
    ${include_path}
    ${PROJECT_BINARY_DIR}/source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/source
    ${CMAKE_CURRENT_BINARY_DIR}

    PUBLIC
    ${DEFAULT_INCLUDE_DIRECTORIES}

    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)


#
# Libraries
#

target_link_libraries(${target}
    PRIVATE

    ${DEFAULT_LIBRARIES}
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
# Deployment
#
install(DIRECTORY ${resource_path} DESTINATION ${INSTALL_DATA} COMPONENT runtime)

# Executable
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN} COMPONENT runtime
    BUNDLE  DESTINATION ${INSTALL_BIN} COMPONENT runtime
)

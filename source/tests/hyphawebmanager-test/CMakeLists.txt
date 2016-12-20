
# 
# Executable name and options
# 

# Target name
set(target hyphawebmanager-test)
message(STATUS "Test ${target}" ${PROJECT_SOURCE_DIR})


# 
# Sources
# 

file(GLOB hypha_webmanager_sources
"${PROJECT_SOURCE_DIR}/source/hyphawebmanager/source/*.cpp"
"${PROJECT_SOURCE_DIR}/source/hyphawebmanager/source/api/*.cpp"
)

list(REMOVE_ITEM hypha_webmanager_sources ${PROJECT_SOURCE_DIR}/source/hyphawebmanager/source/main.cpp)

set(sources
    main.cpp
    hyphawebmanager_test.cpp
    ${hypha_webmanager_sources}
)

file(GLOB hypha_webmanager_headers
"${PROJECT_SOURCE_DIR}/source/hyphawebmanager/include/*.h"
"${PROJECT_SOURCE_DIR}/source/hyphawebmanager/include/api/*.h"
)

set(headers
    ${hypha_webmanager_headers}
)

file(GLOB cpsp_sources
    "${PROJECT_SOURCE_DIR}/source/hyphawebmanager/source/*.cpsp"
)

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
    ${DEFAULT_INCLUDE_DIRECTORIES}
    ${PROJECT_BINARY_DIR}/source/include
    ${PROJECT_SOURCE_DIR}/source/hyphawebmanager/include
    ${PROJECT_BINARY_DIR}/source/hyphawebmanager
)


# 
# Libraries
# 

if("${CMAKE_BUILD_TYPE}" STREQUAL "Coverage")
include(CodeCoverage)
set(libgcov "gcov")

setup_target_for_coverage(${META_PROJECT_NAME}_coverage ${target} coverage)
endif()

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LIBRARIES}
    ${Boost_LIBRARIES}
    ${Poco_LIBRARIES}
    hypha::controller hypha::core hypha::utils hypha::plugin hypha::handler
    libgmock
    ${libgcov}
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

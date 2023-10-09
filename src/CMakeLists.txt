# Get the latest abbreviated commit hash of the working branch
execute_process(
        COMMAND git describe --tags
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions( -DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}" )
set(MAYA_MODULE_NAME claudius-${GIT_COMMIT_HASH}-Maya-${MAYA_VERSION}-${CMAKE_SYSTEM_NAME})
set(MAYA_MODULE_FOLDER ${CMAKE_BINARY_DIR}/${MAYA_MODULE_NAME})
add_compile_definitions(MAYA_VERSION=${MAYA_VERSION})

add_subdirectory(utils)
add_subdirectory(io)
add_subdirectory(plugin)
add_subdirectory(procedural)
add_subdirectory(extension)


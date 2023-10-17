find_package(Arnold REQUIRED)
set(LIBRARY_NAME ArnoldCLI)
add_executable(${LIBRARY_NAME} arnold_cli.cpp)

target_include_directories(${LIBRARY_NAME} PUBLIC ${ARNOLD_INCLUDE_DIR})
target_link_directories(${LIBRARY_NAME} PUBLIC ${ARNOLD_LIBRARY_DIR})

target_link_libraries(${LIBRARY_NAME} ai)

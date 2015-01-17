project(Test)

# Compile gtest
add_subdirectory(gtest)
include_directories( 
    ${gtest_INCLUDE_DIR} 
    ${MBI_src_SOURCE_DIR}
)

set(TEST_SOURCES 
    test.cpp
)


add_executable(test ${TEST_SOURCES})
target_link_libraries(test gtest_main cpplib cudalib)
add_custom_command(TARGET test POST_BUILD COMMAND test)
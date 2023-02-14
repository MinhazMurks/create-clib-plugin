cmake_minimum_required(VERSION 3.24)
project(create_clib_plugin VERSION 0.0.1)

configure_file(create_clib_plugin.h.in create_clib_plugin.h)

include_directories("${PROJECT_BINARY_DIR}")
add_subdirectory("include/libgit2cpp")

add_executable(create_clib_plugin main.cpp create_clib_plugin.h.in)
target_link_libraries(create_clib_plugin git2cpp)
set_target_properties(
        create_clib_plugin PROPERTIES
        OUTPUT_NAME "create-clib-plugin"
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON
        CRT_FLAG_RELEASE "/MD"
)
target_include_directories(create_clib_plugin PUBLIC "include/libgit2cpp/libs/libgit2/include")
target_include_directories(create_clib_plugin PUBLIC "include/libgit2cpp/include")
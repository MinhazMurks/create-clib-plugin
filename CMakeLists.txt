cmake_minimum_required(VERSION 3.24)
SET(CREATE_CLIB_PLUGIN_VERSION 0.0.1 CACHE STRING "Version of libInterface")
project(create_clib_plugin VERSION ${CREATE_CLIB_PLUGIN_VERSION})

add_definitions( -DCREATE_CLIB_PLUGIN_VERSION=${CREATE_CLIB_PLUGIN_VERSION} )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include(FetchContent)
FetchContent_Declare(
        git2cpp
        GIT_REPOSITORY https://github.com/MinhazMurks/libgit2cpp.git
        GIT_TAG        9db97ce6a2f3da8d50193dc33e59d17835957eb2
)
FetchContent_MakeAvailable(git2cpp)


add_executable(create_clib_plugin main.cpp)
set_target_properties(create_clib_plugin PROPERTIES OUTPUT_NAME "create-clib-plugin")
target_link_libraries(create_clib_plugin git2cpp)
cmake_minimum_required(VERSION 3.10)

# set(CMAKE_EXE_LINKER_FLAGS"${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

project(examples)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_definitions(-DUNICODE -D_UNICODE)

option(USE_VCPKG "Use vcpkg for dependencies" OFF)
set(VCPKG_TARGET_TRIPLET "" CACHE STRING "Clearing vcpkg target triplet")
set(CMAKE_TOOLCHAIN_FILE "" CACHE PATH "Clearing toolchain file")

add_subdirectory(third-party)
add_subdirectory(duilib)
add_subdirectory(examples)
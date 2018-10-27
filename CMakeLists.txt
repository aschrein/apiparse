cmake_minimum_required(VERSION 2.8.12)
project(apiparse)

#set LLVM_DIR

find_package(LLVM REQUIRED)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}:")
message(STATUS "   - CMake module path: ${LLVM_CMAKE_DIR}")
message(STATUS "   - Include path     : ${LLVM_INCLUDE_DIRS}")
message(STATUS "   - Binary path      : ${LLVM_TOOLS_BINARY_DIR}")


list(APPEND CMAKE_MODULE_PATH ${LLVM_CMAKE_DIR})
#include(AddLLVM)

include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(apiparse main.cpp)
add_executable(example example.cpp)
add_executable(interceptor interceptor.cpp)

target_link_libraries(apiparse PRIVATE
    clangASTMatchers
    clangFrontend
    clangTooling
    clangParse
    clangSerialization
    clangSema
    clangEdit
    clangFormat
    clangLex
    clangAnalysis
    clangDriver
    clangAST
    clangToolingCore
    clangRewrite
    clangBasic
    LLVMProfileData
    LLVMSupport
    LLVMMCParser
    LLVMMC
    LLVMBitReader
    LLVMOption
    LLVMCore)
target_link_libraries(example PRIVATE
    d3d11
	d3dcompiler
	dxgi)

if ((LLVM_PACKAGE_VERSION VERSION_EQUAL "7") OR (LLVM_PACKAGE_VERSION VERSION_GREATER "7"))
    target_link_libraries(apiparse PRIVATE clangToolingInclusions)
endif()
if(WIN32)
    target_link_libraries(apiparse PRIVATE version)
endif()

if(MSVC)
    target_compile_options(apiparse PRIVATE "/Od /GR- /EHs- /EHc-")
    set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} /SUBSYSTEM:WINDOWS")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -fno-rtti -fvisibility-inlines-hidden")
endif()

install(TARGETS apiparse DESTINATION bin)
install(TARGETS example DESTINATION bin)
install(TARGETS interceptor DESTINATION bin)
configure_file(include_d3d11.cpp ${CMAKE_CURRENT_BINARY_DIR}/include_d3d11.cpp COPYONLY)
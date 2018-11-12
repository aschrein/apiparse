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

add_executable(apiparse main.cpp apiutil.py d3d11wrapgen.py)
add_executable(example
${CMAKE_CURRENT_BINARY_DIR}/dump.cpp
example.cpp)

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
target_include_directories(example PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)
configure_file(include_d3d11.cpp ${CMAKE_CURRENT_BINARY_DIR}/include_d3d11.cpp COPYONLY)
configure_file(apiutil.py ${CMAKE_CURRENT_BINARY_DIR}/apiutil.py COPYONLY)
configure_file(d3d11wrapgen.py ${CMAKE_CURRENT_BINARY_DIR}/d3d11wrapgen.py COPYONLY)
##################################
##################################
##################################
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


add_library (injectee_iat MODULE
    injectee_iat.cpp
	${CMAKE_CURRENT_BINARY_DIR}/d3d11wrapper.cpp
)

target_include_directories(injectee_iat PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)
# set_target_properties(injectee_iat PROPERTIES LINK_FLAGS "${LINK_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:DllMainStartup")
# target_compile_options(injectee_iat PRIVATE "/clr /EHs-")
# set_target_properties (injectee_iat PROPERTIES
    # PREFIX ""
# )  /NODEFAULTLIB /MD

# set_target_properties (injectee_iat PROPERTIES LINK_FLAGS "/ENTRY:DllMainStartup /NODEFAULTLIB")


install (TARGETS injectee_iat LIBRARY DESTINATION bin)
target_link_libraries(injectee_iat PRIVATE
    d3d11
	d3dcompiler
	dxgi)
string (REGEX REPLACE "/GR *" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")

# Disable C++ exceptions
add_definitions (-D_HAS_EXCEPTIONS=0)
string (REGEX REPLACE "/EHsc *" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHs-c-")

# Enable math constants defines
add_definitions (-D_USE_MATH_DEFINES)

# No min/max macros
add_definitions (-DNOMINMAX)

# Adjust warnings
add_definitions (-D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS)
add_definitions (-D_SCL_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
# foreach (flag_var
        # CMAKE_C_FLAGS_DEBUG CMAKE_CXX_FLAGS_DEBUG
    # )
        # if (${flag_var} MATCHES "/MTd")
            # string (REGEX REPLACE "/MTd" "/MDd" ${flag_var} "${${flag_var}}")
        # endif ()
		# if (${flag_var} MATCHES "/MD")
            # string (REGEX REPLACE "/MD" "/MDd" ${flag_var} "${${flag_var}}")
        # endif ()
# endforeach (flag_var)
# foreach (flag_var
        # CMAKE_C_FLAGS_RELEASE
        # CMAKE_CXX_FLAGS_RELEASE
    # )
        # if (${flag_var} MATCHES "/MTd")
            # string (REGEX REPLACE "/MTd" "/MD" ${flag_var} "${${flag_var}}")
        # endif ()
		# if (${flag_var} MATCHES "/MDd")
            # string (REGEX REPLACE "/MDd" "/MD" ${flag_var} "${${flag_var}}")
        # endif ()
# endforeach (flag_var)
# target_link_libraries (injectee_iat
    # #vcruntime
	# libucrt
	# libvcruntime
	# libucrt
	# msvcprt
	# msvcrt
	# libcmt
# )

add_executable (injector
    injector.cpp
)
set_target_properties (injector PROPERTIES
    OUTPUT_NAME inject
)
target_link_libraries (injector
    ${GETOPT_LIBRARIES}
)
install (TARGETS injector RUNTIME DESTINATION bin)

##################################
##################################
##################################
# add_library (d3d11shim MODULE
    # d3d11shim.cpp
# )
# link with vcruntimed.lib and ucrtd.lib
#set_target_properties (d3d11shim PROPERTIES LINK_FLAGS "/ENTRY:DllMainStartup")
# target_link_libraries (d3d11shim
	# vcruntime
    # vcruntimed
	# ucrtd
# )
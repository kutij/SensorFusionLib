PROJECT(SensorFusion)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8) # is enough??

#####################################################################################
### INSTALLING THIRD PARTIES
#####################################################################################
# Create 3rd party folder
set (CMAKE_3RDPARTY_DIR "${CMAKE_BINARY_DIR}/Third parties")
include( cmake/InstallThirdParties.cmake )

# TODO: add gtest

#####################################################################################
#####################Minden kell ebbõl?##############################################
# use folders in IDEs supporting it
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "")


if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
    #This is required to find sodium package for libzmq linking
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
endif()

# if(NOT LINUX) should work, too, if you need that
if(LINUX) 
	message(STATUS ">>> Linux")
    set (CMAKE_CXX_STANDARD 11)
else()
    if(WIN32)
        # for Windows operating system in general
        # on *nixes we need this to use C++11 
        if (MSVC_VERSION GREATER_EQUAL "1900")
            include(CheckCXXCompilerFlag)
            CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
            if (_cpp_latest_flag_supported)
                add_compile_options("/std:c++latest")
            endif()
        endif()
    endif()
endif()

# where to put the resulting binaries?
set (APPLICATION Bin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${APPLICATION})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${APPLICATION})
#####################################################################################
set (CMAKE_CONFIGURATION_TYPES Debug;Release;MinSizeRel;RelWithDebInfo CACHE STRING "Configuration types" FORCE)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

## Generate Compiler options
include (cmake/CompilerFlags.cmake)

option(ADD_PLOTTER "To use OpenCV-based plotters" FALSE)
set(FILTERPLOT ${ADD_PLOTTER}) # necessary?

add_subdirectory (Lib_Common)
if (FILTERPLOT)
	add_subdirectory (Lib_Plotter)
	add_subdirectory (App_Plotter)
endif()
add_subdirectory (Lib_SensorFusion)
add_subdirectory (Lib_Communication)
add_subdirectory (App_ZMQSub)
add_subdirectory (App_ZMQPub)
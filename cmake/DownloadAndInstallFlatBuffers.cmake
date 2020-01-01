set (CMAKE_3RDPARTY_FLATBUFFERS_SRC_DIR "${CMAKE_3RDPARTY_DIR}/Ext-FlatBuffers")
set (CMAKE_3RDPARTY_FLATBUFFERS_BUILD_DIR "${CMAKE_3RDPARTY_DIR}/Ext-FlatBuffers-build")
# FlatBuffers CLONE
set (FLATBUFFERS_TAG "v1.11.0")
message( STATUS "Downloading FlatBuffers ${FLATBUFFERS_TAG} (to ${CMAKE_3RDPARTY_FLATBUFFERS_SRC_DIR})...")
EXEC_PROGRAM( "git clone https://github.com/google/flatbuffers.git -b${FLATBUFFERS_TAG} \"${CMAKE_3RDPARTY_FLATBUFFERS_SRC_DIR}\"" )
message ( STATUS " succeeded. CMake configure started....")
# Call configure, generate and build (Release & Debug) on the externals
message(STATUS "Generating makefile/solution for FlatBuffers")
EXEC_PROGRAM(${CMAKE_COMMAND} \"${CMAKE_3RDPARTY_FLATBUFFERS_SRC_DIR}\" ARGS 
	\"${GENERATOR_ARGUMENT}\" \"-S${CMAKE_3RDPARTY_FLATBUFFERS_SRC_DIR}\" \"-B${CMAKE_3RDPARTY_FLATBUFFERS_BUILD_DIR}\"
	-DFLATBUFFERS_BUILD_FLATC=FALSE
	-DFLATBUFFERS_BUILD_FLATHASH=FALSE
	-DFLATBUFFERS_BUILD_FLATLIB=FALSE
	-DFLATBUFFERS_BUILD_TESTS=FALSE
	-DFLATBUFFERS_LIBXX_WITH_CLANG=FALSE
	-DCMAKE_INSTALL_PREFIX=\"${3RDPARTY_INSTALL_PREFIX}\"
	${EXTERNAL_FLAG_SETTER}
	)
message(STATUS "Installing FlatBuffers.")
EXEC_PROGRAM(${CMAKE_COMMAND} ARGS
	--install \"${CMAKE_3RDPARTY_FLATBUFFERS_BUILD_DIR}\" --config Release )
message(STATUS "done.")
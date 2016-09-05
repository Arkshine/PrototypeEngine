#
#	Engine library
#

include_directories(
	${CMAKE_SOURCE_DIR}/src/common
	${CMAKE_SOURCE_DIR}/src/engine
	${CMAKE_SOURCE_DIR}/external/HL_SDK/common
	${CMAKE_SOURCE_DIR}/external/HL_SDK/engine
	${CMAKE_SOURCE_DIR}/external/HL_SDK/public
	${CMAKE_SOURCE_DIR}/external/HL_SDK/public/math
)

add_sources(
	EngineInterface.h
	EngineInterface.cpp
)

add_subdirectory( ${CMAKE_SOURCE_DIR}/src/common common )

preprocess_sources()

add_library( Engine SHARED ${PREP_SRCS} )

target_compile_definitions( Engine PRIVATE
	${SHARED_DEFS}
)

#CMake places libraries in /Debug or /Release on Windows, so explicitly set the paths for both.
#On Linux, it uses LIBRARY_OUTPUT_DIRECTORY
#The engine is always in cl_dlls, since the client is always loaded from there, while liblist.gam points to it to match files.
set_target_properties( Engine PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY "${GAME_BASE_PATH}/cl_dlls"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${GAME_BASE_PATH}/cl_dlls"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${GAME_BASE_PATH}/cl_dlls"
)

if( WIN32 AND MSVC )
	#Set Windows subsystem
	set( ENGINE_LINK_FLAGS "/SUBSYSTEM:WINDOWS" )
else()
	set( ENGINE_LINK_FLAGS "" )
endif()

set_target_properties( Engine PROPERTIES COMPILE_FLAGS "${LINUX_32BIT_FLAG}" LINK_FLAGS "${ENGINE_LINK_FLAGS} ${LINUX_32BIT_FLAG}" )

#No lib prefix
set_target_properties( Engine PROPERTIES PREFIX "" )

#Needs to be named client so the original engine can load it.
#TODO: make a library that acts as a thunk to the real engine library.
set_target_properties( Engine PROPERTIES OUTPUT_NAME "client" )

#Create filters
create_source_groups( "${CMAKE_SOURCE_DIR}" )

#Set the startup project to the engine
#TODO: does this even work?
set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Engine )

clear_sources()

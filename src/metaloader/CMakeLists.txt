#
#	Metaloader library
#

include_directories(
	${CMAKE_SOURCE_DIR}/src/common
	${CMAKE_SOURCE_DIR}/src/metaloader
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/external/HL_SDK/common
	${CMAKE_SOURCE_DIR}/external/HL_SDK/engine
	${CMAKE_SOURCE_DIR}/external/HL_SDK/public
	${CMAKE_SOURCE_DIR}/external/HL_SDK/public/math
	${CMAKE_SOURCE_DIR}/external/SDL2/include
	${CMAKE_SOURCE_DIR}/external/SOURCE_SDK/src/public/steam
)

add_sources(
	CMetaLoader.h
	CMetaLoader.cpp
	EngineInterface.h
	EngineInterface.cpp
)

add_subdirectory( ${CMAKE_SOURCE_DIR}/external/HL_SDK/public HL_SDK/public )
add_subdirectory( ${CMAKE_SOURCE_DIR}/external/SOURCE_SDK/src/public/steam public/steam )
add_subdirectory( ${CMAKE_SOURCE_DIR}/src/steam steam )
add_subdirectory( ${CMAKE_SOURCE_DIR}/src/common common )

preprocess_sources()

link_directories( "${STEAMCOMMON}/Half-Life" )

add_library( MetaLoader SHARED ${PREP_SRCS} )

target_compile_definitions( MetaLoader PRIVATE
	${SHARED_DEFS}
	STEAM_API_NODLL
)

if( UNIX )
	if ( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
		set( SDL2_NAME "libSDL2-2.0.0.dylib" )
	else()
		set( SDL2_NAME "libSDL2-2.0.so.0" )
	endif()
else()
	set( SDL2_NAME "${CMAKE_SHARED_LIBRARY_PREFIX}SDL2${CMAKE_STATIC_LIBRARY_SUFFIX}" )
endif()

find_library( SDL2 ${SDL2_NAME} PATHS ${CMAKE_SOURCE_DIR}/external/SDL2/lib/ "${STEAMCOMMON}/Half-Life/" )

#Link with meta loader dependencies
target_link_libraries( MetaLoader 
	${SDL2}
	Tier1
	${UNIX_FS_LIB}
)

#CMake places libraries in /Debug or /Release on Windows, so explicitly set the paths for both.
#On Linux, it uses LIBRARY_OUTPUT_DIRECTORY
#The meta loader is always in cl_dlls, since the client is always loaded from there, while liblist.gam points to it to match files.
set_target_properties( MetaLoader PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY "${GAME_BASE_PATH}/cl_dlls"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${GAME_BASE_PATH}/cl_dlls"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${GAME_BASE_PATH}/cl_dlls"
)

if( WIN32 AND MSVC )
	#Set Windows subsystem
	#Add the module definition file. Needed for GiveFnptrsToDll function export.
	set( METALOADER_LINK_FLAGS "/SUBSYSTEM:WINDOWS /DEF:${CMAKE_CURRENT_SOURCE_DIR}/client.def /DELAYLOAD:Tier1.dll" )
else()
	set( METALOADER_LINK_FLAGS "" )
endif()

set_target_properties( MetaLoader 
	PROPERTIES COMPILE_FLAGS "${LINUX_32BIT_FLAG}" 
	LINK_FLAGS "${METALOADER_LINK_FLAGS} ${LINUX_32BIT_FLAG}" )

#No lib prefix
set_target_properties( MetaLoader PROPERTIES PREFIX "" )

#Needs to be named client so the original engine can load it.
set_target_properties( MetaLoader PROPERTIES OUTPUT_NAME "client" )

#Create filters
create_source_groups( "${CMAKE_SOURCE_DIR}" )

clear_sources()

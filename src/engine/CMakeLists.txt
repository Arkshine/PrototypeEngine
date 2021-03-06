#
#	Engine library
#

find_package( OpenGL REQUIRED )

if( NOT OPENGL_FOUND )
	MESSAGE( FATAL_ERROR "Could not locate OpenGL library" )
endif()

include_directories(
	${CMAKE_SOURCE_DIR}/src/common
	${CMAKE_SOURCE_DIR}/src/engine
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/external/HL_SDK/common
	${CMAKE_SOURCE_DIR}/external/HL_SDK/engine
	${CMAKE_SOURCE_DIR}/external/HL_SDK/public
	${CMAKE_SOURCE_DIR}/external/HL_SDK/public/math
	${CMAKE_SOURCE_DIR}/external/SDL2/include
	${CMAKE_SOURCE_DIR}/external/GLEW/include
	${CMAKE_SOURCE_DIR}/external/SOURCE_SDK/src/public/steam
	${CMAKE_SOURCE_DIR}/external/VGUI1/include
	${OPENGL_INCLUDE_DIR}
)

add_sources(
	CEngine.h
	CEngine.cpp
	CFileSystemWrapper.h
	CFileSystemWrapper.cpp
	CVideo.h
	CVideo.cpp
	Engine.h
	Engine.cpp
	EngineInterface.h
	EngineInterface.cpp
	#TODO: needs to be somewhere else. - Solokiller
	GLUtils.h
	GLUtils.cpp
)

add_subdirectory( ${CMAKE_SOURCE_DIR}/external/HL_SDK/public HL_SDK/public )
add_subdirectory( ${CMAKE_SOURCE_DIR}/external/SOURCE_SDK/src/public/steam public/steam )
add_subdirectory( ${CMAKE_SOURCE_DIR}/external/VGUI1/include VGUI1_include )
add_subdirectory( ${CMAKE_SOURCE_DIR}/src/steam steam )
add_subdirectory( ${CMAKE_SOURCE_DIR}/src/common common )
add_subdirectory( console )
add_subdirectory( VGUI1 )

preprocess_sources()

link_directories( "${STEAMCOMMON}/Half-Life" )

add_library( Engine SHARED ${PREP_SRCS} )

target_compile_definitions( Engine PRIVATE
	${SHARED_DEFS}
	STEAM_API_NODLL
	VERSION_SAFE_STEAM_API_INTERFACES
)

if( UNIX )
	if ( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
		set( VGUI1_NAME "vgui.dylib" )
	else()
		set( VGUI1_NAME "vgui.so" )
	endif()
else()
	set( VGUI1_NAME "vgui.lib" )
endif()

find_library( VGUI1 ${VGUI1_NAME} PATHS ${CMAKE_SOURCE_DIR}/external/VGUI1/lib/win32_vc6/ "${STEAMCOMMON}/Half-Life/" "${CMAKE_SOURCE_DIR}/lib/public" )

if( NOT WIN32 )
	#This mucking about is needed because CMake can't figure out how to handle libraries not named lib<name>.so
	if( NOT APPLE )
		set( VGUI1 "-l:\"${VGUI1_NAME}\"" )
	else()
		#Do nothing for OSX.
	endif()

endif()

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

find_library( GLEW2 glew32s ${CMAKE_SHARED_LIBRARY_PREFIX}GLEW${CMAKE_STATIC_LIBRARY_SUFFIX} PATHS ${CMAKE_SOURCE_DIR}/external/GLEW/lib/ )

#Link with engine dependencies
target_link_libraries( Engine 
	${SDL2}
	${GLEW2}
	${OPENGL_LIBRARIES}
	${VGUI1}
	Tier1
	${UNIX_FS_LIB}
)

#CMake places libraries in /Debug or /Release on Windows, so explicitly set the paths for both.
#On Linux, it uses LIBRARY_OUTPUT_DIRECTORY
set_target_properties( Engine PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY "${GAME_TOOL_PATH}"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${GAME_TOOL_PATH}"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${GAME_TOOL_PATH}"
)

if( WIN32 AND MSVC )
	#Set Windows subsystem
	set( ENGINE_LINK_FLAGS "/SUBSYSTEM:WINDOWS /DELAYLOAD:Tier1.dll" )
else()
	set( ENGINE_LINK_FLAGS "" )
endif()

set_target_properties( Engine 
	PROPERTIES COMPILE_FLAGS "${LINUX_32BIT_FLAG}" 
	LINK_FLAGS "${ENGINE_LINK_FLAGS} ${LINUX_32BIT_FLAG}" )

#No lib prefix
set_target_properties( Engine PROPERTIES PREFIX "" )

#Create filters
create_source_groups( "${CMAKE_SOURCE_DIR}" )

#Set the startup project to the engine
#TODO: does this even work?
set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Engine )

clear_sources()

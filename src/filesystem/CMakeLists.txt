#
#	FileSystem library
#	Shared library providing the SteamPipe filesystem. Implements the GoldSource IFileSystem interface.
#

include_directories(
	${CMAKE_SOURCE_DIR}/src/common
	${CMAKE_SOURCE_DIR}/external/HL_SDK/common
	${CMAKE_SOURCE_DIR}/external/HL_SDK/public
	${CMAKE_SOURCE_DIR}/external/SOURCE_SDK/src/public/steam
	${CMAKE_SOURCE_DIR}/external/SDL2/include
)

add_sources(
	CFileHandle.h
	CFileHandle.cpp
	CFileSystem.h
	CFileSystem.cpp
	CFileSystem.obsolete.cpp
	CPackFileEntry.h
	CSearchPath.h
	PackFile.h
)

add_subdirectory( ${CMAKE_SOURCE_DIR}/external/HL_SDK/public HL_SDK/public )
add_subdirectory( ${CMAKE_SOURCE_DIR}/src/common common )

preprocess_sources()

add_library( FileSystem SHARED ${PREP_SRCS} )

target_compile_definitions( FileSystem PRIVATE
	${SHARED_DEFS}
)

#TODO should avoid linking with SDL in more than one library
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

#Link with filesystem dependencies
target_link_libraries( FileSystem 
	${SDL2}
	${UNIX_FS_LIB}
)

#CMake places libraries in /Debug or /Release on Windows, so explicitly set the paths for both.
#On Linux, it uses LIBRARY_OUTPUT_DIRECTORY
set_target_properties( FileSystem PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY "${GAME_BIN_PATH}"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${GAME_BIN_PATH}"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${GAME_BIN_PATH}"
)

if( WIN32 AND MSVC )
	#Set Windows subsystem
	set( FILESYSTEM_LINK_FLAGS "/SUBSYSTEM:WINDOWS" )
else()
	set( FILESYSTEM_LINK_FLAGS "" )
endif()

set_target_properties( FileSystem 
	PROPERTIES COMPILE_FLAGS "${LINUX_32BIT_FLAG}" 
	LINK_FLAGS "${FILESYSTEM_LINK_FLAGS} ${LINUX_32BIT_FLAG}" )

#No lib prefix
set_target_properties( FileSystem PROPERTIES PREFIX "" )

#Create filters
create_source_groups( "${CMAKE_SOURCE_DIR}" )

clear_sources()

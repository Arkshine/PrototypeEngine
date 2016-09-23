#
#	Tier1 library
#	Load time linked library containing shared functionality (command line, logging)
#

include_directories(
	${CMAKE_SOURCE_DIR}/src/common
)

add_sources(
)

add_subdirectory( ${CMAKE_SOURCE_DIR}/src/common common )

preprocess_sources()

add_library( Tier1 SHARED ${PREP_SRCS} )

target_compile_definitions( Engine PRIVATE
	${SHARED_DEFS}
)

#CMake places libraries in /Debug or /Release on Windows, so explicitly set the paths for both.
#On Linux, it uses LIBRARY_OUTPUT_DIRECTORY
set_target_properties( Tier1 PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY "${GAME_BASE_PATH}"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${GAME_BASE_PATH}"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${GAME_BASE_PATH}"
)

if( WIN32 AND MSVC )
	#Set Windows subsystem
	set( ENGINE_LINK_FLAGS "/SUBSYSTEM:WINDOWS" )
else()
	set( ENGINE_LINK_FLAGS "" )
endif()

set_target_properties( Tier1 
	PROPERTIES COMPILE_FLAGS "${LINUX_32BIT_FLAG}" 
	LINK_FLAGS "${ENGINE_LINK_FLAGS} ${LINUX_32BIT_FLAG}" )

#No lib prefix
set_target_properties( Tier1 PROPERTIES PREFIX "" )

#Create filters
create_source_groups( "${CMAKE_SOURCE_DIR}" )

clear_sources()

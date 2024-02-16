
cmake_minimum_required(VERSION 3.5)
project(taradino
	DESCRIPTION "SDL2 port of Rise of the Triad"
	HOMEPAGE_URL "https://github.com/fabiangreffrath/taradino/"
	LANGUAGES C
	VERSION 1.1.3
)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

option(TARADINO_SHAREWARE "Build shareware version" OFF)
option(TARADINO_WERROR "Enable -Werror on build" OFF)
set(TARADINO_DATADIR "" CACHE STRING "Game data file directory")
set(TARADINO_SUFFIX "" CACHE STRING "Executable file name suffix")

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

file(GLOB TARADINO_SOURCES
	${PROJECT_SOURCE_DIR}/rott/*.c
)

add_executable(taradino ${TARADINO_SOURCES})
target_link_libraries(taradino PRIVATE SDL2::Main SDL2::Mixer)
target_compile_definitions(taradino PRIVATE PACKAGE_STRING="${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_VERSION}")
target_compile_definitions(taradino PRIVATE PACKAGE_TARNAME="${CMAKE_PROJECT_NAME}")

if(${TARADINO_SHAREWARE})
	target_compile_definitions(taradino PRIVATE SHAREWARE=1)
endif()

if(${TARADINO_WERROR} AND NOT MSVC)
	target_compile_options(taradino PRIVATE -Werror)
endif()

if(${TARADINO_DATADIR})
	target_compile_definitions(taradino PRIVATE DATADIR="${TARADINO_DATADIR}")
endif()

string(LENGTH "${TARADINO_SUFFIX}" SUFFIX_LEN)
if(SUFFIX_LEN GREATER 0)
	string(CONCAT NEWNAME "${CMAKE_PROJECT_NAME}" "-" "${TARADINO_SUFFIX}")
	if(WIN32)
		string(APPEND NEWNAME ".exe")
	endif()
	add_custom_command(TARGET taradino POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E rename
		"$<TARGET_FILE_DIR:taradino>/$<TARGET_FILE_NAME:taradino>"
		"$<TARGET_FILE_DIR:taradino>/${NEWNAME}")
endif()

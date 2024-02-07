
#
# cmake setup
#

cmake_minimum_required(VERSION 3.5)
project(tarradino
	DESCRIPTION "SDL2 port of Rise of the Triad"
	HOMEPAGE_URL "https://github.com/fabiangreffrath/tarradino/"
	LANGUAGES C
	VERSION 1.1.3
)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

#
# options
#

option(TARRADINO_SHAREWARE "Build shareware version" OFF)

#
# requirements
#

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

#
# sources
#

file(GLOB TARRADINO_SOURCES
	${PROJECT_SOURCE_DIR}/rott/*.c
)

#
# executable
#

add_executable(tarradino ${TARRADINO_SOURCES})
target_link_libraries(tarradino PUBLIC SDL2::Main SDL2::Mixer)
target_compile_definitions(tarradino PUBLIC PACKAGE_STRING="${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_VERSION}")
target_compile_definitions(tarradino PUBLIC PACKAGE_TARNAME="${CMAKE_PROJECT_NAME}")

if(${TARRADINO_SHAREWARE})
	target_compile_definitions(tarradino PUBLIC SHAREWARE=1)
endif()


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

if(WIN32)
	set(TARADINO_SOURCES ${TARADINO_SOURCES} ${PROJECT_SOURCE_DIR}/misc/taradino.rc)
endif()

string(LENGTH "${TARADINO_SUFFIX}" SUFFIX_LEN)
if(SUFFIX_LEN GREATER 0)
	string(CONCAT TARADINO_EXEC "${CMAKE_PROJECT_NAME}" "-" "${TARADINO_SUFFIX}")
else()
	set(TARADINO_EXEC ${CMAKE_PROJECT_NAME})
endif()

if(${TARADINO_SHAREWARE})
	set(TARADINO_TITLE "Taradino (Shareware)")
else()
	set(TARADINO_TITLE "Taradino")
endif()

add_executable(${TARADINO_EXEC} ${TARADINO_SOURCES})
target_link_libraries(${TARADINO_EXEC} PRIVATE SDL2::Main SDL2::Mixer)
target_compile_definitions(${TARADINO_EXEC} PRIVATE PACKAGE_STRING="${TARADINO_TITLE} ${CMAKE_PROJECT_VERSION}")
target_compile_definitions(${TARADINO_EXEC} PRIVATE PACKAGE_TARNAME="${CMAKE_PROJECT_NAME}")

if(MSVC)
	set_property(TARGET ${TARADINO_EXEC} PROPERTY C_STANDARD 17)
	# silence warnings about POSIX functions
	target_compile_options(${TARADINO_EXEC} PUBLIC -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS -Dstrcasecmp=_stricmp -Dalloca=_alloca)
	# silence unsigned/signed and integer truncation warnings
	target_compile_options(${TARADINO_EXEC} PRIVATE /wd4267 /wd4244)
endif()

if(${TARADINO_SHAREWARE})
	target_compile_definitions(${TARADINO_EXEC} PRIVATE SHAREWARE=1)
endif()

if(${TARADINO_WERROR} AND NOT MSVC)
	target_compile_options(${TARADINO_EXEC} PRIVATE -Werror)
endif()

if(${TARADINO_DATADIR})
	target_compile_definitions(${TARADINO_EXEC} PRIVATE DATADIR="${TARADINO_DATADIR}")
endif()

include(GNUInstallDirs)
if(UNIX)
	configure_file(${PROJECT_SOURCE_DIR}/misc/taradino.desktop.in ${PROJECT_BINARY_DIR}/${TARADINO_EXEC}.desktop @ONLY)
	configure_file(${PROJECT_SOURCE_DIR}/doc/taradino.6.in ${PROJECT_BINARY_DIR}/${TARADINO_EXEC}.6 @ONLY)
	install(FILES ${PROJECT_BINARY_DIR}/${TARADINO_EXEC}.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
	install(FILES ${PROJECT_SOURCE_DIR}/misc/taradino.svg DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps RENAME ${TARADINO_EXEC}.svg)
	install(FILES ${PROJECT_BINARY_DIR}/${TARADINO_EXEC}.6 DESTINATION ${CMAKE_INSTALL_MANDIR}/man6)
endif()
install(TARGETS ${TARADINO_EXEC} DESTINATION ${CMAKE_INSTALL_BINDIR})

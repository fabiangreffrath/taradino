
cmake_minimum_required(VERSION 3.10)
project(taradino
	DESCRIPTION "SDL2 port of Rise of the Triad"
	HOMEPAGE_URL "https://github.com/fabiangreffrath/taradino/"
	LANGUAGES C
	VERSION 1.1.3
)
configure_file(rott/version.h.in rott/version.h @ONLY)

option(TARADINO_ASAN "Use Address Sanitizer" OFF)
option(TARADINO_SHAREWARE "Build shareware version" OFF)
option(TARADINO_WARNINGS_AS_ERRORS "Treat all compiler warnings as errors" OFF)
set(TARADINO_DATADIR "" CACHE STRING "Game data file directory")
set(TARADINO_SUFFIX "" CACHE STRING "Executable file name suffix")

add_library(ASAN::ASAN IMPORTED INTERFACE)
target_compile_options(ASAN::ASAN INTERFACE -fsanitize=address $<$<NOT:$<COMPILE_LANG_AND_ID:C,MSVC>>:-fno-omit-frame-pointer>)
target_link_options(ASAN::ASAN INTERFACE -fsanitize=address)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
find_package(SDL2_mixer REQUIRED CONFIG REQUIRED COMPONENTS SDL2_mixer)

file(GLOB TARADINO_SOURCES "${PROJECT_SOURCE_DIR}/rott/*.c")

if(WIN32 OR MSVC OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
	list(APPEND TARADINO_SOURCES "${PROJECT_SOURCE_DIR}/misc/taradino.rc")
endif()

string(LENGTH "${TARADINO_SUFFIX}" TARADINO_SUFFIX_LEN)
if(TARADINO_SUFFIX_LEN GREATER 0)
	string(CONCAT TARADINO_EXEC "${CMAKE_PROJECT_NAME}" "-" "${TARADINO_SUFFIX}")
endif()

if(${TARADINO_SHAREWARE})
	set(TARADINO_TITLE "Taradino (Shareware)")
else()
	set(TARADINO_TITLE "Taradino")
endif()

add_executable(taradino ${TARADINO_SOURCES})
target_link_libraries(taradino PRIVATE $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main> SDL2::SDL2 SDL2_mixer::SDL2_mixer)
target_compile_definitions(taradino PRIVATE PACKAGE_STRING="${TARADINO_TITLE} ${CMAKE_PROJECT_VERSION}")
target_compile_definitions(taradino PRIVATE PACKAGE_TARNAME="${CMAKE_PROJECT_NAME}")
target_include_directories(taradino PRIVATE "${PROJECT_BINARY_DIR}/rott")
target_link_libraries(taradino PUBLIC $<$<BOOL:${TARADINO_ASAN}>:ASAN::ASAN>)

if(NOT DEFINED TARADINO_EXEC)
	set(TARADINO_EXEC taradino)
endif()

set_target_properties(taradino PROPERTIES OUTPUT_NAME ${TARADINO_EXEC})

set_target_properties(taradino PROPERTIES C_STANDARD_REQUIRED ON C_STANDARD 99)
if (MSVC)
	# silence warnings about POSIX functions
	target_compile_options(taradino PUBLIC -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS -Dstrcasecmp=_stricmp -Dalloca=_alloca)
	# silence unsigned/signed and integer truncation warnings
	target_compile_options(taradino PRIVATE /wd4267 /wd4244)
endif()

if(${TARADINO_SHAREWARE})
	target_compile_definitions(taradino PRIVATE SHAREWARE=1)
endif()

if(${TARADINO_WARNINGS_AS_ERRORS})
	if(MSVC)
		target_compile_options(taradino PRIVATE /WX)
	else()
		target_compile_options(taradino PRIVATE -Werror)
	endif()
endif()

if(${TARADINO_DATADIR})
	target_compile_definitions(taradino PRIVATE DATADIR="${TARADINO_DATADIR}")
endif()

include(GNUInstallDirs)
if(UNIX)
	configure_file(${PROJECT_SOURCE_DIR}/misc/taradino.desktop.in ${PROJECT_BINARY_DIR}/${TARADINO_EXEC}.desktop @ONLY)
	configure_file(${PROJECT_SOURCE_DIR}/doc/taradino.6.in ${PROJECT_BINARY_DIR}/${TARADINO_EXEC}.6 @ONLY)
	install(FILES ${PROJECT_BINARY_DIR}/${TARADINO_EXEC}.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
	install(FILES ${PROJECT_SOURCE_DIR}/misc/taradino.svg DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps RENAME ${TARADINO_EXEC}.svg)
	install(FILES ${PROJECT_BINARY_DIR}/${TARADINO_EXEC}.6 DESTINATION ${CMAKE_INSTALL_MANDIR}/man6)
endif()
install(TARGETS taradino DESTINATION ${CMAKE_INSTALL_BINDIR})

include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR ZIP)
set(CPACK_PACKAGE_NAME ${TARADINO_EXEC})
set(CPACK_SOURCE_GENERATOR TGZ ZIP)
set(CPACK_SOURCE_IGNORE_FILES "/.git/;/cmake-build/;/build/;/.vs/;/out/;CMakeSettings.json")
set(CPACK_STRIP_FILES TRUE)
include(CPack)

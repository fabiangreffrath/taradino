cmake_minimum_required(VERSION 3.10)

# Automatically installs dependencies.
set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON)

project(taradino
	DESCRIPTION "SDL2 port of Rise of the Triad"
	HOMEPAGE_URL "https://github.com/fabiangreffrath/taradino/"
	LANGUAGES C
	VERSION 2025.12.22
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

find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_mixer REQUIRED CONFIG)
find_package(libADLMIDI CONFIG)

file(GLOB TARADINO_SOURCES "${PROJECT_SOURCE_DIR}/rott/*.c")
file(GLOB TARADINO_HEADERS "${PROJECT_SOURCE_DIR}/rott/*.h")

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
target_link_libraries(taradino PRIVATE
	$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
	$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
	$<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
)
target_compile_definitions(taradino PRIVATE PACKAGE_STRING="${TARADINO_TITLE} ${CMAKE_PROJECT_VERSION}")
target_compile_definitions(taradino PRIVATE PACKAGE_TARNAME="${CMAKE_PROJECT_NAME}")
target_include_directories(taradino PRIVATE "${PROJECT_BINARY_DIR}/rott")
target_link_libraries(taradino PUBLIC $<$<BOOL:${TARADINO_ASAN}>:ASAN::ASAN>)

include(CheckLibraryExists)
check_library_exists(m pow "" HAVE_LIBM)
if(HAVE_LIBM)
	target_link_libraries(taradino PRIVATE m)
endif()

if(libADLMIDI_FOUND)
	target_compile_definitions(taradino PRIVATE HAVE_ADLMIDI)
	if(NOT TARGET libADLMIDI::ADLMIDI)
		if(TARGET libADLMIDI::ADLMIDI_shared)
			add_library(libADLMIDI::ADLMIDI ALIAS libADLMIDI::ADLMIDI_shared)
		else()
			add_library(libADLMIDI::ADLMIDI ALIAS libADLMIDI::ADLMIDI_static)
		endif()
	endif()
	target_link_libraries(taradino PRIVATE libADLMIDI::ADLMIDI)
endif()

include(CheckIncludeFile)
check_include_file("dirent.h" HAVE_DIRENT_H)
if (HAVE_DIRENT_H)
	target_compile_definitions(taradino PRIVATE HAVE_DIRENT_H)
endif()

if(NOT DEFINED TARADINO_EXEC)
	set(TARADINO_EXEC taradino)
endif()

set_target_properties(taradino PROPERTIES OUTPUT_NAME ${TARADINO_EXEC})

set_target_properties(taradino PROPERTIES C_STANDARD_REQUIRED ON C_STANDARD 99)
if(MSVC)
	# silence warnings about POSIX functions
	target_compile_options(taradino PUBLIC -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS -Dstrcasecmp=_stricmp -Dalloca=_alloca)
	# silence unsigned/signed and integer truncation warnings
	target_compile_options(taradino PRIVATE /wd4267 /wd4244)
else()
	target_compile_options(taradino PRIVATE -Wall)
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

if(NOT TARADINO_DATADIR STREQUAL "")
	target_compile_definitions(taradino PRIVATE DATADIR="${TARADINO_DATADIR}")
endif()

include(GNUInstallDirs)
if(WIN32)
	if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.21 AND NOT VCPKG_TOOLCHAIN)
		install(
			TARGETS taradino
			RUNTIME_DEPENDENCIES
			PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
			POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
			DIRECTORIES $<TARGET_FILE_DIR:taradino> $ENV{PATH}
			RUNTIME DESTINATION .
		)
	else()
		install(TARGETS taradino RUNTIME DESTINATION .)
	endif()
	install(FILES ${PROJECT_SOURCE_DIR}/COPYING DESTINATION .)
	install(FILES ${PROJECT_SOURCE_DIR}/README.md DESTINATION .)
else()
	configure_file(${PROJECT_SOURCE_DIR}/misc/taradino.desktop.in ${PROJECT_BINARY_DIR}/${TARADINO_EXEC}.desktop @ONLY)
	configure_file(${PROJECT_SOURCE_DIR}/doc/taradino.6.in ${PROJECT_BINARY_DIR}/${TARADINO_EXEC}.6 @ONLY)
	install(FILES ${PROJECT_BINARY_DIR}/${TARADINO_EXEC}.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
	install(FILES ${PROJECT_SOURCE_DIR}/misc/taradino.svg DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps RENAME ${TARADINO_EXEC}.svg)
	install(FILES ${PROJECT_BINARY_DIR}/${TARADINO_EXEC}.6 DESTINATION ${CMAKE_INSTALL_MANDIR}/man6)
	install(FILES ${PROJECT_SOURCE_DIR}/COPYING DESTINATION ${CMAKE_INSTALL_DOCDIR})
	install(FILES ${PROJECT_SOURCE_DIR}/README.md DESTINATION ${CMAKE_INSTALL_DOCDIR})
	install(TARGETS taradino DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

find_program(CLANGFORMAT_EXECUTABLE clang-format)
if(CLANGFORMAT_EXECUTABLE)
	add_custom_target(clangformat
		COMMAND ${CLANGFORMAT_EXECUTABLE} -style=file -i ${TARADINO_SOURCES} ${TARADINO_HEADERS}
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	)
endif()

set(CPACK_GENERATOR ZIP)
set(CPACK_PACKAGE_NAME ${TARADINO_EXEC})
set(CPACK_SOURCE_GENERATOR TGZ ZIP)
set(CPACK_SOURCE_IGNORE_FILES "/.git/;/cmake-build/;/build/;/.vs/;/out/;CMakeSettings.json")
set(CPACK_STRIP_FILES TRUE)
include(CPack)

cmake_minimum_required(VERSION 3.15) #Must be 3.15+ because of this: https://discourse.cmake.org/t/how-to-set-warning-level-correctly-in-modern-cmake/1103

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

set(TARGET_NAME MyNewApp)
project(${TARGET_NAME})

# Clear the default flags so we can properly configure them
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "")

# Set custom output directories
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)

file(GLOB_RECURSE SOURCES
	"src/*.cpp"
	"src/*.c"
)

file(GLOB_RECURSE HEADERS
	"src/*.hpp"
	"src/*.h"
)

# Add the executable target
add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS})

# Set output directory to the desired one, without the Release/Debug sudirs that MSBuild adds by default
get_property(current_targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
	foreach(TARGET ${current_targets})
		set_target_properties(${TARGET} PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<0:>
			LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<0:>
			ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<0:>
	)
endforeach()

# Compiler flags for different platforms
if (MSVC)
	target_compile_options(${TARGET_NAME} PRIVATE
		$<$<CONFIG:Debug>:/Ob2>
		$<$<OR:$<CONFIG:RelWithDebInfo>,$<CONFIG:Release>>:/GS- /O2>
		/std:c++latest /W4 /MP /utf-8 /Zi /Gy
		/wd4996
	)

	target_link_options(${TARGET_NAME} PRIVATE
		$<$<OR:$<CONFIG:RelWithDebInfo>,$<CONFIG:Release>>:/OPT:REF /OPT:ICF>
		$<$<CONFIG:Debug>:/INCREMENTAL>
		/DEBUG:FASTLINK
	)

elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
	target_compile_options(${TARGET_NAME} PRIVATE
		-std=c++2b
		-pedantic-errors
		-g
		-Wall -Wextra -Wdelete-non-virtual-dtor -Werror=duplicated-cond
		-Werror=duplicated-branches -Warith-conversion -Warray-bounds
		-Wattributes -Wcast-align -Wcast-qual -Wdate-time
		-Wduplicated-branches -Wendif-labels -Werror=overflow
		-Werror=return-type -Werror=shift-count-overflow -Werror=sign-promo
		-Werror=undef -Wextra -Winit-self -Wlogical-op -Wmissing-include-dirs
		-Wnull-dereference -Wpedantic -Wpointer-arith -Wredundant-decls
		-Wshadow -Wstrict-aliasing -Wstrict-aliasing=3 -Wuninitialized
		-Wunused-const-variable=2 -Wwrite-strings -Wlogical-op
		-Wno-missing-include-dirs -Wno-undef -Wno-switch
		$<$<OR:$<CONFIG:RelWithDebInfo>,$<CONFIG:Release>>:-O3>
	)

	if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		target_compile_options(${TARGET_NAME} PRIVATE -fconcepts)
	endif()

endif()

set(OUTPUT_DIR ${CMAKE_BINARY_DIR})
add_subdirectory(${CMAKE_SOURCE_DIR}/cpputils)

target_link_libraries(${TARGET_NAME} cpputils)
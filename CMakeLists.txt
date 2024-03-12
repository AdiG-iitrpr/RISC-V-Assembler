cmake_minimum_required(VERSION 3.20)

#
# Project settings and generic definitions.
#
project(assembler)

add_definitions(-Wall -Wextra -Wfloat-conversion -std=c++17 -pipe -march=x86-64 -mtune=generic)

# Debug and Release definitions.
# To set the build mode, specify the value of var `CMAKE_BUILD_TYPE` when
# running CMake.
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g")
set (CMAKE_CXX_FLAGS "-lstdc++fs -std=c++17")

# Fallback to Debug build type automatically
# if no compilation mode was specified.
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Debug")
endif()

message("-- Using compilation flags of mode \"" ${CMAKE_BUILD_TYPE} "\"")


#
# Project compilation settings.
#

# Set the compiler to search for headers inside the source code dir.
include_directories(src)


add_executable(assembler
   src/assembler/parser/Parser.cpp
   src/main.cpp
   src/assembler/Assembler.cpp
   src/assembler/lexer/Lexer.cpp
   src/assembler/labelTable/SymbolTable.cpp
   src/assembler/token/Token.cpp
   src/assembler/instruction/Instruction.cpp
   src/utils/FileUtils.cpp
   src/utils/NumberUtils.cpp

)

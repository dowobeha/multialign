cmake_minimum_required (VERSION 3.0.2)
project (align)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
if(COMPILER_SUPPORTS_CXX14)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
else()
        message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
endif()

add_definitions(-pg)
add_executable(align align.c++ util.c++ Alignment.c++ Coordinate.c++ Coordinates.c++ Costs.c++ Dimensions.c++ DimensionsIterator.c++ Distance.c++ Europarl.c++ SentenceAligner.c++)

add_executable(multextEast multext.c++ util.c++  MultextEast.c++)


set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
set(CMAKE_BUILD_TYPE Debug)

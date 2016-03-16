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

add_executable(align align.c++ util.c++ Alignment.c++ Coordinate.c++ Coordinates.c++ DynamicProgrammingTable.c++ Dimensions.c++ DimensionsIterator.c++ Gale_and_Church_1993.c++ Europarl.c++ SentenceAligner.c++ SentenceAlignments.c++)

add_executable(hand_test hand_test.c++ util.c++ Alignment.h++ Alignment.c++ Coordinate.h++ Coordinate.c++ Coordinates.h++ Coordinates.c++ DynamicProgrammingTable.h++ DynamicProgrammingTable.c++ Dimensions.h++ Dimensions.c++ DimensionsIterator.h++ DimensionsIterator.c++ Gale_and_Church_1993.h++ Gale_and_Church_1993.c++ Europarl.h++ Europarl.c++ SentenceAligner.h++ SentenceAligner.c++ SentenceAlignments.h++ SentenceAlignments.c++)

add_executable(multextEast multext.c++ util.c++  MultextEast.c++)

add_executable(run_unit_tests run_unit_tests.c++ util.c++ Alignment.h++ Alignment.c++ Coordinate.h++ Coordinate.c++ Coordinates.h++ Coordinates.c++ DynamicProgrammingTable.h++ DynamicProgrammingTable.c++ Dimensions.h++ Dimensions.c++ DimensionsIterator.h++ DimensionsIterator.c++ Gale_and_Church_1993.h++ Gale_and_Church_1993.c++ Europarl.h++ Europarl.c++ SentenceAligner.h++ SentenceAligner.c++ SentenceAlignments.h++ SentenceAlignments.c++ SentenceAlignments.unit_tests.c++)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
set(CMAKE_BUILD_TYPE Debug)

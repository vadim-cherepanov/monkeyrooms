cmake_minimum_required(VERSION 3.14)
project(MonkeyRooms)

set(CMAKE_CXX_STANDARD 11)

if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
    endif()
endif()

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). ")
endif()

set(GCC_COMPILE_FLAGS "-mpopcnt -msse4.2")

set(CMAKE_CXX_FLAGS "${GCC_COMPILE_FLAGS} ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O1 ${GCC_COMPILE_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 ${GCC_COMPILE_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")

add_executable(MonkeyRooms main.cpp monkeyrooms.cpp monkeyrooms.h timer.h)

#find_package(Boost REQUIRED COMPONENTS chrono)
#target_link_libraries(MonkeyRooms ${Boost_LIBRARIES})

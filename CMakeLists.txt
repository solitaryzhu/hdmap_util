cmake_minimum_required(VERSION 2.8)
PROJECT(hdmap_util)

set(CMAKE_CXX_FLAGS_DEBUG "${CXXFLAGS} -O0 -g -Wall -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CXXFLAGS} -O3 -Wall")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/trunk/inc/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/trunk/inc/tile/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/trunk/inc/linematch/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/trunk/inc/projection/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/trunk/inc/gisproj/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/trunk/inc/hmmmatch/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/trunk/inc/interpolate/")

file(GLOB_RECURSE SRC "${CMAKE_CURRENT_SOURCE_DIR}/trunk/src/*.cpp")
file(GLOB_RECURSE INC "${CMAKE_CURRENT_SOURCE_DIR}/trunk/inc/*.h")
file(GLOB_RECURSE TEST "${CMAKE_CURRENT_SOURCE_DIR}/trunk/test/*.*pp")


find_package(Boost 1.59 REQUIRED)


add_executable(${PROJECT_NAME} ${SRC} ${INC} ${TEST})

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif()

target_link_libraries(${PROJECT_NAME}
	dl
	pthread)

ADD_LIBRARY(hdmaputil SHARED ${SRC} ${INC})
ADD_LIBRARY(hdmaputil_static STATIC ${SRC} ${INC})
SET_TARGET_PROPERTIES(hdmaputil_static PROPERTIES OUTPUT_NAME "hdmaputil")
GET_TARGET_PROPERTY(OUTPUT_VALUE hdmaputil_static OUTPUT_NAME)
SET_TARGET_PROPERTIES (hdmaputil_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES (hdmaputil PROPERTIES CLEAN_DIRECT_OUTPUT 1)

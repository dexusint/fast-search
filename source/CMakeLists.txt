cmake_minimum_required (VERSION 3.2)

set (PROJECT fast-search)
project (${PROJECT} C CXX)

set (HEADERS 
	../include/application.h
	../include/user.h)

set (SOURCES 
	../source/application.cpp 
	../source/main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pthread")
#set (CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(THREADS_PREFER_PTHREAD_FLAG TRUE) 

#find_package (Threads REQUIRED) 

#target_link_libraries(${PROJECT} Threads::Threads) 


include_directories (../)

source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})

add_executable (${PROJECT} ${HEADERS} ${SOURCES})


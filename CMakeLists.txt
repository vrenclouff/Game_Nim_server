#cmake_minimum_required(VERSION 3.6)
project(NimServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp User.cpp User.h Parameters.cpp Parameters.h Logger.cpp Logger.h MainOptions.cpp MainOptions.h NetworkService.cpp NetworkService.h StringUtils.cpp StringUtils.h Application.cpp Application.h Thread.h SafeQueue.cpp RCVMessage.cpp RCVMessage.h RouterService.cpp RouterService.h SNDMessage.cpp SNDMessage.h SenderService.cpp SenderService.h SafeList.cpp Game.cpp Game.h GameManager.cpp GameManager.h UserManager.cpp UserManager.h Enums.h Enums.cpp)
add_executable(NimServer ${SOURCE_FILES})
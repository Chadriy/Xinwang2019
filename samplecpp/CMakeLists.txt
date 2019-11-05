cmake_minimum_required(VERSION 3.14)
project(samplecpp)

set(CMAKE_CXX_STANDARD 14)
aux_source_directory(. SRC_LIST)
aux_source_directory(./extra SRC_LIST)
aux_source_directory(./extra/socket SRC_LIST)

add_executable(samplecpp ${SRC_LIST} src/conn/Channel.h src/conn/GeneralChannel.cpp src/conn/GeneralChannel.h src/Const.h src/Action.cpp src/Action.h src/Scheduler.cpp src/Scheduler.h src/Const.cpp)
target_link_libraries(samplecpp ws2_32)
cmake_minimum_required(VERSION 3.13.4)
project(server)

set(CMAKE_CXX_STANDARD 14)

aux_source_directory(. SRC_LIST)
aux_source_directory(./conn SRC_LIST)
aux_source_directory(./Exception SRC_LIST)
aux_source_directory(./scheduler SRC_LIST)
aux_source_directory(./topology SRC_LIST)
aux_source_directory(./extra SRC_LIST)
aux_source_directory(./extra/socket SRC_LIST)

MESSAGE(SRC_LIST ${SRC_LIST})
add_executable(server ${SRC_LIST})
target_link_libraries(server ws2_32)

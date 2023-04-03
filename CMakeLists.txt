cmake_minimum_required(VERSION 3.15)
project(iKits)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
aux_source_directory(. SRC_LIST) # 搜索当前目录下的所有.cpp文件

add_executable(iKits main.cpp ${SRC_LIST})
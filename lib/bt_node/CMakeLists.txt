cmake_minimum_required(VERSION 3.14)

project(bt_node
    LANGUAGES CXX)

### Project Settings ###
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

### Dependencies ###
include(cmake/external/behaviortree_cpp.cmake)

### bt_node::filesystem ###
add_library(bt_node-filesystem STATIC)
target_sources(bt_node-filesystem
    PUBLIC  src/filesystem/find_file.cpp
            src/filesystem/copy_file.cpp)
target_include_directories(bt_node-filesystem
    PUBLIC  include)
target_link_libraries(bt_node-filesystem
    PUBLIC  BT::behaviortree_cpp)
add_library(bt_node::filesystem ALIAS bt_node-filesystem)

### bt_node::archive ###
add_library(bt_node-archive STATIC)
target_sources(bt_node-archive
    PUBLIC  src/archive/extract_archive.cpp)
target_include_directories(bt_node-archive
    PUBLIC  include)
target_link_libraries(bt_node-archive
    PUBLIC  BT::behaviortree_cpp)
add_library(bt_node::archive ALIAS bt_node-archive)

### bt_node::bt_node ###
add_library(bt_node STATIC)
target_link_libraries(bt_node
    PUBLIC  bt_node::filesystem
            bt_node::archive)
add_library(bt_node::bt_node ALIAS bt_node)
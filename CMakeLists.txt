cmake_minimum_required(VERSION 3.14)

project(sw-updater
    LANGUAGES CXX)

### Project Settings ###
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

### Dependencies ###
include(cmake/external/behaviortree_cpp.cmake)
include(cmake/external/robl_behavior_tree.cmake)

### Executable ###
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
    PRIVATE src/main.cpp)
target_include_directories(${PROJECT_NAME}
    PRIVATE include)
target_link_libraries(${PROJECT_NAME}
    PRIVATE BT::behaviortree_cpp
            robl_behavior_tree::robl_behavior_tree)
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        INSTALL_RPATH "$ORIGIN/../lib")

### Install ###
# install(TARGETS ${PROJECT_NAME}
#     PUBLIC_HEADER DESTINATION include
#     ARCHIVE DESTINATION lib
#     LIBRARY DESTINATION lib
#     RUNTIME DESTINATION bin)
install(TARGETS ${PROJECT_NAME}
    DESTINATION bin)
install(DIRECTORY cfg
    DESTINATION .)
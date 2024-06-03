cmake_minimum_required(VERSION 3.14)

project(sw-updater
    LANGUAGES CXX)

### Project Settings ###
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

### Dependencies ###
# BehaviorTree.CPP
include(cmake/external/behaviortree_cpp.cmake)

# robl_behaviortree Plugin
include(FetchContent)
FetchContent_Declare(robl_behaviortree
    GIT_REPOSITORY https://github.com/shkwon98/robl_behaviortree.git
    GIT_TAG        main
    GIT_PROGRESS   TRUE
    GIT_SHALLOW    TRUE)
FetchContent_MakeAvailable(robl_behaviortree)

### Executable ###
file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
    PRIVATE ${SOURCES})
target_include_directories(${PROJECT_NAME}
    PRIVATE include)
target_link_libraries(${PROJECT_NAME}
    PRIVATE BT::behaviortree_cpp)
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
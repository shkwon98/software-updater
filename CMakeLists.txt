cmake_minimum_required(VERSION 3.14)

project(nb0cp
    LANGUAGES CXX)

### Dependencies ###
# behavior_tree_cpp
# include(FetchContent)
# set(FETCHCONTENT_QUIET OFF)
# FetchContent_Declare(behavior_tree_cpp
#     GIT_REPOSITORY https://github.com/BehaviorTree/BehaviorTree.CPP.git
#     GIT_TAG        4.5.1
#     GIT_PROGRESS   TRUE
#     GIT_SHALLOW    TRUE
#     EXCLUDE_FROM_ALL)
# FetchContent_MakeAvailable(behavior_tree_cpp)


find_package(behaviortree_cpp REQUIRED)


# add_subdirectory(BehaviorTree.CPP)
# option(BTCPP_EXAMPLES   "Build tutorials and examples" OFF)
# option(BTCPP_UNIT_TESTS "Build the unit tests" OFF)

# message("BTCPP_INCLUDE_DESTINATION: ${BTCPP_INCLUDE_DESTINATION}")


add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME}
    PRIVATE src)
target_link_libraries(${PROJECT_NAME}
    PRIVATE BT::behaviortree_cpp)

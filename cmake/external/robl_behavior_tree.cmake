include(FetchContent)

set(FETCHCONTENT_QUIET OFF)
FetchContent_Declare(robl_behavior_tree
    GIT_REPOSITORY https://github.com/shkwon98/robl_behavior_tree.git
    GIT_TAG        main
    GIT_PROGRESS   TRUE
    GIT_SHALLOW    TRUE)
FetchContent_MakeAvailable(robl_behavior_tree)
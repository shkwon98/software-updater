// Standard headers
#include <deque>

// BehaviorTree.CPP headers
#include <behaviortree_cpp/bt_factory.h>
#include <behaviortree_cpp/decorators/loop_node.h>
#include <behaviortree_cpp/loggers/bt_cout_logger.h>
#include <behaviortree_cpp/loggers/bt_file_logger_v2.h>
#include <behaviortree_cpp/loggers/groot2_publisher.h>

// bt_node headers
#include <bt_node/archive/extract_archive.h>
#include <bt_node/filesystem/copy_file.h>
#include <bt_node/filesystem/find_file.h>

// Project headers
#include "software_updater/common.h"
#include "software_updater/node/find_latest_package.hpp"

using namespace std::chrono_literals;

int main()
{
    auto factory = BT::BehaviorTreeFactory();
    factory.registerNodeType<FindFile>("FindFile");
    factory.registerNodeType<FindLatestPackage>("FindLatestPackage");
    factory.registerNodeType<CopyFile>("CopyFile");
    factory.registerNodeType<ExtractArchive>("ExtractArchive");

    const auto &default_bt_xml_file = std::filesystem::path("../cfg/my_tree.xml");
    const auto &default_bt_log_file = std::filesystem::path("../log/bt_trace.btlog");
    std::filesystem::create_directories(default_bt_log_file.parent_path());

    auto blackboard = BT::Blackboard::create();
    auto tree = factory.createTreeFromFile(default_bt_xml_file, blackboard);

    auto groot2_publisher = std::make_unique<BT::Groot2Publisher>(tree, 5555);
    auto bt_file_logger = std::make_unique<BT::FileLogger2>(tree, default_bt_log_file);
    auto bt_cout_logger = std::make_unique<BT::StdCoutLogger>(tree);
    bt_cout_logger->enableTransitionToIdle(false);

    tree.tickWhileRunning();
    // while (true)
    // {
    //     const auto &now = std::chrono::steady_clock::now();
    //     tree.tickOnce();
    //     std::this_thread::sleep_until(now + 1s);
    // }

    return 0;
}

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

namespace fs = std::filesystem;
using namespace std::chrono_literals;

int main()
{
    auto factory = BT::BehaviorTreeFactory();
    factory.registerNodeType<FindFile>("FindFile");
    factory.registerNodeType<FindLatestPackage>("FindLatestPackage");
    factory.registerNodeType<CopyFile>("CopyFile");
    factory.registerNodeType<ExtractArchive>("ExtractArchive");

    const auto &log_file = fs::path("../log/bt_trace.btlog");
    fs::create_directories(log_file.parent_path());

    auto main_tree = factory.createTreeFromFile("../cfg/main_tree.xml");
    auto groot2_publisher = std::make_unique<BT::Groot2Publisher>(main_tree, 5555);
    auto file_logger = std::make_unique<BT::FileLogger2>(main_tree, log_file);
    auto cout_logger = std::make_unique<BT::StdCoutLogger>(main_tree);
    cout_logger->enableTransitionToIdle(false);

    while (main_tree.rootNode()->executeTick() == BT::NodeStatus::RUNNING)
    {
        const auto &now = std::chrono::steady_clock::now();
        std::this_thread::sleep_until(now + 1000ms);
    }
    // main_tree.tickWhileRunning(100ms);
    // while (true)
    // {
    //     const auto &now = std::chrono::steady_clock::now();
    //     main_tree.tickOnce();
    //     std::this_thread::sleep_until(now + 1s);
    // }

    return 0;
}

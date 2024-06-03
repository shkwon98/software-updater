// Standard headers
#include <deque>

// BehaviorTree.CPP headers
#include <behaviortree_cpp/bt_factory.h>
#include <behaviortree_cpp/decorators/loop_node.h>
#include <behaviortree_cpp/loggers/bt_cout_logger.h>
#include <behaviortree_cpp/loggers/bt_file_logger_v2.h>
#include <behaviortree_cpp/loggers/groot2_publisher.h>
#include <behaviortree_cpp/xml_parsing.h>

// bt_node headers
#include <robl_behavior_tree/robl_behavior_tree.h>

// Project headers
#include "software_updater/common.h"
#include "software_updater/node/dummy_nodes.h"
#include "software_updater/node/find_latest_package.hpp"
#include "software_updater/node/install.hpp"
#include "software_updater/node/receive_message.hpp"

using namespace std::chrono_literals;

int main()
{
    auto factory = BT::BehaviorTreeFactory();
    factory.registerFromPlugin("../lib/libbt_node-archive.so");
    factory.registerFromPlugin("../lib/libbt_node-filesystem.so");
    factory.registerNodeType<FindLatestPackage>("FindLatestPackage");
    factory.registerNodeType<ReceiveMessage>("ReceiveMessage");
    factory.registerNodeType<SaySomething>("SaySomething");
    factory.registerNodeType<Install>("Install");

    const auto &log_file = std::filesystem::path("../log/bt_trace.btlog");
    std::filesystem::create_directories(log_file.parent_path());

    auto main_tree = factory.createTreeFromFile("../cfg/main_tree.xml");
    auto groot2_publisher = std::make_unique<BT::Groot2Publisher>(main_tree, 5555);
    auto file_logger = std::make_unique<BT::FileLogger2>(main_tree, log_file);
    auto cout_logger = std::make_unique<BT::StdCoutLogger>(main_tree);
    cout_logger->enableTransitionToIdle(false);

    // while (main_tree.rootNode()->executeTick() == BT::NodeStatus::RUNNING)
    // {
    //     const auto &now = std::chrono::steady_clock::now();
    //     std::this_thread::sleep_until(now + 1s);
    // }
    // main_tree.tickWhileRunning(100ms);
    while (true)
    {
        const auto &now = std::chrono::steady_clock::now();
        // main_tree.tickOnce();
        main_tree.tickExactlyOnce();
        std::this_thread::sleep_until(now + 100ms);
    }

    return 0;
}

// Standard headers
#include <deque>

// BehaviorTree.CPP headers
#include <behaviortree_cpp/bt_factory.h>
#include <behaviortree_cpp/decorators/loop_node.h>
#include <behaviortree_cpp/loggers/bt_cout_logger.h>
#include <behaviortree_cpp/loggers/bt_file_logger_v2.h>
#include <behaviortree_cpp/loggers/groot2_publisher.h>
#include <behaviortree_cpp/xml_parsing.h>

// Project headers
#include "software_updater/common.h"
#include "software_updater/node/find_latest_package_node.h"
#include "software_updater/node/install_node.h"
#include "software_updater/node/receive_message_node.h"

using namespace std::chrono_literals;
using namespace software_updater;

int main()
{
    auto factory = BT::BehaviorTreeFactory();
    factory.registerFromPlugin("../lib/librobl-behaviortree-archive.so");
    factory.registerFromPlugin("../lib/librobl-behaviortree-filesystem.so");
    factory.registerFromPlugin("../lib/librobl-behaviortree-logger.so");
    factory.registerNodeType<node::FindLatestPackageNode>("FindLatestPackage");
    factory.registerNodeType<node::InstallNode>("Install");
    factory.registerNodeType<node::ReceiveMessageNode>("ReceiveMessage");

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

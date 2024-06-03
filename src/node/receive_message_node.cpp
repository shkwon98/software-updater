#include "software_updater/node/receive_message_node.h"

#include <filesystem>

namespace software_updater::node
{

BT::NodeStatus ReceiveMessageNode::onStart(void)
{
    future_ = std::async(std::launch::async, []() {
        std::string path;
        std::cin >> path;
        return path;
    });

    return BT::NodeStatus::RUNNING;
}

BT::NodeStatus ReceiveMessageNode::onRunning(void)
{
    if (future_.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
    {
        setOutput("path", future_.get());
        return BT::NodeStatus::SUCCESS;
    }

    constexpr auto max_dot_count = 5;
    std::cout << "\033[s" << "\rWaiting for user input" << std::string(dot_count_, '.')
              << std::string(max_dot_count - dot_count_, ' ') << std::flush << "\033[u";
    dot_count_ = (dot_count_ + 1) % max_dot_count;

    return BT::NodeStatus::RUNNING;
}

void ReceiveMessageNode::onHalted(void)
{
}

} // namespace software_updater::node
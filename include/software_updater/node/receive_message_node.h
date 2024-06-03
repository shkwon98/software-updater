#pragma once

#include <future>

#include <behaviortree_cpp/action_node.h>

namespace software_updater::node
{

class ReceiveMessageNode : public BT::StatefulActionNode
{
public:
    ReceiveMessageNode(const std::string &name, const BT::NodeConfig &config)
        : BT::StatefulActionNode(name, config)
    {
    }

    static BT::PortsList providedPorts()
    {
        return { BT::OutputPort<std::string>("path") };
    }

    BT::NodeStatus onStart(void) override;
    BT::NodeStatus onRunning(void) override;
    void onHalted(void) override;

private:
    std::future<std::string> future_;
    int dot_count_ = 0;
};

} // namespace software_updater::node
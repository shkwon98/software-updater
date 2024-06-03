#pragma once

#include <behaviortree_cpp/action_node.h>

namespace software_updater::node
{

class FindLatestPackageNode : public BT::SyncActionNode
{
public:
    FindLatestPackageNode(const std::string &name, const BT::NodeConfig &config)
        : BT::SyncActionNode(name, config)
    {
    }

    static BT::PortsList providedPorts(void)
    {
        return { BT::InputPort<std::vector<std::string>>("files"), BT::OutputPort<std::string>("file") };
    }

    BT::NodeStatus tick(void) override;

private:
};

} // namespace software_updater::node
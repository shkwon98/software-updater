#pragma once

#include <filesystem>

#include <behaviortree_cpp/action_node.h>

class CopyFile : public BT::SyncActionNode
{
public:
    CopyFile(const std::string &name, const BT::NodeConfig &config)
        : BT::SyncActionNode(name, config)
    {
    }

    static BT::PortsList providedPorts()
    {
        return { BT::InputPort<std::string>("src"), BT::InputPort<std::string>("dest"),
                 BT::OutputPort<std::string>("output") };
    }

private:
    BT::NodeStatus tick() override;
};
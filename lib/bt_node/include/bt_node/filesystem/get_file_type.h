#pragma once

#include <filesystem>
#include <regex>

#include <behaviortree_cpp/action_node.h>

class GetFileType : public BT::SyncActionNode
{
public:
    GetFileType(const std::string &name, const BT::NodeConfig &config)
        : BT::SyncActionNode(name, config)
    {
    }

    static BT::PortsList providedPorts()
    {
        return { BT::InputPort<std::string>("file"), BT::OutputPort<std::string>("type") };
    }

private:
    BT::NodeStatus tick() override;
};
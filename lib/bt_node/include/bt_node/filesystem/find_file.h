#pragma once

#include <filesystem>
#include <regex>

#include <behaviortree_cpp/action_node.h>

class FindFile : public BT::SyncActionNode
{
public:
    FindFile(const std::string &name, const BT::NodeConfig &config)
        : BT::SyncActionNode(name, config)
    {
    }

    static BT::PortsList providedPorts()
    {
        return { BT::InputPort<std::string>("path"), BT::InputPort<std::string>("expression"),
                 BT::OutputPort<std::vector<std::string>>("files") };
    }

private:
    BT::NodeStatus tick() override;
};
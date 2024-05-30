#pragma once

#include <filesystem>

#include <behaviortree_cpp/action_node.h>

class ExtractArchive : public BT::StatefulActionNode
{
public:
    ExtractArchive(const std::string &name, const BT::NodeConfig &config)
        : BT::StatefulActionNode(name, config)
    {
    }

    static BT::PortsList providedPorts()
    {
        return { BT::InputPort<std::string>("file"), BT::OutputPort<std::string>("out_dir") };
    }

private:
    BT::NodeStatus onStart() override;
    BT::NodeStatus onRunning() override;
    void onHalted() override;

    void Extract(const std::string &file);

    std::string file_;
};
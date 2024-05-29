#pragma once

#include <filesystem>

#include <behaviortree_cpp/decorators/loop_node.h>

class CopyFile : public BT::SyncActionNode
{
public:
    CopyFile(const std::string &name, const BT::NodeConfig &config)
        : BT::SyncActionNode(name, config)
    {
    }

    BT::NodeStatus tick() override
    {
        auto src = getInput<std::string>("src");
        auto dest = getInput<std::string>("dest");

        if (!src)
        {
            throw BT::RuntimeError("missing required input [src]: ", src.error());
        }
        if (!dest)
        {
            throw BT::RuntimeError("missing required input [dest]: ", dest.error());
        }

        std::filesystem::copy(src.value(), dest.value(), std::filesystem::copy_options::update_existing);

        auto output = (std::filesystem::path(dest.value()) / std::filesystem::path(src.value()).filename()).string();

        std::cout << "Copied " << src.value() << " to " << output << std::endl;

        setOutput("output", output);
        return BT::NodeStatus::SUCCESS;
    }

    static BT::PortsList providedPorts()
    {
        return { BT::InputPort<std::string>("src"), BT::InputPort<std::string>("dest"),
                 BT::OutputPort<std::string>("output") };
    }
};
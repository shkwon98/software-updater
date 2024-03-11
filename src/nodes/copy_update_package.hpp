#pragma once

#include <filesystem>

#include <behaviortree_cpp/decorators/loop_node.h>

class CopyUpdatePackage : public BT::SyncActionNode
{
public:
    CopyUpdatePackage(const std::string &name, const BT::NodeConfig &config)
        : BT::SyncActionNode(name, config)
    {
    }

    BT::NodeStatus tick() override
    {
        auto src_path = getInput<std::string>("src_path");
        auto dst_path =
            (std::filesystem::temp_directory_path() / std::filesystem::path(src_path.value()).filename()).string();

        std::cout << "Copying " << src_path.value() << " to " << dst_path << std::endl;
        std::filesystem::copy(src_path.value(), dst_path, std::filesystem::copy_options::overwrite_existing);

        setOutput("dst_path", dst_path);
        return BT::NodeStatus::SUCCESS;
    }

    static BT::PortsList providedPorts()
    {
        return { BT::InputPort<BT::StringView>("src_path"), BT::OutputPort<BT::StringView>("dst_path") };
    }
};
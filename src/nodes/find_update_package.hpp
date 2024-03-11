#pragma once

#include <filesystem>

#include <behaviortree_cpp/decorators/loop_node.h>

class FindUpdatePackage : public BT::SyncActionNode
{
public:
    FindUpdatePackage(const std::string &name, const BT::NodeConfig &config)
        : BT::SyncActionNode(name, config)
    {
    }

    BT::NodeStatus tick() override
    {
        std::string file_path;
        std::cin >> file_path;

        if (!std::filesystem::exists(file_path))
        {
            std::cout << "Given path not found: " << file_path << std::endl;
            return BT::NodeStatus::FAILURE;
        }

        if (!std::filesystem::is_regular_file(file_path))
        {
            std::cout << "Given path is not a file: " << file_path << std::endl;
            return BT::NodeStatus::FAILURE;
        }

        setOutput("file_path", file_path);
        return BT::NodeStatus::SUCCESS;
    }

    static BT::PortsList providedPorts()
    {
        return { BT::OutputPort<BT::StringView>("file_path") };
    }
};
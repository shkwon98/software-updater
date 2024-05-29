#pragma once

#include <filesystem>

#include <behaviortree_cpp/action_node.h>

class FindLatestPackage : public BT::SyncActionNode
{
public:
    FindLatestPackage(const std::string &name, const BT::NodeConfig &config)
        : BT::SyncActionNode(name, config)
    {
    }

    BT::NodeStatus tick() override
    {
        auto files = getInput<std::vector<std::string>>("files");
        if (!files)
        {
            throw BT::RuntimeError("missing required input [files]: ", files.error());
        }

        std::string file;
        for (const auto &f : files.value())
        {
            if (f.find("copilot_") != std::string::npos)
            {
                if (file.empty())
                {
                    file = f;
                }
                else
                {
                    auto file_version = std::stoi(
                        file.substr(file.find_last_of('_') + 1, file.find_last_of('.') - file.find_last_of('_') - 1));
                    auto f_version =
                        std::stoi(f.substr(f.find_last_of('_') + 1, f.find_last_of('.') - f.find_last_of('_') - 1));
                    if (f_version > file_version)
                    {
                        file = f;
                    }
                }
            }
        }

        setOutput("file", file);
        std::cout << "Latest package: " << file << std::endl;

        return BT::NodeStatus::SUCCESS;
    }

    static BT::PortsList providedPorts()
    {
        return { BT::InputPort<std::vector<std::string>>("files"), BT::OutputPort<std::string>("file") };
    }
};
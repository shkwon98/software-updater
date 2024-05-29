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

    BT::NodeStatus tick() override
    {
        auto path = getInput<std::string>("path");
        auto regex = getInput<std::string>("regex");
        if (!path)
        {
            throw BT::RuntimeError("missing required input [path]: ", path.error());
        }
        if (!regex)
        {
            throw BT::RuntimeError("missing required input [regex]: ", regex.error());
        }

        if (!std::filesystem::exists(path.value()))
        {
            throw BT::RuntimeError("path does not exist: ", path.value());
        }

        std::vector<std::string> files;
        for (const auto &entry : std::filesystem::directory_iterator(path.value()))
        {
            if (entry.is_regular_file())
            {
                auto filepath = entry.path().string();
                auto filename = entry.path().filename().string();

                if (std::regex_match(filename, std::regex(regex.value())))
                {
                    files.push_back(filepath);
                }
            }
        }

        setOutput("files", files);
        return BT::NodeStatus::SUCCESS;
    }

    static BT::PortsList providedPorts()
    {
        return { BT::InputPort<std::string>("path"), BT::InputPort<std::string>("regex"),
                 BT::OutputPort<std::vector<std::string>>("files") };
    }
};
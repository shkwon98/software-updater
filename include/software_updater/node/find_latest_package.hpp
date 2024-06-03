#pragma once

#include <filesystem>
#include <regex>

#include <behaviortree_cpp/action_node.h>

class FindLatestPackage : public BT::SyncActionNode
{
public:
    FindLatestPackage(const std::string &name, const BT::NodeConfig &config)
        : BT::SyncActionNode(name, config)
    {
    }

    static BT::PortsList providedPorts()
    {
        return { BT::InputPort<std::vector<std::string>>("files"), BT::OutputPort<std::string>("file") };
    }

private:
    BT::NodeStatus tick() override
    {
        auto files = getInput<std::vector<std::string>>("files");
        if (!files)
        {
            throw BT::RuntimeError("missing required input [files]: ", files.error());
        }

        for (const auto &f : files.value())
        {
            std::cout << "File: " << f << std::endl;
        }

        std::string latest_package_file;
        std::regex regex(R"(copilot_(\d+)\.(\d+)\.(\d+)\.tar\.gz)");

        int ver_major = 0;
        int ver_minor = 0;
        int ver_patch = 0;

        for (const auto &f : files.value())
        {
            auto file_name = std::filesystem::path(f).filename().string();
            std::smatch match;

            if (std::regex_match(file_name, match, regex))
            {
                int major = std::stoi(match[1]);
                int minor = std::stoi(match[2]);
                int patch = std::stoi(match[3]);

                if (latest_package_file.empty() || std::tie(major, minor, patch) > std::tie(ver_major, ver_minor, ver_patch))
                {
                    latest_package_file = f;
                    ver_major = major;
                    ver_minor = minor;
                    ver_patch = patch;
                }
            }
        }

        setOutput("file", latest_package_file);
        std::cout << "Latest package: " << latest_package_file << std::endl;

        return BT::NodeStatus::SUCCESS;
    }
};
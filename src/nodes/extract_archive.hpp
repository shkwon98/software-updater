#pragma once

#include <filesystem>

#include <behaviortree_cpp/decorators/loop_node.h>

class ExtractArchive : public BT::SyncActionNode
{
public:
    ExtractArchive(const std::string &name, const BT::NodeConfig &config)
        : BT::SyncActionNode(name, config)
    {
    }

    BT::NodeStatus tick() override
    {
        auto file_path = getInput<std::string>("file_path").value();
        std::string extension(std::find(file_path.begin(), file_path.end(), '.'), file_path.end());

        if (extension == ".zip")
        {
            std::string out_dir = std::filesystem::path(file_path).replace_extension("").string();
            std::cout << "Extracting " << file_path << " to " << out_dir << "..." << std::endl;

            std::filesystem::create_directories(out_dir);
            std::string cmd = "unzip -o " + file_path + " -d " + out_dir;
            std::system(cmd.c_str());
            setOutput("out_dir", out_dir);
            return BT::NodeStatus::SUCCESS;
        }
        else if (extension == ".tar.gz")
        {
            std::string out_dir = std::filesystem::path(file_path).replace_extension("").replace_extension("").string();
            std::cout << "Extracting " << file_path << " to " << out_dir << "..." << std::endl;

            std::filesystem::create_directories(out_dir);
            std::string cmd = "tar -xzf " + file_path + " -C " + out_dir;
            std::system(cmd.c_str());
            setOutput("out_dir", out_dir);
            return BT::NodeStatus::SUCCESS;
        }

        std::cout << "Unsupported file type: " << file_path << std::endl;
        return BT::NodeStatus::FAILURE;
    }

    static BT::PortsList providedPorts()
    {
        return { BT::InputPort<BT::StringView>("file_path"), BT::OutputPort<BT::StringView>("out_dir") };
    }
};
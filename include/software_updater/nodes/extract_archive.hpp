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
        auto file = getInput<std::string>("file").value();

        if (file.empty())
        {
            throw BT::RuntimeError("missing required input [file]: ", file);
        }

        std::string extension(std::filesystem::path(file).extension());

        if (extension == ".zip")
        {
            std::string out_dir = std::filesystem::path(file).replace_extension("").string();
            std::cout << "Extracting " << file << " to " << out_dir << "..." << std::endl;

            std::filesystem::create_directories(out_dir);
            std::string cmd = "unzip -o " + file + " -d " + out_dir;
            std::system(cmd.c_str());

            std::cout << "Extracted " << file << " to " << out_dir << std::endl;

            setOutput("out_dir", out_dir);
            return BT::NodeStatus::SUCCESS;
        }
        else if (extension == ".gz")
        {
            std::string out_dir = std::filesystem::path(file).replace_extension("").replace_extension("").string();
            std::cout << "Extracting " << file << " to " << out_dir << "..." << std::endl;

            std::filesystem::create_directories(out_dir);
            std::string cmd = "tar -xzf " + file + " -C " + out_dir;
            std::system(cmd.c_str());

            std::cout << "Extracted " << file << " to " << out_dir << std::endl;

            setOutput("out_dir", out_dir);
            return BT::NodeStatus::SUCCESS;
        }

        std::cout << "Unsupported file type: " << file << std::endl;
        return BT::NodeStatus::FAILURE;
    }

    static BT::PortsList providedPorts()
    {
        return { BT::InputPort<std::string>("file"), BT::OutputPort<std::string>("out_dir") };
    }
};
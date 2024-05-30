#include "bt_node/archive/extract_archive.h"

BT::NodeStatus ExtractArchive::onStart()
{
    file_ = getInput<std::string>("file").value();

    if (file_.empty())
    {
        throw BT::RuntimeError("missing required input [file]: ", file_);
    }

    return BT::NodeStatus::RUNNING;
}

BT::NodeStatus ExtractArchive::onRunning()
{
    std::string extension(std::filesystem::path(file_).extension());

    if (extension == ".zip")
    {
        std::string out_dir = std::filesystem::path(file_).replace_extension("").string();
        std::cout << "Extracting " << file_ << " to " << out_dir << "..." << std::endl;

        std::filesystem::create_directories(out_dir);
        std::string cmd = "unzip -o " + file_ + " -d " + out_dir;
        std::system(cmd.c_str());

        std::cout << "Extracted " << file_ << " to " << out_dir << std::endl;

        setOutput("out_dir", out_dir);
        return BT::NodeStatus::SUCCESS;
    }
    else if (extension == ".gz")
    {
        std::string out_dir = std::filesystem::path(file_).replace_extension("").replace_extension("").string();
        std::cout << "Extracting " << file_ << " to " << out_dir << "..." << std::endl;

        std::filesystem::create_directories(out_dir);
        std::string cmd = "tar -xzf " + file_ + " -C " + out_dir;
        std::system(cmd.c_str());

        std::cout << "Extracted " << file_ << " to " << out_dir << std::endl;

        setOutput("out_dir", out_dir);
        return BT::NodeStatus::SUCCESS;
    }

    std::cout << "Unsupported file type: " << file_ << std::endl;
    return BT::NodeStatus::FAILURE;
}

void ExtractArchive::onHalted()
{
}

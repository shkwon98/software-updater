#include "bt_node/filesystem/find_file.h"

BT::NodeStatus FindFile::tick()
{
    auto path = getInput<std::string>("path");
    auto expression = getInput<std::string>("expression");
    if (!path)
    {
        throw BT::RuntimeError("missing required input [path]: ", path.error());
    }
    if (!expression)
    {
        throw BT::RuntimeError("missing required input [expression]: ", expression.error());
    }

    if (!std::filesystem::exists(path.value()))
    {
        throw BT::RuntimeError("path does not exist: ", path.value());
    }

    // Find files that match the expression in the given path
    // path can be a directory or a file
    std::vector<std::string> files;
    if (std::filesystem::is_directory(path.value()))
    {
        for (const auto &entry : std::filesystem::directory_iterator(path.value()))
        {
            if (entry.is_regular_file())
            {
                auto filepath = entry.path().string();
                auto filename = entry.path().filename().string();

                if (std::regex_match(filename, std::regex(expression.value())))
                {
                    files.push_back(filepath);
                }
            }
        }
    }
    else if (std::filesystem::is_regular_file(path.value()))
    {
        auto filename = std::filesystem::path(path.value()).filename().string();
        if (std::regex_match(filename, std::regex(expression.value())))
        {
            files.push_back(path.value());
        }
    }

    if (files.empty())
    {
        return BT::NodeStatus::FAILURE;
    }

    setOutput("files", files);
    return BT::NodeStatus::SUCCESS;
}

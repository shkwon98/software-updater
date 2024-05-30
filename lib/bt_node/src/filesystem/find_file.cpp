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

    std::vector<std::string> files;
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

    setOutput("files", files);
    return BT::NodeStatus::SUCCESS;
}

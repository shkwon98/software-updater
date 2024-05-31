#include "bt_node/filesystem/get_file_type.h"

BT::NodeStatus GetFileType::tick()
{
    auto path = getInput<std::string>("file");
    if (!path)
    {
        throw BT::RuntimeError("missing required input [path]: ", path.error());
    }

    // get file type
    auto type = std::filesystem::status(path.value()).type();

    switch (type)
    {
    case std::filesystem::file_type::regular:
        setOutput("type", "regular");
        break;
    case std::filesystem::file_type::directory:
        setOutput("type", "directory");
        break;
    case std::filesystem::file_type::symlink:
        setOutput("type", "symlink");
        break;
    case std::filesystem::file_type::block:
        setOutput("type", "block");
        break;
    case std::filesystem::file_type::character:
        setOutput("type", "character");
        break;
    case std::filesystem::file_type::fifo:
        setOutput("type", "fifo");
        break;
    case std::filesystem::file_type::socket:
        setOutput("type", "socket");
        break;

    case std::filesystem::file_type::none:
        setOutput("type", "none");
    case std::filesystem::file_type::not_found:
        setOutput("type", "not_found");
    case std::filesystem::file_type::unknown:
        setOutput("type", "unknown");
    default:
        return BT::NodeStatus::FAILURE;
    }

    return BT::NodeStatus::SUCCESS;
}

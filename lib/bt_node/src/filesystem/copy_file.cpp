#include "bt_node/filesystem/copy_file.h"

BT::NodeStatus CopyFile::tick()
{
    auto src = getInput<std::string>("src");
    auto dest = getInput<std::string>("dest");

    if (!src)
    {
        throw BT::RuntimeError("missing required input [src]: ", src.error());
    }
    if (!dest)
    {
        throw BT::RuntimeError("missing required input [dest]: ", dest.error());
    }

    std::filesystem::copy(src.value(), dest.value(), std::filesystem::copy_options::update_existing);

    auto output = (std::filesystem::path(dest.value()) / std::filesystem::path(src.value()).filename()).string();

    std::cout << "Copied " << src.value() << " to " << output << std::endl;

    setOutput("output", output);
    return BT::NodeStatus::SUCCESS;
}

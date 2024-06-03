#include "software_updater/node/find_latest_package_node.h"

#include <filesystem>
#include <regex>

namespace software_updater::node
{

BT::NodeStatus FindLatestPackageNode::tick(void)
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

} // namespace software_updater::node
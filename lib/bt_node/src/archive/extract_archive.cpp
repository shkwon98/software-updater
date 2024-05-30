#include <archive.h>
#include <archive_entry.h>

#include "bt_node/archive/extract_archive.h"

BT::NodeStatus ExtractArchive::onStart()
{
    progress_ = 0.0f;
    file_ = getInput<std::string>("file").value();

    if (file_.empty())
    {
        throw BT::RuntimeError("missing required input [file]: ", file_);
    }

    if (const auto &extension = std::filesystem::path(file_).extension(); extension == ".gz")
    {
        const auto &out_dir = std::filesystem::path(file_).replace_extension("").replace_extension("").string();
        std::filesystem::create_directories(out_dir);
        std::cout << "Extracting " << file_ << " to " << out_dir << "..." << std::endl;

        future_ = std::async(std::launch::async, &ExtractArchive::ExtractGzip, this, file_);

        setOutput("out_dir", out_dir);
        return BT::NodeStatus::RUNNING;
    }

    std::cout << "Unsupported file type: " << file_ << std::endl;
    return BT::NodeStatus::FAILURE;
}

BT::NodeStatus ExtractArchive::onRunning()
{
    if (future_.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
    {
        if (future_.get())
        {
            std::cout << "Progress: " << progress_ << "%" << std::endl;
            std::cout << "Extracted " << file_ << std::endl;
            return BT::NodeStatus::SUCCESS;
        }

        std::cerr << "Failed to extract " << file_ << std::endl;
        return BT::NodeStatus::FAILURE;
    }

    std::cout << "Progress: " << progress_ << "%" << std::endl;
    return BT::NodeStatus::RUNNING;
}

void ExtractArchive::onHalted()
{
}

bool ExtractArchive::ExtractGzip(const std::string &filename)
{
    constexpr const size_t kBlockSize = 10240;
    struct archive_entry *entry;

    size_t total_size = 0;
    size_t extracted_size = 0;

    // Calculate the total size of the archive for progress tracking
    auto *archive = archive_read_new();
    {
        archive_read_support_format_tar(archive);
        archive_read_support_filter_gzip(archive);
        if (archive_read_open_filename(archive, filename.c_str(), kBlockSize) != ARCHIVE_OK)
        {
            std::cerr << "Failed to open archive: " << archive_error_string(archive) << std::endl;
            return false;
        }

        while (archive_read_next_header(archive, &entry) == ARCHIVE_OK)
        {
            total_size += archive_entry_size(entry);
            archive_read_data_skip(archive);
        }

        archive_read_close(archive);
        archive_read_free(archive);
    }

    // Extract the archive
    archive = archive_read_new();
    {
        auto *extracted = archive_write_disk_new();
        archive_read_support_format_tar(archive);
        archive_read_support_filter_gzip(archive);
        if (archive_read_open_filename(archive, filename.c_str(), kBlockSize) != ARCHIVE_OK)
        {
            std::cerr << "Failed to open archive: " << archive_error_string(archive) << std::endl;
            return false;
        }

        while (archive_read_next_header(archive, &entry) == ARCHIVE_OK)
        {
            if (archive_write_header(extracted, entry) != ARCHIVE_OK)
            {
                std::cerr << archive_error_string(extracted) << std::endl;
            }
            else
            {
                const void *buff;
                size_t size;
                int64_t offset;

                while (true)
                {
                    if (isHaltRequested())
                    {
                        return false;
                    }

                    if (auto ret = archive_read_data_block(archive, &buff, &size, &offset); ret == ARCHIVE_EOF)
                    {
                        break;
                    }
                    else if (ret != ARCHIVE_OK)
                    {
                        std::cerr << "Error reading data: " << archive_error_string(archive) << std::endl;
                        return false;
                    }

                    if (archive_write_data_block(extracted, buff, size, offset) != ARCHIVE_OK)
                    {
                        std::cerr << "Error writing data: " << archive_error_string(extracted) << std::endl;
                        return false;
                    }

                    extracted_size += size;
                    progress_ = 100.0f * extracted_size / total_size;
                }
            }
            archive_write_finish_entry(extracted);
        }

        archive_read_close(archive);
        archive_read_free(archive);
        archive_write_close(extracted);
        archive_write_free(extracted);
    }

    progress_ = 100.0f;
    return true;
}
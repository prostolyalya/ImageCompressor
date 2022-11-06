#ifndef IMAGECOMPRESS_H
#define IMAGECOMPRESS_H
#include <filesystem>
#include <future>
#include "Image.h"

namespace ImageCompress {

constexpr const char* file_extension_uncompressed = ".bmp";
constexpr const char* file_extension_compressed = ".barch";

enum class CompressError
{
    OK,
    READING_IMAGE,
    INTERNAL_ERROR
};

enum class ReadingError
{
    OK,
    INVALID_PATH,
    INVALID_FILE,
    INVALID_IMAGE_HEADER
};

bool check_file_exist(const std::filesystem::path& filepath);

void compress_image(const std::string& filepath, std::function<void(CompressError)> callback);

void uncompress_image(const std::string& filepath, std::function<void(CompressError)> callback);
} // namespace ImageCompress

#endif // IMAGECOMPRESS_H

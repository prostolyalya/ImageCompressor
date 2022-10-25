#ifndef IMAGEREADER_H
#define IMAGEREADER_H

#include "include/ImageReader.h"
#include "include/Image.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <deque>
namespace ImageReader {
constexpr const char* file_extension = ".bmp";
constexpr const char* file_extension_compressed = ".barch";
constexpr uint bmp_header_size = 54;

enum class CompressError
{
    OK,
    READING_IMAGE,
    FAILED
};

enum class ReadingError
{
    OK,
    INVALID_PATH,
    INVALID_FILE,
    INVALID_IMAGE_HEADER
};

bool
check_file(const std::filesystem::path& filepath)
{
    return std::filesystem::exists(filepath) && filepath.extension() == file_extension;
}

bool
check_file_compressed(const std::filesystem::path& filepath)
{
    return std::filesystem::exists(filepath) && filepath.extension() == file_extension_compressed;
}

ReadingError
read_image(const std::string& filepath, Image& image)
{
    std::filesystem::path path(filepath);
    if (!check_file(path))
    {
        std::cout << "Cannot read file: " << filepath << std::endl;
        return ReadingError::INVALID_PATH;
    }
    uint file_size = 0;
    int width = 0;
    int height = 0;

    std::ifstream file_stream(filepath);

    file_stream.seekg(0, std::ios::end);
    file_size = file_stream.tellg();
    if (file_size < bmp_header_size)
    {
        std::cout << "Invalid file: " << filepath << std::endl;
        return ReadingError::INVALID_FILE;
    }
    auto data_size = file_size - bmp_header_size;

    image.header_data = std::make_unique<unsigned char[]>(bmp_header_size);
    image.image_data = std::make_unique<unsigned char[]>(data_size);
    auto header_data = image.header_data.get();
    auto image_data = image.image_data.get();

    file_stream.seekg(0);
    file_stream.read(reinterpret_cast<char*>(header_data), bmp_header_size);
    file_stream.read(reinterpret_cast<char*>(image_data), data_size);
    file_stream.close();

    uint file_size_from_header = *(reinterpret_cast<int*>(&header_data[2]));
    width = *(reinterpret_cast<uint*>(&header_data[18]));
    height = *(reinterpret_cast<uint*>(&header_data[22]));
    if (file_size_from_header != file_size)
    {
        std::cout << "Invalid BMP header" << std::endl;
        return ReadingError::INVALID_IMAGE_HEADER;
    }
    std::cout << width << std::endl;
    std::cout << height << std::endl;
    std::cout << file_size_from_header << std::endl;
    image.bitmap = {width, height, image_data};
    return ReadingError::OK;
}

void
compress_bitmap(Bitmap& bitmap)
{
    std::deque<bool> index;
    index.resize(bitmap.height);
    for (uint column = 0; column < bitmap.height; column++)
    {
        for(uint row = 0; row < bitmap.width; row++)
        {

        }
    }
}

CompressError
compress_image(const std::string& filepath)
{
    Image image{};
    auto err_reading = read_image(filepath, image);
    if (err_reading != ReadingError::OK)
    {
        return CompressError::READING_IMAGE;
    }

    return CompressError::OK;
}

CompressError
uncompress_image(const std::string& filepath)
{
    Image image{};
    auto err_reading = read_image(filepath, image);
    if (err_reading != ReadingError::OK)
    {
        return CompressError::READING_IMAGE;
    }

    return CompressError::OK;
}
} // namespace ImageReader

#endif // IMAGEREADER_H

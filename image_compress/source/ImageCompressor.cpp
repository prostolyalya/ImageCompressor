#include "include/ImageCompressor.h"
#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>

namespace ImageCompress {
using bits = std::bitset<8>;

constexpr const char* white_index = "0";
constexpr const char* black_index = "10";
constexpr const char* diff_index = "11";
constexpr const unsigned char white_color = 0xff;
constexpr const unsigned char black_color = 0x00;
constexpr const unsigned char padding_symb = 0xf0;
constexpr uint bmp_header_min_size = 54;
constexpr uint size_for_compress = 4;
constexpr size_t byte_size = 8;

inline bool check_white_color(unsigned char color)
{
    return color == white_color;
}

inline bool check_black_color(unsigned char color)
{
    return color == black_color;
}

bool check_file_exist(const std::filesystem::path& filepath)
{
    return !filepath.empty() && std::filesystem::exists(filepath);
}

bool check_file(const std::filesystem::path& filepath, const char* extension)
{
    return check_file_exist(filepath) && filepath.extension() == extension;
}

bool check_for_padding(int image_width, uint& row_with_padding)
{
    bool padding = false;
    if (image_width & 1 || row_with_padding % 4 != 0) // check for padding
    {
        padding = true;
        ++row_with_padding;
        for (; row_with_padding % 4 != 0; row_with_padding++)
            ;
    }
    return padding;
}

std::string parse_filename_for_save(const std::string& filepath, std::string extension)
{
    auto pos_dot = filepath.find_last_of('.');
    std::string new_file = filepath.substr(0, pos_dot);
    if (extension == file_extension_uncompressed)
    {
        new_file = new_file.substr(0, pos_dot) + "_unpacked(1)" + extension;
    }
    else
    {
        new_file = new_file.substr(0, pos_dot) + "(1)" + extension;
    }
    if (check_file_exist(new_file))
    {
        pos_dot = new_file.find_last_of('.');
        auto first_num_pos = new_file.find_last_of('(') + 1;
        auto str_count = new_file.substr(first_num_pos, pos_dot - first_num_pos - 1);
        auto count = std::stoi(str_count) + 1;
        for (; check_file_exist(new_file); ++count)
        {
            new_file = new_file.substr(0, first_num_pos - 1) + '(' + std::to_string(count) + ')' + extension;
        }
    }

    return new_file;
}

ReadingError read_image(const std::string& filepath, Image& image, const char* extension)
{
    std::filesystem::path path(filepath);
    if (!check_file(path, extension))
    {
        std::cout << "Cannot read file: " << filepath << std::endl;
        return ReadingError::INVALID_PATH;
    }

    std::ifstream file_stream(filepath);

    file_stream.seekg(0, std::ios::end);
    uint file_size = file_stream.tellg();
    file_stream.seekg(0);
    if (file_size < bmp_header_min_size)
    {
        std::cout << "Invalid file: " << filepath << std::endl;
        return ReadingError::INVALID_FILE;
    }
    image.data = std::make_unique<unsigned char[]>(file_size);
    auto data = image.data.get();

    file_stream.read(reinterpret_cast<char*>(data), file_size);
    file_stream.close();

    int width = *(reinterpret_cast<uint*>(&data[18]));
    int height = *(reinterpret_cast<uint*>(&data[22]));
    int depth = *(reinterpret_cast<uint*>(&data[28]));
    int header_size = *(reinterpret_cast<uint*>(&data[10]));

    image.file_size = file_size;
    image.depth = depth;
    image.header_size = header_size;
    image.bitmap = { width, height, data + header_size };
    return ReadingError::OK;
}

std::stringstream uncompress_bitmap(Image& image)
{
    std::stringstream result_stream;
    const auto& bitmap = image.bitmap;
    const double size_for_pixel = image.depth / static_cast<double>(byte_size);
    const uint size_for_parse = size_for_compress * size_for_pixel;

    uint row_size = bitmap.width * size_for_pixel;
    uint row_with_padding = row_size;
    bool padding = check_for_padding(bitmap.width, row_with_padding);
    auto padding_size = row_with_padding - row_size - 1;
    auto image_data = reinterpret_cast<char*>(bitmap.data + bitmap.height);
    const std::string white_row(row_size, 0xff);

    std::stringstream image_data_stream;
    auto compress_data_size = image.file_size - image.bitmap.height - image.header_size;
    for (uint i = 0; i < compress_data_size; i += 2) // convert from hex to bin
    {
        std::stringstream hex_buffer;
        hex_buffer << std::hex << image_data[i] << image_data[i + 1];
        uint value;
        hex_buffer >> value;
        image_data_stream << bits(value);
    }
    for (uint column = 0; column < bitmap.height; column++)
    {
        if (bitmap.data[column] == '0')
        {
            result_stream << white_row;
        }
        else
        {
            uint ssize = uint(result_stream.tellp()) + row_size;
            for (uint i = result_stream.tellp(); i < ssize; i = result_stream.tellp())
            {
                char buffer;
                image_data_stream.read(&buffer, 1);
                if (buffer == '0') // 0 - white pixels
                {
                    for (uint byte_i = 0; byte_i < size_for_parse; byte_i++)
                    {
                        result_stream << white_color;
                    }
                }
                else
                {
                    image_data_stream.read(&buffer, 1);
                    if (buffer == '0') // 10 - black pixels
                    {
                        for (uint byte_i = 0; byte_i < size_for_parse; byte_i++)
                        {
                            result_stream << black_color;
                        }
                    }
                    else // 11 - diff pixels
                    {
                        for (uint byte_i = 0; byte_i < size_for_parse; byte_i++)
                        {
                            char bits_buffer[byte_size];
                            image_data_stream.read(bits_buffer, 8);
                            result_stream << static_cast<unsigned char>(bits(bits_buffer).to_ulong());
                        }
                    }
                }
            }
        }
        if (padding)
        {
            result_stream << padding_symb;
            for (uint byte_i = 0; byte_i < padding_size; byte_i++)
            {
                result_stream << black_color;
            }
        }
    }
    return result_stream;
}

std::stringstream compress_bitmap(Image& image)
{
    std::stringstream stream;
    const auto& bitmap = image.bitmap;

    const double size_for_pixel = image.depth / static_cast<double>(byte_size);
    const uint size_for_parse = size_for_compress * size_for_pixel;
    uint row_size = bitmap.width * size_for_pixel;
    uint row_with_padding = row_size;
    check_for_padding(bitmap.width, row_with_padding);
    image.index = std::make_unique<unsigned char[]>(bitmap.height);
    auto index = image.index.get();
    for (uint column = 0; column < bitmap.height; column++)
    {
        auto current_data = bitmap.data + column * row_with_padding;
        if (std::all_of(current_data, current_data + row_size, check_white_color)) // white row
        {
            index[column] = '0';
        }
        else
        {
            index[column] = '1';
            for (uint row = 0; row < row_size; row += size_for_parse)
            {
                if (std::all_of(current_data + row, current_data + row + size_for_parse,
                                check_white_color)) // 0 - white pixels
                {
                    stream << white_index;
                }
                else if (std::all_of(current_data + row, current_data + row + size_for_parse,
                                     check_black_color)) // 10 - black pixels
                {
                    stream << black_index;
                }
                else // 11 - diff pixels
                {
                    stream << diff_index;
                    for (uint byte_i = row; byte_i < (row + size_for_parse); byte_i++)
                    {
                        stream << bits(*(current_data + byte_i));
                    }
                }
            }
        }
    }
    return stream;
}

void save_uncompress_image(const std::string& filepath, std::stringstream& stream, Image& image)
{
    auto filename = parse_filename_for_save(filepath, file_extension_uncompressed);

    std::ofstream file(filename);
    file.write(reinterpret_cast<char*>(image.data.get()), image.header_size);
    file << stream.rdbuf();
    file.close();
}

void save_compress_image(const std::string& filepath, std::stringstream& stream, Image& image)
{
    std::string new_file = parse_filename_for_save(filepath, file_extension_compressed);

    std::ofstream file(new_file);
    file.write(reinterpret_cast<char*>(image.data.get()), image.header_size);
    file.write(reinterpret_cast<char*>(image.index.get()), image.bitmap.height);
    stream.seekg(0, std::ios::end);
    uint size = stream.tellg();
    stream.seekg(0, std::ios::beg);

    for (uint i = 0; i < size; i += byte_size)
    {
        unsigned char byte[byte_size];
        stream.read(reinterpret_cast<char*>(byte), byte_size);
        auto num = bits(byte).to_ulong();
        if (num < 16)
        {
            file << 0 << num;
        }
        else
        {
            file << std::hex << num;
        }
    }

    file.close();
}

void uncompress_image(const std::string& filepath, std::function<void(CompressError)> callback)
{
    Image image {};
    auto err_reading = read_image(filepath, image, file_extension_compressed);
    if (err_reading != ReadingError::OK)
    {
        return callback(CompressError::READING_IMAGE);
    }
    auto stream = uncompress_bitmap(image);
    save_uncompress_image(filepath, stream, image);
    return callback(CompressError::OK);
}

void compress_image(const std::string& filepath, std::function<void(CompressError)> callback)
{
    Image image {};
    auto err_reading = read_image(filepath, image, file_extension_uncompressed);
    if (err_reading != ReadingError::OK)
    {
        return callback(CompressError::READING_IMAGE);
    }
    auto stream = compress_bitmap(image);
    save_compress_image(filepath, stream, image);
    return callback(CompressError::OK);
}

} // namespace ImageCompress

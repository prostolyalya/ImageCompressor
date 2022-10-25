#ifndef IMAGEREADER_H
#define IMAGEREADER_H
#include "Image.h"
namespace ImageReader {

enum class CompressError;
enum class ReadingError;
ReadingError read_image(const std::string& filepath, Image& bitmap);
CompressError compress_image(const std::string& filepath);
}

#endif // IMAGEREADER_H

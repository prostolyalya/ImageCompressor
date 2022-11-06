#ifndef IMAGE_H
#define IMAGE_H
#include "Bitmap.h"
#include <memory>
#include <deque>

struct Image
{
    Bitmap bitmap;
    std::unique_ptr<unsigned char[]> data = nullptr;
    std::unique_ptr<unsigned char[]> index = nullptr;
    uint depth;
    uint header_size;
    uint file_size;
    bool is_empty()
    {
        return !data;
    }
};

#endif // IMAGE_H

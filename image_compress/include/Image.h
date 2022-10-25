#ifndef IMAGE_H
#define IMAGE_H
#include "Bitmap.h"
#include <memory>
struct Image
{
    Bitmap bitmap;
    std::unique_ptr<unsigned char[]> header_data = nullptr;
    std::unique_ptr<unsigned char[]> image_data = nullptr;
    bool is_empty()
    {
        return !header_data || !image_data;
    }
};




#endif // IMAGE_H

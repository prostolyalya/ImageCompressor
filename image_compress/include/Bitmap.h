#ifndef BITMAP_H
#define BITMAP_H

struct Bitmap // Used from task description
{
    int width;           // bitmap width in pixels
    int height;          // bitmap height in pixels
    unsigned char* data = nullptr; // Pointer to bitmap data. data[j * width + i] is color of pixel in row j
                         // and column i.
};

#endif // BITMAP_H

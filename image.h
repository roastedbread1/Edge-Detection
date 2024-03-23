#include <cstdint>
#include <iostream>

enum ImageType {
    PNG, JPG
};

struct Image {

    uint8_t* data = nullptr;
    size_t size = 0;
    int w;
    int h;
    int channels;

    Image(const char* filename);
    Image(int w, int h, int channels);
    Image(const Image& img);
    ~Image();

    bool read(const char* filename);
    bool write(const char* filename);

    ImageType getFileType(const char* filename);

    Image& grayscale();
    Image& blacknwhite();

    Image& edge_detection();
    int gradX(int x, int y);
    int gradY(int x, int  y);
};


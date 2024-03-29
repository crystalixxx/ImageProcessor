#include <fstream>
#include <cstdint>

#pragma pack(push, 1)
struct BMPHeader {
    char id[2];
    uint32_t size;
    uint16_t unused1;
    uint16_t unused2;
    uint32_t offset;
};

struct BitMapOfHeader {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t color_planes;
    uint16_t bits;
    uint32_t compression;
    uint32_t size_of_image;
    uint32_t per_pixels_x;
    uint32_t per_pixels_y;
    uint32_t used_colors;
    uint32_t important_colors;
};
#pragma pack(pop)

struct Pixel {
    double r;
    double g;
    double b;
};

inline Pixel& operator+=(Pixel& lhs, const Pixel& rhs) {
    lhs.r += rhs.r;
    lhs.g += rhs.g;
    lhs.b += rhs.b;

    return lhs;
}

inline Pixel& operator-=(Pixel& lhs, const Pixel& rhs) {
    lhs.r -= rhs.r;
    lhs.g -= rhs.g;
    lhs.b -= rhs.b;

    return lhs;
}

inline Pixel operator*(const double a, Pixel& rhs) {
    Pixel new_color{rhs.r * a, rhs.g * a, rhs.b * a};

    return new_color;
}

inline bool operator==(const Pixel& lhs, const Pixel& rhs) {
    return (lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == lhs.b);
}

#include "bmp_format.h"

BMP::BMP(std::string& path) {
    std::ifstream file(path, std::ios::binary);

    file.ignore(sizeof(BMPHeader));
    file.read(reinterpret_cast<char*>(&image_header), sizeof(BitMapOfHeader));
    bits.assign(image_header.height, std::vector<Pixel>(image_header.width));

    padding_ = ((4 - (image_header.width * 3) % 4) % 4);

    const double value_to_normal_color = 255.0f;
    for (int i = 0; i < image_header.height; ++i) {
        for (int j = 0; j < image_header.width; ++j) {
            uint8_t colors[3] = {0, 0, 0};
            file.read(reinterpret_cast<char*>(&colors), 3);

            bits[i][j].r = static_cast<double>(colors[2]) / value_to_normal_color;
            bits[i][j].g = static_cast<double>(colors[1]) / value_to_normal_color;
            bits[i][j].b = static_cast<double>(colors[0]) / value_to_normal_color;
        }

        file.ignore(padding_);
    }

    file.close();
}

void BMP::MakeOutputBMP(std::string& output_path) {
    padding_ = ((4 - (image_header.width * 3) % 4) % 4);

    const int size_of_structs = sizeof(BMPHeader) + sizeof(BitMapOfHeader);
    const uint32_t new_size =
        size_of_structs + image_header.height * image_header.width * 3 + padding_ * image_header.height;

    BMPHeader output_header{{'B', 'M'}, new_size, 0, 0, size_of_structs};
    BitMapOfHeader output_image_header{
        image_header.size,         image_header.width,       image_header.height,          image_header.color_planes,
        image_header.bits,         image_header.compression, image_header.size_of_image,   image_header.per_pixels_x,
        image_header.per_pixels_y, image_header.used_colors, image_header.important_colors};

    std::ofstream output(output_path, std::ios::binary);

    output.write(reinterpret_cast<char*>(&output_header), sizeof(BMPHeader));
    output.write(reinterpret_cast<char*>(&output_image_header), sizeof(BitMapOfHeader));

    const double value_to_normal_color = 255.0f;
    for (int i = 0; i < image_header.height; ++i) {
        for (int j = 0; j < image_header.width; ++j) {
            uint8_t r = static_cast<uint8_t>(bits[i][j].r * value_to_normal_color);
            uint8_t g = static_cast<uint8_t>(bits[i][j].g * value_to_normal_color);
            uint8_t b = static_cast<uint8_t>(bits[i][j].b * value_to_normal_color);

            uint8_t color[3] = {b, g, r};
            output.write(reinterpret_cast<char*>(color), 3);
        }

        uint8_t padding_array[3] = {0, 0, 0};
        output.write(reinterpret_cast<char*>(padding_array), padding_);
    }

    output.close();
}

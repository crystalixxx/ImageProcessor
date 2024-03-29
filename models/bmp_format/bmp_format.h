#include "bmp_source.h"
#include <fstream>
#include <vector>

#pragma once
class BMP {
public:
    BitMapOfHeader image_header;

    std::vector<std::vector<Pixel>> bits;

private:
    uint32_t padding_;

public:
    BMP() = default;

    explicit BMP(std::string& path);

    void MakeOutputBMP(std::string& output_path);
};

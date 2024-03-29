#include "filters.h"
#include <set>
#include <cmath>
#include "../exceptions/exceptions.h"

Pixel FindNearest(const BMP& bmp, int x, int y) {
    int new_x = std::max(0, std::min(x, bmp.image_header.height - 1));
    int new_y = std::max(0, std::min(y, bmp.image_header.width - 1));

    return bmp.bits[new_x][new_y];
}

void ApplyMatrix(const BMP& bmp, std::vector<std::vector<Pixel>>& bits, const std::vector<std::vector<int>>& matrix) {
    int height = static_cast<int>(bits.size());
    int width = static_cast<int>(bits[0].size());

    int matrix_height = static_cast<int>(matrix.size());
    int matrix_width = static_cast<int>(matrix[0].size());

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            for (int x = 0; x < matrix_height; ++x) {
                for (int y = 0; y < matrix_width; ++y) {
                    int new_x = i + x - 1;
                    int new_y = j + y - 1;
                    Pixel current = FindNearest(bmp, new_x, new_y);

                    bits[i][j] += matrix[x][y] * current;
                }
            }

            bits[i][j].r = std::min(1.0, std::max(0.0, bits[i][j].r));
            bits[i][j].g = std::min(1.0, std::max(0.0, bits[i][j].g));
            bits[i][j].b = std::min(1.0, std::max(0.0, bits[i][j].b));
        }
    }
}

// Source filter class implementation
SourceFilters::SourceFilters() {
    filter["-crop"] = new CropFilter{};
    filter["-gs"] = new GSFilter{};
    filter["-neg"] = new NegativeFilter{};
    filter["-sharp"] = new SharpeningFilter{};
    filter["-edge"] = new EdgeFilter{};
    filter["-blur"] = new BlurFilter{};
    filter["-pixellate"] = new PixellateFilter{};
}

void CropFilter::ApplyFilter(BMP& bmp, const std::vector<std::string>& params) {
    if (params.size() != 2) {
        throw InvalidParams("Введено неверное число параметров");
    }

    int new_width = bmp.image_header.width;
    new_width = std::min(std::stoi(params[0]), new_width);

    int new_height = bmp.image_header.height;
    new_height = std::min(std::stoi(params[1]), new_height);

    std::vector<std::vector<Pixel>> new_bits(new_height, std::vector<Pixel>(new_width));

    for (int i = 0; i < new_height; ++i) {
        for (int j = 0; j < new_width; ++j) {
            new_bits[new_height - 1 - i][j] = bmp.bits[bmp.image_header.height - 1 - i][j];
        }
    }

    bmp.bits = std::move(new_bits);
    bmp.image_header.width = new_width;
    bmp.image_header.height = new_height;
}

void GSFilter::ApplyFilter(BMP& bmp, const std::vector<std::string>& params) {
    if (!params.empty()) {
        throw InvalidParams("Введено неверное число параметров");
    }

    const double red_coeff = 0.299;
    const double green_coeff = 0.587;
    const double blue_coeff = 0.114;

    for (int i = 0; i < bmp.image_header.height; ++i) {
        for (int j = 0; j < bmp.image_header.width; ++j) {
            double new_color =
                red_coeff * bmp.bits[i][j].r + green_coeff * bmp.bits[i][j].g + blue_coeff * bmp.bits[i][j].b;
            bmp.bits[i][j] = {new_color, new_color, new_color};
        }
    }
}

void NegativeFilter::ApplyFilter(BMP& bmp, const std::vector<std::string>& params) {
    if (!params.empty()) {
        throw InvalidParams("Введено неверное число параметров");
    }

    for (auto& vec_of_pixels : bmp.bits) {
        for (auto& pixel : vec_of_pixels) {
            pixel = {1 - pixel.r, 1 - pixel.g, 1 - pixel.b};
        }
    }
}

void SharpeningFilter::ApplyFilter(BMP& bmp, const std::vector<std::string>& params) {
    if (!params.empty()) {
        throw InvalidParams("Введено неверное число параметров");
    }

    const std::vector<std::vector<int>> matrix = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};

    std::vector<std::vector<Pixel>> new_colors(bmp.image_header.height,
                                               std::vector<Pixel>(bmp.image_header.width, Pixel{0, 0, 0}));
    ApplyMatrix(bmp, new_colors, matrix);

    bmp.bits = std::move(new_colors);
}

void EdgeFilter::ApplyFilter(BMP& bmp, const std::vector<std::string>& params) {
    if (params.size() != 1) {
        throw InvalidParams("Введено неверное число параметров");
    }

    GSFilter gs;
    gs.ApplyFilter(bmp, {});

    const std::vector<std::vector<int>> matrix = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
    std::vector<std::vector<Pixel>> new_colors(bmp.image_header.height,
                                               std::vector<Pixel>(bmp.image_header.width, Pixel{0, 0, 0}));
    ApplyMatrix(bmp, new_colors, matrix);

    const double threshold = std::stod(params[0]);
    for (int i = 0; i < bmp.image_header.height; ++i) {
        for (int j = 0; j < bmp.image_header.width; ++j) {
            if (new_colors[i][j].r > threshold) {
                bmp.bits[i][j] = Pixel{1, 1, 1};
            } else {
                bmp.bits[i][j] = Pixel{0, 0, 0};
            }
        }
    }
}

void BlurFilter::ApplyFilter(BMP& bmp, const std::vector<std::string>& params) {
    if (params.size() != 1) {
        throw InvalidParams("Введено неверное число параметров");
    }

    double sigma = 0;
    if (!params.empty()) {
        sigma = std::stod(params[0]);
    }

    const int size_of_gauss_mat = 3;
    const int sz = static_cast<int>(size_of_gauss_mat * sigma);

    std::vector<std::vector<Pixel>> new_colors(bmp.image_header.height,
                                               std::vector<Pixel>(bmp.image_header.width, Pixel{0, 0, 0}));
    const double square_of_sigma = std::pow(sigma, 2);
    const double sigma_const = 1.0f / sqrt(2.0f * M_PI * square_of_sigma);

    std::vector<double> pre_gauss_function(2 * sz + 1);
    for (int i = -sz; i <= sz; ++i) {
        pre_gauss_function[i + sz] = sigma_const * std::pow(M_E, -pow(i, 2) / (2 * square_of_sigma));
    }

    for (int i = 0; i < bmp.image_header.height; ++i) {
        for (int j = 0; j < bmp.image_header.width; ++j) {
            for (int k = -sz; k <= sz; ++k) {
                int mn = std::min(bmp.image_header.width - 1, std::max(j + k, 0));

                new_colors[i][j] += pre_gauss_function[k + sz] * bmp.bits[i][mn];
            }
        }
    }

    for (int i = 0; i < bmp.image_header.width; ++i) {
        for (int j = 0; j < bmp.image_header.height; ++j) {
            bmp.bits[j][i] = Pixel{0, 0, 0};

            for (int k = -sz; k <= sz; ++k) {
                int mn = std::min(bmp.image_header.height - 1, std::max(j + k, 0));

                bmp.bits[j][i] += pre_gauss_function[k + sz] * new_colors[mn][i];
            }
        }
    }
}

void PixellateFilter::ApplyFilter(BMP& bmp, const std::vector<std::string>& params) {
    if (params.size() != 1) {
        throw InvalidParams("Введено неверное число параметров");
    }

    int pixels = std::stoi(params[0]);
    int new_height = bmp.image_header.height;
    int new_width = bmp.image_header.width;

    if (bmp.image_header.height % pixels != 0) {
        new_height = (bmp.image_header.height / pixels + 1) * pixels;
    }

    if (bmp.image_header.width % pixels != 0) {
        new_width = (bmp.image_header.width / pixels + 1) * pixels;
    }

    std::vector<std::vector<Pixel>> new_colors(new_height, std::vector<Pixel>(new_width));
    const auto count_in_square = std::pow(pixels, 2);

    for (int y = 0; y < new_height; y += pixels) {
        for (int x = 0; x < new_width; x += pixels) {
            Pixel current_value{0, 0, 0};

            for (int k = 0; k < pixels; ++k) {
                for (int l = 0; l < pixels; ++l) {
                    current_value += FindNearest(bmp, y + k, x + l);
                }
            }

            current_value.r = current_value.r / count_in_square;
            current_value.g = current_value.g / count_in_square;
            current_value.b = current_value.b / count_in_square;

            for (int k = 0; k < pixels; ++k) {
                for (int l = 0; l < pixels; ++l) {
                    new_colors[y + k][x + l] = current_value;
                }
            }
        }
    }

    bmp.bits.assign(new_height, std::vector<Pixel>(new_width));
    bmp.image_header.height = new_height;
    bmp.image_header.width = new_width;

    for (int y = 0; y < new_height; ++y) {
        for (int x = 0; x < new_width; ++x) {
            bmp.bits[y][x] = new_colors[y][x];
        }
    }
}

SourceFilters::~SourceFilters() {
    for (auto& [_, removed_filter] : filter) {
        delete removed_filter;
    }
}

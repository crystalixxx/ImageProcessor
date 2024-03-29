#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include "../bmp_format/bmp_format.h"

#pragma once
Pixel FindNearest(const BMP& bmp, int x, int y);
void ApplyMatrix(const BMP& bmp, std::vector<std::vector<Pixel>>& bits, const std::vector<std::vector<int>>& matrix);

class Filter {
public:
    virtual void ApplyFilter(BMP& bmp, const std::vector<std::string>& params){};

    virtual ~Filter(){};
};

class CropFilter : public Filter {
public:
    void ApplyFilter(BMP& bmp, const std::vector<std::string>& params) override;
};

class GSFilter : public Filter {
public:
    void ApplyFilter(BMP& bmp, const std::vector<std::string>& params) override;
};

class NegativeFilter : public Filter {
public:
    void ApplyFilter(BMP& bmp, const std::vector<std::string>& params) override;
};

class SharpeningFilter : public Filter {
public:
    void ApplyFilter(BMP& bmp, const std::vector<std::string>& params) override;
};

class EdgeFilter : public Filter {
public:
    void ApplyFilter(BMP& bmp, const std::vector<std::string>& params) override;
};

class BlurFilter : public Filter {
public:
    void ApplyFilter(BMP& bmp, const std::vector<std::string>& params) override;
};

class PixellateFilter : public Filter {
public:
    void ApplyFilter(BMP& bmp, const std::vector<std::string>& params) override;
};

class SourceFilters {
public:
    std::unordered_map<std::string, Filter*> filter;

    SourceFilters();

    ~SourceFilters();
};

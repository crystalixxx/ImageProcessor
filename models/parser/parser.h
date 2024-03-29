#include <string>
#include <vector>
#include <iostream>

#include "../exceptions/exceptions.h"
#include "../filters/filters.h"

#pragma once

class Parser {
public:
    Parser(int argc, const char* argv[]);

    void CheckValidInput(SourceFilters& source_filters);

    std::string& GetOutputPath();

    std::string& GetInputPath();

    std::vector<std::pair<std::string, std::vector<std::string>>> GetParams();

private:
    std::string input_path_;
    std::string output_path_;

    std::vector<std::pair<std::string, std::vector<std::string>>> params_;
};

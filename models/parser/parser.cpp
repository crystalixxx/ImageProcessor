#include <string>
#include <vector>
#include <iostream>

#include "../exceptions/exceptions.h"
#include "../filters/filters.h"
#include "parser.h"

Parser::Parser(int argc, const char* argv[]) {
    if (argc < 3) {
        std::cout << "Формат ввода: {имя программы} {путь к входному файлу} {путь к выходному файлу} [-{имя фильтра 1} "
                     "[параметр фильтра 1] [параметр фильтра 2] ...] [-{имя фильтра 2} [параметр фильтра 1] "
                     "[параметр фильтра 2] ...] ...\n";
        throw InvalidInput("Произошел неверный ввод.");
    }

    input_path_ = argv[1];
    output_path_ = argv[2];

    for (int i = 3; i < argc; ++i) {
        if (argv[i][0] == '-') {
            params_.push_back({argv[i], std::vector<std::string>()});
        } else {
            params_.back().second.push_back(argv[i]);
        }
    }
}

void Parser::CheckValidInput(SourceFilters& source_filters) {
    for (const auto& [filter_name, _] : params_) {
        if (source_filters.filter.find(filter_name) == source_filters.filter.end()) {
            throw InvalidParams("Параметр с именем " + filter_name + " не обнаружен.");
        }
    }
}

std::string& Parser::GetOutputPath() {
    return output_path_;
}

std::string& Parser::GetInputPath() {
    return input_path_;
}

std::vector<std::pair<std::string, std::vector<std::string>>> Parser::GetParams() {
    return params_;
}

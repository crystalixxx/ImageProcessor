#include "models/filters/filters.h"
#include "models/parser/parser.h"

int main(int argc, const char* argv[]) {
    SourceFilters all_filters{};

    Parser parser(argc, argv);
    parser.CheckValidInput(all_filters);

    BMP image(parser.GetInputPath());

    for (const auto& [applied_filter, params] : parser.GetParams()) {
        if (all_filters.filter.find(applied_filter) != all_filters.filter.end()) {
            all_filters.filter[applied_filter]->ApplyFilter(image, params);
        }
    }

    image.MakeOutputBMP(parser.GetOutputPath());

    return 0;
}

#include <catch.hpp>
#include <exception>
#include "../models/parser/parser.h"

TEST_CASE("parse_test") {
    {
        std::string ex = "./image_processor";
        std::string input = "test_script/data/lenna.bmp";
        std::string output_file = "test.bmp";
        const char* test[] = {ex.c_str(), input.c_str(), output_file.c_str()};

        try {
            const int number_of_input = 3;
            Parser parser(number_of_input, test);

            REQUIRE(parser.GetOutputPath() == output_file);
            REQUIRE(parser.GetInputPath() == input);
        } catch (const std::exception& e) {
            REQUIRE(false);
        }
    }

    {
        std::string ex = "./image_processor";
        std::string input = "test_script/data/lenna.bmp";
        std::string output_file = "test.bmp";
        std::string filter = "-crop";
        std::string param1 = "999";
        std::string param2 = "1999";

        const char* test[] = {ex.c_str(),     input.c_str(),  output_file.c_str(),
                              filter.c_str(), param1.c_str(), param2.c_str()};
        try {
            const int number_of_input = 6;
            Parser parser(number_of_input, test);

            std::vector<std::pair<std::string, std::vector<std::string>>> params = {{"-crop", {"999", "1999"}}};

            REQUIRE(parser.GetOutputPath() == output_file);
            REQUIRE(parser.GetInputPath() == input);
            REQUIRE(parser.GetParams() == params);
        } catch (const std::exception& e) {
            REQUIRE(false);
        }
    }
}

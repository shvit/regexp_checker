#include <cstdlib>
#include <iostream>

#include "checker.hpp"
#include "base.hpp"

constexpr std::string_view test_rule_dir = "test/regexp";
constexpr std::string_view test_data_dir = "test/data";

int main(int argc, char* argv[])
{
    Base b;
    do {
        //std::cout << "Preparing ..." << std::endl;
        if (!b.prepare(argc > 1 ? argv[1] : test_rule_dir,
                       argc > 2 ? argv[2] : test_data_dir)) {
            break;
        }

        //std::cout << "Run ..." << std::endl;
        b.run();
    } while(false);

    b.push_errors(std::cerr);

    if (b.has_errors()) {
        return EXIT_FAILURE;
    } else {
        std::cout << b.get_metric() << std::endl;
        return EXIT_SUCCESS;
    }
}
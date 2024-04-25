#include <cstdlib>
#include <iostream>
#include <string>
#include <filesystem>

#include "routine.hpp"

namespace fs = std::filesystem;

const fs::path test_rule_dir = fs::path("test") / fs::path("regexp");
const fs::path test_data_dir = fs::path("test") / fs::path("data");

int main(int argc, char* argv[])
{
    Routine b;
    do {
        if (!b.prepare(argc > 1 ? argv[1] : test_rule_dir,
                       argc > 2 ? argv[2] : test_data_dir)) {
            break;
        }

        std::cout << std::endl;
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

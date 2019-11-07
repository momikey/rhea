#include <iostream>
#include <string>

#include "grammar/operators.hpp"
#include "debug/parse_tree.hpp"

int main()
{
    std::string input;

    while (std::getline(std::cin, input))
    {
        rhea::debug::print_parse_tree(std::cout, input);
    }

    return 0;
}

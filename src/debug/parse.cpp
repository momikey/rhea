#include <iostream>
#include <string>

#include "grammar/expression.hpp"
#include "debug/parse_tree.hpp"

int main()
{
    std::string input;

    while (std::getline(std::cin, input))
    {
        auto in = rhea::debug::input_from_string(input);
        auto tree = rhea::debug::parse<rhea::ast::parser_node>(*in);
        rhea::debug::print_partial_tree(std::cout, tree.get());
    }

    return 0;
}

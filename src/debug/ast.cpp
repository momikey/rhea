#include <iostream>
#include <string>

#include "grammar/expression.hpp"
#include "debug/parse_tree.hpp"
#include "debug/build_ast.hpp"

int main()
{
    std::string input;

    while (std::getline(std::cin, input))
    {
        auto in = rhea::debug::input_from_string(input);
        auto tree = rhea::debug::parse<rhea::ast::parser_node>(*in);
        
        if (tree)
        {
            auto ast = rhea::debug::build_ast(tree.get());
            rhea::debug::dump_ast(std::cout, ast.get());
            std::cout << '\n';
        }
        else
        {
            std::cout << "Parse error\n";
        }
    }

    return 0;
}

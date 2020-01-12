#include <iostream>
#include <string>

#include "grammar/expression.hpp"
#include "debug/parse_tree.hpp"
#include "debug/build_ast.hpp"
#include "debug/build_ir.hpp"

int main()
{
    std::cout << "Rhea IR debug tool\n";
    
    std::string input;

    while (std::getline(std::cin, input))
    {
        auto in = rhea::debug::input_from_string(input);
        auto tree = rhea::debug::parse<rhea::ast::parser_node>(*in);
        
        if (tree)
        {
            auto ast = rhea::debug::build_ast(tree.get());
            std::cout << ast->to_string() << '\n';
            rhea::debug::print_ir(ast.get());
        }
        else
        {
            std::cout << "Parse error\n";
        }
    }

    return 0;
}

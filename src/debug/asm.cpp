#include <iostream>
#include <string>

#include "grammar/expression.hpp"
#include "debug/parse_tree.hpp"
#include "debug/build_ast.hpp"
#include "debug/build_asm.hpp"

int main()
{
    std::cout << "Rhea ASM output debug tool\n";
    
    std::string input;

    while (std::getline(std::cin, input))
    {
        auto in = rhea::debug::input_from_string(input);
        auto tree = rhea::debug::parse<rhea::ast::parser_node>(*in);
        
        if (tree)
        {
            auto ast = rhea::debug::build_ast(tree.get());
            std::cout << ast->to_string() << '\n';
            rhea::debug::print_asm(ast.get());
        }
        else
        {
            std::cout << "Parse error\n";
        }
    }

    return 0;
}

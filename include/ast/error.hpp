#ifndef RHEA_AST_ERROR_HPP
#define RHEA_AST_ERROR_HPP

#include <stdexcept>
#include <fmt/format.h>

namespace rhea { namespace ast {
    // Exception type for unimplemented AST nodes
    struct unimplemented_type : public std::invalid_argument
    {
        unimplemented_type(std::string type) : std::invalid_argument(type) {}
    };

    // Base exception type for syntax errors
    struct syntax_error : public std::runtime_error
    {
        syntax_error(std::string msg) : std::runtime_error(msg) {}
    };

    // Exception type for type mismatch errors
    struct type_mismatch : public syntax_error
    {
        template <typename T>
        type_mismatch(T* t) : info(
            fmt::format("{0}:{1}:{2}({3}) Type mismatch",
                t->position.source,
                t->position.line,
                t->position.byte_in_line,
                t->position.byte
            )
        ) {}

        const std::string info;
    };
}}

#endif /* RHEA_AST_ERROR_HPP */
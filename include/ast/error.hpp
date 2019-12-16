#ifndef RHEA_AST_ERROR_HPP
#define RHEA_AST_ERROR_HPP

#include <stdexcept>

namespace rhea { namespace ast {
    // Exception type for unimplemented AST nodes
    struct unimplemented_type : public std::invalid_argument
    {
        unimplemented_type(std::string type) : invalid_argument(type) {}
    };
}}

#endif /* RHEA_AST_ERROR_HPP */
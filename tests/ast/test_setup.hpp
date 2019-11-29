#ifndef RHEA_AST_TEST_SETUP_HPP
#define RHEA_AST_TEST_SETUP_HPP

#include <iostream>

#include "../../include/ast/nodes.hpp"

// Boost.Test needs a printer for user-defined types, and it
// must be defined in the same namespace as the type.
namespace rhea { namespace ast {
    std::ostream& boost_test_print_type(std::ostream& ostr, rhea::ast::Numeric const& right)
    {
        ostr << static_cast<int>(right);
        return ostr;

    }
    
    template <typename T>
    std::ostream& boost_test_print_type(std::ostream& ostr, T const& right)
    {
        ostr << right.to_string();
        return ostr;
    }

}}

#endif /* RHEA_AST_TEST_SETUP_HPP */
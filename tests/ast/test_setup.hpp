#ifndef RHEA_AST_TEST_SETUP_HPP
#define RHEA_AST_TEST_SETUP_HPP

#include <iostream>

#include "../../include/ast/nodes.hpp"
#include "../../include/types/types.hpp"

// Boost.Test needs a printer for user-defined types, and it
// must be defined in the same namespace as the type.
namespace rhea { namespace ast {
    std::ostream& boost_test_print_type(std::ostream& ostr, rhea::types::BasicType const& right);
    
    template <typename T>
    std::ostream& boost_test_print_type(std::ostream& ostr, T const& right);

}}

namespace std {
    inline std::ostream& operator<<(std::ostream& os, rhea::types::BasicType rhs)
    { return os << static_cast<int>(rhs); }
}

#endif /* RHEA_AST_TEST_SETUP_HPP */
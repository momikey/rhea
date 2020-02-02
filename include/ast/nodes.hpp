#ifndef RHEA_AST_NODES_HPP
#define RHEA_AST_NODES_HPP

#include <memory>
#include <type_traits>

// Include all the "nodes/" headers.
#include "nodes/node_base.hpp"

#include "nodes/binary_operator.hpp"
#include "nodes/control_statement.hpp"
#include "nodes/exception.hpp"
#include "nodes/function.hpp"
#include "nodes/identifiers.hpp"
#include "nodes/literals.hpp"
#include "nodes/module.hpp"
#include "nodes/simple_statement.hpp"
#include "nodes/structure_type.hpp"
#include "nodes/ternary_operator.hpp"
#include "nodes/typenames.hpp"
#include "nodes/unary_operator.hpp"

namespace rhea { namespace ast {
    /*
     * Define a general node factory function. This does *not* depend
     * on the parser's output tree. Instead, all objects are created
     * from passed arguments. Thus, we can manually create them if,
     * for some reason, we want to do that.
     */
    template <typename NodeType, typename... Args>
    std::unique_ptr<ASTNode> create_node(Args... args)
    {
        static_assert(std::is_convertible<NodeType*, ASTNode*>::value,
            "Invalid AST node class");
        return std::make_unique<NodeType>(args...);
    }
}}

#endif /* RHEA_AST_NODES_HPP */
#ifndef RHEA_AST_TRANSFORM_HPP
#define RHEA_AST_TRANSFORM_HPP

#include <memory>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

#include "../grammar.hpp"

    /*
     * AST transformers: these take the parse tree and modify it
     * to become an AST.
     * 
     * Each transformer is a struct that overrides the transform()
     * method, as explained in the PEGTL docs. We derive from the
     * parse_tree::apply class using CRTP to allow bulk selection,
     * instead of having to make a new transformer for each rule
     * in the grammar.
     */

#include "transform/binary_operator.hpp"
#include "transform/discard_subtree.hpp"
#include "transform/else_block.hpp"
#include "transform/postfix_operator.hpp"
#include "transform/ternary_operator.hpp"
#include "transform/typecheck_operator.hpp"
#include "transform/unary_operator.hpp"

#endif /* RHEA_AST_TRANSFORM_HPP */
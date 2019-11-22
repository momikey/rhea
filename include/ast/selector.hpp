#ifndef RHEA_GRAMMAR_SELECTOR_HPP
#define RHEA_GRAMMAR_SELECTOR_HPP

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

#include "tokens.hpp"
#include "strings.hpp"
#include "expression.hpp"
#include "typenames.hpp"
#include "concepts.hpp"
#include "statement.hpp"
#include "module.hpp"

/*
 * PEGTL provides an automatic parse tree builder. It can also be used
 * to create an AST skeleton by adding a "selector": a templated struct
 * that tells the tree generator how to transform specific nodes.
 * 
 * For Rhea, we mostly want two types of nodes. First, concrete things
 * like integer or string literals. Second, abstract nodes representing
 * expressions, statements, etc.
 */

namespace rhea { namespace ast {
    using namespace tao::pegtl;
    using namesace rhea::grammar;

    /*
     * The base selector. For anything that PEGTL has built in, we can
     * use this. All our custom transformations will instead be specializations
     * of this template.
     */
    template <typename Rule>
    struct tree_selector : parse_tree::selector < Rule, 
        /*
         * parse_tree::store_content::on stores the matched content
         * into its node. This is the best choice for terminals that
         * need no further processing, apart from, e.g., conversion
         * from strings.
         */
        parse_tree::store_content::on <
            integer_literal,
            hex_literal,
            float_literal,
            boolean_literal,
            symbol_name,
            identifier
            // and other rules that produce simple tokens
        >,

        /*
         * parse_tree::remove_content::on removes content (obviosuly);
         * we can use it for "do nothing" nodes that still get stored
         * for whatever reason.
         */

        /*
         * parse_tree::fold_one::on removes its own content if it has multiple
         * children, or replaces itself with an only child. That's useful
         * for a lot of group/list/alternation things.
         */
        parse_tree::fold_one::on <
            numeric_literal,
            fully_qualified
        >

        /*
         * parse_tree::discard_empty:on stores the node if it has children,
         * discarding any of its own content; if no children, then it removes
         * the node entirely. This might come in handy in a few cases.
         */
    > {};
}}

#endif /* RHEA_GRAMMAR_SELECTOR_HPP */
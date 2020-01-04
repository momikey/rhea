#ifndef RHEA_VISITOR_FWD_HPP
#define RHEA_VISITOR_FWD_HPP

/*
 * Forward declaration for the double-dispatch visitor base class.
 * This class is declared in base_visitor.hpp, but the AST headers
 * should import this file instead, to break the circular definition
 * that would otherwise occur.
 */

namespace rhea { namespace visitor {
    struct Visitor;
}}

#endif /* RHEA_VISITOR_FWD_HPP */
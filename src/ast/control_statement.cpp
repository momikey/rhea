#include "ast/nodes/control_statement.hpp"

#include <algorithm>
#include <iterator>

namespace rhea { namespace ast {
    With::With(child_vector<PredicateCall>& ps, statement_ptr b)
        : body(std::move(b))
    {
        // This is the same logic as in a few other nodes,
        // but with the added wrinkle of the body init above.
        std::move(ps.begin(), ps.end(), std::back_inserter(predicates));
    }

    std::string With::to_string()
    {
        return fmt::format("(With{0},{1})",
            util::serialize_array(predicates),
            body->to_string());
    }

    PredicateCall::PredicateCall(expression_ptr t, child_vector<Expression>& as)
        : target(std::move(t))
    {
        // This is the same logic as in a few other nodes,
        // but with the added wrinkle of the body init above.
        std::move(as.begin(), as.end(), std::back_inserter(arguments));
    }

    std::string PredicateCall::to_string()
    {
        return fmt::format("(PredicateCall,{0}{1})",
            target->to_string(),
            util::serialize_array(arguments));
    }

    Match::Match(expression_ptr e, child_vector<Case>& cs)
        : expression(std::move(e))
    {
        // This is the same logic as in other "vector" nodes.
        std::move(cs.begin(), cs.end(), std::back_inserter(cases));
    }

    std::string Match::to_string()
    {
        // The usual vector printing, plus the matching expression.
        std::string s;
        s.reserve(cases.size()*16);    // profile this to find a nice default

        for (auto&& id : cases)
        {
            s += ',';
            s += id->to_string();
        }

        return fmt::format("(Match,{0}{1})",
            expression->to_string(),
            util::serialize_array(cases));
    }
}}
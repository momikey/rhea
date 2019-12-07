#include "ast/nodes/function.hpp"

#include <algorithm>
#include <iterator>

namespace rhea { namespace ast {
    // Function call nodes can take two different types of argument list.
    // The constructor logic is the same for both, as Boost and standard
    // variants have implicit constructors.

    Call::Call(expression_ptr t, child_vector<Expression>& args)
        : target(std::move(t))
    {
        std::move(args.begin(), args.end(), std::back_inserter(arguments));
    }

    Call::Call(expression_ptr t, child_vector<NamedArgument>& args)
        : target(std::move(t))
    {
        std::move(args.begin(), args.end(), std::back_inserter(arguments));
    }

    std::string Call::to_string()
    {
        // This is mostly the same logic, except that we have to worry
        // about visiting the variant.

        std::string s;
        s.reserve(arguments.size()*16);    // profile this to find a nice default

        for (auto&& id : arguments)
        {
            s += ',';
            s += visit([](auto const& v) {
                 return v->to_string(); }, id);
        }

        return fmt::format("(Call,{0}{1})", target->to_string(), s);
    }

    Arguments::Arguments(child_vector<TypePair>& args)
    {
        std::move(args.begin(), args.end(), std::back_inserter(arguments));
    }

    std::string Arguments::to_string()
    {
        // TODO: We'll need to add the visitor logic when we switch to variants.
        // See above for how to do that.
        std::string s;
        s.reserve(arguments.size()*16);    // profile this to find a nice default

        for (auto&& id : arguments)
        {
            s += ',';
            s += id->to_string();
        }

        return fmt::format("(Arguments{0})", s);  
    }
}}
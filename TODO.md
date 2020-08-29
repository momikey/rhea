# Parser

* Detect reserved words used as identifiers, so we can error out early.

* Maybe implement a (marked unsafe) reinterpreting cast for pointers <-> integers?
* Implement better error checking and recovery. (Long-term)

# AST

* Builders for variants, optionals, type aliases, exception handling.

* Delegating constructors for Function AST node.
* Statement AST class mentions doing a declaration pass. Will the type inference pass work for this, too?
* Expression type for member and subscript operations.
* Expression type for unary operator nodes.

# Type engine

I haven't looked at the type engine in a while, so I don't remember how much progress I've made. I'll fill this in later.

* Full support for all types Rhea allows.
* Finish implementing name mangling.

# Codegen

Same as above. I'll fill it in later.

* User-controllable levels of optimization.
* Type conversions should call a `to$` operator function if implemented.
* Size constants instead of magic numbers.
* Handle casting operations and all types.
* Reference support: dereferencing should be automatic.

# Compiler

Pretty much everything, as I haven't even started on making a usable compiler yet.

# Runtime

This is still in the planning stages.

# Documentation

I need to write a comprehensive language reference one of these days. Until then, fleshing out the "syntax notes" doc should be a focus.

## Notes

These are notes I've marked as "TBD":

* Anything else to do with symbols, enums, structure types, `any`, references, pointers, preconditions, return values, FFI/unsafe functions, predicates, type functions, concepts, name mangling.
* Casting between signed and unsigned.
* Show all basic types in examples.
* Destructuring and multiple cases in `on` clauses.
* Do we want a `for` expression, like JS `map` or Python list comprehensions?
* Examples for `break` and `continue`.
* Enforce array elements all having the same type.
* Document possible operations on tuples.
* List of overloadable operators and their corresponding magic function names.
* Overloading functions, argument deduction, overload resolution.
* List of predefined concepts.
* Examples for member match operator `.=`.
* Should we allow catching by concept?
* Configuration or manifest system to search for modules.
* We may need to require externs to be defined with arguments; LLVM needs this, too.

## Possible later features

* Subtypes
* Extensible variants
* Macros

# Conventions

Comments begin with `#` and continue until the end of a line. Multiline comments can be demarcated with `#{` and `#}`.

# Mathematical expressions

## Arithmetic

```
	1 + 2		# = 3
	42 - 13		# = 29
	6 * 9		# = 54
	111 / 3		# = 37
	123 % 4		# = 3
	2 ** 16		# = 65536
	+1			# = 1 (unary + only included for completeness)
	-1 * -1		# = 1 
```

* Operands must be of the same type (integer, floating-point, etc.), and the result will also be of that type. In strict mode, violating this rule causes an error; otherwise, the compiler emits a warning.
* The unary `^` operator can be used to change integers to floating-point in arithmetic contexts:

```
	2.5 + 1.5	# = 4.0 (double + double = double)
	3.0 * 2.5	# = 7.5
	9.0 / 3		# = 3.0 + warning (error in strict mode)
	9.0 / 3.	# = 3.0 (okay, because "3." is a double)
	9.0 / ^3	# = 3.0 (okay, because we're explicit)
```

Arithmetic operations are only valid on numerical types and those for which the appropriate operator functions are defined.

## Bitwise

Hex integers are defined with leading `0x`.

```
	0x55 & 0xf	# = 0x5
	0x20 | 1	# = 0x21
	0xf0 ^ 0x0f	# = 0xFF
	1 << 3		# = 8
	8 >> 3		# = 1
	~0x1		# = 0xFFFFFFFE (integers are 32-bit by default)
```

Bitwise operations are only valid on integers and those types with appropriate operator functions.

## Comparison and Boolean

All comparisons give a boolean result: `true` or `false`.

```
	2 > 1				# = true
	-1 < 0				# = true
	99 >= 100			# = false
	-0.0 <= 0.0			# = true
	2**2 == 4			# = true
	(-3)**3 != -(3**3)	# = true
```

You can also use the boolean operators with these values.

```
	true and false				# = false
	(1 < 100) or (-1 > -100)	# = true
	not true					# = false
```

You can't do that with other values without casting, though:

```
	not 123				# error: incorrect type
	not 123 as boolean	# = false
```

# Variables

All Rhea variables have a type, and they must be declared before they are used.

## Identifiers

Identifiers are strings containing alphanumeric characters and the special character `_`. The characters `?`, `$`, and `!` have special meaning, as discussed below.

An identifier can't be any reserved word, such as `true`, `not`, `def`, or `if`

## Simple types

Rhea has the following simple types for variables:

* `integer`: a 32-bit signed integer
* `byte`: an 8-bit signed integer
* `long`: a 64-bit signed integer
* `double`: a 64-bit floating-point number
* `float`: a 32-bit floating-point number
* `boolean`: a boolean value (true or false)
* `symbol`: an atomic value (internally represented as an integer)

The types `uinteger`, `ubyte`, and `ulong` can be used for unsigned integers of the given precision, but you can't mix signed and unsigned. {TBD: Casting between signed/unsigned.}

Simple types can be initialized with literals; `integer` and `double` are the default types. Others can be specified by type-casts or literal suffixes: `_u` for unsigned integer, `_f` for float, `_b` or `_ub` for signed/unsigned byte, `_l` or `_ul` for signed/unsigned long.

Symbols are prefixed with an `@`, and can be used as dictionary keys or enumeration members. {TBD: More about symbols.}

## Declaration

Variables can be declared with the `var` keyword, and initialized with any expression.

```
	var foo as integer;		# uninitialized
	var bar = 42;			# initialized, "as integer" is inferred
	var baz = bar as byte;	# initialized with explicit type
```

Type inference covers most initialization cases; explicit types are only required when the desired type is different from the default, or if the variable is not initialized immediately.

Uninitialized variables cannot be used until assignment, and such uses will produce a warning (error in strict mode).

```
	var wrong = foo;		# "foo" hasn't been initialized, so it can't be used yet
```

{TBD: Show all basic types}

## Constants

Constant values are declared in the same way as variables, except that they use the `const` keyword, and they must be initialized at the point of declaration.

```
	const Max_items = 8;		# okay, "Max_items" is an integer
	const Min_items;			# error: uninitialized constant
	const Min_items as integer;	# same error as above
```

## Assignment

Assignment is a statement, not an expression, so it does not evaluate to a value. Thus, it can't be used as a condition.

```
	var my_int as integer;

	# ...other code...

	my_int = 42;
```

Obviously, constants can't be assigned after they're declared.

```
	const Con = -1234;

	Con = 4321;			# error: reassignment to constant
```

Rhea also allows compound assignment, with operators such as `+=`.

```
	foo += 1;
```

# Control flow

## If-else

The `if` statement is used as in most languages, but the condition must be a boolean expression.

```
	if (foo > 42)
	{
		print("More");
	}
	else
	{
		print("Less");
	}
```

Rhea also offers an `if` expression, which has slightly different properties:

* It cannot be nested (no `else if`)
* Both branches are limited to a single expression (no assignments or blocks)
* Both branches must evaluate to the same type
* The condition must be followed by `then`

```
	var next_value = if (index > size) then index else size;
```

## Unless (reverse if)

Rhea has an "opposite" version of `if` in `unless`. This statement, however, is more limited. It can't take an `else` clause (this would be a double negative, which is confusing and error-prone), and there is no expression form.

```
	unless index >= size
	{
		next_value = my_array[index];
	}
```

Unless statements should be rare. They're only intended for cases where the inversion of logic would otherwise be a barrier to understanding.

## Match-on (case match)

Instead of chaining `if` statements, Rhea provides a pattern-matching construct `match`.

```
	var value = random(3)	# assume this function exists and provides a random whole number < 3

	match value
	{
		on 0: { print("Nothing"); }
		on 1: { print("Just one"); }
		on 2: { print("You get both!"); }
		default: { print("Somebody's cheating"); }
	}
```

Every `on` case must be of the same type, which must match that of the tested variable. There is no fall-through, so break statements are not necessary.

{TBD: Multiple cases per `on`, destructuring.}

## Match-when (boolean match)

An alternative to `match-on` is `match-when`, which allows a different style of matching.

```
	match foo
	{
		when nonzero?: { print("Not zero"); }
		when zero?: { print("Zero"); }
	}
```

`When` clauses use predicates (see below). Each predicate is tested in order, passing the match value. Thus, the example above calls `nonzero(foo)?`, then `zero(foo)?`.

The first predicate to evaluate to true has its statement or block executed. As with `match-on`, there is no fallthrough. If a `default` case is present, it will be executed if all `when` cases fail.

You can't mix `on` and `when` in the same `match` statement.

## For loop (iteration)

Rhea has a `for` loop, but it is intended only for iteration over collections (arrays, lists, and user-defined types which follow the `Iterable` concept.

```
	for i in [1, 2, 3, 4, 5]
	{
		print(to_string(i));	# pseudocode
	}
```

The induction variable (`i` in the example above) is local to the loop, and acts as a constant.

{TBD: A `for` expression? This would be like `map` in JS or Python comprehensions.}

## While loop

The `while` loop is more general than `for`, but care must be taken. Remember that the condition is a boolean, and assignments are not allowed, unlike in C and C++.

```
	while (score < 100)
	{
		# some function that may increase score
	}
```

Rhea does not have a "do-while" or "until" loop.

The `break` and `continue` statements can be used for finer flow control. {TBD: Example.}

## With (invariant declaration)

The `with` keyword defines a new scope with any number of *invariants*. These are predicates that will be evaluated at the beginning of the scope's execution. If the predicates do not hold, an error will be raised in debug mode, while an exception will be thrown in other modes.

Preconditions can be combined with constructs such as `for` and `while` for natural syntax:

```
	# Sum of reciprocals (a contrived example, but useful)

	var total = 0.0;

	# Before each iteration of the loop, `x` will be checked.
	for x in list_of_numbers with (x.nonzero?)
	{
		total += 1.0 / x;
	}
```

Postconditions are a little different, in that they go inside the loop. Idiomatically, one possible syntax uses an empty statement block, as so:

```
	# Read through a list, acting on each value, but error on a null result.
	for value in list_of_optionals
	{
		var result = do_something_with(value);
		
		with (result.not_nothing?) {}
	}
```

Functions can also take a `with` block, but theirs works in a different manner, as you'll see below.

### Old

The `with` keyword introduces a new scope with any number of temporary variables. These are uninitialized (rather, default-initialized for their type), and are inaccessible outside the scope.

```
	with (n as integer)
	{
		# do stuff with n
	}

	# This is an acceptable alternative, as a single
	# statement can always substitute for a block.
	with (s as string) while (s.length() < 10)
	{
		s.append(getchar());	# pseudocode 
	}
```

## Do

The `do` statement immediately evaluates the given expression.

```
	do some_function();			# a single expression
	do (f(), g(), h());			# multiple expressions can be evaluated at once as a list
```

This can be used, for instance, to invoke side-effects, or to resolve ambiguity, as `do` effectively converts an expression into a statement.

If a function takes no arguments, then `do` allows you to call it without parentheses.

```
	do some_function;		# as above, but call is implied
```

# Complex types

## Arrays

Arrays are fixed-size, and can only contain integers, floating-point numbers, booleans, and references. Each value in a given array must have the same type. {TBD: Explain this, and make it work.}

A variable of an array type must have its size known at declaration time, but this can be a constant value defined in the program.

```
	var ns = [1, 2, 3];			# array of integers
	var bs as byte[16];			# uninitialized array of bytes
```

Arrays are accessed with the index operator `[]`; the index is zero-based.

```
	var foo = ns[1]					# foo now equals 2
	for i in range(bs.length()):	# assumes a range function
	{
		bs[i] = 1 << i;
	}
```

(The `length()` function, when called on an array, returns the total number of elements.)

## Lists

Lists are dynamic in size, though still homogeneous in type. They are created with the `list()` function, which is generic (see below).

```
	var l = list <string> (10);		# reserve 10 elements
	l[3] = "Hi.";					# set an element
	l.append("end");				# add a new element
```

A list can also be initialized with the `()` literal:

```
	var lis = (1, 2, 3, 4, 5);		# list of integers
	var sts = ("abc", "def");		# list of strings
```

All values must have the same type, which will be inferred unless explicitly declared:

```
	var p2 = (1, 2, 4, 8) as list <byte>;	# explicit type: each element will be converted to a byte
```

## Strings

Strings contain UTF-8 characters, and can be initialized using the `""` literal notation.

```
	var s as string;			# initialize as empty string
	s.length() == 0;			# = true
	s = "abcde";				# assignment with literal
	s[0] == "a";				# individual characters are strings, too
```

Rhea strings use the same notation as C for escaped characters such as `\n`.

Single quotes are an acceptable alternative, with no difference in function.

## Dictionaries

Dictionaries map numbers or symbols to values of a specific type. Rhea has special syntax for handling them.

```
	var d1 as dict <string>;		# declare with value type
	var d2 = dict <string> ();		# initialize w/constructor

	# declare and initialize using literal notation
	var d3 = {
		@foo: "Hello",
		@bar: "World"
	};
```

Dictionaries are not sorted, but they can be iterated with a `for` loop.

## Tuples and enums

Tuples are anonymous structures of heterogeneous types. They can be indexed like arrays, but not iterated over. There is no tuple type, however.

```
	var t = {1, "abc", @foo};
	t[0] == 1;						# = true
```

Tuples are immutable; assignment to an element is an error. Operations are provided to combine tuples, but these return a new instance. {TBD: Document operations, possibly including `head` and `tail`?}

Enumerations ("enums") are similar to tuples, but they can only contain symbols, and they define a new type.

```
	type En = @{foo, bar, baz};		# define a new enum type

	var e as En;
	e = @foo;						# this is okay
	e = 42;							# this is an error
```

Note the convenience syntax above: `@{...}` implicitly changes all the identifiers into their corresponding symbols.

If used in a `match-on` statement, the compiler will warn about unused symbols.

```
	# Using the definition of type En above.

	var e as En;

	# ...do something that sets e...
	match e
	{
		on @foo: print("e is foo");
		on @baz: print("e is baz");
	}
```

As we forgot to match against the symbol `bar` in this example, the compiler will issue a warning, which can be upgraded to an error if desired.

{TBD: More about enums.}

## Structures

Structures are complex data types holding a number of fields, which can be of any type, including other structures.

```
	type Person = {
		name: string,
		age: integer
	};
```

After definition, a structure can be used as any other type.

```
	var p as Person;
	p.name = "Michael";		# fields are accessed with '.'
	p.age = 36;
```

In addition to the type, a structure definition also creates a function with the same name, which takes a tuple of named arguments matching the fields.

```
	# Use the Person constructor function
	var b = Person (name: "Andy", age: 31);
```

There are other ways of constructing objects of structure types, as you'll see below.

{TBD: Anything else?}

## Any and nothing

The `any` and `nothing` types are special in Rhea.

A variable defined as type `any` can take any value.

```
	var a as any;
	a = 123;			# assigning an integer
	a = "abc";			# assigning a string
```

However, the compiler can't do most type-checking with `any` variables, so you'll often want to cast the `any` into something more suitable. {TBD: More about `any`.}

The `nothing` type is used as the return type for functions which do not return a value. It has a single value, also called `nothing`, roughly equivalent to `null` in C++ or `None` in Python.

```
	var x = some_function();		# may return nothing
	unless x is nothing
	{
		# do something with x
	}
```

## References

References, strictly speaking, are not themselves a type. Rather, they refer to a value of another type. Most operations implicitly dereference to get the value pointed to.

```
	var n1 = 42;					# an integer
	var r1 = ref n1;				# refernce to that integer

	print(n1 == r1);				# = true (implicit dereferencing)
	print(n1 is integer);			# = false (different types)

	var r2 as ref string;			# a variable declared as a reference
	r2 = "Hi";						# assignment
```

Rhea references are type-safe. In addition, they are checked; a variable declared as a reference cannot be used until it is initialized.

{TBD: More about references, like how to use them in functions.}

### Raw pointers

Although it should be rare, you may need to work directly with pointers. This is considered an unsafe operation, and the compiler can't offer much to help or, in many cases, optimize.

```
	var n = 100;					# a variable
	var r = ref n;					# refernce to the variable
	var p = ptr n;					# pointer to the variable

	print(n == p);					# = false; incompatible types

	p = 200;						# error: can't assign integer to pointer
	*p = 200;						# assign through dereference

	print(p);						# will print the pointer's address
	print(*p);						# will print the value at the pointer's address
```

You can't get a pointer to a `const`, as these are not considered to have a memory address. (It is entirely possible that they don't, although this is platform-specific.) Also, you're not allowed to create an uninitialized pointer.

{TBD: More about using pointers. Do we want to change syntax? Null pointers?}

# Functions

Rhea isn't a functional programming language, but it does place a heavy emphasis on functions.

## Definition

The `def` keyword creates a new function definition.

```
	def my_function =
	{
		return "Hello, World!";
	}
```

Types are as important for functions as they are for variables, but Rhea can infer them in most cases. For `my_function` above, the return type is a string, because that's what the `return` statement produces. We'll see more specific cases below.

The body of a function can also be a single statement, in which case the braces are not necessary. This allows for a more concise definition in some cases.

```
	def ping = return "Ping!";
```

Calling a function is simple, and should be familiar to those coming to Rhea from other languages such as C.

```
	do ping();				# prints "Ping!"

	# pseudocode
	if (n < 10)
	{
		print("0", n);
	}
	else
	{
		print(n);
	}
```

## Arguments

Functions can take any number of arguments, and these become local variables. Arguments are declared in a method similar to structure types.

```
	def power { x: integer, y: integer } =
	{
		return x ** y;
	}
```

Rhea does no type coercion or casting, and the number of arguments must match.

### Preconditions

One of Rhea's key functionalities is its contract system. This is a set of compile-time and run-time checks of function arguments intended to increase security and prevent common mistakes.

Let's use preconditions to better control the `power` function above.

```
	def power { x: integer, y: integer }
	with {
		y.not_negative?,
		y.less_than(32)?
	} = {
		return x ** y;
	}
```

{TBD: More preconditions, how to use them, etc.}

## Return value

Return types can be inferred in most cases. Where they can't, however, you can explicitly specify them.

```
	def lpower [long] { x: integer, y: integer } =
	{
		# note: we could also say "^x ** y" here
		return (x as long) ** y;
	}
```

{TBD: More on return values, and when Rhea can't infer them.}

### Postconditions

Just as arguments to a function can have preconditions, its return value can have postconditions. These are considered part of the contract of the function, and they are checked in strict mode (or, where possible, at compile-time).

```
	def make_foo [Foo]
	with {
		return.is_not_null?
	} =	{
		# Create an object of Foo type
	}
```

(Since `return` is a reserved word in Rhea, there is no potential conflict with function arguments.)

Postconditions can use all the same predicates as preconditions.

{TBD: Whatever else we need.}

## Unchecked functions

Sometimes, the strict type-checking of Rhea can get in the way. When that happens, you can use unchecked functions. These disable all run-time checking; in exchange, they can't have preconditions or postconditions. In addition, they cannot be overloaded based on the types of their arguments or their return values.

```
	def my_unsafe_func! =
	{
		# Do whatever you want here. Just don't expect
		# Rhea to tell you if you mess up. XD
	}

	do my_unsafe_func!;
```

The exclamation mark *is* part of the function's name, so you have to use it in calls to the function.

{TBD: Lots more about this.}

# Generic programming

## Overloading functions

{TBD: Rules on which arguments are considered overloading, how to determine with coerced types, and much, much more.}

## Constructor and destructor

Every type definition creates a constructor function with the same name as the type, but Rhea also provides a generic constructor `new$` that can be overloaded for a specific user-defined type.

```
	# Define a constructor that creates a complex number
	# from polar coordinates.

	use std:math;

	def new$ [Complex] { r: double, phi: double }
	with {
		r.at_least?(0.0),
		phi.between?(0.0, 2.0*math.PI)
	} = {
		return Complex(r * math:cos(phi), r * math:sin(phi));
	}

	# Now we can use it.
	var c = new [Complex] (1.0, math:PI);
	# ...
```

## Methods

Although Rhea is not an object-oriented language, and structures do not have methods, these can be simulated using a common syntax.

```
	var n = 42;
	var s = n.to_string();		# this calls to_string(n)
```

The "object" is prepended to the argument list, and the compiler calls the appropriate global or module-level function.

If the object is of a type imported from a module (see "Modules" below), then using the method syntax will enable better overload resolution.

{TBD: More info about this.}

## Variadic functions

The simplest way to define a function that accepts a variable number of arguments (a variadic function) is to define it with a single list parameter.

```
	def my_func { args: list <string> } =
	{
		for arg in args
		{
			print(arg);
		}
	}
```

In some cases, this isn't possible. For instance, you may want a function that takes multiple types. Since there is no specific tuple type in Rhea, this can be difficult. The language offers a "wildcard" overload in this case.

```
	def my_other_func { args: * } =
	{
		for arg in args
		{
			print(arg as string);
		}
	}
```

The wildcard overload is different from a function that takes a list of `any` in that it does type-check the arguments as much as possible. For instance, the above definition will check its arguments for valid `string` conversions.

When the wildcard function is called, all its arguments are gathered into a tuple by the compiler. Effectively, each set of arguments (rather, set of argument *types*) creates a new overload. This may cause bloating of the object code, so use this feature with care.

The wildcard is always the *last* overload checked by the compiler. If any other overload can fulfill the requirements of the caller, it will be used instead. Only when nothing else will work is the wildcard invoked.

Examples of the wildcard in the standard library are `std:basic:print` and `std:basic:join`.

## Operator overloading

Many of Rhea's builtin operators can be overloaded by defining new functions with specific names. (This is similar to Python.)

```
	# Define the "+" operator for Complex structures.
	def add$ { l: Complex, r: Complex } =
	{
		return Complex(l.real + r.real, l.imag + r.imag);
	}
```

At least one argument must be of a user-defined type; for unary operators such as `-` (`uminus`), the only argument must be of such a type.

{TBD: Full list, caveats, return types.}

## Predicates

Predicate functions are named with a trailing question mark, and they have an implied return type of `boolean`. They can be used in preconditions and postconditions.

```
	# Define a simple (and useless) predicate
	def is_foo? { str: string } =
	{
		return (str == "foo");
	}

	# Use it as a precondition (with a different definition style)
	def do_it { s: string }
	with { str.is_foo? } =
	{
		# do something only to "foo"
	}
```

Predicates can take multiple arguments, as seen further above with the built-in `less_than?`. The built-in predicate collection heavily relies on the implicit reordering of "method" style function calls.

{TBD: More about predicates.}

## Conversion functions

Conversion functions are similar to constructors, but they are used when casting with the `as` operator. All converters take a single argument of one type and return a value of another type, and they are defined as overloads of the `to$` function.

```
	def to$ [string] { c: Complex } =
	{
		var sign = if (c.imag >= 0.0) then '+' else '';
		return join(c.real, sign, c.imag, 'i');
	}
```

Either the argument or return type must be user-defined. All built-in types already have conversions to and from each other.

## Type functions

Type functions are functions that operate on *types* instead of *values*. In general, these are evaluated at compile-time rather than run-time, and they have a much more restricted set of operations.

```
	def foo { x: string } = { return (x == "Hello"); }

	# Is the function "foo" defined for this type?
	string => foo <string> -> boolean;
```

{TBD: Lots more about this important feature.}

### Naming a function type

Every specific overload of a function in Rhea has a unique name of the format `identifier <argument types> -> return type`, where `identifier` is the exact name of the function (including suffixes such as `?`), `return type` is the exact type returned (even if inferred), and `argument types` describes a comma-separated list of types.

For example, the conversion function above has the type `to$ <Complex> -> string`.

## Generic functions (templates)

Function overloads apply at run-time. Generic functions, on the other hand, are a compile-time mechanism to generate overloads based on a template.

```
	# A template that squares any kind of number it's given,
	# no matter the type.
	def square <Ty ~> Numeric>
	{ n: Ty } = {
		return n**2;
	}
```

There's a lot of expressive power packed into this small space, so it bears some examination.

First, note the definition, in particular the part in angle brackets: `Ty ~> Numeric`. This says that the `square` function is a template, with one varying type parameter. Throughout the rest of this function, `Ty` will refer to that type, which must match the `Numeric` concept (see below).

Next, the argument list of the function itself. The function takes a single argument, `n`, which will be of the `Ty` type.

When the function is called, the compiler will know which type is intended, and it will insert a call to the appropriate overload, creating it if necessary using the body of this function as a blueprint. As long as the power operator `**` is defined for the type of the argument, all is well. If not, the compiler will issue an error.

```
	# Calling our generic function

	var n1 = 4;			# an integer
	print(square(n1));	# prints the integer 16

	var n2 = 20_b		# a byte
	print(square(n2));	# prints -112 (lowest signed byte of 400)

	var n3 = "42";		# a string, which has no ** operator
	print(square(n3));	# error: no valid type
```

{TBD: Lots more about this core feature.}

### Specialization

In the case where a specific type must be treated specially, you can define a specialization of a generic function.

```
	# Maybe you'd like to "square" strings that are integers.
	def square <Ty : string>
	{ n: Ty } =
	{
		return (n as integer)**2 as string;
	}
```

## Concepts (interfaces, protocols)

One of the primary advantages of generic programming is that you can design functions that don't care about the types of their arguments. As long as those arguments have the necessary operations defined, you can work with them. (Compare, for instance, Java's interfaces or Elixir's protocols. Or C++'s concepts, which were the inspiration.)

In Rhea, these are called *concepts*. The standard library defines many of these {TBD: Describe them below.}, but you can also create your own.

### Definition

Concepts have their own syntax, which relies heavily on that of type functions.

```
	# Define a "Stringable" type as any type with a defined conversion to string
	concept Stringable <Ty> =
	{
		# Check that the "to$" function is defined for type Ty
		Ty => to$ <Ty> -> string
	}
```

{TBD: Much more. This is one of the core features of Rhea, so give it its due.}

{TBD: `.=` operator to determine if a structure type has a given field.}

### Use

A function can take arguments that are concepts. Instead of defining these using the "named" syntax shown throughout this guide, concept parameters take a different form, using the "like" operator `~>`.

```
	# Print anything that can be converted to a string
	def stringable_print { s ~> Stringable } =
	{
		print(s as string);
	}
```

The compiler will still generate separate functions for different types, but it will accept any call where `s` is of a type that fulfills the `Stringable` concept.

{TBD: Much more, for reasons stated above.}

### Caveats

You can't define variables as having the type of a concept, because concepts are not themselves types. They're only descriptions. Blueprints, in a sense.

## Variants (tagged unions)

Variables and function arguments can also be declared as members of a *variant* type. This is similar to what some languages call a "tagged union".

```
	var v as |integer, string|;
	v = 1234;		# assign an integer
	v = "abc";		# assign a string

	def fn { arg: |integer, string| } =
	{
		match arg
		{
			type integer: print("Integer");
			type string: print("String");
		}
	}
```

Constants can't be variants, and you can't have a variant as a possible type. (In other words, variants can't be nested.)

If a value of variant type is used in a `match-type` statement, the compiler can check to ensure that all possible types are accounted for. It can't do this with `any`, so this is a reason to use variants instead, if possible.

Finally, note that the compiler will consider the type of the variant's current value when choosing an overload of a generic function. Thus, a variable of `|integer, string|` is not an exact match for an argument of `|integer, string|`, and it is possible that it will match simply `integer` or `string`, if those overloads are available *and* the variable's current value is of the correct type.

{TBD: Explain `match-type` and put it in the parser.}

## Optionals

A variant with any specific type and `nothing` is such a useful pattern that Rhea offers a shortcut notation.

```
	var nullable as |integer|?;		# an optional integer

	# ...do some work that assigns to the variable...

	match nullable {
		type integer: print("Integer");
		type nothing: print("nullable is null");
	}
```

This is only valid for two-type variants where the second type is `nothing`; you can't, for instance, write `|integer, string|?`.

## Type aliases

We can declare one type as an alias of another. This is especially useful with variants, which can become unwieldy.

```
	type MyType = |string, integer, MyStructure|;
```

# Exceptions

## Try/catch

```
	try
	{
		z = x / y;
	}
	catch { e: divide_by_zero }
	{
		# "divide_by_zero" is a standard exception
		print(e.message);
	}
```

Exceptions are typed: the `catch` block specifies the type of exception you want to catch. If that exception is thrown during execution of the `try` block, the corresponding `catch` block will be executed.

You can have multiple `catch` blocks, each catching a different type of exception. If you would rather catch all exceptions, use a catch type of `any`.

A `finally` block will be executed last, regardless of whether an exception was thrown.

{TBD: Concept catching?}

## Throw

The `throw` statement throws an exception.

```
	throw bad_input();
	
	# Most standard exception types take a message argument
	throw error("Something went wrong");
```

Strictly speaking, Rhea doesn't have an exception hierarchy, because it doesn't have object hierarchies at all. Instead, exceptions are values of a given type, so the argument to a `throw` statement is typically a call to a type construction function. If you like, you can throw *any* type; `throw "Bad";` is syntactically correct.

## Standard exceptions

The standard library defines a number of basic exception types, such as dividing by zero. These are located in the `std:exception` module, which is automatically imported into all programs.

# Modules

All Rhea code resides in modules. The default module is `main`, while standard library functions are contained within module `std`. Applications can define as many modules as they wish, but only one per file.

```
	### my_module.rhea ###

	module my_module;

	def my_function = {
		# ...some function...
	}

	export { my_function };

	### main.rhea ###

	# Specific import of a function from a module
	import {my_function} from my_module;

	my_function();

	# Import an entire module, then use a function from it
	use my_module;

	my_module:my_function();
```

The name of a module does not have to match the name of the file containing that module's definition, but the Rhea compiler will search for that file first. {TBD: Some kind of config system to map modules to files.}

## Importing and exporting

Individual functions, types, and constants can be exported from a module using the `export` keyword. This takes a tuple of arguments: the names of the exported definitions. All definitions not specifically exported are considered private to the module. (In other words, Rhea defaults to private access.)

To import these definitions and use them in a program, there are two methods. First, the `import...from` syntax (similar to e.g., Typescript) brings individual definitions into the current module's scope, allowing them to be used without qualification.

The alternative `use` statement allows for the use of a module without bringing its identifiers into scope. Instead, they must be written in the form `module:identifier`.

A bare colon identifies a relative import, which is searched for starting in the directory of the importing module.

## Submodules

Modules can themselves contain modules. These are not limited by the filesystem, though the search path expects submodules to be located within their parents' directories by default.

```
	# An example using the Java-style hierarchy.
	module org:example:awesome_stuff;

	def some_function =
	{
		# ...
	}

	### In another file...
	
	# Import the whole submodule
	import { awesome_stuff } from org:example;

	# Import only a single function
	import { some_function } from org:example:awesome_stuff;
```

Many of the standard library definitions are in nested modules of this sort, such as `std:basic`.

## Lookup

By default, identifiers imported from a module will still have access to definitions in their module.

The compiler will search for function overloads in the following order:

* The module of the caller.
* The module of the object, if the method-call syntax is used.
* The module(s) of the arguments, in left-to-right order.

# Programs

## The main function

Every Rhea program must define one (and only one) `main` function. This function takes no arguments and has no return type.

```
	def main = {
		# Program execution starts here
	}
```

Definitions, module declarations, imports, and any other code may come before `main`, but it will only be executed as necessary.

## Program arguments

A program may be invoked with command-line arguments. These are not directly passed to `main`. Instead, the Rhea runtime stores them as a read-only list of strings, which can be accessed as `std:program:args`.

```
	import { args } from std:program;

	def main = {
		for arg in args
		{
			print(arg);
		}
	}
```

This array can also be referenced as `std:basic:argv` unless the compiler is in strict mode.

## Returning error codes

Similarly, although `main` itself isn't defined with a return value (and thus that value is implicitly `nothing`), the Rhea runtime will, at program exit, pass the value in `std:program:ret` to the operating system.

```
	import { ret } from std:program;

	def main = {
		print("Enter a number: ");
		
		var n = input() as integer;

		if (n < 64)
		{
			ret = n*2;
		}
		else
		{
			ret = 0;
		}
	}
```

A simpler alternative is to call the `std:basic:exit` function, passing the integer code as the single parameter.

# Foreign function interface

The `extern` keyword indicates that a function is foreign, i.e., not defined in Rhea source, but in a linked library.

```
	import { File } from std:fs;

	def raw_open! { filename: string } =
	{
		extern fopen;

		var fp = fopen(filename, "r+");

		return File(fp);
	}
```

Foreign functions can only be linked and called inside unchecked Rhea functions. Their argument lists are not specified. {TBD: More on this, and see if we can find a way to check arguments.}

## Name mangling

Rhea defines the method by which the names of potentially overloaded functions (including generic functions) are converted into strings more suitable for use in a linker. The mangling format used is not the same as any C++ compiler, though it shares some similarities with the Itanium C++ ABI.

First, any *unchecked* function will not have its name mangled. This is by design, as unchecked functions are often used for interoperability, and they are Rhea's gateway to FFI. Thus, these functions will be exported as linker symbols with names equal to those with which they were defined, minus the trailing `!`. As an example, the `raw_open!` function defined above will appear in object code with the linker symbol `raw_open`.

In addition, the function named `main` is handled specially, despite not being declared as unchecked. As the entry point to a program, it will always be exported with the symbol `main`.

All other functions are mangled, with their symbol names having the following common elements.

1. The common Rhea prefix `_R`.
2. The type of the function: `f` for "normal", `p` for predicate, `o` for operator.
3. The name of the function, with a length prefixed for non-operator functions.
4. The return type of this particular instance of the function, encoded as below, except that predicates are always boolean, so their return type is implied.
5. Each argument type, in definition order, encoded as below, or 0 if there are no arguments.

### Type mangling

Simple types are encoded in one or two characters, as follows:

| Encoding | Type     |
|----------|----------|
| v        | nothing  |
| c        | byte     |
| C        | ubyte    |
| i        | integer  |
| I        | uinteger |
| l        | long     |
| L        | ulong    |
| Dd       | double   |
| Df       | float    |
| b        | boolean  |
| Sy       | symbol   |
| s        | string   |
| a        | any      |

Arrays are encoded as `A`, the length of the array, an underscore, and the contained type.

Examples:
* `A4_i`: an array of 4 integers (Rhea `integer[4]`)
* `A16_C`: an array of 16 unsigned bytes (Rhea `ubyte[16]`)

Lists are encoded as `Sl` and the contained type.

Examples:
* A list of integers: `Sli`
* A list of strings: `Sls`
* A list of lists of symbols: `SlSlSy`

Dictionaries are encoded as `Sd` and the value type, as keys must always be integral or symbols.

Tuples are encoded as `T`, the length of the tuple, an underscore, and the type of each element. Example: `{1, "abc", @foo}` is encoded as `T3_isSy`. (Note that functions taking the wildcard will create a new overload for each different set of argument types, and those will have different mangled names.)

Enums are encoded as `E`, the length of the name, and then the name itself. Example: `E2En` for an enum declared with the name "En".

Structures are encoded starting with `Ss`, followed by the length-prefixed name of the structure. Example: `Ss6Person` for the `Person` structure used throughout this reference.

References and pointers are encoded with `r` or `p`, followed by the referenced type.

Examples:
* `ri`: a reference to an integer
* `ps`: a pointer to a string
* `rA3_Ss6Person`: a reference to an array of 3 `Person` objects

Optionals are encoded as `Op` followed by the type. Variants are encoded in the same way as tuples, but starting with `V` instead of `T`. Thus, the example of an integer, string, and symbol above, if used as a variant instead, becomes `V3_isSy`.

Type *aliases* do not affect name mangling; they are converted into their underlying types.

{TBD: Any other aspects of this}

# Standard library functions

The modules `std:basic` and `std:exception` are automatically imported into all programs, allowing their definitions to be used without qualification at any time.

## Basic

{TBD: `std:basic`}

## Program

{TBD: `std:program`}

## I/O

{TBD: `std:io`}

## Filesystem

{TBD: `std:fs`}

## Exception

{TBD: `std:exception`}

## Random

{TBD: `std:random`}

## Math

{TBD: `math`}

# Ideas for later

{Everything in this section is TBD, subject to change, etc.}

## Subtypes

* Allow creating a structure type that inherits another structure's fields.
* The subtype would identify as its own type, but also check as true when testing against its parent (as per LSP).

Example with possible syntax:

```
	type Foo = {
		i: integer,
		s: string
	};

	type Bar = Foo +> { b: byte };

	var x as Foo;
	var y as Bar;

	y is Foo;	# evaluates to true, because Bar IS-A Foo
	x is Bar;	# evaluates to false, as the relation is one-way

	x.i = 123;	# this is allowed; Bar inherits Foo's fields
	y.b = -1;	# but it doesn't work the other way
```

Questions to answer:

* How would this affect type-matching?
* What about syntax?

## Extensible variants

* Allow a variant to derive from another, adding new types.

Example with possible syntax:

```
	type foo = |integer, string|;
	type bar = foo |> byte;

	var x as bar;
	x = 42_b;

	# If using `foo` type, the third case could never happen,
	# and the compiler would warn about that.
	match x {
		type integer: print("Int");
		type string: print("String");
		type byte: print("Byte");
	}
```

Questions to answer:

* All questions from above.
* Do we want to allow this to "promote" an optional into a variant? (Probably not.)

## Macros

* Compile-time macros similar to Haxe and other languages.

Questions to answer:

* What should the syntax look like? (Possibly use surrounding backticks.)
* How do we do parsing/AST/etc. for this?
* Do we want hygienic macros, or something different?
* Should we use this feature to rewrite parts of the Rhea base? (Probably not.)

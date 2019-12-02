# Examples

These are example programs to complete the [Rosetta Code](http://rosettacode.org) programming tasks. They should use proper syntax, standard library functions, and any language features necessary.

## Simple Tasks

### A + B

```
	# Read two numbers from standard input, then send their sum to standard output.

	def main =
	{
		var a as integer;
		var b as integer;

		a = input() as integer;
		b = input() as integer;

		print(a + b);
	}
```

### Arithmetic (Integer)

```
	# Get two integers from the user, then display their sum, difference, product, integer quotient, remainder, and exponentiation.

	def main =
	{
		var a as integer;
		var b as integer;

		a = input() as integer;
		b = input() as integer;

		print("a + b = ", a + b);	
		print("a - b = ", a - b);
		print("a * b = ", a * b);
		print("a / b = ", a / b);	# Integer division truncates towards 0
		print("a % b = ", a % b);	# Integer remainder matches sign of first operand
		print("a ** b = ", a ** b);
	}
```

### Array concatenation

```
	# Show how to concatenate two arrays
	def main =
	{
		const a = [1, 2, 3, 4, 5];
		const b = [6, 7, 8, 9, 0];

		# Overloading the `+` operator for array concatenation would be too confusing.
		var c = a.concatenate(b);

		for value in c
		{
			print(value);
		}
	}
```

### Array length

Rhea arrays can't hold strings, so we use a list in this example. The `length` function is defined identically for arrays and strings, however.

```
	# Demonstrate how to find the length of an array.
	def main =
	{
		var fruit = ("apples", "oranges");

		print(fruit.length());	# or `print(length(fruit))`
	}
```

### Arrays

Rhea has fixed-size arrays for numeric, symbolic, and reference types, and dynamic lists of any homogeneous type.

```
	# Show basic array syntax: creation, assignment, retrieval.
	def main =
	{
		# Fixed-length array
		var numbers as integer[10];

		numbers[4] = 16;
		print(numbers[4]);

		# Dynamic list (in this case, a list of strings)
		var words = ("foo", "bar");
		
		words.append("baz");

		print(words[2]);	# lists and arrays are both 0-based

		# Declare a list without initialization
		# The constructor function can take an optional "reserve" argument.
		# In its absence, it will use a small, sensible default.
		var buffer = list <integer> ();
	}
```

### Boolean values

```
	# Show how to represent the boolean states "true" and "false".
	def main = {
		var bool_value as boolean;

		# Assign using a boolean value (reserved words `true` and `false`)
		bool_value = true;

		# Booleans can use the operators `and`, `or`, and `not`.
		bool_value = not bool_value;

		if (bool_value == false)
		{
			print("boolean value is false");
		}

		# No coercions to boolean, not even in `if` statements
		if (0 as boolean == false)
		{
			print("0 converts to false");
		}
	}
```

### Call a function

```
	# Demonstrate the syntax and semantics of calling functions.
	def main = {
		# Function with no arguments
		foo();

		# Alternate syntax, only usable as a statement
		do foo;

		# Function with fixed number of arguments
		bar(arg1, arg2);

		# Named arguments, only allowed for type constructors
		Complex (real: 4, imag: 2);

		# Using the return value
		var r = bar(arg1);

		# Variadic function
		def vf { args: * } =
		{
			for arg in args
			{
				print(arg);
			}
		}
		vf(1, 2, 3, '4');

		# Any more?
	}
```

### Character codes

```
	# Given a character value, print its code, and vice versa.
	def main = {
		# These functions are ASCII only.
		import { chr, ord } from std:string;

		var char = "a";

		print(char.ord());				# prints "97"

		var value = 97;

		print(value.chr());				# prints "a"

		# These functions are for Unicode (UTF-8)
		import { as_bytes, from_bytes } from std:string;

		var unichar = "π";

		print(unichar.as_bytes());		# prints "80 cf"

		var unival = [0x80, 0xcf];

		print(unival.from_bytes());		# prints "π"
	}
```

### Check that file exists

```
	# Verify that the file "input.txt" and directory "docs" exist.

	use std:fs;

	def main = {
		# File and directory in the local directory
		var file1 = std:fs:File("input.txt");
		var dir1 = std:fs:Directory("docs");

		print(file1.exists());
		print(dir1.exists());

		# File and directory in the filesystem root
		var file2 = std:fs:File("/input.txt");
		var dir2 = std:fs:Directory("/docs");

		print(file2.exists());
		print(dir2.exists());
	}
```

### Conditional structures

```
	# List the conditional structures offered.
	def main = {
		var s = "Hello world!";

		# Basic if-then-else
		if (s[0] == ('H'))
		{
			print("H!");
		}
		else
		{
			print("h");
		}

		# Reverse if (no else)
		unless s.length() > 10
		{
			print("Hello world is longer than that.");
		}

		# If-then-else expression (cf. ternary operator)
		var s2 = if (s[5] == ' ') then s else "No";

		# Pattern match
		match s2
		{
			on "Hello world!": { print("Hi!"); }
			on " ": { print("Nothing here"); }
			default: { print("Goodbye cruel world!"); }
		}
		# Also match-when with arbitary predicate functions,
		# and match-type to test for a value's type.
	}
```

### Copy a string

```
	# Copy a string
	def main = {
		var orig = "original";
		var copy = orig;
		
		copy.prepend("not ");

		print("The original: ", orig);	# prints "original"
		print("The copy: ", copy);		# prints "not original"

		# Copying references to strings
		var refer = ref orig;

		refer = "reference";

		# These both print "reference"
		print("The original: ", orig);
		print("The reference: ", refer);
	}
```

### Create a file

```
	# Create a new empty file "output.txt" and directory "docs"
	
	use std:fs;

	def main = {
		var file1 = std:fs:File("output.txt");
		var dir1 = std:fs:Directory("docs");

		do file1.create;
		do file1.close;

		do dir1.create;
		do dir1.close;

		var file2 = std:fs:File("/output.txt");
		var dir2 = std:fs:Directory("/docs");

		do file2.create;
		do file2.close;

		do dir2.create;
		do dir2.close;
	}
```

### Detect division by zero

```
	# Write a function to detect a divide by zero error,
	# without checking if the denominator is zero.
	def main = {
		var x = 1;
		var y = 0;
		var z as integer;

		try
		{
			z = x / y;
		}
		catch { e: divide_by_zero }
		{
			# "divide_by_zero" is a standard exception
			print(e.message);
		}
	}
```

### Determine if a string is numeric

```
	# A function which takes a string and tells whether it is numeric.
	def is_numeric { s: string } = {
		# One possible implemenation
		try
		{
			var value = s as double;
			return true;
		}
		catch { e: conversion_error }
		{
			return false;
		}
	}
```

### Draw a pixel

```
	# Draw a single red pixel at position (100,100) of a 320x240 window.

	# Assume that Rhea has an idiomatic SDL port
	use sdl;

	def main = {
		var init_ret = sdl:init(sdl:flags.video);

		var window = sdl:Window(
			title: "Pixel",
			width: 320,
			height: 240
		)

		var renderer = sdl:Renderer(window, 0, 0);

		renderer.clear();
		renderer.set_draw_color(0xff0000);
		renderer.draw_point(100, 100);
		renderer.present()

		sdl:delay(5000);

		sdl:quit();
	}
```

### Empty program

```
	# The simplest possible Rhea program.
	def main = {}
```

### Empty string

```
	# Features for dealing with empty strings.
	def main = {
		# Assign an empty string to a variable
		var empty = '';

		# Check that a string is empty
		if (str == '')
		{
			# ...
		}

		# Alternative
		if (str.length() == 0)
		{
			# ...
		}

		# Check that a string is not empty
		if (str != '')
		{
			# ...
		}

		# Alternative ("length" always returns a positive value)
		if (str.length() > 0)
		{
			# ...
		}
	}
```

### Environment variables

```
	# Show how to get one your process's environment variables.

	import { env } from std:program;

	def main = {
		# std:program:env is a dictionary of environment variables
		# get() retrieves a value or nothing
		print env.get("HOME");
	}
```

### Factorial

```
	# Write a function to return the factorial of a number.

	# Iterative solution
	def factorial { n: integer }
	with { n.at_least(0)? } =
	{
		var result = 1;

		for i in range(1, n)
		{
			result *= i;
		}

		return result;
	}

	# Recursive solution
	def factorial_recursive { n: integer }
	with { n.at_least(0)? } =
	{
		return (if n == 0 then 1 else n*factorial_recursive(n-1));
	}
```

### File input/output

```
	# Create a file "output.txt" and place the contents of "input.txt" into it.

	use std:fs;

	def main = {
		var input = std:fs:File("input.txt");
		var output = std:fs:File("output.txt");

		do input.open;
		var text = input.read_all();
		output.write(text);

		do input.close;
		do output.close;
	}
```

### Find limit of recursion

```
	# Find the limit of recursion.

	def recurse { n: integer } =
	{
		print(n);
		recurse(n+1);
	}

	def main = {
		recurse(0);
	}
```

### FizzBuzz

```
	# Write a program that prints the integers from 1 to 100 inclusive, except:
	# * Print "Fizz" for multiples of 3
	# * Print "Buzz" for multiples of 5
	# * Print "FizzBuzz" for multiplies of 3 and 5

	def main = {
		for i in range(1, 101)
		{
			# Simplest definition
			if (i % 3 == 0)
			{
				print("Fizz");
			}
			else if (i % 5 == 0)
			{
				print("Buzz");
			}
			else if (i % 3 == 0 and i % 5 == 0)
			{
				print("FizzBuzz");
			}
			else
			{
				print(i);
			}
		}
	}
```

### Formatted numeric output

{TBD}

### Function definition

{TBD}

### Greatest element of a list

```
	# Create a function that returns the maximum value in a list of arbitrary length.
	
	def maximum { l: list <integer> }
	with { l.not_empty? } =
	{
		# The standard library will have a function for this,
		# but it's not hard to make a simple one ourselves.
		var m = l[0];

		for i in l
		{
			if (i > m)
			{
				m = i;
			}
		}

		return m;
	}
```

### Hello world (text)

```
	# Display the string "Hello world!" on a text console.
	def main = {
		print("Hello world!");
	}
```

### Increment a numerical string

```
	# Increment a numerical string.
	def main = {
		var s = "12345";
		s = (1 + s as integer) as string;
	}
```

### Input loop

```
	# Read from a text stream until it runs out of data.
	use std:fs;

	def main = {
		var f = std:fs:File("file.txt");

		with s as string while (not f.eof)
		{
			s = f.read_line();
			print(s);
		}
	}
```

### Integer comparison

```
	# Get two integers from the user, then display a message if the
	# first is less than/equal to/greater than the second.
	def main = {
		var a = input("First integer:");
		var b = input("Second integer:");

		if (a < b)
		{
			print("a is less than b");
		}
		if (a == b)
		{
			print("a is equal to b");
		}
		if (a > b)
		{
			print("a is greater than b");
		}
	}
```

### Jump anywhere

{TBD: Do we want continuations, named breaks, or something like that?}

### Keyboard input

{TBD}

### Literals (integer)

```
	# Show as many ways to express integer literals as available.
	def main = {
		print(42);				# decimal (signed)
		print(0xaa55);			# hex

		# type literals
		print(-127_b);			# signed byte
		print(255_ub);			# unsigned byte
		print(420_l);			# signed long
		print(100000000_ul);	# unsigned long
	}
```

### Logical operations

```
	# Write a function that takes two boolean values and outputs
	# the result of "and" and "or" on both, "not" on the first.
	def logical { a: boolean, b: boolean } =
	{
		print("a and b = ", a and b);
		print("a or b = ", a or b);
		print("not a = ", not a);
	}
```

### Loops

#### Break

```
	# Write a loop that prints random numbers from 0 to 19 inclusive.
	# If the number is 10, stop after printing it,
	# otherwise, print a second random number and restart the loop.

	use std:random;

	def main = {
		var rng = std:random:Random();

		while (true)
		{
			var n = rng.random_integer(19) + 1;
			print(n);

			if (n == 10) break;

			n = rng.random_integer(19) + 1;
			print(n);
		}
	}
```

#### Do-while

```
	# Start with a value of 0, and loop while (value mod 6) != 0,
	# but the loop must execute at least once.
	def main = {
		var n = 0;
		
		# Rhea doesn't have a do-while loop
		while (true)
		{
			n += 1;
			if n % 6 == 0 { break; }
		}
	}
```

#### Downward for

```
	# For loop which writes a countdown from 10 to 0.
	def main = {
		for i in range(10, 0, -1)
		{
			print(i);
		}
	}
```

#### For

```
	# Show how two loops may be nested within each other.
	def main = {
		for i in range(5)
		{
			for j in range(i)
			{
				print('*');
			}

			print('\n');
		}
	}

```

#### For with a specified step

```
	# For loop where the step value is greater than 1.
	def main = {
		for i in range(1, 100, 2)
		{
			print(i);
		}
	}
```

#### Increment loop index within loop body

```
	# Write a loop that:
	# * starts the index variable at 42,
	# * increments the index by 1 each loop,
	# * prints a prime index and total number of primes found,
	# * increments the index to the old index plus the found prime,
	# until 42 primes are shown.

	def is_prime { n: long }
	with { n.greater_than(1)? } =
	{
		if (n % 2_l == 0_l) { return (n == 2_l); }
		if (n % 3_l == 0_l) { return (n == 3_l); }

		var d = 5_l;

		while (d * d <= n)
		{
			if (n % d == 0_l) { return false; }
			d = d + 2;

			if (n % d == 0_l) { return false; }
			d = d + 4;
		}

		return true;
	}

	def main = {
		# Rhea doesn't allow modification of the index variable,
		# so we'll rewrite this a bit.

		var index = 42_l;

		for i in range(1, 43)
		{
			while (is_prime(index) == false)
			{
				index += 1;
			}

			print("n = ", i, ", ", index);
			index = (index * 2) - 1;
		}
	}
```

#### Infinite

```
	# Print "SPAM" in an infinite loop
	def main = {
		while (true)
		{
			print("SPAM\n");
		}
	}
```

#### N plus one half

```
	# Write a loop which prints a comma-separated list of integers
	# from 1 to 10 using separate output statements.
	def main = {
		for i in range(1, 11)
		{
			print(i);
			
			unless i == 10 { print(', '); }
		}
	}
```

#### While

```
	# Loop from 1024 to 0, printing the value plus newline, then
	# dividing by two each time.
	def main = {
		var n = 1024;

		while (n > 0)
		{
			print(n, '\n');
			n = n / 2;
		}
	}
```

#### With multiple ranges

{TBD: I need to wrap my head around what this is asking.}

### Multiplication tables

```
	# Produce a formatted 12x12 multiplication table, only printing
	# the top half of products.
	def main = {
	
		# First print the header
		# (range_in is an inclusive range)
		print(" X|");

		for header in range_in(1,12)
		{
			print(pad_left(header as string, 4, ' '));
		}

		# Now the row line
		print(pad_right("\n--+", 4*12, '-'), '\n');

		# Now the table
		for i in range_in(1, 12)
		{
			print(pad_left(i as string, 2, ' '), '|');

			for j in range_in(1, 12)
			{
				if j < i
					print("    ");
				else
					print(pad_left(i*j as string, 4, ' '));
			}

			print('\n');
		}
	}
```

### Naming conventions

{TBD: We don't have a style guide just yet. These are working guidelines:

	* All keywords are lowercase
	* Variables start with a lowercase letter
	* Constants start with a capital letter
	* Built-in types are lowercase, while user-defined types are usual CamelCase
	* Predicates always end with `?` (this is required)
	* Unchecked functions must end with `!`
	* Operator overloads are named functions ending in `$`
	* Standard built-in functions use underscores to separate words
	* Concepts start with a capital letter and are usually adjectives
	* Generic typenames are usually short and capitalized
}

### Nth root

```
	# Implement the algorithm to compute the nth root of a positive real number.

	use math;

	def nth_root { a : double, n : integer }
	with { a.greater_than(0.0)?, n.greater_than(2) } =
	{
		var x0 = a;
		var x1 = a / ^n;

		# A better method would have the precision as a parameter
		while (math:abs(x1 - x0) > 0.0001)
		{
			x1 = x0;
			x0 = (1.0 / ^n) * (((n - 1) * x1) + (a / math:pow(x1, n-1)));
		}

		return x0;
	}
```

### Null object

{TBD: Tagged union as an optional-like structure.}

```
	# Show how to access a null object or tell if an object is null.
	def is_null { o: any } = {
		return o is nothing;
	}
```

### Operator precedence

{TBD: Full operator precedence table.}

### Program termination

```
	# Show how to completely stop a program inside a conditional.

	use std:program;

	def main = {
		var problem as boolean;

		if (problem)
		{
			# Any nonzero exit code is considered an error
			exit(1);
		}
	}
```

### Read a file line by line

```
	# Read the contents of a file one line at a time.
	use std:fs;

	def main = {
		var input = std:fs:File("input.txt");

		do input.open;

		with s as string while (not input.eof)
		{
			s = input.read_line();
			print(s);
		}

		do input.close;
	}
	
```

### Real constants and functions

```
	# Show how to use various mathematical constants and functions.
	use math;

	def main = {
		print(math:E);			# 2.71828...
		print(math.Pi);			# 3.14159...

		print(math:sqrt(4.0));	# 2.0
		print(math:ln(std:math:E));	# 1.0
		print(math:exp(1.0));	# 2.71828...

		print(math:abs(-2));	# 2
		print(math:abs(-2.0));	# 2.0
		print(math:floor(3.6));	# 3.0
		print(math:ceil(3.6));	# 4.0
		print(math:pow(3.0, 2.0));	# 9.0
	}
```

### RPG attributes generator

{TBD: Finish this.}

```
	# Write a program that generates RPG stats using the 4d6-1 method.
	# Order must be preserved, total must be at least 75, and 2 stats
	# must be at least 15.
	
	def main = {
		var attributes_total = 0;
		var count = 0;


	}
```

### Simple windowed application

{TBD: Until we decide what kind of windowing/GUI we want.}

### Split a character string based on change of character

```
	# Helper function to split a string into runs of characters
	def split { input: string } = {
		var working as string;
		var separate as list <string>;

		# Temp variable to hold our previous character
		with (last as string)
		{
			working = input[0];
			last = input[0];

			for i in range(1,string.length())
			{
				var current = input[i];

				if (current == last)
				{
					# Still on the same character
					working += current;
				}
				else
				{
					# Character has changed
					separate.append(working);
					working = current;
				}

				last = current;
			}
		}
	}

	def main = {
		var split_string = split("gHHH5YY++///\\");

		for el in slice(split_string, -1)
		{
			print(el + ", ");
		}
		print split_string[split_string.length()-1];
	}
```

### String append

```
	# Create a string variable, append another string literal to it, and print.
	def main = {
		var s = "Foo";

		# Rhea does define append and append/assign operators for strings
		# (ref: `std:basic:add_assign$(string, string)`)
		s += "Bar";

		print(s);		# prints "FooBar"
	}
```

### String comparison

{TBD: Lots of work here, especially stdlib stuff. Save for later.}

### String concatenation

```
	# Create a string variable, then another that has that string
	# concatenated to another. Print both.
	def main = {
		var s1 = "Foo";

		# (ref: `std:basic:add$(string, string)`)
		var s2 = s1 + "Bar";

		print(s1);			# prints "Foo"
		print(s2);			# prints "FooBar"
	}
```


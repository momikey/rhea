# Examples

These are example programs fulfilling the [Rosetta Code](http://rosettacode.org) programming tasks. They should use proper syntax, standard library functions, and any language features necessary.

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

		print(fruit.length());
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
		var buffer = list [integer] ();
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
		for i in range(1, 100)
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
	
	def maximum { l: list [integer] }
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


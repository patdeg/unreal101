# Glossary

Plain-language meanings for the words Part 1 uses, in A to Z order. The
number in brackets is the lesson that introduces the word. "JS" means
JavaScript.

**Address** (13) - Where a value lives in memory: a number, like a house
number on a very long street. `&hp` gives the address of `hp`.

**AddressSanitizer, ASan** (05) - A guard you switch on with a build flag
(`/fsanitize=address` or `-fsanitize=address`). While the program runs, it
watches memory and stops with a report when you read or write where you
should not. See also *sanitizer*.

**auto** (02, 05) - Lets the compiler work out a variable's type for you.
The type is still fixed; you just do not have to write it.

**Breakpoint** (12) - A line where the debugger pauses your program, so you
can look at the variables. Like `debugger;` in JS.

**Call stack** (12) - The chain of function calls that led to the line you
are paused on, newest on top. It answers "who called me?".

**Capture** (16) - The `[ ]` at the start of a lambda: which outside
variables it may use. `[x]` copies `x`, `[&x]` uses the real `x`.

**Class** (11) - A type that bundles data with the functions that work on
it, and decides what is `public` and what is `private`. Like a JS or Python
class, with stricter walls.

**Compiler** (01) - The program that reads your C++ and turns it into
machine code. Nothing runs until it accepts your whole file. JS and Python
skip this step: an interpreter runs your text directly.

**const** (02) - "This must not change." The compiler stops anyone who tries.
After a member function (`int getHp() const`), it means "this function only
looks" (11).

**Constructor** (11) - A function with the class's own name that runs when
an object is created, and sets it up. Like `constructor(...)` in JS or
`__init__` in Python.

**Dangling pointer** (13) - A pointer to something that no longer exists,
for example to a vector's old memory after it moved. Following it is
undefined behavior.

**Debug info** (05, 12) - A map from machine code back to your lines and
variable names, added by `/Zi` or `-g`. Debuggers and sanitizer reports need
it.

**Debugger** (12) - A tool that runs your program step by step, so you can
watch it think. Visual Studio, `gdb` and `lldb` are debuggers.

**Declaration** (14) - A promise that something exists: its name and types,
no body. `int rollDice(int min, int max);` Headers are full of them.

**Definition** (14) - The real thing, with the body. Every declared function
needs exactly one definition somewhere, or the linker complains.

**Destructor** (17) - A function written `~ClassName()` that runs by itself
when an object dies. Used for cleanup.

**enum class** (09) - A type with a fixed list of named values, such as
`Stance::Defending`. JS has no enums; people use strings and hope for no
typos.

**Exception** (05) - An error that is thrown and stops the program unless
something catches it. Like `throw` in JS or `raise` in Python. `.at()`
throws one when the position is wrong.

**Header** (14) - A `.h` file that declares what exists, so other files can
`#include` it. The "menu"; the `.cpp` file is the "kitchen".

**Heap** (17) - Memory for objects that are not tied to one `{ }` block.
`new` and `std::make_unique` (18) put objects there.

**Inheritance** (15) - "Is a." `class Goblin : public Monster` means a Goblin
is a Monster and gets everything a Monster has. Like `extends` in JS.

**Initializer list** (11, 15) - The part of a constructor after the colon,
`: Fighter(name, hp, ...)`, that builds the parent and the fields before the
body runs. Like calling `super(...)` first in JS.

**Iterator** (07) - A bookmark into a container. `begin()` is on the first
item, `end()` is one step past the last one, and `std::find` returns one.

**Lambda** (16) - A function without a name, written right where you need
it. C++'s version of a JS arrow function or a Python `lambda`.

**Linker** (14) - The second step of building: it connects every call to
the body of the function, across all your files, and writes the program.
Its errors say `undefined reference` or have an `LNK` code.

**Map** (07) - `std::map<Key, Value>`: look up a value by its key. Like a JS
object used as a dictionary, or a Python `dict`, with fixed types.

**Memory leak** (17) - An object that is never destroyed, because nobody
deleted it. The memory stays taken until the program ends.

**nullptr** (13) - A pointer that points at nobody. Like `null` in JS or
`None` in Python. Never follow it; check first with `if (pointer)`.

**override** (15) - Written after a child's function to say "this replaces a
virtual function in my parent". The compiler checks that it really does.

**Owner** (18) - The one thing responsible for destroying an object. A
`std::unique_ptr` is an owner; a plain pointer only looks.

**Pointer** (13) - A variable that holds an address. `Fighter* target` holds
where a Fighter lives. It can be empty (`nullptr`) and can be re-pointed.

**RAII** (17) - "Setup in the constructor, cleanup in the destructor", so
cleanup can never be forgotten. Python's `with open(...) as f:` is the same
idea.

**Reference** (10) - Another name for an existing value, written with `&`:
`Fighter& f`. Changes through it change the original. It can never be empty.

**Sanitizer** (05) - A checker built into your program by a compiler flag.
AddressSanitizer catches memory mistakes; the "undefined" sanitizer
(`-fsanitize=undefined`) catches some other undefined behavior.

**Scope** (04) - Where a name exists: from where it is made to the closing
`}` of its block. When the block ends, its local objects die.

**Stack** (17) - Memory for local variables. They are created when their
block starts and destroyed automatically when it ends.

**std::** (01) - "From the standard library", the toolbox that comes with
C++. `std::cout`, `std::string`, `std::vector`.

**Struct** (08) - A type you invent that bundles values: `struct Fighter`.
Like a JS object with a fixed shape. Everything in it is public.

**Type** (02) - What kind of value something is: `int`, `double`, `bool`,
`std::string`, or one you make. In C++ a variable's type never changes.

**Undefined behavior, UB** (05) - Code that breaks a C++ rule the compiler
does not check, such as reading past the end of a vector. The rules say
nothing about what happens: junk, a crash, or "works on my machine" are all
possible.

**unique_ptr** (18) - `std::unique_ptr<T>`: a pointer that owns its object
and destroys it when the pointer goes away. It cannot be copied, only moved.

**Vector** (05) - `std::vector<T>`: a growable list that holds one type. Like
a JS array or a Python list, but every item has the same type.

**virtual** (15) - Marks a function that a child may replace, so the real
object's version runs even through a parent pointer.

Back to: [00-start-here](00-start-here/)

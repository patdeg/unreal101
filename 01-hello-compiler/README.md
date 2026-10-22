# 01 - Hello, compiler

## Goal

Compile and run your first C++ program, and read your first error message.

## The new idea

- **C++ takes two steps.** In JavaScript you typed `node hello.js` and it
  ran. In C++ the **compiler** first reads your whole program and builds a
  new file, `main.exe`. Then you run that file. Nothing runs until the whole
  program passes the compiler's checks.
- **The output is a real program.** `main.exe` runs without the compiler, and
  you could send it to a friend.
- **Printing looks different.** `console.log("hi")` becomes
  `std::cout << "hi\n";`. Think of `<<` as arrows pushing text onto the
  screen.

## Try it

**Step 1: make your own folder.** In your terminal, go to the top folder of
this repo (the one that holds `00-start-here`, `01-hello-compiler`, ...) and
make a folder called `my-arena`:

```
cd ..
mkdir my-arena
cd my-arena
```

(If you were in `00-start-here`, `cd ..` takes you up to the top folder.
Use `pwd` to check where you are.) This is where you type the Arena for all
of Part 1. The lesson folders keep the reference copies.

**Step 2: type the program.** In `my-arena`, create a file named `main.cpp`
and **type** the program from `01-hello-compiler/main.cpp` (the comments
are optional, but reading them is the lesson).

The core of it is:

```cpp
#include <iostream>

int main() {
    std::cout << "Hello, Arena!\n";
    return 0;
}
```

**Step 3: build and run it**, from the `my-arena` folder.

**Windows (Developer PowerShell):**

```
cl /nologo /std:c++20 /W4 /EHsc main.cpp
.\main.exe
```

**Linux / macOS:**

```
g++ -std=c++20 -Wall -Wextra main.cpp -o main
./main
```

You should see:

```
Hello, Arena!
```

Now run `dir` (Windows) or `ls` (Linux/macOS). There is a new file,
`main.exe` (just `main` on Linux/macOS). That is your program. On Windows
you will also see `main.obj`, a half-built piece the compiler leaves
behind. Lesson 14 explains it.

## What just happened

1. `#include <iostream>` told the compiler to fetch the part of C++ that
   knows how to print.
2. The compiler read `main.cpp`, checked every line, and wrote `main.exe`.
3. Running `main.exe` started at `main()`, printed the message, and ended.

Two small rules to remember: every statement ends with `;`, and a block of
code lives between `{` and `}`.

## Break it

`break-it.cpp` is the same program with one semicolon missing. You do not
type break-it files: build them where they are, in the lesson folder.

```
cd ../01-hello-compiler
```

Build it with the same command as before, with `break-it.cpp` in place of
`main.cpp`, and read the message. On Linux with g++ you will see:

```
break-it.cpp:6:35: error: expected ';' before 'return'
    6 |     std::cout << "Hello, Arena!\n"
      |                                   ^
      |                                   ;
```

On Windows the words differ a little (you will see an error code such as
`C2143`, depending on your Visual Studio version), but the message has the
same parts:

1. **Where:** the file and line number.
2. **What:** a short description.
3. **A picture** (g++ only): the line, with an arrow.

Notice *when* the compiler noticed: g++ says it expected `;` "before
`return`", the first word of the next line. Visual Studio reports the error
on line 7, the `return` line itself. A compiler only notices a missing `;`
when it reaches the next thing. Lesson: **look at the line it names and the
one above it.** Also: when there are many errors, **the first one is the
real one.**

Fix it by adding the `;` and build again. Then go back to your own folder
(`cd ../my-arena`).

## Try it yourself

1. Change the message and rebuild. Every change needs a rebuild.
2. Add a second `std::cout` line so the program prints two lines.
3. Misspell `cout` as `count` and read the error. Then remove the
   `#include <iostream>` line and read that one.

## Going further

`std::` means "from the standard library", the toolbox that comes with C++.
You will see it everywhere. A line `using namespace std;` would let you skip
typing it, but most professional code avoids that, and so do these
lessons.

**Checkpoint: you can now** compile a C++ file, run the program, and find
the file, line and message in an error.

Next: [02-variables-and-types](../02-variables-and-types/) - give your hero
stats, and meet C++'s big idea: every value has a type.

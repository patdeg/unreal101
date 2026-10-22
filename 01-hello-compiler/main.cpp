// main.cpp - Lesson 01: Hello, compiler
//
// Type this file yourself (don't paste it!). Typing makes your fingers learn
// the shapes: the braces, the semicolons, the double colons.
//
// In JavaScript you wrote:   console.log("Hello, Arena!");
// In C++ the same idea takes a few more lines. Each one has a job.

// "#include" says: before you read my code, go and fetch another file.
// <iostream> is part of C++ itself. It knows how to print to the screen
// and read from the keyboard ("io" = input/output, "stream" = a flow of text).
#include <iostream>

// Every C++ program starts running at a function named "main".
// "int" means main hands back a whole number when it finishes.
// The empty () means we are not passing anything in.
int main() {
    // std::cout is "the screen". The << arrows push text into it,
    // like pouring water into a pipe.  \n means "start a new line".
    // Every statement in C++ ends with a semicolon. JavaScript lets you
    // forget them. C++ does not.
    std::cout << "Hello, Arena!\n";

    // Returning 0 from main means "everything went fine".
    return 0;
}

// pointers-basics.cpp - a pointer, slowly, before the Arena uses one.
//
// Build it, run it, then step through it in the debugger (lesson 12) and
// watch "hp" and "p" in the Locals window.
#include <iostream>

int main() {
    int hp = 30;

    // &hp means "the address of hp": where hp lives in memory.
    // int* p means "p holds the address of an int". p is a pointer.
    int* p = &hp;

    std::cout << "hp is " << hp << "\n";
    std::cout << "hp lives at address " << p << "\n";   // a hex number
    std::cout << "following p gives " << *p << "\n";     // *p: go there, read

    *p = 10;   // go to the address in p, and write 10 THERE
    std::cout << "after *p = 10, hp is " << hp << "\n";  // hp changed!

    p = nullptr;   // now p points at nothing
    if (p) {
        std::cout << "p points somewhere\n";
    } else {
        std::cout << "p is empty (nullptr), so we must not follow it\n";
    }
    return 0;
}

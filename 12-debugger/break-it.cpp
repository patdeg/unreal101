// break-it.cpp - a tiny program to practice the debugger on.
// Three hits of 8, 10 and 12 should add up to 30. Build it, run it, and
// see what it really prints. Then find out WHY with a breakpoint, not by
// staring at the code.
#include <iostream>
#include <vector>

int main() {
    std::vector<int> hits{8, 10, 12};
    int total = 0;

    for (std::size_t i = 1; i < hits.size(); ++i) {
        total += hits[i];   // <- put a breakpoint on this line
    }

    std::cout << "Total damage: " << total << "\n";
    return 0;
}

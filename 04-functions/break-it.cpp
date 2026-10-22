// break-it.cpp - call rollDice with text where a number belongs.
#include <iostream>

int rollDice(int min, int max) {
    return min + max;   // pretend this rolls dice
}

int main() {
    std::cout << rollDice("one", 6) << "\n";
    return 0;
}

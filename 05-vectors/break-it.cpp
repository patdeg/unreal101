// break-it.cpp - read past the end of a vector.
// The bag holds 3 gold pouches, at positions 0, 1 and 2. We ask for
// position 3. JavaScript would quietly give you "undefined".
// What does C++ do?
#include <iostream>
#include <vector>

int main() {
    std::vector<int> goldPouches{10, 25, 5};

    std::cout << "Pouch 0 holds " << goldPouches[0] << " gold\n";
    std::cout << "Pouch 3 holds " << goldPouches[3] << " gold\n";   // no pouch 3!

    return 0;
}

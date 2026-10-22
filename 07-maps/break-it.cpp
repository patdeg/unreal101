// break-it.cpp - look up a key with a typo, using [ ].
// No error. No warning. Count the items in the shop before and after.
#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> prices{
        {"sword", 50},
        {"shield", 30},
    };
    std::cout << "The shop lists " << prices.size() << " items.\n";

    // Oops: "sowrd" instead of "sword".
    std::cout << "A sowrd costs " << prices["sowrd"] << " gold.\n";

    std::cout << "The shop lists " << prices.size() << " items.\n";
    if (prices.contains("sowrd")) {
        std::cout << "...and one of them is a 'sowrd'!\n";
    }
    return 0;
}

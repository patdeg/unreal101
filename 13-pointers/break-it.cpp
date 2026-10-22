// break-it.cpp - keep a pointer into a vector, then grow the vector.
// Build it WITH the sanitizer (/fsanitize=address or -fsanitize=address).
#include <iostream>
#include <string>
#include <vector>

struct Fighter {
    std::string name;
    int hp = 0;
};

int main() {
    std::vector<Fighter> party{{"Ayla", 40}};

    Fighter* captain = &party[0];   // remember where Ayla lives
    std::cout << "Captain HP: " << captain->hp << "\n";

    party.push_back({"Bram", 50});  // a new recruit joins...

    std::cout << "Captain HP: " << captain->hp << "\n";   // ...and now?
    return 0;
}

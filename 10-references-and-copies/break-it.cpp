// break-it.cpp - "heal the whole party". It compiles. It runs.
// Look at the HP before and after. Did anyone get healed?
#include <iostream>
#include <string>
#include <vector>

struct Fighter {
    std::string name;
    int hp = 0;
};

int main() {
    std::vector<Fighter> party{{"Ayla", 20}, {"Bram", 15}};

    std::cout << "Before: " << party[0].name << " " << party[0].hp << ", "
              << party[1].name << " " << party[1].hp << "\n";

    for (auto member : party) {   // <- the bug is on this line
        member.hp += 10;
        std::cout << "  healing " << member.name << " to " << member.hp << "\n";
    }

    std::cout << "After:  " << party[0].name << " " << party[0].hp << ", "
              << party[1].name << " " << party[1].hp << "\n";
    return 0;
}

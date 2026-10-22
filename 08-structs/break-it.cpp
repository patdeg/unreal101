// break-it.cpp - use a field that does not exist.
// In JavaScript, goblin.health would just be undefined. Here?
#include <iostream>
#include <string>

struct Fighter {
    std::string name;
    int hp = 0;
};

int main() {
    Fighter goblin{"Goblin", 30};
    std::cout << goblin.name << " has " << goblin.health << " HP\n";
    return 0;
}

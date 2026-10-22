// break-it.cpp - a function that is DECLARED but never DEFINED.
// The class promises takeDamage exists. Nobody ever writes its body.
// Build it. Which step complains: the compiler, or the linker?
#include <iostream>

class Fighter {
public:
    int takeDamage(int amount);   // declared: "this exists somewhere"
    int getHp() const { return hp; }

private:
    int hp = 30;
};

// (Fighter::takeDamage should be defined here. It is missing.)

int main() {
    Fighter goblin;
    goblin.takeDamage(10);
    std::cout << "Goblin HP: " << goblin.getHp() << "\n";
    return 0;
}

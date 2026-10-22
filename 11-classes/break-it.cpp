// break-it.cpp - reach into a class and set its HP from outside.
// With a struct this worked. With a class?
#include <iostream>
#include <string>

class Fighter {
public:
    Fighter(const std::string& startName, int startHp) {
        name = startName;
        hp = startHp;
    }

    int getHp() const { return hp; }

private:
    std::string name;
    int hp = 0;
};

int main() {
    Fighter goblin{"Goblin", 30};
    goblin.hp = -50;   // "just set it directly, it's faster"
    std::cout << "Goblin HP: " << goblin.getHp() << "\n";
    return 0;
}

// break-it.cpp - the base class FORGOT to say "virtual".
// It compiles. It runs. Read what each monster does.
#include <iostream>
#include <vector>

class Monster {
public:
    void takeTurn() const {           // <- no "virtual" here
        std::cout << "  The monster attacks.\n";
    }
};

class Goblin : public Monster {
public:
    void takeTurn() const {
        std::cout << "  The goblin stabs twice!\n";
    }
};

class Troll : public Monster {
public:
    void takeTurn() const {
        std::cout << "  The troll regenerates, then smashes!\n";
    }
};

int main() {
    Goblin goblin;
    Troll troll;
    std::vector<Monster*> wave{&goblin, &troll};

    std::cout << "Calling each one directly:\n";
    goblin.takeTurn();
    troll.takeTurn();

    std::cout << "Calling through Monster* pointers:\n";
    for (Monster* monster : wave) {
        monster->takeTurn();
    }
    return 0;
}

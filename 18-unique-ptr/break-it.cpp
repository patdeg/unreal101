// break-it.cpp - try to COPY the owner of a monster.
// "There can be only one" owner. Build it and read who complains.
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Monster {
public:
    Monster(const std::string& startName) { name = startName; }
    ~Monster() { std::cout << "The " << name << " fades away.\n"; }
    const std::string& getName() const { return name; }

private:
    std::string name;
};

int main() {
    std::vector<std::unique_ptr<Monster>> wave;
    wave.push_back(std::make_unique<Monster>("Goblin"));

    auto copy = wave[0];   // two owners of one goblin?

    std::cout << copy->getName() << "\n";
    return 0;
}

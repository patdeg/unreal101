// break-it.cpp - compare an enum class with a plain number.
// "Defending is the second one, so it must be 1, right?"
#include <iostream>

enum class Stance {
    Normal,
    Defending,
    Stunned,
};

int main() {
    Stance goblinStance = Stance::Defending;

    if (goblinStance == 1) {
        std::cout << "The goblin is defending.\n";
    }
    return 0;
}

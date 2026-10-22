// main.cpp - Lesson 03: Decisions and loops
//
// Arena step: the hero and a goblin fight until one of them falls.

#include <iostream>
#include <string>

int main() {
    const std::string heroName = "Ayla";
    const std::string monsterName = "Goblin";
    int heroHp = 40;
    int monsterHp = 30;
    const int heroAttack = 12;
    const int monsterAttack = 7;
    const int potionHeal = 10;

    std::cout << heroName << " faces a " << monsterName << "!\n";

    // "while" is the same idea as in JavaScript. The condition goes in ( ).
    // && means "and" (same as JS). || means "or".
    while (heroHp > 0 && monsterHp > 0) {
        std::cout << "\n" << heroName << " HP " << heroHp
                  << "   " << monsterName << " HP " << monsterHp << "\n";
        std::cout << "(a)ttack or (h)eal? ";

        // A char holds ONE character. Single quotes 'a' make a char.
        // Double quotes "a" make text. In C++ they are different things.
        char choice = ' ';

        // std::cin >> choice  is the opposite of cout: it reads from the
        // keyboard into the variable. (Browser JS: prompt(). Python: input().)
        // If the keyboard closes (Ctrl+Z on Windows, Ctrl+D elsewhere),
        // the read fails. We check for that so the loop cannot spin forever.
        if (!(std::cin >> choice)) {
            std::cout << "\nNo more input. Goodbye.\n";
            return 0;
        }

        // if / else if / else look exactly like JavaScript.
        // == compares. C++ has no ===. If the two sides have different
        // types, C++ either converts one by fixed rules (1 == 1.0 is true)
        // or refuses to compile.
        if (choice == 'a') {
            monsterHp -= heroAttack;   // same as monsterHp = monsterHp - heroAttack
            std::cout << heroName << " hits for " << heroAttack << "!\n";
        } else if (choice == 'h') {
            heroHp += potionHeal;
            std::cout << heroName << " drinks a potion (+" << potionHeal << ").\n";
        } else {
            std::cout << "That is not a move. You lose your turn!\n";
        }

        // The goblin only strikes back if it is still alive.
        if (monsterHp > 0) {
            heroHp -= monsterAttack;
            std::cout << monsterName << " hits for " << monsterAttack << "!\n";
        }
    }

    if (heroHp > 0) {
        std::cout << "\nVictory! " << heroName << " wins.\n";
    } else {
        std::cout << "\nDefeat... the " << monsterName << " wins.\n";
    }
    return 0;
}

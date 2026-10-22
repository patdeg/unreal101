// main.cpp - Lesson 06: Working with text
//
// Arena step: no more one-letter menu. The player types real commands:
//   attack            use potion            look            inventory
// and picks the hero's name, spaces and all.

#include <iostream>
#include <random>
#include <string>
#include <vector>

int rollDice(int min, int max) {
    static std::mt19937 generator{std::random_device{}()};
    std::uniform_int_distribution<int> distribution{min, max};
    return distribution(generator);
}

int main() {
    // std::getline reads a WHOLE line, spaces included, up to Enter.
    // (std::cin >> name would stop at the first space. See break-it.cpp.)
    //   JS:   let heroName = prompt("Name your hero:");
    std::string heroName;
    std::cout << "Name your hero: ";
    if (!std::getline(std::cin, heroName) || heroName.empty()) {
        heroName = "Ayla";   // no name typed? use a default
    }

    int heroHp = 60;
    const int potionHeal = 10;
    std::vector<std::string> inventory{"bread"};
    const std::vector<std::string> monsters{"Goblin", "Wolf", "Goblin Chief"};
    const std::vector<std::string> lootTable{"rusty sword", "gold coin",
                                             "goblin ear", "healing herb"};

    for (const auto& monsterName : monsters) {
        int monsterHp = 30;
        std::cout << "\nA " << monsterName << " appears!\n";

        while (heroHp > 0 && monsterHp > 0) {
            std::cout << "\n" << heroName << " HP " << heroHp
                      << "   " << monsterName << " HP " << monsterHp << "\n";
            std::cout << "> ";

            std::string line;
            if (!std::getline(std::cin, line)) {
                std::cout << "\nNo more input. Goodbye.\n";
                return 0;
            }

            // Split "use potion" into a verb ("use") and an object ("potion").
            //
            // find(' ') gives the position of the first space.
            //   JS:   line.indexOf(" ")   returns -1 when there is none
            //   C++:  line.find(' ')      returns std::string::npos instead
            // npos is a special "not found" number. Always compare with it,
            // never with -1.
            std::string verb = line;
            std::string object;
            std::size_t space = line.find(' ');
            if (space != std::string::npos) {
                // substr(start, count) cuts out a piece, like JS substring.
                verb = line.substr(0, space);    // everything before the space
                object = line.substr(space + 1); // everything after it
            }

            // Strings compare with == in C++, just like JS.
            if (verb.empty()) {
                continue;   // the player just pressed Enter
            } else if (verb == "look") {
                std::cout << "A " << monsterName << " with " << monsterHp
                          << " HP glares at " << heroName << ".\n";
                continue;   // looking is free
            } else if (verb == "inventory" || verb == "inv") {
                std::cout << "You carry " << inventory.size() << " item(s):\n";
                for (const auto& item : inventory) {
                    std::cout << "  - " << item << "\n";
                }
                continue;
            } else if (verb == "attack") {
                int damage = rollDice(8, 16);
                monsterHp -= damage;
                std::cout << heroName << " hits for " << damage << "!\n";
            } else if (verb == "use" && object == "potion") {
                heroHp += potionHeal;
                std::cout << heroName << " drinks a potion (+" << potionHeal << ").\n";
            } else if (verb == "use") {
                // + joins strings, like JS. "You can't use '" + object works
                // because object is a std::string. (Two plain "..." texts
                // cannot be joined with + in C++.)
                std::string message = "You can't use '" + object + "' here.";
                std::cout << message << "\n";
                continue;
            } else {
                std::cout << "Unknown command '" << verb
                          << "'. Try: attack, use potion, look, inventory\n";
                continue;   // a typo should not cost you a turn
            }

            if (monsterHp > 0) {
                int damage = rollDice(3, 9);
                heroHp -= damage;
                std::cout << monsterName << " hits for " << damage << "!\n";
            }
        }

        if (heroHp <= 0) {
            std::cout << "\nDefeat... the " << monsterName << " wins.\n";
            return 0;
        }

        std::string loot = lootTable[rollDice(0, 3)];
        inventory.push_back(loot);
        std::cout << "The " << monsterName << " drops a " << loot << ".\n";
    }

    std::cout << "\nVictory! " << heroName << " cleared the arena with "
              << inventory.size() << " items:\n";
    for (const auto& item : inventory) {
        std::cout << "  - " << item << "\n";
    }
    return 0;
}

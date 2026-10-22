// main.cpp - Lesson 08: Your own types
//
// Arena step: heroes and monsters are described by ONE type, Fighter.
// No more loose heroHp / monsterHp variables. Each monster now has its
// own stats, too.

#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

// A struct is a new type that YOU invent. It bundles values that belong
// together.
//
//   JS:   const goblin = { name: "Goblin", hp: 30, minDamage: 4, maxDamage: 9 };
//   C++:  Fighter goblin{"Goblin", 30, 4, 9};
//
// The big difference: the shape is fixed. Every Fighter has exactly these
// four fields, with exactly these types. You cannot add "goblin.color" later
// the way you could in JavaScript. The "= 0" parts are default values, used
// when you don't give one.
struct Fighter {
    std::string name;
    int hp = 0;
    int minDamage = 0;
    int maxDamage = 0;
};   // <- note the semicolon after the closing brace. Easy to forget!

int rollDice(int min, int max) {
    static std::mt19937 generator{std::random_device{}()};
    std::uniform_int_distribution<int> distribution{min, max};
    return distribution(generator);
}

int main() {
    // Make a Fighter. The values fill the fields in order:
    // name, hp, minDamage, maxDamage.
    Fighter hero{"Ayla", 60, 8, 16};

    std::cout << "Name your hero: ";
    std::string typedName;
    if (std::getline(std::cin, typedName) && !typedName.empty()) {
        hero.name = typedName;   // the dot reaches a field, same as JS
    }

    int gold = 0;
    int potions = 3;             // new: potions run out, so choose when to drink
    const int potionHeal = 15;
    std::vector<std::string> inventory{"bread"};

    // A vector of your own type works exactly like a vector of strings.
    const std::vector<Fighter> monsters{
        {"Goblin", 30, 4, 9},
        {"Wolf", 30, 5, 10},
        {"Goblin Chief", 50, 6, 12},
    };
    const std::vector<std::string> lootTable{"rusty sword", "gold coin",
                                             "goblin ear", "healing herb"};
    std::map<std::string, int> prices{
        {"rusty sword", 12}, {"gold coin", 10}, {"goblin ear", 3},
        {"healing herb", 5}, {"bread", 1},
    };

    // "Fighter monster" (no &) makes a fresh COPY of each monster for this
    // fight, so the list itself stays untouched. Lesson 10 is all about
    // copies; for now, notice that we want one here.
    for (Fighter monster : monsters) {
        std::cout << "\nA " << monster.name << " appears!\n";

        while (hero.hp > 0 && monster.hp > 0) {
            std::cout << "\n" << hero.name << " HP " << hero.hp << "  Gold " << gold
                      << "  Potions " << potions
                      << "   " << monster.name << " HP " << monster.hp << "\n";
            std::cout << "> ";

            std::string line;
            if (!std::getline(std::cin, line)) {
                std::cout << "\nNo more input. Goodbye.\n";
                return 0;
            }

            std::string verb = line;
            std::string object;
            std::size_t space = line.find(' ');
            if (space != std::string::npos) {
                verb = line.substr(0, space);
                object = line.substr(space + 1);
            }

            if (verb.empty()) {
                continue;
            } else if (verb == "look") {
                std::cout << "A " << monster.name << " with " << monster.hp
                          << " HP. It hits for " << monster.minDamage << "-"
                          << monster.maxDamage << ".\n";
                continue;
            } else if (verb == "inventory" || verb == "inv") {
                std::cout << "You carry " << inventory.size() << " item(s):\n";
                for (const auto& item : inventory) {
                    std::cout << "  - " << item << "\n";
                }
                continue;
            } else if (verb == "sell") {
                if (!prices.contains(object)) {
                    std::cout << "The shop does not buy '" << object << "'.\n";
                    continue;
                }
                auto found = std::find(inventory.begin(), inventory.end(), object);
                if (found == inventory.end()) {
                    std::cout << "You don't have a " << object << ".\n";
                    continue;
                }
                inventory.erase(found);
                gold += prices[object];
                std::cout << "Sold the " << object << " for " << prices[object]
                          << " gold.\n";
                continue;
            } else if (verb == "attack") {
                // Each fighter's damage range now comes from its own fields.
                int damage = rollDice(hero.minDamage, hero.maxDamage);
                monster.hp -= damage;
                std::cout << hero.name << " hits for " << damage << "!\n";
            } else if (verb == "use" && object == "potion") {
                if (potions == 0) {
                    std::cout << "You have no potions left!\n";
                    continue;   // no potion, no turn used
                }
                potions -= 1;
                hero.hp += potionHeal;
                std::cout << hero.name << " drinks a potion (+" << potionHeal << ").\n";
            } else if (verb == "use") {
                std::cout << "You can't use '" << object << "' here.\n";
                continue;
            } else {
                std::cout << "Unknown command '" << verb
                          << "'. Try: attack, use potion, sell <item>, look, inventory\n";
                continue;
            }

            if (monster.hp > 0) {
                int damage = rollDice(monster.minDamage, monster.maxDamage);
                hero.hp -= damage;
                std::cout << monster.name << " hits for " << damage << "!\n";
            }
        }

        if (hero.hp <= 0) {
            std::cout << "\nDefeat... the " << monster.name << " wins.\n";
            return 0;
        }

        std::string loot = lootTable[rollDice(0, 3)];
        inventory.push_back(loot);
        std::cout << "The " << monster.name << " drops a " << loot << ".\n";
    }

    std::cout << "\nVictory! " << hero.name << " cleared the arena with "
              << gold << " gold and " << inventory.size() << " items.\n";
    return 0;
}

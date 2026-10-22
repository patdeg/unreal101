// main.cpp - Lesson 02: Every value has a type
//
// Arena step: show the hero's stat card.

#include <iostream>
#include <string>   // std::string, C++'s type for text

int main() {
    // In JavaScript:  let hp = 100;      (hp can later become "banana")
    // In C++ you name the type first, and it never changes:
    //
    //   int         whole numbers          100, -5, 0
    //   double      numbers with decimals  0.25, 3.14
    //   bool        true or false
    //   std::string text                   "Ayla"
    //
    // The compiler checks every use against the type. That feels strict at
    // first, but it is a proofreader that checks your whole program.

    // "const" means: this value must never change after it is set.
    // If you try, the compiler stops you.
    const std::string heroName = "Ayla";

    int hp = 100;               // hit points: how much damage the hero can take
    int attack = 12;            // how hard the hero hits
    double critChance = 0.25;   // 0.25 means 25 percent
    bool hasShield = true;

    // Print the stat card. Chain several << arrows on one statement.
    // std::boolalpha makes a bool print as true/false instead of 1/0.
    std::cout << "=== " << heroName << " ===\n";
    std::cout << "HP:     " << hp << "\n";
    std::cout << "Attack: " << attack << "\n";
    std::cout << "Crit:   " << critChance * 100 << "%\n";
    std::cout << std::boolalpha << "Shield: " << hasShield << "\n";

    // Math works like JavaScript, with one trap. Both numbers are int,
    // so C++ throws away the decimals:
    std::cout << "7 / 2 = " << 7 / 2 << "   (not 3.5!)\n";
    std::cout << "7 / 2.0 = " << 7 / 2.0 << "\n";

    return 0;
}

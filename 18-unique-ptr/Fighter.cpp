// Fighter.cpp - HOW a Fighter works.
#include "Fighter.h"

#include "Dice.h"

#include <iostream>

std::string stanceName(Stance stance) {
    switch (stance) {
        case Stance::Normal:
            return "Normal";
        case Stance::Defending:
            return "Defending";
        case Stance::Stunned:
            return "Stunned";
    }
    return "?";
}

Fighter::Fighter(const std::string& startName, int startHp, int minDmg, int maxDmg) {
    name = startName;
    hp = startHp;
    maxHp = startHp;
    minDamage = minDmg;
    maxDamage = maxDmg;
}

int Fighter::rollDamage() const {
    return rollDice(minDamage, maxDamage);
}

// The only way to lower HP: defending halves it, and HP stops at 0.
int Fighter::takeDamage(int amount) {
    if (stance == Stance::Defending) {
        amount = amount / 2;
    }
    hp -= amount;
    if (hp < 0) {
        hp = 0;
    }
    return amount;
}

// The only way to raise HP: the fallen stay fallen, max is max.
void Fighter::heal(int amount) {
    if (!isAlive()) {
        return;
    }
    hp += amount;
    if (hp > maxHp) {
        hp = maxHp;
    }
}

void printStatus(const Fighter& fighter) {
    std::cout << "  " << fighter.getName() << " HP " << fighter.getHp() << "/"
              << fighter.getMaxHp() << " [" << stanceName(fighter.getStance()) << "]\n";
}

bool anyAlive(const std::vector<Fighter>& fighters) {
    for (const auto& fighter : fighters) {
        if (fighter.isAlive()) {
            return true;
        }
    }
    return false;
}

// A perfect roll (the attacker's maximum) is a crushing blow: +5 damage.
// "static" here means "only this file can see this function".
static int addCrushingBonus(int roll) {
    std::cout << "CRUSHING BLOW!\n";
    return roll + 5;
}

void strike(const Fighter& attacker, Fighter& defender) {
    if (defender.getStance() == Stance::Defending) {
        std::cout << defender.getName() << " blocks half the blow.\n";
    }
    int roll = attacker.rollDamage();
    if (roll == attacker.getMaxDamage()) {
        roll = addCrushingBonus(roll);
    }
    int damage = defender.takeDamage(roll);
    std::cout << attacker.getName() << " hits " << defender.getName() << " for "
              << damage << "!\n";

    if (damage >= 14 && defender.isAlive()) {
        defender.setStance(Stance::Stunned);
        std::cout << defender.getName() << " staggers. Stunned!\n";
    }
}

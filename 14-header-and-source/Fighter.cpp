// Fighter.cpp - HOW a Fighter works.
#include "Fighter.h"

#include "Dice.h"

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

// Outside the class, each member function's name gets the class name in
// front: Fighter::takeDamage means "the takeDamage that belongs to Fighter".
// :: is the same "belongs to" you have seen in std::string and Stance::Normal.
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

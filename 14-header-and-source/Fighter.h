// Fighter.h - what a Fighter IS: its stances, its data, its functions.
//
// Notice the quotes: #include "Fighter.h" (quotes) is for YOUR files,
// #include <string> (angle brackets) is for C++'s own files.
#pragma once

#include <string>

enum class Stance {
    Normal,
    Defending,
    Stunned,
};

std::string stanceName(Stance stance);

class Fighter {
public:
    // Declarations only: the bodies are in Fighter.cpp.
    Fighter(const std::string& startName, int startHp, int minDmg, int maxDmg);
    int rollDamage() const;
    int takeDamage(int amount);
    void heal(int amount);

    // Tiny one-line functions may keep their body right here in the
    // header. That is allowed and very common (Unreal does it all the time).
    const std::string& getName() const { return name; }
    int getHp() const { return hp; }
    int getMaxHp() const { return maxHp; }
    int getMaxDamage() const { return maxDamage; }
    bool isAlive() const { return hp > 0; }
    Stance getStance() const { return stance; }
    void setStance(Stance newStance) { stance = newStance; }
    Fighter* getTarget() const { return target; }
    void setTarget(Fighter* newTarget) { target = newTarget; }

private:
    std::string name;
    int hp = 0;
    int maxHp = 0;
    int minDamage = 0;
    int maxDamage = 0;
    Stance stance = Stance::Normal;
    Fighter* target = nullptr;
};

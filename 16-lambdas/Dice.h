// Dice.h - the "menu" for the dice machine.
//
// A header DECLARES: it lists what exists, with its types, but not how it
// works. Any .cpp file that wants to roll dice writes #include "Dice.h".
//
// #pragma once means "paste this file at most once per .cpp", even if it
// gets #included twice by accident (directly, and again through another
// header). Put it at the top of every header you write.
#pragma once

// A declaration: name, inputs, output, and a semicolon instead of a body.
// It is a promise: "a function like this exists somewhere".
int rollDice(int min, int max);

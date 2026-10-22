// break-it.cpp - read a name with >> instead of getline.
// Run it and type a name with a space in it, like:  Sir Gob
// Then watch what happens to the NEXT question.
#include <iostream>
#include <string>

int main() {
    std::string name;
    std::cout << "Name your hero: ";
    std::cin >> name;          // >> stops at the first space!

    std::string command;
    std::cout << "First command: ";
    std::cin >> command;       // ...so this gets whatever was left over

    std::cout << "\nHero name: [" << name << "]\n";
    std::cout << "Command:   [" << command << "]\n";
    return 0;
}

// break-it.cpp - ONE equals sign instead of two. This compiles!
// Compile it with warnings on, then run it and press h. What happens?
#include <iostream>

int main() {
    char choice = ' ';
    std::cout << "(a)ttack or (h)eal? ";
    std::cin >> choice;

    if (choice = 'a') {
        std::cout << "You attack!\n";
    } else {
        std::cout << "You heal.\n";
    }
    return 0;
}

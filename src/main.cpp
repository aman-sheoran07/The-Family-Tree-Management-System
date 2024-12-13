#include "ui/FamilyTreeUI.hpp"
#include <iostream>

int main() {
    try {
        FamilyTreeUI ui;
        ui.start();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
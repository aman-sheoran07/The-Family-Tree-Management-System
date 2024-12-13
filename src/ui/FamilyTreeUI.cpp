#include "ui/FamilyTreeUI.hpp"
#include <iostream>
#include <limits>

FamilyTreeUI::FamilyTreeUI() 
    : tree(std::make_unique<FamilyTree>("family_tree.db")) {}

void FamilyTreeUI::start() {
    while (true) {
        showMainMenu();
    }
}

void FamilyTreeUI::showMainMenu() {
    clearScreen();
    std::cout << "\n=== Family Tree Management System ===\n"
              << "1. Person Operations\n"
              << "2. Relationship Operations\n"
              << "3. Tree Queries\n"
              << "4. Exit\n"
              << "Choose an option: ";

    switch (getIntInput("")) {
        case 1:
            handlePersonOperations();
            break;
        case 2:
            handleRelationshipOperations();
            break;
        case 3:
            handleTreeQueries();
            break;
        case 4:
            std::cout << "Goodbye!\n";
            exit(0);
        default:
            displayError("Invalid option!");
    }
}

void FamilyTreeUI::handlePersonOperations() {
    while (true) {
        clearScreen();
        std::cout << "\n=== Person Operations ===\n"
                  << "1. Add New Person\n"
                  << "2. Update Person\n"
                  << "3. Delete Person\n"
                  << "4. Search Person\n"
                  << "5. Back to Main Menu\n"
                  << "Choose an option: ";

        switch (getIntInput("")) {
            case 1:
                addPerson();
                break;
            case 2:
                updatePerson();
                break;
            case 3:
                deletePerson();
                break;
            case 4:
                searchPerson();
                break;
            case 5:
                return;
            default:
                displayError("Invalid option!");
        }
    }
}

void FamilyTreeUI::addPerson() {
    clearScreen();
    std::cout << "\n=== Add New Person ===\n";
    
    try {
        Person person = inputPersonDetails();
        if (tree->addPerson(person)) {
            std::cout << "\nPerson added successfully!\n";
        } else {
            displayError("Failed to add person.");
        }
    } catch (const std::exception& e) {
        displayError(e.what());
    }
    waitForEnter();
}

Person FamilyTreeUI::inputPersonDetails() {
    std::string id = getInput("Enter ID: ");
    std::string firstName = getInput("Enter First Name: ");
    std::string lastName = getInput("Enter Last Name: ");
    
    std::string gender;
    do {
        gender = getInput("Enter Gender (M/F/O): ");
    } while (gender != "M" && gender != "F" && gender != "O");
    
    std::string dob = getInput("Enter Date of Birth (YYYY-MM-DD): ");
    
    return Person(id, firstName, lastName, gender, dob);
}

void FamilyTreeUI::handleRelationshipOperations() {
    while (true) {
        clearScreen();
        std::cout << "\n=== Relationship Operations ===\n"
                  << "1. Add Relationship\n"
                  << "2. View Relationships\n"
                  << "3. Remove Relationship\n"
                  << "4. Back to Main Menu\n"
                  << "Choose an option: ";

        switch (getIntInput("")) {
            case 1:
                addRelationship();
                break;
            case 2:
                viewRelationships();
                break;
            case 3:
                removeRelationship();
                break;
            case 4:
                return;
            default:
                displayError("Invalid option!");
        }
    }
}

void FamilyTreeUI::addRelationship() {
    clearScreen();
    std::cout << "\n=== Add Relationship ===\n";
    
    std::string person1Id = getInput("Enter ID of first person: ");
    std::string person2Id = getInput("Enter ID of second person: ");
    
    std::cout << "\nRelationship Types:\n"
              << "1. Parent-Child\n"
              << "2. Spouse\n"
              << "3. Sibling\n";
    
    int choice = getIntInput("Choose relationship type: ");
    RelationType type;
    
    switch (choice) {
        case 1:
            type = RelationType::PARENT_CHILD;
            break;
        case 2:
            type = RelationType::SPOUSE;
            break;
        case 3:
            type = RelationType::SIBLING;
            break;
        default:
            displayError("Invalid relationship type!");
            return;
    }
    
    try {
        if (tree->addRelationship(person1Id, person2Id, type)) {
            std::cout << "\nRelationship added successfully!\n";
        } else {
            displayError("Failed to add relationship.");
        }
    } catch (const std::exception& e) {
        displayError(e.what());
    }
    waitForEnter();
}

// Utility methods
std::string FamilyTreeUI::getInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

int FamilyTreeUI::getIntInput(const std::string& prompt) {
    while (true) {
        try {
            std::string input = getInput(prompt);
            return std::stoi(input);
        } catch (...) {
            std::cout << "Please enter a valid number: ";
        }
    }
}

void FamilyTreeUI::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void FamilyTreeUI::waitForEnter() {
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void FamilyTreeUI::displayError(const std::string& message) {
    std::cout << "\nError: " << message << std::endl;
    waitForEnter();
}





void FamilyTreeUI::handleTreeQueries() {
    while (true) {
        clearScreen();
        std::cout << "\n=== Tree Queries ===\n"
                  << "1. View Ancestors\n"
                  << "2. View Descendants\n"
                  << "3. View Family Members\n"
                  << "4. Back to Main Menu\n"
                  << "Choose an option: ";

        switch (getIntInput("")) {
            case 1:
                viewAncestors();
                break;
            case 2:
                viewDescendants();
                break;
            case 3:
                viewFamilyMembers();
                break;
            case 4:
                return;
            default:
                displayError("Invalid option!");
        }
    }
}

void FamilyTreeUI::updatePerson() {
    clearScreen();
    std::cout << "\n=== Update Person ===\n";
    
    std::string id = getInput("Enter ID of person to update: ");
    auto person = tree->getPerson(id);
    
    if (!person) {
        displayError("Person not found!");
        return;
    }

    displayPerson(*person);
    std::cout << "\nEnter new details (press Enter to keep current value):\n";

    std::string input;
    
    input = getInput("First Name [" + person->getFirstName() + "]: ");
    if (!input.empty()) person->setFirstName(input);
    
    input = getInput("Last Name [" + person->getLastName() + "]: ");
    if (!input.empty()) person->setLastName(input);
    
    if (tree->updatePerson(*person)) {
        std::cout << "\nPerson updated successfully!\n";
    } else {
        displayError("Failed to update person.");
    }
    waitForEnter();
}

void FamilyTreeUI::deletePerson() {
    clearScreen();
    std::cout << "\n=== Delete Person ===\n";
    
    std::string id = getInput("Enter ID of person to delete: ");
    auto person = tree->getPerson(id);
    
    if (!person) {
        displayError("Person not found!");
        return;
    }

    displayPerson(*person);
    std::string confirm = getInput("\nAre you sure you want to delete this person? (y/N): ");
    
    if (confirm == "y" || confirm == "Y") {
        if (tree->deletePerson(id)) {
            std::cout << "\nPerson deleted successfully!\n";
        } else {
            displayError("Failed to delete person.");
        }
    }
    waitForEnter();
}

void FamilyTreeUI::searchPerson() {
    clearScreen();
    std::cout << "\n=== Search Person ===\n";
    
    std::string name = getInput("Enter name to search: ");
    auto results = tree->searchByName(name);
    
    if (results.empty()) {
        std::cout << "\nNo matching persons found.\n";
    } else {
        std::cout << "\nFound " << results.size() << " matching persons:\n";
        for (const auto& person : results) {
            displayPerson(person);
            std::cout << "------------------------\n";
        }
    }
    waitForEnter();
}

void FamilyTreeUI::viewRelationships() {
    clearScreen();
    std::cout << "\n=== View Relationships ===\n";
    
    std::string id = getInput("Enter person ID: ");
    auto person = tree->getPerson(id);
    
    if (!person) {
        displayError("Person not found!");
        return;
    }

    auto parents = tree->getParents(id);
    auto children = tree->getChildren(id);
    auto spouse = tree->getSpouse(id);

    std::cout << "\nRelationships for " << person->getFullName() << ":\n\n";
    
    std::cout << "Parents:\n";
    for (const auto& parent : parents) {
        std::cout << "- " << parent.getFullName() << "\n";
    }

    std::cout << "\nChildren:\n";
    for (const auto& child : children) {
        std::cout << "- " << child.getFullName() << "\n";
    }

    std::cout << "\nSpouse: ";
    if (spouse) {
        std::cout << spouse->getFullName() << "\n";
    } else {
        std::cout << "None\n";
    }
    
    waitForEnter();
}

void FamilyTreeUI::removeRelationship() {
    clearScreen();
    std::cout << "\n=== Remove Relationship ===\n";
    
    std::string id = getInput("Enter relationship ID: ");
    
    std::string confirm = getInput("\nAre you sure you want to remove this relationship? (y/N): ");
    
    if (confirm == "y" || confirm == "Y") {
        if (tree->removeRelationship(id)) {
            std::cout << "\nRelationship removed successfully!\n";
        } else {
            displayError("Failed to remove relationship.");
        }
    }
    waitForEnter();
}

void FamilyTreeUI::viewAncestors() {
    clearScreen();
    std::cout << "\n=== View Ancestors ===\n";
    
    std::string id = getInput("Enter person ID: ");
    int generations = getIntInput("Enter number of generations (-1 for all): ");
    
    auto ancestors = tree->getAncestors(id, generations);
    
    if (ancestors.empty()) {
        std::cout << "\nNo ancestors found.\n";
    } else {
        std::cout << "\nAncestors:\n";
        for (const auto& ancestor : ancestors) {
            displayPerson(ancestor);
            std::cout << "------------------------\n";
        }
    }
    waitForEnter();
}

void FamilyTreeUI::viewDescendants() {
    clearScreen();
    std::cout << "\n=== View Descendants ===\n";
    
    std::string id = getInput("Enter person ID: ");
    int generations = getIntInput("Enter number of generations (-1 for all): ");
    
    auto descendants = tree->getDescendants(id, generations);
    
    if (descendants.empty()) {
        std::cout << "\nNo descendants found.\n";
    } else {
        std::cout << "\nDescendants:\n";
        for (const auto& descendant : descendants) {
            displayPerson(descendant);
            std::cout << "------------------------\n";
        }
    }
    waitForEnter();
}

void FamilyTreeUI::viewFamilyMembers() {
    clearScreen();
    std::cout << "\n=== View Family Members ===\n";
    
    std::string id = getInput("Enter person ID: ");
    auto person = tree->getPerson(id);
    
    if (!person) {
        displayError("Person not found!");
        return;
    }

    displayPerson(*person);
    std::cout << "\nImmediate Family Members:\n";
    
    // Show parents
    auto parents = tree->getParents(id);
    std::cout << "\nParents:\n";
    for (const auto& parent : parents) {
        std::cout << "- " << parent.getFullName() << "\n";
    }

    // Show siblings
    auto siblings = tree->getSiblings(id);
    std::cout << "\nSiblings:\n";
    for (const auto& sibling : siblings) {
        std::cout << "- " << sibling.getFullName() << "\n";
    }

    // Show spouse
    auto spouse = tree->getSpouse(id);
    std::cout << "\nSpouse: ";
    if (spouse) {
        std::cout << spouse->getFullName() << "\n";
    } else {
        std::cout << "None\n";
    }

    // Show children
    auto children = tree->getChildren(id);
    std::cout << "\nChildren:\n";
    for (const auto& child : children) {
        std::cout << "- " << child.getFullName() << "\n";
    }
    
    waitForEnter();
}

void FamilyTreeUI::displayPerson(const Person& person) {
    std::cout << "\nID: " << person.getId()
              << "\nName: " << person.getFullName()
              << "\nGender: " << person.getGender()
              << "\nDate of Birth: " << person.getDateOfBirth();
    
    if (person.isDeceased()) {
        std::cout << "\nDate of Death: " << person.getDateOfDeath();
    }
    
    if (!person.getBirthPlace().empty()) {
        std::cout << "\nBirth Place: " << person.getBirthPlace();
    }
    
    if (!person.getDeathPlace().empty()) {
        std::cout << "\nDeath Place: " << person.getDeathPlace();
    }
    
    std::cout << "\n";
}
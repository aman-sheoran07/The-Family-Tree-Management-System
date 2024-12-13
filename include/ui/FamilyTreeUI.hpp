#ifndef FAMILY_TREE_UI_HPP
#define FAMILY_TREE_UI_HPP

#include "models/FamilyTree.hpp"
#include <memory>

class FamilyTreeUI {
private:
    std::unique_ptr<FamilyTree> tree;
    
    // Menu methods
    void showMainMenu();
    void handlePersonOperations();
    void handleRelationshipOperations();
    void handleTreeQueries();
    
    // Person operations
    void addPerson();
    void updatePerson();
    void deletePerson();
    void searchPerson();
    void displayPerson(const Person& person);
    
    // Relationship operations
    void addRelationship();
    void viewRelationships();
    void removeRelationship();
    
    // Tree queries
    void viewAncestors();
    void viewDescendants();
    void viewFamilyMembers();
    
    // Utility methods
    std::string getInput(const std::string& prompt);
    int getIntInput(const std::string& prompt);
    Person inputPersonDetails();
    void clearScreen();
    void waitForEnter();
    void displayError(const std::string& message);

public:
    FamilyTreeUI();
    void start();
};

#endif // FAMILY_TREE_UI_HPP
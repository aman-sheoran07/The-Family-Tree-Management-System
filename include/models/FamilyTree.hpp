#ifndef FAMILY_TREE_HPP
#define FAMILY_TREE_HPP

#include "models/Person.hpp"
#include "models/Relationship.hpp"
#include "database/DatabaseManager.hpp"
#include <memory>
#include <vector>
#include <map>
#include <optional>

class FamilyTree {
private:
    std::unique_ptr<DatabaseManager> dbManager;
    std::string rootPersonId;  // ID of the main person in the family tree

public:
    explicit FamilyTree(const std::string& dbPath);

    // Person management
    bool addPerson(const Person& person);
    bool updatePerson(const Person& person);
    bool deletePerson(const std::string& personId);
    std::optional<Person> getPerson(const std::string& personId);
    
    // Relationship management
    bool addRelationship(const std::string& person1Id, 
                        const std::string& person2Id, 
                        RelationType type);
    bool removeRelationship(const std::string& relationshipId);
    
    // Tree navigation
    std::vector<Person> getParents(const std::string& personId);
    std::vector<Person> getChildren(const std::string& personId);
    std::optional<Person> getSpouse(const std::string& personId);
    std::vector<Person> getSiblings(const std::string& personId);
    
    // Tree queries
    std::vector<Person> getAncestors(const std::string& personId, int generations = -1);
    std::vector<Person> getDescendants(const std::string& personId, int generations = -1);
    std::vector<Person> findCommonAncestors(const std::string& person1Id, 
                                          const std::string& person2Id);
    int calculateGenerationGap(const std::string& person1Id, 
                             const std::string& person2Id);
    
    // Search functionality
    std::vector<Person> searchByName(const std::string& name);
    std::vector<Person> searchByDateRange(const std::string& startDate, 
                                        const std::string& endDate);

    // Tree validation
    bool validateRelationship(const std::string& person1Id, 
                            const std::string& person2Id, 
                            RelationType type);
    void setRootPerson(const std::string& personId);
    std::string getRootPerson() const;

private:
    // Helper methods
    bool isAncestor(const std::string& ancestorId, const std::string& descendantId);
    bool isSibling(const std::string& person1Id, const std::string& person2Id);
    std::vector<Person> getRelatives(const std::string& personId, 
                                   RelationType type);
    void getAncestorsRecursive(const std::string& personId, 
                              std::vector<Person>& ancestors, 
                              int currentGen, 
                              int maxGen);
    void getDescendantsRecursive(const std::string& personId, 
                                std::vector<Person>& descendants, 
                                int currentGen, 
                                int maxGen);
};

#endif // FAMILY_TREE_HPP
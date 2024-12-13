#include "models/FamilyTree.hpp"
#include <algorithm>
#include <climits>
#include <set>

// Constructor
FamilyTree::FamilyTree(const std::string& dbPath)
    : dbManager(std::make_unique<DatabaseManager>(dbPath)) {}

// Person management
bool FamilyTree::addPerson(const Person& person) {
    return dbManager->addPerson(person);
}

bool FamilyTree::updatePerson(const Person& person) {
    return dbManager->updatePerson(person);
}

bool FamilyTree::deletePerson(const std::string& personId) {
    // First check if person exists
    auto person = dbManager->getPerson(personId);
    if (!person) {
        return false;
    }

    // Begin transaction
    dbManager->beginTransaction();
    try {
        // Delete all relationships
        auto relationships = dbManager->getRelationshipsForPerson(personId);
        for (const auto& rel : relationships) {
            dbManager->deleteRelationship(rel.getId());
        }

        // Delete the person
        bool success = dbManager->deletePerson(personId);
        if (success) {
            dbManager->commit();
            return true;
        }
        dbManager->rollback();
        return false;
    }
    catch (...) {
        dbManager->rollback();
        throw;
    }
}

std::optional<Person> FamilyTree::getPerson(const std::string& personId) {
    return dbManager->getPerson(personId);
}

// Relationship management
bool FamilyTree::addRelationship(const std::string& person1Id,
                                const std::string& person2Id,
                                RelationType type) {
    // Validate both persons exist
    if (!dbManager->getPerson(person1Id) || !dbManager->getPerson(person2Id)) {
        return false;
    }

    // Validate relationship
    if (!validateRelationship(person1Id, person2Id, type)) {
        return false;
    }

    // Generate unique ID for relationship
    std::string relationshipId = person1Id + "_" + person2Id + "_" +
                                Relationship::relationTypeToString(type);

    Relationship newRelationship(relationshipId, person1Id, person2Id, type);
    return dbManager->addRelationship(newRelationship);
}

bool FamilyTree::removeRelationship(const std::string& relationshipId) {
    return dbManager->deleteRelationship(relationshipId);
}

// Tree navigation
std::vector<Person> FamilyTree::getParents(const std::string& personId) {
    std::vector<Person> parents;
    auto relationships = dbManager->getRelationshipsForPerson(personId);

    for (const auto& rel : relationships) {
        if (rel.getType() == RelationType::PARENT_CHILD &&
            rel.getPerson2Id() == personId) {
            auto parent = dbManager->getPerson(rel.getPerson1Id());
            if (parent) {
                parents.push_back(*parent);
            }
        }
    }
    return parents;
}

std::vector<Person> FamilyTree::getChildren(const std::string& personId) {
    std::vector<Person> children;
    auto relationships = dbManager->getRelationshipsForPerson(personId);

    for (const auto& rel : relationships) {
        if (rel.getType() == RelationType::PARENT_CHILD &&
            rel.getPerson1Id() == personId) {
            auto child = dbManager->getPerson(rel.getPerson2Id());
            if (child) {
                children.push_back(*child);
            }
        }
    }
    return children;
}

std::optional<Person> FamilyTree::getSpouse(const std::string& personId) {
    auto relationships = dbManager->getRelationshipsForPerson(personId);
    for (const auto& rel : relationships) {
        if (rel.getType() == RelationType::SPOUSE && rel.getEndDate().empty()) {
            std::string spouseId = (rel.getPerson1Id() == personId) ?
                                  rel.getPerson2Id() : rel.getPerson1Id();
            return dbManager->getPerson(spouseId);
        }
    }
    return std::nullopt;
}

std::vector<Person> FamilyTree::getSiblings(const std::string& personId) {
    std::vector<Person> siblings;
    auto parents = getParents(personId);

    for (const auto& parent : parents) {
        auto children = getChildren(parent.getId());
        for (const auto& child : children) {
            if (child.getId() != personId &&
                std::find_if(siblings.begin(), siblings.end(),
                    [&](const Person& p) { return p.getId() == child.getId(); }) == siblings.end()) {
                siblings.push_back(child);
            }
        }
    }
    return siblings;
}

// Tree queries
std::vector<Person> FamilyTree::getAncestors(const std::string& personId, int generations) {
    std::vector<Person> ancestors;
    getAncestorsRecursive(personId, ancestors, 0, generations);
    return ancestors;
}

void FamilyTree::getAncestorsRecursive(const std::string& personId,
                                     std::vector<Person>& ancestors,
                                     int currentGen,
                                     int maxGen) {
    if (maxGen != -1 && currentGen >= maxGen) {
        return;
    }

    auto parents = getParents(personId);
    for (const auto& parent : parents) {
        ancestors.push_back(parent);
        getAncestorsRecursive(parent.getId(), ancestors, currentGen + 1, maxGen);
    }
}

std::vector<Person> FamilyTree::getDescendants(const std::string& personId, int generations) {
    std::vector<Person> descendants;
    getDescendantsRecursive(personId, descendants, 0, generations);
    return descendants;
}

void FamilyTree::getDescendantsRecursive(const std::string& personId,
                                       std::vector<Person>& descendants,
                                       int currentGen,
                                       int maxGen) {
    if (maxGen != -1 && currentGen >= maxGen) {
        return;
    }

    auto children = getChildren(personId);
    for (const auto& child : children) {
        descendants.push_back(child);
        getDescendantsRecursive(child.getId(), descendants, currentGen + 1, maxGen);
    }
}

std::vector<Person> FamilyTree::findCommonAncestors(const std::string& person1Id,
                                                   const std::string& person2Id) {
    auto ancestors1 = getAncestors(person1Id);
    auto ancestors2 = getAncestors(person2Id);
    std::vector<Person> common;

    for (const auto& ancestor : ancestors1) {
        if (std::find_if(ancestors2.begin(), ancestors2.end(),
                [&](const Person& p) { return p.getId() == ancestor.getId(); }) != ancestors2.end()) {
            common.push_back(ancestor);
        }
    }
    return common;
}

int FamilyTree::calculateGenerationGap(const std::string& person1Id,
                                     const std::string& person2Id) {
    auto commonAncestors = findCommonAncestors(person1Id, person2Id);
    if (commonAncestors.empty()) {
        return -1;  // No common ancestors
    }

    int minGap = INT_MAX;
    for (const auto& ancestor : commonAncestors) {
        int gen1 = 0, gen2 = 0;
        auto current1 = person1Id;
        auto current2 = person2Id;

        // Count generations from person1 to ancestor
        while (current1 != ancestor.getId()) {
            auto parents = getParents(current1);
            if (parents.empty()) break;
            current1 = parents[0].getId();
            gen1++;
        }

        // Count generations from person2 to ancestor
        while (current2 != ancestor.getId()) {
            auto parents = getParents(current2);
            if (parents.empty()) break;
            current2 = parents[0].getId();
            gen2++;
        }

        minGap = std::min(minGap, gen1 + gen2);
    }

    return minGap;
}

std::vector<Person> FamilyTree::searchByName(const std::string& name) {
    return dbManager->searchPeople(name);
}

std::vector<Person> FamilyTree::searchByDateRange(const std::string& startDate,
                                                const std::string& endDate) {
    // Implementation needed in DatabaseManager
    return std::vector<Person>();
}

bool FamilyTree::validateRelationship(const std::string& person1Id,
                                    const std::string& person2Id,
                                    RelationType type) {
    if (person1Id == person2Id) {
        return false;
    }

    switch (type) {
        case RelationType::PARENT_CHILD:
            return !isAncestor(person2Id, person1Id);

        case RelationType::SPOUSE: {
            auto spouse1 = getSpouse(person1Id);
            auto spouse2 = getSpouse(person2Id);
            return !spouse1 && !spouse2;
        }

        case RelationType::SIBLING:
            return true;  // Siblings can be added freely

        default:
            return false;
    }
}

void FamilyTree::setRootPerson(const std::string& personId) {
    if (getPerson(personId)) {
        rootPersonId = personId;
    } else {
        throw std::invalid_argument("Person does not exist");
    }
}

std::string FamilyTree::getRootPerson() const {
    return rootPersonId;
}

bool FamilyTree::isAncestor(const std::string& ancestorId, const std::string& descendantId) {
    std::vector<Person> ancestors = getAncestors(descendantId);
    return std::find_if(ancestors.begin(), ancestors.end(),
        [&](const Person& p) { return p.getId() == ancestorId; }) != ancestors.end();
}

bool FamilyTree::isSibling(const std::string& person1Id, const std::string& person2Id) {
    auto siblings = getSiblings(person1Id);
    return std::find_if(siblings.begin(), siblings.end(),
        [&](const Person& p) { return p.getId() == person2Id; }) != siblings.end();
}
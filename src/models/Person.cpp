#include "models/Person.hpp"
#include <algorithm>

// Constructor
Person::Person(const std::string& id,
               const std::string& firstName,
               const std::string& lastName,
               const std::string& gender,
               const std::string& dateOfBirth)
    : id(id),
      firstName(firstName),
      lastName(lastName),
      gender(gender),
      dateOfBirth(dateOfBirth) {
    
    // Validate gender
    if (gender != "M" && gender != "F" && gender != "O") {
        throw std::invalid_argument("Gender must be 'M', 'F', or 'O'");
    }
}

// Setters implementation
void Person::setFirstName(const std::string& name) {
    if (name.empty()) {
        throw std::invalid_argument("First name cannot be empty");
    }
    firstName = name;
}

void Person::setLastName(const std::string& name) {
    if (name.empty()) {
        throw std::invalid_argument("Last name cannot be empty");
    }
    lastName = name;
}

void Person::setDateOfDeath(const std::string& date) {
    if (!date.empty() && date < dateOfBirth) {
        throw std::invalid_argument("Date of death cannot be before date of birth");
    }
    dateOfDeath = date;
}

void Person::setBirthPlace(const std::string& place) {
    birthPlace = place;
}

void Person::setDeathPlace(const std::string& place) {
    if (!isDeceased()) {
        throw std::logic_error("Cannot set death place for living person");
    }
    deathPlace = place;
}

void Person::setSpouse(const std::string& id) {
    if (id == this->id) {
        throw std::invalid_argument("Person cannot be their own spouse");
    }
    spouseId = id;
}

// Relationship management implementation
void Person::addParent(const std::string& parentId) {
    if (parentId == id) {
        throw std::invalid_argument("Person cannot be their own parent");
    }
    if (getParentCount() >= 2) {
        throw std::logic_error("Person cannot have more than two parents");
    }
    if (!isParent(parentId)) {
        parentIds.push_back(parentId);
    }
}

void Person::addChild(const std::string& childId) {
    if (childId == id) {
        throw std::invalid_argument("Person cannot be their own child");
    }
    if (!isChild(childId)) {
        childrenIds.push_back(childId);
    }
}

void Person::removeParent(const std::string& parentId) {
    auto it = std::find(parentIds.begin(), parentIds.end(), parentId);
    if (it != parentIds.end()) {
        parentIds.erase(it);
    }
}

void Person::removeChild(const std::string& childId) {
    auto it = std::find(childrenIds.begin(), childrenIds.end(), childId);
    if (it != childrenIds.end()) {
        childrenIds.erase(it);
    }
}

void Person::removeSpouse() {
    spouseId.clear();
}

// Validation methods implementation
bool Person::isParent(const std::string& parentId) const {
    return std::find(parentIds.begin(), parentIds.end(), parentId) != parentIds.end();
}

bool Person::isChild(const std::string& childId) const {
    return std::find(childrenIds.begin(), childrenIds.end(), childId) != childrenIds.end();
}


#include "models/Relationship.hpp"

Relationship::Relationship(const std::string& id,
                         const std::string& person1Id,
                         const std::string& person2Id,
                         RelationType type)
    : id(id),
      person1Id(person1Id),
      person2Id(person2Id),
      type(type) {
    
    // Validate IDs
    if (person1Id.empty() || person2Id.empty()) {
        throw std::invalid_argument("Person IDs cannot be empty");
    }

    if (person1Id == person2Id) {
        throw std::invalid_argument("A relationship cannot be created between the same person");
    }

    if (!isValidRelationType(type)) {
        throw std::invalid_argument("Invalid relationship type");
    }
}

void Relationship::setStartDate(const std::string& date) {
    if (type != RelationType::SPOUSE && !date.empty()) {
        throw std::invalid_argument("Start date can only be set for spouse relationships");
    }
    startDate = date;
}

void Relationship::setEndDate(const std::string& date) {
    if (type != RelationType::SPOUSE && !date.empty()) {
        throw std::invalid_argument("End date can only be set for spouse relationships");
    }
    if (!startDate.empty() && !date.empty() && date < startDate) {
        throw std::invalid_argument("End date cannot be before start date");
    }
    endDate = date;
}

bool Relationship::isActive() const {
    // For spouse relationships, check if there's no end date
    if (type == RelationType::SPOUSE) {
        return endDate.empty();
    }
    // Parent-child and sibling relationships are always active
    return true;
}

bool Relationship::involves(const std::string& personId) const {
    return (person1Id == personId || person2Id == personId);
}

std::string Relationship::getOtherPerson(const std::string& personId) const {
    if (!involves(personId)) {
        throw std::invalid_argument("Given person is not part of this relationship");
    }
    return (person1Id == personId) ? person2Id : person1Id;
}

bool Relationship::isValidRelationType(RelationType type) {
    switch (type) {
        case RelationType::PARENT_CHILD:
        case RelationType::SPOUSE:
        case RelationType::SIBLING:
            return true;
        default:
            return false;
    }
}

std::string Relationship::relationTypeToString(RelationType type) {
    switch (type) {
        case RelationType::PARENT_CHILD:
            return "Parent-Child";
        case RelationType::SPOUSE:
            return "Spouse";
        case RelationType::SIBLING:
            return "Sibling";
        default:
            return "Unknown";
    }
}
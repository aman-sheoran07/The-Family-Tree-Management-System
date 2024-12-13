#ifndef RELATIONSHIP_HPP
#define RELATIONSHIP_HPP

#include <string>
#include <stdexcept>

enum class RelationType {
    PARENT_CHILD,
    SPOUSE,
    SIBLING
};

class Relationship {
private:
    std::string id;
    std::string person1Id;  // Source person
    std::string person2Id;  // Target person
    RelationType type;
    std::string startDate;  // For marriage date in spouse relationships
    std::string endDate;    // For divorce/death date in spouse relationships

public:
    // Constructor
    Relationship(const std::string& id,
                const std::string& person1Id,
                const std::string& person2Id,
                RelationType type);

    // Getters
    std::string getId() const { return id; }
    std::string getPerson1Id() const { return person1Id; }
    std::string getPerson2Id() const { return person2Id; }
    RelationType getType() const { return type; }
    std::string getStartDate() const { return startDate; }
    std::string getEndDate() const { return endDate; }

    // Setters
    void setStartDate(const std::string& date);
    void setEndDate(const std::string& date);

    // Utility methods
    bool isActive() const;
    bool involves(const std::string& personId) const;
    std::string getOtherPerson(const std::string& personId) const;

    // Static methods for relationship validation
    static bool isValidRelationType(RelationType type);
    static std::string relationTypeToString(RelationType type);
};

#endif // RELATIONSHIP_HPP
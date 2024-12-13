#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

class Person {
private:
    std::string id;
    std::string firstName;
    std::string lastName;
    std::string gender;
    std::string dateOfBirth;
    std::string dateOfDeath;
    std::string birthPlace;
    std::string deathPlace;
    std::vector<std::string> parentIds;
    std::vector<std::string> childrenIds;
    std::string spouseId;

public:
    // Constructor
    Person(const std::string& id,
          const std::string& firstName,
          const std::string& lastName,
          const std::string& gender,
          const std::string& dateOfBirth);

    // Getters
    std::string getId() const { return id; }
    std::string getFirstName() const { return firstName; }
    std::string getLastName() const { return lastName; }
    std::string getFullName() const { return firstName + " " + lastName; }
    std::string getGender() const { return gender; }
    std::string getDateOfBirth() const { return dateOfBirth; }
    std::string getDateOfDeath() const { return dateOfDeath; }
    std::string getBirthPlace() const { return birthPlace; }
    std::string getDeathPlace() const { return deathPlace; }
    std::string getSpouseId() const { return spouseId; }
    const std::vector<std::string>& getParentIds() const { return parentIds; }
    const std::vector<std::string>& getChildrenIds() const { return childrenIds; }

    // Setters
    void setFirstName(const std::string& name);
    void setLastName(const std::string& name);
    void setDateOfDeath(const std::string& date);
    void setBirthPlace(const std::string& place);
    void setDeathPlace(const std::string& place);
    void setSpouse(const std::string& id);

    // Relationship management
    void addParent(const std::string& parentId);
    void addChild(const std::string& childId);
    void removeParent(const std::string& parentId);
    void removeChild(const std::string& childId);
    void removeSpouse();

    // Utility methods
    bool isDeceased() const { return !dateOfDeath.empty(); }
    bool hasParents() const { return !parentIds.empty(); }
    bool hasChildren() const { return !childrenIds.empty(); }
    bool hasSpouse() const { return !spouseId.empty(); }
    int getParentCount() const { return parentIds.size(); }
    int getChildCount() const { return childrenIds.size(); }

    // Validation methods
    bool isParent(const std::string& parentId) const;
    bool isChild(const std::string& childId) const;
};

#endif // PERSON_HPP
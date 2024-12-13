#ifndef DATABASE_MANAGER_HPP
#define DATABASE_MANAGER_HPP

#include "database/SQLiteConnector.hpp"
#include "models/Person.hpp"
#include "models/Relationship.hpp"
#include <memory>
#include <optional>

class DatabaseManager {
private:
    std::unique_ptr<SQLiteConnector> connector;

public:
    explicit DatabaseManager(const std::string& dbPath);

    // Person operations
    bool addPerson(const Person& person);
    bool updatePerson(const Person& person);
    bool deletePerson(const std::string& personId);
    std::optional<Person> getPerson(const std::string& personId);
    std::vector<Person> getAllPeople();
    std::vector<Person> searchPeople(const std::string& searchTerm);

    // Relationship operations
    bool addRelationship(const Relationship& relationship);
    bool updateRelationship(const Relationship& relationship);
    bool deleteRelationship(const std::string& relationshipId);
    std::optional<Relationship> getRelationship(const std::string& relationshipId);
    std::vector<Relationship> getRelationshipsForPerson(const std::string& personId);

    // Transaction management
    void beginTransaction();
    void commit();
    void rollback();

private:
    // Helper methods
    Person createPersonFromRow(const std::map<std::string, std::string>& row);
    Relationship createRelationshipFromRow(const std::map<std::string, std::string>& row);
};

#endif // DATABASE_MANAGER_HPP
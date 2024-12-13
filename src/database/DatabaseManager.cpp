#include "database/DatabaseManager.hpp"
#include <sstream>

DatabaseManager::DatabaseManager(const std::string& dbPath)
    : connector(std::make_unique<SQLiteConnector>(dbPath)) {}

bool DatabaseManager::addPerson(const Person& person) {
    const std::string sql = R"(
        INSERT INTO Person (
            person_id, first_name, last_name, gender, 
            date_of_birth, date_of_death, birth_place, death_place
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?)
    )";

    std::vector<std::string> params = {
        person.getId(),
        person.getFirstName(),
        person.getLastName(),
        person.getGender(),
        person.getDateOfBirth(),
        person.getDateOfDeath(),
        person.getBirthPlace(),
        person.getDeathPlace()
    };

    return connector->executeCommand(sql, params);
}

std::optional<Person> DatabaseManager::getPerson(const std::string& personId) {
    const std::string sql = "SELECT * FROM Person WHERE person_id = ?";
    auto results = connector->executeQuery(sql, {personId});

    if (results.empty()) {
        return std::nullopt;
    }

    return createPersonFromRow(results[0]);
}

Person DatabaseManager::createPersonFromRow(const std::map<std::string, std::string>& row) {
    Person person(
        row.at("person_id"),
        row.at("first_name"),
        row.at("last_name"),
        row.at("gender"),
        row.at("date_of_birth")
    );

    if (row.count("date_of_death") && !row.at("date_of_death").empty()) {
        person.setDateOfDeath(row.at("date_of_death"));
    }

    if (row.count("birth_place") && !row.at("birth_place").empty()) {
        person.setBirthPlace(row.at("birth_place"));
    }

    if (row.count("death_place") && !row.at("death_place").empty()) {
        person.setDeathPlace(row.at("death_place"));
    }

    return person;
}

// Add similar implementations for other methods...

void DatabaseManager::beginTransaction() {
    connector->beginTransaction();
}

void DatabaseManager::commit() {
    connector->commit();
}

void DatabaseManager::rollback() {
    connector->rollback();
}



bool DatabaseManager::updatePerson(const Person& person) {
    const std::string sql = R"(
        UPDATE Person 
        SET first_name = ?, 
            last_name = ?, 
            gender = ?,
            date_of_birth = ?,
            date_of_death = ?,
            birth_place = ?,
            death_place = ?
        WHERE person_id = ?
    )";

    std::vector<std::string> params = {
        person.getFirstName(),
        person.getLastName(),
        person.getGender(),
        person.getDateOfBirth(),
        person.getDateOfDeath(),
        person.getBirthPlace(),
        person.getDeathPlace(),
        person.getId()
    };

    return connector->executeCommand(sql, params);
}

bool DatabaseManager::deletePerson(const std::string& personId) {
    const std::string sql = "DELETE FROM Person WHERE person_id = ?";
    return connector->executeCommand(sql, {personId});
}

bool DatabaseManager::addRelationship(const Relationship& relationship) {
    const std::string sql = R"(
        INSERT INTO Relationship (
            relationship_id, person1_id, person2_id, 
            relationship_type, start_date, end_date
        ) VALUES (?, ?, ?, ?, ?, ?)
    )";

    std::vector<std::string> params = {
        relationship.getId(),
        relationship.getPerson1Id(),
        relationship.getPerson2Id(),
        Relationship::relationTypeToString(relationship.getType()),
        relationship.getStartDate(),
        relationship.getEndDate()
    };

    return connector->executeCommand(sql, params);
}

bool DatabaseManager::deleteRelationship(const std::string& relationshipId) {
    const std::string sql = "DELETE FROM Relationship WHERE relationship_id = ?";
    return connector->executeCommand(sql, {relationshipId});
}

std::vector<Relationship> DatabaseManager::getRelationshipsForPerson(const std::string& personId) {
    const std::string sql = R"(
        SELECT * FROM Relationship 
        WHERE person1_id = ? OR person2_id = ?
    )";

    auto results = connector->executeQuery(sql, {personId, personId});
    std::vector<Relationship> relationships;

    for (const auto& row : results) {
        relationships.push_back(createRelationshipFromRow(row));
    }

    return relationships;
}

Relationship DatabaseManager::createRelationshipFromRow(const std::map<std::string, std::string>& row) {
    RelationType type;
    std::string typeStr = row.at("relationship_type");
    
    if (typeStr == "PARENT_CHILD") type = RelationType::PARENT_CHILD;
    else if (typeStr == "SPOUSE") type = RelationType::SPOUSE;
    else type = RelationType::SIBLING;

    Relationship rel(
        row.at("relationship_id"),
        row.at("person1_id"),
        row.at("person2_id"),
        type
    );

    if (row.count("start_date") && !row.at("start_date").empty()) {
        rel.setStartDate(row.at("start_date"));
    }
    if (row.count("end_date") && !row.at("end_date").empty()) {
        rel.setEndDate(row.at("end_date"));
    }

    return rel;
}

std::vector<Person> DatabaseManager::searchPeople(const std::string& searchTerm) {
    const std::string sql = R"(
        SELECT * FROM Person 
        WHERE first_name LIKE ? OR last_name LIKE ?
        OR first_name || ' ' || last_name LIKE ?
    )";

    std::string searchPattern = "%" + searchTerm + "%";
    auto results = connector->executeQuery(sql, 
        {searchPattern, searchPattern, searchPattern});

    std::vector<Person> people;
    for (const auto& row : results) {
        people.push_back(createPersonFromRow(row));
    }
    
    return people;
}
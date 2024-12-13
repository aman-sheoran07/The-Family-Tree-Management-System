

#include "database/SQLiteConnector.hpp"
#include <stdexcept>
#include <iostream>

SQLiteConnector::SQLiteConnector(const std::string& path) : dbPath(path) {
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc) {
        throw std::runtime_error("Cannot open database: " + std::string(sqlite3_errmsg(db)));
    }
    initializeDatabase();
}

SQLiteConnector::~SQLiteConnector() {
    if (db) {
        sqlite3_close(db);
    }
}

bool SQLiteConnector::executeCommand(const std::string& sql, const std::vector<std::string>& params) {
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    checkError(rc, "Preparing statement");

    // Bind parameters
    for (size_t i = 0; i < params.size(); i++) {
        rc = sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_STATIC);
        checkError(rc, "Binding parameter");
    }

    // Execute
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return (rc == SQLITE_DONE);
}

std::vector<std::map<std::string, std::string>> SQLiteConnector::executeQuery(
    const std::string& sql, 
    const std::vector<std::string>& params
) {
    std::vector<std::map<std::string, std::string>> results;
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    checkError(rc, "Preparing query");

    // Bind parameters
    for (size_t i = 0; i < params.size(); i++) {
        rc = sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_STATIC);
        checkError(rc, "Binding parameter");
    }

    // Fetch results
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::map<std::string, std::string> row;
        int columns = sqlite3_column_count(stmt);
        
        for (int i = 0; i < columns; i++) {
            const char* columnName = sqlite3_column_name(stmt, i);
            const char* value = (const char*)sqlite3_column_text(stmt, i);
            row[columnName] = value ? value : "";
        }
        
        results.push_back(row);
    }

    sqlite3_finalize(stmt);
    return results;
}

void SQLiteConnector::beginTransaction() {
    executeCommand("BEGIN TRANSACTION");
}

void SQLiteConnector::commit() {
    executeCommand("COMMIT");
}

void SQLiteConnector::rollback() {
    executeCommand("ROLLBACK");
}

bool SQLiteConnector::initializeDatabase() {
    // Create tables one by one to better handle errors
    const std::vector<std::string> initQueries = {
        // Person table
        R"(
            CREATE TABLE IF NOT EXISTS Person (
                person_id TEXT PRIMARY KEY,
                first_name TEXT NOT NULL,
                last_name TEXT NOT NULL,
                gender TEXT CHECK(gender IN ('M', 'F', 'O')),
                date_of_birth TEXT NOT NULL,
                date_of_death TEXT,
                birth_place TEXT,
                death_place TEXT
            )
        )",
        
        // Relationship table
        R"(
            CREATE TABLE IF NOT EXISTS Relationship (
                relationship_id TEXT PRIMARY KEY,
                person1_id TEXT NOT NULL,
                person2_id TEXT NOT NULL,
                relationship_type TEXT NOT NULL,
                start_date TEXT,
                end_date TEXT,
                FOREIGN KEY (person1_id) REFERENCES Person(person_id),
                FOREIGN KEY (person2_id) REFERENCES Person(person_id)
            )
        )"
    };

    try {
        beginTransaction();
        
        for (const auto& query : initQueries) {
            if (!executeCommand(query)) {
                rollback();
                std::cerr << "Failed to execute query: " << query << std::endl;
                return false;
            }
        }
        
        commit();
        return true;
    } catch (const std::exception& e) {
        rollback();
        std::cerr << "Database initialization failed: " << e.what() << std::endl;
        return false;
    }
}

void SQLiteConnector::checkError(int result, const std::string& operation) {
    if (result != SQLITE_OK) {
        std::string error = sqlite3_errmsg(db);
        throw std::runtime_error(operation + " failed: " + error);
    }
}
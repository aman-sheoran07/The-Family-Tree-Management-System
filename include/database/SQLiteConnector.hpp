#ifndef SQLITE_CONNECTOR_HPP
#define SQLITE_CONNECTOR_HPP

#include <sqlite3.h>
#include <string>
#include <vector>
#include <map>
#include <memory>

class SQLiteConnector {
private:
    sqlite3* db;
    std::string dbPath;

public:
    // Constructor and destructor
    explicit SQLiteConnector(const std::string& dbPath);
    ~SQLiteConnector();

    // Query execution methods
    bool executeCommand(const std::string& sql, const std::vector<std::string>& params = {});
    std::vector<std::map<std::string, std::string>> executeQuery(
        const std::string& sql, 
        const std::vector<std::string>& params = {}
    );

    // Transaction methods
    void beginTransaction();
    void commit();
    void rollback();

    // Database initialization
    bool initializeDatabase();

private:
    // Helper methods
    static int callback(void* data, int argc, char** argv, char** azColName);
    void checkError(int result, const std::string& operation);
};

#endif // SQLITE_CONNECTOR_HPP
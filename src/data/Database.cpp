#include "../../include/data/Database.h"


Database& Database::getInstance() {
    static Database instance;
    return instance;
}

Database::Database(const std::string& filename) : db(nullptr), isOpen(false) {
    int result = sqlite3_open(filename.c_str(), &db);
    if (result != SQLITE_OK) {
        throw std::runtime_error("Cannot open database: " + std::string(sqlite3_errmsg(db)));
    }
    isOpen = true;
}

Database::~Database() {
    if (isOpen) {
        sqlite3_close(db);
    }
}

void Database::execute(const std::string& sql) {
    // std::cout << "[DEBUG] Executing SQL:\n" << sql << "\n\n";
    char* errorMsg = nullptr;
    int result = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMsg);
    if (result != SQLITE_OK) {
        std::string error = "SQL error: " + std::string(errorMsg);
        sqlite3_free(errorMsg);
        throw std::runtime_error(error);
    }
}

sqlite3_stmt* Database::prepare(const std::string& sql) {
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    checkError(result, "prepare statement");
    return stmt;
}

void Database::execute(sqlite3_stmt* stmt) {
    int result = sqlite3_step(stmt);
    checkError(result, "execute statement");
    sqlite3_finalize(stmt);
}

int Database::lastInsertId() const {
    return sqlite3_last_insert_rowid(db);
}

void Database::checkError(int resultCode, const std::string& context) const {
    if (resultCode != SQLITE_OK && resultCode != SQLITE_DONE) {
        throw std::runtime_error("Database error in " + context + ": " + sqlite3_errmsg(db));
    }
}

sqlite3* Database::handle() const {
    return db;
}

#pragma once

#include <sqlite3.h>
#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>

class Database {
public:
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    static Database& getInstance();

    void execute(const std::string& sql);

    sqlite3_stmt* prepare(const std::string& sql);

    void execute(sqlite3_stmt* stmt);

    int lastInsertId() const;

    sqlite3* handle() const;

private:
    sqlite3* db;
    bool isOpen;

    explicit Database(const std::string& filename = "store.db");

    ~Database();

    void checkError(int resultCode, const std::string& context) const;
};
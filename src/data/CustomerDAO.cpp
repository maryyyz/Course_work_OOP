#include "../../include/data/CustomerDAO.h"
#include "../../include/data/Database.h"
#include "../../include/core/CustomerLoyaltyLevel.h"
#include <sqlite3.h>
#include <sstream>


void CustomerDAO::createTable(Database& db) {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS customers (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL UNIQUE,
            loyalty_points INTEGER DEFAULT 0 CHECK(loyalty_points >= 0),
            available_points INTEGER DEFAULT 0 CHECK(available_points >= 0),
            loyalty_level TEXT CHECK(loyalty_level IN ('Bronze', 'Silver', 'Gold')) NOT NULL
        )
    )";
    db.execute(sql);
}

int CustomerDAO::save(Database& db, Customer& customer) {
    sqlite3_stmt* stmt = db.prepare(
        "INSERT INTO customers (name, loyalty_points, available_points, loyalty_level) "
        "VALUES (?, ?, ?, ?)"
    );

    sqlite3_bind_text(stmt, 1, customer.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, customer.getLoyaltyPoints());
    sqlite3_bind_int(stmt, 3, customer.getAvailablePoints());
    sqlite3_bind_text(stmt, 4, customer.getLoyaltyLevel().getLevelName().c_str(), -1, SQLITE_TRANSIENT);

    db.execute(stmt);
    return sqlite3_last_insert_rowid(db.handle());
}

std::unique_ptr<Customer> CustomerDAO::getById(Database& db, int id) {
    sqlite3_stmt* stmt = db.prepare(
        "SELECT id, name, loyalty_points, available_points, loyalty_level "
        "FROM customers WHERE id = ?"
    );

    sqlite3_bind_int(stmt, 1, id);
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int customerId = sqlite3_column_int(stmt, 0);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int loyaltyPoints = sqlite3_column_int(stmt, 2);
        int availablePoints = sqlite3_column_int(stmt, 3);
        std::string level = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

        auto customer = std::make_unique<Customer>(
            customerId, 
            name, 
            createLoyaltyLevelFromString(level)
        );
        customer->addLoyaltyPoints(loyaltyPoints);
        customer->setAvailablePoints(availablePoints);
        
        sqlite3_finalize(stmt);
        return customer;
    }
    
    sqlite3_finalize(stmt);
    return nullptr;
}

std::vector<std::unique_ptr<Customer>> CustomerDAO::getAll(Database& db) {
    std::vector<std::unique_ptr<Customer>> result;
    sqlite3_stmt* stmt = db.prepare(
        "SELECT id, name, loyalty_points, available_points, loyalty_level FROM customers"
    );
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int points = sqlite3_column_int(stmt, 2);
        int availablePoints = sqlite3_column_int(stmt, 3); // Добавлено
        std::string levelStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

        auto level = createLoyaltyLevelFromString(levelStr);
        auto customer = std::make_unique<Customer>(id, name, std::move(level));
        customer->addLoyaltyPoints(points);
        customer->setAvailablePoints(availablePoints); // Добавлено
        result.push_back(std::move(customer));
    }
    
    sqlite3_finalize(stmt);
    return result;
}
void CustomerDAO::updateLoyaltyPoints(Database& db, int id, int newPoints) {
    sqlite3_stmt* stmt = db.prepare(
        "UPDATE customers SET loyalty_points = ? WHERE id = ?");
    
    sqlite3_bind_int(stmt, 1, newPoints);
    sqlite3_bind_int(stmt, 2, id);
    db.execute(stmt);
}

std::unique_ptr<LoyaltyLevel> CustomerDAO::createLoyaltyLevelFromString(const std::string& level) {
    if (level == "Bronze") return std::make_unique<BronzeLevel>();
    if (level == "Silver") return std::make_unique<SilverLevel>();
    if (level == "Gold") return std::make_unique<GoldLevel>();
    throw std::invalid_argument("Invalid loyalty level: " + level);
}


void CustomerDAO::update(Database& db, const Customer& customer) {
    sqlite3_stmt* stmt = db.prepare(
        "UPDATE customers SET "
        "name = ?, loyalty_points = ?, available_points = ?, loyalty_level = ? "
        "WHERE id = ?"
    );
    
    sqlite3_bind_text(stmt, 1, customer.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, customer.getLoyaltyPoints());
    sqlite3_bind_int(stmt, 3, customer.getAvailablePoints());
    sqlite3_bind_text(stmt, 4, customer.getLoyaltyLevel().getLevelName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, customer.getId());
    
    db.execute(stmt);
}
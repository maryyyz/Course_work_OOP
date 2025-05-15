#include "../../include/data/TransactionDAO.h"
#include <sqlite3.h>

void TransactionDAO::createTable(Database& db) {
    const char* TRANSACTION_TABLE_SQL = R"(
        CREATE TABLE IF NOT EXISTS transactions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            customer_id INTEGER NOT NULL,
            amount REAL NOT NULL,
            points_earned INTEGER NOT NULL,
            discount_applied REAL DEFAULT 0,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY(customer_id) REFERENCES customers(id)
        )
    )";
    db.execute(TRANSACTION_TABLE_SQL);
}

std::vector<Transaction> TransactionDAO::getByCustomer(Database& db, int customerId) {
    std::vector<Transaction> transactions;
    sqlite3_stmt* stmt = db.prepare(
        "SELECT id, amount, points_earned, discount_applied, "
        "strftime('%d.%m.%Y %H:%M', created_at) " 
        "FROM transactions WHERE customer_id = ?"
    );
    
    sqlite3_bind_int(stmt, 1, customerId);
    
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        Transaction t;
        t.id = sqlite3_column_int(stmt, 0);
        t.amount = sqlite3_column_double(stmt, 1);
        t.points_earned = sqlite3_column_int(stmt, 2);
        t.discount = sqlite3_column_double(stmt, 3);
        t.date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        transactions.push_back(t);
    }
    
    sqlite3_finalize(stmt);
    return transactions;
}


void TransactionDAO::logTransaction(Database& db, int customerId, 
    double amount, int points, 
    double discount) {
    sqlite3_stmt* stmt = db.prepare(
    "INSERT INTO transactions (customer_id, amount, points_earned, discount_applied) "
    "VALUES (?, ?, ?, ?)"
    );

    // Проверяем привязку параметров
    sqlite3_bind_int(stmt, 1, customerId);
    sqlite3_bind_double(stmt, 2, amount);
    sqlite3_bind_int(stmt, 3, points);
    sqlite3_bind_double(stmt, 4, discount);

    db.execute(stmt);
}
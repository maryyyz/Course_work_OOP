#pragma once
#include "Database.h"
#include <vector>
#include <string>

struct Transaction {
    int id;
    int customer_id;
    double amount;
    int points_earned;
    double discount;
    std::string date;
};

class TransactionDAO {
public:
    static void createTable(Database& db);
    
    static void logTransaction(Database& db, int customerId, 
                              double amount, int points, 
                              double discount = 0.0);
    
    static std::vector<Transaction> getByCustomer(Database& db, int customerId);
};
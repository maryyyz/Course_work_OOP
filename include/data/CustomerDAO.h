#pragma once

#include <vector>
#include <memory>

#include "Database.h"
#include "../core/Customer.h"

class CustomerDAO {
public:
    static void createTable(Database& db);

    static int save(Database& db, Customer& customer);

    static std::unique_ptr<Customer> getById(Database& db, int id);

    static std::vector<std::unique_ptr<Customer>> getAll(Database& db);

    static void updateLoyaltyPoints(Database& db, int id, int newPoints);

    static void update(Database& db, const Customer& customer);
    
private:
    static std::unique_ptr<LoyaltyLevel> createLoyaltyLevelFromString(const std::string& level);
};
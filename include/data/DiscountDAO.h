#pragma once

#include <vector>
#include <memory>

#include "Database.h"
#include "../core/Discount.h"

class DiscountDAO {
public:
    static void createTable(Database& db);

    static int save(Database& db, const Discount& discount);

    static std::unique_ptr<Discount> getById(Database& db, int id);
    
    static std::vector<std::unique_ptr<Discount>> getAll(Database& db);

private:
    static std::unique_ptr<Discount> createDiscountFromType(
        const std::string& type, 
        double baseValue, 
        double extraValue = 0.0
    );
};
#include "../../include/data/DiscountDAO.h"
#include "../../include/data/Database.h"
#include <sqlite3.h>
#include <stdexcept>


void DiscountDAO::createTable(Database& db) {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS discounts (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            type TEXT CHECK(type IN ('Percentage', 'Seasonal')),  
            base_rate REAL,
            extra_rate REAL,
            description TEXT
        )
    )";    
    db.execute(sql);
}

int DiscountDAO::save(Database& db, const Discount& discount) {
    sqlite3_stmt* stmt = db.prepare(R"(
        INSERT INTO discounts (type, base_rate, extra_rate, description)
        VALUES (?, ?, ?, ?)
    )");

    std::string type;
    double baseRate = 0.0;
    double extraRate = 0.0;
    std::string description = discount.getDescription();

    if (const auto* pd = dynamic_cast<const PercentageDiscount*>(&discount)) {
        type = "Percentage";
        baseRate = pd->getPercentage();
    } else if (const auto* sd = dynamic_cast<const SeasonalDiscount*>(&discount)) {
        type = "Seasonal";
        if (const auto* base = dynamic_cast<const PercentageDiscount*>(sd->getWrapped())) {
            baseRate = base->getPercentage();
        }
        extraRate = sd->getSeasonalRate();
    }

    sqlite3_bind_text(stmt, 1, type.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, baseRate);
    sqlite3_bind_double(stmt, 3, extraRate);
    sqlite3_bind_text(stmt, 4, description.c_str(), -1, SQLITE_STATIC);
    
    db.execute(stmt);
    return sqlite3_last_insert_rowid(db.handle());
}

std::unique_ptr<Discount> DiscountDAO::getById(Database& db, int id) {
    sqlite3_stmt* stmt = db.prepare(
        "SELECT type, base_rate, extra_rate FROM discounts WHERE id = ?");
    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        double base = sqlite3_column_double(stmt, 1);
        double extra = sqlite3_column_double(stmt, 2);

        auto discount = createDiscountFromType(type, base, extra);
        sqlite3_finalize(stmt);
        return discount;
    }

    sqlite3_finalize(stmt);
    return nullptr;
}

std::vector<std::unique_ptr<Discount>> DiscountDAO::getAll(Database& db) {
    std::vector<std::unique_ptr<Discount>> discounts;
    sqlite3_stmt* stmt = db.prepare("SELECT * FROM discounts");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        double base = sqlite3_column_double(stmt, 2);
        double extra = sqlite3_column_double(stmt, 3);

        discounts.push_back(createDiscountFromType(type, base, extra));
    }

    sqlite3_finalize(stmt);
    return discounts;
}

std::unique_ptr<Discount> DiscountDAO::createDiscountFromType(
    const std::string& type, double base, double extra) 
{
    if (type == "Percentage") {
        return std::make_unique<PercentageDiscount>(base);
    }
    else if (type == "Seasonal") {
        auto baseDiscount = std::make_unique<PercentageDiscount>(base);
        return std::make_unique<SeasonalDiscount>(std::move(baseDiscount), extra);
    }
    throw std::invalid_argument("Unknown discount type: " + type);
}
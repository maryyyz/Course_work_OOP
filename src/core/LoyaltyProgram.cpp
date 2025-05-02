#include "../../include/core/LoyaltyProgram.h"
#include <./../include/data/TransactionDAO.h>

LoyaltyProgram::LoyaltyProgram(double rate) : pointsPerDollar(rate) {
    if(rate < 0) throw std::invalid_argument("Курс не может быть отрицательным");
}

void LoyaltyProgram::processPurchase(int customerId, double amount) {
    auto& db = Database::getInstance();
    db.execute("BEGIN TRANSACTION");
    
    try {
        auto customer = CustomerDAO::getById(db, customerId);
        if(!customer) throw std::runtime_error("Клиент не найден");

        double discount = customer->calculateDiscount(amount);
        double finalAmount = amount - discount;
        
        int points = static_cast<int>(finalAmount * (pointsPerDollar / 100.0));
        customer->addLoyaltyPoints(points);
        
        CustomerDAO::update(db, *customer);
        TransactionDAO::logTransaction(db, customerId, finalAmount, points, discount);
        
        db.execute("COMMIT");
    } catch(...) {
        db.execute("ROLLBACK");
        throw;
    }
}

double LoyaltyProgram::getPointsRate() const {
    return pointsPerDollar;
}
#include "../../include/core/Customer.h"
 
Customer::Customer(int id, std::string name, std::unique_ptr<LoyaltyLevel> level)
    : id(id), name(std::move(name)), loyaltyPoints(0), availablePoints(0),
      loyaltyLevel(std::move(level)) 
{
    if (!loyaltyLevel) {
        throw std::invalid_argument("Уровень лояльности не может быть null");
    }
}

int Customer::getId() const {
    return id;
}

std::string Customer::getName() const {
    return name;
}

int Customer::getLoyaltyPoints() const {
    return loyaltyPoints;
}

const LoyaltyLevel& Customer::getLoyaltyLevel() const {
    return *loyaltyLevel;
}

double Customer::calculateDiscount(double price) const {
    return loyaltyLevel->calculateDiscount(price);
}

void Customer::addLoyaltyPoints(int points) {
    if (points < 0) {
        throw std::invalid_argument("Points value must be non-negative");
    }
    loyaltyPoints += points;
}

void Customer::upgradeLoyaltyLevel(std::unique_ptr<LoyaltyLevel> newLevel) {
    if (!newLevel) {
        throw std::invalid_argument("New loyalty level cannot be null");
    }
    loyaltyLevel = std::move(newLevel);
}

int Customer::getAvailablePoints() const { 
    return availablePoints; 
}

void Customer::setAvailablePoints(int points) {
    if(points < 0) throw std::invalid_argument("Баллы не могут быть отрицательными");
    availablePoints = points;
}
#pragma once

#include <string>
#include <memory>
#include <stdexcept>
#include <utility> 

#include "CustomerLoyaltyLevel.h"

class Customer {
    private:
        int id;
        std::string name;
        int loyaltyPoints;
        int availablePoints;
        std::unique_ptr<LoyaltyLevel> loyaltyLevel;
        
    public:
        Customer(int id, std::string name, std::unique_ptr<LoyaltyLevel> level);
        
        int getId() const;
        std::string getName() const;
        int getLoyaltyPoints() const;
        int getAvailablePoints() const;
        const LoyaltyLevel& getLoyaltyLevel() const;
        
        double calculateDiscount(double price) const;
        void addLoyaltyPoints(int points);
        void setAvailablePoints(int points);
        void upgradeLoyaltyLevel(std::unique_ptr<LoyaltyLevel> newLevel);
    };
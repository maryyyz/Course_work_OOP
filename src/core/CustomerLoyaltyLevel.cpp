#include "../../include/core/CustomerLoyaltyLevel.h"

double BronzeLevel::calculateDiscount(double price) const { 
    return price * 0.05; 
}

std::string BronzeLevel::getLevelName() const { 
    return "Bronze"; 
}

int BronzeLevel::getUpgradeThreshold() const {
    return 1000;
}

std::unique_ptr<LoyaltyLevel> BronzeLevel::clone() const {
    return std::make_unique<BronzeLevel>(*this); 
}

double SilverLevel::calculateDiscount(double price) const { 
    return price * 0.10; 
}

std::string SilverLevel::getLevelName() const { 
    return "Silver"; 
}

int SilverLevel::getUpgradeThreshold() const {
    return 2000;
}

std::unique_ptr<LoyaltyLevel> SilverLevel::clone() const {
    return std::make_unique<SilverLevel>(*this);
}

double GoldLevel::calculateDiscount(double price) const { 
    return price * 0.20; 
}

std::string GoldLevel::getLevelName() const { 
    return "Gold"; 
}

int GoldLevel::getUpgradeThreshold() const {
    return 3500;
}

std::unique_ptr<LoyaltyLevel> GoldLevel::clone() const {
    return std::make_unique<GoldLevel>(*this);
}
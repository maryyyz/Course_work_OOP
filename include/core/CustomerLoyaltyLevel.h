#pragma once

#include <string>
#include <memory>

class LoyaltyLevel {
public:
    virtual ~LoyaltyLevel() = default;
    virtual double calculateDiscount(double price) const = 0;
    virtual std::string getLevelName() const = 0;
    virtual int getUpgradeThreshold() const = 0;
    virtual std::unique_ptr<LoyaltyLevel> clone() const = 0;;
};

class BronzeLevel : public LoyaltyLevel {
public:

    BronzeLevel() = default;
    BronzeLevel(const BronzeLevel&) = default; 
    BronzeLevel(BronzeLevel&&) = default; 

    double calculateDiscount(double price) const override;

    std::string getLevelName() const override;

    int getUpgradeThreshold() const override;

    std::unique_ptr<LoyaltyLevel> clone() const override;
};

class SilverLevel : public LoyaltyLevel {
public:

    SilverLevel() = default;
    SilverLevel(const SilverLevel&) = default; 
    SilverLevel(SilverLevel&&) = default; 

    double calculateDiscount(double price) const override;

    std::string getLevelName() const override;

    int getUpgradeThreshold() const override;
    
    std::unique_ptr<LoyaltyLevel> clone() const override;
};

class GoldLevel : public LoyaltyLevel {
public:

    GoldLevel() = default;
    GoldLevel(const GoldLevel&) = default; 
    GoldLevel(GoldLevel&&) = default; 

    double calculateDiscount(double price) const override;
    
    std::string getLevelName() const override;

    int getUpgradeThreshold() const override;
    
    std::unique_ptr<LoyaltyLevel> clone() const override;
};
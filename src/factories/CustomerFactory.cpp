#include "../../include/factories/CustomerFactory.h"
#include "../../include/core/Customer.h"
#include "../../include/core/CustomerLoyaltyLevel.h"
#include <memory>  


std::unique_ptr<Customer> CustomerFactory::create(
    int id,
    const std::string& name,
    std::unique_ptr<LoyaltyLevel> level) {
    if (!level) {
        throw std::invalid_argument("Loyalty level cannot be null");
    }
    return std::make_unique<Customer>(id, name, std::move(level));
}

std::unique_ptr<Customer> CustomerFactory::createVip(int id, const std::string& name) {
    return create(id, name, std::make_unique<SilverLevel>());
}

std::unique_ptr<Customer> CustomerFactory::createWholesale(int id, const std::string& name) {
    return create(id, name, std::make_unique<GoldLevel>());
}
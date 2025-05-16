#pragma once

#include <memory>

#include "../core/Customer.h"
#include "../core/CustomerLoyaltyLevel.h"

class CustomerFactory {
public:
    static std::unique_ptr<Customer> create(
        int id,
        const std::string& name,
        std::unique_ptr<LoyaltyLevel> level
    );
    
    static std::unique_ptr<Customer> createVip(int id, const std::string& name);

    static std::unique_ptr<Customer> createWholesale(int id, const std::string& name);
};
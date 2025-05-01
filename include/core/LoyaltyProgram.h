#pragma once

#include "Customer.h"
#include <stdexcept>
#include "../../include/data/CustomerDAO.h"
#include "../../include/data/Database.h"

class LoyaltyProgram {
    private:
        double pointsPerDollar; 
    public:
        explicit LoyaltyProgram(double rate);
        void processPurchase(int customerId, double amount);
        void setPointsRate(double newRate);
        double getPointsRate() const;
    };
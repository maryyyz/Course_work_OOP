#pragma once

#include <memory>

#include "../core/Discount.h"

class DiscountFactory {
public:
    static std::unique_ptr<Discount> createPercentageDiscount(double percentage);

    static std::unique_ptr<Discount> createCompositeDiscount(
        std::unique_ptr<Discount> first,
        std::unique_ptr<Discount> second
    );
    
    static std::unique_ptr<Discount> createSeasonalDiscount(
        std::unique_ptr<Discount> base,
        double seasonalRate
    );
};
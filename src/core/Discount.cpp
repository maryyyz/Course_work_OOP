#include "../../include/core/Discount.h"


PercentageDiscount::PercentageDiscount(double p) : percentage(p) {
    if (p < 0 || p > 1.0) {
        throw std::invalid_argument("Discount percentage must be between 0 and 1.0");
    }
}

double PercentageDiscount::applyTo(double price) const {
    if (price < 0) throw std::invalid_argument("Price cannot be negative");
    return price * (1.0 - percentage);
}

std::string PercentageDiscount::getDescription() const {
    std::ostringstream oss;
    oss << percentage * 100 << "% discount";
    return oss.str();
}

std::unique_ptr<Discount> PercentageDiscount::clone() const {
    return std::make_unique<PercentageDiscount>(*this);
}

DiscountDecorator::DiscountDecorator(std::unique_ptr<Discount> discount)
    : wrapped(std::move(discount)) {
    if (!wrapped) {
        throw std::invalid_argument("Wrapped discount cannot be null");
    }
}

SeasonalDiscount::SeasonalDiscount(std::unique_ptr<Discount> discount, double rate)
    : DiscountDecorator(std::move(discount)), seasonalRate(rate) {
    if (rate < 0 || rate > 1.0) {
        throw std::invalid_argument("Seasonal rate must be between 0 and 1.0");
    }
}

double SeasonalDiscount::applyTo(double price) const {
    double discounted = wrapped->applyTo(price);
    return discounted * (1.0 - seasonalRate);
}

std::string SeasonalDiscount::getDescription() const {
    std::ostringstream oss;
    oss << wrapped->getDescription() 
        << " + additional " << seasonalRate * 100 << "% seasonal discount";
    return oss.str();
}

std::unique_ptr<Discount> SeasonalDiscount::clone() const {
    return std::make_unique<SeasonalDiscount>(
        wrapped->clone(), 
        seasonalRate
    );
}
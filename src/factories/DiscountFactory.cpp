#include "../../include/factories/DiscountFactory.h"
#include "../../include/core/Discount.h"
#include <memory>  
#include <string>  

std::unique_ptr<Discount> DiscountFactory::createPercentageDiscount(double percentage) {
    return std::make_unique<PercentageDiscount>(percentage);
}

std::unique_ptr<Discount> DiscountFactory::createCompositeDiscount(
    std::unique_ptr<Discount> first,
    std::unique_ptr<Discount> second) {
    class CompositeDiscount : public Discount {
        std::unique_ptr<Discount> first_;
        std::unique_ptr<Discount> second_;
    public:
        CompositeDiscount(std::unique_ptr<Discount> first, std::unique_ptr<Discount> second)
            : first_(std::move(first)), second_(std::move(second)) {}
            
        double applyTo(double price) const override {
            return second_->applyTo(first_->applyTo(price));
        }
        
        std::string getDescription() const override {
            return first_->getDescription() + " + " + second_->getDescription();
        }
        
        std::unique_ptr<Discount> clone() const override {
            return std::make_unique<CompositeDiscount>(first_->clone(), second_->clone());
        }
    };
    
    return std::make_unique<CompositeDiscount>(std::move(first), std::move(second));
}

std::unique_ptr<Discount> DiscountFactory::createSeasonalDiscount(
    std::unique_ptr<Discount> base,
    double seasonalRate) {
    return std::make_unique<SeasonalDiscount>(std::move(base), seasonalRate);
}
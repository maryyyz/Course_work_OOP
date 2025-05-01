#pragma once

#include <memory>
#include <string>
#include <stdexcept>
#include <sstream>
#include <utility>

class Discount {
public:
    virtual ~Discount() = default;
    virtual double applyTo(double price) const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::unique_ptr<Discount> clone() const = 0;
};

class PercentageDiscount : public Discount {
    double percentage;
public:
    explicit PercentageDiscount(double p);

    double applyTo(double price) const override;

    std::string getDescription() const override;

    std::unique_ptr<Discount> clone() const override;

    double getPercentage() const { return percentage; }
};

class DiscountDecorator : public Discount {
protected:
    std::unique_ptr<Discount> wrapped;
public:
    explicit DiscountDecorator(std::unique_ptr<Discount> discount);
};

class SeasonalDiscount : public DiscountDecorator {
    double seasonalRate;
public:
    SeasonalDiscount(std::unique_ptr<Discount> discount, double rate);

    double applyTo(double price) const override;

    std::string getDescription() const override;
    
    std::unique_ptr<Discount> clone() const override;

    double getSeasonalRate() const { return seasonalRate; }
    
    const Discount* getWrapped() const { return wrapped.get(); }
};
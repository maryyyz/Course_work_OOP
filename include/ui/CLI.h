#pragma once

#include <memory>
#include "../data/Database.h"
#include "../core/LoyaltyProgram.h"

class CLI {
    Database& db;
    LoyaltyProgram& loyaltyProgram;
    
    void displayMainMenu();
    void processChoice(int choice);
    
    void addCustomer();
    void listCustomers();
    void makePurchase();
    void showCustomerDetails();
    void updateLoyaltyPoints();
    void exitProgram();
    void pressToContinue();
    void printBox(const std::string& text);
    
public:
    CLI(Database& database, LoyaltyProgram& program);
    void run();
};
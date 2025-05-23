#include "../include/ui/CLI.h"
#include "../include/data/Database.h"
#include "../include/core/LoyaltyProgram.h"
#include <memory>
#include <iostream>

int main() {
    try {
        Database& db = Database::getInstance();
        LoyaltyProgram program(0.1); 
        
        CLI cli(db, program);
        cli.run();
    } catch(const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
#include "../../include/ui/CLI.h"
#include "../../include/data/CustomerDAO.h"
#include "../../include/data/DiscountDAO.h"
#include "../../include/data/TransactionDAO.h"
#include "../../include/factories/CustomerFactory.h"
#include "../../include/factories/DiscountFactory.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include <memory>

#define HEADER_LINE "╔════════════════════════════════════════════╗\n"
#define FOOTER_LINE "╚════════════════════════════════════════════╝\n"
#define SEPARATOR   "├────────────────────────────────────────────┤\n"

CLI::CLI(Database& database, LoyaltyProgram& program) 
    : db(database), loyaltyProgram(program) {}

void CLI::printBox(const std::string& text) {
    std::cout << HEADER_LINE
              << "║ " << std::left << std::setw(42) << text << " ║\n"
              << FOOTER_LINE;
}

void CLI::run() {
    CustomerDAO::createTable(db);
    DiscountDAO::createTable(db);
    TransactionDAO::createTable(db);
    
    printBox("🎉 Добро пожаловать в систему лояльности!");
    
    int choice = 0;
    do {
        displayMainMenu();
        std::cout << "➡️  Ваш выбор: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        processChoice(choice);
    } while(choice != 6);
}

void CLI::displayMainMenu() {
    system("clear || cls");
    std::cout << HEADER_LINE
              << "║            🏪 МАГАЗИН ПРИВЕТСТВУЕТ!        ║\n"
              << SEPARATOR
              << "║ 1. 📝 Добавить нового клиента              ║\n"
              << "║ 2. 📋 Список всех клиентов                 ║\n"
              << "║ 3. 🛒 Оформить покупку                     ║\n"
              << "║ 4. 🔍 Просмотр данных клиента              ║\n"
              << "║ 5. ⭐ Изменить баллы лояльности            ║\n"
              << "║ 6. 🚪 Выход                                ║\n"
              << FOOTER_LINE;
}

void CLI::processChoice(int choice) {
    try {
        switch(choice) {
            case 1: addCustomer(); break;
            case 2: listCustomers(); break;
            case 3: makePurchase(); break;
            case 4: showCustomerDetails(); break;
            case 5: updateLoyaltyPoints(); break;
            case 6: exitProgram(); break;
            default: 
                std::cout << "⚠️  Неверный выбор! Попробуйте снова.\n";
        }
        pressToContinue();
    } catch(const std::exception& e) {
        std::cerr << "❌ Ошибка: " << e.what() << "\n";
        pressToContinue();
    }
}

void CLI::pressToContinue() {
    std::cout << "\n⏎  Нажмите Enter для продолжения...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void CLI::addCustomer() {
    system("clear || cls");
    printBox("👤 ДОБАВЛЕНИЕ НОВОГО КЛИЕНТА");
    
    std::string name;
    int typeChoice;
    
    std::cout << "📛 Введите ФИО клиента: ";
    std::getline(std::cin, name);
    
    std::cout << "\n🎚️  Выберите категорию:\n"
              << "1. 🥉 Обычный клиент\n"
              << "2. 🥈 VIP клиент\n"
              << "3. 🥇 Оптовый покупатель\n"
              << "👉 Ваш выбор: ";
    std::cin >> typeChoice;
    
    std::unique_ptr<Customer> customer;
    switch(typeChoice) {
        case 1:
            customer = CustomerFactory::create(-1, name, std::make_unique<BronzeLevel>());
            break;
        case 2:
            customer = CustomerFactory::createVip(-1, name);
            break;
        case 3:
            customer = CustomerFactory::createWholesale(-1, name);
            break;
        default:
            throw std::invalid_argument("🚫 Неверный тип клиента!");
    }
    
    int id = CustomerDAO::save(db, *customer);
    std::cout << "\n✅ Успех! Клиент создан. ID: " << id << std::endl;
}

void CLI::listCustomers() {
    system("clear || cls");
    printBox("📃 СПИСОК ВСЕХ КЛИЕНТОВ");
    
    auto customers = CustomerDAO::getAll(db);
    if(customers.empty()) {
        std::cout << "😞 Пока нет ни одного клиента\n";
        return;
    }
    
    std::cout << "┌──────────┬───────────────────────┬───────────────┐\n"
              << "│   ID     │        ФИО           │    Статус     │\n"
              << "├──────────┼───────────────────────┼───────────────┤\n";
    
    for(const auto& customer : customers) {
        std::cout << "│ " << std::setw(8) << customer->getId() << " │ "
                  << std::setw(21) << customer->getName() << " │ "
                  << std::setw(13) << customer->getLoyaltyLevel().getLevelName() << " │\n";
    }
    std::cout << "└──────────┴───────────────────────┴───────────────┘\n";
}

void CLI::makePurchase() {
    system("clear || cls");
    printBox("💰 ОФОРМЛЕНИЕ ПОКУПКИ");
    
    int customerId = 0;
    double amount = 0.0;
    
    try {
        std::cout << "🔢 Введите ID клиента: ";
        if(!(std::cin >> customerId) || customerId <= 0) {
            throw std::invalid_argument("Неверный ID клиента");
        }
        
        std::cout << "💵 Сумма покупки (₽): ";
        if(!(std::cin >> amount) || amount <= 0) {
            throw std::invalid_argument("Неверная сумма покупки");
        }
        
        db.execute("BEGIN TRANSACTION");
        
        auto customer = CustomerDAO::getById(db, customerId);
        if(!customer) {
            throw std::runtime_error("Клиент не найден");
        }
        
        // Расчет скидки
        double discount = customer->calculateDiscount(amount);
        double finalAmount = amount - discount;
        
        // Использование доступных баллов
        int usedPoints = 0;
        if(customer->getAvailablePoints() > 0) {
            double pointsValue = customer->getAvailablePoints() * 0.1;
            finalAmount = std::max(finalAmount - pointsValue, 0.0);
            usedPoints = static_cast<int>(pointsValue * 10);
            customer->setAvailablePoints(customer->getAvailablePoints() - usedPoints);
        }
        
        // Начисление новых баллов
        int earnedPoints = static_cast<int>(finalAmount * loyaltyProgram.getPointsRate());
        customer->addLoyaltyPoints(earnedPoints);
        
        // Обновление уровня
        const int currentPoints = customer->getLoyaltyPoints();
        const std::string currentLevel = customer->getLoyaltyLevel().getLevelName();
        
        if(currentPoints >= 1000 && currentLevel != "Gold") {
            customer->upgradeLoyaltyLevel(std::make_unique<GoldLevel>());
        }
        else if(currentPoints >= 500 && currentLevel != "Silver") {
            customer->upgradeLoyaltyLevel(std::make_unique<SilverLevel>());
        }
        
        // Сохранение изменений
        CustomerDAO::update(db, *customer);
        TransactionDAO::logTransaction(db, customerId, finalAmount, earnedPoints, discount);
        db.execute("COMMIT");
        
        std::cout << "\n✅ Покупка оформлена!\n"
                  << "💸 Итоговая сумма: " << std::fixed << std::setprecision(2) << finalAmount << " ₽\n"
                  << "💎 Использовано баллов: " << usedPoints << "\n"
                  << "🎁 Начислено баллов: " << earnedPoints << std::endl;
                  
    } catch(const std::exception& e) {
        db.execute("ROLLBACK");
        std::cerr << "\n❌ Ошибка: " << e.what() << std::endl;
    }
    
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void CLI::showCustomerDetails() {
    system("clear || cls");
    printBox("📄 ПОДРОБНАЯ ИНФОРМАЦИЯ");
    
    int customerId;
    std::cout << "🔢 Введите ID клиента: ";
    if(!(std::cin >> customerId)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "❌ Неверный формат ID\n";
        return;
    }
    
    try {
        auto customer = CustomerDAO::getById(db, customerId);
        if(!customer) {
            std::cout << "😞 Клиент с ID " << customerId << " не найден\n";
            return;
        }
        
        std::cout << "\n👤 ФИО: " << customer->getName()
                  << "\n🆔 ID: " << customer->getId()
                  << "\n⭐ Накопленные баллы: " << customer->getLoyaltyPoints()
                  << "\n🎁 Доступные баллы: " << customer->getAvailablePoints()
                  << "\n🏅 Текущий статус: " << customer->getLoyaltyLevel().getLevelName()
                  << "\n📈 Текущая скидка: " << (customer->getLoyaltyLevel().calculateDiscount(100.0)) << "%\n";
                  
        std::cout << "\n📜 История покупок:\n";
        auto transactions = TransactionDAO::getByCustomer(db, customerId);
        
        if(transactions.empty()) {
            std::cout << "   🕒 Покупок пока нет\n";
        } else {
            for(const auto& t : transactions) {
                std::cout << "   🕒 " << t.date 
                          << " - Сумма: " << std::fixed << std::setprecision(2) << t.amount << " ₽"
                          << " | Баллы: +" << t.points_earned
                          << " | Скидка: " << t.discount << " ₽\n";
            }
        }
        
    } catch(const std::exception& e) {
        std::cerr << "\n❌ Ошибка: " << e.what() << "\n";
    }
    
    // Очистка буфера ввода
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void CLI::updateLoyaltyPoints() {
    system("clear || cls");
    printBox("⚖️  ИЗМЕНЕНИЕ БАЛЛОВ");
    
    int customerId, points;
    std::cout << "🔢 Введите ID клиента: ";
    std::cin >> customerId;
    
    std::cout << "➕ Введите количество баллов: ";
    std::cin >> points;
    
    CustomerDAO::updateLoyaltyPoints(db, customerId, points);
    std::cout << "\n✅ Баллы успешно обновлены!\n";
}

void CLI::exitProgram() {
    system("clear || cls");
    std::cout << HEADER_LINE
              << "║            🛑 ВЫХОД ИЗ ПРОГРАММЫ            ║\n"
              << SEPARATOR
              << "║          Спасибо за использование!          ║\n"
              << "║             До новых встреч! 👋             ║\n"
              << FOOTER_LINE;
}
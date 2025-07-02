#include "appUI.hpp"

void AppUI::run() {
    initialSetup();
    int option = 0;
    do {
        showMainMenu();
        std::cin >> option;
        handleMenuOption(option);
    } while (option != 0);
}

void AppUI::initialSetup() {
    std::cout << "==== SmartBudget - Initial Setup ====\n";
    std::cout << "1 - Create new file\n";
    std::cout << "2 - Load existing file\n";
    std::cout << "0 - Exit\n";
    std::cout << "Choose an option: ";
    int initialOption;
    std::cin >> initialOption;

    if (initialOption == 1) {
        std::cout << "Enter base name for the new file: ";
        std::cin >> filename;
        filename = FileManager::generateUniqueFilename(filename);
        std::cout << "New file created: '" << filename << "'\n";
    } else if (initialOption == 2) {
        std::cout << "\nAvailable CSV files:\n";
        DIR* dir;
        struct dirent* entry;
        dir = opendir(".");
        if (dir != nullptr) {
            while ((entry = readdir(dir)) != nullptr) {
                std::string filenameEntry = entry->d_name;
                if (filenameEntry.length() >= 4 && filenameEntry.substr(filenameEntry.length() - 4) == ".csv") {
                    std::cout << "- " << filenameEntry << std::endl;
                }
            }
            closedir(dir);
        }

        std::cout << "Enter existing file name: ";
        std::cin >> filename;
        filename = FileManager::ensureCSVExtension(filename);

        if (FileManager::fileExists(filename)) {
            std::vector<Transaction> loaded;
            if (FileManager::loadFromFile(loaded, filename)) {
                for (const auto& t : loaded) {
                    manager.addTransaction(t);
                }
                std::cout << "Transactions loaded from '" << filename << "'\n";
            } else {
                std::cout << "Error loading file.\n";
                filename.clear();
            }
        } else {
            std::cout << "File not found.\n";
            filename.clear();
        }
    } else if (initialOption == 0) {
        std::cout << "Exiting program...\n";
        exit(0);
    } else {
        std::cout << "Invalid option.\n";
        filename.clear();
        exit(0);
    }
}

void AppUI::showMainMenu() {
    std::cout << "\n==== SmartBudget - Menu ====\n";
    
    if (!filename.empty()) {
        std::cout << "[Current file: " << filename << "]\n";
    } else {
        std::cout << "[No file currently loaded]\n";
    }

    std::cout << "1. Add transaction\n";
    std::cout << "2. List transactions\n";
    std::cout << "3. Edit transaction\n";
    std::cout << "4. Remove transaction\n";
    std::cout << "5. Calculate total balance\n";
    std::cout << "6. Total by category\n";
    std::cout << "7. Total by type\n";
    std::cout << "8. Filter by date range\n";
    std::cout << "9. Filter by amount range\n";
    std::cout << "10. Switch transaction file\n";
    std::cout << "11. Delete transaction file\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose an option: ";
}

void AppUI::handleMenuOption(int option) {
    switch (option) {
        case 1: addTransaction(); break;
        case 2: listTransactions(); break;
        case 3: editTransaction(); break;
        case 4: removeTransaction(); break;
        case 5: calculateTotalBalance(); break;
        case 6: totalByCategory(); break;
        case 7: totalByType(); break;
        case 8: filterByDateRange(); break;
        case 9: filterByAmountRange(); break;
        case 10: switchTransactionFile(); break;
        case 11: deleteTransactionFile(); break;
        case 0: std::cout << "Exiting program...\n"; break;
        default: std::cout << "Invalid option!\n"; break;
    }
}

void AppUI::addTransaction() {
    if (filename.empty()) {
        std::cout << "No file selected. Use option 10 to choose a file.\n";
        return;
    }

    double amount;
    std::string type, category, date, description;

    std::cout << "Enter amount: ";
    std::cin >> amount;
    std::cout << "Enter type (renda/despesa): ";
    std::cin >> type;
    std::cout << "Enter category: ";
    std::cin >> category;
    std::cin.ignore();
    std::cout << "Enter date (YYYY-MM-DD): ";
    getline(std::cin, date);
    std::cout << "Enter description: ";
    getline(std::cin, description);

    Transaction newTransaction(amount, type, category, date, description);
    manager.addTransaction(newTransaction);
    FileManager::saveToFile(manager.getTransactions(), filename);
    std::cout << "Transaction added and saved successfully!\n";
}

void AppUI::listTransactions() {
    const auto& list = manager.getTransactions();
    if (list.empty()) {
        std::cout << "No transactions recorded.\n";
    } else {
        std::cout << "\n=== Transaction List ===\n";
        for (size_t i = 0; i < list.size(); i++) {
            std::cout << "[" << i << "] ";
            list[i].print();
        }
    }
}

void AppUI::editTransaction() {
    const auto& list = manager.getTransactions();
    if (list.empty()) {
        std::cout << "No transactions to edit.\n";
        return;
    }

    int index;
    std::cout << "Enter the index of the transaction to edit: ";
    std::cin >> index;

    if (index >= 0 && index < static_cast<int>(list.size())) {
        std::cout << "\n=== Selected Transaction ===\n";
        list[index].print();

        double amount;
        std::string type, category, date, description;

        std::cout << "Enter new amount (current: " << list[index].getAmount() << "): ";
        std::cin >> amount;
        std::cout << "Enter new type (renda/despesa) (current: " << list[index].getType() << "): ";
        std::cin >> type;
        std::cout << "Enter new category (current: " << list[index].getCategory() << "): ";
        std::cin >> category;
        std::cin.ignore();
        std::cout << "Enter new date (YYYY-MM-DD) (current: " << list[index].getDate() << "): ";
        getline(std::cin, date);
        std::cout << "Enter new description (current: " << list[index].getDescription() << "): ";
        getline(std::cin, description);

        Transaction updatedTransaction(amount, type, category, date, description);
        manager.updateTransaction(index, updatedTransaction);
        FileManager::saveToFile(manager.getTransactions(), filename);
        std::cout << "Transaction updated and file saved.\n";

    } else {
        std::cout << "Invalid index! No transaction edited.\n";
    }
}

void AppUI::removeTransaction() {
    int index;
    std::cout << "Enter the index of the transaction to remove: ";
    std::cin >> index;

    const auto& list = manager.getTransactions();
    if (index >= 0 && index < static_cast<int>(list.size())) {
        std::cout << "\n=== Selected Transaction ===\n";
        list[index].print();
        std::cout << "Are you sure you want to remove this transaction? (y/n): ";
        char confirm;
        std::cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            manager.removeTransaction(index);
            FileManager::saveToFile(manager.getTransactions(), filename);
            std::cout << "Transaction removed and file updated.\n";
        } else {
            std::cout << "Removal canceled.\n";
        }
    } else {
        std::cout << "Invalid index! No transaction removed.\n";
    }
}

void AppUI::calculateTotalBalance() {
    double balance = analyzer.calculateBalance(manager.getTransactions());
    std::cout << "Total balance: " << balance << std::endl;
}

void AppUI::totalByCategory() {
    std::string category;
    std::cout << "Enter category: ";
    std::cin >> category;
    double total = analyzer.calculateTotalByCategory(manager.getTransactions(), category);
    std::cout << "Total for category '" << category << "': " << total << std::endl;
}

void AppUI::totalByType() {
    std::string type;
    std::cout << "Enter type (renda/despesa): ";
    std::cin >> type;
    double total = analyzer.calculateTotalByType(manager.getTransactions(), type);
    std::cout << "Total for type '" << type << "': " << total << std::endl;
}

void AppUI::filterByDateRange() {
    std::string start, end;
    std::cout << "Enter start date (YYYY-MM-DD): ";
    std::cin >> start;
    std::cout << "Enter end date (YYYY-MM-DD): ";
    std::cin >> end;

    auto results = manager.filterByDateRange(start, end);

    if (results.empty()) {
        std::cout << "No transactions found in the date range.\n";
    } else {
        for (const auto& t : results) {
            t.print();
        }
    }
}

void AppUI::filterByAmountRange() {
    double min, max;
    std::cout << "Enter minimum amount: ";
    std::cin >> min;
    std::cout << "Enter maximum amount: ";
    std::cin >> max;

    auto results = manager.filterByAmountRange(min, max);

    if (results.empty()) {
        std::cout << "No transactions found in the amount range.\n";
    } else {
        for (const auto& t : results) {
            t.print();
        }
    }
}

void AppUI::switchTransactionFile() {
    manager = TransactionManager();
    std::cout << "\n==== File Switch ====\n";
    std::cout << "1 - Create new file\n";
    std::cout << "2 - Load existing file\n";
    std::cout << "Choose an option: ";
    int fileOption;
    std::cin >> fileOption;

    if (fileOption == 1) {
        std::cout << "Enter base name for the new file: ";
        std::cin >> filename;
        filename = FileManager::generateUniqueFilename(filename);
        std::cout << "New file created: '" << filename << "'\n";
    } else if (fileOption == 2) {
        std::cout << "\nAvailable CSV files:\n";
        DIR* dir;
        struct dirent* entry;
        dir = opendir(".");
        if (dir != nullptr) {
            while ((entry = readdir(dir)) != nullptr) {
                std::string filenameEntry = entry->d_name;
                if (filenameEntry.length() >= 4 && filenameEntry.substr(filenameEntry.length() - 4) == ".csv") {
                    std::cout << "- " << filenameEntry << std::endl;
                }
            }
            closedir(dir);
        }

        std::cout << "Enter existing file name: ";
        std::cin >> filename;
        filename = FileManager::ensureCSVExtension(filename);

        if (FileManager::fileExists(filename)) {
            std::vector<Transaction> loaded;
            if (FileManager::loadFromFile(loaded, filename)) {
                for (const auto& t : loaded) {
                    manager.addTransaction(t);
                }
                std::cout << "Transactions loaded from '" << filename << "'\n";
            } else {
                std::cout << "Error loading file.\n";
                filename.clear();
            }
        } else {
            std::cout << "File not found.\n";
            filename.clear();
        }
    } else {
        std::cout << "Invalid option.\n";
        filename.clear();
    }
}

void AppUI::deleteTransactionFile() {
    std::cout << "\nAvailable CSV files:\n";
    DIR* dir;
    struct dirent* entry;
    dir = opendir(".");
    if (dir != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            std::string filenameEntry = entry->d_name;
            if (filenameEntry.length() >= 4 && filenameEntry.substr(filenameEntry.length() - 4) == ".csv") {
                std::cout << "- " << filenameEntry << std::endl;
            }
        }
        closedir(dir);
    }

    std::cout << "Enter the name of the file to delete: ";
    std::string fileToDelete;
    std::cin >> fileToDelete;
    fileToDelete = FileManager::ensureCSVExtension(fileToDelete);

    if (FileManager::fileExists(fileToDelete)) {
        std::cout << "Are you sure you want to permanently delete the file '" << fileToDelete << "'? (y/n): ";
        char confirm;
        std::cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            if (FileManager::deleteFile(fileToDelete)) {
                std::cout << "File '" << fileToDelete << "' deleted successfully.\n";
                if (fileToDelete == filename) {
                    manager = TransactionManager();
                    filename.clear();
                    std::cout << "You deleted the currently loaded file. Current session is now cleared.\n";
                }
            } else {
                std::cout << "Error: Could not delete the file.\n";
            }
        } else {
            std::cout << "Deletion canceled.\n";
        }
    } else {
        std::cout << "File '" << fileToDelete << "' not found.\n";
    }
}

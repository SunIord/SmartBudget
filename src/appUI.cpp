#include "AppUI.hpp"
using namespace smartbudget;

void AppUI::run() {
    initialSetup();
    int option = 0;
    do {
        showMainMenu();
        cin >> option;
        handleMenuOption(option);
    } while (option != 0);
}

void AppUI::initialSetup() {
    cout << "==== SmartBudget - Initial Setup ====\n";
    cout << "1 - Create new file\n";
    cout << "2 - Load existing file\n";
    cout << "0 - Exit\n";
    cout << "Choose an option: ";
    int initialOption;
    cin >> initialOption;

    if (initialOption == 1) {
        cout << "Enter base name for the new file: ";
        cin >> filename;
        filename = FileManager::generateUniqueFilename(filename);
        cout << "New file created: '" << filename << "'\n";
    } else if (initialOption == 2) {
        cout << "\nAvailable CSV files:\n";
        DIR* dir;
        struct dirent* entry;
        dir = opendir(".");
        if (dir != nullptr) {
            while ((entry = readdir(dir)) != nullptr) {
                string filenameEntry = entry->d_name;
                if (filenameEntry.length() >= 4 && filenameEntry.substr(filenameEntry.length() - 4) == ".csv") {
                    cout << "- " << filenameEntry << endl;
                }
            }
            closedir(dir);
        }

        cout << "Enter existing file name: ";
        cin >> filename;
        filename = FileManager::ensureCSVExtension(filename);

        if (FileManager::fileExists(filename)) {
            vector<Transaction> loaded;
            if (FileManager::loadFromFile(loaded, filename)) {
                for (const auto& t : loaded) {
                    manager.addTransaction(t);
                }
                cout << "Transactions loaded from '" << filename << "'\n";
            } else {
                cout << "Error loading file.\n";
                filename.clear();
            }
        } else {
            cout << "File not found.\n";
            filename.clear();
        }
    } else if (initialOption == 0) {
        cout << "Exiting program...\n";
        exit(0);
    } else {
        cout << "Invalid option.\n";
        filename.clear();
        exit(0);
    }
}

void AppUI::showMainMenu() {
    cout << "\n==== SmartBudget - Menu ====\n";
    
    if (!filename.empty()) {
        cout << "[Current file: " << filename << "]\n";
    } else {
        cout << "[No file currently loaded]\n";
    }

    cout << "1. Add transaction\n";
    cout << "2. List transactions\n";
    cout << "3. Edit transaction\n";
    cout << "4. Remove transaction\n";
    cout << "5. Calculate total balance\n";
    cout << "6. Total by category\n";
    cout << "7. Total by type\n";
    cout << "8. Filter by date range\n";
    cout << "9. Filter by amount range\n";
    cout << "10. Switch transaction file\n";
    cout << "11. Delete transaction file\n";
    cout << "0. Exit\n";
    cout << "Choose an option: ";
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
        case 0: cout << "Exiting program...\n"; break;
        default: cout << "Invalid option!\n"; break;
    }
}

void AppUI::addTransaction() {
    if (filename.empty()) {
        cout << "No file selected. Use option 10 to choose a file.\n";
        return;
    }

    double amount;
    string type, category, date, description;

    cout << "Enter amount: ";
    cin >> amount;
    cout << "Enter type (renda/despesa): ";
    cin >> type;
    cout << "Enter category: ";
    cin >> category;
    cin.ignore();
    cout << "Enter date (YYYY-MM-DD): ";
    getline(cin, date);
    cout << "Enter description: ";
    getline(cin, description);

    Transaction newTransaction(amount, type, category, date, description);
    manager.addTransaction(newTransaction);
    FileManager::saveToFile(manager.getTransactions(), filename);
    cout << "Transaction added and saved successfully!\n";
}

void AppUI::listTransactions() {
    const auto& list = manager.getTransactions();
    if (list.empty()) {
        cout << "No transactions recorded.\n";
    } else {
        cout << "\n=== Transaction List ===\n";
        for (size_t i = 0; i < list.size(); i++) {
            cout << "[" << i << "] ";
            list[i].print();
        }
    }
}

void AppUI::editTransaction() {
    const auto& list = manager.getTransactions();
    if (list.empty()) {
        cout << "No transactions to edit.\n";
        return;
    }

    int index;
    cout << "Enter the index of the transaction to edit: ";
    cin >> index;

    if (index >= 0 && index < static_cast<int>(list.size())) {
        cout << "\n=== Selected Transaction ===\n";
        list[index].print();

        double amount;
        string type, category, date, description;

        cout << "Enter new amount (current: " << list[index].getAmount() << "): ";
        cin >> amount;
        cout << "Enter new type (renda/despesa) (current: " << list[index].getType() << "): ";
        cin >> type;
        cout << "Enter new category (current: " << list[index].getCategory() << "): ";
        cin >> category;
        cin.ignore();
        cout << "Enter new date (YYYY-MM-DD) (current: " << list[index].getDate() << "): ";
        getline(cin, date);
        cout << "Enter new description (current: " << list[index].getDescription() << "): ";
        getline(cin, description);

        Transaction updatedTransaction(amount, type, category, date, description);
        manager.updateTransaction(index, updatedTransaction);
        FileManager::saveToFile(manager.getTransactions(), filename);
        cout << "Transaction updated and file saved.\n";

    } else {
        cout << "Invalid index! No transaction edited.\n";
    }
}

void AppUI::removeTransaction() {
    int index;
    cout << "Enter the index of the transaction to remove: ";
    cin >> index;

    const auto& list = manager.getTransactions();
    if (index >= 0 && index < static_cast<int>(list.size())) {
        cout << "\n=== Selected Transaction ===\n";
        list[index].print();
        cout << "Are you sure you want to remove this transaction? (y/n): ";
        char confirm;
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            manager.removeTransaction(index);
            FileManager::saveToFile(manager.getTransactions(), filename);
            cout << "Transaction removed and file updated.\n";
        } else {
            cout << "Removal canceled.\n";
        }
    } else {
        cout << "Invalid index! No transaction removed.\n";
    }
}

void AppUI::calculateTotalBalance() {
    double balance = analyzer.calculateBalance(manager.getTransactions());
    cout << "Total balance: " << balance << endl;
}

void AppUI::totalByCategory() {
    string category;
    cout << "Enter category: ";
    cin >> category;
    double total = analyzer.calculateTotalByCategory(manager.getTransactions(), category);
    cout << "Total for category '" << category << "': " << total << endl;
}

void AppUI::totalByType() {
    string type;
    cout << "Enter type (renda/despesa): ";
    cin >> type;
    double total = analyzer.calculateTotalByType(manager.getTransactions(), type);
    cout << "Total for type '" << type << "': " << total << endl;
}

void AppUI::filterByDateRange() {
    string start, end;
    cout << "Enter start date (YYYY-MM-DD): ";
    cin >> start;
    cout << "Enter end date (YYYY-MM-DD): ";
    cin >> end;

    auto results = manager.filterByDateRange(start, end);

    if (results.empty()) {
        cout << "No transactions found in the date range.\n";
    } else {
        for (const auto& t : results) {
            t.print();
        }
    }
}

void AppUI::filterByAmountRange() {
    double min, max;
    cout << "Enter minimum amount: ";
    cin >> min;
    cout << "Enter maximum amount: ";
    cin >> max;

    auto results = manager.filterByAmountRange(min, max);

    if (results.empty()) {
        cout << "No transactions found in the amount range.\n";
    } else {
        for (const auto& t : results) {
            t.print();
        }
    }
}

void AppUI::switchTransactionFile() {
    manager = TransactionManager();
    cout << "\n==== File Switch ====\n";
    cout << "1 - Create new file\n";
    cout << "2 - Load existing file\n";
    cout << "Choose an option: ";
    int fileOption;
    cin >> fileOption;

    if (fileOption == 1) {
        cout << "Enter base name for the new file: ";
        cin >> filename;
        filename = FileManager::generateUniqueFilename(filename);
        cout << "New file created: '" << filename << "'\n";
    } else if (fileOption == 2) {
        cout << "\nAvailable CSV files:\n";
        DIR* dir;
        struct dirent* entry;
        dir = opendir(".");
        if (dir != nullptr) {
            while ((entry = readdir(dir)) != nullptr) {
                string filenameEntry = entry->d_name;
                if (filenameEntry.length() >= 4 && filenameEntry.substr(filenameEntry.length() - 4) == ".csv") {
                    cout << "- " << filenameEntry << endl;
                }
            }
            closedir(dir);
        }

        cout << "Enter existing file name: ";
        cin >> filename;
        filename = FileManager::ensureCSVExtension(filename);

        if (FileManager::fileExists(filename)) {
            vector<Transaction> loaded;
            if (FileManager::loadFromFile(loaded, filename)) {
                for (const auto& t : loaded) {
                    manager.addTransaction(t);
                }
                cout << "Transactions loaded from '" << filename << "'\n";
            } else {
                cout << "Error loading file.\n";
                filename.clear();
            }
        } else {
            cout << "File not found.\n";
            filename.clear();
        }
    } else {
        cout << "Invalid option.\n";
        filename.clear();
    }
}

void AppUI::deleteTransactionFile() {
    cout << "\nAvailable CSV files:\n";
    DIR* dir;
    struct dirent* entry;
    dir = opendir(".");
    if (dir != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            string filenameEntry = entry->d_name;
            if (filenameEntry.length() >= 4 && filenameEntry.substr(filenameEntry.length() - 4) == ".csv") {
                cout << "- " << filenameEntry << endl;
            }
        }
        closedir(dir);
    }

    cout << "Enter the name of the file to delete: ";
    string fileToDelete;
    cin >> fileToDelete;
    fileToDelete = FileManager::ensureCSVExtension(fileToDelete);

    if (FileManager::fileExists(fileToDelete)) {
        cout << "Are you sure you want to permanently delete the file '" << fileToDelete << "'? (y/n): ";
        char confirm;
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            if (FileManager::deleteFile(fileToDelete)) {
                cout << "File '" << fileToDelete << "' deleted successfully.\n";
                if (fileToDelete == filename) {
                    manager = TransactionManager();
                    filename.clear();
                    cout << "You deleted the currently loaded file. Current session is now cleared.\n";
                }
            } else {
                cout << "Error: Could not delete the file.\n";
            }
        } else {
            cout << "Deletion canceled.\n";
        }
    } else {
        cout << "File '" << fileToDelete << "' not found.\n";
    }
}
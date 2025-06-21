// cd src
// g++ -std=c++11 -I../include -o smartbudget main2.cpp transaction.cpp transactionManager.cpp budgetAnalyzer.cpp fileManager.cpp
// ./smartbudget

#include "transaction.hpp"
#include "transactionManager.hpp"
#include "budgetAnalyzer.hpp"
#include "fileManager.hpp"
using namespace smartbudget;

int main() {
    TransactionManager manager;
    BudgetAnalyzer analyzer;
    string filename;

    cout << "==== SmartBudget - Initial Setup ====\n";
    cout << "1 - Create new file\n";
    cout << "2 - Load existing file\n";
    cout << "Choose an option: ";
    int initialOption;
    cin >> initialOption;

    if (initialOption == 1) {
        cout << "Enter base name for the new file (without .csv): ";
        cin >> filename;
        filename = FileManager::generateUniqueFilename(filename);
        cout << "New file created: '" << filename << "'\n";
    } else if (initialOption == 2) {
        cout << "Enter existing file name (without .csv): ";
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
            }
        } else {
            cout << "File not found.\n";
            filename.clear();
        }
    } else {
        cout << "Invalid option.\n";
        filename.clear();
    }

    int option = 0;

    do {
        cout << "\n==== SmartBudget - Menu ====\n";
        cout << "1. Add transaction\n";
        cout << "2. List transactions\n";
        cout << "3. Remove transaction\n";
        cout << "4. Calculate total balance\n";
        cout << "5. Total by category\n";
        cout << "6. Total by type\n";
        cout << "7. Filter by date range\n";
        cout << "8. Filter by amount range\n";
        cout << "9. Delete transaction file\n";
        cout << "10. Switch transaction file\n";
        cout << "0. Exit\n";
        cout << "Choose an option: ";
        cin >> option;

        if (option == 1) {
            if (filename.empty()) {
                cout << "No file selected. Use option 10 to choose a file.\n";
                continue;
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

        else if (option == 2) {
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

        else if (option == 3) {
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

        else if (option == 4) {
            double balance = analyzer.calculateBalance(manager.getTransactions());
            cout << "Total balance: " << balance << endl;
        }

        else if (option == 5) {
            string category;
            cout << "Enter category: ";
            cin >> category;
            double total = analyzer.calculateTotalByCategory(manager.getTransactions(), category);
            cout << "Total for category '" << category << "': " << total << endl;
        }

        else if (option == 6) {
            string type;
            cout << "Enter type (renda/despesa): ";
            cin >> type;
            double total = analyzer.calculateTotalByType(manager.getTransactions(), type);
            cout << "Total for type '" << type << "': " << total << endl;
        }

        else if (option == 7) {
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

        else if (option == 8) {
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

        else if (option == 9) {
            cout << "Enter the name of the file to delete (without .csv): ";
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
                        cout << "Error: Could not delete the file. It may not exist.\n";
                    }
                } else {
                    cout << "Deletion canceled.\n";
                }
            } else {
                cout << "File '" << fileToDelete << "' not found.\n";
            }
        }

        else if (option == 10) {
            manager = TransactionManager();
            cout << "\n==== File Switch ====\n";
            cout << "1 - Create new file\n";
            cout << "2 - Load existing file\n";
            cout << "Choose an option: ";
            int fileOption;
            cin >> fileOption;

            if (fileOption == 1) {
                cout << "Enter base name for the new file (without .csv): ";
                cin >> filename;
                filename = FileManager::generateUniqueFilename(filename);
                cout << "New file created: '" << filename << "'\n";
            } else if (fileOption == 2) {
                cout << "Enter existing file name (without .csv): ";
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

        else if (option == 0) {
            cout << "Exiting program...\n";
        }

        else {
            cout << "Invalid option!\n";
        }

    } while (option != 0);
    
    return 0;
}
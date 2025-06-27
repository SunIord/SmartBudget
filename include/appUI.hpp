#ifndef APPUI_HPP
#define APPUI_HPP

#include "transaction.hpp"
#include "transactionManager.hpp"
#include "budgetAnalyzer.hpp"
#include "fileManager.hpp"

class AppUI {
private:
    TransactionManager manager;
    BudgetAnalyzer analyzer;
    std::string filename;

    void initialSetup();
    void showMainMenu();
    void handleMenuOption(int option);
    void addTransaction();
    void listTransactions();
    void removeTransaction();
    void calculateTotalBalance();
    void totalByCategory();
    void totalByType();
    void filterByDateRange();
    void filterByAmountRange();
    void deleteTransactionFile();
    void switchTransactionFile();
    void editTransaction();

public:
    void run();
};

#endif
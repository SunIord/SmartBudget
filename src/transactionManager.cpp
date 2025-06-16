#include "transactionManager.hpp"
using namespace smartbudget;

void TransactionManager::addTransaction(const Transaction& t) {
    transactions.push_back(t);
}

bool TransactionManager::removeTransaction(size_t index) {
    if (index < transactions.size()) {
        transactions.erase(transactions.begin() + index);
        return true;
    }
    return false;
}

const vector<Transaction>& TransactionManager::getTransactions() const {
    return transactions;
}

vector<Transaction> TransactionManager::filterByType(const string& type) const {
    vector<Transaction> filtered;
    for (const auto& transaction : transactions) {
        if (transaction.getType() == type) {
            filtered.push_back(transaction);
        }
    }
    return filtered;
}

vector<Transaction> TransactionManager::filterByCategory(const string& category) const {
    vector<Transaction> filtered;
    for (const auto& transaction : transactions) {
        if (transaction.getCategory() == category) {
            filtered.push_back(transaction);
        }
    }
    return filtered;
}

vector<Transaction> TransactionManager::filterByDateRange(const string& startDate, const string& endDate) const {
    vector<Transaction> filtered;

    for (const auto& transaction : transactions) {
        const string& date = transaction.getDate();
        if (date >= startDate && date <= endDate) {
            filtered.push_back(transaction);
        }
    }

    return filtered;
}

vector<Transaction> TransactionManager::filterByAmountRange(double minAmount, double maxAmount) const {
    vector<Transaction> filtered;

    for (const auto& transaction : transactions) {
        if (transaction.getAmount() >= minAmount && transaction.getAmount() <= maxAmount) {
            filtered.push_back(transaction);
        }
    }

    return filtered;
}

bool TransactionManager::updateTransaction(int index, const Transaction& newTransaction) {
    if (index >= 0 && index < transactions.size()) {
        transactions[index] = newTransaction;
        return true;
    }
    return false;
}
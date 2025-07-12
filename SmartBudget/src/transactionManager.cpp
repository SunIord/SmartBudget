#include "transactionManager.hpp"

// Adiciona uma nova transação à lista de transações
void TransactionManager::addTransaction(const Transaction& t) {
    transactions.push_back(t);
}

// Remove uma transação pelo índice
bool TransactionManager::removeTransaction(size_t index) {
    if (index < transactions.size()) {
        transactions.erase(transactions.begin() + index);
        return true;
    }
    return false;
}

// Retorna uma referência constante de todas as transações armazenadas
const std::vector<Transaction>& TransactionManager::getTransactions() const {
    return transactions;
}

// Retorna uma lista contendo apenas transações do tipo especificado
std::vector<Transaction> TransactionManager::filterByType(const std::string& type) const {
    std::vector<Transaction> filtered;
    for (const auto& transaction : transactions) {
        if (transaction.getType() == type) {
            filtered.push_back(transaction);
        }
    }
    return filtered;
}

// Retorna uma lista contendo apenas transações da categoria especificada
std::vector<Transaction> TransactionManager::filterByCategory(const std::string& category) const {
    std::vector<Transaction> filtered;
    for (const auto& transaction : transactions) {
        if (transaction.getCategory() == category) {
            filtered.push_back(transaction);
        }
    }
    return filtered;
}

// Retorna uma lista de transações cuja data esteja dentro do intervalo especificado
std::vector<Transaction> TransactionManager::filterByDateRange(const std::string& startDate, const std::string& endDate) const {
    std::vector<Transaction> filtered;

    for (const auto& transaction : transactions) {
        const std::string& date = transaction.getDate();
        if (date >= startDate && date <= endDate) {
            filtered.push_back(transaction);
        }
    }

    return filtered;
}

// Retorna uma lista de transações cujo valor esteja dentro do intervalo especificado
std::vector<Transaction> TransactionManager::filterByAmountRange(double minAmount, double maxAmount) const {
    std::vector<Transaction> filtered;

    for (const auto& transaction : transactions) {
        if (transaction.getAmount() >= minAmount && transaction.getAmount() <= maxAmount) {
            filtered.push_back(transaction);
        }
    }

    return filtered;
}

// Atualiza uma transação existente no índice especificado com os novos dados
bool TransactionManager::updateTransaction(int index, const Transaction& newTransaction) {
    if (index >= 0 && index < transactions.size()) {
        transactions[index] = newTransaction;
        return true;
    }
    return false;
}
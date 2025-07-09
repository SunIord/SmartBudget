#ifndef TRANSACTIONMANAGER_HPP
#define TRANSACTIONMANAGER_HPP

#include "transaction.hpp"

class TransactionManager {
private:
    std::vector<Transaction> transactions;

public:
    void addTransaction(const Transaction& t);
    bool removeTransaction(size_t index);
    std::vector<Transaction> filterByType(const std::string& type) const;
    std::vector<Transaction> filterByCategory(const std::string& category) const;
    std::vector<Transaction> filterByDateRange(const std::string& startDate, const std::string& endDate) const;
    std::vector<Transaction> filterByAmountRange(double minAmount, double maxAmount) const;
    bool updateTransaction(int index, const Transaction& newTransaction);
    const std::vector<Transaction>& getTransactions() const;
};

#endif
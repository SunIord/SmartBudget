#ifndef TRANSACTIONMANAGER_HPP
#define TRANSACTIONMANAGER_HPP

#include "transaction.hpp"
#include <vector>

class TransactionManager {
private:
    vector<Transaction> transactions;

public:
    void addTransaction(const Transaction& t);
    bool removeTransaction(size_t index);
    void listTransactions() const;
    void filterByType(const string& type) const;
    void filterByCategory(const string& category) const;
    void updateTransaction(int index, const Transaction& newTransaction);
    const vector<Transaction>& getTransactions() const;
};

#endif

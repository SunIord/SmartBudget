#ifndef TRANSACTIONMANAGER_HPP
#define TRANSACTIONMANAGER_HPP

#include "transaction.hpp"

namespace smartbudget
{

    class TransactionManager
    {
    private:
        vector<Transaction> transactions;

    public:
        void addTransaction(const Transaction &t);
        bool removeTransaction(size_t index);
        void listTransactions() const;
        vector<Transaction> filterByType(const string &type) const;
        vector<Transaction> filterByCategory(const string &category) const;
        vector<Transaction> filterByDateRange(const string &startDate, const string &endDate) const;
        vector<Transaction> filterByAmountRange(double minAmount, double maxAmount) const;
        bool updateTransaction(int index, const Transaction &newTransaction);
        const vector<Transaction> &getTransactions() const;
    };

}

#endif
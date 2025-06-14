#ifndef BUDGETANALYZER_HPP
#define BUDGETANALYZER_HPP

#include "transaction.hpp"
#include <vector>

class BudgetAnalyzer {
public:
    double calculateBalance(const vector<Transaction>& transactions) const;
    double calculateTotalByCategory(const vector<Transaction>& transactions, const string& category) const;
    double calculateTotalByType(const vector<Transaction>& transactions, const string& type) const;
};

#endif
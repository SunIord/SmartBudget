#ifndef BUDGETANALYZER_HPP
#define BUDGETANALYZER_HPP

#include "transaction.hpp"

class BudgetAnalyzer {
public:
    double calculateBalance(const std::vector<Transaction>& transactions) const;
    double calculateTotalByCategory(const std::vector<Transaction>& transactions, const std::string& category) const;
    double calculateTotalByType(const std::vector<Transaction>& transactions, const std::string& type) const;
};

#endif
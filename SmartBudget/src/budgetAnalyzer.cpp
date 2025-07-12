#include "budgetAnalyzer.hpp"

// Calcula o saldo total com base nas transações registradas
double BudgetAnalyzer::calculateBalance(const std::vector<Transaction>& transactions) const {
    double balance = 0.0;
    for (const auto& transaction : transactions) {
        if (transaction.getType() == "renda") {
            balance += transaction.getAmount();
        } else if (transaction.getType() == "despesa") {
            balance -= transaction.getAmount();
        }
    }
    return balance;
}

// Calcula o total de transações por categoria
double BudgetAnalyzer::calculateTotalByCategory(const std::vector<Transaction>& transactions, const std::string& category) const {
    double total = 0.0;
    for (const auto& transaction : transactions) {
        if (transaction.getCategory() == category) {
            if (transaction.getType() == "renda") {
                total += transaction.getAmount();
            } else if (transaction.getType() == "despesa") {
                total -= transaction.getAmount();
            }
        }
    }
    return total;
}

// Parecido com o método anterior, mas calcula o total de transações por tipo (renda ou despesa)
double BudgetAnalyzer::calculateTotalByType(const std::vector<Transaction>& transactions, const std::string& type) const {
    double total = 0.0;
    for (const auto& transaction : transactions) {
        if (transaction.getType() == type) {
            total += transaction.getAmount();
        }
    }
    return total;
}
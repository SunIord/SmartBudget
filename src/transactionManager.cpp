#include "transactionManager.hpp"

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

void TransactionManager::listTransactions() const {
    if (transactions.empty()) {
        cout << "Nenhuma transacao cadastrada." << endl;
        return;
    }

    for (size_t i = 0; i < transactions.size(); ++i) {
        const Transaction& t = transactions[i];
        cout << "[" << i << "] "
             << t.getDate() << " | "
             << t.getType() << " | "
             << t.getCategory() << " | "
             << t.getAmount() << " | "
             << t.getDescription() << endl;
    }

}

void TransactionManager::filterByType(const std::string& type) const {
    cout << "Transacoes do tipo: " << type << endl;
    for (const auto& transaction : transactions) {
        if (transaction.getType() == type) {
            transaction.print();
        }
    }
}

void TransactionManager::filterByCategory(const std::string& category) const {
    cout << "Transacoes da categoria: " << category << endl;
    for (const auto& transaction : transactions) {
        if (transaction.getCategory() == category) {
            transaction.print();
        }
    }
}

void TransactionManager::updateTransaction(int index, const Transaction& newTransaction) {
    if (index >= 0 && index < transactions.size()) {
        transactions[index] = newTransaction;
        cout << "Transacao atualizada com sucesso." << endl;
    } else {
        cout << "Indice invalido. Nenhuma transacao foi atualizada." << endl;
    }
}

const vector<Transaction>& TransactionManager::getTransactions() const {
    return transactions;
}

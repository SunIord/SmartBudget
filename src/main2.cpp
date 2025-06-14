//g++ -std=c++11 -I../include -o smartbudget main2.cpp transaction.cpp transactionManager.cpp
//./smartbudget
#include "transaction.hpp"
#include "transactionManager.hpp"

int main() {
    TransactionManager manager;

    Transaction t1(100.0, "renda", "salario", "2025-06-14", "Pagamento Junho");
    Transaction t2(50.0, "despesa", "mercado", "2025-06-15", "Compras");

    manager.addTransaction(t1);
    manager.addTransaction(t2);

    cout << "\nTodas as transacoes:" << endl;
    manager.listTransactions();

    cout << "\nFiltrar por tipo (renda):" << endl;
    manager.filterByType("renda");

    cout << "\nFiltrar por categoria (mercado):" << endl;
    manager.filterByCategory("mercado");

    Transaction nova(200.0, "renda", "freelance", "2025-06-16", "Projeto X");
    manager.updateTransaction(0, nova);

    cout << "\nApos atualizacao:" << endl;
    manager.listTransactions();

    cout << "\nRemovendo a segunda transacao (indice 1):" << endl;
    manager.removeTransaction(1);

    cout << "\nLista apos remocao:" << endl;
    manager.listTransactions();
    
    return 0;
}
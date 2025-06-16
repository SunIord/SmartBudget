//cd src
//g++ -std=c++11 -I../include -o smartbudget main2.cpp transaction.cpp transactionManager.cpp budgetAnalyzer.cpp
//./smartbudget
#include "transaction.hpp"
#include "transactionManager.hpp"
#include "budgetAnalyzer.hpp"
using namespace smartbudget;

int main() {
    TransactionManager manager;
    BudgetAnalyzer analyzer;

    int opcao = 0;

    do {
        cout << "\n==== SmartBudget - Menu ====\n";
        cout << "1. Adicionar transacao\n";
        cout << "2. Listar transacoes\n";
        cout << "3. Remover transacao\n";
        cout << "4. Calcular saldo total\n";
        cout << "5. Total por categoria\n";
        cout << "6. Total por tipo\n";
        cout << "7. Filtrar por intervalo de datas\n";
        cout << "8. Filtrar por intervalo de valores\n";
        cout << "0. Sair\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        if (opcao == 1) {
            double amount;
            string type, category, date, description;

            cout << "Digite o valor: ";
            cin >> amount;
            cout << "Digite o tipo (renda/despesa): ";
            cin >> type;
            cout << "Digite a categoria: ";
            cin >> category;
            cin.ignore();
            cout << "Digite a data (YYYY-MM-DD): ";
            getline(cin, date);
            cout << "Digite a descricao: ";
            getline(cin, description);

            Transaction nova(amount, type, category, date, description);
            manager.addTransaction(nova);
            cout << "Transacao adicionada com sucesso!\n";
        }

        else if (opcao == 2) {
            const auto& lista = manager.getTransactions();
            if (lista.empty()) {
                cout << "Nenhuma transacao cadastrada.\n";
            } else {
                cout << "\n=== Lista de Transacoes ===\n";
                for (size_t i = 0; i < lista.size(); i++) {
                    cout << "[" << i << "] ";
                    lista[i].print();
                }
            }
        }

        else if (opcao == 3) {
            int index;
            cout << "Digite o indice da transacao a remover: ";
            cin >> index;
            manager.removeTransaction(index);
        }

        else if (opcao == 4) {
            double saldo = analyzer.calculateBalance(manager.getTransactions());
            cout << "Saldo total: " << saldo << endl;
        }

        else if (opcao == 5) {
            string categoria;
            cout << "Digite a categoria: ";
            cin >> categoria;
            double total = analyzer.calculateTotalByCategory(manager.getTransactions(), categoria);
            cout << "Total da categoria '" << categoria << "': " << total << endl;
        }

        else if (opcao == 6) {
            string tipo;
            cout << "Digite o tipo (renda/despesa): ";
            cin >> tipo;
            double total = analyzer.calculateTotalByType(manager.getTransactions(), tipo);
            cout << "Total do tipo '" << tipo << "': " << total << endl;
        }

        else if (opcao == 7) {
            string inicio, fim;
            cout << "Digite a data inicial (YYYY-MM-DD): ";
            cin >> inicio;
            cout << "Digite a data final (YYYY-MM-DD): ";
            cin >> fim;

            auto resultados = manager.filterByDateRange(inicio, fim);

            if (resultados.empty()) {
                cout << "Nenhuma transacao encontrada no intervalo de datas.\n";
            } else {
                for (const auto& t : resultados) {
                    t.print();
                }
            }
        }

        else if (opcao == 8) {
            double minimo, maximo;
            cout << "Digite o valor minimo: ";
            cin >> minimo;
            cout << "Digite o valor maximo: ";
            cin >> maximo;

            auto resultados = manager.filterByAmountRange(minimo, maximo);

            if (resultados.empty()) {
                cout << "Nenhuma transacao encontrada no intervalo de valores.\n";
            } else {
                for (const auto& t : resultados) {
                    t.print();
                }
            }
        }

        else if (opcao == 0) {
            cout << "Encerrando o programa...\n";
        }

        else {
            cout << "Opcao invalida!\n";
        }

    } while (opcao != 0);

    return 0;
}
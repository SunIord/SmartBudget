// cd src
// g++ -std=c++11 -I../include -o smartbudget main2.cpp transaction.cpp transactionManager.cpp budgetAnalyzer.cpp fileManager.cpp appUi.cpp
// ./smartbudget

#include "appUI.hpp"
using namespace smartbudget;

int main() {
    AppUI app;
    app.run();
    return 0;
}
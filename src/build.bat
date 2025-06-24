@echo off
g++ -std=c++11 -I../include -o smartbudget main2.cpp transaction.cpp transactionManager.cpp budgetAnalyzer.cpp fileManager.cpp appUi.cpp

#include "transaction.hpp"
using namespace smartbudget;

Transaction::Transaction(double amount, const string& type, const string& category, const string& date, const string& description)
    : amount(amount), type(type), category(category), date(date), description(description) {}

double Transaction::getAmount() const { return amount; }
string Transaction::getType() const { return type; }
string Transaction::getCategory() const { return category; }
string Transaction::getDate() const { return date; }
string Transaction::getDescription() const { return description; }

void Transaction::setAmount(double newAmount) { amount = newAmount; }
void Transaction::setType(const string& newType) { type = newType; }
void Transaction::setCategory(const string& newCategory) { category = newCategory; }
void Transaction::setDate(const string& newDate) { date = newDate; }
void Transaction::setDescription(const string& newDescription) { description = newDescription; }
//função apenas para imprimir os dados da transação na main2.cpp
void Transaction::print() const {
    cout << "Valor: " << amount << endl;
    cout << "Tipo: " << type << endl;
    cout << "Categoria: " << category << endl;
    cout << "Data: " << date << endl;
    cout << "Descricao: " << description << endl;
    cout << "------------------------------" << endl;
}

//função para converter os dados da transação em uma string no formato CSV
string Transaction::toCSV() const {
    return to_string(amount) + "," + type + "," + category + "," + date + "," + description;
}
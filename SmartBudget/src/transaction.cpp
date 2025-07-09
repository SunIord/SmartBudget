#include "transaction.hpp"

Transaction::Transaction(double amount, const std::string& type, const std::string& category, const std::string& date, const std::string& description)
    : amount(amount), type(type), category(category), date(date), description(description) {}

double Transaction::getAmount() const { return amount; }
std::string Transaction::getType() const { return type; }
std::string Transaction::getCategory() const { return category; }
std::string Transaction::getDate() const { return date; }
std::string Transaction::getDescription() const { return description; }

void Transaction::setAmount(double newAmount) { amount = newAmount; }
void Transaction::setType(const std::string& newType) { type = newType; }
void Transaction::setCategory(const std::string& newCategory) { category = newCategory; }
void Transaction::setDate(const std::string& newDate) { date = newDate; }
void Transaction::setDescription(const std::string& newDescription) { description = newDescription; }
//função apenas para imprimir os dados da transação na main2.cpp
void Transaction::print() const {
    std::cout << "Valor: " << amount << std::endl;
    std::cout << "Tipo: " << type << std::endl;
    std::cout << "Categoria: " << category << std::endl;
    std::cout << "Data: " << date << std::endl;
    std::cout << "Descricao: " << description << std::endl;
    std::cout << "------------------------------" << std::endl;
}

//função para converter os dados da transação em uma std::string no formato CSV
std::string Transaction::toCSV() const {
    auto escape = [](const std::string& str) {
        bool needsQuotes = str.find(',') != std::string::npos || str.find('"') != std::string::npos || str.find('\n') != std::string::npos;
        if (needsQuotes) {
            std::string escaped = "\"";
            for (char c : str) {
                if (c == '"') escaped += "\"\"";  // Aspas internas viram dupla
                else escaped += c;
            }
            escaped += "\"";
            return escaped;
        } else {
            return str;
        }
    };

    return std::to_string(amount) + "," + escape(type) + "," + escape(category) + "," + escape(date) + "," + escape(description);
}
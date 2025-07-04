#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>  // Para dividir strings por delimitadores
#include <fstream>  // Para leitura e escrita em arquivos
#include <dirent.h> // Para listar arquivos em diretórios

class Transaction {
private:
    double amount;
    std::string type;
    std::string category;
    std::string date;
    std::string description;

public:
    Transaction(double amount, const std::string& type, const std::string& category,
                const std::string& date, const std::string& description);

    double getAmount() const;
    std::string getType() const;
    std::string getCategory() const;
    std::string getDate() const;
    std::string getDescription() const;
    void setAmount(double newAmount);
    void setType(const std::string& newType);
    void setCategory(const std::string& newCategory);
    void setDate(const std::string& newDate);
    void setDescription(const std::string& newDescription);
    std::string toCSV() const;
};

#endif
#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <iostream>
#include <string>
using namespace std; 

class Transaction {
private:
    double amount;
    string type;
    string category;
    string date;
    string description;

public:
    Transaction(double amount, const string& type, const string& category,
                const string& date, const string& description);

    double getAmount() const;
    string getType() const;
    string getCategory() const;
    string getDate() const;
    string getDescription() const;

    void setAmount(double newAmount);
    void setType(const string& newType);
    void setCategory(const string& newCategory);
    void setDate(const string& newDate);
    void setDescription(const string& newDescription);
    void print() const;

    string toCSV() const;
};

#endif

// Transaction.h
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
private:
    std::string type;     // "income" or "expense"
    float amount;
    std::string category;
    std::string date;

public:
    Transaction(std::string t, float a, std::string c, std::string d);
    void display() const;
    std::string getType() const;
    float getAmount() const;
    std::string getCategory() const;
    std::string getDate() const;

    void setType(const std::string &t);
    void setAmount(float a);
    void setCategory(const std::string &c);
    void setDate(const std::string &d);
};

#endif

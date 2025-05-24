#include "Transaction.h"
#include <iostream>

Transaction::Transaction(std::string t, float a, std::string c, std::string d)
    : type(t), amount(a), category(c), date(d) {}

void Transaction::display() const {
    std::cout << "[" << date << "] " << type << " - " << category << ": $" << amount << std::endl;
}

std::string Transaction::getType() const {
    return type;
}

float Transaction::getAmount() const {
    return amount;
}

std::string Transaction::getCategory() const {
    return category;
}

std::string Transaction::getDate() const {
    return date;
}

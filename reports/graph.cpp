#include "../include/Transaction.h"
#include<fstream>
#include<map>
#include<vector>
#include<iostream>


extern std::vector<Transaction> transactions;

void genereateMonthlyExpenseReport(){
    std::map<std::string, float> monthlyTotals;

    for(const auto &t: transactions){
        if(t.getType() == "Expense"){
            std::string month = t.getDate().substr(0, 7);
            monthlyTotals[month] += t.getAmount();
        }
    }

    std::ofstream out("monthly_report.csv");
    out<< "Monthly Total Spent\n";  
    for(const auto &[month, total]: monthlyTotals){
        out << month << "," << total << "\n";
    }
    out.close();
    std::cout << "MOnthly expense report generated: monthly_report.csv\n";
}

void generateCategoryReport(){
    std::map<std::string, float> categoryTotals;

    for(const auto &t: transactions){
        if(t.getType() == "Expense"){
            categoryTotals[t.getCategory()] += t.getAmount();
        }
    }
    std::ofstream out("Category, Total Spent\n");
    for (const auto &[cat, amt] : categoryTotals) {
        out << cat << "," << amt << "\n";
    }
    out.close();
    std::cout << "Category-wise report generated: category_report.csv\n";
}
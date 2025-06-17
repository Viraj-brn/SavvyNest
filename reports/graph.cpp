#include "../include/Transaction.h"
#include<fstream>
#include<map>
#include<unordered_map>
#include<vector>
#include<iostream>
#include<algorithm>


extern std::vector<Transaction> transactions;

void generateMonthlyExpenseReport(){
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
    std::ofstream out("category_report.csv");
    out << "Category, Total spent\n";
    for (const auto &[cat, amt] : categoryTotals) {
        out << cat << "," << amt << "\n";
    }
    out.close();
    std::cout << "Category-wise report generated: category_report.csv\n";
}

void showVisualSummary(){
    if(transactions.empty()){
        std::cout<< "No transactions available to visualize.\n";
        return;
    }

    float totalIncome = 0, totalExpense = 0;
    std::unordered_map<std::string, float> categoryExpense;

    for(const auto &t: transactions){
        std::string type = t.getType();
        std::string category = t.getCategory();
        float amount = t.getAmount();
        std::transform(type.begin(), type.end(), type.begin(), ::tolower);
        std::transform(category.begin(), category.end(), category.begin(), ::tolower);
        
        if(type == "income") {
            totalIncome += amount;
        }
        else if(type == "expense"){
            totalExpense += amount;
            categoryExpense[category] += amount;
        }
    }
    // Income vs Expense Bar
    std::cout<<"\nIncome vs Expense:\n";
    int incomebars = totalIncome/100;
    int expenseBars = totalExpense/100;

    std::cout<< "Income: ";
    for (int i = 0; i < incomebars; ++i) std::cout << "#";
    std::cout << " ₹" << totalIncome << "\n";
    
    std::cout << "Expense: ";
    for (int i = 0; i < expenseBars; ++i) std::cout << "#";
    std::cout << " ₹" << totalExpense << "\n";

    // Category Wise Spending
    std::cout<< "Category-wise Spending:\n";
    for(const auto &entry : categoryExpense){
        std::cout<< entry.first << ": ";
        int bars = entry.second / 100;
        for(int i=0; i < bars; ++i) std::cout<< "*";
        std::cout<< " ₹" << entry.second <<"\n";
    }
    std::cout<<"\n";
}
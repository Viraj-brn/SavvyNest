#include<iostream>
#include<vector>
#include "../include/Transaction.h"
#include "../reports/graph.h"
#include<fstream>
#include<sstream>
#include<algorithm>
#include<iomanip>
#include<stack>
#include<string>
#include<unordered_set>
#include<unordered_map>
#include<ctime>
#include "../src/log.h"
#include<cmath>
using namespace std;

vector<Transaction> transactions;
stack<pair<int, Transaction>> deletedStack;
unordered_map<string, float> monthlyBudget;
unordered_map<string, float> monthlySpent; 

void writeLoginLog(){
    ofstream log("logs.txt", ios::app);
    time_t now = time(0);
    log << "Login: " << ctime(&now);
    log.close();
}

void writeLogoutLog(){
    ofstream log("logs.txt", ios::app);
    time_t now = time(0);
    log << "Logout: " << ctime(&now) <<"\n";
    log.close();
}
string convertToLower(string s){
    string converted = "";
    for (int i = 0; i < s.length(); i++)
    {
        if(isupper(s[i])) converted += tolower(s[i]);
        else converted += s[i];
    }
    return converted;

}
bool checkMonthlyLimit(const string &month_year, float amount) {
    if (monthlyBudget.find(month_year) == monthlyBudget.end()) return true;
    return (monthlySpent[month_year] + amount <= monthlyBudget[month_year]);
}

void saveDataToFile(){
    ifstream src("data/transactions.txt", ios::binary);
    ofstream dst("data/transactions_backup.txt", ios::binary);
    dst << src.rdbuf();

    ofstream outFile("data/transactions.txt");
    for(const auto &t: transactions){
        outFile << t.getType()<<","<<t.getAmount()<<","<<t.getCategory()<<","<<t.getDate()<<"\n";
    }
    outFile.close();
ofstream budgetFile("data/budget.txt");
    for (const auto &entry : monthlyBudget) {
        budgetFile << entry.first << "," << entry.second << "\n";
    }
    budgetFile.close();

    ofstream spentFile("data/spent.txt");
    for (const auto &entry : monthlySpent) {
        spentFile << entry.first << "," << entry.second << "\n";
    }
    spentFile.close();

    cout << "Data saved to file successfully.\n";
}
void loadDataFromFile(){
    transactions.clear();
    monthlyBudget.clear();
    monthlySpent.clear();

    ifstream inFile("data/transactions.txt");
    string line;
    while (getline(inFile, line))
    {
        stringstream ss(line);
        string type, amountStr, category, date;
        getline(ss, type, ',');
        getline(ss, amountStr, ',');
        getline(ss, category, ',');
        getline(ss, date, ',');

        float amount = stof(amountStr);
        Transaction t(type, amount, category, date);
        transactions.push_back(t);
    }
    inFile.close();

    ifstream budgetFile("data/budget.txt");
    while (getline(budgetFile, line))
    {
        stringstream ss(line);
        string month, budgetStr;
        getline(ss, month, ',');
        getline(ss, budgetStr, '\n');
        monthlyBudget[month] = stof(budgetStr);
    }
    budgetFile.close();

    ifstream spentFile("data/spent.txt");
    while (getline(spentFile, line))
    {
        stringstream ss(line);
        string month, spentStr;
        getline(ss, month, ',');
        getline(ss, spentStr, '\n');
        monthlySpent[month] = stof(spentStr);
    }
    spentFile.close();

    cout<<"Data Loaded Successfully.\n";
}

void addTransaction() {
    string type, category, date;
    float amount;
    cout << "Enter transaction type (Income/Expense): ";
    cin >> type;
    cout << "Enter amount: ";
    cin >> amount;
    cout << "Enter category (e.g., salary, food, rent, etc): ";
    cin >> category;
    cout << "Enter date (DD-MM-YYYY): ";
    cin >> date;

    string month_year = date.substr(3);

    if (convertToLower(type) == "expense") {
        monthlySpent[month_year] += amount;

        if (monthlyBudget.find(month_year) != monthlyBudget.end()) {
            float budget = monthlyBudget[month_year];
            if (monthlySpent[month_year] > budget) {
                cout << "Warning: You've exceeded your budget for " << month_year << "!\n";
                cout << "Budget: Rs." << budget << " | Spent: Rs." << monthlySpent[month_year] << endl;
            }
        }
    }
    Transaction newTransaction(type, amount, category, date);
    transactions.push_back(newTransaction);

    cout << "Transaction added successfully.\n";
}


void setMonthlyLimit() {
    string month, year;
    float limit;

    cout << "Enter year (YYYY): ";
    cin >> year;
    cout << "Enter month (MM): ";
    cin >> month;

    if (month.size() == 1) month = "0" + month; 
    string key = month + "-" + year;           

    cout << "Enter spending limit for " << key << ": Rs.";
    cin >> limit;

    monthlyBudget[key] = limit;

    cout << "Limit of Rs." << limit << " set for " << key << ".\n";
}

void viewTransactions() {
    if (transactions.empty()) {
        cout << "No transactions to display.\n";
        return;
    }

    string filterType, filterCategory;
    cout << "Filter by type (Income / Expense / All): ";
    cin >> filterType;
    cout << "Filter by category (or All): ";
    cin >> filterCategory;

    transform(filterType.begin(), filterType.end(), filterType.begin(), ::tolower);
    transform(filterCategory.begin(), filterCategory.end(), filterCategory.begin(), ::tolower);

    cout << "\n========== Filtered Transaction History ==========\n";

    int count = 0;
    for (const auto &t : transactions) {
        string tType = t.getType(), tCategory = t.getCategory();
        transform(tType.begin(), tType.end(), tType.begin(), ::tolower);
        transform(tCategory.begin(), tCategory.end(), tCategory.begin(), ::tolower);

        bool matchType = (filterType == "all" || filterType == tType);
        bool matchCategory = (filterCategory == "all" || filterCategory == tCategory);

        if (matchCategory && matchType) {
            cout << ++count << ". ";
            t.display();
        }
    }
    if (count == 0) {
        cout << "No transactions matched your filter.\n";
    } else {
        cout << "===============================================\n";
        cout << "Total Matching Transactions: " << count << "\n";
    }
    cout << endl;
}
void showSummary() {
    vector<Transaction> filtered;

    int choice;
    cout << "1. Full summary\n2. Filter by categories\n3. Custom date range\n4. Specific month\n5. Specific year\nPick: ";
    cin >> choice;
    if(choice == 1) filtered = transactions;
    else if (choice == 2) {
        unordered_set<string> s;
        string cat;
        int cat_cnt = 0;
        while (true)
        {
            cout<<"Enter the category: ";
            cin>>cat;
            cat_cnt++;
            cout<<cat_cnt<<" categories stored\n";
            s.insert(convertToLower(cat));
            cout<<"Do you wish to enter more categories(y/n): ";
            char op;
            cin>>op;
            if(tolower(op) == 'n') break;
            else if(tolower(op) == 'y') continue;
            else {
                cout<<"Invalid input, expected a y/n only!!\n";
                break;
            }
        }
        for(const auto &t: transactions){
            if(s.find(convertToLower(t.getCategory())) != s.end()) filtered.push_back(t);
        }
    }
    else if(choice == 3) {
        string from, to;
        cout<<"Enter start date(YYYY-MM-DD): ";
        cin>> from;
        cout<<"Enter end date(YYYY-MM-DD): ";
        cin>> to;
        for(const auto &t: transactions){
            string date = t.getDate();
            if(date>=from && date<=to) filtered.push_back(t);
        }
    }  
    else if(choice == 4) {
        string year, month;
        cout<< "Enter year(YYYY): ";
        cin>>year;
        cout<< "Enter month (MM): ";
        cin>>month;
        string prefix = year + '-' + month;
        for(const auto &t: transactions){
            if(t.getDate().substr(0,7) == prefix) filtered.push_back(t);
        }
    }
     else if (choice == 5) {
        string year;
        cout << "Enter year (YYYY): ";
        cin >> year;
        for (const auto &t : transactions) {
            if (t.getDate().substr(0, 4) == year) filtered.push_back(t);
        }
    }
    else {
        cout<<"Invalid choice. Returning. \n";
        return; 
    } 
    float total_income = 0.0f, total_expense = 0.0f;
    int inc_cnt = 0, exp_cnt = 0;
    float mx_income = INT_MIN, mx_expense = INT_MAX;
    if(filtered.empty()) {
        cout<<"No transaction matched the filter.\n";
        return;
    }
    for (const auto &t : filtered) {
        string type = t.getType();
        transform(type.begin(), type.end(), type.begin(), ::tolower);
        if (type == "income"){
            inc_cnt++;
            total_income += t.getAmount();
            mx_income = max(mx_income, t.getAmount());
        }
        else if (type == "expense") {
            exp_cnt++;
            total_expense += t.getAmount();
            mx_expense = min(mx_expense, t.getAmount());
        }
    }

    float balance = total_income - total_expense;

    cout << fixed<<setprecision(2);
    cout << "\nSummary:\n";
    cout << "Total Income  : Rs." << total_income <<"\t\tNumber of incomes: " <<inc_cnt<< endl;
    cout << "Total Expense : Rs." << total_expense <<"\t\tNumber of expenses: " <<exp_cnt<< endl;
    cout << "Net Balance   : Rs." << balance << endl<<endl;
    cout << "Highest Income: Rs." << mx_income<<endl;
    cout << "Highest Expense: Rs." << mx_expense<<endl;
}
int convertDate(string date) {
    string arr[3];
    string temp = "";
    int j = 0;

    for (int i = 0; i < date.length(); i++) {
        if (date[i] == '-') {
            arr[j++] = temp;
            temp = "";
        } else {
            temp += date[i];
        }
    }
    arr[j] = temp; 

    string res = arr[2] + arr[1] + arr[0];  
    return stoi(res);
}

void sortTransactions(){
    int c;
    cout<<"Sort Transactions by: \n 1. Amount \n2. Date \n Select an option: ";
    cin>>c;
    char s;
    if(c==1) {
        cout<<"Pick sorting order: \n A. Ascending \n D. Descending \n Choose:";
        cin>>s;
        if(tolower(s) == 'a'){
            sort(transactions.begin(), transactions.end(), [](const Transaction &a, const Transaction &b){
            return a.getAmount() < b.getAmount(); 
            });
        }
        else if(tolower(s) == 'd'){
            sort(transactions.begin(), transactions.end(), [](const Transaction &a, const Transaction &b){
            return a.getAmount() > b.getAmount(); 
            });
        }
    }
    else if(c==2){
        cout<<"Pick sorting order: \n A. Ascending \n D. Descending \n Choose:";
        cin>>s;
        if(tolower(s) == 'a'){
            sort(transactions.begin(), transactions.end(), [](const Transaction &a, const Transaction &b) {
            return convertDate(a.getDate()) < convertDate(b.getDate());
            });

        }
        else if(tolower(s) == 'd'){
        sort(transactions.begin(), transactions.end(), [](const Transaction &a, const Transaction &b) {
            return convertDate(a.getDate()) > convertDate(b.getDate());
            });

        }
    }
    else {cout<<"Invalid choice!\n";}
    cout<<"Transactions Sorted Successfully.\n";
}

void editTransaction() {
    int i = 1;
    cout << "Here's a list of your transactions....\n";
    for (auto &t : transactions) {
        cout << i << ". ";
        t.display();
        i++;
    }

    int c;
    cout << "Pick a transaction number to edit: ";
    cin >> c;

    if (c > transactions.size() || c <= 0) {
        cout << "Invalid input, transaction does not exist!!\n";
        return;
    }

    int x;
    cout << "Select a field to edit:\n 1. Type\n 2. Amount\n 3. Category\n 4. Date\n 5. Exit\nPick: ";
    cin >> x;

    if (x < 1 || x > 5) {
        cout << "Invalid field selection!!\n";
        return;
    }

    if (x == 5) {
        cout << "No changes made....Exiting\n";
        return;
    }
    deletedStack.push({c - 1, transactions[c - 1]});

    switch (x) {
        case 1: {
            string s;
            cout << "Enter the new Transaction type: ";
            cin >> s;
            transactions[c - 1].setType(s);
            cout << "Transaction Type changed to " << s << endl;
            break;
        }
        case 2: {
            float amt;
            cout << "Enter the new Transaction amount: ";
            cin >> amt;
            transactions[c - 1].setAmount(amt);
            cout << "Transaction Amount changed to " << amt << endl;
            break;
        }
        case 3: {
            string cat;
            cout << "Enter the new Transaction category: ";
            cin >> cat;
            transactions[c - 1].setCategory(cat);
            cout << "Transaction Category changed to " << cat << endl;
            break;
        }
        case 4: {
            string d;
            cout << "Enter the new Transaction date: ";
            cin >> d;
            transactions[c - 1].setDate(d);
            cout << "Transaction Date changed to " << d << endl;
            break;
        }
    }

    // Undo prompt
    char u;
    cout << "Would you like to undo the last change? (y/n): ";
    cin >> u;
    while (tolower(u) == 'y' && !deletedStack.empty()) {
        auto [pos, trans] = deletedStack.top();
        deletedStack.pop();
        transactions[pos] = trans;
        cout << "Undo successful.\n";

        cout << "Undo more? (y/n): ";
        cin >> u;
        if(deletedStack.empty()) cout<<"Nothing to undo. \n";
    }
}
void deleteTransaction() {
    auto displayTransactions = []() {
        cout << "Here's a list of your transactions....\n";
        int i = 1;
        for (const auto &t : transactions) {
            cout << i << ". ";
            t.display();
            i++;
        }
    };

    displayTransactions();

    while (true) {
        int c;
        cout << "Pick a transaction number to delete: ";
        cin >> c;

        if (c > transactions.size() || c <= 0) {
            cout << "Invalid input, transaction does not exist!!\n";
        } else {
            cout << "Are you sure you want to delete this transaction? (y/n): ";
            char confirm;
            cin >> confirm;

            if (confirm == 'y' || confirm == 'Y') {
                deletedStack.push({c - 1, transactions[c - 1]});
                transactions.erase(transactions.begin() + (c - 1));
                cout << "Transaction deleted successfully.\n";
            } else {
                cout << "Deletion cancelled.\n";
            }
        }

        char o;
        cout << "Do you wish to delete more transactions? (y/n): ";
        cin >> o;
        if (tolower(o) != 'y') break;

        displayTransactions();
    }

    char u;
    cout << "Would you like to undo the last deletion(s)? (y/n): ";
    cin >> u;
    while (tolower(u) == 'y' && !deletedStack.empty()) {
        auto [pos, trans] = deletedStack.top();
        deletedStack.pop();

        if (pos >= transactions.size()) {
            transactions.push_back(trans);
        } else {
            transactions.insert(transactions.begin() + pos, trans);
        }
        cout << "Undo successful.\n";

        cout << "Undo more? (y/n): ";
        cin >> u;
    }
}
void searchTransaction() {
    int r = 1;
    while (r) {
        bool found = false;
        int c;
        cout << "\nPick a search criterion:\n"
             << "1. Type\n2. Category\n3. Amount\n4. Date\n5. Exit searching\nPick: ";
        cin >> c;

        switch (c) {
            case 1: {
                string type;
                cout << "Enter the transaction type (Income/Expense): ";
                cin >> type;
                for (const auto &t : transactions) {
                    if (t.getType() == type) {
                        if (!found) cout << "Matching transactions:\n";
                        t.display();
                        found = true;
                    }
                }
                break;
            }
            case 2: {
                string cat;
                cout << "Enter the transaction category: ";
                cin >> cat;
                for (const auto &t : transactions) {
                    if (t.getCategory() == cat) {
                        if (!found) cout << "Matching transactions:\n";
                        t.display();
                        found = true;
                    }
                }
                break;
            }
            case 3: {
                float amount;
                cout << "Enter the transaction amount: ";
                cin >> amount;
                for (const auto &t : transactions) {
                    if (t.getAmount() == amount) {
                        if (!found) cout << "Matching transactions:\n";
                        t.display();
                        found = true;
                    }
                }
                break;
            }
            case 4: {
                string date;
                cout << "Enter the transaction date (YYYY-MM-DD): ";
                cin >> date;
                for (const auto &t : transactions) {
                    if (t.getDate() == date) {
                        if (!found) cout << "Matching transactions:\n";
                        t.display();
                        found = true;
                    }
                }
                break;
            }
            case 5:
                r = 0;
                cout << "__You ended your search session__\n";
                break;
            default:
                cout << "Invalid input, try again!!\n";
        }
        if (c >= 1 && c <= 4 && !found) {
            cout << "No matching transactions found.\n";
        }
    }
}
//Investment Simulator
void investmentSimulator(){
    float principal, rate;
    int time;
    string interestType;

    cout << "\n--- Investment Simulator ---\n";
    cout << "Enter the amount you want to invest (Rs.): ";
    cin >> principal;

    cout << "Enter annual interest rate (%): ";
    cin >> rate;

    cout << "Enter time period (in years): ";
    cin >> time;

    cout << "Choose interest type (Simple / Compound): ";
    cin >> interestType;

    float finalAmount = 0.0f;
    if (convertToLower(interestType) == "simple") {
        float interest = (principal * rate * time) / 100;
        finalAmount = principal + interest;
        cout << "\nSimple Interest Earned: Rs." << fixed << setprecision(2) << interest << endl;
    }
    else if (convertToLower(interestType) == "compound") {
        finalAmount = principal * pow((1 + rate / 100), time);
        cout << "\nCompound Interest Earned: Rs." << fixed << setprecision(2) << (finalAmount - principal) << endl;
    }
    else {
        cout << "Invalid interest type entered!\n";
        return;
    }

    cout << "Final Value after " << time << " years: Rs." << fixed << setprecision(2) << finalAmount << "\n\n";
}

int main() {
    writeLoginLog();
    loadDataFromFile();

    int choice;
    do {
        cout << "\n====== SavvyNest Menu ======\n";
        cout << "0. Exit\n";
        cout << "1. Add Transaction\n";
        cout << "2. View Transactions\n";
        cout << "3. Show Summary\n";
        cout << "4. Sort Transactions\n";
        cout << "5. Edit Transaction\n";
        cout << "6. Delete Transaction\n";
        cout << "7. Search Transaction\n";
        cout << "8. Set Monthly Limit\n";
        cout << "9 Generate Report\n";
        cout << "10. Show Visual Summary\n";
        cout << "11. Investment Simulator\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 0:
                saveDataToFile();
                writeLogoutLog();
                cout << "Exiting SavvyNest.\n";
                return 0;
            case 1:
                addTransaction();
                break;
            case 2:
                viewTransactions();
                break;
            case 3:
                showSummary();
                break;
            case 4:
                sortTransactions();
                break;
            case 5:
                editTransaction();
                break;
            case 6:
                deleteTransaction();
                break;
            case 7:
                searchTransaction();
                break;
            case 8:
                setMonthlyLimit();
                break;
            case 9:{
                int opt;
                cout << "1. Monthly Expense Graph\n2. Category Spending Graph\nPick:";
                cin>>opt;
                if (opt == 1) generateMonthlyExpenseReport();
                else if (opt == 2) generateCategoryReport();
                else cout << "Invalid option.\n";
                break;
                }
            case 10:
                showVisualSummary();
                break;
            case 11:
                investmentSimulator();
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } 
    while (choice <= 10);
    return 0;
}
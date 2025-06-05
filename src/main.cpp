#include<iostream>
#include<vector>
#include "Transaction.h"
#include<fstream>
#include<sstream>
#include<algorithm>
#include<iomanip>
#include<string>
using namespace std;

vector<Transaction> transactions;

void addTransaction(){
    string type, category, date;
    float amount;
    cout<<"Enter transaction type(Income/ Expense):";
    cin>>type;
    cout<<"Enter amount:";
    cin>>amount;
    cout<<"Enter category(eg: salary, food, rent, etc):";
    cin>>category;
    cout<<"Enter date(DD-MM-YYYY)";
    cin>>date;

    Transaction newTransaction(type, amount, category, date);
    transactions.push_back(newTransaction);
    cout<<"Transaction added successfully.\n";
}

void viewTransactions(){
    if(transactions.empty()){
        cout<<"No transactions to display.\n";
        return;
    }
    string filterType, filterCategory;
    cout<<"Filter by type(Income/Expense or All): ";
    cin>>filterType;
    cout<<"Filter by category(or All): ";
    cin>>filterCategory;

    transform(filterType.begin(), filterType.end(), filterType.begin(), ::tolower);
    transform(filterCategory.begin(), filterCategory.end(), filterCategory.begin(), ::tolower);

    cout << "\nFiltered Transaction History:\n";
    for (const auto &t : transactions) {
        string tType = t.getType(), tCategory = t.getCategory();
        transform(tType.begin(), tType.end(), tType.begin(), ::tolower);
        transform(tCategory.begin(), tCategory.end(), tCategory.begin(), ::tolower);

        bool matchType = (filterType == "all" || filterType == tType);
        bool matchCategory = (filterCategory == "all" || filterCategory == tCategory);

        if(matchCategory && matchType){
            t.display();
        }
    }
    cout << endl;
}

void showSummary() {
    float total_income = 0.0f, total_expense = 0.0f;

    for (const auto &t : transactions) {
        string type = t.getType();
        transform(type.begin(), type.end(), type.begin(), ::tolower);

        if (type == "income") total_income += t.getAmount();
        else if (type == "expense") total_expense += t.getAmount();
    }

    float balance = total_income - total_expense;

    cout << fixed << setprecision(2);
    cout << "\nSummary:\n";
    cout << "Total Income  : $" << total_income << endl;
    cout << "Total Expense : $" << total_expense << endl;
    cout << "Net Balance   : $" << balance << endl;
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
            return a.getAmount() < b.getAmount(); // ascending
            });
        }
        else if(tolower(s) == 'd'){
            sort(transactions.begin(), transactions.end(), [](const Transaction &a, const Transaction &b){
            return a.getAmount() > b.getAmount(); // descending
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

void saveToFile(const string &filename){
    ofstream file(filename);
    if(!file){
        cout<<"Could not open the file.\n";
        return;
    }
    for(const Transaction &t: transactions){
        file << t.getType() <<","
             << t.getAmount() <<","
             << t.getCategory() <<","
             << t.getDate() <<"\n";
    }
    file.close();
}
void loadFromFile(const string &filename){
    ifstream file("data.csv");
    if(!file) {
        cout<<"Could not open file.\n";
        return;
    }
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string type, category, date, amountStr;
        float amount;

        getline(ss, type, ',');
        getline(ss, amountStr, ',');
        getline(ss, category, ',');
        getline(ss, date, ',');      
        
        amount = stof(amountStr);
        Transaction t(type, amount, category, date);
        transactions.push_back(t);
    }
}
int main() {
    int choice;
    loadFromFile("data.csv");
    do {
        cout << "\n====== Finance Tracker Menu ======\n";
        cout << "1. Add Transaction\n";
        cout << "2. View Transactions\n";
        cout << "3. Show Summary\n";
        cout << "4. Sort Transactions\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
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
                cout << "👋 Exiting program.\n";
                saveToFile("data.csv");
                break;
            default:
                cout << "❌ Invalid choice. Try again.\n";
        }

    } while (choice != 3);

    return 0;
}
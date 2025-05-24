#include<iostream>
#include<vector>
#include "Transaction.h"

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
    cout << "\nTransaction History:\n";
    for (const auto &t : transactions) {
        t.display();
    }
    cout << endl;
}

int main() {
    int choice;

    do {
        cout << "\n====== Finance Tracker Menu ======\n";
        cout << "1. Add Transaction\n";
        cout << "2. View Transactions\n";
        cout << "3. Exit\n";
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
                cout << "👋 Exiting program.\n";
                break;
            default:
                cout << "❌ Invalid choice. Try again.\n";
        }

    } while (choice != 3);

    return 0;
}
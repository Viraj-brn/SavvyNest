#include<iostream>
#include<vector>
#include "Transaction.h"
#include<fstream>
#include<sstream>
#include<algorithm>
#include<iomanip>
#include<stack>
#include<string>
using namespace std;

vector<Transaction> transactions;
stack<pair<int, Transaction>> deletedStack;

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
            deletedStack.push({c - 1, transactions[c - 1]});
            transactions.erase(transactions.begin() + (c - 1));
            cout << "Transaction deleted successfully.\n";
        }

        char o;
        cout << "Do you wish to delete more transactions? (y/n): ";
        cin >> o;
        if (tolower(o) != 'y') break;

        displayTransactions();
    }

    // Undo prompt
    char u;
    cout << "Would you like to undo the last deletion(s)? (y/n): ";
    cin >> u;
    while (tolower(u) == 'y' && !deletedStack.empty()) {
        auto [pos, trans] = deletedStack.top();
        deletedStack.pop();

        // Safely reinsert at the correct position
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
        cout << "5. Edit Transaction\n";
        cout << "6. Delete Transaction\n";
        cout << "7. Search Transaction\n";
        cout << "8. Exit\n";
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
                editTransaction();
                break;
            case 6:
                deleteTransaction();
                break;
            case 7:
                searchTransaction();
            case 8:
                cout << "Exiting program.\n";
                saveToFile("data.csv");
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 3);

    return 0;
}
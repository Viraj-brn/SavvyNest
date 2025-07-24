#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "../include/Transaction.h" // Use your Transaction class

std::vector<Transaction> transactions;

// Load transactions from file
void loadTransactions() {
    std::ifstream file("data/transactions.txt");
    if (!file) return;

    std::string type, category, date;
    float amount;
    while (file >> type >> amount >> category >> date) {
        transactions.emplace_back(type, amount, category, date);
    }
    file.close();
}

// Save transactions to file
void saveTransactions() {
    std::ofstream file("data/transactions.txt");
    for (const auto& t : transactions) {
        file << t.getType() << " "
             << t.getAmount() << " "
             << t.getCategory() << " "
             << t.getDate() << "\n";
    }
    file.close();
}

struct FinanceSummary {
    float totalIncome = 0.0f;
    float totalExpense = 0.0f;
    float balance = 0.0f;
};

FinanceSummary calculateSummary(const std::vector<Transaction>& transactions) {
    FinanceSummary summary;
    for (const auto& t : transactions) {
        if (t.getType() == "Income") {
            summary.totalIncome += t.getAmount();
        } else if (t.getType() == "Expense") {
            summary.totalExpense += t.getAmount();
        }
    }
    summary.balance = summary.totalIncome - summary.totalExpense;
    return summary;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(900, 600), "SavvyNest GUI");
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window)) {
        std::cerr << "Failed to initialize ImGui-SFML.\n";
        return 1;
    }

    loadTransactions();

    char type[16] = "Income";
    char category[32] = "";
    char date[16] = "2025-07-24";
    float amount = 0.0f;

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            ImGui::SFML::ProcessEvent(window, event);
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        // --- Main GUI ---
        ImGui::Begin("SavvyNest");
        ImGui::Text("Welcome to the future of budget tracking!");
        ImGui::Separator();

        FinanceSummary summary = calculateSummary(transactions);

        ImGui::Text("Total Income: ₹%.2f", summary.totalIncome);
        ImGui::Text("Total Expense: ₹%.2f", summary.totalExpense);
        ImGui::Text("Balance: ₹%.2f", summary.balance);
        ImGui::Separator();


        // Transaction form
        ImGui::InputText("Type (Income/Expense)", type, IM_ARRAYSIZE(type));
        ImGui::InputFloat("Amount", &amount);
        ImGui::InputText("Category", category, IM_ARRAYSIZE(category));
        ImGui::InputText("Date (YYYY-MM-DD)", date, IM_ARRAYSIZE(date));

        if (ImGui::Button("Add Transaction")) {
            transactions.emplace_back(type, amount, category, date);
            saveTransactions();
        }
        ImGui::Separator();

        // Show transaction list
        ImGui::Text("All Transactions:");
        if (ImGui::BeginTable("Transactions", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("Type");
            ImGui::TableSetupColumn("Amount");
            ImGui::TableSetupColumn("Category");
            ImGui::TableSetupColumn("Date");
            ImGui::TableHeadersRow();

            for (const auto& t : transactions) {
                ImGui::TableNextRow();

                ImVec4 textColor = (t.getType() == "Income") 
                                    ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) // Green
                                    : ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red

                ImGui::TableSetColumnIndex(0); ImGui::TextColored(textColor, "%s", t.getType().c_str());
                ImGui::TableSetColumnIndex(1); ImGui::TextColored(textColor, "%.2f", t.getAmount());
                ImGui::TableSetColumnIndex(2); ImGui::Text("%s", t.getCategory().c_str());
                ImGui::TableSetColumnIndex(3); ImGui::Text("%s", t.getDate().c_str());
            }

            ImGui::EndTable();
        }

        if (ImGui::Button("Exit")) window.close();
        ImGui::End();

        window.clear(sf::Color(30, 30, 30));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h> 
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SavvyNest GUI");
    window.setFramerateLimit(60);

    // Initialize ImGui-SFML
    if (!ImGui::SFML::Init(window)) {
        std::cerr << "Failed to initialize ImGui-SFML.\n";
        return 1;
    }

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            ImGui::SFML::ProcessEvent(window, event);
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        // GUI Code
        ImGui::Begin("SavvyNest");
        ImGui::Text("Welcome to the future of budget tracking!");
        if (ImGui::Button("Exit")) {
            window.close();
        }
        ImGui::End();

        window.clear(sf::Color(30, 30, 30)); // Dark background
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}

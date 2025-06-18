#include <SFML/Graphics.hpp>
#include <imgui.h>
#include "imgui-SFML.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SavvyNest GUI");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed) window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("SavvyNest GUI");
        ImGui::Text("Welcome to the future of budget tracking!");
        if (ImGui::Button("Exit")) window.close();
        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown(); 
    return 0;
}

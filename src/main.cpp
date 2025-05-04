#include "AssetManager.hpp"
#include "Menu.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/View.hpp"
#include "SFML/System/Angle.hpp"
#include <SFML/Graphics.hpp>

#pragma region imgui
#include "imgui-SFML.h"
#include "imgui.h"
#include "imguiThemes.h"
#pragma endregion

enum directions { down, right, up, left };
int main() {
  Game::AssetManager *assets = Game::AssetManager::GetInstance();

  unsigned int width{640U};
  unsigned int height{480U};

  sf::RenderWindow *window =
      new sf::RenderWindow(sf::VideoMode({width, height}), "Game");

  sf::View *view = new sf::View(sf::FloatRect(
      {0, 0}, {static_cast<float>(width), static_cast<float>(height)}));
  view->setCenter({width / 2.0f, height / 2.0f});
  window->setView(*view);

  window->setFramerateLimit(60U);
  sf::Texture texture;
  assets->LoadFont("main", "Kurland-Regular.ttf");
  sf::Font defFont = assets->GetFont("fallback");

  Game::StateMachine stateMachine;
  stateMachine.AddState(
      "menu", std::make_unique<Menu>(stateMachine, defFont, texture), false);
  sf::Clock clock;
  while (window->isOpen()) {

    sf::Time deltaTime = clock.restart();
    float deltaTimeSeconds = deltaTime.asSeconds();

    // make sure delta time stays within normal bounds, like between one FPS and
    // zero FPS
    deltaTimeSeconds = std::min(deltaTimeSeconds, 1.F);
    deltaTimeSeconds = std::max(deltaTimeSeconds, 0.F);

    window->clear();
    stateMachine.ProcessStateChanges();
    if (stateMachine.GetCurrentState()) {
      stateMachine.GetCurrentState()->HandleInput(*window, *view);
      stateMachine.GetCurrentState()->Update(deltaTimeSeconds);
      stateMachine.GetCurrentState()->Draw(*window, *view);
    }
    window->display();
  }
  Game::AssetManager::Cleanup();
  delete window;
  return 0;
}

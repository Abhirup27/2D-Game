
#include "GameLoop.hpp"
#include "AssetManager.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/View.hpp"
#include "SFML/System/Vector2.hpp"
#include <optional>

void Game::Init() {
  Engine::AssetManager *assets = Engine::AssetManager::GetInstance();

  if (!assets->HasTexture("pawn")) {
    assets->LoadTexture("pawn", "W_Pawn.png");
  }
  gameObj.setTexture(assets->GetTexture("pawn"), true);
  gameObj.setOrigin({8, 16});
  gameObj.setPosition({640U / 2.0F, 480U / 2.0F});
}
void Game::Draw(sf::RenderWindow &window, sf::View &view) {
  window.setView(view);
  window.draw(gameObj);
}
void Game::Update(float deltaTime) {}

void Game::HandleInput(sf::RenderWindow &window, sf::View &view) {
  while (const std::optional event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window.close();
    }
    if (const auto *resized = event->getIf<sf::Event::Resized>()) {
      HandleResize(window, view, static_cast<sf::Vector2f>(resized->size));
    }
  }
}

void Game::HandleResize(sf::RenderWindow &window, sf::View &view,
                        sf::Vector2f size) {
  view.setSize(size);
  view.setCenter({size.x / 2.0F, size.y / 2.0F});
  window.setView(view);

  // update all the UI and other sprite bounding boxes, origins and positions
}

void Game::Pause() {}
void Game::Resume() {}

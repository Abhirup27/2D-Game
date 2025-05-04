#include "AssetManager.hpp"
#include "GameLoop.hpp"
#include "Menu.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/View.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include <StateMachine.hpp>

namespace Engine {

void StateMachine::AddState(const std::string &name, StateRef newState,
                            bool isReplacing) {
  m_isAdding = true;
  m_isReplacing = isReplacing;
  m_newStateName = name;
  m_pendingState = std::move(newState);
}
void StateMachine::RemoveState(const std::string &name) {
  m_isRemoving = true;
  m_stateToRemove = name;
}
void StateMachine::ProcessStateChanges() {
  // Handle state removal
  if (m_isRemoving) {
    auto it = m_states.find(m_stateToRemove);
    if (it != m_states.end()) {
      // Check if the state to remove is the current state
      if (m_currentState == it->second.get()) {
        m_currentState = nullptr;
      }
      m_states.erase(it);
    }
    m_isRemoving = false;
  }

  // Handle state addition
  if (m_isAdding) {
    if (m_currentState && !m_isReplacing) {
      m_currentState->Pause();
    }

    if (m_isReplacing) {
      // Clear all states if replacing
      m_currentState = nullptr;
      m_states.clear();
    }

    // Add the new state to m_states
    StateRef &ref = m_states[m_newStateName] = std::move(m_pendingState);
    m_currentState = ref.get();
    m_currentState->Init();

    m_isAdding = false;
  }
}

State *StateMachine::GetCurrentState() { return m_currentState; }
} // namespace Engine

void Menu::Init() {

  Engine::AssetManager *assets = Engine::AssetManager::GetInstance();

  if (!assets->HasFont("main")) {
    assets->LoadFont("main", "Kurland-Regular.ttf");
  }
  if (!assets->HasTexture("menu_background")) {
    assets->LoadTexture("menu_background", "menu_background.png");
  }

  // Setup background
  m_backgroundSprite.setTexture(assets->GetTexture("menu_background"), true);
  m_backgroundSprite.setOrigin({272, 160});
  // m_backgroundSprite.scale({2.0F, 2.0F});
  m_backgroundSprite.setPosition({640U / 2.0F, 480U / 2.0F});

  m_title.setFont(assets->GetFont("main"));
  m_title.setString("Game");
  m_title.setCharacterSize(48);
  m_title.setFillColor(sf::Color::White);
  m_title.setOutlineThickness(5.0F);
  m_title.setOutlineColor(sf::Color::Red);

  sf::FloatRect titleBounds = m_title.getLocalBounds();
  m_title.setOrigin({titleBounds.size.x / 2.0F, titleBounds.size.y / 2.0F});
  m_title.setPosition({640U / 2.0F, 150});

  m_playButton.setFont(assets->GetFont("main"));
  m_playButton.setString("Play");
  m_playButton.setCharacterSize(32);
  m_playButton.setFillColor(sf::Color::White);
  sf::FloatRect rc = m_playButton.getLocalBounds();
  m_playButton.setOrigin({rc.size.x / 2, rc.size.y / 2});
  m_playButton.setPosition({640U / 2.0F, 250});

  m_exitButton.setFont(assets->GetFont("main"));
  m_exitButton.setString("Exit");
  m_exitButton.setCharacterSize(32);
  m_exitButton.setFillColor(sf::Color::White);
  sf::FloatRect exitBounds = m_exitButton.getLocalBounds();
  m_exitButton.setOrigin({exitBounds.size.x / 2.0F, exitBounds.size.y / 2.0F});
  m_exitButton.setPosition({640U / 2.0F, 300});
}

void Menu::Draw(sf::RenderWindow &window, sf::View &view) {
  // window.clear(sf::Color(50, 50, 50));
  window.setView(view);
  window.draw(m_backgroundSprite);
  window.draw(m_title);
  window.draw(m_playButton);
  window.draw(m_exitButton);
}

void Menu::Update(float deltaTime) {
  // update menu sprites
}

void Menu::HandleInput(sf::RenderWindow &window, sf::View &view) {
  while (const std::optional event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window.close();
    }
    if (const auto *resized = event->getIf<sf::Event::Resized>()) {
      HandleResize(window, view, static_cast<sf::Vector2f>(resized->size));
    }
    if (event->is<sf::Event::MouseButtonPressed>()) {
      if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (m_playButton.getGlobalBounds().contains(
                {static_cast<float>(mousePos.x),
                 static_cast<float>(mousePos.y)})) {

          m_stateMachine.AddState("game",
                                  std::make_unique<Game>(m_stateMachine), true);
          // m_stateMachine.RemoveState("menu");

        } else if (m_exitButton.getGlobalBounds().contains(
                       static_cast<sf::Vector2f>(mousePos))) {
          window.close();
        }
      }
    }
  }
}

void Menu::HandleResize(sf::RenderWindow &window, sf::View &view,
                        const sf::Vector2f size) {
  // m_playButton.setPosition(size / 2.0F);
  view.setSize(size);
  view.setCenter({size.x / 2.0F, size.y / 2.0F});
  window.setView(view);
  sf::Vector2f center = size / 2.0f;

  // Position background at center
  m_backgroundSprite.setPosition(center);
  float scaleX = size.x / 640.0F;
  float scaleY = size.y / 480.0F;
  m_backgroundSprite.setScale({scaleX, scaleY});

  m_title.setPosition({center.x, size.y * 0.3F});
  sf::FloatRect titleBounds = m_title.getLocalBounds();
  m_title.setOrigin({titleBounds.size.x / 2.0F, titleBounds.size.y / 2.0F});

  m_playButton.setPosition({center.x, center.y});

  m_exitButton.setPosition({center.x, center.y + 50.0F});
  sf::FloatRect exitBounds = m_exitButton.getLocalBounds();
  m_exitButton.setOrigin({exitBounds.size.x / 2.0F, exitBounds.size.y / 2.0F});
}

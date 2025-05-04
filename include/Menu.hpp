#pragma once
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/View.hpp"
#include "SFML/System/Vector2.hpp"
#include "State.hpp"
#include "StateMachine.hpp"
class Menu : public Game::State {
private:
  Game::StateMachine &m_stateMachine;
  sf::Text m_title;
  sf::Text m_playButton;
  sf::Text m_exitButton;
  sf::Sprite m_backgroundSprite;

public:
  Menu(Game::StateMachine &stateMachine, sf::Font &defaultFont,
       sf::Texture &defaultTexture)
      : m_stateMachine(stateMachine), m_title(defaultFont),
        m_exitButton(defaultFont), m_playButton(defaultFont),
        m_backgroundSprite(defaultTexture) {}

  void Init() override;
  void HandleInput(sf::RenderWindow &window, sf::View &view) override;
  void HandleResize(sf::RenderWindow &window, sf::View &view,
                    sf::Vector2f size) override;

  void Update(float deltaTime) override;

  void Draw(sf::RenderWindow &window, sf::View &view) override;
};

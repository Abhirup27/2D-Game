#pragma once
#include "State.hpp"
#include "StateMachine.hpp"
extern sf::Texture g_texture;
extern sf::Font g_font;

class Game : public Engine::State {
private:
  Engine::StateMachine &m_stateMachine;

  sf::Sprite gameObj;

public:
  Game(Engine::StateMachine &stateMachine)
      : m_stateMachine(stateMachine), gameObj(g_texture) {}

  void Init() override;
  void HandleInput(sf::RenderWindow &window, sf::View &view) override;
  void HandleResize(sf::RenderWindow &window, sf::View &view,
                    sf::Vector2f size) override;

  void Update(float deltaTime) override;

  void Draw(sf::RenderWindow &window, sf::View &view) override;
  void Pause() override;
  void Resume() override;
};

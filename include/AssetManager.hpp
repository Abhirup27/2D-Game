#pragma once

#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Shader.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <map>
#include <memory>
#include <string_view>

namespace Engine {
class AssetManager {

private:
  std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;
  std::map<std::string, std::unique_ptr<sf::Font>> m_fonts;
  std::map<std::string, std::unique_ptr<sf::SoundBuffer>> m_soundBuffers;
  std::map<std::string, std::unique_ptr<sf::Shader>> m_shaders;

  static AssetManager *s_instance;

public:
  static AssetManager *GetInstance() {
    if (s_instance == nullptr) {
      s_instance = new AssetManager();
      s_instance->LoadTexture("debug", "g_texture.png");
    }
    return s_instance;
  }
  static void Cleanup() {
    if (s_instance != nullptr) {
      delete s_instance;
      s_instance = nullptr;
    }
  }

  bool LoadTexture(const std::string &name, const std::string &filename);
  sf::Texture &GetTexture(const std::string &name);

  bool HasTexture(const std::string &name) const;

  void RemoveTexture(const std::string &name);

  bool LoadFont(const std::string &name, const std::string &filename);
  sf::Font &GetFont(const std::string &name);
  bool HasFont(const std::string &name) const;

  void RemoveFont(const std::string &name);

  bool LoadSound(const std::string &name, const std::string &filename);
  sf::SoundBuffer &GetSound(const std::string &name);

  bool HasSound(const std::string &name) const;
  void RemoveSound(const std::string &name);
};
} // namespace Engine

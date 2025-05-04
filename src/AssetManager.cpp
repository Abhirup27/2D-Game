

#include "AssetManager.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Image.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <iostream>
#include <memory>
#include <string_view>

namespace Engine {

AssetManager *AssetManager::s_instance = nullptr;

bool AssetManager::LoadTexture(const std::string &name,
                               const std::string &filename) {
  std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();

  if (!texture->loadFromFile(RESOURCES_PATH + filename)) {
    std::cerr << "ERROR:: Failed to load texture: " << filename << '\n';
    return false;
  }
  m_textures[name] = std::move(texture);
  return true;
}
sf::Texture &AssetManager::GetTexture(const std::string &name) {
  if (m_textures.find(name) == m_textures.end()) {
    std::cerr << "Texture not found: " << name << '\n';

    if (m_textures.find("error") == m_textures.end()) {
      std::unique_ptr<sf::Texture> errorTexture =
          std::make_unique<sf::Texture>();

      sf::Image errorImage;
      errorImage.setPixel({32, 32}, sf::Color::Magenta);
      errorTexture->loadFromImage(errorImage);
      m_textures["error"] = std::move(errorTexture);
    }
    return *m_textures["error"];
  }
  return *m_textures[name];
}

bool AssetManager::HasTexture(const std::string &name) const {
  return m_textures.find(name) != m_textures.end();
}

void AssetManager::RemoveTexture(const std::string &name) {
  m_textures.erase(name);
}

bool AssetManager::LoadFont(const std::string &name,
                            const std::string &filename) {
  std::unique_ptr<sf::Font> font = std::make_unique<sf::Font>();

  if (!font->openFromFile(RESOURCES_PATH + filename)) {
    std::cerr << "ERROR:: Failed to load font: " << filename << '\n';
    return false;
  }
  m_fonts[name] = std::move(font);
  return true;
}

sf::Font &AssetManager::GetFont(const std::string &name) {
  if (m_fonts.find(name) == m_fonts.end()) {
    std::cerr << "Font not found: " << name << '\n';
    // Load a system font as fallback
    if (m_fonts.find("fallback") == m_fonts.end()) {
      std::unique_ptr<sf::Font> fallbackFont = std::make_unique<sf::Font>();
      // Try to load a common system font
      if (!fallbackFont->openFromFile("/usr/share/fonts/TTF/DejaVuSans.ttf") &&
          !fallbackFont->openFromFile(
              "/user/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cerr << "Failed to load fallback font" << '\n';
      }
      m_fonts["fallback"] = std::move(fallbackFont);
    }
    return *m_fonts["fallback"];
  }
  return *m_fonts[name];
}

bool AssetManager::HasFont(const std::string &name) const {
  return m_fonts.find(name) != m_fonts.end();
}

void AssetManager::RemoveFont(const std::string &name) { m_fonts.erase(name); }

bool AssetManager::LoadSound(const std::string &name,
                             const std::string &filename) {
  std::unique_ptr<sf::SoundBuffer> soundBuffer =
      std::make_unique<sf::SoundBuffer>();

  if (!soundBuffer->loadFromFile(filename)) {
    std::cerr << "ERROR:: Failed to load sound: " << filename << '\n';
    return false;
  }
  m_soundBuffers[name] = std::move(soundBuffer);
  return true;
}
sf::SoundBuffer &AssetManager::GetSound(const std::string &name) {
  if (m_soundBuffers.find(name) == m_soundBuffers.end()) {
    std::cerr << "Sound buffer not found: " << name << std::endl;
    // Create an empty sound buffer for error cases
    if (m_soundBuffers.find("error") == m_soundBuffers.end()) {
      m_soundBuffers["error"] = std::make_unique<sf::SoundBuffer>();
    }
    return *m_soundBuffers["error"];
  }

  return *m_soundBuffers[name];
}

bool AssetManager::HasSound(const std::string &name) const {
  return m_soundBuffers.find(name) != m_soundBuffers.end();
}

void AssetManager::RemoveSound(const std::string &name) {
  m_soundBuffers.erase(name);
}

} // namespace Engine

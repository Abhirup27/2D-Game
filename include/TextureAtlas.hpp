#pragma once
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include <map>
#include <vector>

namespace Engine {
struct AtlasRegion {
  sf::IntRect rect;
  bool occupied;

  AtlasRegion(int x, int y, int width, int height, bool occupied = false)
      : rect({x, y}, {width, height}), occupied(occupied) {}
};

class TextureAtlas {
private:
  sf::Texture texture;
  std::map<std::string, sf::IntRect> m_sprites;
  std::vector<AtlasRegion> regions;

  unsigned int width;
  unsigned int height;
  unsigned int padding;

  int findFreeGregion(unsigned int width, unsigned int height) const;
  void splitRegion(int regionIndex, unsigned int width, unsigned int height);
  bool expandAtlas(unsigned int requiredWidth, unsigned int requiredHeight);
  void mergeAdjacentRegions();
  bool areRegionsAdjacent(const AtlasRegion &r1, const AtlasRegion &r2) const;
  sf::IntRect getUnionRect(const sf::IntRect &r1, const sf::IntRect &r2) const;

public:
  TextureAtlas(unsigned int width, unsigned int height,
               unsigned int padding = 2)
      : width(width), height(height), padding(padding) {}

  bool addTexture(const std::string &name, const sf::Texture &texture);
  bool removeTexture(const std::string &name);
  bool hasTexture(const std::string &name) const;

  unsigned int getWidth() const;
  unsigned int getHeight() const;
  sf::Vector2i getSize() const;

  const sf::Texture &getTexture() const;
  sf::IntRect getTextureRect(const std::string &name) const;
};
} // namespace Engine

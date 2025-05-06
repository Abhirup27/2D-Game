#include "TextureAtlas.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"
namespace Engine {
bool TextureAtlas::addTexture(const std::string &name,
                              const sf::Texture &newTexture) {
  sf::Vector2u size = texture.getSize();

  int regionIndex = findFreeGregion(size.x + padding * 2, size.y + padding * 2);

  if (regionIndex == -1) {

    if (!expandAtlas(size.x + padding * 2, size.y + padding * 2)) {
      return false;
    }

    regionIndex = findFreeGregion(size.x + padding * 2, size.y + padding * 2);

    if (regionIndex == -1) {
      return false;
    }
  }

  AtlasRegion &region = regions[regionIndex];
  region.occupied = true;

  sf::IntRect spriteRect({static_cast<int>(region.rect.position.x + padding),
                          static_cast<int>(region.rect.position.y + padding)},
                         {static_cast<int>(size.x), static_cast<int>(size.y)});

  texture.update(newTexture, static_cast<sf::Vector2u>(spriteRect.position));
  m_sprites[name] = spriteRect;

  splitRegion(regionIndex, size.x + padding * 2, size.y + padding * 2);

  return true;
}

int TextureAtlas::findFreeGregion(unsigned int width,
                                  unsigned int height) const {
  for (size_t i = 0; i < regions.size(); ++i) {
    const AtlasRegion &region = regions[i];
    if (!region.occupied && region.rect.size.x >= static_cast<int>(width) &&
        region.rect.size.y >= static_cast<int>(height)) {
      return static_cast<int>(i);
    }
  }
  return -1;
}

void TextureAtlas::splitRegion(int regionIndex, unsigned int width,
                               unsigned int height) {
  AtlasRegion &region = regions[regionIndex];

  // int remainingWidth = region.rect.
}

bool TextureAtlas::expandAtlas(unsigned int requiredWidth,
                               unsigned int requiredHeight) {

  return true;
}
} // namespace Engine

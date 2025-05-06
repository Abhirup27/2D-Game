#include "Entity.hpp"
#include <cstdint>
#include <map>
#include <memory>
namespace Engine {
class LevelManager {
private:
  std::map<std::uint16_t, std::unique_ptr<Entity>> m_Entities;
};
} // namespace Engine

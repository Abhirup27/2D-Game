#include <cstdint>
#include <map>
#include <memory>
// idk what i'm doing here honestly but I guess it will be something like this
struct Item {

  enum type {
    storage,
    consumable,
    equipable,
    other,
  } i_type;
  std::uint16_t i_id;
  void init(type i_type, std::uint16_t i_id);
};

class Entity {
protected:
  std::uint16_t id;
  std::map<std::uint16_t, std::unique_ptr<typename Tp>> m_resource;

public:
  virtual void getResource();
  virtual void setResource();
};

// for entities like ground, walls
class StaticEntity : public Entity {};
class DynamicEntity : public Entity {
protected:
  enum State { Active, Paused, Disabled };

public:
  virtual void Active();
  virtual void Paused();
  virtual void Disabled();
};

class UsableEntity : public DynamicEntity {
private:
  std::map<std::uint16_t, std::unique_ptr<Item>> item;
  std::uint16_t count;

protected:
  enum State {

  };

public:
  // init item, count, call Entity constructor to load textures from
  // AssetManager...
};

class Character : public DynamicEntity {
protected:
  enum State {
    idle,
    moving,
    eating,
  };

public:
  virtual void move();
  virtual void idle();
  virtual void eating();
};

class NPC : public Character {
protected:
  enum State {
    idle,
    moving,
    eating,
    attacking,
  };

public:
  void move();
  void eat();
  void attack();
  void idle();
};
class Player : Public Character {

protected:
  enum State { idle, moving, eating, attacking };

public:
  void move();
  void idle();
  void eat();
  void attack();
};

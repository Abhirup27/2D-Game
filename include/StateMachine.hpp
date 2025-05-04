#pragma once

#include <State.hpp>
#include <map>
#include <memory>
#include <string>

namespace Game {
using StateRef = std::unique_ptr<State>;

class StateMachine {
private:
  std::map<std::string, StateRef> m_states;
  State *m_currentState;
  State *m_pendingState;
  bool m_isReplacing;
  bool m_isAdding;
  bool m_isRemoving;

public:
  StateMachine()
      : m_currentState(nullptr), m_pendingState(nullptr), m_isReplacing(false),
        m_isAdding(false), m_isRemoving(false) {}
  void AddState(const std::string &name, StateRef newState,
                bool isReplacing = true);
  void RemoveState(const std::string &name);
  void RemoveState();

  void ProcessStateChanges();
  State *GetCurrentState();
};
} // namespace Game

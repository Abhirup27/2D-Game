#pragma once

#include <State.hpp>
#include <map>
#include <memory>
#include <string>

namespace Engine {
using StateRef = std::unique_ptr<State>;

class StateMachine {
private:
  std::map<std::string, StateRef> m_states;
  State *m_currentState;
  StateRef m_pendingState;
  bool m_isReplacing;
  bool m_isAdding;
  bool m_isRemoving;
  std::string m_stateToRemove;
  std::string m_newStateName;

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
} // namespace Engine

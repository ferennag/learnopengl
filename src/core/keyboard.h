#pragma once

#include <memory>
#include <SDL3/SDL_events.h>
#include <unordered_map>

enum Key {
    Up,
    Down,
    Left,
    Right,
};

class Keyboard {
  public:
    static Keyboard &GetInstance();

    Keyboard(const Keyboard &other) = delete;
    Keyboard &operator=(const Keyboard &other) = delete;

    void HandleEvent(SDL_KeyboardEvent *event);
    bool IsPressed(Key key) const;

  private:
    static std::unique_ptr<Keyboard> sInstance;
    std::unordered_map<Key, bool> mState{};

    Keyboard() = default;
};

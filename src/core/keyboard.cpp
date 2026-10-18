#include "keyboard.h"

std::unique_ptr<Keyboard> Keyboard::sInstance = nullptr;

Keyboard &Keyboard::GetInstance() {
    if (sInstance == nullptr) {
        sInstance = std::unique_ptr<Keyboard>(new Keyboard());
    }

    return *sInstance;
}

void Keyboard::HandleEvent(SDL_KeyboardEvent *event) {
    switch (event->key) {
        case SDLK_W:
        case SDLK_UP:
            mState[Key::Up] = event->down;
            break;
        case SDLK_S:
        case SDLK_DOWN:
            mState[Key::Down] = event->down;
            break;
        case SDLK_A:
        case SDLK_LEFT:
            mState[Key::Left] = event->down;
            break;
        case SDLK_D:
        case SDLK_RIGHT:
            mState[Key::Right] = event->down;
            break;
    }
}

bool Keyboard::IsPressed(Key key) const {
    if (!mState.contains(key)) {
        return false;
    }

    return mState.at(key);
}

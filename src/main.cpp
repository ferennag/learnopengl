// cmake-format off
#include "Application.h"
#include "Game.h"
// cmake-format on

int main(int argc, char **argv) {
    Application app;
    if (!app.initialize()) {
        return -1;
    }

    Game game;
    if (!game.initialize()) {
        app.shutdown();
        return -1;
    }

    app.run(game);

    game.destroy();
    app.shutdown();
    return 0;
}

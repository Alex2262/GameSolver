//
// Created by Alexander Tian on 5/25/25.
//

#ifndef GAMESOLVER_INTERFACE_H
#define GAMESOLVER_INTERFACE_H

#include "engine.h"
#include "game.h"

class Interface {
private:

    std::string msg{};
    std::vector<std::string> tokens{};

    std::unique_ptr<Core::Engine> engine;
    std::unique_ptr<Core::Game> game;

public:
    Interface();

    void game_loop();
};

#endif //GAMESOLVER_INTERFACE_H

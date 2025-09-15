//
// Created by Alexander Tian on 9/14/25.
//

#ifndef GAMESOLVER_CONFIG_H
#define GAMESOLVER_CONFIG_H

#include <string>
#include "../core/config_helper.h"
#include "engine.h"
#include "game.h"

class Config {
public:
    explicit Config(const std::string& file_path);

    std::unique_ptr<Core::Game> get_game();
    std::unique_ptr<Core::Engine> get_engine();

private:
    nlohmann::json config;
};

#endif //GAMESOLVER_CONFIG_H

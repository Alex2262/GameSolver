//
// Created by Alexander Tian on 9/14/25.
//

#include <fstream>
#include <iostream>

#include "config.h"
#include "../gomoku/gomoku.h"
#include "../alpha_beta/alpha_beta.h"
#include "../mcts/mcts.h"
#include "../random_engine/random_engine.h"

using json = nlohmann::json;

Config::Config(const std::string& file_path) {
    std::ifstream file(file_path);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open config file path: " + file_path);
    }

    file >> config;

    std::cout << "GOT CONFIG" << std::endl;
}

std::unique_ptr<Core::Game> Config::get_game() {
    std::string game_name = config["game"]["name"].get<std::string>();

    std::unique_ptr<Core::Game> game;
    if (game_name == "Gomoku") {
        game = std::make_unique<Gomoku::Game>(config["game"]);
    } else {
        throw std::runtime_error(std::string("Game name invalid"));
    }

    return game;
}

std::unique_ptr<Core::Engine> Config::get_engine() {
    std::string engine_name = config["engine"]["name"].get<std::string>();

    std::unique_ptr<Core::Engine> engine;
    if (engine_name == "AlphaBeta") {
        engine = std::make_unique<AlphaBeta::Engine>();
    } else if (engine_name == "MCTS") {
        engine = std::make_unique<MCTS::Engine>();
    } else if (engine_name == "Random") {
        engine = std::make_unique<Random::Engine>();
    } else {
        throw std::runtime_error(std::string("Engine name invalid"));
    }

    return engine;
}
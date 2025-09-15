//
// Created by Alexander Tian on 5/25/25.
//

#ifndef GAMESOLVER_CONSTANTS_H
#define GAMESOLVER_CONSTANTS_H

#include <cstdint>
#include <cstdio>
#include <string>
#include "utilities.h"

inline const std::string PROJECT_ROOT = get_project_root();

const std::string CONFIG_PATH = PROJECT_ROOT + "/config.json";

constexpr size_t MAX_MOVES = 4096;

constexpr Action NO_ACTION = (1ULL << 60);

constexpr Score SCORE_INF = 300'000;


const std::string RESET  = "\033[0m";
const std::string RED    = "\033[31m";
const std::string YELLOW = "\033[33m";
const std::string GREEN  = "\033[32m";
const std::string CYAN   = "\033[36m";


#endif //GAMESOLVER_CONSTANTS_H

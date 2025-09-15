//
// Created by Alexander Tian on 5/24/25.
//

#ifndef GAMESOLVER_TYPES_H
#define GAMESOLVER_TYPES_H

#include <cstdint>
#include <cstdio>

using Action = uint64_t;
using Score = int32_t;
using Ply = size_t;

struct ScoredAction {
    Action action;
    Score score;
};

enum class Result : Score {
    NONE = 200'000,
    WIN = 100'000,
    LOSS = -100'000,
    DRAW = 0
};

enum class EngineType {
    AlphaBeta,
    MCTS,
    Random
};

enum class GameType {
    Gomoku
};


#endif //GAMESOLVER_TYPES_H

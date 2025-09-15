cmake_minimum_required(VERSION 3.28)
project(GameSolver)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_DEBUG "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-march=native -O3 -flto")

add_executable(GameSolver main.cpp
        core/state.h
        gomoku/gomoku.cpp
        gomoku/gomoku.h
        core/types.h
        core/game.h
        core/fixed_vector.h
        core/constants.h
        alpha_beta/alpha_beta.cpp
        alpha_beta/alpha_beta.h
        core/engine.h
        core/interface.cpp
        core/interface.h
        core/utilities.cpp
        core/utilities.h
        random_engine/random_engine.cpp
        random_engine/random_engine.h
        mcts/mcts.cpp
        mcts/mcts.h
)

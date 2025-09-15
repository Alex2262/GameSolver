cmake_minimum_required(VERSION 3.28)
project(GameSolver)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_DEBUG "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-march=native -O3 -flto")

add_executable(GameSolver cpp/main.cpp
        cpp/core/state.h
        cpp/gomoku/gomoku.cpp
        cpp/gomoku/gomoku.h
        cpp/core/types.h
        cpp/core/game.h
        cpp/core/fixed_vector.h
        cpp/core/constants.h
        cpp/alpha_beta/alpha_beta.cpp
        cpp/alpha_beta/alpha_beta.h
        cpp/core/engine.h
        cpp/core/interface.cpp
        cpp/core/interface.h
        cpp/core/utilities.cpp
        cpp/core/utilities.h
        cpp/random_engine/random_engine.cpp
        cpp/random_engine/random_engine.h
        cpp/mcts/mcts.cpp
        cpp/mcts/mcts.h
        cpp/core/config.h
        cpp/core/config.cpp
        cpp/include/nlohmann/json.hpp
        cpp/core/config_helper.h
)

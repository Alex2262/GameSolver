//
// Created by Alexander Tian on 5/24/25.
//

#ifndef GAMESOLVER_GOMOKU_H
#define GAMESOLVER_GOMOKU_H

#include "../core/state.h"
#include "../core/game.h"

namespace Gomoku {

    constexpr int EMPTY = 2;
    constexpr int MAX_SIZE = 63;

    struct Increment {
        int row = 0;
        int col = 0;
    };

    constexpr Increment get_opposite_increment(Increment increment) {
        return Increment{-increment.row, -increment.col};
    }

    constexpr Increment UNIQUE_INCREMENTS[4] = {
            Increment{-1, 0}, Increment{-1, 1}, Increment{0, 1}, Increment{1, 1},
    };

    Action encode_action(int row, int col);
    std::pair<int, int> decode_action(Action action);


class Game : public Core::Game {
private:
    int height;
    int width;
    int win_amt;

public:
    Game(const json& file);

    void parse_config(const json& file);
    std::string name() override { return "Gomoku"; }
    std::unique_ptr<Core::State> new_initial_state() override;
};


class State : public Core::State {
private:
    int height;
    int width;
    int win_amt;
    int side{};

    FixedVector<Action, MAX_SIZE * MAX_SIZE> past_actions;
    std::vector<std::vector<int>> board;

public:
    State(int p_height, int p_width, int p_win_amt);

    void reset() override;
    void print() override;
    void apply_action(Action action) override;
    void undo_action(Action action) override;
    void get_legal_actions(FixedVector<ScoredAction, MAX_MOVES>& actions) override;

    Action parse_action(std::string& str) override;
    std::string get_action_string(Action action) override;

    [[nodiscard]] Result get_result() override;
    [[nodiscard]] int get_current_player() override;
};


}

#endif //GAMESOLVER_GOMOKU_H

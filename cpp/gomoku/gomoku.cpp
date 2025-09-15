//
// Created by Alexander Tian on 5/24/25.
//

#include "gomoku.h"
#include "../core/utilities.h"

namespace Gomoku {

    Action encode_action(int row, int col) {
        Action action = row | (col << 6);
        return action;
    }

    std::pair<int, int> decode_action(Action action) {
        return {action & 63, (action >> 6) & 63};
    }

    void Game::parse_config(const json& file) {
        height = require_range<int>(file, "height", 1, MAX_SIZE);
        width = require_range<int>(file, "width", 1, MAX_SIZE);
        win_amt = require_range<int>(file, "win_amt", 1, MAX_SIZE);
    }

    Game::Game(const json& file) {
        assert(MAX_SIZE * MAX_SIZE <= MAX_MOVES);
        parse_config(file);
    }

    std::unique_ptr<Core::State> Game::new_initial_state() {
        return std::make_unique<Gomoku::State>(height, width, win_amt);
    }

    State::State(int p_height, int p_width, int p_win_amt) {
        height = p_height;
        width = p_width;
        win_amt = p_win_amt;

        Gomoku::State::reset();

        std::cout << "INITIALIZED GOMOKU STATE" << std::endl;
    }

    void State::reset() {
        past_actions.clear();

        board.resize(height, std::vector<int>(width, EMPTY));

        side = 0;
    }

    void State::print() {
        std::string string = "  ";
        int max_digits = static_cast<int>(std::max(std::to_string(height).length(), std::to_string(width).length()));

        for (int col = 0; col < width; col++) {
            std::string col_str = std::to_string(col);
            for (int i = 0; i < max_digits - col_str.length(); i++) string += "0";
            string += col_str;
            string += " ";
        }

        string += "\n";

        for (int row = 0; row < height; row++) {
            std::string row_str = std::to_string(row);
            for (int i = 0; i < max_digits - row_str.length(); i++) string += "0";
            string += std::to_string(row);
            string += " ";

            for (int col = 0; col < width; col++) {
                if (board[row][col] == 0) {
                    string += "O";
                }
                else if (board[row][col] == 1) {
                    string += RED + "X" + RESET;
                }
                else string += ".";

                for (int j = 0; j < max_digits; j++) string += " ";
            }
            string += "\n";
        }

        std::cout << string;
    }

    void State::apply_action(Action action) {
        auto [row, col] = decode_action(action);
        board[row][col] = side;
        side ^= 1;
        past_actions.push_back(action);
    }

    void State::undo_action(Action action) {
        past_actions.pop();
        side ^= 1;
        auto [row, col] = decode_action(action);
        board[row][col] = EMPTY;
    }

    void State::get_legal_actions(FixedVector<ScoredAction, MAX_MOVES>& actions) {
        actions.clear();

        for (int row = 0; row < height; row++) {
            for (int col = 0; col < width; col++) {
                if (board[row][col] == EMPTY) {
                    actions.push_back({encode_action(row, col), 0});
                }
            }
        }
    }

    Action State::parse_action(std::string& str) {
        std::vector<std::string> tokens = split(str, ' ');
        int row = std::stoi(tokens[0]);
        int col = std::stoi(tokens[1]);

        if (row < 0 || row >= height || col < 0 || col >= width || board[row][col] != EMPTY) return NO_ACTION;

        Action action = encode_action(row, col);
        return action;
    }

    std::string State::get_action_string(Action action) {
        std::string res = "(";
        auto [row, col] = decode_action(action);
        res += std::to_string(row);
        res += ", ";
        res += std::to_string(col);
        res += ")";
        return res;
    }

    [[nodiscard]] Result State::get_result() {
        if (past_actions.empty()) return Result::NONE;
        if (past_actions.size() == height * width) return Result::DRAW;  // draw

        auto [last_row, last_col] = decode_action(past_actions.back());
        int color = board[last_row][last_col];

        for (Increment increment : UNIQUE_INCREMENTS) {
            Increment opposite_increment = get_opposite_increment(increment);

            int count = 1;
            int new_row = last_row;
            int new_col = last_col;

            for (int i = 0; i < win_amt; i++) {
                if (count == win_amt) break;

                new_row += increment.row;
                new_col += increment.col;

                if (new_row < 0 || new_row >= height || new_col < 0 || new_col >= width) break;
                if (board[new_row][new_col] != color) break;

                count++;
            }

            new_row = last_row;
            new_col = last_col;
            for (int i = 0; i < win_amt; i++) {
                if (count == win_amt) break;

                new_row += opposite_increment.row;
                new_col += opposite_increment.col;

                if (new_row < 0 || new_row >= height || new_col < 0 || new_col >= width) break;
                if (board[new_row][new_col] != color) break;

                count++;
            }

            assert(count <= win_amt);

            if (count == win_amt) {
                return Result::LOSS;  // current side has lost
            }
        }

        return Result::NONE;
    }

    [[nodiscard]] int State::get_current_player() {
        return side;
    }
}
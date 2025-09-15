//
// Created by Alexander Tian on 6/6/25.
//

#ifndef GAMESOLVER_MCTS_H
#define GAMESOLVER_MCTS_H

#include "../core/state.h"
#include "../core/engine.h"


namespace MCTS {

    using NodeIndex = uint32_t;
    constexpr Ply MAX_DEPTH = 128;
    constexpr NodeIndex MAX_NODES = 1'000'000'000;

    class Node {
    public:
        NodeIndex parent = 0;
        NodeIndex children_start = 0;
        NodeIndex children_end = 0;
        double d_score = 0;
        int visits = 0;
        Action last_action;
        double policy = 0.0;

        Node(NodeIndex p_parent, Action p_last_action) {
            parent = p_parent;
            d_score = 0;
            visits = 0;
            last_action = p_last_action;
        }
    };


    class Engine : public Core::Engine {
    private:

        bool stopped = true;
        uint64_t max_time = 10000;
        uint64_t max_depth = 100;

        Core::State* state;

        std::vector<Node> tree{};

        uint64_t start_time = 0;
        Ply seldepth = 0;
        double average_depth = 1;
        uint64_t nodes = 0;

        NodeIndex root_node_index = 0;
        Ply ply = 0;

        std::array<FixedVector<ScoredAction, MAX_MOVES>, MAX_DEPTH> scored_actions;

        //


        // void update_tree(Move move);
        bool has_children(NodeIndex node_index);
        void print_info();

        void descend_to_root(NodeIndex node_index);

        double get_policy(NodeIndex node_index);
        void set_children_policies(NodeIndex node_index);
        NodeIndex select_best_child(NodeIndex node_index);
        NodeIndex selection();
        void expansion(NodeIndex node_index);

        double evaluate_mcts();
        void back_propagation(NodeIndex node_index, double evaluation, Result result);
        NodeIndex get_best_node();
        void mcts();

    public:
        void new_game() override;
        SearchInfo search(Core::State& p_state) override;
    };

}

#endif //GAMESOLVER_MCTS_H

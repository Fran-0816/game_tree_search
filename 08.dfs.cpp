/*
深さ優先探索ですべての節点を訪問
また, トランスポジションテーブルで同一盤面を除去
*/

#include <iostream>
#include <unordered_map>

#include "games/play.hpp"
#include "games/tic_tac_toe.hpp"

using State = tic_tac_toe::State;
using Action = tic_tac_toe::Action;

using tic_tac_toe::zobrist_hashing::HashValue;
std::unordered_map<HashValue, int> state_values;

static int node_count = 0;

int dfs(const State& state, const bool eliminate_same_position) {
    ++node_count;
    int state_value = -1;
    if (state.is_done()) {
        switch (state.get_winning_status()) {
        case WinningStatus::WIN:
            state_value = 1;
        case WinningStatus::LOSE:
            state_value = -1;
        default:
            state_value = 0;
        }
        if (eliminate_same_position) {
            state_values[state.hash_value] = state_value;
        }
        return state_value;
    }
    auto legal_actions = state.legal_actions();
    for (const auto action : legal_actions) {
        State next_state = state;
        next_state.step(action);
        if (eliminate_same_position && state_values.contains(next_state.hash_value)) {
            continue;
        }
        int value = -dfs(next_state, eliminate_same_position);
        if (value > state_value) {
            state_value = value;
        }
    }
    if (eliminate_same_position) {
        state_values[state.hash_value] = state_value;
    }
    return state_value;
}

int main() {
    auto initial_state = State();
    dfs(initial_state, true);
    std::cout << "visited node\t" << node_count << std::endl;
    return 0;
}
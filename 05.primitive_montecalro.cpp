/*
原始モンテカルロ木探索
*/

#include <array>

#include "games/play.hpp"
#include "games/othello.hpp"

using State = othello::State;
using Action = othello::Action;
using othello::random_action;

int playout(State& state) {
    switch (state.get_winning_status()) {
    case WinningStatus::WIN:
        return 1;
    case WinningStatus::LOSE:
    case WinningStatus::DRAW:
        return 0;
    default:
        state.step(random_action(state));
        return 1 - playout(state);
    }
}

Action primitive_montecalro_action(const State& state, int playout_number) {
    auto legal_actions = state.legal_actions();
    int action_size = legal_actions.size();
    if (!action_size) {
        return othello::NO_POS;
    }
    std::vector<int> values(action_size);
    std::vector<int> counts(action_size);
    for (int t = 0; t < playout_number; ++t) {
        int action_idx = t % action_size;

        State next_state = state;
        next_state.step(legal_actions[action_idx]);
        values[action_idx] += 1 - playout(next_state);
        ++counts[action_idx];
    }

    int best_action_idx = -1;
    static constexpr int INF = 1000000001;
    double best_score = -INF;
    for (int action_idx = 0; action_idx < action_size; ++action_idx) {
        double value_mean = static_cast<double>(values[action_idx]) / counts[action_idx];
        if (value_mean > best_score) {
            best_score = value_mean;
            best_action_idx = action_idx;
        }
    }
    return legal_actions[best_action_idx];
}

int main() {
    std::array<play::Player<State, Action>, 2> players = {
        [](const State& state) { return primitive_montecalro_action(state, 500); },
        [](const State& state) { return random_action(state); },
    };
    play::test_ai(players, 100);
    return 0;
}
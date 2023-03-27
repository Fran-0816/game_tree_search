/*
トランスポジションテーブルで同一盤面を除去し, AND/OR 木探索を高速化
*/

#include <array>
#include <random>

#include "games/play.hpp"
#include "games/tic_tac_toe.hpp"

using State = tic_tac_toe::State;
using Action = tic_tac_toe::Action;
using tic_tac_toe::random_action;

using tic_tac_toe::zobrist_hashing::HashValue;
std::unordered_map<HashValue, int> state_values;

int or_score(const State& state);
int and_score(const State& state);

int and_score(const State& state) {
    int state_value = 1;
    switch (state.get_winning_status()) {
    case WinningStatus::LOSE:
        break;
    case WinningStatus::WIN:
    case WinningStatus::DRAW:
        state_value = 0;
        break;
    default:
        auto legal_actions = state.legal_actions();
        for (const auto action : legal_actions) {
            State next_state = state;
            next_state.step(action);
            if (state_values.contains(next_state.hash_value)) {
                state_value &= state_values[next_state.hash_value];
            } else {
                state_value &= or_score(next_state);
            }
            if (!state_value) {
                break;
            }
        }
        break;
    }
    state_values[state.hash_value] = state_value;
    return state_value;
}

int or_score(const State& state) {
    int state_value = 0;
    switch (state.get_winning_status()) {
    case WinningStatus::WIN:
        state_value = 1;
        break;
    case WinningStatus::LOSE:
    case WinningStatus::DRAW:
        break;
    default:
        auto legal_actions = state.legal_actions();
        for (const auto action : legal_actions) {
            State next_state = state;
            next_state.step(action);
            if (state_values.contains(next_state.hash_value)) {
                state_value |= state_values[next_state.hash_value];
            } else {
                state_value |= and_score(next_state);
            }
            if (state_value) {
                break;
            }
        }
        break;
    }
    state_values[state.hash_value] = state_value;
    return state_value;
}

Action and_or_action(const State& state) {
    state_values.clear();
    auto legal_actions = state.legal_actions();
    for (const auto action : legal_actions) {
        State next_state = state;
        next_state.step(action);
        if (and_score(next_state)) {
            return action;
        }
    }
    return legal_actions[0];
}

int main() {
    std::array<play::Player<State, Action>, 2> players = {
        [](const State& state) { return and_or_action(state); },
        [](const State& state) { return random_action(state); },
    };
    play::test_ai(players, 10000);
    return 0;
}
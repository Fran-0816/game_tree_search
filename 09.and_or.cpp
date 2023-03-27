/*
AND/OR 木探索
*/

#include <array>
#include <random>

#include "games/play.hpp"
#include "games/tic_tac_toe.hpp"

using State = tic_tac_toe::State;
using Action = tic_tac_toe::Action;
using tic_tac_toe::random_action;

int or_score(const State& state);
int and_score(const State& state);

int and_score(const State& state) {
    switch (state.get_winning_status()) {
    case WinningStatus::LOSE:
        return 1;
    case WinningStatus::WIN:
    case WinningStatus::DRAW:
        return 0;
    default:
        auto legal_actions = state.legal_actions();
        for (const auto action : legal_actions) {
            State next_state = state;
            next_state.step(action);
            if (!or_score(next_state)) {
                return 0;
            }
        }
        return 1;
    }
}

int or_score(const State& state) {
    switch (state.get_winning_status()) {
    case WinningStatus::WIN:
        return 1;
    case WinningStatus::LOSE:
    case WinningStatus::DRAW:
        return 0;
    default:
        auto legal_actions = state.legal_actions();
        for (const auto action : legal_actions) {
            State next_state = state;
            next_state.step(action);
            if (and_score(next_state)) {
                return 1;
            }
        }
        return 0;
    }
}

Action and_or_action(const State& state) {
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
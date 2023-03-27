/*
反復深化 A* 探索
std::shared_ptr は explore_with_threshold を抜けるたびにメモリを解放する
*/

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <vector>

#include "games/fifteen_puzzle.hpp"

using State = fifteen_puzzle::State;
using StatePtr = fifteen_puzzle::StatePtr;
using Action = fifteen_puzzle::Action;

using fifteen_puzzle::zobrist_hashing::HashValue;

std::vector<State> get_shortest_path(StatePtr state) {
    std::vector<State> shortest_path;
    while (state->pre_state_ptr != nullptr) {
        shortest_path.emplace_back(*state);
        state = state->pre_state_ptr;
    }
    shortest_path.emplace_back(*state);
    std::reverse(shortest_path.begin(), shortest_path.end());
    return shortest_path;
}

std::pair<bool, std::vector<State>> explore_with_threshold(int threshold) {
    auto initial_state = std::make_shared<State>();
    auto frontier = std::priority_queue<StatePtr, std::vector<StatePtr>, std::function<bool(const StatePtr, const StatePtr)>>(fifteen_puzzle::compare_f_cost);
    frontier.emplace(initial_state);
    std::unordered_map<HashValue, int> state_cost;
    std::unordered_set<HashValue> explored;
    state_cost[initial_state->hash_value] = initial_state->get_f_cost();
    std::vector<State> shortest_path;
    bool is_solved = false;
    while (!frontier.empty()) {
        auto state = frontier.top();
        if (state->is_done()) {
            std::cout << threshold << std::endl;
            shortest_path = get_shortest_path(state);
            is_solved = true;
            break;
        }
        frontier.pop();
        if (state->get_f_cost() != state_cost[state->hash_value]) {
            continue;
        }
        explored.emplace(state->hash_value);
        auto legal_actions = state->legal_actions();
        for (const auto action : legal_actions) {
            StatePtr next_state = std::make_shared<State>(*state);
            next_state->step(action);
            if (explored.contains(next_state->hash_value)) {
                continue;
            }
            int next_f_cost = next_state->get_f_cost();
            if (next_f_cost > threshold) {
                return {false, std::vector<State>()};
            }
            if (state_cost.contains(next_state->hash_value) && state_cost[next_state->hash_value] < next_f_cost) {
                continue;
            }
            next_state->pre_state_ptr = state;
            frontier.emplace(next_state);
            state_cost[next_state->hash_value] = next_f_cost;
        }
    }
    return {is_solved, shortest_path};
}

std::vector<State> explore() {
    for (int threshold = 1; ; ++threshold) {
        const auto [is_solved, shortest_path] = explore_with_threshold(threshold);
        if (is_solved) {
            return shortest_path;
        }
    }
}

int main() {
    auto shortest_path = explore();
    std::cout << "shortest path length\t" << shortest_path.size() << std::endl;
    for (const auto& state : shortest_path) {
        std::cout << state << std::endl;
    }
    return 0;
}
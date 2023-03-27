#include "tic_tac_toe.hpp"

#include <random>

namespace tic_tac_toe {

namespace zobrist_hashing {

HashMap make_hash_map() {
    static std::mt19937 engine{std::random_device()()};
    HashMap hash_map;
    BitBoard piece = 1;
    for (int h = 0; h < 3; ++h) {
        for (int w = 0; w < 3; ++w) {
            hash_map[piece] = engine();
            piece <<= 1;
        }
        piece <<= 1;
    }
    return hash_map;
}

} // namespace zobrist_hashing

zobrist_hashing::HashMap TicTacToeState::hash_maps[2] = {zobrist_hashing::make_hash_map(), zobrist_hashing::make_hash_map()};

void TicTacToeState::step(const BitBoard action) {
    player_position_ ^= action;
    hash_value ^= hash_maps[!is_black_turn][action];
    std::swap(player_position_, opponent_position_);
    ++turn;
    is_black_turn = !is_black_turn;
}

std::vector<BitBoard> TicTacToeState::legal_actions() const {
    std::vector<BitBoard> actions;
    BitBoard pieces = cells_can_put();
    while (pieces) {
        // x & -x で x の最も下位にある 1 ビットを取得できる
        BitBoard action = pieces & -pieces;
        actions.emplace_back(action);
        pieces ^= action;
    }
    return actions;
}

WinningStatus TicTacToeState::get_winning_status() const {
    if (is_done()) {
        if (piece_end_of_three_sequences(player_position_)) {
            return WinningStatus::WIN;
        } else if (piece_end_of_three_sequences(opponent_position_)) {
            return WinningStatus::LOSE;
        } else {
            return WinningStatus::DRAW;
        }
    } else {
        return WinningStatus::NONE;
    }
}

// ゲーム状況の出力
std::ostream& operator<<(std::ostream& os, const TicTacToeState& state) {
    BitBoard black_position;
    BitBoard white_position;
    if (state.is_black_turn) {
        black_position = state.player_position_;
        white_position = state.opponent_position_;
    } else {
        black_position = state.opponent_position_;
        white_position = state.player_position_;
    }

    os << "Turn\t" << state.turn << '\n';
    BitBoard piece = 1;
    for (int h = 0; h < 3; ++h) {
        for (int w = 0; w < 3; ++w) {
            if (w) os << ' ';
            if (black_position & piece) {
                os << 'x';
            } else if (white_position & piece) {
                os << 'o';
            } else {
                os << '.';
            }
            piece <<= 1;
        }
        os << '\n';
        piece <<= 1;
    }
    return os;
}

Action random_action(const State& state) {
    static std::mt19937 engine{std::random_device()()};
    const auto legal_actions = state.legal_actions();
    return legal_actions[engine() % legal_actions.size()];
}

} // namespace tic_tac_toe;
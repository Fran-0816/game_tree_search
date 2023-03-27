#include "othello.hpp"

#include <iostream>
#include <random>

namespace othello {

// position にコマが置かれたマスの評価値の合計を計算
score::ScoreType score::compute_score(const BitBoard position) {
    score::ScoreType score = 0;
    for (const auto& [point_per_cells, point] : score::points_per_cells) {
        score += point * std::popcount(position & point_per_cells);
    }
    return score;
}

void OthelloState::step(const BitBoard action) {
    if (action) {
        put_piece(action);
    }
    std::swap(player_position_, opponent_position_);
    ++turn;
    is_black_turn = !is_black_turn;
}

std::vector<BitBoard> OthelloState::legal_actions() const {
    std::vector<BitBoard> actions;
    BitBoard pieces = cells_can_put(player_position_, opponent_position_);
    while (pieces) {
        // x & -x で, x の 1 が立っている最も下位のビットを取得できる
        BitBoard action = pieces & -pieces;
        actions.emplace_back(action);
        pieces ^= action;
    }
    return actions;
}

// コマの多いプレイヤーが勝ち
WinningStatus OthelloState::get_winning_status() const {
    if (is_done()) {
        const int player_piece_count = count_pieces(player_position_);
        const int opponent_piece_count = count_pieces(opponent_position_);
        if (player_piece_count > opponent_piece_count) {
            return WinningStatus::WIN;
        } else if (player_piece_count < opponent_piece_count) {
            return WinningStatus::LOSE;
        } else {
            return WinningStatus::DRAW;
        }
    } else {
        return WinningStatus::NONE;
    }
}

// ゲーム状況の出力
std::ostream& operator<<(std::ostream& os, const OthelloState& state) {
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
    os << "Black\t" << count_pieces(black_position) << "\tWhite\t" << count_pieces(white_position) << '\n';
    BitBoard piece = 1;
    for (int h = 0; h < 8; ++h) {
        for (int w = 0; w < 8; ++w) {
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
    }
    return os;
}

void OthelloState::put_piece(const BitBoard piece) {
    BitBoard flip_pieces = 0;
    if (auto piece_sequence = opponent_pieces_adjacent_left(piece, opponent_position_); (piece_sequence >> 1) & player_position_) {
        flip_pieces |= piece_sequence;
    }
    if (auto piece_sequence = opponent_pieces_adjacent_right(piece, opponent_position_); (piece_sequence << 1) & player_position_) {
        flip_pieces |= piece_sequence;
    }
    if (auto piece_sequence = opponent_pieces_adjacent_up(piece, opponent_position_); (piece_sequence >> 8) & player_position_) {
        flip_pieces |= piece_sequence;
    }
    if (auto piece_sequence = opponent_pieces_adjacent_down(piece, opponent_position_); (piece_sequence << 8) & player_position_) {
        flip_pieces |= piece_sequence;
    }
    if (auto piece_sequence = opponent_pieces_adjacent_upper_left(piece, opponent_position_); (piece_sequence >> 9) & player_position_) {
        flip_pieces |= piece_sequence;
    }
    if (auto piece_sequence = opponent_pieces_adjacent_upper_right(piece, opponent_position_); (piece_sequence >> 7) & player_position_) {
        flip_pieces |= piece_sequence;
    }
    if (auto piece_sequence = opponent_pieces_adjacent_lower_left(piece, opponent_position_); (piece_sequence << 7) & player_position_) {
        flip_pieces |= piece_sequence;
    }
    if (auto piece_sequence = opponent_pieces_adjacent_lower_right(piece, opponent_position_); (piece_sequence << 9) & player_position_) {
        flip_pieces |= piece_sequence;
    }
    player_position_ ^= piece | flip_pieces;
    opponent_position_ ^= flip_pieces;
}

// position のコマの左に連続する相手コマを列挙
// 以降 7 方向も同様
BitBoard OthelloState::opponent_pieces_adjacent_left(BitBoard position, const BitBoard opponent_position) const {
    BitBoard mask = opponent_position & 0x7e7e7e7e7e7e7e7e;
    BitBoard pieces = mask & (position >> 1);
    pieces |= mask & (pieces >> 1);
    pieces |= mask & (pieces >> 1);
    pieces |= mask & (pieces >> 1);
    pieces |= mask & (pieces >> 1);
    pieces |= mask & (pieces >> 1);
    return pieces;
}

BitBoard OthelloState::opponent_pieces_adjacent_right(BitBoard position, const BitBoard opponent_position) const {
    BitBoard mask = opponent_position & 0x7e7e7e7e7e7e7e7e;
    BitBoard pieces = mask & (position << 1);
    pieces |= mask & (pieces << 1);
    pieces |= mask & (pieces << 1);
    pieces |= mask & (pieces << 1);
    pieces |= mask & (pieces << 1);
    pieces |= mask & (pieces << 1);
    return pieces;
}

BitBoard OthelloState::opponent_pieces_adjacent_up(BitBoard position, const BitBoard opponent_position) const {
    BitBoard mask = opponent_position & 0x00FFFFFFFFFFFF00;
    BitBoard pieces = mask & (position >> 8);
    pieces |= mask & (pieces >> 8);
    pieces |= mask & (pieces >> 8);
    pieces |= mask & (pieces >> 8);
    pieces |= mask & (pieces >> 8);
    pieces |= mask & (pieces >> 8);
    return pieces;
}

BitBoard OthelloState::opponent_pieces_adjacent_down(BitBoard position, const BitBoard opponent_position) const {
    BitBoard mask = opponent_position & 0x00FFFFFFFFFFFF00;
    BitBoard pieces = mask & (position << 8);
    pieces |= mask & (pieces << 8);
    pieces |= mask & (pieces << 8);
    pieces |= mask & (pieces << 8);
    pieces |= mask & (pieces << 8);
    pieces |= mask & (pieces << 8);
    return pieces;
}

BitBoard OthelloState::opponent_pieces_adjacent_upper_left(BitBoard position, const BitBoard opponent_position) const {
    BitBoard mask = opponent_position & 0x007e7e7e7e7e7e00;
    BitBoard pieces = mask & (position >> 9);
    pieces |= mask & (pieces >> 9);
    pieces |= mask & (pieces >> 9);
    pieces |= mask & (pieces >> 9);
    pieces |= mask & (pieces >> 9);
    pieces |= mask & (pieces >> 9);
    return pieces;
}

BitBoard OthelloState::opponent_pieces_adjacent_upper_right(BitBoard position, const BitBoard opponent_position) const {
    BitBoard mask = opponent_position & 0x007e7e7e7e7e7e00;
    BitBoard pieces = mask & (position >> 7);
    pieces |= mask & (pieces >> 7);
    pieces |= mask & (pieces >> 7);
    pieces |= mask & (pieces >> 7);
    pieces |= mask & (pieces >> 7);
    pieces |= mask & (pieces >> 7);
    return pieces;
}

BitBoard OthelloState::opponent_pieces_adjacent_lower_left(BitBoard position, const BitBoard opponent_position) const {
    BitBoard mask = opponent_position & 0x007e7e7e7e7e7e00;
    BitBoard pieces = mask & (position << 7);
    pieces |= mask & (pieces << 7);
    pieces |= mask & (pieces << 7);
    pieces |= mask & (pieces << 7);
    pieces |= mask & (pieces << 7);
    pieces |= mask & (pieces << 7);
    return pieces;
}

BitBoard OthelloState::opponent_pieces_adjacent_lower_right(BitBoard position, const BitBoard opponent_position) const {
    BitBoard mask = opponent_position & 0x007e7e7e7e7e7e00;
    BitBoard pieces = mask & (position << 9);
    pieces |= mask & (pieces << 9);
    pieces |= mask & (pieces << 9);
    pieces |= mask & (pieces << 9);
    pieces |= mask & (pieces << 9);
    pieces |= mask & (pieces << 9);
    return pieces;
}

// 合法手マスを列挙
BitBoard OthelloState::cells_can_put(const BitBoard position, const BitBoard opponent_position) const {
    BitBoard pieces = opponent_pieces_adjacent_left(position, opponent_position) >> 1;
    pieces |= opponent_pieces_adjacent_right(position, opponent_position) << 1;
    pieces |= opponent_pieces_adjacent_up(position, opponent_position) >> 8;
    pieces |= opponent_pieces_adjacent_down(position, opponent_position) << 8;
    pieces |= opponent_pieces_adjacent_upper_left(position, opponent_position) >> 9;
    pieces |= opponent_pieces_adjacent_upper_right(position, opponent_position) >> 7;
    pieces |= opponent_pieces_adjacent_lower_left(position, opponent_position) << 7;
    pieces |= opponent_pieces_adjacent_lower_right(position, opponent_position) << 9;
    return pieces & ~(position | opponent_position);
}

Action random_action(const State& state) {
    static std::mt19937 engine{std::random_device()()};
    const auto legal_actions = state.legal_actions();
    if (legal_actions.empty()) {
        return NO_POS;
    } else {
        return legal_actions[engine() % legal_actions.size()];
    }
}

} // namespace othello
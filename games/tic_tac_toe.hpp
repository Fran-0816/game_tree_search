/*
三目並べの実装
zobrist hashing 付き
*/

#pragma once

#include <ostream>
#include <unordered_map>
#include <vector>

#include "play.hpp"

namespace tic_tac_toe {

using BitBoard = unsigned short;
constexpr BitBoard FULL_POS = 0x0777;

namespace zobrist_hashing {

using HashValue = uint64_t;
using HashMap = std::unordered_map<BitBoard, HashValue>;

HashMap make_hash_map();

} // namespace zobrist_hashing

class TicTacToeState {
public:
    unsigned int turn = 0;
    bool is_black_turn = 1;

    zobrist_hashing::HashValue hash_value = 0;

    void step(const BitBoard action);

    bool is_done() const;

    std::vector<BitBoard> legal_actions() const;

    WinningStatus get_winning_status() const;

    friend std::ostream& operator<<(std::ostream& os, const TicTacToeState& state);

private:
    static zobrist_hashing::HashMap hash_maps[2];

    BitBoard player_position_ = 0;
    BitBoard opponent_position_ = 0;

    BitBoard piece_rightmost_three_sequences(BitBoard position) const;
    BitBoard piece_bottommost_three_sequences(BitBoard position) const;
    BitBoard piece_bottom_rightmost_three_sequences(BitBoard position) const;
    BitBoard piece_top_rightmost_three_sequences(BitBoard position) const;
    BitBoard piece_end_of_three_sequences(const BitBoard position) const;

    BitBoard cells_can_put() const;
};

// 横に 3 つ並んだコマの端のマスを列挙
// 以降 3 方向も同様
inline BitBoard TicTacToeState::piece_rightmost_three_sequences(BitBoard position) const {
    position &= (position << 1);
    position &= (position << 1);
    return position;
}

inline BitBoard TicTacToeState::piece_bottommost_three_sequences(BitBoard position) const {
    position &= (position << 4);
    position &= (position << 4);
    return position;
}

inline BitBoard TicTacToeState::piece_bottom_rightmost_three_sequences(BitBoard position) const {
    position &= (position << 5);
    position &= (position << 5);
    return position;
}

inline BitBoard TicTacToeState::piece_top_rightmost_three_sequences(BitBoard position) const {
    position &= (position << 3);
    position &= (position << 3);
    return position;
}

// 3 つ揃ってなくて, すべてのマスにコマが置かれていれば終端
inline bool TicTacToeState::is_done() const {
    return piece_end_of_three_sequences(player_position_) || piece_end_of_three_sequences(opponent_position_) || ((player_position_ | opponent_position_) == FULL_POS);
}

inline BitBoard TicTacToeState::piece_end_of_three_sequences(const BitBoard position) const {
    return piece_rightmost_three_sequences(position) | piece_bottommost_three_sequences(position) | piece_bottom_rightmost_three_sequences(position) | piece_top_rightmost_three_sequences(position);
}

// まだコマが置かれていないマスが合法手マス
inline BitBoard TicTacToeState::cells_can_put() const {
    return ~(player_position_ | opponent_position_) & FULL_POS;
}

using State = TicTacToeState;
using Action = BitBoard;

Action random_action(const State& state);

} // namespace tic_tac_toe
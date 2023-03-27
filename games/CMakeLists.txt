# コンパイラオプション
add_compile_options(-O3)

# 静的ライブラリを生成
add_library(play STATIC play.cpp)
add_library(othello STATIC othello.cpp)
add_library(tic_tac_toe STATIC tic_tac_toe.cpp)
add_library(fifteen_puzzle STATIC fifteen_puzzle.cpp)
# cmake のバージョンが 3.12 以降であることを要請
# CMAKE_CXX_STANDARD 20 は version 3.12 から
cmake_minimum_required(VERSION 3.12)
# プロジェクト名
project(game_tree_search)

# コンパイラが C++20 に対応していることを要請
# std::popcount など C++20 の機能を使用している
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 一部の記述を ./games と ./utils の CMakeLists.txt に分割
add_subdirectory(games)
add_subdirectory(utils)

# コンパイルオプション
add_compile_options(-O3)

# 実行ファイル生成
add_executable(mini_max 01.mini_max.cpp)
add_executable(alpha_beta 02.alpha_beta.cpp)
add_executable(iterative_deeping 03.iterative_deeping.cpp)
add_executable(evaluate_function 04.evaluate_function.cpp)
add_executable(primitive_montecalro 05.primitive_montecalro.cpp)
add_executable(uct 06.uct.cpp)
add_executable(puct 07.puct.cpp)
add_executable(dfs 08.dfs.cpp)
add_executable(and_or 09.and_or.cpp)
add_executable(transposition_table 10.transposition_table.cpp)
add_executable(a_star 11.a_star.cpp)
add_executable(ida_star 12.ida_star.cpp)

# ライブラリのリンク
target_link_libraries(mini_max PRIVATE play othello)
target_link_libraries(alpha_beta PRIVATE play othello)
target_link_libraries(iterative_deeping PRIVATE play othello time_keeper)
target_link_libraries(evaluate_function PRIVATE play othello time_keeper)
target_link_libraries(primitive_montecalro PRIVATE play othello)
target_link_libraries(uct PRIVATE play othello)
target_link_libraries(puct PRIVATE play othello)
target_link_libraries(dfs PRIVATE play tic_tac_toe)
target_link_libraries(and_or PRIVATE play tic_tac_toe)
target_link_libraries(transposition_table PRIVATE play tic_tac_toe)
target_link_libraries(a_star PRIVATE play fifteen_puzzle)
target_link_libraries(ida_star PRIVATE play fifteen_puzzle)
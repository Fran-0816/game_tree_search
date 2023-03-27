# コンパイラオプション
add_compile_options(-O3)

# 静的ライブラリを生成
add_library(time_keeper STATIC time_keeper.cpp)
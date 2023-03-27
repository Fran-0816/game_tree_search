/*
時間計測器
*/

#pragma once

#include <chrono>

class TimeKeeper {
public:
    explicit TimeKeeper(const int64_t time_threshold)
        : start_time_(std::chrono::high_resolution_clock::now()),
          time_threshold_(time_threshold)
    {}

    bool is_time_over() const;

private:
    std::chrono::high_resolution_clock::time_point start_time_;
    int64_t time_threshold_;
};

inline bool TimeKeeper::is_time_over() const {
    auto diff = std::chrono::high_resolution_clock::now() - start_time_;
    return std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() >= time_threshold_;
}
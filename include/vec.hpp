#pragma once
#include <print>
#include <vector>

template <typename T> void print_vec(const std::vector<T> &vec) {
    for (const auto &elem : vec) {
        std::print("{} ", elem);
    }
}

/**
 * @brief vector demo
 *
 */
void demo_vec();
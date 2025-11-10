#pragma once
#include <print>
#include <queue>

template <typename T> void pop_print(std::priority_queue<T> &q) {
    while (!q.empty()) {
        std::print("{} ", q.top());
        q.pop();
    }
    std::print("\n");
}

template <typename T>
void pop_print2(std::priority_queue<T, std::vector<T>, std::greater<T>> &q) {
    while (!q.empty()) {
        std::print("{} ", q.top());
        q.pop();
    }
    std::print("\n");
}

struct Employee;
void demo_queue();
void demo_heap();
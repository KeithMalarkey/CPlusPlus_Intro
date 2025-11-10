#include "set.hpp"
#include <print>
#include <set>
#include <string>

void print_set(const auto &s) {
    for (const auto &x : s) {
        std::print("{} ", x);
    }
    std::println();
}

void demo_set() {
    std::set<std::string> s{};
    s.emplace("track");
    s.emplace("car");
    s.emplace("boat");
    s.emplace("plane");
    print_set(s);
}
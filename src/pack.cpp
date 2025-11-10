#include "pack.hpp"
#include <print>

template <typename... Args> auto sum(const Args &...args) {
    return (args + ...);
} // 一元右折叠
template <typename... Args> auto product(const Args &...args) {
    return (args * ...);
}
template <typename... Args> auto same_v(const Args &...args) {
    return (... & args);
} // 一元左折叠

template <typename... Args> void print_pack(const Args &...args) {
    (std::print("{} ", args), ...); // 展开参数
}

void demo_pack() {
    std::println("sum(1, 2, 3, 4, 5) = {}",
                 sum(1, 2, 3, 4, 5)); // (1 + ( 2 + ( 3 + ( 4 + 5 ))))
    std::println("product(1, 2, 3, 4, 5) = {}",
                 product(1, 2, 3, 4, 5)); // (1 * ( 2 * ( 3 * ( 4 * 5 ))))
    std::println("same_v(1, 2, 3, 4, 5) = {}",
                 same_v(1, 2, 3, 4, 5)); // ((((1 & 2) & 3) & 4) & 5)
    print_pack("nihao", 3, 4.5, true);
}
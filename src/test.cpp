#include "test.hpp"
#include <algorithm>
#include <cstdint>
#include <print>
#include <vector>

void test() {
    std::vector<int> v{1, 2, 3, 4, 5};
    int sum{0};
    auto f = std::for_each(v.begin(), v.end(), [&sum](int e) { sum += e; });
    std::println("sum of vec: {}", sum);

    // + 小知识点：逗号运算符
    // 逗号运算符允许在一个单一表达式中，对多个子表达进行计算
    // example: 简单实例
    std::uint8_t x{1}, y{2};
    std::println("pre-incremented, x_val: {}, y_val: {}", ++x, ++y); // 2 3
    std::println("pre-incremented, y_val: {}",
                 (++x, ++y)); // +4--->这里表示对y的evaluate
    // *note:
    // *在逗号运算符中，多个子表达式的计算顺序是从左到右，但是返回值却只有最右边一个
    // *参考上面的第二个标准输出
}
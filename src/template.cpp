#include "template.hpp"
#include <algorithm>
#include <cstdlib>
#include <format>
#include <iostream>
#include <print>
#include <vector>

using namespace templateDemo;
// + full specialize 全特化的模板函数不再是一个模板，而是
// + 可以看作一个独立的函数
template <> void templateDemo::fn<int>(int a) {
    std::println("There is {}", a);
}

// 成员模板函数只能在类外实现特化定义
template <> void templateDemo::A::fn<int>(int a) {}

void demo_template() {
    fn('a');                // Here is a
    fn<decltype('a')>('a'); // Here is a
    fn<char>('a');          // Here is a
    fn(7);                  // There is 7
    fn("string");           // Nobody
    // nullptr的类型是nullptr_t
    fn(nullptr); // !Here is 0x0, nullptr解析是有preference
    // * 从上面的重载模板函数解析有如下结论：
    // * there is an inner matching order; that's overload resolution
    // * 这部分重载模板函数推导设计很多推导规则，请参考PPT和CPP-Reference

    std::vector<int> vec(10);
    std::generate(vec.begin(), vec.end(), std::rand);
    for (const auto e : vec) {
        std::print("{} ", e);
    }
    std::println();

    if (true or false) {
        std::println("try for and&or");
    }
    // std::cout << add(3.2f, 1.0f) << std::endl;
    std::cout << std::format("sum of 3.2 and 1.0 is {}", add(3.2f, 1.0f))
              << std::endl;
}

// * 模板是一个 C++ 实体，它定义了以下之一
// + 一族类（类模板），它们可以是嵌套类
// + 一族函数（函数模板），它们可以是成员函数
// + 一族类型的别名（别名模板）    (C++ 11 起)
// + 一族变量（变量模板）  (C++ 14 起)
// + 一个概念（约束与概念）  (C++ 20 起)
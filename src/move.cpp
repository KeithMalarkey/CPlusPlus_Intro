#include "move.hpp"
#include <print>
#include <string>

void demo_move() {
    std::string s{"Hello, Move Semantics!"};
    std::println("Before move: {}", s);

    // std::move实现移动语义，将左值变为右值，更准确地说是将lvalue变成xvalue
    // std::move() 等价于完成静态类型转换
    // std::move（T&& t）完全等价于
    // std::static_cast<T&&>(t)[note:这句话适用于since c++14]
    std::string &&s_rvalue = std::move(s);
    std::println("original string: [{}] and rvalue reference s_ral is [{}]", s,
                 s_rvalue); // 可以看到original还没有变化

    // 真正的移动发生在构造或赋值时(移动构造/赋值)
    std::string new_str{std::move(s)};                  // 这里调用移动构造函数
    std::println("After move, original string: {}", s); // 此时s指针指向nullptr
}
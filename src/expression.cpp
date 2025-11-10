#include "expression.hpp"
#include <cstdint>
#include <iostream>
#include <print>

struct S {
    S() : m{42} {};
    S(int x) : m{x} {}
    int m;
};

namespace value_category {
// *值类型:
// 分类一:泛左值(generalized lvalue, glvalue), 右值(rvalue)
// 分类二: 左值(lvalue), 将亡值(xvalue), 纯右值(prvalue)
struct Exam {
    uint32_t num_{};
    Exam(auto num) : num_(num) {}
    Exam() = default;
};

namespace lvalue {
// + Named variables
// + 左值的数据成员
// + 返回左值引用的函数调用结果
// + 目标类型是左值引用的类型转换/创建左值引用的运算
void check4lvalue() {
    Exam *ex;
    decltype(ex->num_) a{};
    decltype((ex->num_)) b{a};

    int x = 1;
    decltype(auto) y = x;            // y is int
    decltype(auto) z = (y);          // z is int &
    auto h = z;                      // h is int ,可以看到auto忽略const和引用
    decltype(auto) m = std::move(a); // m is uint
    decltype(auto) n = 1;            // n is int
}
} // namespace lvalue

/*
    纯右值：temporary value,such as literals, functions that return type value
*/
namespace prvalue {
// literal except for string literal
void check4prvalue() {
    // nullptr, 1, true等临时值均为纯右值
    // ! string literal不是纯右值
    auto ld = [] { return 1; };

    int a{};
    a++; // + post-increment and post-decrement is prvalue
    //  a++ = 3; // 纯右值是没有storage duration的，没办法赋值
    ++a = 3; // ++a 是一个左值
    std::println("a = {}", a);

    // + 内置算术/逻辑/比较/取地址运算表达式返回纯右值
    // a+b, a||b, a<b , &a

    // + enumerator枚举成员，非指针表示的成员对象和非静态成员函数（a.f,
    // + a->f）；指针表示的成员函数(a.*f,a->*f)

    // + this指针
    // + 非引用类型的转换表达式， 比如static_cast<int>(x), (int)42,
    // + std::string{}

    // * NTTP todo 在引入模板元编程之后介绍
    // * 三元表达式： a ? b : c;
    // * 如果b，c是不同类型或值分类，则三元表达式是纯右值，否则为左值
    int b = 7, c{};
    // a > b ? a : b是左值，因为ab是same type,此时等价于b = c
    (a > b ? a : b) = c;
    // (a > b ? a : 2) = c; // a > b ? a : 2是纯右值
    std::println("a = {}, b = {}", a, b);
}
} // namespace prvalue

namespace xvalue {
// + 右值的成员对象
auto examer = Exam{71}.num_;
// + 创建右值引用的表达式，比如函数调用、类型转换等等
// * static_cast<Tp &&>(t) 完全等价于std::move(Tp t),注意since c++14
Exam exer{static_cast<Exam &&>(examer)};
// + others: [], ?: , ,.
} // namespace xvalue

void print_info(std::string str) { std::println("{}", str); }
} // namespace value_category

void demo_expression() {
    using namespace value_category::lvalue;
    using namespace value_category::prvalue;
    using namespace value_category::xvalue;
    int a{}; // zero initialization , a is an lvalue
    std::println("\na = {}", a);

    S s{};
    std::println("s.m = {}", s.m);
    std::cout << (S{} = S{7}).m << '\n'; // 纯右值可以在左侧

    check4prvalue();
}
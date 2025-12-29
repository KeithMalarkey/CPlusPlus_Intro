#include "decl_type.hpp"
#include <algorithm>
#include <cstdint>
#include <functional>
#include <print>
#include <ranges>
#include <type_traits>
#include <utility>
#include <vector>

/*
 * Content:
 * auto (decayed type) -> decltype -> decltype ([entity])
 * -> decltype([expression]) -> decltype(auto) -> template
 * 前备知识： value category, move semantics
 */

namespace declareAuto {
// + 两个完美转发的形参F和Args,返回类型并不确定，适用decltype(auto)
template <typename F, typename... Args>
decltype(auto) Fuc(F &&func, Args &&...args) noexcept {
    return std::forward<F>(func)(std::forward<Args>(args)...);
}

// F设计如下几个函数：add, max, sort
double add(const double a, const double b) { return a + b; }

auto max(const std::vector<int> &a) {
    return *std::max_element(a.begin(), a.end());
}

} // namespace declareAuto

struct A {
    std::int8_t num_{};
    auto getNum() { return num_; }
};

void demo_decltype() {
    int a{};
    const int b{};
    int &c = a;
    const int &d = a;
    volatile int k = -1;

    // * 求一个type的decayed type
    using dt = std::decay_t<decltype(d)>;
    dt mn = 20;
    std::println("decayed type of const-int & is int: {}",
                 std::is_same_v<dt, int>); // true
    // * auto 可以用于类型推导，但是会忽略cv和引用限定(即发生类型退化)
    auto e = a; // int
    auto f = b; // int
    auto g = c; // int
    auto h = d; // int
    auto i = k; // int
    auto str = "hello cpp";
    // * decltype()有两种用法,decltype([entity]) / decltype([expression])
    // * 一. 对于entity，相较于auto会保留const和reference, 不会发生类型退化
    // * entity可以是objects, functions or functors, references and so on.
    decltype(a) ad{};   // int
    decltype(b) bd{};   // const int
    decltype(c) cd = a; // int &
    decltype(d) dd = 1; // const int &
    decltype(k) kd{};   // volatile int
    A *pS;
    decltype(pS->num_) num;
    decltype([] { return 1; }) s;
    std::println("return type of add(1, 3.14) : {}",
                 typeid(add(1, 3.14)).name()); // double
    // * 二. 对于expression表达式
    // * 1.如果表达式值类别是lvalue，则decltype(exp)返回T&；
    // * 2.如果表达式值类别是xvalue，则decltype(exp)返回T&&；
    // * 3.如果表达式值类别是prvalue，则decltype(exp)返回T；
    // + 一个有意思的例子,decltype(a)是对entity a的类型推导，而decltype((a))
    // + 表示对左值表达式(a)的类型推导
    decltype(a) am{};     // am is int
    decltype((a)) bm{mn}; // bm is int &

    // ---------------------------split--------------------
    auto lbd = [] { return 1; };
    decltype(lbd()) lx;
    auto lbd1 = [](int end) {
        for (auto ele : std::views::iota(1, 10) | std::views::filter([](int x) {
                            return x % 2 == 0;
                        })) {
            std::print("{} ", ele);
        }
        std::println();
    };
    auto lbd2 = [] { return 1; };
    auto lbd3 = [=] { return a + 1; };
    std::println("lambda expressions (lbd & lbd1) is the same type and value "
                 "category ? {}",
                 std::is_same_v<decltype(lbd1), decltype(lbd)>); // false
    std::println("lambda expressions (lbd1 & lbd2) is the same type and value "
                 "category ? {}",
                 std::is_same_v<decltype(lbd1), decltype(lbd2)>); // false
    std::println("lambda expressions (lbd & lbd2) is the same type and value "
                 "category ? {}",
                 std::is_same_v<decltype(lbd2), decltype(lbd)>); // false
    std::println("lambda expressions (lbd2 & lbd3) is the same type and value "
                 "category ? {}",
                 std::is_same_v<decltype(lbd2), decltype(lbd3)>); // false

    // !lambda是unique的，看到下面两个完全一样的lambda表达式decltype的类型返回值也不一样
    auto fl = [i](int av, int bv) -> int { return av * bv + i; };
    auto hl = [i](int av, int bv) -> int { return av * bv + i; };
    static_assert(!std::is_same_v<decltype(fl), decltype(hl)>,
                  "The type of a lambda function is unique and unnamed");

    // * 三、decltype(auto)可以直接代替decltype([entity])和decltype([expression])
    decltype(auto) axv = a;
    decltype(auto) ax = (a);
    decltype(auto) bx = lbd;

    // * 四、模板类推导，特别是完美转发的场景
    // 这里采用一个function封装
    std::function<double(const double, const double)> fc = &declareAuto::add;
    // 指针表示
    int (*pfunc)(const std::vector<int> &) = &declareAuto::max;
    std::println("sum: {}", declareAuto::Fuc(fc, 3.2, 5.1));
    std::println("sum: {}",
                 declareAuto::Fuc(pfunc, std::vector<int>{1, -2, 7, 5, 11}));
    // declareAuto::Fuc(F &&func, Args &&args...)
}

/* 什么是类型退化？
类型退化是指编译器自动执行的类型转换，主要包括：
   + 数组类型 → 指针类型
   + 函数类型 → 函数指针类型
   + 移除顶层const/volatile限定符
   + 移除引用
 */

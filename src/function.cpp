#include "function.hpp"
#include <cstdint>
#include <functional>
#include <print>
#include <string>
#include <utility>

// A是一个仿函数，他重写了()的一元和二元运算
struct A {
    bool operator()(int a, int b) const noexcept {
        return a < b;
    } // 算子/运算符重载
    bool operator()(int a) const noexcept { return a < 0; }
};

struct B {
    std::uint8_t x_{}; // 0
    B(std::uint8_t x) : x_{x} {}
    B() = default;
    virtual void test() { std::println("B::test()"); }
    virtual ~B() = default;
};

struct C : virtual public B {
    C() = default;
    void test() override { std::println("C::test()"); }
    ~C() override = default;
};

void show(const std::string &str) { std::println("{}", str); }

void demo_function() {
    // * 存储一个普通函数
    std::function<void(const std::string &)> a{&show};
    a("Hello Cpp");

    // * 存储一个成员函数
    std::function<void(B)> b(&B::test);
    std::function<bool(int, int)> c{A{}};
    std::function<bool(int)> cx{A{}};
    std::println("Compare 3 and 5, 3 < 5 ? {}", c(3, 5));
    std::println("3 > 0 ? {}", cx(3));
    // * 存储一个bind函数
    // bind已经废弃,不建议使用,而是使用lambda表达式代替;或者使用bind_front/bind_back
    std::function<void(std::string)> show_str =
        std::bind(show, std::placeholders::_1);
    show_str("Hello C++23");
    // * 存储lambda表达式
    // 注意<>中指定返回值和参数类型,不能使用auto关键字
    std::function<int(B *, int)> f{[](B *b, int x) { return b->x_ * x; }};
    B b1{3};
    std::println("Result: {}", f(&b1, 2)); // 3 * 2 = 6

    // since c++17
    std::function g{show};

    int i{10};
    std::function fc{[&](const float a, const float b) { return i * (a + b); }};
    std::println("{}", fc(.2, .3));

    // * std::function可以存储模板函数吗？

    // * 所以为什么有了函数指针,还要std::function呢?
    // + std::function可以绑定到任意函数,而函数指针只能绑定到已有函数,
    // + 不能绑定到其他函数而且std::function可以提供[多态],
    // + 可以绑定到不同类型的函数(仿函数Functor),而函数指针只能绑定到
    // + 同一类型的函数所以std::function更加灵活,
    // + 更加适合面向对象编程,可以绑定到任意函数/仿函数
    void (*fp)(const std::string &) = &show;
    fp("Hello C++23 from function pointer");

    // * 多态特性实例
    // 可以看到我们存储的是B类（基类）的test方法
    std::function<void(B &&)> test{
        [](B &&b) { std::forward<B>(b).test(); }}; // 完美转发
    test(C{}); // 输出: C::test(), 实现多态特性
}

/*  std::function的用法:
 + std::function提供多态的函数封装器function warpper;
 + std::function可以指定任意函数来封装,而不是只限于已有函数比如上面的show函数
 + 比如lambda表达式,函数指针,成员函数指针等等,
 + bind函数可以将参数绑定到函数上,并返回一个std::function对象
 + std::function<R(Args...)>可以指定返回值和参数类型,不能使用auto关键字
 + e.g. std::function<int(B *, int)> f{[](B *b, int x) { return b->x * x; }};
 + 这里使用了lambda表达式来封装一个函数,参数为B指针和int,
 + 返回值为B指针的x成员乘以int的值
 + 可以这样调用: f(&b1, 2),其中&b1为B的指针实例,2为int值
 */
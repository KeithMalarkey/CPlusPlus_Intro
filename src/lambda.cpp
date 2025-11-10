#include "lambda.hpp"
#include <cstdint>
#include <iostream>
#include <memory>
#include <print>
#include <string>

namespace tempLambda {
class MyClass {
  private:
    std::int8_t x_{};
    std::string name_{};

  public:
    MyClass(std::int8_t x, std::string name) : x_{x}, name_{name} {}
    std::int8_t get_x() const;
    std::string get_name() const;
};

std::int8_t MyClass::get_x() const { return x_; }
std::string MyClass::get_name() const { return name_; }

auto glambda = [](auto a, auto &&b) { return a < b; };

// * lambda表达式其实质是匿名结构体,其capture可以理解为lambda表达式的外部链接变量
// * 下面举一个例子:
struct Pointer {
    Pointer() = default;
    Pointer(std::int16_t x, std::int16_t y) : x_{x}, y_{y} {};
    std::int16_t x_;
    std::int16_t y_;

    bool operator()(const Pointer &a, const Pointer &b) const noexcept {
        return a.x_ < b.x_ && a.y_ < b.y_;
    }
};
// 看一个等价的lambda表达式:
auto pointer = [](const Pointer &a, const Pointer &b) {
    return a.x_ < b.x_ && a.y_ < b.y_;
};

// generic lambda, operator() is a template with one parameter
auto vglambda = [](auto printer) {
    return [=](auto &&...ts) // generic lambda, ts is a parameter pack
    {
        printer(std::forward<decltype(ts)>(ts)...);
        // nullary lambda (takes no parameters):
        return [=] { printer(ts...); };
    };
};

// * Note: lambda objects are copyable, but some scenarios may not work,
// * depending on the captured variables
auto p =
    vglambda([](auto v1, auto v2, auto v3) { std::cout << v1 << v2 << v3; });
// * Note: lambda objects are movable
auto move_p = std::move(p);

auto ptr = [init_ptr = std::make_unique<MyClass>(10, "hello")] {
    return init_ptr->get_name() + " " + std::to_string(init_ptr->get_x());
};
// ptr is not copyable, so the following line will not compile: copy constructor
// is deleted implicitly
// auto copy_ptr = ptr;

// so, in this case, u just need to move the lambda object to another variable
auto move_ptr = std::move(ptr);

} // namespace tempLambda

void demo_lambda() {
    const int &x = 10;
    int y = x;
    auto add = [](int x, int y) { return x + y; };
    auto say_hello = [] { std::cout << "Hello, world!\n"; };
    auto get_double = [&] {
        y *= 2;
        std::cout << "inner y = " << y << '\n';
        return y;
    };
    auto lambda_capture = [x]() { std::cout << "x = " << x << '\n'; };
    auto copy = lambda_capture;

    std::cout << "add(5, 7) = " << add(5, 7) << '\n';
    say_hello();
    std::cout << "get_double() = " << get_double() << std::endl;
    std::cout << "outer y = " << y << '\n';

    bool b = tempLambda::glambda(3, 3.14); // OK
    std::cout << "b = " << b << std::endl;
    auto q = tempLambda::p(1, 'a', 3.14); // outputs 1a3.14

    std::println("\n{}", tempLambda::move_ptr()); // out:hello 1o
    std::print("Lambda demo done.\n");
}
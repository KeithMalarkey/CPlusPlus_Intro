#include "pack.hpp"
#include <cmath>
#include <print>

// consteval立即函数
template <typename... Args> consteval auto sum(const Args &...args) {
    return (args + ...);
} // 一元右折叠
template <typename... Args> auto product(const Args &...args) {
    std::println("size of args: {}", sizeof...(args)); // 返回pack的长度
    return (args * ...);
}
template <typename... Args> auto same_v(const Args &...args) {
    return (... & args);
} // 一元左折叠

template <typename... Args> void print_pack(const Args &...args) {
    (std::print("{} ", args), ...); // 展开参数
}

namespace const_init_eval_expr {
// 算术平方根, constexpr修饰函数可以是编译时常量表达也可以在运行时调用
template <typename... Args> constexpr auto sumAndRoot(Args... args) {
    return std::sqrt((args + ...));
}
template <typename Tp> consteval auto square(Tp input) { return input * input; }

const char *g() { return "dynamic initialization"; }
constexpr const char *f(bool p) { return p ? "constant initialization" : g(); }
constinit const char *ch = f(true);
// constinit const char *dh = f(false); // error
// int arr[size]{}; // error
// constinit声明的size大小可变，但是数组大小要在编译器确定，故报错
} // namespace const_init_eval_expr

void demo_pack() {
    std::println("sum(1, 2, 3, 4, 5) = {}",
                 sum(1, 2, 3, 4, 5)); // (1 + ( 2 + ( 3 + ( 4 + 5 ))))
    std::println("product(1, 2, 3, 4, 5) = {}",
                 product(1, 2, 3, 4, 5)); // (1 * ( 2 * ( 3 * ( 4 * 5 ))))
    std::println("same_v(1, 2, 3, 4, 5) = {}",
                 same_v(1, 2, 3, 4, 5)); // ((((1 & 2) & 3) & 4) & 5)
    print_pack("nihao", 3, 4.5, true);

    using namespace const_init_eval_expr;
    // constexpr的求值时机：编译时或运行时
    std::println("\nRes of compile-time sumRoot: {}", sumAndRoot(9, 16));
    // 但是constexpr也接受运行时计算
    int x{}, y = 64; // 运行时变量
    std::println("Res of running-time sumRoot: {}", sumAndRoot(x, y));
    // consteval的求值时机只能是编译时
    // std::println("Res of consteval sumRoot: {}", square(x)); // 禁止调用
    const int z = 9;
    std::println("Res of consteval square: {}", square(z));
    int a{};
    std::println("[a size is {}]", sizeof a);

    // * 断言变量具有静态初始化，即零初始化和常量初始化，否则程序格式错误
    // * constinit 说明符声明static/global/threadlocal的变量。
    // * constinit 说明符也可以应用于结构化绑定声明。在这种情况下，constinit
    // * 也应用于声明引入的唯一命名变量
    // !见上述namespace中的应用示例
}
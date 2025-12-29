#include "macros.hpp"
#include <print>

// #1
#define SQUARE(x) ((x) * (x)) // ! 注意括号的使用
#define SQUARE1(x) x *x
#define SQUARE2()
// #2 变长参数宏
#define LOG(fmt, ...) std::println("[log] --- " fmt, __VA_ARGS__)
// #3 宏嵌套
#define CONCAT(a, b) a##b
#define MAKE_VAR(name) CONCAT(var_, name)
// #4 条件编译
#ifdef DEBUG
#define DEBUG_PRINT(msg) std::println("[DEBUG] {}", msg)
#else
#define DEBUG_PRINT(msg)
#endif
// #5 宏与字符串化
#define TO_STRING(x) #x
// #6 宏与代码生成
#define GENERATE_GETTER(type, name)                                            \
    type get_##name() const { return name; }
// #7 __VA_OPT__
#define LOG_OPT(fmt, ...)                                                      \
    std::println("[log] --- " fmt __VA_OPT__(, ) __VA_ARGS__)

void demo_macros() {
    int a = 5;
    int result = SQUARE(a + 1);                // Expands to ((a + 1) * (a + 1))
    int result1 = SQUARE1(a + 1);              // Expands to a + 1 * a + 1
    std::println("SQUARE result: {}", result); // Outputs: 36=6*6
    std::println("SQUARE1 result: {}", result1); // Outputs: 11=5 + 1 * 5 + 1

    auto MAKE_VAR(1) = 3;
    std::println("MAKE_VAR(1): {}", var_1); // Outputs: 3

    LOG("Value of a: {}, Result: {}", a, result);
    LOG_OPT("Optional log with no extra args");
    LOG_OPT("Optional log with args: {}, {}", a, result);
    // #if DEBUG
    // DEBUG_PRINT("This is a debug message.");
    // #endif
}
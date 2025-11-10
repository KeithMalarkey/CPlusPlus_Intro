#include "exception.hpp"
#include <print>

void demo_exception() {
    // 示例代码: 抛出并捕获一个标准异常
    try {
        throw std::runtime_error("This is a runtime error");
    } catch (const std::exception &e) {
        std::print("Caught exception: {}\n", e.what());
    }
}
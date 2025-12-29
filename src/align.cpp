#include "align.hpp"
#include <print>

void demo_align() {
    std::println("在这里阐明alignof运算符的用法和alignas说明符的用法。");
    std::println("alignof(int) = {}", alignof(int));                 // 4
    std::println("alignof(double) = {}", alignof(double));           // 8
    std::println("alignof(char) = {}", alignof(char));               // 1
    std::println("alignof(long long) = {}", alignof(long long));     // 8
    std::println("alignof(long double) = {}", alignof(long double)); // 16

    struct Empty {
    } em;
    struct Example {
        int b;
        char c;
        char16_t a;
    };
    std::println("sizeof(em) and alignof(em) empty object: {} and {}",
                 sizeof(Empty), alignof(Empty)); // 1 1
    std::println("alignof(Example) = {}, sizeof(Example) = {}",
                 alignof(Example), sizeof(Example)); // 4 8
    int a = 0;
    Example e{1, '2', u'3'};
    std::println("alignof(a) = {}, alignof(e) = {}, sizeof(e) = {}", alignof(a),
                 alignof(e), sizeof(e));

    struct alignas(16) AlignedExample {
        int b;
        char c;
        char16_t a;
    };
}

/*
 * alignof 是一个运算符，用于查询类型或对象的对齐要求（以字节为单位）
 */
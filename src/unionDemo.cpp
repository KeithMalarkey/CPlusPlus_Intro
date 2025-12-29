#include "unionDemo.hpp"
#include <cstdint>
#include <iostream>

namespace unionDemo {
// 联合体是一种特殊的类类型，它在任何给定时间只能容纳其非静态数据成员中的一个。
// 因此，其大小只和最大的非静态成员相关
union S {
    std::int32_t n;     // occupies 4 bytes
    std::uint16_t s[2]; // occupies 4 bytes
    std::uint8_t c;     // occupies 1 byte
    // char &ch = 'g'; // 联合体无法拥有引用类型对象
    void hello(); // 联合可以有成员函数
    // virtual void hello();
    // 但无法容纳虚函数，因为联合既不能被继承也无法继承其他类
}; // the whole union occupies 4 bytes

// union H : S{} // 无法继承， S也无法被继承

} // namespace unionDemo

void demo_union() {
    using namespace unionDemo;
    S s{0x12345678}; // s.n is now the active member
    // At this point, reading from s.s or s.c is undefined behavior,
    // but most compilers define it.
    std::cout << std::hex << "s.n = " << s.n << '\n';

    s.s[0] = 0x0011; // s.s is now the active member
    // At this point, reading from s.n or s.c is undefined behavior,
    // but most compilers define it.
    std::cout << "s.c is now " << +s.c
              << '\n' // 11 or 00, depending on platform
              << "s.n is now " << s.n << '\n'; // 12340011 or 00115678
}
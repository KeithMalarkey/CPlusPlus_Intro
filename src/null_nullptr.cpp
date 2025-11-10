#include "null_nullptr.hpp"
#include "print"
// #include <cstddef>
namespace show_diff {
void testNull(int) { std::println("int null"); }
void testNull(int *) { std::println("int* null"); }
} // namespace show_diff

void demo_null() {
    show_diff::testNull(nullptr);
    show_diff::testNull(0);
    // overloaded function ambiguity
    // show_diff::testNull(NULL);
}

// * NULL和nullptr的区别
// 1.
// NULL是一个宏，它是一个整型常量，值为0(主要是在MSVC中)或__null(其他编译器);
// nullptr是一个std::nullptr_t类型的纯右值字面量;
// 2.
// NULL在C语言中为(void*)类型,它支持隐式转换为任意指针类型;但是在C++中,NULL被一般定义为0,因此不能隐式转换为任意指针类型;
// nullptr在C++11中被引入,支持隐式转换为任意指针类型;
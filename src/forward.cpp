#include <cstddef>
#include <cstring>
#include <forward.hpp>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <print>
#include <string>
#include <utility>
#include <vector>

namespace forwarding_reference {
/* Forward reference 包含两种定义:
1. 函数模板中形如 T&& (右值)的参数, 其中 T
是模板类型参数;注意:参数不能被cv限定符修饰, 即不能是 const T&& 或 volatile T&&
*/
template <class T>
int f(T &&x) // x is a forwarding reference
{
    return g(std::forward<T>(x)); // and so can be forwarded
}

template <class T>
int g(const T &&
          x); // x is not a forwarding reference: const T is not cv-unqualified

template <class T> struct A {
    template <class U>
    A(T &&x, U &&y, int *p); // x is not a forwarding reference: T is not a
                             // type template parameter of the constructor,
                             // but y is a forwarding reference
};

std::vector<int> vec1{1, 2, 3, 4, 5};
auto &&vec = vec1;
auto i = std::begin(vec);

} // namespace forwarding_reference

namespace Ref {
// 引用折叠
using ref = int &;
using cref = const int &;
using rref = int &&;

int i{10};
ref a = i;    // 左值引用a
ref &b = i;   // 左值引用b
ref &&c = i;  // 左值引用c
cref d = i;   // const左值引用d
cref &e = i;  // const左值引用e
cref &&f = i; // const左值引用f
rref g{10};   // 右值引用g
rref &h = g;  // 左值引用h
rref &&j{20}; // 右值引用j

class Mystring {
    char *data = nullptr;
    std::size_t size = 0;

  public:
    explicit Mystring(const char *str) {
        if (str) {
            size = std::strlen(str);
            data = new char[size + 1];
            std::strcpy(data, str);
        }
        std::print("Constructed Mystring: {}\n", data ? data : "null");
    }

    Mystring(const Mystring &other) {
        if (other.data) {
            size = other.size;
            data = new char[size + 1];
            std::strcpy(data, other.data);
        }
        std::print("Copy constructed Mystring: {}\n", data ? data : "null");
    }

    Mystring(Mystring &&other) noexcept {
        data = other.data;
        size = other.size;
        other.data = nullptr;
        other.size = 0;
        std::print("Move constructed Mystring: {}\n", data ? data : "null");
    }

    Mystring &operator=(Mystring &&other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        std::print("Move assigned Mystring: {}\n", data ? data : "null");
        return *this;
    }

    ~Mystring() {
        std::print("Destructing Mystring: {}\n", data ? data : "null");
        delete[] data;
    }
};

auto print_info(std::string &s) { std::println("Lvalue Reference"); }
auto print_info(std::string &&s) { std::println("Rvalue Reference"); }
/* 模板函数,且T为forwarding reference(转发引用) */
// 情形一:参数为forwarding reference,传入参数为右值引用,
// 折叠语义为右值引用,故输出为右值引用
// !注意转发类型根据decltype(T)决定
// void get_val(auto &&T) { print_info(std::forward<decltype(T)>(T)); }

// 情形二:参数为forwarding
// reference,传入参数为左值/右值,折叠语义为左值/右值,故输出为左值/右值
template <typename Tp> void get_val(Tp &&T) { print_info(T); }

// 情形三:参数为forwarding reference,传入参数为移动语义的右值,故输出一直为右值
// void get_val(auto &&T) { print_info(std::move(T)); }

struct A {
    A(int &&n) { std::println("rvalue overload, n = {}", n); }
    A(int &n) { std::println("lvalue overload, n = {}", n); }
};

class B {
  public:
    template <class T1, class T2, class T3>
    B(T1 &&t1, T2 &&t2, T3 &&t3)
        : a1_{std::forward<T1>(t1)}, a2_{std::forward<T2>(t2)},
          a3_{std::forward<T3>(t3)} {}

  private:
    A a1_, a2_, a3_;
};

template <class T, class U> std::unique_ptr<T> make_unique1(U &&u) {
    return std::unique_ptr<T>(new T(std::forward<U>(u)));
}

template <class T, class... U> std::unique_ptr<T> make_unique2(U &&...u) {
    return std::unique_ptr<T>(new T(std::forward<U>(u)...));
}

auto make_B(auto &&...args) // since C++20
{
    return B(std::forward<decltype(args)>(args)...);
}

} // namespace Ref

void demo_forward() {
    // condition 1
    Ref::get_val("checkin");
    std::string str{"checkout"};
    Ref::get_val(str);

    auto p1 = Ref::make_unique1<Ref::A>(2); // rvalue
    int i = 1;
    auto p2 = Ref::make_unique1<Ref::A>(i); // lvalue

    std::println("B");
    auto t = Ref::make_unique2<Ref::B>(2, i, 3);

    std::println("make_B");
    [[maybe_unused]] Ref::B b = Ref::make_B(4, i, 5);

    // auto x = NULL;
    // std::println("{}", x);
}

// todo std::forward_like std::as_const
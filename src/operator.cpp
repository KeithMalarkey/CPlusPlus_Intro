#include "operator.hpp"
#include <print>

// + “+”号运算符重载
std::vector<int> operator+(const std::vector<int> &a,
                           const std::vector<int> &b) {
    std::vector<int> res{};
    for (int i{}; i < std::max(a.size(), b.size()); ++i) {
        int r1{}, r2{};
        if (i < a.size()) {
            r1 = a.at(i);
        }
        if (i < b.size()) {
            r2 = b.at(i);
        }
        res.emplace_back(r1 + r2);
    }
    return res;
}

struct Integer {
  private:
    int number_;

  public:
    Integer(int n) : number_{n} {}
    // + 重载 "+" 运算符
    friend Integer operator+(const Integer &,
                             const Integer &) noexcept; // 友元函数声明
    Integer &operator+=(const Integer &other) noexcept {
        number_ += other.number_;
        return *this;
    }
    // 只允许左值调用
    // Integer &operator+=(const Integer &other) & {
    //     number_ += other.number_;
    //     return *this;
    // }
};

// 友元函数实现
Integer operator+(const Integer &a, const Integer &b) noexcept {
    return {a.number_ + b.number_};
}

void demo_operator() {
    std::vector<int> v1{1, 2, 4, 3, 4}, v2{5, 6, 7, 8, 9, 10};
    for (const auto it : v1 + v2) {
        std::print("{} ", it);
    }

    Vec<int> x{1, 3}, y{3, 5}; // 1 x 3 + 3 x 5
    // std::print("\n Dot product of x and y: {}", x * y); //
    // 下面是等价的，只不过cpp std允许省略operator
    std::print("\n Dot product of x and y: {}", x.operator*(y));

    int a{1}; // a是lvalue
    a += 1;   // lvalue
    // (a + 1) += 1; // a+1是一个纯右值，没办法被赋值

    Integer b{2};
    b += 1;
    // !与上面内置的+=运算符不同，自定义的+=未限定只有左值可以使用
    // !solution:在+=的overloaded后面加上后置&限定符
    // (b + 1).operator+=(1);
    (b + 1) += 1;
}
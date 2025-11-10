#include "enum.hpp"
#include <cstdint>
#include <map>
#include <print>

namespace enum_demo {
// 无作用枚举
enum Color { Red, Green, Blue }; // Red=0, Green=1, Blue=2
enum Status { Ok = 0, Error = -1 };
// enum Platte { Yellow = 1, Blue = 2, Red = 3 }; //
// 环境被污染,Blue和Red重复定义

// 有作用枚举
enum class Platte { Yellow = 1, Blue = 2, Red = 3 }; // 不会污染环境
enum struct AnimalNum : std::uint16_t {
    Dog = 100,
    Cat = 3000,
    Bird = Cat + 20
}; // 结构体枚举, Dog=100, Cat=3000, Bird=3001

// enum struct Price : float {}; // !只接受整型数据类型
enum struct Altitude : char {
    high = 'h',
    width = 'w', // 尾部逗号在CWG518
}; // char/byte也是可以接受的

/* 类型安全 */
int val1 = Red;
int Val = Status::Ok;

enum struct FruitPrice : short {
    Watermelon = 5, // 5
    Apple = 8,      // (8 - 2) * .7 = 4.2
    Pear = 6,       // 6 * .5 = 3
    Banana = 10,    // 10 * .9 = 9
};

} // namespace enum_demo

void demo_enum() {
    using namespace enum_demo;
    if (Val == val1) { // 隐式转换,虽然正确但不合逻辑
        std::println("equal");
        // std::println("{}", val1); //
        // val1是可以直接进行打印的，但是带作用域的（enum struct |
        // class）枚举类enumerator不行
    } else {
        std::println("not equal");
    }
    std::println("Number of Bird: {}",
                 static_cast<std::uint16_t>(enum_demo::AnimalNum::Bird));

    std::map<FruitPrice, float> discount{{FruitPrice::Apple, .7},
                                         {FruitPrice::Pear, .5},
                                         {FruitPrice::Banana, .9}};
    // FruitPrice fp{FruitPrice::Banana};
    FruitPrice fp{FruitPrice::Pear};
    // FruitPrice fp{FruitPrice::Apple};
    // FruitPrice fp{FruitPrice::Watermelon};
    switch (int ft = 2; fp) { // switch中是可以实现安全初始化，不必泄漏ft
    case FruitPrice::Watermelon:
        std::println("Price of watermelon : {}", static_cast<int>(fp));
        break;
    case FruitPrice::Apple:
        std::println("Price of Apple : {}",
                     (static_cast<int>(fp) - ft) * discount.at(fp));
        break;
    case FruitPrice::Banana:
        std::println("Price of Banana : {}",
                     static_cast<int>(fp) * discount.at(fp));
        break;
    case FruitPrice::Pear:
        std::println("Price of Pear : {}",
                     static_cast<int>(fp) * discount.at(fp));
        break;
    default:;
        std::println("Regretably, no discount today");
    }

    enum struct Sample : std::uint8_t { crack, trunk, air_plane }; // 0 1 2
    switch (Sample s = Sample::trunk; s) {
    case Sample::crack:
        std::println("{}", static_cast<int>(Sample::crack));
        [[fallthrough]]; // 故意贯穿不使用break
    case Sample::air_plane:
        std::println("{}", static_cast<int>(Sample::air_plane));
        break;
    case Sample::trunk:
        std::println("{}", static_cast<int>(Sample::trunk));
    }
}
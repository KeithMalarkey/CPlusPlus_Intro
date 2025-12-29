#include "opt_expect_any_var.hpp"
#include <algorithm>
#include <any>
#include <cmath>
#include <cstddef>
#include <expected>
#include <iomanip>
#include <iostream>
#include <optional>
#include <print>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>

namespace optionalDemo {
// optional 一种类似于指针的智能指针，可以用来存储值或空值，since c++17
// + 函数可能失败、不确定是否有值、不想用裸指针 / “哨兵值 (magic value)” /
// + out-parameter
std::optional<int> maybeDivide(int x, int y) {
    if (y == 0) {
        return std::nullopt; // return empty optional
    }
    return x / y;
}

std::optional<std::size_t> FindInStr(std::string &str, auto predicate) {
    auto it = std::ranges::find_if(str, predicate);
    if (it == str.end()) {
        return std::nullopt;
    }
    return it - str.begin();
}

void demo_opt1() {
    std::string str{"123"};
    auto position = FindInStr(str, [](char c) {
        return c >= '0' and c <= '9' and ((c - '0') % 2 == 0);
    });
    auto ch = position
                  .transform([&str](std::size_t idx) {
                      std::println("the first ocurrence is {}", str.at(idx));
                      return str.at(idx);
                  })
                  .or_else([] {
                      std::println("no ocurrence found!");
                      return std::optional{'?'};
                  });
    std::println("the final character is {}", *ch);
}

void demo_optional() {
    std::println("****************Demo std::optional*************");
    std::optional<int> opt1{1};
    using T = std::integral_constant<bool, false>;
    std::println("value of opt1: {}", opt1.value());

    auto opt2 = maybeDivide(10, 3);
    if (opt2.has_value()) {
        std::println("value of opt2: {}", opt2.value());
        // std::println("value of opt2: {}", *opt2); // + equivalent to above
    } else {
        std::println("opt2 is empty");
    }
    std::optional<int> opt3 = maybeDivide(1, 0);
    if (opt3) { // + equivalent to has_value()
        std::println("value of opt3: {}", opt3.value());
    } else {
        std::println("maybe divide by zero");
    }

    std::map<std::string, int> map1{
        {std::string("e1"), 1}, {std::string("e2"), 2}, {std::string("e3"), 3}};
    auto opt4 = getValue(map1, std::string("e1"));
    if (opt4.has_value()) {
        std::println("value of opt4: {}", opt4.value());
    } else {
        std::println("key e1 not found in map");
    }
    auto opt5 = getValue(map1, std::string("e4"));
    if (opt5.has_value()) {
        std::println("value of opt5: {}", opt5.value());
    } else {
        std::println("key e4 not found in map");
    }

    std::optional opt6 =
        std::make_optional(std::map<std::string, int>{{std::string("e5"), 7}});
    opt6->swap(map1);
    std::println("value of opt6: {}",
                 std::any_cast<int>(opt6->at(std::string("e1"))));

    //  + value_or返回默认值，除非默认值为空
    std::optional<double> opt7{1.0};
    std::println("default value of opt7: {:.1f}", opt7.value_or(3.0));
    opt7 = std::nullopt;
    std::println("default value of opt7: {:.1f} when opt7 is null",
                 opt7.value_or(3.0));

    //  + monadic operations
    // 1、or_else
    // 如果optional非空，则返回optional对象
    // 如果optional是nullopt空对象，则返回or_else中的返回值
    std::println(
        "or_else result when optional is null: {}",
        opt7.or_else([] { return std::make_optional<double>(1.2); }).value());
    std::println(
        "or_else result when optional is not null: {}",
        opt1.or_else([] { return std::make_optional<int>(2); }).value());

    demo_opt1();
}
} // namespace optionalDemo

namespace expectDemo {
enum struct parse_error {
    invalid_input,
    overflow,
};

auto parser_number(std::string_view &str)
    -> std::expected<double, parse_error> {
    const char *begin = str.data();
    char *end;
    double retval = std::strtod(begin, &end);
    std::println("what is retval: {}", retval);

    if (begin == end)
        return std::unexpected(parse_error::invalid_input);
    else if (std::isinf(retval))
        return std::unexpected(parse_error::overflow);

    str.remove_prefix(end - begin);
    return retval;
}

void demo_expected() {
    auto process = [](std::string_view str) {
        std::cout << "str: " << std::quoted(str) << ", ";
        if (const auto num = parser_number(str); num.has_value())
            std::cout << "value: " << *num << '\n';
        // If num did not have a value, dereferencing num
        // would cause an undefined behavior, and
        // num.value() would throw std::bad_expected_access.
        // num.value_or(123) uses specified default value 123.
        else if (num.error() == parse_error::invalid_input)
            std::cout << "error: invalid input\n";
        else if (num.error() == parse_error::overflow)
            std::cout << "error: overflow\n";
        else
            std::cout << "unexpected!\n"; // or invoke std::unreachable();
    };

    for (auto src : {"42", "42abc", "meow", "inf"})
        process(src);
}

} // namespace expectDemo

namespace anyDemo {
// std::any 一种类型安全的类型擦除容器，since c++17
void demo_any() {
    std::println("****************Demo std::any******************");
    std::any a{1};
    std::println("type of a: {}", a.type().name());
    a.reset();
    std::println("check if a has value: {}", a.has_value());
    a = 2.5;
    std::println("value of a: {}", std::any_cast<double>(a));
    // emplace可以是构造参数，也可以是构造对象作为参数，注意decay_copy(type)
    a.emplace<std::string>("hello");
    std::println("value of a: {}", std::any_cast<const std::string &>(a));
    try {
        a = std::any_cast<int &>(a);
    } catch (const std::bad_any_cast &e) {
        std::println("exception caught: {}", e.what());
    }
    auto b = std::any_cast<std::string &&>(std::move(a));
    static_assert(std::is_same_v<decltype(b), std::string>, // b is std::string
                  "b type is not std::string");
}
} // namespace anyDemo

namespace variantDemo {}

void demo_opt_expect_any_var() {
    using namespace optionalDemo;
    using namespace expectDemo;
    using namespace anyDemo;
    using namespace variantDemo;
    // demo_any();
    demo_optional();
    // demo_expected();
}

// * expected
// + template <typename T, typename E> class expected;
// + template <typename T, typename E> class expected<T, E>;
// + 该类模板提供了一种值二选一的表达方式；其中预期值为类型T,非预期值为类型E;
#pragma once

// #include <concepts>
#include <print>
#include <type_traits>
#include <utility>
namespace templateDemo {
template <typename T> void fn(T arg) { std::println("Here is {}", arg); }
template <typename T> void fn(T *arg) { std::println("Nobody"); } // overloads

class A {
  public:
    template <typename T> void fn(T arg) {}
    // template <> void fn(int args) {} // 不能在类内特化
};

template <typename Tp>
    requires std::is_floating_point_v<Tp>
Tp add(Tp a, Tp b) {
    return a + b;
}
// 简洁书写
// template <std::integral Tp> Tp add(Tp a, Tp b) { return a + b; }
} // namespace templateDemo

namespace templateParms {
// +类型一：类型模板形参
template <typename T>
// concept MyConcept = requires(T t1, T t2) { t1 * t2; } || true; //
// concepts是bool
concept MyConcept = requires(T t1, T t2) { t1 * t2; };
template <typename T> class My_vector {};             // 无默认值的模板形参
template <typename T = void> struct My_op_functor {}; // 带默认值的模板形参
template <typename... Ts> class My_tuple {};          // 类型模板形参包
template <MyConcept T>
class My_constrained_vector {}; // 没有默认值的受约束类型模板形参
template <MyConcept T = void>
class My_constrained_op_functor {}; // 带默认值的受约束类型模板形参
template <MyConcept... Ts>
class My_constrained_tuple {}; // 受约束类型模板形参包
// +类型二：非类型模板形参
template <templateDemo::A a> int fn() {}
// +类型三：模板模板形参
// template <parms_list> typename ...
template <typename T> class my_array {};
// two type template parameters and one template template parameter:
template <typename K, typename V, template <typename> typename C = my_array>
class Map {
    C<K> key;
    C<V> value;
};
} // namespace templateParms

namespace Sfinae {
// * substitution failure is not an error(读作：瑟fer内)
// + 当编译器在替换模板参数（template substitution）过程中
// + 发现某个模板实例化会生成无效代码时，这种失败不是编译错误(not an
// + error)，而是从重载候选集中移除该模板，继续尝试其他可行的重载。
// * 主要针对函数模板的重载解析（overload resolution）-适用before c++20(until
// * c++17)
} // namespace Sfinae

namespace Concepts {
// * requires生成一个描述约束的 bool 类型的纯右值表达式
// + 形式一：requires {parms} {requires seqs}
// = [注意requires表达式中的参数没有生命周期、链接和存储]
// + 形式二：requires {requires seqs}
// + notes: requires的不可以具备默认实参
// + notes: requires的参数列表不可以使用...结尾

// * requires seqs可以分为4种约束：简单约束、类型约束、复合约束、嵌套约束
// + 简单约束举例
template <typename T>
concept Addable = requires(T a, T b) { a + b; };

template <typename T, typename U>
concept Swappable = requires(T &&t, U &&u) {
    std::swap(std::forward<T>(t), std::forward<U>(u));
    std::swap(std::forward<U>(u), std::forward<T>(t));
};

// + 类型约束
template <typename T> using Ref = T &;
template <typename T>
concept C = requires {
    typename T::inner; // required nested member name
    // typename S<T>;     // required class template specialization
    typename Ref<T>; // required alias template substitution
};
template <class T, class U> using CommonType = std::common_type_t<T, U>;
template <class T, class U>
concept Common = requires(T &&t, U &&u) {
    typename CommonType<T, U>; // CommonType<T, U> is valid and names a type
    { CommonType<T, U>{std::forward<T>(t)} };
    { CommonType<T, U>{std::forward<U>(u)} };
};

// + 复合约束
template <typename T>
concept C2 = requires(T x) {
    // the expression *x must be valid
    // AND the type T::inner must be valid
    // AND the result of *x must be convertible to T::inner
    { *x } -> std::convertible_to<typename T::inner>;

    // the expression x + 1 must be valid
    // AND std::same_as<decltype((x + 1)), int> must be satisfied
    // i.e., (x + 1) must be a prvalue of type int
    { x + 1 } -> std::same_as<int>;

    // the expression x * 1 must be valid
    // AND its result must be convertible to T
    { x * 1 } -> std::convertible_to<T>;
};

// + 嵌套约束
// template <class T>
// concept Semiregular =
//     DefaultConstructible<T> && CopyConstructible<T> && CopyAssignable<T> &&
//     Destructible<T> && requires(T a, std::size_t n) {
//         requires Same<T *,
//                       decltype(&a)>; // nested: "Same<...> evaluates to true"
//         {
//             a.~T()
//         } noexcept; // compound: "a.~T()" is a valid expression that doesn't
//                     // throw
//         requires Same<T *,
//                       decltype(new T)>; // nested: "Same<...> evaluates to
//                       true"
//         requires Same<T *, decltype(new T[n])>; // nested
//         { delete new T };                       // compound
//         { delete new T[n] };                    // compound
//     };
} // namespace Concepts

void demo_template();
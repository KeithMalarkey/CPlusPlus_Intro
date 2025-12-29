#include "vec.hpp"
#include "queue.hpp"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <print>
#include <string>
#include <utility>
#include <vector>

namespace Vec {
void custom_delimiter() {
    std::println("=== 自定义分隔符 ===");

    std::vector<std::string> words = {"Hello", "World", "C++", "Stream"};

    std::print("单词: ");
    std::copy(words.begin(), words.end(),
              std::ostream_iterator<std::string>(std::cout, " | "));
    std::cout << std::endl;

    // 无分隔符
    std::cout << "连接: ";
    std::copy(words.begin(), words.end(),
              std::ostream_iterator<std::string>(std::cout, ""));
    std::cout << std::endl;
}

class Employee1 {
    std::string name_{};
    std::uint16_t ID_{};

  public:
    Employee1(const std::string &name, const std::uint16_t id)
        : name_{name}, ID_{id} {
        std::println("Common Constructor");
    }
    Employee1(Employee1 &other) noexcept : name_{other.name_}, ID_{other.ID_} {
        std::println("Copy Constructor");
    }

    // Employee1(Employee1 &&other) noexcept
    //     : name_{std::exchange(other.name_, {})},
    //       ID_(std::exchange(other.ID_, {})) {}
    // Employee1(Employee1 &&other) noexcept
    //     : name_{std::move(other.name_)}, ID_{std::move(other.ID_)} {}
    // + 基本数据类型拷贝的代价和移动几乎无差异。所以移动构造时，
    // + 基础类型无所谓是拷贝构造还是移动构造
    // * 考虑到移动操作实际上对原对象资源的窃取，所以是“不安全”操作，因此移动构造
    // * 和赋值一般是nothrow的，即noexcept的，至少是条件noexcept(condition)
    Employee1(Employee1 &&other) noexcept
        : name_{std::move(other.name_)}, ID_{other.ID_} {
        std::println("Move Constructor");
    }

    Employee1 &operator=(const Employee1 &other) {
        if (this == &other)
            return *this;
        this->name_ = other.name_;
        this->ID_ = other.ID_;
        std::println("Copy Assignment");
        return *this;
    }

    // !other是右值吗？
    // !no,右值一般指的是临时值或xvalue(std::move)
    // !它表示来自[caller]的一个右值，但是被调用者[callee]处(即此处)它实际上为左值
    Employee1 &operator=(Employee1 &&other) noexcept {
        if (this == &other)
            return *this;
        // name_ = std::exchange(other.name_, {}); or
        name_ = std::move(other.name_);
        // ID_ = std::exchange(other.ID_, {});
        ID_ = other.ID_;
        std::println("Move Assignment");
        return *this;
    }

    void print() const { std::print("name: {}, id: {}\n", name_, ID_); }
};
} // namespace Vec

void demo_vec() {
    using namespace Vec;
    std::println("*********************** std::vector 示例 "
                 "*****************************");
    // !我们来看看vector容器是如何存入元素并容器
    std::println("[1]");
    std::vector<Employee1> ve{};
    // 预分配capacity=3
    ve.reserve(3);
    Employee1 ee{"Katrina", 27}; // common ctor
    ve.emplace_back(ee);         // copy ctor
    std::println("[2]");
    ve.emplace_back("John", 30); // common + move ctor
    std::println("[3]");
    ve.push_back(Employee1{"Tom", 35}); // common + move ctor

    std::println("List of elements in vector:");
    // 若这里ele不是引用，这需要拷贝构造
    for (const auto &ele : ve) {
        ele.print();
    }

    // method 1 列表初始化
    std::vector<int> v{7, 1, 2, 5, 10, 4, 5, 6};

    // method 2 assidn方式,assign需要添加一个变长列表参数,从而进行拷贝
    // std::vector<int> v{};
    // v.assign({1, 2, 3, 4, 5, 6});

    // method 3
    // 构造函数,指定元素个数和值;这里构造了一个元素个数为6,值均为5的vector
    // std::vector<int> v(6, 5);

    // int a[]{1, 2, 3, 4, 5, 6};
    // method 4
    // std::vector<int> v{a, a + 6};
    // method 5
    // std::vector<int> v(a, a + 6);

    // std::vector<int> v{};
    // v.assign_range(std::initializer_list<int>{1, 2, 3, 4, 5, 6});
    print_vec(v); // 7 1 2 5 10 4 5 6
    std::println();
    std::reverse(v.begin(), v.end());
    print_vec(v); // 6 5 4 10 5 2 1 7
    std::println();
    /* 排序:比较std::sort和std::ranges::sort的区别,
     * 可以发现std::ranges::sort是C++20引入的新算法, 其性能更好,且类型安全,
     * 但是需要编译器支持C++20  */
    std::ranges::sort(v, std::greater{});
    // std::ranges::sort(v);
    // std::sort(v.begin(), v.end(), std::greater{});
    print_vec(v);
    std::println();

    auto end_it = v.end();
    // 指向第一个元素
    // 指向最后一个元素, 这里n可以是positive或negative,表示向前或向后移动n个元素
    std::advance(end_it, -1);
    std::println("Last element: {}", *end_it);
    size_t dist = std::distance(v.begin(), end_it);
    std::println("the distance from the first element {} to {}: {}", v.front(),
                 *end_it, dist);
    v.erase(std::unique(v.begin(), v.end()), v.end());
    std::print("After removing duplicates: ");
    print_vec(v);
    std::println();
    std::rotate(v.begin(), v.begin() + 2, v.end());
    std::print("After rotating: ");
    print_vec(v);
    std::println();

    std::print("after removing even numbers: "); // 移除偶数
    // remove_if
    // 将想要删除的元素移到容器末尾，返回指向第一个待删除元素位置的迭代器
    auto rm = [&v]() {
        v.erase(std::remove_if(v.begin(), v.end(),
                               [](int n) { return n % 2 == 0; }),
                v.end());
    };
    rm();
    print_vec(v);
    std::println();

    // std::vector<int> v{7, 1, 2, 5, 10, 4, 5, 6};
    const std::vector<std::string> &strs{"hello", "world", "cpp", "is",
                                         "awesome"};
    using ref1 = std::iter_value_t<decltype(v.begin())>;
    using ref2 = std::iter_reference_t<decltype(v.rbegin())>;
    using ref3 = std::iter_difference_t<decltype(strs.begin())>;
    std::println("ref1 is int: {}", std::is_same_v<ref1, int>);
    std::println("ref2 is int&: {}", std::is_same_v<ref2, int &>);
    std::println("ref3 is ptrdiff_t: {}", std::is_same_v<ref3, ptrdiff_t>);
    std::println("ref1: {}, ref2: {}, ref3: {}", typeid(ref1).name(),
                 typeid(ref2).name(), typeid(ref3).name());

    /* vector的内存分配在堆内存上,vector对象会自动释放内存资源 */
    {
        std::vector<int> temporaries1{1, 2, 3, 4, 5};
        std::vector<int> temporaries2{6, 7, 8, 9, 10};
        temporaries1.insert(temporaries1.end(), temporaries2.begin(),
                            temporaries2.end());
        std::print("temporaries1: ");
        print_vec(temporaries1);
        std::println();
    } // 注意temporaries1的生命周期在此结束

    Vec::custom_delimiter();

    // * emplace_back & push_back 之间有差异
    std::vector<std::string> str{};
    // emplace_back只需要输入构造参数, 然后进行就地构造
    // push_back需要输入一个对象,可能涉及拷贝构造,移动构造等操作,不使用大对象的尾部插入
    str.emplace_back("hello");
    str.push_back(std::string("world"));
    print_vec(str);
    str.insert(str.begin() + 1, {"keith,", "welcome to cpp"});
    std::println();
    print_vec(str);
    std::println();
    std::println("1'st size of str: {} and capacity: {}", str.size(),
                 str.capacity());
    str.insert_range(str.end(), std::initializer_list<std::string>{
                                    "this", "is", "a", "test"});
    auto ele_last = str.back();
    std::cout << std::format("[{}] is the last element of str\n", ele_last);
    std::println("2'nd size of str: {} and capacity: {}", str.size(),
                 str.capacity());
    // * clear会改变size,但不会改变capacity
    str.clear();
    std::println("3'rd size of str: {} and capacity: {}", str.size(),
                 str.capacity());
    // * resize改变size;如果size大于capacity,重置capacity为size;如果小于size,则进行current_size裁剪
    str.resize(10, "cpp");
    print_vec(str);
    std::println();
    std::println("4'th size of str: {} and capacity: {}", str.size(),
                 str.capacity());
    // * max_size是对空闲空间上进行分配的ele个数，一般很大
    std::println("Current max_size: {}", str.max_size());
    // * reserve为容器分配[预分配]指定大小的capacity;若小于现有capacity,则不会改变capacity
    // * 若大于现有capacity,则扩容capacity
    // ! reserve一般在容器初期设置/预分配capacity
    str.reserve(12);
    std::println("5'th size of str: {} and capacity: {}", str.size(),
                 str.capacity());
    std::println("6'th size of str: {} and capacity: {}", str.size(),
                 str.capacity());
    str.clear();
    std::println("7'th size of str: {} and capacity: {}", str.size(),
                 str.capacity());
    // * 这一步很重要,由于动态分配的缘故,opacity大于size的部分不会主动释放,需要shrink2fit来释放
    str.shrink_to_fit();
}
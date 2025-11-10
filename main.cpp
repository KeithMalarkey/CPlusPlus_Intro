#include "align.hpp"
#include "cast.hpp"
#include "class.hpp"
#include "decl_type.hpp"
#include "enum.hpp"
#include "exception.hpp"
#include "expression.hpp"
#include "forward.hpp"
#include "function.hpp"
#include "lambda.hpp"
#include "memory_sector.hpp"
#include "move.hpp"
#include "null_nullptr.hpp"
#include "operator.hpp"
#include "pack.hpp"
#include "queue.hpp"
#include "ranges_views.hpp"
#include "set.hpp"
#include "smart.hpp"
#include "vec.hpp"

// struct Entry {
//     std::uint32_t id{};
//     std::string desc{"test"};
// };

// struct Individual {
//   private:
//     std::string name;
//     int age;
// };

int main() {
    // demo_forward();
    // demo_null();
    // demo_cast();
    // demo_lambda();
    // demo_pack();
    // demo_expression();
    // demo_operator();
    // demo_move();
    // demo_exception();
    // demo_vec();
    // demo_memory_sector();
    // demo_queue();
    // demo_function();
    // demo_ranges_views();
    // demo_set();
    demo_align();
    // demo_enum();
    // demo_decltype();

    // Entry item1{.id = 1, .desc = "An entry"};
    // order is that matters 顺序很重要
    // Entry item2{.desc = "Another entry", .id = 2};

    // Entry item2{2, "Another entry"};
    // Entry item3{.id = 3, .desc{"Third entry"}};
    // 非聚合类型不能使用聚合初始化
    // Individual person{.name = "Alice", .age = 30};
    // std::cout << "Color value: " << color << std::endl;
    // enum_demo::demo();

    // int x = 1, y = 2;
    // const auto &[a, b] = std::pair{x, y};
    // const auto &[t, _] = std::pair{x, y};
    // std::println("a: {}, b: {}, t: {}", a, b, t);
    // std::string name{"keith"};
    // int score{98};
    // const auto &record = std::tie(name, score); // tuple
    // std::print("Name: {}, Score: {}\n", std::get<0>(record),
    //            std::get<1>(record));

    // struct Entry {
    //     int id;
    //     std::string desc{"test"};
    // };
    // Entry item{.id = 1};
    // const auto &[id, desc] = item;
    // std::println("ID: {}, Description: {}", id, desc);

    // std::tuple<int, float> ti{3, 92.1f};
    // const auto &[i, f]{ti};
    // const auto &[j, g] = std::move(ti);
    // std::println("the first element of ti: {}, the second element: {}",
    //              std::get<0>(ti), std::get<1>(ti));
    // auto mk = std::move(ti);
    // std::println("the first element of mk: {}, the second element: {}",
    //              std::get<0>(ti), std::get<1>(ti));

    // // 移动语义对基本类型无效
    // std::tuple<std::string, bool> m{"wangfang", false};
    // auto [name2, status] = std::move(m);
    // std::println("the first element of m: {}, the second element: {}",
    //              std::get<0>(m), std::get<1>(m));

    // constructor::Base base(1, "BaseOne");
    // std::print("Base ID: {}, Name: {}\n", base.getID(), base.getName());
    // constructor::Base base1;
    // std::print("Base1 ID: {}, Name: {}\n", base1.getID(), base1.getName());
    // 每次构建派生类都要先构建相应的基类;
    // 这会造成额外的开销, 包括基类内容相应的虚表等等;
    // 注意,派生类必须实现基类的虚析构方法,否则基类资源可能不会被释放,导致内存泄漏;
    // constructor::Derived derived(2, "DerivedOne", "Engineer", "R&D");

    // std::vector<constructor::Base> bases;
    // bases.emplace_back(1, "BaseOne");
    // bases.emplace_back(2, "BaseTwo");
    // bases.emplace_back(3, "BaseThree");
    // constructor::Derived derived(100, "Sirius", "Manager", "Sales");
    // static_cast会发生隐式转换,即将派生类转换为基类;注意:会调用基类的拷贝构造函数;
    // auto res = static_cast<constructor::Base>(derived);
    // push_bask会调用拷贝/移动构造函数,将派生类对象切割为基类对象存储在vector中;
    // bases.push_back(static_cast<constructor::Base>(derived));
    // auto it = bases.end();
    // std::advance(it, -1);
    // insert/erase都会销毁(堆上)迭代器对象,导致迭代器失效;
    // 这里的it在insert之后会失效,所以不能再使用it访问元素;
    // bases.insert(it, {4, "BaseFour"});
    // std::print("Last Base in vector - ID: {}, Name: {}\n", it->getID(),
    //            it->getName());

    // tiny demo of smart pointer
    // std::unique_ptr<int> p1 = std::make_unique<int>(10);
    // auto data = p1.get(); // 指针
    // std::println("Value pointed to by p1: {}", *data);
    // std::println("Value pointed to by p1: {}", *p1); // 解引用重载
    // p1.reset();                                      // 释放资源
    // p1.reset(new int(20));                           // 重新分配资源
    // std::println("Value pointed to by p1: {}", *p1);

    // std::weak_ptr<int> wp;
    // {
    //     auto sp = std::make_shared<int>(10);
    //     wp = sp;
    //     std::println("wp.use_count(): {}", wp.use_count());
    //     if (auto sp = wp.lock(); !sp) {
    //         std::println("wp is expired or null");
    //     } else {
    //         std::println("Value pointed to by sp: {}", *sp);
    //     }
    //     std::println("Expireding sp...{}", wp.expired() ? "yes" : "no");
    // }
    // std::println("wp.use_count(): {}", wp.use_count());
    // if (auto sp = wp.lock(); !sp) {
    //     std::println("wp is expired or null");
    // } else {
    //     std::println("Value pointed to by sp: {}", *sp);
    // }
    // std::println("Expireding sp...{}", wp.expired() ? "yes" : "no");
    // return 0;
}
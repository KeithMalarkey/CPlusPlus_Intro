#include "queue.hpp"
#include <algorithm>
#include <print>
#include <queue>
#include <string>
#include <string_view>
#include <vector>

/*
 * 优先级队列的实例:优先级队列是序列容器的adaptor;它维护的是二叉堆结构;
 */

struct Employee {
    std::string name;
    int age;
    double salary;
};

void demo_heap() {
    std::vector<std::string> v{"banana", "apple", "orange", "pear"};
    std::print("Original data:");
    std::for_each(v.begin(), v.end(),
                  [](const std::string_view &s) { std::print("{} ", s); });
    std::println();
    // make_heap参数的迭代器必须满足LegacyRandomAccessIterator的要求_随机访问迭代器
    // std::make_heap(v.begin(), v.end());
    std::make_heap(v.begin(), v.end(), std::greater<std::string>());
    std::print("Heapified data:"); // apple banana orange pear
    std::for_each(v.begin(), v.end(),
                  [](const std::string_view &s) { std::print("{} ", s); });
    std::println();
    std::pop_heap(v.begin(), v.end());
    std::print("After pop_heap:");
    std::for_each(v.begin(), v.end(),
                  [](const std::string_view &s) { std::print("{} ", s); });
    std::println();

    std::string s = "pear", x = "apple";
    std::println("{}", s > x); // true
}

void demo_queue() {
    demo_heap();

    const auto data = {1, 8, 5, 6, 3, 4, 0, 9, 7, 2};
    std::println("Original data:");
    std::for_each(data.begin(), data.end(),
                  [](int x) { std::print("{} ", x); });
    std::priority_queue<int> pq{};
    std::for_each(data.begin(), data.end(), [&pq](int x) { pq.push(x); });
    std::println("\nAfter pushing data to priority queue:");
    pop_print(pq);
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq2{};
    std::for_each(data.begin(), data.end(), [&pq2](int x) { pq2.push(x); });
    std::println("After pushing data to priority queue with greater<int>:");
    pop_print2(pq2);

    // Cmp comparator for Employee struct, 结构体实现比较器
    // struct Cmp {
    //     bool operator()(const Employee &a, const Employee &b) const {
    //         return a.salary > b.salary;
    //     }
    // };

    // Create a queue of employees with custom comparator
    // std::priority_queue<Employee, std::vector<Employee>, Cmp> employees{};
    std::priority_queue<Employee, std::vector<Employee>,
                        decltype([](const Employee &a, const Employee &b) {
                            return a.salary > b.salary;
                        })>
        employees{}; // lambda表达式作为比较器
    // Add employees to the queue
    employees.push({"John", 30, 50000});
    employees.push({.name = "Bosh", .age = 25, .salary = 60000});
    employees.push({"Sarah", 35, 70000});
    employees.push({"Mike", 28, 45000});
    employees.push({"Alice", 40, 80000});
    for (; !employees.empty(); employees.pop()) {
        std::println("Employee: {}, Age: {}, Salary: {}", employees.top().name,
                     employees.top().age, employees.top().salary);
    }

    std::queue<Employee> q{}; // 默认是双端队列作为底层容器
    q.push({"John", 30, 50000});
    q.push({.name = "Bosh", .age = 25, .salary = 60000});
    q.push({"Sarah", 35, 70000});
    q.push({"Mike", 28, 45000});
    q.push({"Alice", 40, 80000});
    std::println("First employee in queue: {}, Age: {}, Salary: {}",
                 q.front().name, q.front().age, q.front().salary);
    auto last = q.back(); // 访问队尾元素
    std::println("Last employee in queue: {}, Age: {}, Salary: {}", last.name,
                 last.age, last.salary);
    std::println("After popping the first employee:");
    q.pop();
    std::println("First employee in queue: {}, Age: {}, Salary: {}",
                 q.front().name, q.front().age, q.front().salary);
    // emplace成员函数可以直接构造元素并入队,大对象请选择emplace;
    q.emplace("Tom", 32, 55000);
    std::println("After emplacing a new employee:");
    std::println("Last employee in queue: {}, Age: {}, Salary: {}",
                 q.back().name, q.back().age, q.back().salary);
    // 尾部插入多个元素
    // q.push_range(Rg &&rg)
}

/*
 * 优先级队列中Compare比较器的设定有些反直觉;
 * 底层可以看到它主要在std::make_heap中调用,它给定了一个"触发器";
 * 在新的element进入优先级队列时,会调用这个触发器;具体地,它会调用operator()函数;
 * 这个函数的返回值决定了新element(new_elem)的位置;
 * 比如,如果使用std::greater<int>作为"触发器", 以vector作为可随机访问的底层容器,
 * 实际操作是std::greater<int>{parent of new_elem, new_elem}; (note:
 * 每个new_elem都在vector的末尾),一旦触发器返回true,
 * 那么new_elem就会和其父亲结点交换位置;
 * 举例说明: 以这里的员工名称举例. 假设结构体Cmp作为触发器,
 * vector假设初始化为{"John"[1], "Bosh"[2], "Sarah"[3], "Mike"[4], "Alice"[5]};
 * "John"入堆后, 触发器返回false, 所以不会交换位置;
 * "Bosh"入堆后,由于字典序"John">"Bosh", 触发器返回true,
 */
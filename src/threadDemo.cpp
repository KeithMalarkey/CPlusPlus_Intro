#include "threadDemo.hpp"
#include <chrono>
#include <cstddef>
#include <functional>
#include <print>
#include <thread>
#include <vector>

namespace concur_thread {
// common func as thread func
void MyFunc() {
    std::println("Thread ID of MyFunc: {}", std::this_thread::get_id());
}

void viewer(std::vector<int> &vec, std::size_t begin, std::size_t end,
            int &result) {
    result = 0;
    for (auto i{begin}; i < end; ++i) {
        result += vec.at(i);
    }
    return;
}

void worker(int id) {
    std::println("Worker {} started", id);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::println("Worker {} finished", id);
}

class ThreadDemo {
  public:
    ThreadDemo() {
        std::println("Constructor called in thread ID: {}",
                     std::this_thread::get_id());
    }
    ~ThreadDemo() {
        std::println("Destructor called in thread ID: {}",
                     std::this_thread::get_id());
    }
    ThreadDemo(const ThreadDemo &) {
        std::println("Const copy called in thread ID: {}",
                     std::this_thread::get_id());
    }
    ThreadDemo(ThreadDemo &&) {
        std::println("Move assign called in thread ID: {}",
                     std::this_thread::get_id());
    }
    ThreadDemo operator=(const ThreadDemo &other) {
        std::println("Const copy assign called in thread ID: {}",
                     std::this_thread::get_id());
        return *this;
    }
    ThreadDemo operator=(ThreadDemo &&other) noexcept {
        std::println("Move assign called in Thread ID: {}",
                     std::this_thread::get_id());
        return *this;
    }
};

void fn_demothread(ThreadDemo td) {
    std::println("Thread ID: {}", std::this_thread::get_id());
}

/**
 * @brief 简要介绍一下join()的用法
 *
 */
void demo1() {
    std::println("demo1 function started");
    // * thread object can be move ctor , move assignment and
    // swapped(.swap() or
    // * std::swap()), but not copy ctor and copy assignment
    std::thread t1{worker, 1};
    std::thread t2{worker, 2};

    std::println("thread of demo1 is blocked for waiting t1 and t2");
    // demo1中调用了join()方法，导致demo在该位置被阻塞blocked
    // * 一般需要先检查线程是否可连接joinable
    // if (t1.joinable()) {
    //     t1.join();
    // }
    // if (t2.joinable()) {
    //     t2.join();
    // }
    // 上面的写法是更严谨的写法
    // 那么什么时候恢复？在所有join的线程执行完毕之后
    // here, 需要线程t1和t2执行完成
    // !需要注意的是，t1和t2的执行是相对独立的，你无法只是通过join方法实现并发控制
    // !从而使t1、t2按序执行
    t1.join();
    std::println("thread t1 completed");
    t2.join();
    std::println("thread t2 completed");

    std::println("Okay! thread of demo finished");
    return;
}

void demo2() {
    std::vector<int> vec;
    for (auto i{0}; i < 10; ++i) {
        vec.push_back(i + 1);
    }
    // std::println("ID of demo2: {}", std::this_thread::get_id());
    int result1{0}, result2{0};
    std::thread t1{viewer, std::ref(vec), 0, 10, std::ref(result1)};
    std::thread t2{viewer, std::ref(vec), 1, 2, std::ref(result2)};
    t1.join();
    t2.join();
    std::println("Result1: {}, Result2: {}", result1, result2);
    // * 如果需要一个后台运行的线程，可以使用detach()方法
    // * 比如日志线程或其他快速启动后不关心的线程
    return;
}

void fn() { std::println("Thread ID of fn: {}", std::this_thread::get_id()); }

struct A {
    void operator()() const {
        std::println("Thread ID of functor A: {}", std::this_thread::get_id());
    }
};

void demo3() {
    // * std::thread接受普通函数、函数对象、lambda表达式及成员函数（需绑定对象）作为线程函数
    std::thread t0{[]() {
        std::println("Thread ID in lambda: {}", std::this_thread::get_id());
    }};
    t0.join();
    std::function func = &fn;
    std::thread t1{func};
    {
        std::thread t2{A()};
        t2.detach();
        // t2.join(); // if not join or detach, will std::terminate
    }
    t1.join();
    return;
}

void fn1(int &type) {
    type = 2;
    return;
}
void fn2(int &&type) {
    type = 2;
    return;
}
void fn3(int type) {
    type = 3;
    return;
}
void demo4() {
    int type{1};
    // * std::thread arguments must be invocable after conversion to rvalues
    // std::thread t{fn1, type}; // 类型错误
    std::thread t1{fn1, std::ref(type)};
    std::thread t2{fn2, type};
    std::thread t3{fn3, type};
    // t.join();
    t1.join();
    t2.join();
    t3.join();
}

void demo_jthread() {
    int a{7};
    std::jthread jt{fn1, std::ref(a)};
    std::println("0: now val of a: {}", a); // 7, a is a decay_copy
    std::jthread jt1{fn2, a};
    std::println("1: now val of a: {}", a); // 7, a is a decay_copy
    std::println("Jthread is coming!");
    // jthread 会在析构时自动加入join
}
} // namespace concur_thread

// * 线程模型:一对一（一个创建的thread对应一个内核thread）
// * std::thread创建一个thread对象并和一个thread excutor关联
// * excutor执行的是std::invoke(since c++23)/INVOKE(BEFORE C++23)
// * 对于thread的参数Callable F和Args ...args要求他们可以移动构造

void demo_thread() {
    using namespace concur_thread;
    // + let us see the procedure of thread creation and destruction
    // std::println("Demo thread ID: {}", std::this_thread::get_id());
    std::thread t{fn_demothread, ThreadDemo{}}; // inplace common ctor & move
    if (t.joinable())
        t.join();

    // demo1();
    // demo2();
    // demo3();
    // demo4();
    std::println("number of cores {}", t.hardware_concurrency());
    demo_jthread();
}
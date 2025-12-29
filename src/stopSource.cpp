#include "stopSource.hpp"
// #include <chrono>
#include <chrono>
#include <print>
#include <stop_token>
#include <thread>

namespace myThread {
// std::thread没有线程停止机制，只能通过外部stop_source请求
void worker(std::stop_token token) {
    while (!token.stop_requested()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::println("Still working...");
    }
    std::println("thread exiting...");
}
void stop_thread_with_source() {
    // std::stop_source src{std::nostopstate}; // 1、stop src with no stop state
    std::stop_source src;                   // 1、生成stop src
    std::thread t{worker, src.get_token()}; // 2、生成thread
    std::this_thread::sleep_for(std::chrono::seconds(1));
    src.request_stop(); // 3、请求线程停止
    t.join();
}
void stop_jthread() {}
} // namespace myThread

namespace case_jt {
using namespace std::chrono_literals;

void worker_fun(int id, std::stop_token stoken) {
    for (int i = 10; i; --i) {
        std::this_thread::sleep_for(300ms);
        if (stoken.stop_requested()) {
            std::printf("  worker%d is requested to stop\n", id);
            return;
        }
        std::printf("  worker%d goes back to sleep\n", id);
    }
}

void demo() {
    std::jthread threads[4];
    auto print = [](const std::stop_source &source) {
        std::println("stop_source stop_possible = {}, stop_requested = {}",
                     source.stop_possible(), source.stop_requested());
    };

    // Common source
    std::stop_source stop_source;
    // #1
    print(stop_source);

    // Create worker threads
    for (int i = 0; i < 4; ++i)
        threads[i] = std::jthread(worker_fun, i + 1, stop_source.get_token());

    std::this_thread::sleep_for(500ms);

    std::puts("Request stop");
    stop_source.request_stop();
    // #2
    print(stop_source);

    // Note: destructor of jthreads will call join so no need for explicit calls
}
} // namespace case_jt

void demo_jthread() {
    using namespace case_jt;
    demo();
    myThread::stop_thread_with_source();
}

// * std::jthread提供了安全、可控的线程请求(request)停止方式，是jthread的核心机制
// * 主要涉及stop_token/stop_source/stop_callback/condition_variable_any
// * stop_source可以看作请求线程停止的主体requester or sender
// * stop_token作为线程请求信号的接收器receiver,用来查询是否收到请求停止信号
// * stop_callback则是和stop_token绑定，一旦stop_source的停止请求发出，则callback立即执行
// + 依据stop_callback的特点，可以用来1、在线程结束时请求资源清理；
// + 2、通知条件变量；3、通知event loop推出；4、中断阻塞系统调用。

// * stop 机制适用场景（99 % 的用途）
// ✔ 可中断的循环（典型）
// ✔ 网络 IO 线程
// ✔ event - loop
// ✔ 后台 worker thread
// ✔ 队列消费者线程
// ✔ 定时任务线程
// ✔ 需要安全退出的模块
// + 不适用：
// ✘ 强制中断
// ✘ 杀死线程（C++ 永远不会支持）
// ✘ 中断 OS 阻塞调用（必须配合条件变量或 cancelable IO）
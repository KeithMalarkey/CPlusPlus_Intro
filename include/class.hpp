#pragma once
#include <cstdint>
#include <print>
#include <string>

/**
 * @brief 类相关演示
 *
 */
namespace constructor {

class Base {
  public:
    Base() = default;
    Base(uint16_t id, const std::string &name = "default")
        : id_(id), name_(name) {
        std::println("Base constructor implemented!", id, name);
    }
    Base(const Base &other) : id_(other.id_), name_(other.name_) {
        std::println("Base copy constructor implemented!", id_, name_);
    }
    // 移动构造函数必须加 noexcept
    // 解释:
    // 移动构造过程是将新建对象指针指向旧对象的资源，然后将旧对象的资源指针置为空。
    // 如果移动构造过程中抛出异常，旧对象的资源指针已经被置为空，导致资源泄漏。
    // 因此，移动构造函数必须保证不抛出异常，否则会导致资源泄漏。
    Base(Base &&other) noexcept
        : id_(other.id_), name_(std::move(other.name_)) {
        std::println("Base move constructor implemented!", id_, name_);
        other.id_ = 0;
        other.name_ = "moved-from";
    }
    // delegating constructor
    Base(const std::string &name) : Base(0, name) {}

    Base &operator=(const Base &other) {
        if (this != &other) {
            id_ = other.id_;
            name_ = other.name_;
        }
        return *this;
    }

    virtual ~Base() { std::println("Base destructor called!"); };
    int getID() const { return id_; }
    const std::string &getName() const { return name_; }

  private:
    uint16_t id_;
    std::string name_{};
};

class Derived : public virtual Base {
  private:
    std::string role_{};
    std::string department_{};

  public:
    Derived() = default;
    Derived(uint16_t id, const std::string &name, const std::string &role,
            const std::string &department)
        : Base(id, name), role_(role), department_(department) {
        std::println("Derived constructor implemented!");
    }
    Derived(std::string &role, std::string &department)
        : Base(), role_(role), department_(department) {}
    ~Derived() override { std::println("Derived destructor called!"); };
};

} // namespace constructor
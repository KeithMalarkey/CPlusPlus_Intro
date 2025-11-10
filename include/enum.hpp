#pragma once

/**
 * @brief 展示C-style枚举的缺陷(类型安全等)及C++枚举的特性
 *
 */
#include <cstdint>
namespace enum_demo {
enum class Platte;
enum struct AnimalNum : std::uint16_t;
// enum struct Altitude;
} // namespace enum_demo
void demo_enum();
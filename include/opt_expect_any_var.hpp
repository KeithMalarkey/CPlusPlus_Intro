#pragma once
#include <map>
#include <optional>

template <typename Key, typename Value>
std::optional<Value> getValue(const std::map<Key, Value> &map, const Key &key) {
    auto it = map.find(key);
    if (it == map.end()) {
        return std::nullopt; // return empty optional
    }
    return it->second;
}
void demo_opt_expect_any_var();
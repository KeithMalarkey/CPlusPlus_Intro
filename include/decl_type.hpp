#pragma once

struct A;
void demo_decltype();

template <typename Tp1, typename Tp2>
auto add(Tp1 val1, Tp2 val2) -> decltype(val1 + val2) {
    return val1 + val2;
}
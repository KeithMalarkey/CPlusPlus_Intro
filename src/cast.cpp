#include "cast.hpp"
#include <cmath>
#include <iostream>

void StaticCast() {}

void DynamicCast() {}

void reinterpretCast() {}

void ConstCast() {}

void demo_cast() {
    StaticCast();
    ConstCast();
    DynamicCast();
    reinterpretCast();

    // number to string
    std::float_t c = 10.26718F;
    std::cout << std::to_string(c) << std::endl;
    // string to number
    std::string d = "10.26718", e = "10", f = "10.26718F", g = "7.26718d";
    std::cout << std::stof(d) << std::endl; // string to float
    std::cout << std::stoi(e) << std::endl; // string to int
    std::cout << std::stof(f) << std::endl; // string to float
    std::cout << std::stod(g) << std::endl; // string to double
}
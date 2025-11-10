#pragma once

#include <vector>

template <typename Tp> struct Vec {
  private:
    Tp x_{}, y_{};

  public:
    Vec(Tp x, Tp y) : x_{x}, y_{y} {}
    Tp operator*(Vec<Tp> &a) { return a.x_ * this->x_ + a.y_ * this->y_; }
};

struct Integer;
void demo_operator();
std::vector<int> operator+(const std::vector<int> &, const std::vector<int> &);
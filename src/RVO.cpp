#include "RVO.hpp"
#include <cstdint>

class RVO {
  public:
    std::uint8_t data_{};

  public:
    RVO(std::uint8_t data) : data_{data} {}
    RVO(const RVO &) = default;
    RVO &operator=(const RVO &) = default;
    RVO(RVO &&) = default;
    RVO &operator=(RVO &&) = default;
    ~RVO() {}
};

void demo_rvo() {}
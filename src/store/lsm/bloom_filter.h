#pragma once

#include <bitset>

class BoolFilter {
private:
    // 1024 * 8 * 4 bit == 4KB
    constexpr static std::size_t BOOL_FILETER_SiZE = 1024 * 8 * 4;
    std::bitset<BOOL_FILETER_SiZE> bitset {};

private:
    // hash function take from https://vorbrodt.blog/2019/04/09/multi-hashing
    auto hash(std::string_view element) const -> std::array<std::size_t, 3>;

public:
    void set(std::string_view element);

    bool is_exist(std::string_view element) const;
};

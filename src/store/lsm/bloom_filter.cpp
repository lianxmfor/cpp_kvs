#include <algorithm>
#include <array>
#include <iterator>
#include <random>
#include <string_view>

#include "store/lsm/bloom_filter.h"

using std::array;
using std::generate;
using std::string_view;

auto BoolFilter::hash(std::string_view element) const -> array<std::size_t, 3>
{
    auto key = std::hash<string_view> {}(element);

    std::minstd_rand0 rng(key);

    array<std::size_t, 3> hashes {};
    generate(hashes.begin(), hashes.end(), [&]() -> std::size_t {
        return rng() % BOOL_FILETER_SiZE;
    });

    return hashes;
}

auto BoolFilter::set(std::string_view element) -> void
{
    auto [index1, index2, index3] = hash(element);
    bitset.set(index1);
    bitset.set(index2);
    bitset.set(index3);
}

auto BoolFilter::is_exist(std::string_view element) const -> bool
{
    auto [index1, index2, index3] = hash(element);

    return bitset.test(index1) && bitset.test(index2) && bitset.test(index3);
}

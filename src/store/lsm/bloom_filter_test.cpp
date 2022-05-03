#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

#include "store/lsm/bloom_filter.h"

TEST_CASE("test bloom_filter")
{
    BoolFilter filter;

    for (int i = 0; i < 500; i++) {
        auto key = std::to_string(i);

        REQUIRE(filter.is_exist(key) == false);

        filter.set(key);

        REQUIRE(filter.is_exist(key) == true);
    }
}

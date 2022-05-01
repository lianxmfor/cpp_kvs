#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <optional>
#include <string>
#include <tuple>

#include "error.h"
#include "store/memory/memory.h"
#include "store/store.h"

TEST_CASE("get tests")
{
    std::unique_ptr<Store> s(new MemoryStore);

    auto [v, err] = s->get("k1");
    REQUIRE(v == std::nullopt);
    REQUIRE(err == nullptr);

    REQUIRE(s->set("k1", "v1") == nullptr);

    std::tie(v, err) = s->get("k1");
    REQUIRE(v == "v1");
    REQUIRE(err == nullptr);
}

TEST_CASE("set tests")
{
    std::unique_ptr<Store> s(new MemoryStore);

    REQUIRE(s->set("k1", "v1") == nullptr);

    REQUIRE(s->set("k1", "v2") == nullptr);

    auto [v, err] = s->get("k1");
    REQUIRE(v == "v2");
    REQUIRE(err == nullptr);
}

TEST_CASE("remove tests")
{
    std::unique_ptr<Store> s(new MemoryStore);

    REQUIRE(s->remove("k1") == nullptr);
    REQUIRE(s->set("k1", "v1") == nullptr);

    auto [v, err] = s->get("k1");
    REQUIRE(v == "v1");
    REQUIRE(err == nullptr);

    REQUIRE(s->remove("k1") == nullptr);

    std::tie(v, err) = s->get("k1");
    REQUIRE(v == std::nullopt);
    REQUIRE(err == nullptr);
}

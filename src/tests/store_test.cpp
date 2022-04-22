#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <string>
#include <tuple>

#include "error.h"
#include "store/memory/memory.h"
#include "store/store.h"

TEST_CASE("get tests"
          " [memory] ")
{
    std::unique_ptr<Store> s(new MemoryStore);

    std::string v;
    std::unique_ptr<error> err;

    std::tie(v, err) = s->get("k1");
    REQUIRE(v == "");
    REQUIRE(err == nullptr);

    err = s->set("k1", "v1");
    REQUIRE(err == nullptr);

    std::tie(v, err) = s->get("k1");
    REQUIRE(v == "v1");
    REQUIRE(err == nullptr);
}

TEST_CASE("set tests"
          " [memory] ")
{
    std::unique_ptr<Store> s(new MemoryStore);

    auto err = s->set("k1", "v1");
    REQUIRE(err == nullptr);

    err = s->set("k1", "v2");
    REQUIRE(err == nullptr);

    std::string v;

    std::tie(v, err) = s->get("k1");
    REQUIRE(v == "v2");
    REQUIRE(err == nullptr);
}

TEST_CASE("remove tests"
          " [memory] ")
{
    std::unique_ptr<Store> s(new MemoryStore);

    auto err = s->remove("k1");
    REQUIRE(err == nullptr);

    REQUIRE(s->set("k1", "v1") == nullptr);

    std::string v;
    std::tie(v, err) = s->get("k1");
    REQUIRE(v == "v1");
    REQUIRE(err == nullptr);

    REQUIRE(s->remove("k1") == nullptr);

    std::tie(v, err) = s->get("k1");
    REQUIRE(v == "");
    REQUIRE(err == nullptr);
}

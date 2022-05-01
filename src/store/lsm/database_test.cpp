#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

#include <filesystem>
#include <optional>

#include "error.h"
#include "store/lsm/database.h"
#include "store/lsm/utils.h"
#include "store/store.h"

namespace fs = std::filesystem;

void remove_walfile(fs::path dir)
{
    auto files = file_with_ext(dir, ".wal");
    for (const auto& file : files) {
        std::filesystem::remove(file);
    }
}

TEST_CASE("get tests")
{
    auto dir = fs::temp_directory_path();

    remove_walfile(dir);

    std::unique_ptr<Store> s(new Database(dir));

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
    auto dir = fs::temp_directory_path();
    remove_walfile(dir);

    std::unique_ptr<Store> s(new Database(dir));

    REQUIRE(s->set("k1", "v1") == nullptr);

    REQUIRE(s->set("k1", "v2") == nullptr);

    auto [v, err] = s->get("k1");
    REQUIRE(v == "v2");
    REQUIRE(err == nullptr);
}

TEST_CASE("remove tests")
{
    auto dir = fs::temp_directory_path();
    remove_walfile(dir);

    std::unique_ptr<Store> s(new Database(dir));

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

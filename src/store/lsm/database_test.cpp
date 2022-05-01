#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"


#include <filesystem>

#include "store/lsm/database.h"
#include "store/lsm/utils.h"
#include "store/store.h"
#include "error.h"

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

TEST_CASE("set tests")
{
	auto dir = fs::temp_directory_path();
	remove_walfile(dir);

    std::unique_ptr<Store> s(new Database(dir));

    auto err = s->set("k1", "v1");
    REQUIRE(err == nullptr);

    err = s->set("k1", "v2");
    REQUIRE(err == nullptr);

    std::string v;

    std::tie(v, err) = s->get("k1");
    REQUIRE(v == "v2");
    REQUIRE(err == nullptr);
}

TEST_CASE("remove tests")
{
	auto dir = fs::temp_directory_path();
	remove_walfile(dir);

    std::unique_ptr<Store> s(new Database(dir));

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

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

#include <filesystem>
#include <string>
#include <tuple>

#include "store/lsm/utils.h"
#include "store/lsm/wal.h"
#include "store/lsm/wal_iterator.h"

TEST_CASE("test WAL::load_from_dir")
{
    auto dir = std::filesystem::temp_directory_path();

    auto files = file_with_ext(dir, ".wal");
    for (const auto& file : files) {
        std::filesystem::remove(file);
    }

    WAL wal1(dir);
    wal1.set("k1", "v1", 0);
    wal1.set("k2", "v2", 10);

    WAL wal2(dir);
    wal2.remove("k1", 20);
    wal2.remove("k2", 30);

    wal1.flush();
    wal2.flush();

    auto [wal, memtable] = WAL::load_from_dir(dir);

    SECTION("check memtable")
    {
        vector<WALEntry> want_entries = {
            {
                .key = "k1",
                .value = new string("v1"),
                .timestamp = 0,
                .deleted = false,
            },
            {
                .key = "k2",
                .value = new string("v2"),
                .timestamp = 10,
                .deleted = false,
            },
            {
                .key = "k1",
                .value = nullptr,
                .timestamp = 20,
                .deleted = true,
            },
            {
                .key = "k2",
                .value = nullptr,
                .timestamp = 30,
                .deleted = true,
            },
        };

        vector<WALEntry> get_entries;
        WALEntryScan scan(wal.get_filepath());
        while (scan.has_next()) {
            auto get_entry = scan.next();
            get_entries.push_back(std::move(get_entry));
        }

        REQUIRE(want_entries.size() == get_entries.size());
        for (int i = 0; i < want_entries.size(); i++) {
            auto want_entry = want_entries[i];
            auto get_entry = get_entries[i];

            REQUIRE(want_entry.key == get_entry.key);
            REQUIRE(want_entry.deleted == get_entry.deleted);
            REQUIRE(want_entry.timestamp == get_entry.timestamp);

            if (want_entry.value == nullptr) {
                REQUIRE(get_entry.value == nullptr);
            } else {
                REQUIRE(*want_entry.value == *get_entry.value);
            }
        }
    }

    SECTION("check memtable")
    {
        auto k1 = memtable.get("k1");
        REQUIRE(k1 != nullptr);
        REQUIRE(k1->key == "k1");
        REQUIRE(k1->value == nullptr);
        REQUIRE(k1->timestamp == 20);

        auto k2 = memtable.get("k2");
        REQUIRE(k2 != nullptr);
        REQUIRE(k2->key == "k2");
        REQUIRE(k2->value == nullptr);
        REQUIRE(k2->timestamp == 30);
    }
}

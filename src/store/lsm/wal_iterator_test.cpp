#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <optional>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "store/lsm//wal_iterator.h"
#include "store/lsm/utils.h"
#include "store/lsm/wal.h"

TEST_CASE("test read empty file")
{

    TempFile tempFile("wal");
    const std::filesystem::path path = tempFile.get_filepath();

    WALEntryScan scan(path);

    REQUIRE(scan.has_next() == false);
}

TEST_CASE("test read/write wal entry")
{
    uint64_t timestamp = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now())
                             .time_since_epoch()
                             .count();

    std::vector<WALEntry> entries = {
        {
            .key = "k1",
            .value = "v1",
            .timestamp = timestamp + 1,
            .deleted = false,
        },
        {
            .key = "k2",
            .value = "v2",
            .timestamp = timestamp + 2,
            .deleted = false,
        },
        {
            .key = "k3",
            .value = "v3",
            .timestamp = timestamp + 3,
            .deleted = false,
        },
        {
            .key = "k4",
            .value = "v4",
            .timestamp = timestamp + 4,
            .deleted = false,
        },
        {
            .key = "k1",
            .value = std::nullopt,
            .timestamp = timestamp + 5,
            .deleted = true,
        },
        {
            .key = "k2",
            .value = std::nullopt,
            .timestamp = timestamp + 6,
            .deleted = true,
        },
        {
            .key = "k3",
            .value = std::nullopt,
            .timestamp = timestamp + 7,
            .deleted = true,
        },
        {
            .key = "k1",
            .value = "v11",
            .timestamp = timestamp + 8,
            .deleted = false,
        },
        {
            .key = "k2",
            .value = "v222",
            .timestamp = timestamp + 9,
            .deleted = false,
        },
        {
            .key = "k4",
            .value = "v44",
            .timestamp = timestamp + 10,
            .deleted = false,
        },
    };

    TempFile tempFile("wal");
    const std::filesystem::path path = tempFile.get_filepath();

    WAL wal(path, std::fstream(path, std::ios::binary | std::ios::app));
    for (const auto& entry : entries) {
        if (!entry.deleted) {
            wal.set(entry.key, *entry.value, entry.timestamp);
        } else {
            wal.remove(entry.key, entry.timestamp);
        }
    }
    wal.flush();

    vector<WALEntry> get_entries;
    WALEntryScan scan(path);
    while (scan.has_next()) {
        auto get_entry = scan.next();
        get_entries.push_back(std::move(get_entry));
    }

    REQUIRE(entries.size() == get_entries.size());
    for (int i = 0; i < entries.size(); i++) {
        auto want_entry = entries[i];
        auto get_entry = get_entries[i];

        REQUIRE(want_entry.key == get_entry.key);
        REQUIRE(want_entry.deleted == get_entry.deleted);
        REQUIRE(want_entry.timestamp == get_entry.timestamp);
        REQUIRE(want_entry.value == get_entry.value);
    }
}

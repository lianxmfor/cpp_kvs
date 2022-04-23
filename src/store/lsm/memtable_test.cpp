#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

#include <iterator>
#include <string>
#include <tuple>
#include <vector>

#include "store/lsm/memtable.h"

TEST_CASE("test get_index")
{
    unique_ptr<MemTable> table(new MemTable);

    struct TestCase {
        string msg;
        vector<MemTableEntry> entries;
        string key;

        int want_index;
        bool want_if_exist;
    };

    vector<MemTableEntry> entries = {
        { "1", nullptr, false, 0 },
        { "2", nullptr, false, 0 },
        { "3", nullptr, false, 0 },
        { "5", nullptr, false, 0 },
        { "7", nullptr, false, 0 },
    };

    vector<TestCase> cases = {
        {
            .msg = "case1",
            .entries = entries,
            .key = "0",
            .want_index = 0,
            .want_if_exist = false,
        },
        {
            .msg = "case2",
            .entries = entries,
            .key = "1",
            .want_index = 0,
            .want_if_exist = true,
        },
        {
            .msg = "case3",
            .entries = entries,
            .key = "4",
            .want_index = 3,
            .want_if_exist = false,
        },
        {
            .msg = "case4",
            .entries = entries,
            .key = "5",
            .want_index = 3,
            .want_if_exist = true,
        },
        {
            .msg = "case5",
            .entries = entries,
            .key = "6",
            .want_index = 4,
            .want_if_exist = false,
        },
        {
            .msg = "case6",
            .entries = entries,
            .key = "8",
            .want_index = 5,
            .want_if_exist = false,
        },
    };

    for (const auto& c : cases) {
        SECTION(c.msg)
        {
            table->entries = entries;
            vector<MemTableEntry>::iterator iter;
            bool got_if_exist;

            std::tie(iter, got_if_exist) = table->get_index(c.key);

            REQUIRE(c.want_index == std::distance(table->entries.begin(), iter));
            REQUIRE(c.want_if_exist == got_if_exist);
        }
    }
}

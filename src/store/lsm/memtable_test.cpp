#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

#include <memory>
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
            auto [iter, got_if_exist] = table->get_index(c.key);

            REQUIRE(c.want_index == std::distance(table->entries.begin(), iter));
            REQUIRE(c.want_if_exist == got_if_exist);
        }
    }
}

TEST_CASE("MemTable put start")
{
    unique_ptr<MemTable> table(new MemTable());

    table->set("Lime", "Lime Smoothie", 0); // 17 + 16 + 1
    table->set("Orange", "Orange Smoothie", 10); // 21 + 16 + 1
    table->set("Apple", "Apple Smoothie", 20); // 19 + 16 + 1

    REQUIRE(table->entries[0].key == "Apple");
    REQUIRE(*table->entries[0].value == "Apple Smoothie");
    REQUIRE(table->entries[0].timestamp == 20);
    REQUIRE(table->entries[0].deleted == false);

    REQUIRE(table->entries[1].key == "Lime");
    REQUIRE(*table->entries[1].value == "Lime Smoothie");
    REQUIRE(table->entries[1].timestamp == 0);
    REQUIRE(table->entries[1].deleted == false);

    REQUIRE(table->entries[2].key == "Orange");
    REQUIRE(*table->entries[2].value == "Orange Smoothie");
    REQUIRE(table->entries[2].timestamp == 10);
    REQUIRE(table->entries[2].deleted == false);

    REQUIRE(table->len() == 108);
}

TEST_CASE("MemTable put middle")
{
    unique_ptr<MemTable> table(new MemTable());

    table->set("Apple", "Apple Smoothie", 0); // 19 + 16 + 1
    table->set("Orange", "Orange Smoothie", 10); // 21 + 16 + 1
    table->set("Lime", "Lime Smoothie", 20); // 17 + 16 + 1

    REQUIRE(table->entries[0].key == "Apple");
    REQUIRE(*table->entries[0].value == "Apple Smoothie");
    REQUIRE(table->entries[0].timestamp == 0);
    REQUIRE(table->entries[0].deleted == false);

    REQUIRE(table->entries[1].key == "Lime");
    REQUIRE(*table->entries[1].value == "Lime Smoothie");
    REQUIRE(table->entries[1].timestamp == 20);
    REQUIRE(table->entries[1].deleted == false);

    REQUIRE(table->entries[2].key == "Orange");
    REQUIRE(*table->entries[2].value == "Orange Smoothie");
    REQUIRE(table->entries[2].timestamp == 10);
    REQUIRE(table->entries[2].deleted == false);

    REQUIRE(table->len() == 108);
}

TEST_CASE("MemTable put end")
{
    unique_ptr<MemTable> table(new MemTable());

    table->set("Apple", "Apple Smoothie", 0); // 19 + 16 + 1
    table->set("Lime", "Lime Smoothie", 10); // 17 + 16 + 1
    table->set("Orange", "Orange Smoothie", 20); // 21 + 16 + 1

    REQUIRE(table->entries[0].key == "Apple");
    REQUIRE(*table->entries[0].value == "Apple Smoothie");
    REQUIRE(table->entries[0].timestamp == 0);
    REQUIRE(table->entries[0].deleted == false);

    REQUIRE(table->entries[1].key == "Lime");
    REQUIRE(*table->entries[1].value == "Lime Smoothie");
    REQUIRE(table->entries[1].timestamp == 10);
    REQUIRE(table->entries[1].deleted == false);

    REQUIRE(table->entries[2].key == "Orange");
    REQUIRE(*table->entries[2].value == "Orange Smoothie");
    REQUIRE(table->entries[2].timestamp == 20);
    REQUIRE(table->entries[2].deleted == false);

    REQUIRE(table->len() == 108);
}

TEST_CASE("MemTable get exist")
{
    unique_ptr<MemTable> table(new MemTable);

    table->set("Apple", "Apple Smoothie", 0); // 19 + 16 + 1
    table->set("Lime", "Lime Smoothie", 10); // 17 + 16 + 1
    table->set("Orange", "Orange Smoothie", 20); // 21 + 16 + 1

    auto entry = table->get("Lime");
    REQUIRE(entry != nullptr);
    REQUIRE(entry->key == "Lime");
    REQUIRE(entry->value != nullptr);
    REQUIRE(*entry->value == "Lime Smoothie");
    REQUIRE(entry->deleted == false);
    REQUIRE(entry->timestamp == 10);

    table->remove("Lime", 30);
    entry = table->get("Lime");
    REQUIRE(entry != nullptr);
    REQUIRE(entry->key == "Lime");
    REQUIRE(entry->value == nullptr);
    REQUIRE(entry->deleted == true);
    REQUIRE(entry->timestamp == 30);
}

TEST_CASE("MemTable get not exist")
{
    unique_ptr<MemTable> table(new MemTable);

    auto entry = table->get("Lime");
    REQUIRE(entry == nullptr);
}

TEST_CASE("MemTable remove")
{
    unique_ptr<MemTable> table(new MemTable);

    table->set("Orange", "Orange Smoothie", 0);
    table->remove("Orange", 10);

    auto entry = table->get("Orange");
    REQUIRE(entry != nullptr);
    REQUIRE(entry->key == "Orange");
    REQUIRE(entry->value == nullptr);
    REQUIRE(entry->deleted == true);
    REQUIRE(entry->timestamp == 10);
}

TEST_CASE("MemTable remove empty")
{
    unique_ptr<MemTable> table(new MemTable);

    table->remove("Orange", 10);

    auto entry = table->get("Orange");
    REQUIRE(entry != nullptr);
    REQUIRE(entry->key == "Orange");
    REQUIRE(entry->value == nullptr);
    REQUIRE(entry->deleted == true);
    REQUIRE(entry->timestamp == 10);

    REQUIRE(table->entries[0].key == "Orange");
    REQUIRE(table->entries[0].value == nullptr);
    REQUIRE(table->entries[0].deleted == true);
    REQUIRE(table->entries[0].timestamp == 10);
}

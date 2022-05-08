#pragma once

// MemTable entry
#include <cstdint>
#include <iterator>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

using std::optional;
using std::string;
using std::tuple;
using std::uint64_t;
using std::unique_ptr;
using std::vector;

class MemTableEntry {
public:
    MemTableEntry(const string& key,
        optional<string> value,
        bool deleted,
        uint64_t timestamp) noexcept;

    MemTableEntry(const MemTableEntry& other) noexcept;
    MemTableEntry(MemTableEntry&& other) noexcept;
    MemTableEntry& operator=(const MemTableEntry& other) noexcept;
    MemTableEntry& operator=(MemTableEntry&& other) noexcept;

    ~MemTableEntry() = default;

public:
    string key;
    optional<string> value {};
    bool deleted {};
    uint64_t timestamp {};
};

// MemTable holds a sorted vector of the latest written records.
class MemTable {
public:
    vector<MemTableEntry> entries;
    unsigned int usize;

public:
    MemTable();
    ~MemTable();

    // Performs Binary Search to find a record in the MemTable.
    // If the record is found `[iter, true]` is returned, iter is the index of record.
    // If the record is not found `[iter, false]` is returned. iter is the index to
    // insert at the record at.
    auto get_index(const string& key)
        -> tuple<vector<MemTableEntry>::iterator, bool>;

    auto len() const
        -> unsigned int;

    auto is_empty() const
        -> bool;

public:
    // Get the MemTableEntry value of a given string key.
    // If key not exist, returned `[nullptr]`
    auto get(const string& key)
        -> unique_ptr<MemTableEntry>;

    // Set the value of a string key to a string.
    // If the key already exists, the previous value will be overwritten.
    auto set(const string& key, const string& value, uint64_t timestamp)
        -> void;

    // Removes a given key;
    auto remove(const string& key, uint64_t timestamp)
        -> void;
};

#pragma once

// MemTable entry
#include <cstdint>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

using std::string;
using std::tuple;
using std::uint64_t;
using std::unique_ptr;
using std::vector;

class MemTableEntry {
public:
    MemTableEntry(const string& key,
        string* value,
        bool deleted,
        uint64_t timestamp);

    ~MemTableEntry();

public:
    string key;
    string* value;
    bool deleted;
    uint64_t timestamp;
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
    auto get_index(const string& key) -> std::tuple<decltype(entries)::iterator, bool>;

    unsigned int len();

    bool is_empty();

public:
    // Get the MemTableEntry value of a given string key.
    // If key not exist, returned `[nullptr]`
    unique_ptr<MemTableEntry> get(const string& key);

    // Set the value of a string key to a string.
    // If the key already exists, the previous value will be overwritten.
    void set(const string& key, const string& value, uint64_t timestamp);

    // Removes a given key;
    void remove(const string& key, uint64_t timestamp);
};

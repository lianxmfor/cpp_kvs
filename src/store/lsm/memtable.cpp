#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "store/lsm/memtable.h"

MemTableEntry::MemTableEntry(const string& key,
    string* value,
    bool deleted,
    uint64_t timestamp)
{
    this->key = key;
    this->deleted = deleted;
    this->timestamp = timestamp;

    if (value != nullptr) {
        this->value = new string( *value );
    } else {
        this->value = nullptr;
    }
}

MemTableEntry::~MemTableEntry()
{
    if (this->value != nullptr) {
        delete this->value;
        this->value = nullptr;
    }
}

MemTable::MemTable()
{
    entries = {};
    usize = 0;
}

MemTable::~MemTable()
{
    entries.clear();
}

unsigned int MemTable::len()
{
    return usize;
}

bool MemTable::is_empty()
{
    return usize == 0;
}

auto MemTable::get_index(const string& key) -> tuple<decltype(entries)::iterator, bool>
{
    auto iter = std::lower_bound(entries.begin(), entries.end(), key,
        [](const MemTableEntry& entry, const string& key) {
            return entry.key < key;
        });

    return std::make_tuple(iter, iter != entries.end() && iter->key == key);
}

unique_ptr<MemTableEntry> MemTable::get(const string& key)
{
    decltype(entries)::iterator iter;
    bool exist;

    std::tie(iter, exist) = this->get_index(key);
    if (!exist) {
        return nullptr;
    }

    return unique_ptr<MemTableEntry>(new MemTableEntry(
        iter->key,
        iter->value,
        iter->deleted,
        iter->timestamp));
}

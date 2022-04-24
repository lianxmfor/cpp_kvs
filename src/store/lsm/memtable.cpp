#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "store/lsm/memtable.h"

MemTableEntry::MemTableEntry(const string& key,
    string* value,
    bool deleted,
    uint64_t timestamp) noexcept
{
    this->key = key;
    this->deleted = deleted;
    this->timestamp = timestamp;

    if (value != nullptr) {
        this->value = new string(*value);
    } else {
        this->value = nullptr;
    }
}

MemTableEntry::MemTableEntry(const MemTableEntry& other) noexcept
{
    this->key = other.key;
    this->deleted = other.deleted;
    this->timestamp = other.timestamp;

    if (other.value != nullptr) {
        this->value = new string(*other.value);
    } else {
        this->value = nullptr;
    }
}

MemTableEntry::MemTableEntry(MemTableEntry&& other) noexcept
{
    this->key = other.key;
    this->deleted = other.deleted;
    this->timestamp = other.timestamp;
    this->value = other.value;

    other.value = nullptr;
}

MemTableEntry& MemTableEntry::operator=(const MemTableEntry& other) noexcept
{
    this->key = other.key;
    this->deleted = other.deleted;
    this->timestamp = other.timestamp;

    if (other.value != nullptr) {
        this->value = new string(*other.value);
    } else {
        this->value = nullptr;
    }
    return *this;
}

MemTableEntry& MemTableEntry::operator=(MemTableEntry&& other) noexcept
{
    this->key = other.key;
    this->deleted = other.deleted;
    this->timestamp = other.timestamp;
    this->value = other.value;

    other.value = nullptr;

    return *this;
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
    usize = 0;
}

unsigned int MemTable::len() const
{
    return usize;
}

bool MemTable::is_empty() const
{
    return usize == 0;
}

auto MemTable::get_index(const string& key) -> tuple<vector<MemTableEntry>::iterator, bool>
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

void MemTable::set(const string& key, const string& value, uint64_t timestamp)
{
    MemTableEntry entry = { key, (string*)&value, false, timestamp };

    vector<MemTableEntry>::iterator iter;
    bool exist;

    std::tie(iter, exist) = this->get_index(key);
    if (exist) {
        if (iter->value != nullptr) {
            usize += value.size();
            usize -= iter->value->size();
        } else {
            usize += value.size();
        }
        *iter = std::move(entry);
    } else {
        entries.insert(iter, std::move(entry));
        // Increase the size of the MemTable by the key size, value size, timestamp size(16 byte)
        // deleted size(1 byte).
        usize += key.size() + value.size() + 16 + 1;
    }
}

void MemTable::remove(const string& key, uint64_t timestamp)
{
    MemTableEntry entry = { key, nullptr, true, timestamp };

    vector<MemTableEntry>::iterator iter;
    bool exist;

    std::tie(iter, exist) = this->get_index(key);
    if (exist) {
        if (iter->value != nullptr) {
            usize -= iter->value->size();
        }
        *iter = std::move(entry);
    } else {
        usize += key.size() + 16 + 1;
        entries.insert(iter, std::move(entry));
    }
}

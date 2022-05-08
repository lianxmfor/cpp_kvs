#include <algorithm>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "store/lsm/memtable.h"

MemTableEntry::MemTableEntry(const string& key,
    optional<string> value,
    bool deleted,
    uint64_t timestamp) noexcept
{
    this->key = key;
    this->deleted = deleted;
    this->timestamp = timestamp;
    this->value = value;
}

MemTableEntry::MemTableEntry(const MemTableEntry& other) noexcept
{
    this->key = other.key;
    this->deleted = other.deleted;
    this->timestamp = other.timestamp;
    this->value = other.value;
}

MemTableEntry::MemTableEntry(MemTableEntry&& other) noexcept
{
    this->key = other.key;
    this->deleted = other.deleted;
    this->timestamp = other.timestamp;
    this->value = other.value;

    other.value = std::nullopt;
}

MemTableEntry& MemTableEntry::operator=(const MemTableEntry& other) noexcept
{
    this->key = other.key;
    this->deleted = other.deleted;
    this->timestamp = other.timestamp;
    this->value = other.value;

    return *this;
}

MemTableEntry& MemTableEntry::operator=(MemTableEntry&& other) noexcept
{
    this->key = other.key;
    this->deleted = other.deleted;
    this->timestamp = other.timestamp;
    this->value = other.value;

    other.value = std::nullopt;

    return *this;
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

auto MemTable::len() const -> unsigned int
{
    return usize;
}

auto MemTable::is_empty() const -> bool
{
    return usize == 0;
}

auto MemTable::get_index(const string& key)
    -> tuple<vector<MemTableEntry>::iterator, bool>
{
    auto iter = std::lower_bound(entries.begin(), entries.end(), key,
        [](const MemTableEntry& entry, const string& key) {
            return entry.key < key;
        });

    return std::make_tuple(iter, iter != entries.end() && iter->key == key);
}

auto MemTable::get(const string& key)
    -> unique_ptr<MemTableEntry>
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

auto MemTable::set(const string& key, const string& value, uint64_t timestamp) -> void
{
    MemTableEntry entry = { key, value, false, timestamp };

    auto [iter, exist] = this->get_index(key);
    if (exist) {
        if (iter->value != std::nullopt) {
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

auto MemTable::remove(const string& key, uint64_t timestamp) -> void
{
    MemTableEntry entry = { key, std::nullopt, true, timestamp };

    auto [iter, exist] = this->get_index(key);
    if (exist) {
        if (iter->value != std::nullopt) {
            usize -= iter->value->size();
        }
        *iter = std::move(entry);
    } else {
        usize += key.size() + 16 + 1;
        entries.insert(iter, std::move(entry));
    }
}

#include <algorithm>
#include <iterator>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "store/lsm/memtable.h"

MemTableEntry::MemTableEntry(string key,
    string* value,
    bool deleted,
    uint64_t timestamp)
{
    this->key = key;
    this->deleted = deleted;
    this->timestamp = timestamp;
    if (value != nullptr) {
        this->value = new string{ *value };
    }
}

MemTableEntry::~MemTableEntry()
{
    if (this->value != nullptr) {
        delete this->value;
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


#include <cstddef>
#include <cstdio>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <utility>

#include "store/lsm/wal.h"
#include "store/lsm/wal_iterator.h"

using std::ifstream;

WALEntryScan::WALEntryScan(const string& filepath)
{
    this->filepath = filepath;
    this->file = ifstream(filepath, std::ios::binary);
    this->file.seekg(0);
}

WALEntryScan::~WALEntryScan()
{
    file.close();
}

bool WALEntryScan::has_next()
{
    return file.peek() != EOF;
}

WALEntry WALEntryScan::next()
{
    WALEntry entry {};

    size_t key_size {};
    file.read(reinterpret_cast<char*>(&key_size), 4);
    entry.key.resize(key_size);

    file.read(reinterpret_cast<char*>(&entry.deleted), 1);

    if (!entry.deleted) {
        size_t value_size {};
        file.read(reinterpret_cast<char*>(&value_size), 4);
        entry.value = new string;
        entry.value->resize(value_size);

        file.read(entry.key.data(), key_size);
        file.read(entry.value->data(), value_size);
    } else {
        file.read(entry.key.data(), key_size);
    }

    file.read(reinterpret_cast<char*>(&entry.timestamp), 8);

    return std::move(entry);
}

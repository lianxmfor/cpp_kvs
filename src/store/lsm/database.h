#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

#include "store/lsm/memtable.h"
#include "store/lsm/wal.h"
#include "store/store.h"

using std::string;
using std::uint64_t;

namespace fs = std::filesystem;

struct DatabaseEntry {
    string key;
    string value;
    uint64_t timestamp;
};

class Database : public Store {
private:
    fs::path dir;
    MemTable memtable;
    WAL wal;

public:
    explicit Database(fs::path dir);

    fs::path get_dir() const;

public:
    std::unique_ptr<error> set(const std::string& key, const std::string& value) override;

    std::tuple<std::string, std::unique_ptr<error>> get(const std::string& key) override;

    std::unique_ptr<error> remove(const std::string& key) override;
};

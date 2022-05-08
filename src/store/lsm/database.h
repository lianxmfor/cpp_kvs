#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>

#include "store/lsm/memtable.h"
#include "store/lsm/wal.h"
#include "store/store.h"

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
    auto set(const std::string& key, const std::string& value)
        -> std::unique_ptr<error> override;

    auto get(const std::string& key)
        -> std::tuple<std::optional<std::string>, std::unique_ptr<error>> override;

    auto remove(const std::string& key)
        -> std::unique_ptr<error> override;
};

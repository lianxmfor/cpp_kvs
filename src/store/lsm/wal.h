#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <istream>
#include <optional>
#include <string>
#include <tuple>

#include "store/lsm/memtable.h"

using std::fstream;
using std::optional;
using std::string;
using std::tuple;
using std::uint64_t;

struct WALEntry {
    string key;
    optional<string> value;
    uint64_t timestamp;
    bool deleted;
};

class WAL {
private:
    std::filesystem::path filepath;
    fstream file;

public:
    WAL() = default;

    explicit WAL(const std::filesystem::path& dir);

    WAL(const std::filesystem::path& filepath, std::fstream&& file);

    WAL(WAL&& other);

    WAL& operator=(WAL&& other);

    ~WAL();

public:
    static auto load_from_dir(const std::filesystem::path& dir)
        -> tuple<WAL, MemTable>;

    auto get_filepath() const
        -> std::filesystem::path;

public:
    auto set(const string& key, const string& value, uint64_t timestamp)
        -> void;

    auto remove(const string& key, uint64_t timestamp)
        -> void;

    auto flush()
        -> void;
};

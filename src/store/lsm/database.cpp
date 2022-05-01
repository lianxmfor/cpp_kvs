#include <chrono>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <tuple>

#include "error.h"
#include "store/lsm/database.h"
#include "store/lsm/memtable.h"

using std::nullopt;
using std::string;
using std::tuple;
using std::uint64_t;
using std::unique_ptr;

namespace fs = std::filesystem;

Database::Database(fs::path dir)
{
    auto [wal, memtable] = WAL::load_from_dir(dir);

    this->dir = dir;
    this->wal = std::move(wal);
    this->memtable = memtable;
}

fs::path Database::get_dir() const
{
    return dir;
}

auto Database::get(const string& key) -> tuple<std::optional<std::string>, unique_ptr<error>>
{
    auto entry = memtable.get(key);

    if (entry == nullptr || entry->value == std::nullopt) {
        return std::make_tuple(nullopt, nullptr);
    }

    return std::make_tuple(*entry->value, nullptr);
}

auto Database::set(const std::string& key, const std::string& value) -> std::unique_ptr<error>
{
    uint64_t timestamp = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now())
                             .time_since_epoch()
                             .count();

    wal.set(key, value, timestamp);
    memtable.set(key, value, timestamp);

    wal.flush();

    return nullptr;
}

auto Database::remove(const std::string& key) -> std::unique_ptr<error>
{
    uint64_t timestamp = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now())
                             .time_since_epoch()
                             .count();

    wal.remove(key, timestamp);
    memtable.remove(key, timestamp);

    wal.flush();

    return nullptr;
}

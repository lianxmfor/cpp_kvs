#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>

#include "store/lsm/memtable.h"
#include "store/lsm/utils.h"
#include "store/lsm/wal.h"
#include "store/lsm/wal_iterator.h"

WAL::WAL(const std::filesystem::path& dir)
{
    uint64_t timestamp = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now())
                             .time_since_epoch()
                             .count();

    this->filepath = dir / (std::to_string(timestamp) + ".wal");

    this->file = fstream(this->filepath, std::ios::binary | std::ios::app);
}

WAL::WAL(const std::filesystem::path& filepath, std::fstream&& file)
{
    this->filepath = filepath;
    this->file = std::move(file);
}

WAL::WAL(WAL&& other)
{
    this->filepath = other.filepath;
    this->file = std::move(other.file);
}

WAL& WAL::operator=(WAL&& other)
{
    this->filepath = other.filepath;
    this->file = std::move(other.file);
    return *this;
}

WAL::~WAL()
{
    this->file.close();
}

auto WAL::load_from_dir(const std::filesystem::path& dir) -> tuple<WAL, MemTable>
{
    auto files = file_with_ext(dir, ".wal");
    std::sort(files.begin(), files.end());

    WAL new_wal(dir);
    MemTable new_memtable;

    for (const auto& file : files) {
        WALEntryScan scan(file);

        while (scan.has_next()) {
            WALEntry entry = scan.next();

            if (!entry.deleted) {
                new_memtable.set(entry.key, *entry.value, entry.timestamp);
                new_wal.set(entry.key, *entry.value, entry.timestamp);
            } else {
                new_memtable.remove(entry.key, entry.timestamp);
                new_wal.remove(entry.key, entry.timestamp);
            }
        }
    }

    for (const auto& file : files) {
        std::filesystem::remove(file);
    }
    new_wal.flush();

    return std::make_tuple(std::move(new_wal), std::move(new_memtable));
}

auto WAL::get_filepath() const -> std::filesystem::path
{
    return filepath;
}

auto WAL::set(const string& key, const string& value, uint64_t timestamp) -> void
{
    size_t key_size = key.size();
    file.write(reinterpret_cast<const char*>(&key_size), 4);

    bool deleted = false;
    file.write(reinterpret_cast<const char*>(&deleted), 1);

    size_t value_size = value.size();
    file.write(reinterpret_cast<const char*>(&value_size), 4);

    file.write(key.data(), key_size);
    file.write(value.data(), value_size);

    file.write(reinterpret_cast<const char*>(&timestamp), 8);
}

auto WAL::remove(const string& key, uint64_t timestamp) -> void
{
    size_t key_size = key.size();
    file.write(reinterpret_cast<const char*>(&key_size), 4);

    bool deleted = true;
    file.write(reinterpret_cast<const char*>(&deleted), 1);

    file.write(key.data(), key_size);

    file.write(reinterpret_cast<const char*>(&timestamp), 8);
}

auto WAL::flush() -> void
{
    file.flush();
}

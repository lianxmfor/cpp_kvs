#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "error.h"
#include "memory.h"

using std::optional;
using std::string;
using std::tuple;
using std::unique_ptr;

auto MemoryStore::set(const string& key, const string& value) -> unique_ptr<error>
{
    kvs[key] = value;
    return nullptr;
}

auto MemoryStore::get(const string& key) -> tuple<optional<string>, unique_ptr<error>>
{

    auto value = kvs.find(key);
    if (value != kvs.end()) {
        return make_tuple(value->second, nullptr);
    }
    return make_tuple(std::nullopt, nullptr);
}

auto MemoryStore::remove(const string& key) -> unique_ptr<error>
{
    kvs.erase(key);
    return nullptr;
}

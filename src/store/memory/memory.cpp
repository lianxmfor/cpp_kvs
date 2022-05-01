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

unique_ptr<error> MemoryStore::set(const string& key, const string& value)
{
    kvs[key] = value;
    return nullptr;
}

tuple<optional<string>, unique_ptr<error>> MemoryStore::get(const string& key)
{

    auto value = kvs.find(key);
    if (value != kvs.end()) {
        return make_tuple(value->second, nullptr);
    }
    return make_tuple(std::nullopt, nullptr);
}

unique_ptr<error> MemoryStore::remove(const string& key)
{
    kvs.erase(key);
    return nullptr;
}

#include <iostream>
#include <vector>

#include "error.h"
#include "memory.h"

std::unique_ptr<error> MemoryStore::set(const std::string& key, const std::string& value)
{
    kvs[key] = value;
    return nullptr;
}

std::tuple<std::string, std::unique_ptr<error>> MemoryStore::get(const std::string& key)
{

    auto value = kvs.find(key);
    if (value != kvs.end()) {
        return std::make_tuple(value->second, nullptr);
    }
    return std::make_tuple("", nullptr);
}

std::unique_ptr<error> MemoryStore::remove(const std::string& key)
{
    kvs.erase(key);
    return nullptr;
}

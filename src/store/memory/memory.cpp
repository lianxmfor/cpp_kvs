#include <iostream>
#include <vector>

#include "error.h"
#include "memory.h"

using std::vector;
using std::string;


unique_ptr<error> MemoryStore::set(const string key, const string value)
{
    kvs[key] = value;
    return nullptr;
}

tuple<string, unique_ptr<error>> MemoryStore::get(const string key)
{

    auto value = kvs.find(key);
    if ( value != kvs.end() ) {
        return std::make_tuple(value->second, nullptr);
    } 
    return std::make_tuple("", nullptr);
}

unique_ptr<error> MemoryStore::remove(const string key)
{
    kvs.erase(key);
    return nullptr;
}

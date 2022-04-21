#include <iterator>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "error.h"
#include "memory.h"

using std::vector;


unique_ptr<error> MemoryStore::set(vector<char> key, vector<char> value)
{
    string s_key{ key.begin(), key.end() };
    kvs.insert({s_key, value});
    return nullptr;
}

tuple<vector<char>, unique_ptr<error>> MemoryStore::get(vector<char> key)
{
    string s_key { key.begin(), key.end() }; 

    auto value = kvs.find(s_key);
    if ( value != kvs.end() ) {
        return std::make_tuple(value->second, nullptr);
    } 
    return std::make_tuple(vector<char>{}, nullptr);
}

unique_ptr<error> MemoryStore::remove(vector<char> key)
{
    string s_key { key.begin(), key.end() };
    kvs.erase(s_key);

    return nullptr;
}

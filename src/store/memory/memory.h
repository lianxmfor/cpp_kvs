#pragma once

#include <memory>
#include <string>
#include <tuple>
#include <vector>
#include <unordered_map>

#include "error.h"
#include "store/store.h"

using std::unordered_map;
using std::vector;
using std::string;

class MemoryStore : public Store
{
private:
    unordered_map<string, vector<char>>  kvs;

public:
    virtual unique_ptr<error> set(vector<char> key, vector<char> value) override;

    virtual tuple<vector<char>, unique_ptr<error>> get(vector<char> key) override;

    virtual unique_ptr<error> remove(vector<char> key) override;
};

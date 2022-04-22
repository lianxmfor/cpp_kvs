#pragma once

#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>

#include "error.h"
#include "store/store.h"

using std::string;
using std::unique_ptr;
using std::unordered_map;

class MemoryStore : public Store {
private:
    unordered_map<string, string> kvs{};

public:
    virtual unique_ptr<error> set(const string key, const string value) override;

    virtual tuple<string, unique_ptr<error>> get(const string key) override;

    virtual unique_ptr<error> remove(const string key) override;
};

#pragma once

#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <unordered_map>

#include "error.h"
#include "store/store.h"

class MemoryStore : public Store {
private:
    std::unordered_map<std::string, std::string> kvs {};

public:
    auto set(const std::string& key, const std::string& value)
        -> std::unique_ptr<error> override;

    auto get(const std::string& key)
        -> std::tuple<std::optional<std::string>, std::unique_ptr<error>> override;

    auto remove(const std::string& key)
        -> std::unique_ptr<error> override;
};

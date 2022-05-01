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
    std::unique_ptr<error> set(const std::string& key, const std::string& value) override;

    std::tuple<std::optional<std::string>, std::unique_ptr<error>> get(const std::string& key) override;

    std::unique_ptr<error> remove(const std::string& key) override;
};

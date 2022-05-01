#pragma once

#include <memory>
#include <optional>
#include <string>
#include <tuple>

#include "error.h"

// Store is an interface for description store actions.
class Store {
public:
    // Sets the value of a vector<char> key to a vector<char>.
    // If the key already exists, the previous value will be overwritten.
    virtual std::unique_ptr<error> set(const std::string& key, const std::string& value) = 0;

    // Gets the vector<char> value of a given vector<char> key.
    // Return empty vector if given key does not exist.
    virtual std::tuple<std::optional<std::string>, std::unique_ptr<error>> get(const std::string& key) = 0;

    // Remove a given key.
    virtual std::unique_ptr<error> remove(const std::string& key) = 0;

public:
    virtual ~Store() = default;
};

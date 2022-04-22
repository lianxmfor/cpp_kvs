#pragma once

#include <memory>
#include <string>
#include <tuple>

#include "error.h"

using std::string;
using std::tuple;
using std::unique_ptr;

// Store is an interface for description store actions.
class Store {
public:
    // Sets the value of a vector<char> key to a vector<char>.
    // If the key already exists, the previous value will be overwritten.
    virtual unique_ptr<error> set(const string key, const string value) = 0;

    // Gets the vector<char> value of a given vector<char> key.
    //
    // Return empty vector if given key does not exist.
    virtual tuple<string, unique_ptr<error>> get(const string key) = 0;

    // Remove a given key.
    virtual unique_ptr<error> remove(const string key) = 0;

public:
    virtual ~Store(){};
};

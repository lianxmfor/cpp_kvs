#pragma once

#include <tuple>
#include <vector>
#include <memory>

#include "error.h"

using std::vector;
using std::tuple;
using std::unique_ptr;

// Store is an interface for description store actions.
class Store
{
public:
    // Sets the value of a vector<char> key to a vector<char>.
    // If the key already exists, the previous value will be overwritten.
    virtual unique_ptr<error> set(vector<char> key, vector<char> value) = 0;

    // Gets the vector<char> value of a given vector<char> key.
    //
    // Return empty vector if given key does not exist.
    virtual tuple<vector<char>, unique_ptr<error>> get(vector<char> key) = 0;

    // Remove a given key.
    virtual unique_ptr<error> remove(vector<char> key) = 0;
};

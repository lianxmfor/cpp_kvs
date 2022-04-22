#pragma once

#include <string>

class error {
public:
    virtual std::string Error() = 0;

public:
    virtual ~error(){};
};

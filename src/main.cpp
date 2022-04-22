#include <iostream>
#include <memory>
#include <string>
#include <tuple>

#include "store/memory/memory.h"
#include "store/store.h"

int main()
{
    std::unique_ptr<Store> s(new MemoryStore);

    s->set("name", "lianxm");

    std::string value;
    std::unique_ptr<error> err;

    std::tie(value, err) = s->get("name");
    if (err != nullptr) {
        std::cout << "get value failed: " << err->Error() << std::endl;
    } else {
        std::cout << value << std::endl;
    }
}

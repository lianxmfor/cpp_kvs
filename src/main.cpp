#include <iostream>
#include <memory>
#include <ostream>
#include <tuple>

#include "store/memory/memory.h"
#include "store/store.h"

int main()
{
    std::unique_ptr<Store> s(new MemoryStore);

    s->set("name", "lianxm");

    auto [value, err] = s->get("name");
    if (err != nullptr) {
        std::cout << "get value failed: " << err->Error() << std::endl;
    } else {
        std::cout << value.value_or("not value") << std::endl;
    }
}

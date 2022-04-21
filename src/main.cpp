#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>

#include "store/store.h"
#include "store/memory/memory.h"

using namespace std;

int main()
{
    unique_ptr<Store> s(new MemoryStore);

    s->set("name", "lianxm");

    string value;
    unique_ptr<error> err;

    std::tie(value, err) = s->get("name");
    if (err != nullptr) {
        cout << "get value failed: " << err->Error() << endl;
    } else {
        cout << value << endl;
    }
}

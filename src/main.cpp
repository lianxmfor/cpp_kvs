#include <iostream>
#include <memory>
#include <tuple>
#include <type_traits>
#include <vector>

#include "error.h"
#include "store/store.h"
#include "store/memory/memory.h"

using namespace std;

int main()
{
    Store* s = new MemoryStore();

    s->set(vector<char>{'n'}, vector<char>{'l', 'i', 'a', 'n'});

    vector<char> value;
    unique_ptr<error> err;

    std::tie(value, err) = s->get(vector<char>{'n'});
    if (err != nullptr)
    {
        cout << "get value failed: " << err->Error() << endl;
    }
    else
    {
        for (const auto& v : value)
        {
            cout << v;
        }
        cout << endl;
    }
}

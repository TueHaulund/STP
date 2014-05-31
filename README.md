Sequence Transformation Pipeline
===

stplib is an experimental implementation of a LINQ-like list-comprehension mechanism for C++11. It uses variadic templates to allow generation of arbitrary composite function objects which can be used to perform complex sequence tranformations.

Example Usage
---

```c++
#include <iostream>
#include "stp.hpp"

using namespace stp;

struct account
{
    std::string  acct_holder;
    double       acct_balance;
    unsigned int acct_id;
};

int main()
{
    auto is_even = [](int i){return i % 2 == 0;};
    auto even_sum = make_stp( where(is_even), sum() );

    std::cout << "Sum of even numbers between 50 and 100: "
              << even_sum(range(50, 100))
              << std::endl;

    std::vector<account> accts = { {"Ben", 1550.4, 103},
                                   {"Mary", 240.0, 101},
                                   {"Eve", 1200.0, 104},
                                   {"Mark", 450.1, 102} };

    auto get_balance = [](account i){return i.acct_balance;};
    auto avg_balance = make_stp( map(get_balance), avg() );

    std::cout << "Average of all account balances: "
              << avg_balance(accts)
              << std::endl;

    auto is_above_avg = [&](account i){return i.acct_balance > avg_balance(accts);};
    auto is_eve = [](account i){return i.acct_holder == "Eve";};
    auto eve_above_avg = make_stp( where(is_above_avg), any(is_eve) );

    std::cout << "Is there an account holder named 'Eve' with above average account balance: "
              << eve_above_avg(accts)
              << std::endl;

    return 0;
}
```

The unit tests were written using Boost.Test, so Boost is required to run the tests. The library itself does not use Boost.
The scons setup will compile the unit tests with either g++ or MinGW. The library has not been tested with other compilers, but any decently compliant C++11 compiler should work.

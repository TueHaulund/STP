Sequence Transformation Pipeline
===

stplib is an experimental implementation of a LINQ-like list-comprehension mechanism for C++11. It uses variadic templates to allow generation of arbitrary composite function objects which can be used to perform complex sequence tranformations.

Example Usage
---

```c++
#include <iostream>
#include <vector>

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
              << std::boolalpha
              << eve_above_avg(accts)
              << std::endl;

    return 0;
}
```

Installation and build instructions
---

stplib is a header-only library, so just copy the files and folders in *include* to your compilers or projects include directory. A convenience header *stp.hpp* is provided, which includes the entire library.

A test suite and a small example program is included, to build these simply invoke scons in the root of the repository. Currently, MSVC does not have adequate support for C++11 to succesfully build stplib, but GCC 4.8+ and Clang will work. The scons setup will attempt to compile the test suite with MinGW on Windows, and GCC on Linux. If you want to use another compiler then set the 'env' parameter accordingly when invoking scons.

The test suite uses Boost.test, but stplib itself does not depend on Boost or any external library other than the STL.

Using stplib
---

To create a new transformation, use *make_stp*. It accepts any number of function objects, called operations, and returns a composite function object which will call each operation in order when given an input sequence. For the transformation to be well-defined, each operation must return a type which can be used as a parameter for the next operation in the transformation. Any mismatches between return and parameter types in a transformation will be caught at compile-time, rather than at run-time.

Note that the type of a transformation object is rather long-winded and involved, so the use of *auto* is encouraged to ease readability and usability.

stplib includes a range of predefined operations, see [the documentation](DOCUMENTATION.md) for a detailed list.

Advanced usage
---

As stplib makes use of recursive templates it is possible, in extreme cases, to exceed the maximum template instantiation depth when compiling. For GCC/MinGW/Clang use *-ftemplate-depth=1024* to increase the maximum depth to 1024.

To create additional operations, define a new function object with operator() overloaded to accept a single argument of some type (usually a templated sequence type). If the operation requires more than one argument, pass the remaining arguments in the constructor and store them until the operation is invoked. Note that operator() must be declared const, which means it cannot alter the internal state of the functor.

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
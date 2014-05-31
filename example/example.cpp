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
    auto even_sum = make_stp( where([](int i){return i % 2 == 0;}), sum() );
    std::cout << "Sum of even numbers between 50 and 100: " << even_sum(range(50, 100)) << std::endl;

    std::vector<account> accts = { {"Ben", 1550.4, 101}, {"Mary", 240.0, 102}, {"Eve", 12000.0, 103} };

    auto accts_balance_sum = make_stp( map([](account i){return i.acct_balance;}), sum() );
    std::cout << "Sum of all account balances: " << accts_balance_sum(accts) << std::endl;

    return 0;
}
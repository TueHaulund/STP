#include <vector>
#include <string>
#include <algorithm>

#include <boost/test/minimal.hpp>

#include "stp.hpp"

int test_main(int, char *[])
{
    using namespace stp;

    std::vector<int> intvec(5, 5);
    std::vector<float> floatvec(5, 5.1f);
    std::vector<char> charvec(5, 'a');
    std::vector<bool> boolvec(5, true);
    std::vector<int> numbers({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    std::vector<std::string> stringvec({"s1 ", "s2  ", "s3   ", "s4    "});

    //Simple tovector
    BOOST_CHECK( Query(FromVector(intvec), ToVector()) == intvec );
    BOOST_CHECK( Query(FromVector(floatvec), ToVector()) == floatvec );
    BOOST_CHECK( Query(FromVector(charvec), ToVector()) == charvec );
    BOOST_CHECK( Query(FromVector(boolvec), ToVector()) == boolvec );
    BOOST_CHECK( Query(FromVector(numbers), ToVector()) == numbers );
    BOOST_CHECK( Query(FromVector(stringvec), ToVector()) == stringvec );

    //Simple sum
    BOOST_CHECK( Query(FromVector(floatvec), Sum()) == 25.5 );
    BOOST_CHECK( Query(FromVector(intvec), Sum()) == 25 );
    BOOST_CHECK( Query(FromVector(boolvec), Sum()) == 1 );
    BOOST_CHECK( Query(FromVector(stringvec), Sum()) == "s1 s2  s3   s4    " );

    //Simple take
    BOOST_CHECK( Query(FromVector(numbers), Take(5), ToVector()) == std::vector<int>({1, 2, 3, 4, 5}) );
    BOOST_CHECK( Query(FromVector(numbers), Take(6), ToVector()) == std::vector<int>({1, 2, 3, 4, 5, 6}) );
    BOOST_CHECK( Query(FromVector(numbers), Take(1000), ToVector()) == numbers );
    BOOST_CHECK( Query(FromVector(numbers), Take(0), ToVector()) == std::vector<int>({}) );

    return 0;
}

//Query(from_array({1, 2, 3}), take(2), sum());
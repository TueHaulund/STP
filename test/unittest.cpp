#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>

#include <boost/test/minimal.hpp>

#include "stp.hpp"

int test_main(int, char *[])
{
    using namespace stp;

    std::vector<int> int_vec(5, 5);
    std::vector<float> float_vec(5, 5.1f);
    std::vector<char> char_vec(5, 'a');
    std::vector<bool> bool_vec(5, true);
    std::vector<std::string> string_vec({"s1 ", "s2  ", "s3   ", "s4    "});

    std::vector<int> ordered_ints({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    std::vector<int> unordered_ints({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(unordered_ints.begin(), unordered_ints.end(), std::default_random_engine(seed));

    //-----BEGIN STL TRANSFORMATIONS-----

    //Simple tovector
    BOOST_CHECK( Transform(FromVector(int_vec), ToVector()) == int_vec );
    BOOST_CHECK( Transform(FromVector(float_vec), ToVector()) == float_vec );
    BOOST_CHECK( Transform(FromVector(char_vec), ToVector()) == char_vec );
    BOOST_CHECK( Transform(FromVector(bool_vec), ToVector()) == bool_vec );
    BOOST_CHECK( Transform(FromVector(ordered_ints), ToVector()) == ordered_ints );
    BOOST_CHECK( Transform(FromVector(string_vec), ToVector()) == string_vec );

    //Simple tolist
    BOOST_CHECK( Transform(FromVector(int_vec), ToList()) == std::list<int>(5, 5) );
    BOOST_CHECK( Transform(FromVector(float_vec), ToList()) == std::list<float>(5, 5.1f) );
    BOOST_CHECK( Transform(FromVector(char_vec), ToList()) == std::list<char>(5, 'a') );
    BOOST_CHECK( Transform(FromVector(bool_vec), ToList()) == std::list<bool>(5, true) );
    BOOST_CHECK( Transform(FromVector(ordered_ints), ToList()) == std::list<int>(ordered_ints.begin(), ordered_ints.end()) );
    BOOST_CHECK( Transform(FromVector(string_vec), ToList()) == std::list<std::string>(string_vec.begin(), string_vec.end()) );

    //TODO: COMPOSITES

    //-----END STL TRANSFORMATIONS-----

    //Simple sum
    BOOST_CHECK( Transform(FromVector(float_vec), Sum()) == 25.5 );
    BOOST_CHECK( Transform(FromVector(int_vec), Sum()) == 25 );
    BOOST_CHECK( Transform(FromVector(bool_vec), Sum()) == 1 );
    BOOST_CHECK( Transform(FromVector(string_vec), Sum()) == "s1 s2  s3   s4    " );
    BOOST_CHECK( Transform(FromVector(ordered_ints), Sum()) == 66 );
    BOOST_CHECK( Transform(FromVector(unordered_ints), Sum()) == 66 );

    //Simple take
    BOOST_CHECK( Transform(FromVector(ordered_ints), Take(5), ToVector()) == std::vector<int>({1, 2, 3, 4, 5}) );
    BOOST_CHECK( Transform(FromVector(ordered_ints), Take(6), ToVector()) == std::vector<int>({1, 2, 3, 4, 5, 6}) );
    BOOST_CHECK( Transform(FromVector(ordered_ints), Take(1000), ToVector()) == ordered_ints );
    BOOST_CHECK( Transform(FromVector(ordered_ints), Take(0), ToVector()) == std::vector<int>({}) );

    //TODO: WHERE

    //Simple count
    BOOST_CHECK( Transform(FromVector(float_vec), Count()) == 5 );
    BOOST_CHECK( Transform(FromVector(int_vec), Count()) == 5 );
    BOOST_CHECK( Transform(FromVector(bool_vec), Count()) == 5 );
    BOOST_CHECK( Transform(FromVector(string_vec), Count()) == 4 );
    BOOST_CHECK( Transform(FromVector(ordered_ints), Count()) == 11 );
    BOOST_CHECK( Transform(FromVector(unordered_ints), Count()) == 11 );

    return 0;
}

//Query(from_array({1, 2, 3}), take(2), sum());
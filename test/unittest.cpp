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

    int arr[] = {1, 2, 3, 4, 5};
    std::vector<int> int_vec(5, 5);
    std::vector<float> float_vec(5, 5.1f);
    std::vector<char> char_vec(5, 'a');
    std::vector<bool> bool_vec(5, true);
    std::vector<std::string> string_vec({"s1 ", "s2  ", "s3   ", "s4    "});

    std::vector<int> ordered_ints({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    std::vector<int> unordered_ints({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(unordered_ints.begin(), unordered_ints.end(), std::default_random_engine(seed));

    //-----BEGIN SEQUENCE SOURCES-----//
    //BOOST_CHECK( Transform(FromArray(arr), ToVector()) == std::vector<int>({1, 2, 3, 4, 5}) );
    BOOST_CHECK( Transform(int_vec, ToVector()) == int_vec );
    //BOOST_CHECK( Transform(FromIterators(int_vec.begin(), int_vec.end()), ToVector()) == int_vec );

    BOOST_CHECK( Transform(Range(1, 5)) == std::vector<int>({1, 2, 3, 4}) );
    BOOST_CHECK( Transform(Range(5, 1)) == std::vector<int>({5, 4, 3, 2}) );
    BOOST_CHECK( Transform(Range(-1, 1)) == std::vector<int>({-1, 0}) );
    BOOST_CHECK( Transform(Range(1, -1)) == std::vector<int>({1, 0}) );
    BOOST_CHECK( Transform(Range(1, 1)) == std::vector<int>() );
    BOOST_CHECK( Transform(Range(-1, -1)) == std::vector<int>() );
    BOOST_CHECK( Transform(Range(0, 0)) == std::vector<int>() );
    BOOST_CHECK( Transform(Range(0, 1)) == std::vector<int>({0}) );
    BOOST_CHECK( Transform(Range(1, 0)) == std::vector<int>({1}) );

    BOOST_CHECK( Transform(Range(1, 5, 2)) == std::vector<int>({1, 3}) );
    BOOST_CHECK( Transform(Range(5, 1, 2)) == std::vector<int>({5, 3}) );
    BOOST_CHECK( Transform(Range(1, 5, 0)) == std::vector<int>() );
    BOOST_CHECK( Transform(Range(1, 5, 6)) == std::vector<int>() );
    BOOST_CHECK( Transform(Range(1, 10, 3)) == std::vector<int>({1, 4, 7}) );
    BOOST_CHECK( Transform(Range(10, 1, 3)) == std::vector<int>({10, 7, 4}) );

    BOOST_CHECK( Transform(Range(-1, -5, 2)) == std::vector<int>({-1, -3}) );
    BOOST_CHECK( Transform(Range(-5, -1, 2)) == std::vector<int>({-5, -3}) );
    BOOST_CHECK( Transform(Range(-1, -5, 0)) == std::vector<int>() );
    BOOST_CHECK( Transform(Range(-1, -5, 6)) == std::vector<int>() );
    BOOST_CHECK( Transform(Range(-1, -10, 3)) == std::vector<int>({-1, -4, -7}) );
    BOOST_CHECK( Transform(Range(-10, -1, 3)) == std::vector<int>({-10, -7, -4}) );
    //-----END SEQUENCE SOURCES-----//


    //-----BEGIN STL TRANSFORMATIONS-----//
    //Simple tovector
    BOOST_CHECK( Transform(int_vec, ToVector()) == int_vec );
    BOOST_CHECK( Transform(float_vec, ToVector()) == float_vec );
    BOOST_CHECK( Transform(char_vec, ToVector()) == char_vec );
    BOOST_CHECK( Transform(bool_vec, ToVector()) == bool_vec );
    BOOST_CHECK( Transform(ordered_ints, ToVector()) == ordered_ints );
    BOOST_CHECK( Transform(string_vec, ToVector()) == string_vec );

    //Simple tolist
    BOOST_CHECK( Transform(int_vec, ToList()) == std::list<int>(5, 5) );
    BOOST_CHECK( Transform(float_vec, ToList()) == std::list<float>(5, 5.1f) );
    BOOST_CHECK( Transform(char_vec, ToList()) == std::list<char>(5, 'a') );
    BOOST_CHECK( Transform(bool_vec, ToList()) == std::list<bool>(5, true) );
    BOOST_CHECK( Transform(ordered_ints, ToList()) == std::list<int>(ordered_ints.begin(), ordered_ints.end()) );
    BOOST_CHECK( Transform(string_vec, ToList()) == std::list<std::string>(string_vec.begin(), string_vec.end()) );

    //TODO: COMPOSITES

    //-----END STL TRANSFORMATIONS-----//

    //Simple sum
    BOOST_CHECK( Transform(float_vec, Sum()) == 25.5 );
    BOOST_CHECK( Transform(int_vec, Sum()) == 25 );
    BOOST_CHECK( Transform(bool_vec, Sum()) == 1 );
    BOOST_CHECK( Transform(string_vec, Sum()) == "s1 s2  s3   s4    " );
    BOOST_CHECK( Transform(ordered_ints, Sum()) == 66 );
    BOOST_CHECK( Transform(unordered_ints, Sum()) == 66 );

    //Simple take
    BOOST_CHECK( Transform(ordered_ints, Take(5)) == std::vector<int>({1, 2, 3, 4, 5}) );
    BOOST_CHECK( Transform(ordered_ints, Take(6)) == std::vector<int>({1, 2, 3, 4, 5, 6}) );
    BOOST_CHECK( Transform(ordered_ints, Take(1000)) == ordered_ints );
    BOOST_CHECK( Transform(ordered_ints, Take(0)) == std::vector<int>({}) );

    //TODO: WHERE

    //Simple count
    BOOST_CHECK( Transform(float_vec, Count()) == 5 );
    BOOST_CHECK( Transform(int_vec, Count()) == 5 );
    BOOST_CHECK( Transform(bool_vec, Count()) == 5 );
    BOOST_CHECK( Transform(string_vec, Count()) == 4 );
    BOOST_CHECK( Transform(ordered_ints, Count()) == 11 );
    BOOST_CHECK( Transform(unordered_ints, Count()) == 11 );

    return 0;
}

//Query(from_array({1, 2, 3}), take(2), sum());